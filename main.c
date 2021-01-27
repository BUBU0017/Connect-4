#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#define W 5
#define K 6
#define ILE 4//ile połączyć
typedef struct _Plansza
{
    int elementy[W][K];
    int p1_p2;//ktory gracz 1 czy 2
}Plansza;
typedef struct _Ruchy
{
  	int kolumna;
  	struct _Ruchy *next;
}Ruchy;

void zwolnij_ruchy(Ruchy *o)
{
    if(o->next!=NULL)
    free(o->next);
    free(o);
}
void zwolnij_macierz(Plansza *m){
    for (int i = 0; i <W; i++)
    free(m->elementy[i]);
    free(m->elementy);
    free(m);
}
Plansza* wczytaj(FILE * fin) {
    int i,j;
    Plansza *m=(Plansza*) malloc(sizeof(Plansza));
    
    for (i=0; i < W; i++)
        for (j=0; j <K; j++)
            fscanf(fin, "%d", &m->elementy[i][j]);
    return m;
}

void zapisz_macierz(char nazwa[], Plansza *m) {
    FILE* fout = fopen(nazwa, "w");
    int i,j;

    for (i = 0; i < W; i++)
        for (j=0; j < K; j++) 
            fprintf(fout, "%d\n", m->elementy[i][j]);//dla wiekszej dokladnosci po przecinku usunac .1!

    fclose(fout);
}

void wypisz(Plansza *m)
{
    int i, j;
    printf("      ");
    for(int x=0; x<K; x++)
    {
        printf("%3d",x);
    }
    printf("\n       ");
    for(int x=0; x<K; x++)
    printf("---");
    printf("\n");
    for (i = 0; i < W; i++) {
    printf("%3d | [ ",i);
        for (j=0; j < K; j++) {
 
            printf("%2d ", m->elementy[i][j]);
        }
            printf("]\n");
    }
    printf("\n");
}

int czyj_ruch(Plansza m)
{  
    int ile1=0;
    int ile2=0;
    for (int x=0; x<W; x++)
        for (int y=0; y<K; y++)
        {
            if(m.elementy[x][y] == 1)
            {
                ile1++;
                // printf("ile1:%d\n", ile1);
            }                

            else if(m.elementy[x][y] == 2)
            {
                ile2++;
                // printf("ile2:%d\n", ile2);
            }
                
        }
    if(ile1 == 0 && ile2 == 0)
        return 1;
    
    else if(ile1 == ile2)
        return 1;
    
    else if(ile1 > ile2)
        return 2;
}

int ocena(Plansza m)
{
    int gracz = czyj_ruch(m);
    int przeciwnik;
    if(czyj_ruch(m)==1)
    przeciwnik = 2;
    else
    przeciwnik=1;

    for (int x=0; x<W; x++)
        for (int y=0; y<K; y++)
        {
            int licznikx1=0, liczniky1=0, licznikx2=0, liczniky2=0, licznikxy1=0, licznikyx1=0, licznikxy2=0, licznikyx2=0;
            for (int i=0; i<ILE; i++)
            {
                        if(m.elementy[x][y] == gracz)
                        {
                            if((m.elementy[x][y] == m.elementy[x][y+i])&&(y+i<K))
                            {
                                liczniky1++;
                                if(liczniky1==ILE&&i==ILE-1)
                                return 100;
                            }

                            if((m.elementy[x][y] == m.elementy[x+i][y]) && (x+i<W) )
                            {
                                licznikx1++;
                                if(licznikx1==ILE&&i==ILE-1)
                                return 100;
                            }
                            
                            if((m.elementy[x][y] == m.elementy[x+i][y+i])  && (x+i<W) && (y+i<K))
                            {
                                licznikxy1++;
                                if(licznikxy1==ILE&&i==ILE-1)
                                return 100;
                            }
                            if((m.elementy[x][y] == m.elementy[x+i][y-i]) && (x+i<W)  && (y-i>=0))
                            {
                                licznikyx1++;
                                if(licznikyx1==ILE&&i==ILE-1)
                                return 100;
                            }

                        }

                        if(m.elementy[x][y] == przeciwnik)
                        {
                            if((m.elementy[x][y] == m.elementy[x][y+i]) && (y+i<K) )
                            {
                                liczniky2++;
                                if(liczniky2==ILE&&i==ILE-1)
                                return -100;
                            }

                            if((m.elementy[x][y] == m.elementy[x+i][y]) && (x+i<W) )
                            {
                                licznikx2++;
                                if(licznikx2==ILE&&i==ILE-1)
                                return -100;
                            }
                            if((m.elementy[x][y] == m.elementy[x+i][y+i])  && (x+i<W) && (y+i<K))
                            {
                                licznikxy2++;
                                if(licznikxy2==ILE&&i==ILE-1)
                                return -100;
                            }
                            if((m.elementy[x][y] == m.elementy[x+i][y-i]) && (x+i<W)  && (y-i>=0))
                            {
                                licznikyx2++;
                                if(licznikyx2==ILE&&i==ILE-1)
                                return -100;
                            }

                        }
                        if((x>=W-1)&&(y>=K-1))
                        return 0;
            }
        }
}

