/*Creator Tasos Anastasas 3166 */
#include<stdio.h>
#include <ctype.h>
#include<string.h>

char words[20],a,b;
int n,i,j,max,einai,k;

int main() {
printf("Parakalw dwste enan mh arnhtiko arithmo:");
scanf("%d", &n);
while (n<=0 || n>30) /*Elegxos egkyrothtas toy n */
{
	if (n<0) printf("O arithmos poy dwsate einai arnhtikos!\n");
	if (n==0) return 0;
	if (n>30) printf("O arithmos poy dwsate ksepernaei ta oria ths askhshs! :P\n");
	printf("Parakalw dwste enan mh arnhtiko arithmo:");
	scanf("%d\n", &n);
}
	for (k=0; k<n; k++)
	{
		einai=1;
		printf("Parakalw dwse thn %d leksh:\n", k+1);
                scanf(" %[^\n]",&(words[0]));/*Diabasma ths prwths lekseis */
		max=strlen(words);
		i=0;
		j=max-1;
		while (i<j && einai==1)
		{
			a=tolower(words[i]);/*Metatroph twn grammatwn se mikra */
			b=tolower(words[j]);
			if (a!=' ')/*An to gramma den einai to keno tote */
			{
				if (b!=' ')/*An to gramma den einai to keno tote */
				{
					if (a==b)/*Sygkrinei ta grammata to teleytaio me to prwto, to proteleytaio me to deytero ktl*/
					{
						i++;/*Phgenei sto deytero gramma */
						j--;/*Phgenei sto proteleytaio gramma */
					}
					if (a!=b)/*Ean brei ena diaforetiko gramma stamataei na sygkrinei ta ypoloipa kai teleiwnei h WHILE */
					{
						einai=0;
					}
				}
				if (b==' ')/*Ean to gramma ayto einai to keno tote synexizei sto prohgoymeno h epomeno antistoixa */
				{
					j--;
				}
			}
			if (a==' ')
			{
				i++;
			}
		}
		if (einai==1)
		{
			printf("H leksh %d einai palindromo! (Input is a palindrome)\n", k+1);
		}
		if (einai==0)
		{
			printf("H leksh %d DEN einai palindromo! (Input is not a palindrome)\n", k+1);
			einai=1;
		}
	}
return 0;
}
/*Creator Tasos Anastasas 3166 */
