#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "myFunctions.h"

/*---------------------------------------------------------------------------------------------*/
p_present stock_list = NULL;
p_present stock_sentinel = NULL;

void init_sentinel() {
    stock_sentinel = malloc(sizeof (struct present));
    assert(stock_sentinel != NULL);
    stock_sentinel->pid = -1;
    stock_sentinel->request_cnt = -1;
    stock_sentinel->stock_cnt = -1;
    stock_sentinel->next = NULL;
    stock_sentinel->prev = NULL;
}

void init_presents() {
    p_present present = make_present(110, 10);
    insert_present(present);
    assert(stock_sentinel->next == stock_list);
    assert(stock_sentinel->prev == stock_list);
    assert(stock_list->prev == stock_sentinel);

    present = make_present(111, 10);
    insert_present(present);
    assert(stock_sentinel->next == stock_list);
    assert(stock_list->prev == stock_sentinel);

    present = make_present(112, 10);
    insert_present(present);
    assert(stock_sentinel->next == stock_list);
    assert(stock_list->prev == stock_sentinel);

    present = make_present(113, 10);
    insert_present(present);
    assert(stock_sentinel->next == stock_list);
    assert(stock_list->prev == stock_sentinel);

    present = make_present(114, 10);
    insert_present(present);
    assert(stock_sentinel->next == stock_list);
    assert(stock_list->prev == stock_sentinel);

    present = make_present(115, 10);
    insert_present(present);
    assert(stock_sentinel->next == stock_list);
    assert(stock_list->prev == stock_sentinel);

    present = make_present(116, 10);
    insert_present(present);
    assert(stock_sentinel->next == stock_list);
    assert(stock_list->prev == stock_sentinel);
}

int print_B_output(int pid, int stock_cnt) {
    printf("B <%d> <%d>\n\t", pid, stock_cnt);
    printf("presents = ");
    print_presents();
    printf("DONE\n");

}

int buy_present(int pid, int stock_cnt) {
    p_present newPresent = make_present(pid, stock_cnt);
    assert(newPresent->next == NULL && newPresent->prev == NULL);
    insert_present(newPresent);
    print_B_output(pid, stock_cnt);
}

p_present make_present(int pid, int stock_cnt) {
    p_present newPresent;
    newPresent = malloc(sizeof (struct present));
    assert(newPresent != NULL);
    newPresent->pid = pid;
    newPresent->stock_cnt = stock_cnt;
    newPresent->request_cnt = 0;
    newPresent->next = NULL;
    newPresent->prev = NULL;
    ;
    return newPresent;
}

void insert_present(p_present newPresent) {
    if (newPresent != NULL) {
        if (stock_list != NULL) {
            stock_sentinel->prev->next = newPresent;
            newPresent->prev = stock_sentinel->prev;
        } else {
            stock_sentinel->next = newPresent;
            newPresent->prev = stock_sentinel;
            stock_list = newPresent;
        }
        newPresent->next = stock_sentinel;
        stock_sentinel->prev = newPresent;
    } else {
        printf("Can't insert a NULL present\n");
    }
}

void swap(p_present this, p_present this_prev) {
    this_prev->next = this->next;
    (this->next)->prev = this_prev;
    this->next = this_prev;
    this->prev = this_prev->prev;
    (this_prev->prev)->next = this;
    this_prev->prev = this;
    if (stock_list == this->next) {
        stock_list = this;
    }
}

void finder(p_present here) {
    int i = 0;
    while (i < 5) {
        if (here != stock_sentinel->next) {
            swap(here, here->prev);
        } else {
            assert(here->prev == stock_sentinel);
            stock_list = here;
            break;
        }
        i++;
    }
}

int print_S_output(p_present here, int pid) {
    if (here == stock_sentinel) {
        printf("S <%d>\n\t<%d>,<%d>\nDONE\n", pid, 0, 0);
        return 0;
    } else {
        printf("S <%d>\n\t<%d>,<%d>\nDONE\n", pid, here->stock_cnt, here->request_cnt);
        return 1;
    }
}

