/* vriskontai sto mixanima tis sxolhs: fthonos
 * open & write se exist file dika mou
 */

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#define UR "/spare/csd3166/r.txt"
#define UW "/spare/csd3166/w.txt"
#define URW "/spare/csd3166/rw.txt"

#define OR "/spare/Gle1der/r.txt"
#define OW "/spare/Gle1der/w.txt"
#define ORW "/spare/Gle1der/rw.txt"

int main(void) {

    printf("Try to open file: %s for append\n", UR);
    int fd = open(UR, O_WRONLY | O_APPEND);
    if (fd < 0) {
        printf("open() returned -1 \n");
        return 1;
    }
    char str[] = "This will be output\n";
    if (write(fd, str, strlen(str)) != strlen(str)) {
        printf("write() cant write on file\n");
        return 1;
    }
    printf("END-----Try to open file: %s for append\n", UR);

    return 0;
}
