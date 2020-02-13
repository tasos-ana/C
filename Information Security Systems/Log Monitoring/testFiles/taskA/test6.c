/* vriskontai sto mixanima tis sxolhs: fthonos
 * open & write se not exist file me read only 
 */

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#define UR "/spare/csd3166/r.txt"
#define UW "/spare/csd3166/w.txt"
#define URW "/spare/csd3166/rw.txt"
#define NOTEXIST "/spare/csd3166/test.txt"

#define OR "/spare/Gle1der/r.txt"
#define OW "/spare/Gle1der/w.txt"
#define ORW "/spare/Gle1der/rw.txt"

int main(void) {
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    printf("Try to open file: %s for append\n", NOTEXIST);
    int fd = open(NOTEXIST, O_WRONLY | O_APPEND, mode);
    if (fd < 0) {
        printf("open() returned -1 \n");
        return 1;
    }
    char str[] = "This will be output\n";
    if (write(fd, str, strlen(str)) != strlen(str)) {
        printf("write() cant write on file\n");
        return 1;
    }
    printf("END-----Try to open file: %s for append\n", NOTEXIST);

    return 0;
}