Ruchy *mozliwosci(Plansza m)
{
    Ruchy *glowa =NULL;
    Ruchy*last = NULL;
    for(int i=0; i<K; i++)
    {

        if(m.elementy[0][i]==0)
        {
            Ruchy *o = (Ruchy *)malloc(sizeof(Ruchy));
            o->next = NULL;
            if(last==NULL)
            glowa=o;
            else
                last->next = o;
            last = o;
            o->kolumna = i;
        }
    }
    return glowa;
}
void listuj_ruchy(Ruchy *glowa) {
    while (glowa != NULL) {
        printf("%d, ", glowa->kolumna);
        glowa = glowa->next;
    }
    printf("\n");
}

Plansza wykonaj_ruch(Plansza m, int kolumna)
{
  	Plansza n;
  	for(int i=0;i<W;i++)
      	for(int j=0;j<K;j++)
          	n.elementy[i][j]=m.elementy[i][j];
  	
    int i=W-1;
  	while(n.elementy[i][kolumna]!=0)
	{
        i--;
    }
  	n.elementy[i][kolumna]=czyj_ruch(m);
    return n;
}

int negmax( Plansza* sz, int glebokosc, int alfa, int beta)
{
if (!glebokosc){
    return ocena(*sz);
}

    int nowaocena;
    int ocenawezla=-100;//(-PRZEGRANA)
    int i=0;
for( Ruchy *lr = mozliwosci(*sz), *ptr = lr; lr; i++, lr = lr->next, free(ptr), ptr = lr)
{
    Plansza dziecko = wykonaj_ruch(*sz, ptr->kolumna); //plansza po ruchu to dziecko
    nowaocena = -negmax(&dziecko, glebokosc - 1, -beta, -alfa);
    // printf("iteracja:%d\n",i);
    // printf("nowaocena1:%d\n",nowaocena);
    // printf("ocenawezla1:%d\n",ocenawezla);
    if (nowaocena > ocenawezla)
    ocenawezla = nowaocena;
    if (ocenawezla > alfa)
    alfa = ocenawezla;
    // printf("alfa:%d\n\n",alfa);
    if (alfa > beta)
    {
        zwolnij_ruchy(lr);
        break;
    }
}
    return ocenawezla;
}

int najlepszy_ruch(Plansza* sz, int glebokosc, int alfa, int beta)
{
int nowaocena[K];
int naj_kolumna[K];//RUCH
int i=-1;
int j=0;//RUCH
int maxocena;
    for( Ruchy *lr = mozliwosci(*sz); lr; lr = lr->next)
    {
        printf("ruchy:%d ",lr->kolumna);
        i++;
        Plansza dziecko = wykonaj_ruch(*sz, lr->kolumna);
        nowaocena[i] = -negmax(&dziecko, glebokosc, alfa, beta);
        naj_kolumna[i]=lr->kolumna;
        printf("naj_kolumna[%d]=%d\n",i,naj_kolumna[i]);
    }
        printf("\ni:%d\n",i);
        j=i;
        maxocena=nowaocena[i];
        //ocena ruchu[2] -> 1,2 ==1
        //ocena ruchu[1] -> 1,0 ==-1
        //ocena ruchu[0] -> 0,0 ==15
        for(i;i>=0;i--)
        {
            printf("nowaocena[%d]=%d\n",i,nowaocena[i]);
            if(maxocena<nowaocena[i])
            {
                printf("i=%d\n",i);
                printf("j=%d == naj_k[%d]=%d\n",j,i,naj_kolumna[i]);
                j=naj_kolumna[i];
                maxocena=nowaocena[i];
                printf("wieksze j: %d\n",j);
            }
        }
    return j;
}
void reset(char nazwa[], Plansza *m)
{
    for(int i=0;i<W;i++)
    for(int j=0;j<K;j++)
    m->elementy[i][j]=0;

    zapisz_macierz(nazwa,m);
}

int main(int argc, char *argv[])
{
    FILE *fin=fopen("A.txt","r");
    Plansza *m=wczytaj(fin);
    wypisz(m);
    int a=0;
    // listuj_ruchy(mozliwosci(*m));
    // int i = ocena(*m);
    // wypisz(*m);
    // printf("ocena:%d\n", i);
    if((argc==2)&&strcmp(argv[1],"reset")==0)
    reset("A.txt",m);
    while (ocena(*m)==0)
    {
        // m=wczytaj(fin);
        int ruch =najlepszy_ruch(m, 7, -500, 500);
        printf("najlepszy ruch:%d\n",ruch);
        *m=wykonaj_ruch(*m,ruch);
        printf("%d",ocena(*m));
        wypisz(m);
        if(ocena(*m)!=0)
        break;
        printf("Twój ruch:");
        scanf("%d",&a);
        *m=wykonaj_ruch(*m,a);
        printf("%d",ocena(*m));
        zapisz_macierz("A.txt",m);
    }
    wypisz(m);
    int wygrana;
    if(czyj_ruch(*m)==1)
    wygrana=2;
    else
    wygrana=1;
    printf("WYGRANA %d\n",wygrana);
    // zwolnij_macierz(m);
}
