#include <assert.h>
#include <stdlib.h>

#include "avmHashtable.h"
#include "avm.h"

extern unsigned int executeFinished;
extern unsigned int pc;

extern avm_memcell_s ax, bx, retval;
extern avm_memcell_s avm_stack[];
extern unsigned int globalCounter;
extern unsigned int top;
extern unsigned int currLine;

extern unsigned int from_exe_set;

avm_table_p avm_tablenew(void) {
    avm_table_p t = (avm_table_p) malloc(sizeof (avm_table_s));

    AVM_WIPEOUT(*t);

    t->refCounter = 0;
    avm_tablebucketsinit(t->strIndexed);
    avm_tablebucketsinit(t->numIndexed);
    t->total = 0;

    return t;
}

void avm_tabledestroy(avm_table_p t) {
    avm_tablebucketsdestroy(t->strIndexed);
    avm_tablebucketsdestroy(t->numIndexed);
    t->total = 0;

    int i;
    /*
        for (i = AVM_STACKSIZE - 1; i >= AVM_STACKSIZE - globalCounter; --i) {
            if (avm_stack[i].type == table_m && avm_stack[i].data.tableVal == t) {
                avm_stack[i].type = undef_m;
                avm_stack[i].data.tableVal = NULL;
            }
        }
     */
    // otidhpote edixne ston table stamataei na deixnei
    for (i = top + 1; i < AVM_STACKSIZE; ++i) {
        if (avm_stack[i].type == table_m && avm_stack[i].data.tableVal == t) {
            avm_stack[i].type = undef_m;
            avm_stack[i].data.tableVal = NULL;
        }
    }
    if (ax.type == table_m && ax.data.tableVal == t) {
        ax.type = undef_m;
        ax.data.tableVal = NULL;
    }
    if (bx.type == table_m && bx.data.tableVal == t) {
        bx.type = undef_m;
        bx.data.tableVal = NULL;
    }
    if (retval.type == table_m && retval.data.tableVal == t) {
        retval.type = undef_m;
        retval.data.tableVal = NULL;
    }

    free(t);
}

void avm_table_print(avm_table_p t, unsigned int total_table_printed) {
    if (t->total == 0) {
        printf("[]");
        return;
    }
    printf("[ ");

    int i;
    for (i = 0; i < AVM_TABLE_HASHSIZE; ++i) {
        if (total_table_printed == t->total) {
            break;
        }
        total_table_printed = avm_table_bucket_print(t->strIndexed[i], total_table_printed);
    }

    for (i = 0; i < AVM_TABLE_HASHSIZE; ++i) {

        if (total_table_printed == t->total) {
            break;
        }
        total_table_printed = avm_table_bucket_print(t->numIndexed[i], total_table_printed);
    }

    assert(total_table_printed == t->total);

    printf(" ]");
}

unsigned int avm_table_bucket_print(avm_table_bucket_p head, unsigned int total_table_printed) {
    while (head != NULL) {
        ++total_table_printed;
        if (total_table_printed > 1) {
            printf(" , ");
        }

        printf("{ %s : ", avm_tostring(&(head->key)));

        if (head->value.type == table_m) {
            avm_table_print(head->value.data.tableVal, 0);
            printf(" }");
        } else {
            printf("%s }", avm_tostring(&(head->value)));
        }
        head = head->next;
    }

    return total_table_printed;
}

avm_table_bucket_p avm_bucket_new(avm_memcell_p key, avm_memcell_p value) {
    avm_table_bucket_p newBucket = (avm_table_bucket_p) malloc(sizeof (avm_table_bucket_s));
    assert(newBucket != NULL); // TODO remove

    memmove(&(newBucket->key), key, sizeof (avm_memcell_s));
    memmove(&(newBucket->value), value, sizeof (avm_memcell_s));

    if (key->type == string_m && key != &ax) {
        if (from_exe_set == 1) {
            assert(key != NULL && ((&(avm_stack[AVM_STACKSIZE - 1]) >= key) && (key > &(avm_stack[top]))));
        }
        assert(newBucket->key.type == string_m);
        newBucket->key.data.strVal = strdup(key->data.strVal);
    }
    if (value->type == string_m && value != &bx) {
        if (from_exe_set == 1) {
            assert(value != NULL && ((&(avm_stack[AVM_STACKSIZE - 1]) >= value) && (value > &(avm_stack[top]))));
        }
        assert(newBucket->value.type == string_m);
        newBucket->value.data.strVal = strdup(value->data.strVal);
    }

    newBucket->next = NULL;

    return newBucket;
}

void avm_bucket_check(avm_table_p table, avm_memcell_p value, avm_table_bucket_p head) {
    if (value->type == table_m) {
        assert(value->data.tableVal == head->value.data.tableVal);
        avm_tableincrefcounter(value->data.tableVal);
    }
}

