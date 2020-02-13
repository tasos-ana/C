#ifndef MYLIBRARY_H
#define MYLIBRARY_H
#include <stdio.h>

FILE *fopen(const char* path, const char* mode);

size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);

int open(const char *path, int oflags, ...);

ssize_t write(int fd, const void *buf, size_t nbytes);

#endif // MYLIBRARY_H

