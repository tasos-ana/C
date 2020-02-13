/* vriskontai sto mixanima tis sxolhs: fthonos
 * fopen & fwrite se exist file dika mou
 */
#include <stdio.h>
#include <unistd.h>

#define UR "/spare/csd3166/r.txt"
#define UW "/spare/csd3166/w.txt"
#define URW "/spare/csd3166/rw.txt"

#define OR "/spare/Gle1der/r.txt"
#define OW "/spare/Gle1der/w.txt"
#define ORW "/spare/Gle1der/rw.txt"

int main(void) {
    
    printf("Try to open file: %s for append\n", UW);
    FILE *fd = fopen(UW, "a");
    if (!fd) {
        printf("fopen() returned NULL \n");
        return 1;
    }
    char str[] = "hello kosme\n";
    fwrite(str, sizeof (str), 1, fd);
    fclose(fd);
    printf("END-----Try to open file: %s for append\n", UW);
   
    
    return 0;
}