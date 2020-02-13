#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <errno.h>

#include "avmLibfuncs.h"
#include "avmFuncs.h"
#include "avm.h"
#include "avmHashtable.h"

#define PI 3.14159265

extern unsigned int topsp;
extern avm_memcell_s avm_stack[];
extern unsigned int executeFinished;
extern unsigned int currLine;
extern unsigned int globalCounter;

extern avm_memcell_s retval;

extern char *typeStrings[];

unsigned int avm_totalactuals(void) {
    return avm_get_envvalue(topsp + AVM_NUMACTUALS_OFFSET);
}

avm_memcell_p avm_getactual(unsigned int i) {
    assert(i < avm_totalactuals());
    return &avm_stack[topsp + AVM_STACKENV_SIZE + 1 + i];
}

void avm_init_libfuncs_hashtable() {
    libfuncs_hashtable = malloc(AVM_LIBFUNCS_HASHSIZE * sizeof (avm_libfuncs_bucket_p));

    int i;
    for (i = 0; i < AVM_LIBFUNCS_HASHSIZE; ++i) {
        libfuncs_hashtable[i] = NULL;
    }
}

library_func_t avm_getlibraryfunc(char * id) {
    int index;
    index = str_hashFunc(id, AVM_LIBFUNCS_HASHSIZE);

    assert(libfuncs_hashtable[index] != NULL);

    avm_libfuncs_bucket_p curr = libfuncs_hashtable[index];
    while (curr != NULL) {
        if (strcmp(curr->key, id) == 0) {
            return curr->value;
        }
        curr = curr->next;
    }
}

void avm_registerlibfunc(char* id, library_func_t addr) {
    int index;
    index = str_hashFunc(id, AVM_LIBFUNCS_HASHSIZE);

    avm_libfuncs_bucket_p newBucket = new_libfuncs_bucket(id, addr);

    if (libfuncs_hashtable[index] == NULL) {
        libfuncs_hashtable[index] = newBucket;

    } else {
        avm_libfuncs_bucket_p curr = libfuncs_hashtable[index];
        while (curr->next != NULL) {
            curr = curr->next;
        }
        curr->next = newBucket;
    }
}

/*
    Implementantion of the library functions
 */

void libfunc_print(void) {
    unsigned int n = avm_totalactuals();
    unsigned int i;
    for (i = 0; i < n; ++i) {
        if (avm_getactual(i)->type == table_m) {
            avm_table_print(avm_getactual(i)->data.tableVal, 0);
        } else {
            char* s = avm_tostring(avm_getactual(i));
            if (executeFinished == 1) {
                assert(s == NULL);
                avm_error("%u: Error: Illegal argument of function call.\n", currLine);
                return;
            }
            printf("%s", s);
            free(s);
        }
    }
}

void libfunc_input(void) {
    char str_input[512];

    avm_memcellclear(&retval);
    if (fgets(str_input, 512, stdin) != NULL) {

        //actions to read something from console
        int length = strcspn(str_input, "\n");
        str_input[length] = '\0';

        //actions to convert to number
        double num;
        char str[512];
        str[0] = '\0';

        int ret = sscanf(str_input, "%lf%s", &num, str);
        int len = strlen(str);

        if (ret > 0 && len == 0) { //check if number
            retval.type = number_m;
            retval.data.numVal = num;
        } else if (strcmp(str_input, "true") == 0) { //check if bool:true
            retval.type = bool_m;
            retval.data.boolVal = 1;
        } else if (strcmp(str_input, "false") == 0) { //check if bool:false
            retval.type = bool_m;
            retval.data.boolVal = 0;
        } else if (strcmp(str_input, "nil") == 0) { //check if nil
            retval.type = nil_m;
        } else {
            retval.type = string_m;
            retval.data.strVal = strdup(str_input);
        }
        printf("%s\n", typeStrings[retval.type]);
    } else {
        avm_error("%u: Error: (memory dump) 'input' can't read from console!\n", currLine);
        executeFinished = 1;
        retval.type = undef_m;
    }
}

