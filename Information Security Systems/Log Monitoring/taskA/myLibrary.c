/*
 * HY-457 / Exercise 2 / access controll system
 * Author: Anastasas Anastasios
 * Code: csd3166
 * Year: 2017-2018
 */
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include "myLibrary.h"
#include "logfile.h"
#include "originalFunctions.h"

FILE *fopen(const char* path, const char* mode) {
    initLogFile();
    FILE * retVal = originalFopen(path, mode);

    if (retVal == NULL) {
        appendLogFile(getuid(), path, 1, 0);
    } else {
        char* actualPath = getFilePath(fileno(retVal));
        appendLogFile(getuid(), actualPath, 1, 1);
        free(actualPath);
    }

    return retVal;
}

size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream) {
    initLogFile();
    size_t retVal = originalFwrite(ptr, size, nmemb, stream);
    char* path = getFilePath(fileno(stream));

    if (retVal == nmemb) {
        appendLogFile(getuid(), path, 0, 0);
    } else {
        appendLogFile(getuid(), path, 0, 1);
    }

    free(path);
    return retVal;
}

int open(const char *path, int oflags, ...) {
    initLogFile();
    va_list args;
    va_start(args, oflags);
    mode_t mode = va_arg(args, mode_t);
    FILE* fp;
    if (mode <= 4095) {
        fp = originalFopen(path, "w");
        if (chmod(path, mode) < 0) {
            printf("Fail to change permittion on file: %s: chmod() returned -1 \n", path);
        }
        fclose(fp);
    }
    int retVal = originalOpen(path, oflags);

    if (retVal < 0) {
        appendLogFile(getuid(), path, 1, 1);
    } else {
        appendLogFile(getuid(), path, 1, 0);
    }

    return retVal;
}

ssize_t write(int fd, const void *buf, size_t nbytes) {
    char* path = getFilePath(fd);
    ssize_t retVal = originalWrite(fd, buf, nbytes);

    if (retVal < 0) {
        appendLogFile(getuid(), path, 0, 1);
    } else {
        appendLogFile(getuid(), path, 0, 0);
    }

    return retVal;
}


/*
 * HY-457 / Exercise 2 / access controll system
 * Author: Anastasas Anastasios
 * Code: csd3166
 * Year: 2017-2018
 */