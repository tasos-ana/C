#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <openssl/md5.h>

#include "taskB.h"

node_t * push(node_t * head, uid_t user, int modified) {
    node_t * current = head;
    if (current == NULL) {
        current = malloc(sizeof (node_t));
        current->user = user;
        current->cnt = 1;
        current->m = modified;
        current->next = NULL;
        return current;
    } else {
        while (current->next != NULL) {
            if (current->user == user && current->m == modified) {
                current->cnt += 1;
                return head;
            }
            current = current->next;
        }
        if (current->user == user && current->m == modified) {
            current->cnt += 1;
            return head;
        }
        /* now we can add a new variable */
        current->next = malloc(sizeof (node_t));
        current->next->user = user;
        current->next->cnt = 1;
        current->next->m = modified;
        current->next->next = NULL;
    }
    return head;

}

void print_list(node_t * head, int mode) {
    node_t * current = head;
    if (mode == 1) {
        while (current != NULL) {
            printf("%u\n", current->user);
            current = current->next;
        }
    } else if (mode == 2) {
        while (current != NULL) {
            printf("%u ", current->user);
            if (current->m == 1) {
                printf("M\n");
            } else {
                printf("A\n");
            }
            current = current->next;
        }
    } else {
        while (current != NULL) {
            if (current->cnt >= 10) {
                printf("%u\n", current->user);
            }
            current = current->next;
        }
    }

}

void operation1(char* logfile) {
    FILE *fp = fopen(logfile, "r");

    if (fp == NULL) {
        printf("error: cant open %s", logfile);
        return;
    }
    node_t *userList = NULL;
    uid_t userID;
    char filePath[2048];
    int day, month, year, hour, min, sec;
    int openStatus;
    int actionStatus;
    unsigned char hash[MD5_DIGEST_LENGTH];
    int i;

    while (fscanf(fp, "%u %s %d-%d-%d %d:%d:%d %d %d",
            &userID, filePath, &year, &month, &day, &hour, &min, &sec, &openStatus, &actionStatus) != EOF) {

        if (actionStatus == 1) {
            userList = push(userList, userID, 0);
        }
        for (i = 0; i < MD5_DIGEST_LENGTH; i++) fscanf(fp, "%02x", &hash[i]);

    }
    print_list(userList, 1);
    fclose(fp);
}

void operation2(char* logfile, char* otherfile) {
    FILE *fp = fopen(logfile, "r");

    if (fp == NULL) {
        printf("error: cant open %s", logfile);
        return;
    }
    node_t *userList = NULL;
    uid_t userID;
    char filePath[2048];
    int day, month, year, hour, min, sec;
    int openStatus;
    int actionStatus;
    unsigned char hash1[MD5_DIGEST_LENGTH];
    unsigned char hash2[MD5_DIGEST_LENGTH];
    int i;
    int first = 1;

    memset(hash1, 1, MD5_DIGEST_LENGTH);
    memset(hash2, 1, MD5_DIGEST_LENGTH);
    while (fscanf(fp, "%u %s %d-%d-%d %d:%d:%d %d %d",
            &userID, filePath, &year, &month, &day, &hour, &min, &sec, &openStatus, &actionStatus) != EOF) {

        if (actionStatus == 0) {//ean eixan access
            for (i = 0; i < MD5_DIGEST_LENGTH; i++) fscanf(fp, "%02x", &hash1[i]);
            if (strcmp(filePath, otherfile) == 0) {//ean einai to file pou theloume
                if (first == 1) {
                    memcpy(hash2, hash1, MD5_DIGEST_LENGTH); //init hash2
                    first = 0;
                }
                if (compareHashes(hash1, hash2) == 0) {
                    userList = push(userList, userID, 0);
                } else {
                    memcpy(hash2, hash1, MD5_DIGEST_LENGTH); //kratame to neo hash value
                    userList = push(userList, userID, 1);
                }
            }
        } else {
            for (i = 0; i < MD5_DIGEST_LENGTH; i++) fscanf(fp, "%02x", &hash1[i]);
        }
    }
    print_list(userList, 2);
    fclose(fp);
}

void operation3(char* logfile) {
    FILE *fp = fopen(logfile, "r");

    if (fp == NULL) {
        printf("error: cant open %s", logfile);
        return;
    }
    node_t *userList = NULL;
    uid_t userID;
    char filePath[2048];
    int day, month, year, hour, min, sec, curDay = -1, curMonth = -1, curYear = -1;
    int openStatus;
    int actionStatus;
    unsigned char hash[MD5_DIGEST_LENGTH];
    int i;

    while (fscanf(fp, "%u %s %d-%d-%d %d:%d:%d %d %d",
            &userID, filePath, &year, &month, &day, &hour, &min, &sec, &openStatus, &actionStatus) != EOF) {

        if (curDay == -1) {
            curDay = day;
            curMonth = month;
            curYear = year;
        }
        if (year == curYear && month == curMonth && day == curDay) {
            if (actionStatus == 1) {
                userList = push(userList, userID, 0);
            }
        } else {
            curDay = day;
            curMonth = month;
            curYear = year;
            userList = NULL;
        }
        for (i = 0; i < MD5_DIGEST_LENGTH; i++) fscanf(fp, "%02x", &hash[i]);
    }
    print_list(userList, 3);
    fclose(fp);
}

// Print the MD5 sum as hex-digits.

void print_md5_sum(unsigned char* md) {
    int i;
    for (i = 0; i < MD5_DIGEST_LENGTH; i++) {
        printf("%02x", md[i]);
    }
    printf("\n");
}

//return 0 if it's equal

int compareHashes(unsigned char* md1, unsigned char* md2) {
    return memcmp(md1, md2, MD5_DIGEST_LENGTH);
}

int main(int argc, char** argv) {
    int program;
    char* logs;
    char* externFile;

    if (argc < 3 || argc > 4) {
        printf("Invalid parameters. Run program like this ./Task_B /path/logs.txt 1 \n");
        return 1;
    } else {
        logs = argv[1];
        program = atoi(argv[2]);
        if (program == 2) {
            if (argc != 4) {
                printf("Invalid parameters. Run program like this ./Task_B /path/logs.txt 2 /path/example.txt\n");
                return 1;
            }
            externFile = argv[3];
        } else {
            if (argc != 3) {
                printf("Invalid parameters. Run program like this ./Task_B /path/logs.txt 1 \n");
                return 1;
            }
        }
    }

    if (program == 1) {
        operation1(logs);
    } else if (program == 2) {
        operation2(logs, externFile);
    } else if (program == 3) {
        operation3(logs);
    } else {
        printf("Invalid ranging number. Use 1 or 2 or 3\n");
        return 1;
    }
    return 0;
}

