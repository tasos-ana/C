/* Creator Tasos Anastasas 3166 */
#include<stdio.h>
#include<string.h>
int main(void) {

int thesi=1,epilogi,i,j,katastasi[9]={0},k=1,mpike=0;
char arithmoi[3][3]={'1','2','3',
                     '4','5','6',
                     '7','8','9'};
char pinakas[3][3];
/*
H thesi einai metavliti gia tin seira tou kathe paixti.
H metavliti k einai enas metritis gia na arxikopoiithei o  pinakas arithmoi[3][3] me tous arithmous 1,2...9.
H metavliti mpike=0 ean to paixnidi einai isopalo enw mpike=1 ean uparxei kapoios nikitis.
*/
for (i=0; i<3; i++)
{
	for (j=0;j<3; j++)
	{
		pinakas[i][j]=' '; /*Arxikopoiisi twn pinakwn*/
	}
}
printf("\n\n***************************\n");
printf("*       Tic Tac Toe       *\n");
printf("***************************\n");
printf("*      Lets's  Start      *\n");
printf("***************************\n\n");
printf(" %c | %c | %c 	 %c | %c | %c \n",pinakas[0][0],pinakas[0][1],pinakas[0][2],arithmoi[0][0],arithmoi[0][1],arithmoi[0][2]);
printf("---+---+---	---+---+---\n");
printf(" %c | %c | %c 	 %c | %c | %c \n",pinakas[1][0],pinakas[1][1],pinakas[1][2],arithmoi[1][0],arithmoi[1][1],arithmoi[1][2]);
printf("---+---+---	---+---+---\n");
printf(" %c | %c | %c 	 %c | %c | %c \n",pinakas[2][0],pinakas[2][1],pinakas[2][2],arithmoi[2][0],arithmoi[2][1],arithmoi[2][2]);
for (thesi=1; thesi<10; thesi++)
{
		do
		{
                        if (thesi%2==1) printf("\nPlayer X:"); /*Emfanizei enalaks pios paixtis paizei */
                        if (thesi%2==0) printf("\nPlayer O:");
			scanf("%d",&epilogi);
                 }
		while ( (epilogi<1 || epilogi>9 || katastasi[epilogi-1]==1)); /*Mexri h epilogi na exei apodektes times*/
		katastasi[epilogi-1]=1;/*Egxwrei ston pinaka tin timi 1 diladi oti afti i thesi exei paixti*/
		/*Topothetei to katalilo sumvolo sto katallilo keli*/
		if (epilogi<=3)
		{
			if (thesi%2==1) pinakas[0][epilogi-1]='X';
			if (thesi%2==0) pinakas[0][epilogi-1]='O';
			arithmoi[0][epilogi-1]=' ';
		}
		if (epilogi>3 && epilogi<=6)
		{
			if (thesi%2==1) pinakas[1][epilogi-4]='X';
			if (thesi%2==0) pinakas[1][epilogi-4]='O';
			arithmoi[1][epilogi-4]=' ';
		}
		if (epilogi>6 && epilogi<=9)
		{
			if (thesi%2==1) pinakas[2][epilogi-7]='X';
			if (thesi%2==0) pinakas[2][epilogi-7]='O';
			arithmoi[2][epilogi-7]=' ';
		}
		/*Emfanizei tin nea diataksi tou pinaka kai kanei 0 to arithmo pou exei paixti*/
		printf(" %c | %c | %c       %c | %c | %c \n",pinakas[0][0],pinakas[0][1],pinakas[0][2],arithmoi[0][0],arithmoi[0][1],arithmoi[0][2]);
		printf("---+---+---     ---+---+---\n");
		printf(" %c | %c | %c       %c | %c | %c \n",pinakas[1][0],pinakas[1][1],pinakas[1][2],arithmoi[1][0],arithmoi[1][1],arithmoi[1][2]);
		printf("---+---+---     ---+---+---\n");
		printf(" %c | %c | %c       %c | %c | %c \n",pinakas[2][0],pinakas[2][1],pinakas[2][2],arithmoi[2][0],arithmoi[2][1],arithmoi[2][2]);
		for (i=0; i<3; i++)
		{
			/*Elegxei ean uparxei nikitis arxika se grammi*/
			if ((pinakas[i][0]==pinakas[i][1]) && (pinakas[i][0]==pinakas[i][2]) &&(pinakas[i][0]!=' '))
			{
				printf("\nNikitis einai o Player %c. Sunxaritiria!!\n",pinakas[i][0]);
				mpike=1;
				return 0;
			}
			/*Stin sunexeia elegxei se stili*/
			if ((pinakas[0][i]==pinakas[1][i]) && (pinakas[0][i]==pinakas[2][i]) &&(pinakas[0][i]!=' '))
                        {
                                printf("\nNikitis einai o Player %c. Sunxaritiria!!\n",pinakas[0][i]);
				mpike=1;
				return 0;
                        }
		}
		/*Elegxei an uparxei nikitis kai stis duo stiles*/
		if ((pinakas[0][0]==pinakas[1][1]) && (pinakas[0][0]==pinakas[2][2]) &&(pinakas[0][0]!=' '))
		{
			printf("\nNikitis einai o Player %c. Sunxaritiria!!\n",pinakas[0][0]);
			mpike=1;
			break;
		}
		if ((pinakas[0][2]==pinakas[1][1]) && (pinakas[0][2]==pinakas[2][0]) &&(pinakas[0][2]!=' '))
		{
			printf("\nNikitis einai o Player %c. Sunxaritiria!!\n",pinakas[2][0]);
			mpike=1;
			break;
		}
}
/*An i mpike einai 0 tote uparxei isopalia*/
if (mpike==0) printf("\nDen uparxei nikitis, to paixnidi irthe isopalia!!\n");
system("PAUSE");
return 0;
}
/* Creator Tasos Anastasas 3166 */