int search_present(int pid) {
    p_present here = stock_list;
    while (here != stock_sentinel) {
        if (here->pid == pid) {
            break;
        }
        here = here->next;
    }
    return print_S_output(here, pid);
}

void print_presents() {
    p_present here = stock_list;
    while (here != stock_sentinel) {
        printf("<%d> ", here->pid);
        here = here->next;
    }
    printf("\n");
}

void search_preferences(int pid1, int pid2, int pid3) {
    p_present pid1_position = NULL, pid2_position = NULL, pid3_position = NULL;
    p_present here = stock_list, tmp;
    while (here != stock_sentinel) {
        if (here->pid == pid1) {
            pid1_position = here;
            here->request_cnt++;

            tmp = here->next;
            finder(here);
            here = tmp->prev;
        } else if (here->pid == pid2) {
            pid2_position = here;
            here->request_cnt++;

            tmp = here->next;
            finder(here);
            here = tmp->prev;
        } else if (here->pid == pid3) {
            pid3_position = here;
            here->request_cnt++;

            tmp = here->next;
            finder(here);
            here = tmp->prev;
        }
        if (pid1_position != NULL && pid2_position != NULL && pid3_position != NULL) {
            break;
        }
        here = here->next;
    }
    p_present newPresent;
    if (pid1_position == NULL) {
        newPresent = make_present(pid1, 10);
        newPresent->request_cnt = 1;
        insert_present(newPresent);
    }
    if (pid2_position == NULL) {
        newPresent = make_present(pid2, 10);
        newPresent->request_cnt = 1;
        insert_present(newPresent);
    }
    if (pid3_position == NULL) {
        newPresent = make_present(pid3, 10);
        newPresent->request_cnt = 1;
        insert_present(newPresent);
    }
}

int find_best_pid(int *choice) {
    p_present here = stock_list;
    p_present pid2_position = NULL, pid3_position = NULL;
    while (here != stock_sentinel) {
        if (here->pid == choice[0]) {
            if (here->stock_cnt > 0) {
                here->stock_cnt--;
                return here->pid;
            }
        } else if (here->pid == choice[1]) {
            if (here->stock_cnt > 0) {
                pid2_position = here;
            }
        } else if (here->pid == choice[2]) {
            if (here->stock_cnt > 0) {
                pid3_position = here;
            }
        }
        here = here->next;
    }
    if (pid2_position != NULL) {
        pid2_position->stock_cnt--;
        return pid2_position->pid;
    } else if (pid3_position != NULL) {
        pid3_position->stock_cnt--;
        return pid3_position->pid;
    } else {
        return -2;
    }
}

int print_stock(void) {
    printf("T\nstock = ");
    print_presents();
    printf("DONE\n");
}

int analytics(void) {
    insertionSort_stock_list();
    printf("A\n");
    print_A_output();
    printf("DONE\n");
}

void insertionSort_stock_list() {
    int key;
    p_present i;
    p_present j = (stock_list)->next;

    while (j != stock_sentinel) {
        key = j->request_cnt;
        i = j->prev;
        while (i != stock_sentinel && (i->request_cnt) < key) {

            p_present tmp = i->next;
            (tmp->next)->prev = tmp->prev;
            i->next = tmp->next;
            (i->prev)->next = tmp;
            tmp->prev = i->prev;
            i->prev = tmp;
            tmp->next = i;
            i = tmp;

            i = i->prev;
        }
        j = j->next;
    }
    stock_list = stock_sentinel->next;
}

void print_A_output() {
    p_present here = stock_list;
    while (here != stock_sentinel) {
        printf("\t<%d> : <%d>\n", here->pid, here->request_cnt);
        here = here->next;
    }
}

void manage_stock(){
    p_present here = stock_list, tmp;
    while(here!=NULL){
        if(here->stock_cnt == 0){
            if(here == stock_list){
                stock_list = here->next;
            }
            tmp = here->next;
            delete_stock(here);
            here = tmp;
        }else{
            here->request_cnt = 0;
            here = here->next;
        }
    }
}

void delete_stock(p_present here){
    here->next = NULL;
    free(here);
}
/*---------------------------------------------------------------------------------------------*/