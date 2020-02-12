/*Creator Tasos Anastasas 3166*/
#include<stdio.h>
#include<stdlib.h>

/*SYNARTISTEIS*/
void tupwnw(int **emfanisi,int k,int l);/*H sinartisi afti tipwnei tous purgous mazi me tous diskous*/
int  **transfer (int **emfanisi,int N,int topa,int a,int topb,int b,int topc,int c);
void tupwsi(int **emfanisi);
/*--------------------------------------------------------------------------------------------------------*/
int Ns,top1,top2,top3;
int main(int argc, char *argv[])
{

int N,i,j,kinisis=2,k,l,**emfanisi=malloc(2*sizeof(int *));
/*
-To N einai o arithmos twn diskon.
-To sum1,sum3 einai metrites. Metrane to athroisma twn arithmwn tou prwtou kai tritou purgou.
-To top1,top2,top3 deixnoun tin korifi tou 1ou,2ou,3ou pirgou antistoixa stin opoia uparxei arithmos.
-H seira afora metaksi pion purgwn tha ginei elegxos kai metakinisi.
-H kinisis upologizei poses kiniseis eginan wste na epilithei to provlima.
*/
if (argc!=2)
{
        printf("Parakalw dwste enan arithmo!\n");
        exit(1);
}
else
{
	N=atoi(argv[1]);
	if (N<=1)
	{
		printf("Parakalw dwste enan arithmo megalutero tou 1.\n");
		exit(1);
	}
}
Ns=N;
*emfanisi=realloc(emfanisi,(N+1)*sizeof(int *));
printf("\n\n");
for (i=0; i<N+1; i++)
{
	emfanisi[i]=malloc(3*sizeof(int));
	for (j=0; j<3; j++)
	{
		emfanisi[i][j]=124; /*Arxikopoioume ton pinaka me ton arithmo 124 pou antistoixi sumfona me ton kwdika ASCI ston xaraktira | */
	}
}
for (i=1; i<N+1; i++)/*Prosthetoume arithmous kata pthinousa seira ston prwto pirgo*/
{
	emfanisi[i][0]=i;
}
top1=0; /*Arxikopoiisi twn metavlitwn top pou deixnoun ston mikrotero arithmo kathe purgou*/
top2=Ns;
top3=Ns;
for (i=0; i<3; i++)
{
		if ((i==0) || (i==2))	printf("*********************\n");
		else			printf("*    Havoi-Tower    *\n");
}
/*Emfanizoume tin katastasi twn triwn pirgwn me tin boithia tis sinartisis tupwnw(...)*/
tupwsi(emfanisi);
printf("       Start in\n");
sleep(1);
printf("          3\n");
sleep(1);
printf("          2\n");
sleep(1);
printf("          1\n");
sleep(1);
printf("\n\n");
transfer(emfanisi,N,top1,0,top3,2,top2,1);
for (i=1; i<N; i++)
{
        kinisis=kinisis+kinisis; /*Upologizei to plithos ton kinisewn gia tin epilisi tou provlimatos kai stin sunexeia emfanizei to kinisi-1*/
}
printf("\nOi kiniseis gia tin epilisi tou pyrgou itan %d.\n\n",kinisis-1);
free(emfanisi);
return 0;
}

int **transfer (int **emfanisi,int N,int topa,int a,int topc,int c,int topb,int b)
{
	if (N==1)
	{
		printf("%d %d %d\n",top1,top2,top3);
		printf("\n%d\n",N);
                if ((emfanisi[topa][a]==124) && (topa!=Ns)) topa++;
		emfanisi[topc][c]=emfanisi[topa][a];
                emfanisi[topa][a]=124;
                if (a==0)
		{
			printf("A->");
			if (topa!=Ns) top1++;
		}
                else if (a==1)
		{
			printf("B->");
			if (topa!=Ns) top2++;
		}
                else
		{
			printf("C->");
			if (topa!=Ns) top3++;
		}
                if (c==0)
		{
			printf("A\n");
			top1--;
		}
                else if (c==1)
		{
			printf("B\n");
			top2--;
		}
                else
		{
			printf("C\n");
			top3--;
		}
		tupwsi(emfanisi);
	}
	else
	{
		transfer(emfanisi,N-1,top1,0,top2,1,top3,2);
		if ((emfanisi[topa][a]==124) && (topa!=Ns)) topa++;
                emfanisi[topc][c]=emfanisi[topa][a];
                emfanisi[topa][a]=124;
                if (a==0)
                {
                        printf("A->");
                        if (topa!=Ns) top1++;
                }
                else if (a==1)
                {
                        printf("B->");
                        if (topa!=Ns) top2++;
                }
                else
                {
                        printf("C->");
                        if (topa!=Ns) top3++;
                }
                if (c==0)
                {
                        printf("A\n");
                        top1--;
                }
                else if (c==1)
                {
                        printf("B\n");
                        top2--;
                }
                else
                {
                        printf("C\n");
                        top3--;
                }
                tupwsi(emfanisi);
		transfer(emfanisi,N-1,top2,1,top3,2,top1,0);
	}
	return emfanisi;
}

void tupwsi(int **emfanisi)
{
int i,j;

 	for (i=0; i<Ns+1; i++)
        {
                        for (j=0; j<3; j++)
                        {
                                tupwnw(emfanisi,i,j);
                        }
	printf("\n");
        }
	tupwnw(emfanisi,Ns+1,1);
}

void tupwnw(int **emfanisi,int k,int l) /*Me tin sunartisis aftin ektipwnw olokliri tin katastasi kathe fora pou kalite*/
{

int i,j;
     if(k!=Ns+1) /*an i grammi pou exei perasti den einai i teleftaia tote...*/
     {
        printf(" "); /*afinoume ena space perithorio*/
        if (emfanisi[k][l]==124) /*an stin grammi k kai stin stili l uparxei | tote ...*/
        {
                for (j=1; j<=2*Ns-1; j++) /*afinei aristera kai deksia kena oso einai to N-1*/
                {
                        if (j==Ns)  printf("|");
                        else printf(" ");
                }
        }
        else /*an i grammi kai i stili afti exexei arithmo tote emfanizei kena= N-(timi tou arithmou (1,2,3,4...))*/
        {
                for (j=1; j<=Ns-emfanisi[k][l]; j++)
                {
                        printf(" ");/*emfanizw kapia kena prin ton arithmo*/
                }
                if (emfanisi[k][l]<10)
                {
                        for (j=1; j<=2*emfanisi[k][l]-1; j++)
                        {
                                printf("%d",emfanisi[k][l]);/*emfanizw ton artithmo wses fores xreiazete*/
                        }
                }
                else
                {
                        for (j=1; j<=(2*emfanisi[k][l]-1)/2; j++)
                        {
                                printf("%d",emfanisi[k][l]);
                        }
                        printf("%d",emfanisi[k][l]/10);
                }
                for (j=1; j<=Ns-emfanisi[k][l]; j++)
                {
                        printf(" ");/*emfanizw osa kena emfanisa kai prin ton arithmo*/
                }
        }
      }
     else/*an k=n+1*/
        {
                for (j=1; j<=6*Ns+1; j++)
                {
                        printf("-");/*emfanizw sto telos 6*n+1 kena*/
                }
                printf("\n");
        }
}
/*Creator Tasos Anastasas 3166*/