void libfunc_objectmemberkeys(void) {
    unsigned int n = avm_totalactuals();


    avm_memcellclear(&retval);

    if (n != 1) {
        avm_error("%u: Error: one argument (not %u) expected in 'objectmemberkeys'!\n", currLine, n);
    } else if (avm_getactual(0)->type != table_m) {
        avm_error("%u: Error: expected table in 'objectmemberkeys' but argument's type is '%s' !\n", currLine, typeStrings[avm_getactual(0)->type]);
    } else {
        avm_table_p newTable = avm_tablenew();
        avm_tableincrefcounter(newTable);

        avm_table_p currTable = avm_getactual(0)->data.tableVal;

        int i, j = 0;

        avm_table_bucket_p head;
        avm_memcell_s key;

        key.type = number_m;

        for (i = 0; i < AVM_TABLE_HASHSIZE; ++i) {
            if (j == currTable->total) {
                break;
            }

            head = currTable->numIndexed[i];

            while (head != NULL) {
                key.data.numVal = j;
                avm_tablesetelem(newTable, &key, &(head->key));
                ++j;
                head = head->next;
            }
        }

        for (i = 0; i < AVM_TABLE_HASHSIZE; ++i) {
            if (j == currTable->total) {
                break;
            }

            head = currTable->strIndexed[i];

            while (head != NULL) {
                key.data.numVal = j;
                avm_tablesetelem(newTable, &key, &(head->key));
                ++j;
                head = head->next;
            }
        }
        assert(j == currTable->total);

        retval.type = table_m;
        retval.data.tableVal = newTable;
    }
}

void libfunc_objecttotalmembers(void) {

    unsigned int n = avm_totalactuals();
    avm_memcellclear(&retval);
    if (n != 1) {
        avm_error("%u: Error: one argument (not %u) expected in 'objecttotalmembers'!\n", currLine, n);
    } else if (avm_getactual(0)->type != table_m) {
        avm_error("%u: Error: expected table in 'objecttotalmembers' but argument's type is '%s' !\n", currLine, typeStrings[avm_getactual(0)->type]);
    } else {
        retval.type = number_m;
        retval.data.numVal = avm_getactual(0)->data.tableVal->total;
        return;
    }
    retval.type = nil_m;
    executeFinished = 1;
}

void libfunc_objectcopy(void) {
    unsigned int n = avm_totalactuals();
    avm_memcellclear(&retval);
    if (n != 1) {
        avm_error("%u: Error: one argument (not %u) expected in 'objectcopy'!\n", currLine, n);
    } else if (avm_getactual(0)->type != table_m) {
        avm_error("%u: Error: expected table in 'objectcopy' but argument's type is '%s' !\n", currLine, typeStrings[avm_getactual(0)->type]);
    } else {
        avm_table_p newTable = avm_tablenew();
        avm_table_p currTable = avm_getactual(0)->data.tableVal;

        avm_tableincrefcounter(newTable);

        int i;
        avm_table_bucket_p numhead = NULL;
        avm_table_bucket_p strhead = NULL;

        for (i = 0; i < AVM_TABLE_HASHSIZE; ++i) {

            numhead = currTable->numIndexed[i];
            while (numhead != NULL) {
                avm_tablesetelem(newTable, &(numhead->key), &(numhead->value));
                numhead = numhead->next;
            }

            strhead = currTable->strIndexed[i];
            while (strhead != NULL) {
                avm_tablesetelem(newTable, &(strhead->key), &(strhead->value));
                strhead = strhead->next;
            }
        }

        assert(newTable->total == currTable->total);

        retval.type = table_m;
        retval.data.tableVal = newTable;

        return;
    }
    retval.type = nil_m;
    executeFinished = 1;
}

void libfunc_typeof(void) {
    unsigned int n = avm_totalactuals();
    avm_memcellclear(&retval);
    if (n != 1) {
        avm_error("%u: Error: one argument (not %u) expected in 'typeof'!\n", currLine, n);
    } else {
        retval.type = string_m;
        retval.data.strVal = strdup(typeStrings[avm_getactual(0)->type]);
        return;
    }
    retval.type = nil_m;
    executeFinished = 1;
}

void libfunc_strtonum(void) {
    unsigned int n = avm_totalactuals();
    avm_memcellclear(&retval);
    if (n != 1) {
        avm_error("%u: Error: one argument (not %u) expected in 'strtonum'!\n", currLine, n);
    } else if (avm_getactual(0)->type != string_m) {
        avm_error("%u: Error: expected string in 'strtonum' but argument's type is '%s' !\n", currLine, typeStrings[avm_getactual(0)->type]);
    } else {
        double num;
        char str[512];
        str[0] = '\0';
        int ret = sscanf(avm_getactual(0)->data.strVal, "%lf%s", &num, str);
        int len = strlen(str);

        if (ret > 0 && len == 0) {
            retval.type = number_m;
            retval.data.numVal = num;
        } else {
            retval.type = nil_m;
        }
        return;
    }
    retval.type = nil_m;
    executeFinished = 1;
}