unsigned int str_hashFunc(char * key, int hsize) {
    unsigned int index = 0U;
    size_t ui;
    for (ui = 0U; key[ui] != '\0'; ui++) {
        index = index * HASH_MULTIPLIER + key[ui];
    }

    return index % hsize;
}

unsigned int num_hashFunc(double key) {
    return ((unsigned int) key % AVM_TABLE_HASHSIZE);
}

avm_memcell_p avm_tablegetelem(avm_table_p table, avm_memcell_p key) {
    assert(table != NULL);
    assert(key != NULL);

    int index;
    avm_table_bucket_p head;
    switch (key->type) {
        case string_m:
        {
            index = str_hashFunc(key->data.strVal, AVM_TABLE_HASHSIZE);
            head = table->strIndexed[index];

            while (head != NULL) {
                if (strcmp(head->key.data.strVal, key->data.strVal) == 0) {
                    return &(head->value);
                }
                head = head->next;
            }

            return NULL;
        }
        case number_m:
        {
            index = num_hashFunc(key->data.numVal);
            head = table->numIndexed[index];

            while (head != NULL) {
                if (head->key.data.numVal == key->data.numVal) {
                    return &(head->value);
                }
                head = head->next;
            }

            return NULL;
        }
        default:
        {
            avm_error("%u: Error: Unsupported index type\n", currLine);
            executeFinished = 1;
            return NULL;
        }
    }

}

void avm_tablesetelem(avm_table_p table, avm_memcell_p key, avm_memcell_p value) {
    assert(table != NULL);
    assert(key != NULL);
    assert(value != NULL);

    int index;
    avm_table_bucket_p head;
    switch (key->type) {
        case string_m:
        {
            index = str_hashFunc(key->data.strVal, AVM_TABLE_HASHSIZE);
            head = table->strIndexed[index];

            if (head == NULL) {
                head = avm_bucket_new(key, value);
                avm_bucket_check(table, value, head);
                table->strIndexed[index] = head;
                table->total++;
                return;
            }
            while (head != NULL) {
                if (strcmp(head->key.data.strVal, key->data.strVal) == 0) {
                    memmove(&(head->value), value, sizeof (avm_memcell_s));
                    if (value->type == string_m) {
                        head->value.data.strVal = strdup(value->data.strVal);
                    }

                    avm_bucket_check(table, value, head);
                    return;
                }
                if (head->next == NULL) {
                    break;
                }
                head = head->next;
            }

            head->next = avm_bucket_new(key, value);
            avm_bucket_check(table, value, head->next);
            table->total++;
            break;
        }
        case number_m:
        {
            index = num_hashFunc(key->data.numVal);
            head = table->numIndexed[index];

            if (head == NULL) {
                head = avm_bucket_new(key, value);
                avm_bucket_check(table, value, head);

                table->numIndexed[index] = head;
                table->total++;
                return;
            }
            while (head != NULL) {
                if (head->key.data.numVal == key->data.numVal) {
                    memmove(&(head->value), value, sizeof (avm_memcell_s));
                    if (value->type == string_m) {
                        head->value.data.strVal = strdup(value->data.strVal);
                    }

                    avm_bucket_check(table, value, head);
                    return;
                }
                if (head->next == NULL) {
                    break;
                }
                head = head->next;
            }

            head->next = avm_bucket_new(key, value);
            avm_bucket_check(table, value, head->next);
            table->total++;
            break;
        }
        default:
        {
            avm_error("%u: Error: Unsupported index type\n", currLine);
            executeFinished = 1;
            return;
        }
    }
}

void avm_tablebucketsinit(avm_table_bucket_p* p) {
    unsigned int i;
    for (i = 0; i < AVM_TABLE_HASHSIZE; ++i) {
        p[i] = (avm_table_bucket_p) 0;
    }
}

void avm_tablebucketsdestroy(avm_table_bucket_p* p) {
    unsigned int i;
    avm_table_bucket_p head;
    for (i = 0; i < AVM_TABLE_HASHSIZE; ++i) {
        head = p[i];
        while (head != NULL) {
            avm_table_bucket_p del = head;
            head = head->next;

            avm_memcellclear(&(del->key));
            avm_memcellclear(&(del->value));
            free(del);
        }
        p[i] = (avm_table_bucket_p) 0;
    }
}

avm_libfuncs_bucket_p new_libfuncs_bucket(char * id, library_func_t addr) {
    avm_libfuncs_bucket_p newBucket = malloc(sizeof (avm_libfuncs_bucket_s));
    assert(newBucket != NULL); // TODO remove

    newBucket->key = strdup(id);
    newBucket->value = addr;
    newBucket->next = NULL;

    return newBucket;
}
