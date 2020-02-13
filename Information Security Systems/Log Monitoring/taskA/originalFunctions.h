#ifndef ORIGINALFUNCTIONS_H
#define ORIGINALFUNCTIONS_H

FILE *originalFopen(const char *path, const char* mode);

size_t originalFwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);

int originalOpen(const char *path, int oflags);

ssize_t originalWrite(int fd, const void *buf, size_t nbytes);

#endif // ORIGINALFUNCTIONS_H

