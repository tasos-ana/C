#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "myFunctions.h"

/*---------------------------------------------------------------------------------------------*/
void insert_assignment(int cid, int pid, int did) {
    p_district here = find_proper_district(did);
    p_assign newAssign = make_assignment(cid, pid);
    if (here->assignL != NULL) {
        newAssign->next = here->assignL;
    }
    here->assignL = newAssign;
}

p_assign make_assignment(int cid, int pid) {
    p_assign newAssign = malloc(sizeof (struct present_assign));
    newAssign->cid = cid;
    newAssign->pid = pid;
    newAssign->s_degree = -2;
    newAssign->next = NULL;
    return newAssign;
}

void print_assignments(p_assign here) {
    while (here != NULL) {
        printf("<%d, %d> ", here->cid, here->pid);
        here = here->next;
    }
    printf("\n");
}

p_assign find_assignment(p_assign start, int cid) {
    p_assign here = start;
    while (here != NULL) {
        if (here->cid == cid) {
            return here;
        }
        here = here->next;
    }
    return NULL;
}

void set_degree(p_assign here, int s_degree) {
    if (here->pid == -2) {
        here->s_degree = 1;
        return;
    }
    if (s_degree >= 1 && s_degree <= 5) {
        here->s_degree = s_degree;
    } else {
        printf("s_degree must be between 1 and 5\n");
    }
}