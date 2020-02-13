#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "myFunctions.h"

void init_child() {
    insert_child(101, 1, 10, 111, 222, 333);
    insert_child(103, 12, 10, 111, 222, 333);
    insert_child(102, 5, 10, 111, 222, 333);
    insert_child(107, 10, 10, 111, 222, 333);
    insert_child(105, 1, 10, 111, 222, 333);
    insert_child(106, 3, 10, 111, 222, 333);
    insert_child(104, 3, 10, 111, 222, 333);
    insert_child(100, 10, 10, 111, 222, 333);
    insert_child(109, 10, 10, 111, 222, 333);
}
/*---------------------------------------------------------------------------------------------*/
/* Child operations */
/*---------------------------------------------------------------------------------------------*/
struct child *Age_categories[N];

int print_child() {
    int i, result;
    printf("H\n");
    for (i = 0; i < 4; i++) {
        result = print_ageCategory(Age_categories[i], i);
        if (result == 0) {
            return 0;
        }
    }
    printf("DONE\n");
    return 1;
}

int print_ageCategory(p_child thisChild, int i) {
    if (i == 0) {
        printf("\t0-3:\t");
    } else if (i == 1) {
        printf("\t4-7:\t");
    } else if (i == 2) {
        printf("\t8-11:\t");
    } else if (i == 3) {
        printf("\t12-15:\t");
    } else {
        printf("Category is wrong\n");
        return 0;
    }

    while (thisChild != NULL) {
        printf("<%d> ", thisChild->cid);
        thisChild = thisChild->next;
    }
    printf("\n");
    return 1;
}

p_child make_child(int cid, int age, int did, int pid1, int pid2, int pid3) {
    p_child newChild = malloc(sizeof (struct child));
    if (newChild == NULL) {
        printf("Can't allocate memory for the child\n");
        exit(1);
    }
    newChild->cid = cid;
    newChild->did = did;
    newChild->age = age;
    newChild->present_choices[0] = pid1;
    newChild->present_choices[1] = pid2;
    newChild->present_choices[2] = pid3;

    newChild->next = NULL;
    return newChild;
}

int check_ageCategory(int age) {
    assert(age >= 0);
    if (age < 4) {
        return 0;
    } else if (age < 8) {
        return 1;
    } else if (age < 12) {
        return 2;
    } else if (age < 16) {
        return 3;
    } else {
        printf("You can't take a present, you are too old\n");
        return -1;
    }
}

/* upothetoume oti den tha dwthoun 2 paidia me idio cid kai diaforetikh hlikia,
 * gia auto kai DEN ginetai antistoixos elegxos */
void insert_child(int cid, int age, int did, int pid1, int pid2, int pid3) {
    int i = check_ageCategory(age);
    p_child this = Age_categories[i];
    p_child this_prev = NULL;

    while (this != NULL && (this->cid < cid)) {
        this_prev = this;
        this = this->next;
    }
    if (this != NULL && (this->cid == cid)) {
        return;
    }

    p_child newChild = make_child(cid, age, did, pid1, pid2, pid3);
    newChild->next = this;

    if (this_prev == NULL) {
        Age_categories[i] = newChild;
    } else {
        this_prev->next = newChild;
    }
}

void assign_present_to_every_child() {
    int i;
    p_child here;
    for (i = 0; i < 4; i++) {
        here = Age_categories[i];
        while (here != NULL) {
            search_district(here->did);
            assign_present(here);
            here = here->next;
        }
    }
}

void manage_children() {
    int i;
    p_child thisChild, prev_child, last_moved = NULL;
    for (i = 3; i >= 0; i--) {
        prev_child = Age_categories[i];
        thisChild = prev_child;
        if (i < 3) {
            last_moved = Age_categories[i + 1];
            printf("i + 1 = %d\n", i + 1);
        }
        while (thisChild != NULL) {
            last_moved = check_age(prev_child, thisChild, last_moved);
            if (Age_categories[i] == NULL) {
                break;
            }
            if (prev_child->next == thisChild) {
                prev_child = thisChild;
                thisChild = thisChild->next;
            } else {
                thisChild = prev_child->next;
            }

        }
    }
}

