#define _GNU_SOURCE
#include <stdio.h>
#include <dlfcn.h>

#include "originalFunctions.h"

/*
 *Util function
 */
FILE *originalFopen(const char *path, const char* mode) {
    FILE * (*original_fopen) (const char*, const char*);
    original_fopen = dlsym(RTLD_NEXT, "fopen");

    return (*original_fopen)(path, mode);
}

size_t originalFwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t(*original_fwrite) (const void *, size_t, size_t, FILE *);
    original_fwrite = dlsym(RTLD_NEXT, "fwrite");
    return (*original_fwrite)(ptr, size, nmemb, stream);
}

int originalOpen(const char *path, int oflags) {
    int (*original_open) (const char*, int);
    original_open = dlsym(RTLD_NEXT, "open");

    return (*original_open)(path, oflags);
}

ssize_t originalWrite(int fd, const void *buf, size_t nbytes) {
    int (*original_write) (int, const void *, size_t);
    original_write = dlsym(RTLD_NEXT, "write");

    return (*original_write)(fd, buf, nbytes);
}