void libfunc_totalarguments(void) {
    unsigned int p_topsp = avm_get_envvalue(topsp + AVM_SAVEDTOPSP_OFFSET);
    avm_memcellclear(&retval);

    if (p_topsp == 0) {
        avm_error("%u: Error: 'totalarguments' called outside a function!\n", currLine);
    } else {
        retval.type = number_m;
        retval.data.numVal = avm_get_envvalue(p_topsp + AVM_NUMACTUALS_OFFSET);
        return;
    }
    retval.type = nil_m;
    executeFinished = 1;
}

void libfunc_argument(void) {
    unsigned int p_topsp = avm_get_envvalue(topsp + AVM_SAVEDTOPSP_OFFSET);
    avm_memcellclear(&retval);

    if (p_topsp == 0) {
        avm_error("%u: Error: 'argument' called outside a function!\n", currLine);
    } else {
        unsigned int n = avm_totalactuals();
        unsigned int func_actuals = avm_get_envvalue(p_topsp + AVM_NUMACTUALS_OFFSET);

        if (n != 1) {
            avm_error("%u: Error: one argument (not %u) expected in 'argument'!\n", currLine, n);
        } else if (avm_getactual(0)->type != number_m) {
            avm_error("%u: Error: expected number in 'argument' but argument's type is '%s'!\n", currLine, typeStrings[avm_getactual(0)->type]);
        } else if (func_actuals <= avm_getactual(0)->data.numVal) {
            avm_error("%u: Error: function has less than %.3lf actual arguments!\n", currLine, avm_getactual(0)->data.numVal + 1);
        } else if (avm_getactual(0)->data.numVal < 0) {
            avm_error("%u: Error: argument in 'argument' must be positive!\n", currLine);
        } else {
            unsigned int actual_index = p_topsp + AVM_STACKENV_SIZE + avm_getactual(0)->data.numVal + 1;
            assert(actual_index <= AVM_STACKSIZE - 1 - globalCounter);

            memmove(&retval, &avm_stack[actual_index], sizeof (avm_memcell_s));
            if (retval.type == string_m) {
                retval.data.strVal = strdup(avm_stack[actual_index].data.strVal);
            }
            return;
        }
    }
    retval.type = nil_m;
    executeFinished = 1;
}

void libfunc_sqrt(void) {
    unsigned int n = avm_totalactuals();

    avm_memcellclear(&retval);

    if (n != 1) {
        avm_error("%u: Error: one argument (not %u) expected in 'sqrt'!\n", currLine, n);
    } else if (avm_getactual(0)->type != number_m) {
        avm_error("%u: Error: expected number in 'sqrt' but argument's type is '%s'!\n", currLine, typeStrings[avm_getactual(0)->type]);
    } else {
        retval.type = number_m;

        double temp;
        temp = sqrt(avm_getactual(0)->data.numVal);
        if (errno == EDOM) {
            retval.type = nil_m;
            errno = 0;
        } else {
            retval.data.numVal = temp;
        }
        return;
    }
    retval.type = nil_m;
    executeFinished = 1;
}

void libfunc_cos(void) {
    unsigned int n = avm_totalactuals();

    avm_memcellclear(&retval);

    if (n != 1) {
        avm_error("%u: Error: one argument (not %u) expected in 'cos'!\n", currLine, n);
    } else if (avm_getactual(0)->type != number_m) {
        avm_error("%u: Error: expected number in 'cos' but argument's type is '%s'!\n", currLine, typeStrings[avm_getactual(0)->type]);
    } else {
        double angle_in_degrees = avm_getactual(0)->data.numVal;
        double one_degree_in_rads = PI / 180;
        double angle_in_rads = angle_in_degrees * one_degree_in_rads;

        retval.type = number_m;
        retval.data.numVal = cos(angle_in_rads);
        return;
    }
    retval.type = nil_m;
    executeFinished = 1;
}

void libfunc_sin(void) {
    unsigned int n = avm_totalactuals();

    avm_memcellclear(&retval);

    if (n != 1) {
        avm_error("%u: Error: one argument (not %u) expected in 'sin'!\n", currLine, n);
    } else if (avm_getactual(0)->type != number_m) {
        avm_error("%u: Error: expected number in 'sin' but argument's type is '%s'!\n", currLine, typeStrings[avm_getactual(0)->type]);
    } else {
        double angle_in_degrees = avm_getactual(0)->data.numVal;
        double one_degree_in_rads = PI / 180;
        double angle_in_rads = angle_in_degrees * one_degree_in_rads;

        retval.type = number_m;
        retval.data.numVal = sin(angle_in_rads);
        return;
    }
    retval.type = nil_m;
    executeFinished = 1;
}
