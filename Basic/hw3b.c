/*Creator Tasos Anastasas 3166 */
#include<stdio.h>

/*H sinartisi lathos vriskei kathe fora ean upaxoun diplotipa ston pinaka[9], an uparxoun tote emfanizei to einai+*/
void lathos(int pinakas[],int einai);
/*H sinartisi elegxos elegxei ton katastasi[9][9] kai metraei poses fores emfanizete to kathe noumero, ean to kathe noumero emfanizete 9 fores tote epistefei 1 alliws 0. */
int elegxos(int katastasi[][9]);

int main(void){


int pi[9][9],i,j=0,k=0,einai,l,n,katastasi[9],box=0;

/*Parakatw emfanizete i grafiki anaparastasi tou pinaka SUDOKU kai to pws tha diavazontai oi arithmoi! */
printf("\n\n****************************************************\n");
printf("********************** SUDOKU **********************\n");
printf("****************************************************\n");
printf("\nOi arithmoi diavazontai opws fainetai parakatw!\n");
printf("++----+----+----+ +----+----+----+ +----+----+----++\n");
printf("|| 00 | 01 | 02 | | 03 | 04 | 05 | | 06 | 08 | 09 ||\n");
printf("++   -+-  -+-  -+ +-  -+-  -+-  -+ +-  -+-  -+-  -++\n");
printf("|| 10 | 11 | 12 | | 13 | 14 | 15 | | 16 | 17 | 18 ||\n");
printf("++   -+-  -+-  -+ +-  -+-  -+-  -+ +-  -+-  -+-  -++\n");
printf("|| 19 | 20 | 21 | | 22 | 23 | 24 | | 25 | 26 | 27 ||\n");
printf("++----+----+----+ +----+----+----+ +----+----+----++\n");
printf("++----+----+----+ +----+----+----+ +----+----+----++\n");
printf("|| 28 | 29 | 30 | | 31 | 32 | 33 | | 34 | 35 | 36 ||\n");
printf("++   -+-  -+-  -+ +-  -+-  -+-  -+ +-  -+-  -+-  -++\n");
printf("|| 37 | 38 | 39 | | 40 | 41 | 42 | | 43 | 44 | 45 ||\n");
printf("++   -+-  -+-  -+ +-  -+-  -+-  -+ +-  -+-  -+-  -++\n");
printf("|| 46 | 47 | 48 | | 49 | 50 | 51 | | 52 | 53 | 54 ||\n");
printf("++----+----+----+ +----+----+----+ +----+----+----++\n");
printf("++----+----+----+ +----+----+----+ +----+----+----++\n");
printf("|| 55 | 56 | 57 | | 58 | 59 | 60 | | 61 | 62 | 63 ||\n");
printf("++   -+-  -+-  -+ +-  -+-  -+-  -+ +-  -+-  -+-  -++\n");
printf("|| 64 | 65 | 66 | | 67 | 68 | 69 | | 70 | 71 | 72 ||\n");
printf("++   -+-  -+-  -+ +-  -+-  -+-  -+ +-  -+-  -+-  -++\n");
printf("|| 73 | 74 | 75 | | 76 | 77 | 78 | | 79 | 80 | 81 ||\n");
printf("++----+----+----+ +----+----+----+ +----+----+----++\n");
/*Parakatw ginetai to diavasma twn 81 arithmwn!*/
for (i=0; i<9; i++)
{
	for(j=0; j<9; j++)
	{
		k++;
		do
		{
			printf("Arithmos %d/%d: ",k,81);
			scanf("%d",&pi[i][j]);
		}
		while (pi[i][j]<1 || pi[i][j]>9);
	}
}
/*Emfanizei tin nea formi tou sudoku simliromenw me ta noumera pou edwse o xristis!*/
printf("\nTo sudoku pou dwsate einai to parakatw:\n");
printf("++---+---+---+ +---+---+---+ +---+---+---++\n");
printf("|| %d | %d | %d | | %d | %d | %d | | %d | %d | %d ||\n",pi[0][0],pi[0][1],pi[0][2],pi[0][3],pi[0][4],pi[0][5],pi[0][6],pi[0][7],pi[0][8]);
printf("++   +   +   + +   +   +   + +   +   +   ++\n");
printf("|| %d | %d | %d | | %d | %d | %d | | %d | %d | %d ||\n",pi[1][0],pi[1][1],pi[1][2],pi[1][3],pi[1][4],pi[1][5],pi[1][6],pi[1][7],pi[1][8]);
printf("++   +   +   + +   +   +   + +   +   +   ++\n");
printf("|| %d | %d | %d | | %d | %d | %d | | %d | %d | %d ||\n",pi[2][0],pi[2][1],pi[2][2],pi[2][3],pi[2][4],pi[2][5],pi[2][6],pi[2][7],pi[2][8]);
printf("++---+---+---+ +---+---+---+ +---+---+---++\n");
printf("++---+---+---+ +---+---+---+ +---+---+---++\n");
printf("|| %d | %d | %d | | %d | %d | %d | | %d | %d | %d ||\n",pi[3][0],pi[3][1],pi[3][2],pi[3][3],pi[3][4],pi[3][5],pi[3][6],pi[3][7],pi[3][8]);
printf("++   +   +   + +   +   +   + +   +   +   ++\n");
printf("|| %d | %d | %d | | %d | %d | %d | | %d | %d | %d ||\n",pi[4][0],pi[4][1],pi[4][2],pi[4][3],pi[4][4],pi[4][5],pi[4][6],pi[4][7],pi[4][8]);
printf("++   +   +   + +   +   +   + +   +   +   ++\n");
printf("|| %d | %d | %d | | %d | %d | %d | | %d | %d | %d ||\n",pi[5][0],pi[5][1],pi[5][2],pi[5][3],pi[5][4],pi[5][5],pi[5][6],pi[5][7],pi[5][8]);
printf("++---+---+---+ +---+---+---+ +---+---+---++\n");
printf("++---+---+---+ +---+---+---+ +---+---+---++\n");
printf("|| %d | %d | %d | | %d | %d | %d | | %d | %d | %d ||\n",pi[6][0],pi[6][1],pi[6][2],pi[6][3],pi[6][4],pi[6][5],pi[6][6],pi[6][7],pi[6][8]);
printf("++   +   +   + +   +   +   + +   +   +   ++\n");
printf("|| %d | %d | %d | | %d | %d | %d | | %d | %d | %d ||\n",pi[7][0],pi[7][1],pi[7][2],pi[7][3],pi[7][4],pi[7][5],pi[7][6],pi[7][7],pi[7][8]);
printf("++   +   +   + +   +   +   + +   +   +   ++\n");
printf("|| %d | %d | %d | | %d | %d | %d | | %d | %d | %d ||\n",pi[8][0],pi[8][1],pi[8][2],pi[8][3],pi[8][4],pi[8][5],pi[8][6],pi[8][7],pi[8][8]);
printf("++---+---+---+ +---+---+---+ +---+---+---++\n");

/*Elegxei ean einai swsto to sudoku! An nai emfanizei pio katw katallilo minima (einai==1)!*/
einai=elegxos(pi);
if (einai==0)
{
	printf("\nTo sudoku pou dwsate den einai swsto! :(");
	printf("\nDiorthwste ta eksis: ");
	printf("\nLathos grammes (Invalid rows): ");
	for (i=0; i<9; i++)
	{
		for (j=0; j<9; j++)
		{
			katastasi[j]=pi[i][j];  /*Topothetei ston pinaka katastasi[i] olous tous arithmous kathe grammis tou sudoku*/
		}
		lathos(katastasi,i); /*H sinartisi afti elegxei ean iparxoun  diplotipa! Ean uparxoun emfanizei san minima tin grammia tou pinaka.*/
	}
	printf("\nLathos stiles (Invalid columns): ");
	for (i=0; i<9; i++)
	{
		for (j=0; j<9; j++)
		{
			katastasi[j]=pi[j][i];/*Topothetei ston pinaka katastasi[i] olous tous arithmous kathe GRAMMIS tou sudoku*/
		}
		lathos(katastasi,i);/*H sinartisi afti elegxei ean iparxoun  diplotipa! Ean uparxoun emfanizei san minima tin stili tou pinaka pou vriskete aftin tin stigmi*/
	}
	printf("\nLathos koutia (Invalid blocks): ");
	for (i=0; i<=6; i=i+3)
	{
		k=0;
		for (j=0; j<=6; j=j+3) /*Oi duo parapanw epanalipsis allazoun koutia apo aristera->deksia kai meta apo katw aristera->deksia.*/
		{
			for (l=j; l<j+3; l++)
			{
				for (n=i; n<i+3; n++)
				{
					katastasi[k]=pi[n][l];/*Topothetei ston pinaka katastasi[i] olous tous arithmous tou antistoixou koutiou tou sudoku*/
					k++;
				}
			}
		lathos(katastasi,box);/*H sinartisi afti elegxei ean iparxoun  diplotipa! Ean uparxoun emfanizei san minima to kouti pou vriskete aftin tin stigmi*/
		k=0;
		box++;
		}
	}
	printf("\n");
	printf("++---+---+---+ +---+---+---+ +---+---+---++\n");
	printf("|| %d | %d | %d | | %d | %d | %d | | %d | %d | %d ||\n",pi[0][0],pi[0][1],pi[0][2],pi[0][3],pi[0][4],pi[0][5],pi[0][6],pi[0][7],pi[0][8]);
	printf("++   +   +   + +   +   +   + +   +   +   ++\n");
	printf("|| %d | %d | %d | | %d | %d | %d | | %d | %d | %d ||\n",pi[1][0],pi[1][1],pi[1][2],pi[1][3],pi[1][4],pi[1][5],pi[1][6],pi[1][7],pi[1][8]);
	printf("++   +   +   + +   +   +   + +   +   +   ++\n");
	printf("|| %d | %d | %d | | %d | %d | %d | | %d | %d | %d ||\n",pi[2][0],pi[2][1],pi[2][2],pi[2][3],pi[2][4],pi[2][5],pi[2][6],pi[2][7],pi[2][8]);
	printf("++---+---+---+ +---+---+---+ +---+---+---++\n");
	printf("++---+---+---+ +---+---+---+ +---+---+---++\n");
	printf("|| %d | %d | %d | | %d | %d | %d | | %d | %d | %d ||\n",pi[3][0],pi[3][1],pi[3][2],pi[3][3],pi[3][4],pi[3][5],pi[3][6],pi[3][7],pi[3][8]);
	printf("++   +   +   + +   +   +   + +   +   +   ++\n");
	printf("|| %d | %d | %d | | %d | %d | %d | | %d | %d | %d ||\n",pi[4][0],pi[4][1],pi[4][2],pi[4][3],pi[4][4],pi[4][5],pi[4][6],pi[4][7],pi[4][8]);
	printf("++   +   +   + +   +   +   + +   +   +   ++\n");
	printf("|| %d | %d | %d | | %d | %d | %d | | %d | %d | %d ||\n",pi[5][0],pi[5][1],pi[5][2],pi[5][3],pi[5][4],pi[5][5],pi[5][6],pi[5][7],pi[5][8]);
	printf("++---+---+---+ +---+---+---+ +---+---+---++\n");
	printf("++---+---+---+ +---+---+---+ +---+---+---++\n");
	printf("|| %d | %d | %d | | %d | %d | %d | | %d | %d | %d ||\n",pi[6][0],pi[6][1],pi[6][2],pi[6][3],pi[6][4],pi[6][5],pi[6][6],pi[6][7],pi[6][8]);
	printf("++   +   +   + +   +   +   + +   +   +   ++\n");
	printf("|| %d | %d | %d | | %d | %d | %d | | %d | %d | %d ||\n",pi[7][0],pi[7][1],pi[7][2],pi[7][3],pi[7][4],pi[7][5],pi[7][6],pi[7][7],pi[7][8]);
	printf("++   +   +   + +   +   +   + +   +   +   ++\n");
	printf("|| %d | %d | %d | | %d | %d | %d | | %d | %d | %d ||\n",pi[8][0],pi[8][1],pi[8][2],pi[8][3],pi[8][4],pi[8][5],pi[8][6],pi[8][7],pi[8][8]);
	printf("++---+---+---+ +---+---+---+ +---+---+---++\n");
}
	if (einai==1)
	{
	printf("\nTo sudoku pou dwsate einai swsto!!! Nice! :)\n\n");
	return 0;
	}
}


int elegxos(int katastasi[][9])
{
	int i,j,emfanisi[10]={0};
	for (i=0; i<9; i++)
	{
		for (j=0; j<9; j++)
		{
			emfanisi[katastasi[i][j]]++;   /*Metraei poses fores uparxoun oi arithmoi 1-9*/
		}
	}
	for (i=1; i<10; i++)
	{
		if (emfanisi[i]<9) return 0; /*Ean den uparxei kapoios arithmos 9 fores tote epistrefei 0, diladi o pinakas exei toulaxiston ena lathos*/
	}
	return 1;
}




void lathos(int pinakas[],int einai)
{
	int i,j,emfanisi[9]={0};
	for (i=0; i<9; i++)
	{
		for (j=0; j<9; j++)
		{
			if (pinakas[i]==pinakas[j]) emfanisi[i]++; /*Elegxei tous arithmous tou pinaka, kai ean uparxoun diploi tote*/
		}
	}
	for (i=0; i<9; i++)
	{
		if (emfanisi[i]!=1)
		{
			printf("%d ", einai+1);
                        break;
		}
	}
}
