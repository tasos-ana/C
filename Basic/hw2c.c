/*Creator Tasos Anastasas 3166 */
#include<stdio.h>

int n,i,j;

int main() {
printf("Parakalw dwste enan akeraio mh arnhtiko arithmo: ");
scanf("%d", &n);
 while(n<0) {
  printf("Parakalw dwste enan akeraio mh arnhtiko arithmo: ");
  scanf("%d", &n);
 }
 i=1;
 while (i<=n) {
  for (j=1; j<=i; j++) {
     printf("* ");
  }
  i++;
  printf("\n");
 }
  return 0;
}
/*Creator Tasos Anastasas 3166 */
