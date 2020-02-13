#include "santa_claus.h"

typedef struct child *p_child;
typedef struct present *p_present;
typedef struct present_assign *p_assign;
typedef struct district *p_district;

extern void init_sentinel();

extern void init_presents();
extern void init_child();

/* Multi List Functions */
extern int letter_received(int cid, int age, int did, int pid1, int pid2, int pid3); /* Event L */
extern int prepare_presents(void); /* Event P */
extern void assign_present(p_child thisChild);

extern int give_feedback(int cid, int s_degree); /* Event F */
extern int new_season(void); /* Event N ??? */

/* Double Linked List Functions */
extern int buy_present(int pid, int stock_cnt); /* Event B */
extern int search_present(int pid); /* Event S */
extern int print_stock(void); /* Event T */
extern int analytics(void); /* Event A */

extern p_present make_present(int pid, int stock_cnt);
extern void insert_present(p_present newPresent);
extern void swap(p_present this, p_present this_prev);

extern void finder(p_present here);
extern void print_presents();
extern int print_S_output(p_present here, int pid);

extern int print_B_output(int pid, int stock_cnt);
extern void search_preferences(int pid1, int pid2, int pid3);
extern int find_best_pid(int *choices);

extern void insertionSort_stock_list();
extern void print_A_output();
extern void manage_stock();

/* Sorted Single Linked List Functions */

/*        Child Functions              */
int print_child(void); /* Event H */

extern p_child make_child(int cid, int age, int did, int pid1, int pid2, int pid3);
extern void insert_child(int cid, int age, int did, int pid1, int pid2, int pid3);
extern int check_ageCategory(int age);

extern int print_ageCategory(p_child thisChild, int i);
extern void assign_present_to_every_child();
extern void manage_children();

extern p_child check_age(p_child prev_child, p_child thisChild, p_child last_moved);
extern void delete_child(p_child thisChild, p_child prev_child);
extern p_child move_child(p_child prev_child, p_child thisChild, p_child last_moved);

/*        District Functions           */
extern p_district district_list; /*pointer to the beginning of district list*/

extern int print_district(void); /* Event I */

extern p_district make_district(int did);
extern void insert_district(int did);
extern void print_districts();

extern void search_district(int did);
extern p_district find_proper_district(int did);
extern void print_I_output();

extern void manage_districts();
extern void delete_assignments(p_assign here);
extern void delete_stock(p_present here);

/* Single Linked List Functions */
extern void insert_assignment(int cid, int pid, int did);
extern p_assign make_assignment(int cid, int pid);
extern void print_assignments(p_assign here);

extern p_assign find_assignment(p_assign start, int cid);
extern void set_degree(p_assign here, int s_degree);