p_child check_age(p_child prev_child, p_child thisChild, p_child last_moved) {
    thisChild->age++;
    if (thisChild->age == 16) {
        delete_child(prev_child, thisChild);
    } else {
        thisChild->present_choices[0] = 0;
        thisChild->present_choices[1] = 0;
        thisChild->present_choices[2] = 0;
        if (thisChild->age == 12) {
            last_moved = move_child(prev_child, thisChild, last_moved);
        } else if (thisChild->age == 8) {
            last_moved = move_child(prev_child, thisChild, last_moved);
        } else if (thisChild->age == 4) {
            last_moved = move_child(prev_child, thisChild, last_moved);
        }
    }

    return last_moved;
}

/* upothetoume oti den uparxoun paidia me idio cid se diaforetikes  *
 * hlikiakes kathgories                                             */
p_child move_child(p_child prev_child, p_child thisChild, p_child last_moved) {
    p_child this = last_moved;
    p_child tmp = last_moved;
    int i;

    last_moved = NULL;
    while (this != NULL && (this->cid < thisChild->cid)) {
        last_moved = this;
        this = this->next;
    }
    for (i = 1; i < 4; i++) {
        if (thisChild == Age_categories[i]) {
            Age_categories[i] = thisChild->next;
            break;
        }
    }
    prev_child->next = thisChild->next;

    thisChild->next = this;
    if (last_moved != NULL) {
        last_moved->next = thisChild;
    } else {
        for (i = 3; i >= 1; i--) {
            if (tmp == Age_categories[i]) {
                Age_categories[i] = thisChild;
                break;
            }
        }
    }
    last_moved = thisChild;

    return last_moved;
}

void delete_child(p_child thisChild, p_child prev_child) {
    if (thisChild != prev_child) {
        prev_child->next = thisChild->next;
    } else {/* is the first node */
        int i;
        for (i = 0; i < 4; i++) {
            if (thisChild == Age_categories[i]) {
                Age_categories[i] = thisChild->next;
                break;
            }
        }
        prev_child = thisChild->next;
    }
    thisChild->next = NULL;
    free(thisChild);
    thisChild = prev_child;
}
/*---------------------------------------------------------------------------------------------*/
/* District operations */
/*---------------------------------------------------------------------------------------------*/
p_district district_list = NULL;

p_district make_district(int did) {
    p_district newDistrict = malloc(sizeof (struct district));
    if (newDistrict == NULL) {
        printf("Can't allocate memory for the child\n");
        exit(1);
    }
    newDistrict->assignL = NULL;
    newDistrict->did = did;
    newDistrict->next = NULL;
    return newDistrict;
}

void insert_district(int did) {
    p_district this = district_list;
    p_district this_prev = NULL;

    while (this != NULL && (this->did < did)) {
        this_prev = this;
        this = this->next;
    }
    if (this != NULL && (this->did == did)) {
        return;
    }

    p_district newDistrict = make_district(did);
    newDistrict->next = this;

    if (this_prev == NULL) {
        district_list = newDistrict;
    } else {
        this_prev->next = newDistrict;
    }
}

void search_district(int did) {
    p_district here = district_list;
    while (here != NULL) {
        if (here->did == did) {
            break;
        }
        here = here->next;
    }
    if (here == NULL) {
        insert_district(did);
    }
}

p_district find_proper_district(int did) {
    p_district here = district_list;
    while (here != NULL) {
        if (here->did == did) {
            return here;
        }
        here = here->next;
    }
}

void print_I_output() {
    p_district here = district_list;
    while (here != NULL) {
        printf("\t<%d>\n", here->did);
        printf("\t\tASSIGNMENT: ");
        print_assignments(here->assignL);
        here = here->next;
    }
}

int print_district(void) {
    printf("I\n\tDISTRICTS:\n");
    print_I_output();
    printf("DONE\n");
}

void manage_districts() {
    p_district here = district_list;
    while (here != NULL) {
        delete_assignments(here->assignL);
        here->assignL = NULL;
        here = here->next;
    }
}

void delete_assignments(p_assign here) {
    p_assign tmp;
    while (here != NULL) {
        tmp = here->next;
        here->next = NULL;
        free(here);
        here = tmp;
    }
}