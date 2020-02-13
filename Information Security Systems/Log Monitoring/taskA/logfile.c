#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#include <openssl/md5.h>


#include "logfile.h"
#include "originalFunctions.h"
#include "myLibrary.h"

/*
 *  Log file functions
 */
void initLogFile() {
    if (access(LOGFILE, F_OK) == -1) {

        FILE *fd = originalFopen(LOGFILE, "w");

        if (!fd) {
            printf("Fail to initialize logFile: fopen() returned NULL \n");
            exit(0);
        }
        char* str = "UID file_name date time open action_denied hash";
        originalFwrite(str, 1, strlen(str),fd);
        if (chmod(LOGFILE, S_IRWXU | S_IWGRP | S_IWOTH) < 0) {
            printf("Fail to initialize logFile: chmod() returned -1 \n");
            exit(0);
        }
        fclose(fd);
    }
}

void appendLogFile(uid_t userID, const char* filePath, int openStatus, int actionStatus) {
    time_t t = time(NULL);
    int i;
    struct tm tm = *localtime(&t);
    unsigned char* hash = calculateHash(filePath);

    FILE *fd = originalFopen(LOGFILE, "a");

    fprintf(fd, "\n%u %s %d-%d-%d %d:%d:%d %d %d ", userID, filePath,
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,
            openStatus, actionStatus);

    if (hash != NULL) {
        for (i = 0; i < MD5_DIGEST_LENGTH; i++) fprintf(fd, "%02x", hash[i]);
        free(hash);
    } else {
        for (i = 0; i < MD5_DIGEST_LENGTH; i++) fprintf(fd, "%02x", 0);
    }
    fclose(fd);
    printf("logfile updated\n");
}

unsigned char* calculateHash(const char* filePath) {
    unsigned char *retVal = (unsigned char*) malloc(MD5_DIGEST_LENGTH);
    FILE *fd = originalFopen(filePath, "rb");
    MD5_CTX mdContext;
    int bytes;
    unsigned char data[1024];

    if (fd == NULL) {
        printf("%s can't be opened to calculate hash value.\n", filePath);
        free(retVal);
        return NULL;
    }
    MD5_Init(&mdContext);
    while ((bytes = fread(data, 1, 1024, fd)) != 0) {
        MD5_Update(&mdContext, data, bytes);
    }
    MD5_Final(retVal, &mdContext);
    fclose(fd);
    return retVal;
}

char* getFilePath(int fp) {
    int MAXSIZE = 0xFFF;
    char proclnk[MAXSIZE];
    char *filename = (char*) malloc(MAXSIZE);
    ssize_t r;

    sprintf(proclnk, "/proc/self/fd/%d", fp);

    r = readlink(proclnk, filename, MAXSIZE);

    if (r < 0) {
        printf("failed to readlink\n");
        filename = " ";
        return filename;
    }
    filename[r] = '\0';
    return filename;
}