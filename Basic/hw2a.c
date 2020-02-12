/*Creator Tasos Anastasas 3166 */
#include<string.h>
#include <stdio.h>

int main(void)
{

int  n,p,k=0,einai=0,i,j;
char leksi[21],words[31][21],unique[31][21];

printf("Parakalw dwste enan akeraio MH arnhtiko arithmo: ");
scanf("%d", &n);
while (n<=0 || n>30)
          { /* Elegxos egkyrothtas toy N. */
        if (n<0) printf("O arithmos poy dwsate einai arnhtikos!\n");
        if (n==0) return 0;
        if (n>30) printf("O arithmos poy dwsate ksepernaei ta oria ths askhshs! :P\n");
        printf("Parakalw dwste enan akeraio mh arnhtiko arithmo megalytero toy 0 kai mikrotero toy 30:\n");
        scanf("%d\n", &n);
          }
for (i=0; i<n; i++)
	{ /*Diabasma twn N leksewn. */
    		printf("Parakalw dwse thn %d leksh: ",i+1);
   	        scanf(" %[^\n]", &(words[i][0]));
         	einai=0;
	 	for (j=0; j<n; j++)
			{
			     p=strncmp(words[i],unique[j],20);/*Elegxos ean yparxei h leksh ths ekswterikhs FOR stis monadikes lekseis*/
    			     if (p==0)
				{
				 (einai=1);
			        }
			}
   			     if (einai==0) /*Ean den yparxei antigrafei thn leksei ayth ston pinaka me tis monadikes lekseis*/
   				        {
						strncpy(unique[k],words[i],19);
                    			 	k=k+1;
    		        	        }
	}
int  frequence[k];
for(i=0; i<k; i++)
{
        frequence[i]=0;/* Arxikopoiei ton pinaka me tis syxnothtes me 0 */
}
for (i=0; i<k; i++)
	{
		for (j=0; j<n; j++)
			{
				p=strncmp(words[j],unique[i],19);/*Briskei poses fores yparxei h monadikh leksh stis synolikes lekseis*/
				if (p==0)
					{
						frequence[i]=frequence[i] + 1;
					}
     			}
	}
int max = - 1,l=1;
for (i=0; i<k; i++)
	{
		if (max<frequence[i]) /*Eyresh ths megisths syxnothtas */
			{
				max=frequence[i];
      			}
        }
for (i=0; i<k; i++)
        {
                if (max==frequence[i])
                        {
                                l=l+1; /*An l>1 tote tha emfanizei perissoteres apo mia lekseis ara emfanizei katallhlo mhnyma */
                        }
        }
if (l=1)
{
	printf("\nH syxnoterh leksh einai:\n");
}
if (l>1)
{
	printf("\nOi syxnoteres lekseis einai:\n");
}
for (i=0; i<k; i++)
{
if (max==frequence[i])  printf("%s\n",unique[i]);
}
return 0;
}
/*Creator Tasos Anastasas 3166 */
