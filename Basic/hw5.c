#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char *argv[])
{
char *arxeio,*plithos,*keimeno;
int i,k,x=1;
FILE *f;
if (argc>=2)
{
	arxeio=argv[1];
	plithos=argv[2];
}
if ((f=fopen(arxeio,"r"))==NULL)
{
	printf("Cannot open the file.\n");
	exit(1);
}
for (;;)
{
	i=fgetc(f);
	if (i==EOF) break;
	x++;
}
keimeno=malloc((x+x/2)*sizeof(char));
x=0;
for (;;)
{
	i=fgetc(f);
	if (i==EOF) break;
	keimeno[x]=i;
	x++;
}
x=0;
for (;;)
{
	x++;
        i=fgetc(f);
        if (i==EOF) break;
	printf("%c",keimeno[x]);
}
fclose(f);






return 0;
}
