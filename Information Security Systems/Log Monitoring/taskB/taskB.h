/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   taskB.h
 * Author: Tasos198
 *
 * Created on April 23, 2017, 1:37 PM
 */
#ifndef TASKB_H
#define TASKB_H

#include <stdio.h>
#include <sys/types.h>


void operation1(char* logfile);

void operation2(char* logfile, char* otherfile);

void operation3(char* logfile);

void print_md5_sum(unsigned char* md);

int compareHashes(unsigned char* md1, unsigned char* md2);

typedef struct node {
    uid_t user;
    int cnt;
    int m;
    struct node * next;
} node_t;

node_t* push(node_t * head, uid_t user, int modified);

void print_list(node_t * head, int mode);

#endif /* TASKB_H */

