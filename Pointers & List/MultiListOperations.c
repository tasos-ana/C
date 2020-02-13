#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "myFunctions.h"

int letter_received(int cid, int age, int did, int pid1, int pid2, int pid3) {
    insert_child(cid, age, did, pid1, pid2, pid3);
    search_preferences(pid1, pid2, pid3);
    printf("L <%d> <%d> <%d> <%d> <%d> <%d>\nDONE\n", cid, age, did, pid1, pid2, pid3);
    return 1;
}

int prepare_presents(void) {
    int i, cnt = 0;
    for (i = 0; i < 4; i++) {
        if (Age_categories[i] == NULL) {
            cnt++;
        }
    }
    if(cnt == 4){
        return 0;
    }
    assign_present_to_every_child();
    printf("P\n\tDISTRICTS:\n");
    print_I_output();
    printf("DONE\n");
    return 1;
}

void assign_present(p_child thisChild) {
    int pid = find_best_pid(thisChild->present_choices);
    insert_assignment(thisChild->cid, pid, thisChild->did);
}

int give_feedback(int cid, int s_degree) {
    p_district here = district_list;
    p_assign this;
    while (here != NULL) {
        this = find_assignment(here->assignL, cid);
        if (this != NULL) {
            set_degree(this, s_degree);
            printf("F <%d> <%d> <%d>\nDONE\n", this->cid, this->s_degree, this->pid);
            return 1;
        }
        here = here->next;
    }
    return 0;
}

int new_season(void) {
    manage_children(); /* xalia */
    manage_districts(); /* koble */
    manage_stock(); /* xalia */
    printf("N\nDONE\n");
}