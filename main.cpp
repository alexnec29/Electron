#include <graphics.h>
#include <winbgim.h>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <conio.h>
#include <tchar.h>
#pragma comment(lib, "kernel32.lib")

using namespace std;

#define AMPLOP 1
#define BATERIE 2
#define CONDENS 3
#define DIODA 4
#define NOD 5
#define POLARIZ 6
#define REZIST 7
#define SERVOMOT 8
#define SINU 9
#define STOP 10
#define TRANZNPN 11
#define TRANZPNP 12
#define ZENNER 13

#define DREAPTA 0

#define ecranCircuit 1
#define meniuPrincipal 2
#define meniuCustomizare 3
#define meniuCustomizare2 4

bool executatInput=false;
int butonApasat=0;
int meniu=meniuPrincipal;
int componentaSelectata=0;
int xglobal=0, yglobal=0;

//culori
int culoareMeniu=CYAN;
int culoareText=WHITE;
int culoareFundal=BLACK;
int culoareLegaturi=WHITE;
int culoareComponente=WHITE;
int culoareFundalText=CYAN;
int stilFundal=1;
int stilMeniu=1;
int culoareBackg=1;

struct desen{
    int nr_operatii, nr_noduri;
    char tip_linie[25];
    double coordonate[25][5];
    double noduri[5][3];
    float zoom;
};

desen tip_desen[25];

struct componenta{
    int tipComponenta;
    int sensRotatie;
    char numeComponenta[100],rezistenta[100];
    char filename[50];
    float zoom;
    bool plasat;
    bool legat[4];
    double xcomponenta,ycomponenta;
    double xnod[4], ynod[4];
};

struct legatura{
    int componenta1;
    int componenta2;
    int nod1, nod2;
    double x1;
    double x2;
    double y1;
    double y2;
    int tipLeg;
};

legatura leg[100];
componenta comp[100];

int numarComponente=0,numarLegaturi=0,tipLegaturaTrasare=0;

double max(double a, double b)
{
    if(a>=b) return a;
    else return b;
}
double min(double a, double b)
{
    if(a>=b) return b;
    else return a;
}
double punctrotit(double cx, double cy, int angle, double px, double py, int pct) { //Alex
    // Converteste unghiul din grade in radiani
    float radian = angle * M_PI / 180.0;

    // Calculeaza sin si cos de unghi
    float s = sin(radian);
    float c = cos(radian);

    // Translateaza punctul inapoi la origine:
    px -= cx;
    py -= cy;

    // roteste punctul folosind formula de rotatie
    double xnew = px * c - py * s;
    double ynew = px * s + py * c;

    // Translateaza punctul inapoi
    xnew += cx;
    ynew += cy;

    if(pct == 1) return xnew;
    else if(pct == 2) return ynew;
}

void adaugareValoareRezistenta(int numarComponenta) //Alex
{
    if(numarComponenta==0) return;
    char Enter = 13, BackSpace = 8, Escape = 27;
    char MultimeDeCaractereAcceptabile[200] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz ";
    char Sinitial[200], tasta;
    char Ss[200], mesajs[200];
    int x=300,y=700;
    strcpy(Sinitial,comp[numarComponenta].rezistenta);
    settextstyle(DEFAULT_FONT, 0, 3);
    setcolor(WHITE);
    setbkcolor(culoareFundal);
    settextjustify(LEFT_TEXT,TOP_TEXT);
    strcpy(mesajs,"Valoare");
    strcat(mesajs,":");
    outtextxy(x,y,mesajs);
    x=x+textwidth(mesajs);
    strcpy(comp[numarComponenta].rezistenta,"");
    do
    {
        tasta=getch();
        if (tasta==0)
        {
            tasta=getch();
            Beep(200,200);
        }
        else if (strchr(MultimeDeCaractereAcceptabile,tasta))
        {
            // stergem fostul sir
            strcpy(Ss,comp[numarComponenta].rezistenta);
            strcat(Ss,"_ ");
            setcolor(BLACK);
            outtextxy(x,y,Ss);
            char tt2[2];
            tt2[0]=tasta;
            tt2[1]=0;
            strcat(comp[numarComponenta].rezistenta,tt2);
            // afisam pe cel nou }
            strcpy(Ss,comp[numarComponenta].rezistenta);
            strcat(Ss,"_ ");
            setcolor(WHITE);
            outtextxy(x,y,Ss);
        }
        else if (tasta == BackSpace)
        {
            if (!strcmp(comp[numarComponenta].rezistenta,""))
                Beep(200,200);
            else
            {
                strcpy(Ss,comp[numarComponenta].rezistenta);
                strcat(Ss,"_");
                setcolor(BLACK);
                outtextxy(x,y,Ss);
                setcolor(WHITE);
                comp[numarComponenta].rezistenta[strlen(comp[numarComponenta].rezistenta)-1]=0;
                strcpy(Ss,comp[numarComponenta].rezistenta);
                strcat(Ss,"_ ");
                outtextxy(x,y,Ss);
            }
        }
        else if (tasta!=Enter && tasta!=Escape)
            Beep(200,200);
    }
    while (tasta!=Enter && tasta!=Escape);
    if (tasta == Escape)
        strcpy(comp[numarComponenta].rezistenta,Sinitial);
    setcolor(BLACK);
    strcpy(Ss,comp[numarComponenta].rezistenta);
    strcat(Ss," ");
    outtextxy(x,y,Ss);
    setcolor(culoareFundal);
    outtextxy(x,y,comp[numarComponenta].rezistenta);
    if(!strcmp(comp[numarComponenta].rezistenta,"")) strcpy(comp[numarComponenta].rezistenta,"-");
}
void adaugareNumeComponenta(int numarComponenta) //Alex
{
    if(numarComponenta==0) return;
    char Enter = 13, BackSpace = 8, Escape = 27;
    char MultimeDeCaractereAcceptabile[200] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz ";
    char Sinitial[200], tasta;
    char Ss[200], mesajs[200];
    int x=300,y=700;
    strcpy(Sinitial,comp[numarComponenta].numeComponenta);
    settextstyle(DEFAULT_FONT, 0, 3);
    setbkcolor(culoareFundal);
    setcolor(WHITE);
    settextjustify(LEFT_TEXT,TOP_TEXT);
    strcpy(mesajs,"Nume componenta");
    strcat(mesajs,":");
    outtextxy(x,y,mesajs);
    x=x+textwidth(mesajs);
    strcpy(comp[numarComponenta].numeComponenta,"");
    do
    {
        tasta=getch();
        if (tasta==0)
        {
            tasta=getch();
            Beep(200,200);
        }
        else if (strchr(MultimeDeCaractereAcceptabile,tasta))
        {
            // stergem fostul sir
            strcpy(Ss,comp[numarComponenta].numeComponenta);
            strcat(Ss,"_ ");
            setcolor(BLACK);
            outtextxy(x,y,Ss);
            char tt2[2];
            tt2[0]=tasta;
            tt2[1]=0;
            strcat(comp[numarComponenta].numeComponenta,tt2);
            // afisam pe cel nou }
            strcpy(Ss,comp[numarComponenta].numeComponenta);
            strcat(Ss,"_ ");
            setcolor(WHITE);
            outtextxy(x,y,Ss);
        }
        else if (tasta == BackSpace)
        {
            if (!strcmp(comp[numarComponenta].numeComponenta,""))
                Beep(200,200);
            else
            {
                strcpy(Ss,comp[numarComponenta].numeComponenta);
                strcat(Ss,"_");
                setcolor(BLACK);
                outtextxy(x,y,Ss);
                setcolor(WHITE);
                comp[numarComponenta].numeComponenta[strlen(comp[numarComponenta].numeComponenta)-1]=0;
                strcpy(Ss,comp[numarComponenta].numeComponenta);
                strcat(Ss,"_ ");
                outtextxy(x,y,Ss);
            }
        }
        else if (tasta!=Enter && tasta!=Escape)
            Beep(200,200);
    }
    while (tasta!=Enter && tasta!=Escape);
    if (tasta == Escape)
        strcpy(comp[numarComponenta].numeComponenta,Sinitial);
    setcolor(BLACK);
    strcpy(Ss,comp[numarComponenta].numeComponenta);
    strcat(Ss," ");
    outtextxy(x,y,Ss);
    setcolor(culoareFundal);
    setfillstyle(SOLID_FILL,culoareFundal);
    outtextxy(x,y,comp[numarComponenta].numeComponenta);
    bar(60,y,x+1000,y+100);
    if(!strcmp(comp[numarComponenta].numeComponenta,"")) strcpy(comp[numarComponenta].numeComponenta,"-");
}
void initializare()  //Tudor
{
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile(_T("componente\\*"), &findFileData);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        // Handle the error. You might want to add more error handling.
        return;
    }

    do
    {
        if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        {
            ifstream fin;
            TCHAR filename[MAX_PATH];
            _stprintf(filename, _T("componente\\%s"), findFileData.cFileName);
            fin.open(filename);
                int index;
                fin >> index;
                fin >> tip_desen[index].nr_noduri;
                for(int j = 1; j <= tip_desen[index].nr_noduri; j++)
                {
                    fin >> tip_desen[index].noduri[j][1] >> tip_desen[index].noduri[j][2];
                }
                char descriere[20];
                fin >> descriere;
                fin >> tip_desen[index].nr_operatii;
                for(int j = 1; j <= tip_desen[index].nr_operatii; j++)
                {
                    fin >> tip_desen[index].tip_linie[j];
                    for(int k = 1; k <= 4; k++)
                        fin >> tip_desen[index].coordonate[j][k];
                }
                fin >> tip_desen[index].zoom;
                fin.close();
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
}
void stergereLegatura(int i)//Tudor
{
    if(leg[i].nod1==1) comp[leg[i].componenta1].legat[1]=false;
    if(leg[i].nod1==2) comp[leg[i].componenta1].legat[2]=false;
    if(leg[i].nod1==3) comp[leg[i].componenta1].legat[3]=false;
    if(leg[i].nod2==1) comp[leg[i].componenta2].legat[1]=false;
    if(leg[i].nod2==2) comp[leg[i].componenta2].legat[2]=false;
    if(leg[i].nod2==3) comp[leg[i].componenta2].legat[3]=false;
    for(int j=i;j<numarLegaturi;j++)
    {
        leg[j]=leg[j+1];
    }
    numarLegaturi--;
}
void stergereComponenta(int i)//Tudor
{
    for(int j=i;j<numarComponente;j++)
    {
        comp[j]=comp[j+1];
    }
    for(int j=1;j<=numarLegaturi;j++)
    {
        if(leg[j].componenta1>i) leg[j].componenta1--;
        if(leg[j].componenta2>i) leg[j].componenta2--;
    }
    numarComponente--;
}
void resetareProiect()//Tudor
{
    for(int i=numarLegaturi;i>=1;i--)
    {
        stergereLegatura(i);
    }
    for(int i = numarComponente; i >= 1; i--)
    {
        stergereComponenta(i);
    }
}
void rotireComponenta(int i, int angle)//Alex
{
    comp[i].sensRotatie=(comp[i].sensRotatie+angle)%360;
    for(int j=1;j<=tip_desen[comp[i].tipComponenta].nr_noduri;j++)
    {
        double xnou,ynou;
        xnou=punctrotit(comp[i].xcomponenta,comp[i].ycomponenta,angle,comp[i].xnod[j],comp[i].ynod[j],1);
        ynou=punctrotit(comp[i].xcomponenta,comp[i].ycomponenta,angle,comp[i].xnod[j],comp[i].ynod[j],2);
        comp[i].xnod[j]=xnou;
        comp[i].ynod[j]=ynou;
    }
    for(int j=1;j<=numarLegaturi;j++)
    {
        if(leg[j].componenta1==i)
        {
            double xnou,ynou;
            xnou=punctrotit(comp[i].xcomponenta,comp[i].ycomponenta,angle,leg[j].x1,leg[j].y1,1);
            ynou=punctrotit(comp[i].xcomponenta,comp[i].ycomponenta,angle,leg[j].x1,leg[j].y1,2);
            leg[j].x1=xnou;
            leg[j].y1=ynou;
        }
        if(leg[j].componenta2==i)
        {
            double xnou,ynou;
            xnou=punctrotit(comp[i].xcomponenta,comp[i].ycomponenta,angle,leg[j].x2,leg[j].y2,1);
            ynou=punctrotit(comp[i].xcomponenta,comp[i].ycomponenta,angle,leg[j].x2,leg[j].y2,2);
            leg[j].x2=xnou;
            leg[j].y2=ynou;
        }
    }
}
void adaugareCablu(int numarNod1, int numarComponenta1, int numarNod2, int numarComponenta2)//Alex
{
    if(comp[numarComponenta1].legat[1]==FALSE&&numarNod1==1||comp[numarComponenta1].tipComponenta==NOD||comp[numarComponenta1].legat[2]==FALSE&&numarNod1==2||comp[numarComponenta1].legat[3]==FALSE&&numarNod1==3)
    if(comp[numarComponenta2].legat[1]==FALSE&&numarNod2==1||comp[numarComponenta2].tipComponenta==NOD||comp[numarComponenta2].legat[2]==FALSE&&numarNod2==2||comp[numarComponenta2].legat[3]==FALSE&&numarNod2==3)
    {
        numarLegaturi++;
        leg[numarLegaturi].nod1=numarNod1;
        leg[numarLegaturi].nod2=numarNod2;
        leg[numarLegaturi].componenta1=numarComponenta1;
        leg[numarLegaturi].componenta2=numarComponenta2;
        leg[numarLegaturi].tipLeg=tipLegaturaTrasare;
        if(numarNod1==1) comp[numarComponenta1].legat[1]=TRUE;
        if(numarNod1==2) comp[numarComponenta1].legat[2]=TRUE;
        if(numarNod1==3) comp[numarComponenta1].legat[3]=TRUE;
        if(numarNod2==1) comp[numarComponenta2].legat[1]=TRUE;
        if(numarNod2==2) comp[numarComponenta2].legat[2]=TRUE;
        if(numarNod2==3) comp[numarComponenta2].legat[3]=TRUE;
        leg[numarLegaturi].x1=comp[leg[numarLegaturi].componenta1].xnod[leg[numarLegaturi].nod1];
        leg[numarLegaturi].y1=comp[leg[numarLegaturi].componenta1].ynod[leg[numarLegaturi].nod1];
        leg[numarLegaturi].x2=comp[leg[numarLegaturi].componenta2].xnod[leg[numarLegaturi].nod2];
        leg[numarLegaturi].y2=comp[leg[numarLegaturi].componenta2].ynod[leg[numarLegaturi].nod2];
    }
}
void trasareCablu(int x1, int y1, int x2, int y2, int c, int tipLeg)//Alex
{
    setcolor(c);
    if(tipLeg==0)
    {
        line(x1,max(y1,100),(x1+x2)/2,max(y1,100));
        line((x1+x2)/2,max(y1,100),(x1+x2)/2,max(y2,100));
        line((x1+x2)/2,max(y2,100),x2,max(y2,100));
    }
    else if(tipLeg==1)
    {
        if(x1<x2)
        {
            line(x1,max(y1,100),x1,max(y2,100));
            line(x1,max(y2,100),x2,max(y2,100));
        }
        else
        {
            line(x1,max(y1,100),x2,max(y1,100));
            line(x2,max(y1,100),x2,max(y2,100));
        }
    }
    else if(tipLeg==2)
    {
        if(x1<x2)
        {
            line(x1,max(y1,100),x2,max(y1,100));
            line(x2,max(y1,100),x2,max(y2,100));
        }
        else
        {
            line(x1,max(y1,100),x1,max(y2,100));
            line(x1,max(y2,100),x2,max(y2,100));
        }
    }

}
void desenareComponenta(int i, int x, int y, int c)//Tudor
{
    for(int j = 1; j <= tip_desen[comp[i].tipComponenta].nr_operatii; j++)
    {
        setcolor(c);
        int c1=tip_desen[comp[i].tipComponenta].coordonate[j][1]*comp[i].zoom;
        int c2=tip_desen[comp[i].tipComponenta].coordonate[j][2]*comp[i].zoom;
        int c3=tip_desen[comp[i].tipComponenta].coordonate[j][3]*comp[i].zoom;
        int c4=tip_desen[comp[i].tipComponenta].coordonate[j][4]*comp[i].zoom;
        if(tip_desen[comp[i].tipComponenta].tip_linie[j] == 'L')
        {
            int x1=x + c1,y1=y+c2,x2=x+c3,y2=y+c4;
            line(punctrotit(x,y,comp[i].sensRotatie,x1,y1,1), max(punctrotit(x,y,comp[i].sensRotatie,x1,y1,2),100), punctrotit(x,y,comp[i].sensRotatie,x2,y2,1), max(punctrotit(x,y,comp[i].sensRotatie,x2,y2,2),100));
        }
        else if(tip_desen[comp[i].tipComponenta].tip_linie[j] == 'R')
        {
            int x1=x + c1,y1=y+c2,x2=x+c3,y2=y+c4;
            line(punctrotit(x,y,comp[i].sensRotatie,x1,y1,1), max(punctrotit(x,y,comp[i].sensRotatie,x1,y1,2),100), punctrotit(x,y,-comp[i].sensRotatie,x2,y2,1), max(punctrotit(x,y,-comp[i].sensRotatie,x1,y1,2),100));
            line(punctrotit(x,y,-comp[i].sensRotatie,x2,y2,1), max(punctrotit(x,y,-comp[i].sensRotatie,x1,y1,2),100), punctrotit(x,y,comp[i].sensRotatie,x2,y2,1), max(punctrotit(x,y,comp[i].sensRotatie,x2,y2,2),100));
            line(punctrotit(x,y,comp[i].sensRotatie,x2,y2,1), max(punctrotit(x,y,comp[i].sensRotatie,x2,y2,2),100), punctrotit(x,y,-comp[i].sensRotatie,x1,y1,1), max(punctrotit(x,y,-comp[i].sensRotatie,x2,y2,2),100));
            line(punctrotit(x,y,-comp[i].sensRotatie,x1,y1,1), max(punctrotit(x,y,-comp[i].sensRotatie,x2,y2,2),100), punctrotit(x,y,comp[i].sensRotatie,x1,y1,1), max(punctrotit(x,y,comp[i].sensRotatie,x1,y1,2),100));
        }
        else if(tip_desen[comp[i].tipComponenta].tip_linie[j] == 'O')
        {
            int x1=c3,y1=c4;
            if(comp[i].ycomponenta>100) ellipse(x,y,0,360,x1,y1);
        }
        else if(tip_desen[comp[i].tipComponenta].tip_linie[j] == 'C')
        {
            arc(x , y, 0 , 360, c1);
        }
        else if(tip_desen[comp[i].tipComponenta].tip_linie[j] == 'A')
        {
            arc(punctrotit(x,y,comp[i].sensRotatie,x+c1,y+c2,1), punctrotit(x,y,comp[i].sensRotatie,x+c1,y+c2,2), 270-comp[i].sensRotatie, 90-comp[i].sensRotatie, c3);
        }
    }
    delay(0);
}
void adaugareComponenta(int tipComponenta, int x, int y)//Tudor
{
    numarComponente++;//adaugarea componentei in vector
    comp[numarComponente].legat[1] = false;
    comp[numarComponente].legat[2] = false;
    comp[numarComponente].legat[3] = false;
    comp[numarComponente].plasat=false;

    if(tipComponenta==AMPLOP)
    {
        comp[numarComponente].tipComponenta=AMPLOP;
        comp[numarComponente].zoom=8;
        comp[numarComponente].sensRotatie=DREAPTA;
        strcpy(comp[numarComponente].numeComponenta,"Amplop");
    }
    if(tipComponenta==BATERIE)
    {
        comp[numarComponente].tipComponenta=BATERIE;
        comp[numarComponente].sensRotatie=DREAPTA;
        comp[numarComponente].zoom = 10;
        strcpy(comp[numarComponente].numeComponenta,"Baterie");
    }
    if(tipComponenta==CONDENS)
    {
        comp[numarComponente].tipComponenta=CONDENS;
        comp[numarComponente].sensRotatie=DREAPTA;
        comp[numarComponente].zoom = 10;
        strcpy(comp[numarComponente].numeComponenta,"Condens");
    }
    if(tipComponenta==DIODA)
    {
        comp[numarComponente].tipComponenta=DIODA;
        comp[numarComponente].sensRotatie=DREAPTA;
        comp[numarComponente].zoom = 10;
        strcpy(comp[numarComponente].numeComponenta,"Dioda");
    }
    if(tipComponenta==NOD)
    {
        comp[numarComponente].tipComponenta=NOD;
        comp[numarComponente].sensRotatie=DREAPTA;
        comp[numarComponente].zoom = 10;
        strcpy(comp[numarComponente].numeComponenta,"Nod");
    }
    if(tipComponenta==POLARIZ)
    {
        comp[numarComponente].tipComponenta=POLARIZ;
        comp[numarComponente].sensRotatie=DREAPTA;
        comp[numarComponente].zoom = 10;
        strcpy(comp[numarComponente].numeComponenta,"Polariz");
    }
    if(tipComponenta==REZIST)
    {
        comp[numarComponente].tipComponenta=REZIST;
        comp[numarComponente].sensRotatie=DREAPTA;
        comp[numarComponente].zoom = 10;
        strcpy(comp[numarComponente].numeComponenta,"Rezist");
    }
    if(tipComponenta==SERVOMOT)
    {
        comp[numarComponente].tipComponenta=SERVOMOT;
        comp[numarComponente].sensRotatie=DREAPTA;
        comp[numarComponente].zoom = 8;
        strcpy(comp[numarComponente].numeComponenta,"Servomot");
    }
    if(tipComponenta==SINU)
    {
        comp[numarComponente].tipComponenta=SINU;
        comp[numarComponente].sensRotatie=DREAPTA;
        comp[numarComponente].zoom = 8;
        comp[numarComponente].plasat = false;
        strcpy(comp[numarComponente].numeComponenta,"Sinu");
    }
    if(tipComponenta==STOP)
    {
        comp[numarComponente].tipComponenta=STOP;
        comp[numarComponente].sensRotatie=DREAPTA;
        comp[numarComponente].zoom = 8;
        comp[numarComponente].plasat = false;
        strcpy(comp[numarComponente].numeComponenta,"Stop");
    }
    if(tipComponenta==TRANZNPN)
    {
        comp[numarComponente].tipComponenta=TRANZNPN;
        comp[numarComponente].sensRotatie=DREAPTA;
        comp[numarComponente].zoom = 8;
        comp[numarComponente].plasat = false;
        strcpy(comp[numarComponente].numeComponenta,"Tranznpn");
    }
    if(tipComponenta==TRANZPNP)
    {
        comp[numarComponente].tipComponenta=TRANZPNP;
        comp[numarComponente].sensRotatie=DREAPTA;
        comp[numarComponente].zoom = 8;
        comp[numarComponente].plasat = false;
        strcpy(comp[numarComponente].numeComponenta,"Tranzpnp");
    }
    if(tipComponenta==ZENNER)
    {
        comp[numarComponente].tipComponenta=ZENNER;
        comp[numarComponente].sensRotatie=DREAPTA;
        comp[numarComponente].zoom = 8;
        comp[numarComponente].plasat = false;
        strcpy(comp[numarComponente].numeComponenta,"Zenner");
    }
}

void dragEcran()//tragerea ecranului //Comun
{
    int x=mousex(), y=mousey();
    int xprev=mousex(),yprev=mousey();
    setfillstyle(SOLID_FILL,culoareFundal);
            while(executatInput==false)
            {
                x=mousex(),y=mousey();
                xglobal=xglobal+(x-xprev);
                yglobal=yglobal+(y-yprev);
                for(int i=1;i<=numarComponente;i++)
                {
                    comp[i].xcomponenta=comp[i].xcomponenta+(x-xprev);
                    comp[i].ycomponenta=comp[i].ycomponenta+(y-yprev);
                    for(int j=1;j<=tip_desen[comp[i].tipComponenta].nr_noduri;j++)
                    {
                        comp[i].xnod[j]=comp[i].xnod[j]+(x-xprev);
                        comp[i].ynod[j]=comp[i].ynod[j]+(y-yprev);
                    }
                    desenareComponenta(i,comp[i].xcomponenta,comp[i].ycomponenta,culoareComponente);
                }
                for(int i=1;i<=numarLegaturi;i++)
                {
                    trasareCablu(leg[i].x1,leg[i].y1,leg[i].x2,leg[i].y2,culoareLegaturi,leg[i].tipLeg);
                    leg[i].x1=leg[i].x1+(x-xprev);
                    leg[i].x2=leg[i].x2+(x-xprev);
                    leg[i].y1=leg[i].y1+(y-yprev);
                    leg[i].y2=leg[i].y2+(y-yprev);
                }
                delay(15);
                bar(0,100,1496,800);
                if(ismouseclick(WM_LBUTTONDOWN))
                {
                    clearmouseclick(WM_LBUTTONDOWN);
                    executatInput=true;
                }
                xprev=x;
                yprev=y;
            }
}
bool verifCircuit() //Tudor
{
    for(int i=1;i<=numarComponente;i++)
    {
        for(int j=1;j<=tip_desen[comp[i].tipComponenta].nr_noduri;j++)
        {

            if(comp[i].legat[j]==false)
            {
                return false;
            }
        }
    }
    return true;
}
void salvareCircuit()//Tudor
{
    bar(0, 0, 1496, 800);
    settextstyle(BOLD_FONT, HORIZ_DIR, 9);
    setcolor(culoareText);
    outtextxy(400, 200, "SAVING");

    OPENFILENAME ofn;
    char szFileName[MAX_PATH] = "";

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = szFileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrTitle = "Save Circuit As";
    ofn.Flags = OFN_OVERWRITEPROMPT;

    if (GetSaveFileNameA(&ofn)) {  // Corrected to GetSaveFileNameA for ANSI version
        ofstream outFile(ofn.lpstrFile);

        if (!outFile.is_open()) {
            cerr << "Error: Unable to open file " << ofn.lpstrFile << " for writing." << endl;
            return;
        }

        // Write component information to the file
        outFile<<numarComponente<<" "<<numarLegaturi<<"\n\n";
        for (size_t i = 1; i <= numarComponente; i++) {
            outFile << comp[i].tipComponenta << "\n";
            outFile << comp[i].sensRotatie << " ";
            outFile << comp[i].zoom << " ";
            outFile << comp[i].plasat << "\n";
            outFile << comp[i].legat[1] << " " << comp[i].legat[2] << " " << comp[i].legat[3] << "\n";
            outFile << comp[i].xcomponenta << " " << comp[i].ycomponenta << "\n";
            outFile << comp[i].xnod[1] << " " << comp[i].ynod[1] << " " << comp[i].xnod[2] << " "
                    << comp[i].ynod[2] << " " << comp[i].xnod[3] << " " << comp[i].ynod[3] << "\n";
            outFile << comp[i].numeComponenta<<"\n";
            outFile << comp[i].rezistenta<<"\n\n";
        }

        // Write connection information to the file
        for (size_t i = 1; i <= numarLegaturi; ++i) {
            outFile << leg[i].nod1<<" " << leg[i].componenta1 << "\n";
            outFile << leg[i].nod2<<" " << leg[i].componenta2 << "\n";
            outFile << leg[i].x1 << " " << leg[i].y1 << " " << leg[i].x2 << " "
                    << leg[i].y2<<"\n"<<leg[i].tipLeg<<"\n\n";
        }
        outFile.close();
        outtextxy(400, 200, "CIRCUIT SAVED");
        delay(1000);
    }
}
void incarcareCircuit()//Tudor
{
    setfillstyle(SOLID_FILL, culoareMeniu);
    bar(0, 0, 1496, 800);
    settextstyle(BOLD_FONT, HORIZ_DIR, 9);
    setcolor(culoareText);
    outtextxy(400, 200, "LOADING");

    OPENFILENAME ofn;
    char szFileName[MAX_PATH] = "";

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = szFileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrTitle = "Load Circuit File";
    ofn.Flags = OFN_FILEMUSTEXIST;

    if (GetOpenFileNameA(&ofn)) {
        ifstream inFile(ofn.lpstrFile);
        if (!inFile.is_open()) {
            cerr << "Error: Unable to open file " << ofn.lpstrFile << " for reading." << endl;
            return;
        }

        // Clear any existing components and connections
        numarComponente = 0;
        numarLegaturi = 0;
        string currentLine;

        // Read the total number of components and connections
        inFile >> numarComponente >> numarLegaturi;
        // Read component information from the file
        for (int i = 1; i <= numarComponente; i++) {
        inFile >> comp[i].tipComponenta;
        inFile >> comp[i].sensRotatie >> comp[i].zoom >> comp[i].plasat;
        inFile >> comp[i].legat[1] >> comp[i].legat[2] >> comp[i].legat[3];
        inFile >> comp[i].xcomponenta >> comp[i].ycomponenta;
        inFile >> comp[i].xnod[1] >> comp[i].ynod[1] >> comp[i].xnod[2] >> comp[i].ynod[2] >> comp[i].xnod[3] >> comp[i].ynod[3];
        if(comp[i].tipComponenta==AMPLOP) strcpy(comp[i].filename,"AMPLOP.PS");
        if(comp[i].tipComponenta==BATERIE) strcpy(comp[i].filename,"BATERIE.PS");
        if(comp[i].tipComponenta==CONDENS) strcpy(comp[i].filename,"CONDENS.PS");
        if(comp[i].tipComponenta==DIODA) strcpy(comp[i].filename,"DIODA.PS");
        if(comp[i].tipComponenta==NOD) strcpy(comp[i].filename,"NOD.PS");
        if(comp[i].tipComponenta==POLARIZ) strcpy(comp[i].filename,"POLARIZ.PS");
        if(comp[i].tipComponenta==REZIST) strcpy(comp[i].filename,"REZIST.PS");
        if(comp[i].tipComponenta==SERVOMOT) strcpy(comp[i].filename,"SERVOMOT.PS");
        if(comp[i].tipComponenta==SINU) strcpy(comp[i].filename,"SINU.PS");
        if(comp[i].tipComponenta==STOP) strcpy(comp[i].filename,"STOP.PS");
        if(comp[i].tipComponenta==TRANZNPN) strcpy(comp[i].filename,"TRANZNPN.PS");
        if(comp[i].tipComponenta==TRANZPNP) strcpy(comp[i].filename,"TRANZPNP.PS");
        if(comp[i].tipComponenta==ZENNER) strcpy(comp[i].filename,"ZENNER.PS");

            string componentName;
            getline(inFile >> ws, componentName);
            strcpy(comp[i].numeComponenta, componentName.c_str());
             string rezistentaName;
            getline(inFile >> ws, rezistentaName);
            strcpy(comp[i].rezistenta, rezistentaName.c_str());
        }
        // Read connection information from the file
        for (int i = 1; i <= numarLegaturi; ++i) {
            inFile >> leg[i].nod1
                    >> leg[i].componenta1
                    >> leg[i].nod2
                    >> leg[i].componenta2
                    >> leg[i].x1 >> leg[i].y1
                    >> leg[i].x2 >> leg[i].y2
                    >> leg[i].tipLeg;
        }
        inFile.close();
        outtextxy(400, 200, "CIRCUIT LOADED");
        delay(1000);
    }
}
void meniuConfirmare() //Tudor
{
    settextstyle(8,HORIZ_DIR,3);
    setcolor(culoareText);
    setbkcolor(culoareMeniu);

    outtextxy(0,101,"ESTI SIGUR?");
    setbkcolor(GREEN);
    outtextxy(textwidth("ESTI SIGUR?")+10,101,"DA");
    setbkcolor(RED);
    outtextxy(textwidth("ESTI SIGUR?")+textwidth("DA")+15,101,"NU");
    while(executatInput==false)
    {
        int x=mousex(),y=mousey();
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            clearmouseclick(WM_LBUTTONDOWN);
            if(x>=textwidth("ESTI SIGUR?")+10&&x<=textwidth("ESTI SIGUR?")+10+textwidth("DA")&&y>=101&&y<=101+textheight("DA"))
            {
                resetareProiect();
                executatInput=true;
            }
            else if(x>=textwidth("ESTI SIGUR?")+textwidth("DA")+15&&x<=textwidth("ESTI SIGUR?")+textwidth("DA")+15+textwidth("NU")&&y>=101&&y<=101+textheight("NU"))
            {
                executatInput=true;
            }
        }
    }
}
void apasareButon() //Alex
{
    int x=mousex();
    int y=mousey();
    if(x<=180&&x>=0&&y<=70&&y>=0)
    {
        meniu=meniuPrincipal;
        butonApasat=0;
        executatInput=true;
    }
    else if(x<=180&&x>=0&&y<=100&&y>=70)
    {
        meniuConfirmare();
    }
    else if(x>=180&&x<=180+(1500-180)/13&&y<=100&&y>=0)
    {
        if(butonApasat==AMPLOP)
        {
            butonApasat=0;
            executatInput=true;
        }
        else if(butonApasat!=0)
        {
            stergereComponenta(numarComponente);
            adaugareComponenta(AMPLOP,x+xglobal,y+yglobal);
            butonApasat=AMPLOP;
        }
        else
        {
            adaugareComponenta(AMPLOP,x+xglobal,y+yglobal);
            butonApasat=AMPLOP;
        }
    }
    else if(x>=180+(1500-180)/13&&x<=180+2*(1500-180)/13&&y<=100&&y>=0)
    {
        if(butonApasat==BATERIE)
        {
            butonApasat=0;
            executatInput=true;
        }
        else if(butonApasat!=0)
        {
            stergereComponenta(numarComponente);
            adaugareComponenta(BATERIE,x+xglobal,y+yglobal);
            butonApasat=BATERIE;
        }
        else
        {
            adaugareComponenta(BATERIE,x+xglobal,y+yglobal);
            butonApasat=BATERIE;
        }
    }
    else if(x>=180+2*(1500-180)/13&&x<=180+3*(1500-180)/13&&y<=100&&y>=0)
    {
        if(butonApasat==CONDENS)
        {
            butonApasat=0;
            executatInput=true;
        }
        else if(butonApasat!=0)
        {
            stergereComponenta(numarComponente);
            adaugareComponenta(CONDENS,x+xglobal,y+yglobal);
            butonApasat=CONDENS;
        }
        else
        {
            adaugareComponenta(CONDENS,x+xglobal,y+yglobal);
            butonApasat=CONDENS;
        }
    }
    else if(x>=180+3*(1500-180)/13&&x<=180+4*(1500-180)/13&&y<=100&&y>=0)
    {
        if(butonApasat==DIODA)
        {
            butonApasat=0;
            executatInput=true;
        }
        else if(butonApasat!=0)
        {
            stergereComponenta(numarComponente);
            adaugareComponenta(DIODA,x+xglobal,y+yglobal);
            butonApasat=DIODA;
        }
        else
        {
            adaugareComponenta(DIODA,x+xglobal,y+yglobal);
            butonApasat=DIODA;
        }
    }
    else if(x>=180+4*(1500-180)/13&&x<=180+5*(1500-180)/13&&y<=100&&y>=0)
    {
        if(butonApasat==NOD)
        {
            butonApasat=0;
            executatInput=true;
        }
        else if(butonApasat!=0)
        {
            stergereComponenta(numarComponente);
            adaugareComponenta(NOD,x+xglobal,y+yglobal);
            butonApasat=NOD;
        }
        else
        {
            adaugareComponenta(NOD,x+xglobal,y+yglobal);
            butonApasat=NOD;
        }
    }
    else if(x>=180+5*(1500-180)/13&&x<=180+6*(1500-180)/13&&y<=100&&y>=0)
    {
        if(butonApasat==POLARIZ)
        {
            butonApasat=0;
            executatInput=true;
        }
        else if(butonApasat!=0)
        {
            stergereComponenta(numarComponente);
            adaugareComponenta(POLARIZ+xglobal,x,y+yglobal);
            butonApasat=POLARIZ;
        }
        else
        {
            adaugareComponenta(POLARIZ,x+xglobal,y+yglobal);
            butonApasat=POLARIZ;
        }
    }
    else if(x>=180+6*(1500-180)/13&&x<=180+7*(1500-180)/13&&y<=100&&y>=0)
    {
        if(butonApasat==REZIST)
        {
            butonApasat=0;
            executatInput=true;
        }
        else if(butonApasat!=0)
        {
            stergereComponenta(numarComponente);
            adaugareComponenta(REZIST,x+xglobal,y+yglobal);
            butonApasat=REZIST;
        }
        else
        {
            adaugareComponenta(REZIST,x+xglobal,y+yglobal);
            butonApasat=REZIST;
        }
    }
    else if(x>=180+7*(1500-180)/13&&x<=180+8*(1500-180)/13&&y<=100&&y>=0)
    {
        if(butonApasat==SERVOMOT)
        {
            butonApasat=0;
            executatInput=true;
        }
        else if(butonApasat!=0)
        {
            stergereComponenta(numarComponente);
            adaugareComponenta(SERVOMOT,x+xglobal,y+yglobal);
            butonApasat=SERVOMOT;
        }
        else
        {
            adaugareComponenta(SERVOMOT,x+xglobal,y+yglobal);
            butonApasat=SERVOMOT;
        }
    }
    else if(x>=180+8*(1500-180)/13&&x<=180+9*(1500-180)/13&&y<=100&&y>=0)
    {
        if(butonApasat==SINU)
        {
            butonApasat=0;
            executatInput=true;
        }
        else if(butonApasat!=0)
        {
            stergereComponenta(numarComponente);
            adaugareComponenta(SINU,x+xglobal,y+yglobal);
            butonApasat=SINU;
        }
        else
        {
            adaugareComponenta(SINU,x+xglobal,y+yglobal);
            butonApasat=SINU;
        }
    }
    else if(x>=180+9*(1500-180)/13&&x<=180+10*(1500-180)/13&&y<=100&&y>=0)
    {
        if(butonApasat==STOP)
        {
            butonApasat=0;
            executatInput=true;
        }
        else if(butonApasat!=0)
        {
            stergereComponenta(numarComponente);
            adaugareComponenta(STOP,x+xglobal,y+yglobal);
            butonApasat=STOP;
        }
        else
        {
            adaugareComponenta(STOP,x+xglobal,y+yglobal);
            butonApasat=STOP;
        }
    }
    else if(x>=180+10*(1500-180)/13&&x<=180+11*(1500-180)/13&&y<=100&&y>=0)
    {
        if(butonApasat==TRANZNPN)
        {
            butonApasat=0;
            executatInput=true;
        }
        else if(butonApasat!=0)
        {
            stergereComponenta(numarComponente);
            adaugareComponenta(TRANZNPN,x+xglobal,y+yglobal);
            butonApasat=TRANZNPN;
        }
        else
        {
            adaugareComponenta(TRANZNPN,x+xglobal,y+yglobal);
            butonApasat=TRANZNPN;
        }
    }
    else if(x>=180+11*(1500-180)/13&&x<=180+12*(1500-180)/13&&y<=100&&y>=0)
    {
        if(butonApasat==TRANZPNP)
        {
            butonApasat=0;
            executatInput=true;
        }
        else if(butonApasat!=0)
        {
            stergereComponenta(numarComponente);
            adaugareComponenta(TRANZPNP,x+xglobal,y+yglobal);
            butonApasat=TRANZPNP;
        }
        else
        {
            adaugareComponenta(TRANZPNP,x+xglobal,y+yglobal);
            butonApasat=TRANZPNP;
        }
    }
    else if(x>=180+12*(1500-180)/13&&x<=180+13*(1500-180)/13&&y<=100&&y>=0)
    {
        if(butonApasat==ZENNER)
        {
            butonApasat=0;
            executatInput=true;
        }
        else if(butonApasat!=0)
        {
            stergereComponenta(numarComponente);
            adaugareComponenta(ZENNER,x+xglobal,y+yglobal);
            butonApasat=ZENNER;
        }
        else
        {
            adaugareComponenta(ZENNER,x+xglobal,y+yglobal);
            butonApasat=ZENNER;
        }
    }
}
void whileButonApasat(int i)//Alex
{
    int x,y;
    while(butonApasat)
    {
        x=mousex(); y=mousey();
        desenareComponenta(i,x,y,culoareComponente);
        delay(15);
        desenareComponenta(i,x,y,culoareFundal);
        if(y>=100&&ismouseclick(WM_LBUTTONDOWN))
        {
            clearmouseclick(WM_LBUTTONDOWN);
            comp[i].plasat=true;
            comp[i].xcomponenta=x;
            comp[i].ycomponenta=y;
            for(int j = 1; j <= tip_desen[comp[i].tipComponenta].nr_noduri; j++)
            {
                comp[i].xnod[j]=x+tip_desen[comp[i].tipComponenta].noduri[j][1] *comp[i].zoom;
                comp[i].ynod[j]=y+tip_desen[comp[i].tipComponenta].noduri[j][2] *comp[i].zoom;
            }
            butonApasat=0;
            desenareComponenta(i,comp[i].xcomponenta,comp[i].ycomponenta,culoareComponente);
            adaugareNumeComponenta(i);
            adaugareValoareRezistenta(i);
            executatInput=true;
        }
        else if(ismouseclick(WM_LBUTTONDOWN))
        {
            clearmouseclick(WM_LBUTTONDOWN);
            apasareButon();
        }
    }
}
void whileNodApasat(int &numarNod, int &numarComponenta)  //Alex
{
  while(numarNod!=0)
  {
    int x=mousex(), y=mousey();
    trasareCablu(x,y,comp[numarComponenta].xnod[numarNod],comp[numarComponenta].ynod[numarNod],culoareLegaturi,tipLegaturaTrasare);
    delay(10);
    trasareCablu(x,y,comp[numarComponenta].xnod[numarNod],comp[numarComponenta].ynod[numarNod],culoareFundal,tipLegaturaTrasare);
    if(y>=100)
        {
            for(int i=1;i<=numarComponente;i++)
            {
                if(x<=comp[i].xnod[1]+5&&x>=comp[i].xnod[1]-5&&y<=comp[i].ynod[1]+5&&y>=comp[i].ynod[1]-5)
                {
                    setcolor(culoareFundal+1);
                    rectangle(comp[i].xnod[1]-5,comp[i].ynod[1]-5,comp[i].xnod[1]+5,comp[i].ynod[1]+5);
                }
                else
                {
                    setcolor(culoareFundal);
                    rectangle(comp[i].xnod[1]-5,comp[i].ynod[1]-5,comp[i].xnod[1]+5,comp[i].ynod[1]+5);
                }
                if(tip_desen[comp[i].tipComponenta].nr_noduri>=2&&x<=comp[i].xnod[2]+5&&x>=comp[i].xnod[2]-5&&y<=comp[i].ynod[2]+5&&y>=comp[i].ynod[2]-5)
                {
                    setcolor(culoareFundal+1);
                    rectangle(comp[i].xnod[2]-5,comp[i].ynod[2]-5,comp[i].xnod[2]+5,comp[i].ynod[2]+5);
                }
                else
                {
                    setcolor(culoareFundal);
                    rectangle(comp[i].xnod[2]-5,comp[i].ynod[2]-5,comp[i].xnod[2]+5,comp[i].ynod[2]+5);
                }
                if(tip_desen[comp[i].tipComponenta].nr_noduri>=3&&x<=comp[i].xnod[3]+5&&x>=comp[i].xnod[3]-5&&y<=comp[i].ynod[3]+5&&y>=comp[i].ynod[3]-5)
                {
                    setcolor(culoareFundal+1);
                    rectangle(comp[i].xnod[3]-5,comp[i].ynod[3]-5,comp[i].xnod[3]+5,comp[i].ynod[3]+5);
                }
                else
                {
                    setcolor(culoareFundal);
                    rectangle(comp[i].xnod[3]-5,comp[i].ynod[3]-5,comp[i].xnod[3]+5,comp[i].ynod[3]+5);
                }
            }
        }
    if(ismouseclick(WM_LBUTTONDOWN))
    {
        clearmouseclick(WM_LBUTTONDOWN);
        for(int j=1;j<=numarComponente;j++)
        {
            if(x<=comp[j].xnod[1]+5&&x>=comp[j].xnod[1]-5&&y<=comp[j].ynod[1]+5&&y>=comp[j].ynod[1]-5)
            {
                if(numarComponenta!=j)
                adaugareCablu(numarNod, numarComponenta,1,j);
                break;
            }
            else if(tip_desen[comp[j].tipComponenta].nr_noduri>=2&&x<=comp[j].xnod[2]+5&&x>=comp[j].xnod[2]-5&&y<=comp[j].ynod[2]+5&&y>=comp[j].ynod[2]-5)
            {
                if(numarComponenta!=j)
                adaugareCablu(numarNod, numarComponenta,2,j);
                break;
            }
            else if(tip_desen[comp[j].tipComponenta].nr_noduri>=3&&x<=comp[j].xnod[3]+5&&x>=comp[j].xnod[3]-5&&y<=comp[j].ynod[3]+5&&y>=comp[j].ynod[3]-5)
            {
                if(numarComponenta!=j)
                adaugareCablu(numarNod, numarComponenta,3,j);
                break;
            }
        }
        numarNod=0;
        numarComponenta=0;
        executatInput=TRUE;
    }
    if(ismouseclick(WM_RBUTTONDOWN))
    {
        clearmouseclick(WM_RBUTTONDOWN);
        tipLegaturaTrasare=(tipLegaturaTrasare+1)%3;
    }
  }
}
void mutareComponenta(int numarComponenta)//Alex
{
    int xpreluat=comp[numarComponenta].xcomponenta,ypreluat=comp[numarComponenta].ycomponenta;
    cout<<endl<<endl<<xpreluat<<" "<<ypreluat<<endl<<endl;
    while(executatInput==false)
    {
        int x=mousex(),y=mousey();
        int xdif=x-xpreluat,ydif=y-ypreluat;
        desenareComponenta(numarComponenta,x,y,culoareComponente);
        for(int i=1;i<=numarLegaturi;i++)
        {
            if(leg[i].componenta1==numarComponenta)
                {
                    if(leg[i].nod2==1)
                    {
                        trasareCablu(x,y,comp[leg[i].componenta2].xnod[1],comp[leg[i].componenta2].ynod[1],culoareLegaturi,leg[i].tipLeg);
                    }
                    if(leg[i].nod2==2)
                    {
                        trasareCablu(x,y,comp[leg[i].componenta2].xnod[2],comp[leg[i].componenta2].ynod[2],culoareLegaturi,leg[i].tipLeg);
                    }
                    if(leg[i].nod2==3)
                    {
                        trasareCablu(x,y,comp[leg[i].componenta2].xnod[3],comp[leg[i].componenta2].ynod[3],culoareLegaturi,leg[i].tipLeg);
                    }
                }
                if(leg[i].componenta2==numarComponenta)
                {
                    if(leg[i].nod1==1)
                    {
                        trasareCablu(x,y,comp[leg[i].componenta1].xnod[1],comp[leg[i].componenta1].ynod[1],culoareLegaturi,leg[i].tipLeg);
                    }
                    if(leg[i].nod1==2)
                    {
                        trasareCablu(x,y,comp[leg[i].componenta1].xnod[2],comp[leg[i].componenta1].ynod[2],culoareLegaturi,leg[i].tipLeg);
                    }
                    if(leg[i].nod1==3)
                    {
                        trasareCablu(x,y,comp[leg[i].componenta1].xnod[3],comp[leg[i].componenta1].ynod[3],culoareLegaturi,leg[i].tipLeg);
                    }
                }
        }
        delay(15);
        desenareComponenta(numarComponenta,x,y,culoareFundal);
        for(int i=1;i<=numarLegaturi;i++)
        {
            if(leg[i].componenta1==numarComponenta)
                {
                    if(leg[i].nod2==1)
                    {
                        trasareCablu(x,y,comp[leg[i].componenta2].xnod[1],comp[leg[i].componenta2].ynod[1],culoareFundal,leg[i].tipLeg);
                    }
                    if(leg[i].nod2==2)
                    {
                        trasareCablu(x,y,comp[leg[i].componenta2].xnod[2],comp[leg[i].componenta2].ynod[2],culoareFundal,leg[i].tipLeg);
                    }
                    if(leg[i].nod2==3)
                    {
                        trasareCablu(x,y,comp[leg[i].componenta2].xnod[3],comp[leg[i].componenta2].ynod[3],culoareFundal,leg[i].tipLeg);
                    }
                }
                if(leg[i].componenta2==numarComponenta)
                {
                    if(leg[i].nod1==1)
                    {
                        trasareCablu(x,y,comp[leg[i].componenta1].xnod[1],comp[leg[i].componenta1].ynod[1],culoareFundal,leg[i].tipLeg);
                    }
                    if(leg[i].nod1==2)
                    {
                        trasareCablu(x,y,comp[leg[i].componenta1].xnod[2],comp[leg[i].componenta1].ynod[2],culoareFundal,leg[i].tipLeg);
                    }
                    if(leg[i].nod1==3)
                    {
                        trasareCablu(x,y,comp[leg[i].componenta1].xnod[3],comp[leg[i].componenta1].ynod[3],culoareFundal,leg[i].tipLeg);
                    }
                }
        }
        if(ismouseclick(WM_LBUTTONDOWN)&&y>=100)
        {
            clearmouseclick(WM_LBUTTONDOWN);
            executatInput=true;
            comp[numarComponenta].xcomponenta=x;
            comp[numarComponenta].ycomponenta=y;
            comp[numarComponenta].plasat=true;
            cout<<x<<" "<<y<<endl;
            for(int j = 1; j <= tip_desen[comp[numarComponenta].tipComponenta].nr_noduri; j++)
            {
                comp[numarComponenta].xnod[j]=comp[numarComponenta].xnod[j]+xdif;
                comp[numarComponenta].ynod[j]=comp[numarComponenta].ynod[j]+ydif;
            }
            for(int i=1;i<=numarLegaturi;i++)
            {
                if(leg[i].componenta1==numarComponenta)
                {
                    if(leg[i].nod1==1)
                    {
                        leg[i].x1=comp[leg[i].componenta1].xnod[1];
                        leg[i].y1=comp[leg[i].componenta1].ynod[1];
                    }
                    if(leg[i].nod1==2)
                    {
                        leg[i].x1=comp[leg[i].componenta1].xnod[2];
                        leg[i].y1=comp[leg[i].componenta1].ynod[2];
                    }
                    if(leg[i].nod1==3)
                    {
                        leg[i].x1=comp[leg[i].componenta1].xnod[3];
                        leg[i].y1=comp[leg[i].componenta1].ynod[3];
                    }
                }
                if(leg[i].componenta2==numarComponenta)
                {
                    if(leg[i].nod2==1)
                    {
                        leg[i].x2=comp[leg[i].componenta2].xnod[1];
                        leg[i].y2=comp[leg[i].componenta2].ynod[1];
                    }
                    if(leg[i].nod2==2)
                    {
                        leg[i].x2=comp[leg[i].componenta2].xnod[2];
                        leg[i].y2=comp[leg[i].componenta2].ynod[2];
                    }
                    if(leg[i].nod2==3)
                    {
                        leg[i].x2=comp[leg[i].componenta2].xnod[3];
                        leg[i].y2=comp[leg[i].componenta2].ynod[3];
                    }
                }
            }
        }
    }
}
void apasareComponenta(int numarComponenta)//Alex
{
    int x=mousex(), y=mousey();
    for(int i=1;i<=numarComponente;i++)
    {
       if(x<=comp[i].xcomponenta+5*comp[i].zoom/5&&x>=comp[i].xcomponenta-5*comp[i].zoom/5&&y<=comp[i].ycomponenta+5*comp[i].zoom/5&&y>=comp[i].ycomponenta-5*comp[i].zoom/5)
       {
            componentaSelectata=i;
            executatInput=true;
            //comp[numarComponenta].plasat=FALSE;
            //mutareComponenta(numarComponenta);
       }
    }
}
void apasareNod(int &numarNod, int &numarComponenta)//Alex
{
    int x=mousex(), y=mousey();
    componentaSelectata=0;
    for(int i=1;i<=numarComponente;i++)
    {
        if(tip_desen[comp[i].tipComponenta].nr_noduri>=1&&x<=comp[i].xnod[1]+7&&x>=comp[i].xnod[1]-7&&y<=comp[i].ynod[1]+7&&y>=comp[i].ynod[1]-7)
        {
            if(comp[i].legat[1]==FALSE||comp[i].tipComponenta==NOD)
            {
                numarNod=1;
                numarComponenta=i;
            }
        }
        if(tip_desen[comp[i].tipComponenta].nr_noduri>=2&&x<=comp[i].xnod[2]+7&&x>=comp[i].xnod[2]-7&&y<=comp[i].ynod[2]+7&&y>=comp[i].ynod[2]-7)
        {
            if(comp[i].legat[2]==FALSE)
            {
                numarNod=2;
                numarComponenta=i;
            }
        }
        if(tip_desen[comp[i].tipComponenta].nr_noduri>=3&&x<=comp[i].xnod[3]+7&&x>=comp[i].xnod[3]-7&&y<=comp[i].ynod[3]+7&&y>=comp[i].ynod[3]-7)
        {
            if(comp[i].legat[3]==FALSE)
            {
                numarNod=3;
                numarComponenta=i;
            }
        }
    }
}
void apasareButonRezistenta(int i)//Alex
{
      if(i==0) return;
      int x=mousex(), y=mousey();
      if(comp[i].xcomponenta<=700&&comp[i].ycomponenta>=500)
      {
          if(x>=comp[i].xcomponenta+20&&y<=comp[i].ycomponenta-50&&x<=comp[i].xcomponenta+40&&y>=comp[i].ycomponenta-70)
          {
              adaugareValoareRezistenta(componentaSelectata);
              componentaSelectata=0;
              executatInput=true;
          }
      }
      if(comp[i].xcomponenta>=700&&comp[i].ycomponenta>=500)
      {
          if(x<=comp[i].xcomponenta-20&&y<=comp[i].ycomponenta-50&&x>=comp[i].xcomponenta-40&&y>=comp[i].ycomponenta-70)
          {
              adaugareValoareRezistenta(componentaSelectata);
              componentaSelectata=0;
              executatInput=true;
          }
      }
      else if(comp[i].ycomponenta<=500&&comp[i].ycomponenta>=100&&comp[i].xcomponenta>=700)
      {
          if(x<=comp[i].xcomponenta-20&&y>=comp[i].ycomponenta+50&&x>=comp[i].xcomponenta-40&&y<=comp[i].ycomponenta+70)
          {
              adaugareValoareRezistenta(componentaSelectata);
              componentaSelectata=0;
              executatInput=true;
          }
      }
      else if(comp[i].ycomponenta<=500&&comp[i].ycomponenta>=100&&comp[i].xcomponenta<=700)
      {
          if(x>=comp[i].xcomponenta+20&&y>=comp[i].ycomponenta+50&&x<=comp[i].xcomponenta+40&&y<=comp[i].ycomponenta+70)
          {
              adaugareValoareRezistenta(componentaSelectata);
              componentaSelectata=0;
              executatInput=true;
          }
      }
}
void apasareButonNume(int i)//Alex
{
      if(i==0) return;
      int x=mousex(), y=mousey();
      if(comp[i].xcomponenta<=700&&comp[i].ycomponenta>=500)
      {
          if(x>=comp[i].xcomponenta+40&&y<=comp[i].ycomponenta-50&&x<=comp[i].xcomponenta+60&&y>=comp[i].ycomponenta-70)
          {
              adaugareNumeComponenta(componentaSelectata);
              componentaSelectata=0;
              executatInput=true;
          }
      }
      if(comp[i].xcomponenta>=700&&comp[i].ycomponenta>=500)
      {
          if(x<=comp[i].xcomponenta-40&&y<=comp[i].ycomponenta-50&&x>=comp[i].xcomponenta-60&&y>=comp[i].ycomponenta-70)
          {
              adaugareNumeComponenta(componentaSelectata);
              componentaSelectata=0;
              executatInput=true;
          }
      }
      else if(comp[i].ycomponenta<=500&&comp[i].ycomponenta>=100&&comp[i].xcomponenta>=700)
      {
          if(x<=comp[i].xcomponenta-40&&y>=comp[i].ycomponenta+50&&x>=comp[i].xcomponenta-60&&y<=comp[i].ycomponenta+70)
          {
              adaugareNumeComponenta(componentaSelectata);
              componentaSelectata=0;
              executatInput=true;
          }
      }
      else if(comp[i].ycomponenta<=500&&comp[i].ycomponenta>=100&&comp[i].xcomponenta<=700)
      {
          if(x>=comp[i].xcomponenta+40&&y>=comp[i].ycomponenta+50&&x<=comp[i].xcomponenta+60&&y<=comp[i].ycomponenta+70)
          {
              cout<<comp[i].xcomponenta+40<<" "<<comp[i].xcomponenta+60<<endl;
              cout<<comp[i].ycomponenta+50<<" "<<comp[i].ycomponenta+70<<endl;
              adaugareNumeComponenta(componentaSelectata);
              componentaSelectata=0;
              executatInput=true;
          }
      }
}
void apasareButonZoom(int i)//Alex
{
    int x=mousex(), y=mousey(), zoom=0;
    if(comp[i].ycomponenta>=500&&comp[i].xcomponenta<=700)
    {
        if(x>=comp[i].xcomponenta+60&&y<=comp[i].ycomponenta-50&&x<=comp[i].xcomponenta+80&&y>=comp[i].ycomponenta-70)
        {
            zoom=1;
        }
        else if(x>=comp[i].xcomponenta+60&&y>=comp[i].ycomponenta-50&&x<=comp[i].xcomponenta+80&&y<=comp[i].ycomponenta-30)
        {
            zoom=-1;
        }
    }
    else if(comp[i].ycomponenta>=500&&comp[i].xcomponenta>=700)
    {
        if(x<=comp[i].xcomponenta-60&&y<=comp[i].ycomponenta-50&&x>=comp[i].xcomponenta-80&&y>=comp[i].ycomponenta-70)
        {
            zoom=1;
        }
        else if(x<=comp[i].xcomponenta-60&&y>=comp[i].ycomponenta-50&&x>=comp[i].xcomponenta-80&&y<=comp[i].ycomponenta-30)
        {
            zoom=-1;
        }
    }
    else if(comp[i].ycomponenta<=500&&comp[i].ycomponenta>=100&&comp[i].xcomponenta>=700)
    {
        if(x<=comp[i].xcomponenta-60&&y>=comp[i].ycomponenta+30&&x>=comp[i].xcomponenta-80&&y<=comp[i].ycomponenta+50)
        {
            zoom=-1;
        }
        else if(x<=comp[i].xcomponenta-60&&y>=comp[i].ycomponenta+50&&x>=comp[i].xcomponenta-80&&y<=comp[i].ycomponenta+70)
        {
            zoom=1;
        }
    }
    else if(comp[i].ycomponenta<=500&&comp[i].ycomponenta>=100&&comp[i].xcomponenta<=700)
    {
        if(x>=comp[i].xcomponenta+60&&y>=comp[i].ycomponenta+30&&x<=comp[i].xcomponenta+80&&y<=comp[i].ycomponenta+50)
        {
            zoom=-1;
        }
        else if(x>=comp[i].xcomponenta+60&&y>=comp[i].ycomponenta+50&&x<=comp[i].xcomponenta+80&&y<=comp[i].ycomponenta+70)
        {
            zoom=1;
        }
    }
    if(zoom==1)
    {
        comp[i].zoom++;
    }
    else if(zoom==-1)
    {
        comp[i].zoom--;
    }
    if(zoom!=0)
    {
        for(int j = 1; j <= tip_desen[comp[i].tipComponenta].nr_noduri; j++)
        {
            comp[i].xnod[j]=comp[i].xcomponenta+tip_desen[comp[i].tipComponenta].noduri[j][1]*comp[i].zoom;
            comp[i].ynod[j]=comp[i].ycomponenta+tip_desen[comp[i].tipComponenta].noduri[j][2]*comp[i].zoom;
            double xnou, ynou;
            xnou=punctrotit(comp[i].xcomponenta,comp[i].ycomponenta,comp[i].sensRotatie,comp[i].xnod[j],comp[i].ynod[j],1);
            ynou=punctrotit(comp[i].xcomponenta,comp[i].ycomponenta,comp[i].sensRotatie,comp[i].xnod[j],comp[i].ynod[j],2);
            comp[i].xnod[j]=xnou;
            comp[i].ynod[j]=ynou;
        }
        for(int j=1;j<=numarLegaturi;j++)
        {
            if(leg[j].componenta1==i)
            {
                leg[j].x1=comp[i].xcomponenta+tip_desen[comp[i].tipComponenta].noduri[leg[j].nod1][1] *comp[i].zoom;
                leg[j].y1=comp[i].ycomponenta+tip_desen[comp[i].tipComponenta].noduri[leg[j].nod1][2] *comp[i].zoom;
                double xnou, ynou;
                xnou=punctrotit(comp[leg[j].componenta1].xcomponenta,comp[leg[j].componenta1].ycomponenta,comp[leg[j].componenta1].sensRotatie,leg[j].x1,leg[j].y1,1);
                ynou=punctrotit(comp[leg[j].componenta1].xcomponenta,comp[leg[j].componenta1].ycomponenta,comp[leg[j].componenta1].sensRotatie,leg[j].x1,leg[j].y1,2);
                leg[j].x1=xnou;
                leg[j].y1=ynou;
            }
            if(leg[j].componenta2==i)
            {
                leg[j].x2=comp[i].xcomponenta+tip_desen[comp[i].tipComponenta].noduri[leg[j].nod2][1] *comp[i].zoom;
                leg[j].y2=comp[i].ycomponenta+tip_desen[comp[i].tipComponenta].noduri[leg[j].nod2][2] *comp[i].zoom;
                double xnou, ynou;
                xnou=punctrotit(comp[leg[j].componenta2].xcomponenta,comp[leg[j].componenta2].ycomponenta,comp[leg[j].componenta2].sensRotatie,leg[j].x2,leg[j].y2,1);
                ynou=punctrotit(comp[leg[j].componenta2].xcomponenta,comp[leg[j].componenta2].ycomponenta,comp[leg[j].componenta2].sensRotatie,leg[j].x2,leg[j].y2,2);
                leg[j].x2=xnou;
                leg[j].y2=ynou;
            }
        }
        executatInput=true;
    }
    zoom=0;
}
void apasareButonRotireStanga(int i)//Alex
{
    int x=mousex(), y=mousey();
    if(comp[i].ycomponenta>=500&&comp[i].xcomponenta<=700)
    {
        if(x>=comp[i].xcomponenta+40&&y>=comp[i].ycomponenta-50&&x<=comp[i].xcomponenta+60&&y<=comp[i].ycomponenta-30)
        {
            rotireComponenta(componentaSelectata,270);
            executatInput=true;
        }
    }
    else if(comp[i].ycomponenta>=500&&comp[i].xcomponenta>=700)
    {
        if(x<=comp[i].xcomponenta-40&&y>=comp[i].ycomponenta-50&&x>=comp[i].xcomponenta-60&&y<=comp[i].ycomponenta-30)
        {
            rotireComponenta(componentaSelectata,270);
            executatInput=true;
        }
    }
    else if(comp[i].ycomponenta<=500&&comp[i].ycomponenta>=100&&comp[i].xcomponenta>=700)
    {
        if(x<=comp[i].xcomponenta-40&&y>=comp[i].ycomponenta+30&&x>=comp[i].xcomponenta-60&&y<=comp[i].ycomponenta+50)
        {
            rotireComponenta(componentaSelectata,270);
            executatInput=true;
        }
    }
    else if(comp[i].ycomponenta<=500&&comp[i].ycomponenta>=100&&comp[i].xcomponenta<=700)
    {
        if(x>=comp[i].xcomponenta+40&&y>=comp[i].ycomponenta+30&&x<=comp[i].xcomponenta+60&&y<=comp[i].ycomponenta+50)
        {
            rotireComponenta(componentaSelectata,270);
            executatInput=true;
        }
    }
}
void apasareButonRotireDreapta(int i)//Alex
{
    int x=mousex(), y=mousey();
    if(comp[i].ycomponenta>=500&&comp[i].xcomponenta<=700)
    {
        if(x>=comp[i].xcomponenta+20&&y>=comp[i].ycomponenta-50&&x<=comp[i].xcomponenta+40&&y<=comp[i].ycomponenta-30)
        {
            rotireComponenta(componentaSelectata,90);
            executatInput=true;
        }
    }
    else if(comp[i].ycomponenta>=500&&comp[i].xcomponenta>=700)
    {
        if(x<=comp[i].xcomponenta-20&&y>=comp[i].ycomponenta-50&&x>=comp[i].xcomponenta-40&&y<=comp[i].ycomponenta-30)
        {
            rotireComponenta(componentaSelectata,90);
            executatInput=true;
        }
    }
    else if(comp[i].ycomponenta<=500&&comp[i].ycomponenta>=100&&comp[i].xcomponenta>=700)
    {
        if(x<=comp[i].xcomponenta-20&&y>=comp[i].ycomponenta+30&&x>=comp[i].xcomponenta-40&&y<=comp[i].ycomponenta+50)
        {
            rotireComponenta(componentaSelectata,90);
            executatInput=true;
        }
    }
    else if(comp[i].ycomponenta<=500&&comp[i].ycomponenta>=100&&comp[i].xcomponenta<=700)
    {
        if(x>=comp[i].xcomponenta+20&&y>=comp[i].ycomponenta+30&&x<=comp[i].xcomponenta+40&&y<=comp[i].ycomponenta+50)
        {
            rotireComponenta(componentaSelectata,90);
            executatInput=true;
        }
    }
}
void apasareButonStergere(int i)//Tudor
{
    int x=mousex(), y=mousey();
    if(comp[i].ycomponenta>=500&&comp[i].xcomponenta<=700)
    {
        if(x>=comp[i].xcomponenta&&y>=comp[i].ycomponenta-70&&x<=comp[i].xcomponenta+20&&y<=comp[i].ycomponenta-50)
        {
            for(int j=1;j<=numarLegaturi;j++)
            {
                if(leg[j].componenta1==i||leg[j].componenta2==i)
                {
                    stergereLegatura(j);
                    j--;
                }
            }
            stergereComponenta(i);
            executatInput=true;
            componentaSelectata=0;
        }
    }
    else if(comp[i].ycomponenta>=500&&comp[i].xcomponenta>=700)
    {
        if(x<=comp[i].xcomponenta&&y>=comp[i].ycomponenta-70&&x>=comp[i].xcomponenta-20&&y<=comp[i].ycomponenta-50)
        {
            for(int j=1;j<=numarLegaturi;j++)
            {
                if(leg[j].componenta1==i||leg[j].componenta2==i)
                {
                    stergereLegatura(j);
                    j--;
                }
            }
            stergereComponenta(i);
            executatInput=true;
            componentaSelectata=0;
        }
    }
    else if(comp[i].ycomponenta<=500&&comp[i].ycomponenta>=100&&comp[i].xcomponenta>=700)
    {
        if(x<=comp[i].xcomponenta&&y>=comp[i].ycomponenta+50&&x>=comp[i].xcomponenta-20&&y<=comp[i].ycomponenta+70)
        {
            for(int j=1;j<=numarLegaturi;j++)
            {
                if(leg[j].componenta1==i||leg[j].componenta2==i)
                {
                    stergereLegatura(j);
                    j--;
                }
            }
            stergereComponenta(i);
            executatInput=true;
            componentaSelectata=0;
        }
    }
    else if(comp[i].ycomponenta<=500&&comp[i].ycomponenta>=100&&comp[i].xcomponenta<=700)
    {
        if(x>=comp[i].xcomponenta&&y>=comp[i].ycomponenta+50&&x<=comp[i].xcomponenta+20&&y<=comp[i].ycomponenta+70)
        {
            for(int j=1;j<=numarLegaturi;j++)
            {
                if(leg[j].componenta1==i||leg[j].componenta2==i)
                {
                    stergereLegatura(j);
                    j--;
                }
            }
            stergereComponenta(i);
            componentaSelectata=0;
            executatInput=true;
        }
    }
}
void apasareButonMutare(int i)//Alex
{
    int x=mousex(), y=mousey();
    if(comp[i].ycomponenta>=500&&comp[i].xcomponenta<=700)
    {
        if(x>=comp[i].xcomponenta&&y>=comp[i].ycomponenta-50&&x<=comp[i].xcomponenta+20&&y<=comp[i].ycomponenta-30)
        {
            setfillstyle(SOLID_FILL,culoareFundal);
            bar(comp[i].xcomponenta,comp[i].ycomponenta-70,comp[i].xcomponenta+80,comp[i].ycomponenta-10);
            desenareComponenta(i,comp[i].xcomponenta,comp[i].ycomponenta,culoareFundal);
            mutareComponenta(componentaSelectata);
        }
    }
    else if(comp[i].ycomponenta>=500&&comp[i].xcomponenta>=700)
    {
        if(x<=comp[i].xcomponenta&&y>=comp[i].ycomponenta-50&&x>=comp[i].xcomponenta-20&&y<=comp[i].ycomponenta-30)
        {
            setfillstyle(SOLID_FILL,culoareFundal);
            bar(comp[i].xcomponenta,comp[i].ycomponenta-70,comp[i].xcomponenta-80,comp[i].ycomponenta-10);
            desenareComponenta(i,comp[i].xcomponenta,comp[i].ycomponenta,culoareFundal);
            mutareComponenta(componentaSelectata);
        }
    }
    else if(comp[i].ycomponenta<=500&&comp[i].ycomponenta>=100&&comp[i].xcomponenta>=700)
    {
        if(x<=comp[i].xcomponenta&&y>=comp[i].ycomponenta+30&&x>=comp[i].xcomponenta-20&&y<=comp[i].ycomponenta+50)
        {
            setfillstyle(SOLID_FILL,culoareFundal);
            bar(comp[i].xcomponenta,comp[i].ycomponenta+70,comp[i].xcomponenta-80,comp[i].ycomponenta+10);
            desenareComponenta(i,comp[i].xcomponenta,comp[i].ycomponenta,culoareFundal);
            mutareComponenta(componentaSelectata);
        }
    }
    else if(comp[i].ycomponenta<=500&&comp[i].ycomponenta>=100&&comp[i].xcomponenta<=700)
    {
        if(x>=comp[i].xcomponenta&&y>=comp[i].ycomponenta+30&&x<=comp[i].xcomponenta+20&&y<=comp[i].ycomponenta+50)
        {
            setfillstyle(SOLID_FILL,culoareFundal);
            bar(comp[i].xcomponenta,comp[i].ycomponenta+70,comp[i].xcomponenta+80,comp[i].ycomponenta+10);
            desenareComponenta(i,comp[i].xcomponenta,comp[i].ycomponenta,culoareFundal);
            mutareComponenta(componentaSelectata);
        }
    }
}
void activeRotation() //Alex
{
    int x=mousex(), y=mousey();
    for(int i=1;i<=numarComponente;i++)
    {
        if(x<=comp[i].xcomponenta+5*comp[i].zoom/5&&x>=comp[i].xcomponenta-5*comp[i].zoom/5&&y<=comp[i].ycomponenta+5*comp[i].zoom/5&&y>=comp[i].ycomponenta-5*comp[i].zoom/5)
        {
            int xprev=mousex();
            while(executatInput==false)
            {
                x=mousex();
                rotireComponenta(i,(x-xprev)%360);
                while(comp[i].sensRotatie<0) comp[i].sensRotatie=comp[i].sensRotatie+360;
                desenareComponenta(i,comp[i].xcomponenta,comp[i].ycomponenta,culoareComponente);
                delay(20);
                desenareComponenta(i,comp[i].xcomponenta,comp[i].ycomponenta,culoareFundal);
                for(int j=1;j<=numarLegaturi;j++)
                {
                    if(leg[j].componenta1==i||leg[j].componenta2==i)
                    {
                       trasareCablu(leg[j].x1,leg[j].y1,leg[j].x2,leg[j].y2,culoareLegaturi,leg[j].tipLeg);
                       delay(5);
                       trasareCablu(leg[j].x1,leg[j].y1,leg[j].x2,leg[j].y2,culoareFundal,leg[j].tipLeg);
                    }
                }
                if(ismouseclick(WM_RBUTTONDOWN))
                {
                    clearmouseclick(WM_RBUTTONDOWN);
                    executatInput=true;
                }
                xprev=x;
            }
        }
    }
}
void userInput()//Comun
{
    int x=mousex(),y=mousey();
    int numarNod=0, numarComponenta=0;
    if(meniu==ecranCircuit)
    {
        if(y>=100)
        {
            for(int i=1;i<=numarComponente;i++)
            {
                for(int j=1;j<=tip_desen[comp[i].tipComponenta].nr_noduri;j++)
                {
                  //if(x<=comp[i].xnod[j]+5&&x>=comp[i].xnod[j]-5&&y<=comp[i].ynod[j]+5&&y>=comp[i].ynod[j]-5)
                    {
                        setcolor((culoareFundal+1)%16);
                        rectangle(comp[i].xnod[j]-5,max(comp[i].ynod[j]-5,100),comp[i].xnod[j]+5,max(comp[i].ynod[j]+5,100));
                    }
                    //else
                    //{
                      //  setcolor(culoareFundal);
                     //   rectangle(comp[i].xnod[j]-5,comp[i].ynod[j]-5,comp[i].xnod[j]+5,comp[i].ynod[j]+5);
                    //}
                }

            }
        }
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            clearmouseclick(WM_LBUTTONDOWN);
            apasareButonMutare(componentaSelectata);
            if(executatInput==true) return;
            apasareButonRotireStanga(componentaSelectata);
            if(executatInput==true) return;
            apasareButonRotireDreapta(componentaSelectata);
            if(executatInput==true) return;
            apasareButonNume(componentaSelectata);
            if(executatInput==true) return;
            apasareButonStergere(componentaSelectata);
            if(executatInput==true) return;
            apasareButonRezistenta(componentaSelectata);
            if(executatInput==true) return;
            apasareButonZoom(componentaSelectata);
            if(executatInput==true) return;
            apasareButon();
            whileButonApasat(numarComponente);
            if(executatInput==true) return;
            apasareNod(numarNod,numarComponenta);
            whileNodApasat(numarNod,numarComponenta);
            if(executatInput==true) return;
            apasareComponenta(numarComponenta);
            if(executatInput==true) return;
            else
            {
                if(componentaSelectata!=0) componentaSelectata=0;
                else dragEcran();
                executatInput=true;
                return;
            }
        }
        else if(ismouseclick(WM_RBUTTONDOWN))
        {
             clearmouseclick(WM_RBUTTONDOWN);
             activeRotation();
        }
    }
    else if(meniu==meniuPrincipal)
    {
        if(x>=575&&y>=200&&x<=995&&y<=296) setcolor(culoareText+1);
        else setcolor(culoareText);
        rectangle(575,200,995,296);
        if(x>=575&&y>=350&&x<=995&&y<=446) setcolor(culoareText+1);
        else setcolor(culoareText);
        rectangle(575,350,995,446);
        if(x>=575&&y>=500&&x<=995&&y<=596) setcolor(culoareText+1);
        else setcolor(culoareText);
        rectangle(575,500,995,596);
        if(x>=520&&y>=650&&x<=1065&&y<=746) setcolor(culoareText+1);
        else setcolor(culoareText);
        rectangle(520,650,1065,746);
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            clearmouseclick(WM_LBUTTONDOWN);
            x=mousex(); y=mousey();
            if(x>=575&&y>=200&&x<=975&&y<=295)
            {
                meniu=ecranCircuit;
                executatInput=true;
            }
            else if(x>=575&&x<=975&&y>=350&&y<=440)
            {
                meniu=meniuCustomizare;
                executatInput=true;
            }
            else if(x>=575&&y>=500&&x<=975&&y<=600)
            {
                salvareCircuit();
                executatInput=true;
            }
            else if(x>=520&&y>=650&&x<=1030&&y<=745)
            {
                incarcareCircuit();
                executatInput=true;
            }
            else
            {
                clearmouseclick(WM_LBUTTONDOWN);
                executatInput=true;
            }
        }
    }
    else if(meniu==meniuCustomizare)
    {
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            clearmouseclick(WM_LBUTTONDOWN);
           for(int i=1;i<=6;i++)
            {
            for(int j=0;j<=15;j++)
                {
                    if(x>=370+j*(1400-370)/16&&y>=100+(i-1)*(550-100)/5&&x<=370+(j+1)*(1400-370)/16&&y<=100+i*(550-100)/5)
                    {
                        if(i==1&&culoareText!=j) culoareFundal=j;
                        else if(i==2&&culoareText!=j) culoareMeniu=j;
                        else if(i==3) culoareComponente=j;
                        else if(i==4) culoareLegaturi=j;
                        else if(i==5&&culoareMeniu!=j&&culoareFundal!=j) culoareText=j;
                        else if(i==6) culoareFundalText=j;
                        executatInput=true;
                    }
                }
            }
            if(x>=1400&&y>=700&&x<=1450&&y<=750)
            {
                meniu=meniuPrincipal;
                executatInput=true;
            }
        }

    }
}
void desenareMeniuComponenta(int i)//Comun
{
    if(comp[i].ycomponenta>=500&&comp[i].xcomponenta<=700)
    {
        setfillstyle(SOLID_FILL,culoareMeniu);
        bar(comp[i].xcomponenta,comp[i].ycomponenta-50,comp[i].xcomponenta+20,comp[i].ycomponenta-30);//buton Mutare
        setcolor(culoareFundal);
        line(comp[i].xcomponenta+2,comp[i].ycomponenta-40,comp[i].xcomponenta+18,comp[i].ycomponenta-40);
        line(comp[i].xcomponenta+10,comp[i].ycomponenta-48,comp[i].xcomponenta+10,comp[i].ycomponenta-32);
        line(comp[i].xcomponenta+7,comp[i].ycomponenta-45,comp[i].xcomponenta+10,comp[i].ycomponenta-48);
        line(comp[i].xcomponenta+13,comp[i].ycomponenta-45,comp[i].xcomponenta+10,comp[i].ycomponenta-48);
        line(comp[i].xcomponenta+7,comp[i].ycomponenta-35,comp[i].xcomponenta+10,comp[i].ycomponenta-32);
        line(comp[i].xcomponenta+13,comp[i].ycomponenta-35,comp[i].xcomponenta+10,comp[i].ycomponenta-32);
        line(comp[i].xcomponenta+2,comp[i].ycomponenta-40,comp[i].xcomponenta+5,comp[i].ycomponenta-43);
        line(comp[i].xcomponenta+2,comp[i].ycomponenta-40,comp[i].xcomponenta+5,comp[i].ycomponenta-37);
        line(comp[i].xcomponenta+18,comp[i].ycomponenta-40,comp[i].xcomponenta+15,comp[i].ycomponenta-43);
        line(comp[i].xcomponenta+18,comp[i].ycomponenta-40,comp[i].xcomponenta+15,comp[i].ycomponenta-37);

        bar(comp[i].xcomponenta+20,comp[i].ycomponenta-50,comp[i].xcomponenta+40,comp[i].ycomponenta-30);//buton rotire dreapta
        ellipse(comp[i].xcomponenta+30,comp[i].ycomponenta-35,0,180,6,8);
        line(comp[i].xcomponenta+34,comp[i].ycomponenta-35,comp[i].xcomponenta+32,comp[i].ycomponenta-37);
        line(comp[i].xcomponenta+34,comp[i].ycomponenta-35,comp[i].xcomponenta+36,comp[i].ycomponenta-37);

        bar(comp[i].xcomponenta+40,comp[i].ycomponenta-50,comp[i].xcomponenta+60,comp[i].ycomponenta-30);//buton rotire stanga
        ellipse(comp[i].xcomponenta+50,comp[i].ycomponenta-35,0,180,6,8);
        line(comp[i].xcomponenta+45,comp[i].ycomponenta-35,comp[i].xcomponenta+43,comp[i].ycomponenta-37);
        line(comp[i].xcomponenta+45,comp[i].ycomponenta-35,comp[i].xcomponenta+47,comp[i].ycomponenta-37);

        bar(comp[i].xcomponenta,comp[i].ycomponenta-50,comp[i].xcomponenta+20,comp[i].ycomponenta-70);//buton stergere componenta
        line(comp[i].xcomponenta+5,comp[i].ycomponenta-55,comp[i].xcomponenta+15,comp[i].ycomponenta-65);
        line(comp[i].xcomponenta+15,comp[i].ycomponenta-55,comp[i].xcomponenta+5,comp[i].ycomponenta-65);

        bar(comp[i].xcomponenta+20,comp[i].ycomponenta-50,comp[i].xcomponenta+40,comp[i].ycomponenta-70);//buton R
        settextstyle(8,HORIZ_DIR,1);
        setbkcolor(culoareMeniu);
        setcolor(culoareFundal);
        outtextxy(comp[i].xcomponenta+24,comp[i].ycomponenta-70,"R");

        bar(comp[i].xcomponenta+40,comp[i].ycomponenta-50,comp[i].xcomponenta+60,comp[i].ycomponenta-70);//buton N
        outtextxy(comp[i].xcomponenta+44,comp[i].ycomponenta-70,"N");

        bar(comp[i].xcomponenta+60,comp[i].ycomponenta-50,comp[i].xcomponenta+80,comp[i].ycomponenta-70);//buton +
        line(comp[i].xcomponenta+70,comp[i].ycomponenta-55,comp[i].xcomponenta+70,comp[i].ycomponenta-65);
        line(comp[i].xcomponenta+65,comp[i].ycomponenta-60,comp[i].xcomponenta+75,comp[i].ycomponenta-60);

        bar(comp[i].xcomponenta+60,comp[i].ycomponenta-50,comp[i].xcomponenta+80,comp[i].ycomponenta-30);//buton -
        line(comp[i].xcomponenta+65,comp[i].ycomponenta-40,comp[i].xcomponenta+75,comp[i].ycomponenta-40);

        rectangle(comp[i].xcomponenta,comp[i].ycomponenta-50,comp[i].xcomponenta+20,comp[i].ycomponenta-30);
        rectangle(comp[i].xcomponenta+20,comp[i].ycomponenta-50,comp[i].xcomponenta+40,comp[i].ycomponenta-30);
        rectangle(comp[i].xcomponenta+40,comp[i].ycomponenta-50,comp[i].xcomponenta+60,comp[i].ycomponenta-30);
        rectangle(comp[i].xcomponenta,comp[i].ycomponenta-50,comp[i].xcomponenta+20,comp[i].ycomponenta-70);
        rectangle(comp[i].xcomponenta+20,comp[i].ycomponenta-50,comp[i].xcomponenta+40,comp[i].ycomponenta-70);
        rectangle(comp[i].xcomponenta+40,comp[i].ycomponenta-50,comp[i].xcomponenta+60,comp[i].ycomponenta-70);
        rectangle(comp[i].xcomponenta+60,comp[i].ycomponenta-50,comp[i].xcomponenta+80,comp[i].ycomponenta-70);
        rectangle(comp[i].xcomponenta+60,comp[i].ycomponenta-50,comp[i].xcomponenta+80,comp[i].ycomponenta-30);
    }
    else if(comp[i].ycomponenta>=500&&comp[i].xcomponenta>=700)
    {
        setfillstyle(SOLID_FILL,culoareMeniu);
        setcolor(culoareFundal);
        bar(comp[i].xcomponenta,comp[i].ycomponenta-50,comp[i].xcomponenta-20,comp[i].ycomponenta-30);//buton mutare
        line(comp[i].xcomponenta-2,comp[i].ycomponenta-40,comp[i].xcomponenta-18,comp[i].ycomponenta-40);
        line(comp[i].xcomponenta-10,comp[i].ycomponenta-48,comp[i].xcomponenta-10,comp[i].ycomponenta-32);
        line(comp[i].xcomponenta-7,comp[i].ycomponenta-45,comp[i].xcomponenta-10,comp[i].ycomponenta-48);
        line(comp[i].xcomponenta-13,comp[i].ycomponenta-45,comp[i].xcomponenta-10,comp[i].ycomponenta-48);
        line(comp[i].xcomponenta-7,comp[i].ycomponenta-35,comp[i].xcomponenta-10,comp[i].ycomponenta-32);
        line(comp[i].xcomponenta-13,comp[i].ycomponenta-35,comp[i].xcomponenta-10,comp[i].ycomponenta-32);
        line(comp[i].xcomponenta-2,comp[i].ycomponenta-40,comp[i].xcomponenta-5,comp[i].ycomponenta-43);
        line(comp[i].xcomponenta-2,comp[i].ycomponenta-40,comp[i].xcomponenta-5,comp[i].ycomponenta-37);
        line(comp[i].xcomponenta-18,comp[i].ycomponenta-40,comp[i].xcomponenta-15,comp[i].ycomponenta-43);
        line(comp[i].xcomponenta-18,comp[i].ycomponenta-40,comp[i].xcomponenta-15,comp[i].ycomponenta-37);

        bar(comp[i].xcomponenta-20,comp[i].ycomponenta-50,comp[i].xcomponenta-40,comp[i].ycomponenta-30);//buton rotire dreapta
        ellipse(comp[i].xcomponenta-30,comp[i].ycomponenta-35,0,180,6,8);
        line(comp[i].xcomponenta-25,comp[i].ycomponenta-35,comp[i].xcomponenta-23,comp[i].ycomponenta-37);
        line(comp[i].xcomponenta-25,comp[i].ycomponenta-35,comp[i].xcomponenta-27,comp[i].ycomponenta-37);

        bar(comp[i].xcomponenta-40,comp[i].ycomponenta-50,comp[i].xcomponenta-60,comp[i].ycomponenta-30);//buton rotire stanga
        ellipse(comp[i].xcomponenta-50,comp[i].ycomponenta-35,0,180,6,8);
        line(comp[i].xcomponenta-54,comp[i].ycomponenta-35,comp[i].xcomponenta-52,comp[i].ycomponenta-37);
        line(comp[i].xcomponenta-54,comp[i].ycomponenta-35,comp[i].xcomponenta-56,comp[i].ycomponenta-37);

        bar(comp[i].xcomponenta,comp[i].ycomponenta-50,comp[i].xcomponenta-20,comp[i].ycomponenta-70);//buton stergere
        line(comp[i].xcomponenta-5,comp[i].ycomponenta-55,comp[i].xcomponenta-15,comp[i].ycomponenta-65);
        line(comp[i].xcomponenta-15,comp[i].ycomponenta-55,comp[i].xcomponenta-5,comp[i].ycomponenta-65);

        bar(comp[i].xcomponenta-20,comp[i].ycomponenta-50,comp[i].xcomponenta-40,comp[i].ycomponenta-70);//buton R
        settextstyle(8,HORIZ_DIR,1);
        setbkcolor(culoareMeniu);
        setcolor(culoareFundal);
        outtextxy(comp[i].xcomponenta-36,comp[i].ycomponenta-69,"R");

        bar(comp[i].xcomponenta-40,comp[i].ycomponenta-50,comp[i].xcomponenta-60,comp[i].ycomponenta-70);//buton N
        outtextxy(comp[i].xcomponenta-55,comp[i].ycomponenta-70,"N");

        bar(comp[i].xcomponenta-60,comp[i].ycomponenta-50,comp[i].xcomponenta-80,comp[i].ycomponenta-70);//+
        line(comp[i].xcomponenta-70,comp[i].ycomponenta-55,comp[i].xcomponenta-70,comp[i].ycomponenta-65);
        line(comp[i].xcomponenta-65,comp[i].ycomponenta-60,comp[i].xcomponenta-75,comp[i].ycomponenta-60);

        bar(comp[i].xcomponenta-60,comp[i].ycomponenta-50,comp[i].xcomponenta-80,comp[i].ycomponenta-30);//-
        line(comp[i].xcomponenta-65,comp[i].ycomponenta-40,comp[i].xcomponenta-75,comp[i].ycomponenta-40);

        rectangle(comp[i].xcomponenta,comp[i].ycomponenta-50,comp[i].xcomponenta-20,comp[i].ycomponenta-30);
        rectangle(comp[i].xcomponenta-20,comp[i].ycomponenta-50,comp[i].xcomponenta-40,comp[i].ycomponenta-30);
        rectangle(comp[i].xcomponenta-40,comp[i].ycomponenta-50,comp[i].xcomponenta-60,comp[i].ycomponenta-30);
        rectangle(comp[i].xcomponenta,comp[i].ycomponenta-50,comp[i].xcomponenta-20,comp[i].ycomponenta-70);
        rectangle(comp[i].xcomponenta-20,comp[i].ycomponenta-50,comp[i].xcomponenta-40,comp[i].ycomponenta-70);
        rectangle(comp[i].xcomponenta-40,comp[i].ycomponenta-50,comp[i].xcomponenta-60,comp[i].ycomponenta-70);
        rectangle(comp[i].xcomponenta-60,comp[i].ycomponenta-50,comp[i].xcomponenta-80,comp[i].ycomponenta-70);
        rectangle(comp[i].xcomponenta-60,comp[i].ycomponenta-50,comp[i].xcomponenta-80,comp[i].ycomponenta-30);
    }
    else if(comp[i].ycomponenta<=500&&comp[i].ycomponenta>=100&&comp[i].xcomponenta>=700)
    {
        setfillstyle(SOLID_FILL,culoareMeniu);
        bar(comp[i].xcomponenta,comp[i].ycomponenta+50,comp[i].xcomponenta-20,comp[i].ycomponenta+30);
        setcolor(culoareFundal);
        line(comp[i].xcomponenta-2,comp[i].ycomponenta+40,comp[i].xcomponenta-18,comp[i].ycomponenta+40);
        line(comp[i].xcomponenta-10,comp[i].ycomponenta+48,comp[i].xcomponenta-10,comp[i].ycomponenta+32);
        line(comp[i].xcomponenta-7,comp[i].ycomponenta+45,comp[i].xcomponenta-10,comp[i].ycomponenta+48);
        line(comp[i].xcomponenta-13,comp[i].ycomponenta+45,comp[i].xcomponenta-10,comp[i].ycomponenta+48);
        line(comp[i].xcomponenta-7,comp[i].ycomponenta+35,comp[i].xcomponenta-10,comp[i].ycomponenta+32);
        line(comp[i].xcomponenta-13,comp[i].ycomponenta+35,comp[i].xcomponenta-10,comp[i].ycomponenta+32);
        line(comp[i].xcomponenta-2,comp[i].ycomponenta+40,comp[i].xcomponenta-5,comp[i].ycomponenta+43);
        line(comp[i].xcomponenta-2,comp[i].ycomponenta+40,comp[i].xcomponenta-5,comp[i].ycomponenta+37);
        line(comp[i].xcomponenta-18,comp[i].ycomponenta+40,comp[i].xcomponenta-15,comp[i].ycomponenta+43);
        line(comp[i].xcomponenta-18,comp[i].ycomponenta+40,comp[i].xcomponenta-15,comp[i].ycomponenta+37);

        bar(comp[i].xcomponenta-20,comp[i].ycomponenta+50,comp[i].xcomponenta-40,comp[i].ycomponenta+30);
        ellipse(comp[i].xcomponenta-30,comp[i].ycomponenta+45,0,180,6,8);
        line(comp[i].xcomponenta-25,comp[i].ycomponenta+45,comp[i].xcomponenta-23,comp[i].ycomponenta+43);
        line(comp[i].xcomponenta-25,comp[i].ycomponenta+45,comp[i].xcomponenta-27,comp[i].ycomponenta+43);

        bar(comp[i].xcomponenta-40,comp[i].ycomponenta+50,comp[i].xcomponenta-60,comp[i].ycomponenta+30);
        ellipse(comp[i].xcomponenta-50,comp[i].ycomponenta+45,0,180,6,8);
        line(comp[i].xcomponenta-54,comp[i].ycomponenta+45,comp[i].xcomponenta-52,comp[i].ycomponenta+43);
        line(comp[i].xcomponenta-54,comp[i].ycomponenta+45,comp[i].xcomponenta-56,comp[i].ycomponenta+43);

        bar(comp[i].xcomponenta,comp[i].ycomponenta+50,comp[i].xcomponenta-20,comp[i].ycomponenta+70);
        line(comp[i].xcomponenta-5,comp[i].ycomponenta+55,comp[i].xcomponenta-15,comp[i].ycomponenta+65);
        line(comp[i].xcomponenta-15,comp[i].ycomponenta+55,comp[i].xcomponenta-5,comp[i].ycomponenta+65);

        bar(comp[i].xcomponenta-20,comp[i].ycomponenta+50,comp[i].xcomponenta-40,comp[i].ycomponenta+70);
        settextstyle(8,HORIZ_DIR,1);
        setbkcolor(culoareMeniu);
        setcolor(culoareFundal);
        outtextxy(comp[i].xcomponenta-35,comp[i].ycomponenta+51,"R");

        bar(comp[i].xcomponenta-40,comp[i].ycomponenta+50,comp[i].xcomponenta-60,comp[i].ycomponenta+70);
        outtextxy(comp[i].xcomponenta-55,comp[i].ycomponenta+51,"N");

        bar(comp[i].xcomponenta-60,comp[i].ycomponenta+50,comp[i].xcomponenta-80,comp[i].ycomponenta+70);//+
        line(comp[i].xcomponenta-70,comp[i].ycomponenta+56,comp[i].xcomponenta-70,comp[i].ycomponenta+65);
        line(comp[i].xcomponenta-65,comp[i].ycomponenta+60,comp[i].xcomponenta-75,comp[i].ycomponenta+60);

        bar(comp[i].xcomponenta-60,comp[i].ycomponenta+50,comp[i].xcomponenta-80,comp[i].ycomponenta+30);//-
        line(comp[i].xcomponenta-65,comp[i].ycomponenta+40,comp[i].xcomponenta-75,comp[i].ycomponenta+40);

        rectangle(comp[i].xcomponenta,comp[i].ycomponenta+50,comp[i].xcomponenta-20,comp[i].ycomponenta+70);
        rectangle(comp[i].xcomponenta-20,comp[i].ycomponenta+50,comp[i].xcomponenta-40,comp[i].ycomponenta+70);
        rectangle(comp[i].xcomponenta-40,comp[i].ycomponenta+50,comp[i].xcomponenta-60,comp[i].ycomponenta+70);
        rectangle(comp[i].xcomponenta,comp[i].ycomponenta+50,comp[i].xcomponenta-20,comp[i].ycomponenta+30);
        rectangle(comp[i].xcomponenta-20,comp[i].ycomponenta+50,comp[i].xcomponenta-40,comp[i].ycomponenta+30);
        rectangle(comp[i].xcomponenta-40,comp[i].ycomponenta+50,comp[i].xcomponenta-60,comp[i].ycomponenta+30);
        rectangle(comp[i].xcomponenta-60,comp[i].ycomponenta+50,comp[i].xcomponenta-80,comp[i].ycomponenta+70);
        rectangle(comp[i].xcomponenta-60,comp[i].ycomponenta+50,comp[i].xcomponenta-80,comp[i].ycomponenta+30);
    }
    else if(comp[i].ycomponenta<=500&&comp[i].ycomponenta>=100&&comp[i].xcomponenta<=700)
    {
        setfillstyle(SOLID_FILL,culoareMeniu);
        bar(comp[i].xcomponenta,comp[i].ycomponenta+50,comp[i].xcomponenta+20,comp[i].ycomponenta+30);
        setcolor(culoareFundal);
        line(comp[i].xcomponenta+2,comp[i].ycomponenta+40,comp[i].xcomponenta+18,comp[i].ycomponenta+40);
        line(comp[i].xcomponenta+10,comp[i].ycomponenta+48,comp[i].xcomponenta+10,comp[i].ycomponenta+32);
        line(comp[i].xcomponenta+7,comp[i].ycomponenta+45,comp[i].xcomponenta+10,comp[i].ycomponenta+48);
        line(comp[i].xcomponenta+13,comp[i].ycomponenta+45,comp[i].xcomponenta+10,comp[i].ycomponenta+48);
        line(comp[i].xcomponenta+7,comp[i].ycomponenta+35,comp[i].xcomponenta+10,comp[i].ycomponenta+32);
        line(comp[i].xcomponenta+13,comp[i].ycomponenta+35,comp[i].xcomponenta+10,comp[i].ycomponenta+32);
        line(comp[i].xcomponenta+2,comp[i].ycomponenta+40,comp[i].xcomponenta+5,comp[i].ycomponenta+43);
        line(comp[i].xcomponenta+2,comp[i].ycomponenta+40,comp[i].xcomponenta+5,comp[i].ycomponenta+37);
        line(comp[i].xcomponenta+18,comp[i].ycomponenta+40,comp[i].xcomponenta+15,comp[i].ycomponenta+43);
        line(comp[i].xcomponenta+18,comp[i].ycomponenta+40,comp[i].xcomponenta+15,comp[i].ycomponenta+37);

        bar(comp[i].xcomponenta+20,comp[i].ycomponenta+50,comp[i].xcomponenta+40,comp[i].ycomponenta+30);
        ellipse(comp[i].xcomponenta+30,comp[i].ycomponenta+45,0,180,6,8);
        line(comp[i].xcomponenta+34,comp[i].ycomponenta+45,comp[i].xcomponenta+32,comp[i].ycomponenta+43);
        line(comp[i].xcomponenta+34,comp[i].ycomponenta+45,comp[i].xcomponenta+36,comp[i].ycomponenta+43);

        bar(comp[i].xcomponenta+40,comp[i].ycomponenta+50,comp[i].xcomponenta+60,comp[i].ycomponenta+30);
        ellipse(comp[i].xcomponenta+50,comp[i].ycomponenta+45,0,180,6,8);
        line(comp[i].xcomponenta+45,comp[i].ycomponenta+45,comp[i].xcomponenta+43,comp[i].ycomponenta+43);
        line(comp[i].xcomponenta+45,comp[i].ycomponenta+45,comp[i].xcomponenta+47,comp[i].ycomponenta+43);

        bar(comp[i].xcomponenta,comp[i].ycomponenta+50,comp[i].xcomponenta+20,comp[i].ycomponenta+70);
        line(comp[i].xcomponenta+5,comp[i].ycomponenta+55,comp[i].xcomponenta+15,comp[i].ycomponenta+65);
        line(comp[i].xcomponenta+15,comp[i].ycomponenta+55,comp[i].xcomponenta+5,comp[i].ycomponenta+65);

        bar(comp[i].xcomponenta+20,comp[i].ycomponenta+50,comp[i].xcomponenta+40,comp[i].ycomponenta+70);//R
        settextstyle(8,HORIZ_DIR,1);
        setbkcolor(culoareMeniu);
        setcolor(culoareFundal);
        outtextxy(comp[i].xcomponenta+25,comp[i].ycomponenta+51,"R");

        bar(comp[i].xcomponenta+40,comp[i].ycomponenta+50,comp[i].xcomponenta+60,comp[i].ycomponenta+70);//N
        outtextxy(comp[i].xcomponenta+45,comp[i].ycomponenta+51,"N");

        bar(comp[i].xcomponenta+60,comp[i].ycomponenta+50,comp[i].xcomponenta+80,comp[i].ycomponenta+70);//+
        line(comp[i].xcomponenta+70,comp[i].ycomponenta+55,comp[i].xcomponenta+70,comp[i].ycomponenta+65);
        line(comp[i].xcomponenta+65,comp[i].ycomponenta+60,comp[i].xcomponenta+75,comp[i].ycomponenta+60);

        bar(comp[i].xcomponenta+60,comp[i].ycomponenta+50,comp[i].xcomponenta+80,comp[i].ycomponenta+30);//-
        line(comp[i].xcomponenta+65,comp[i].ycomponenta+40,comp[i].xcomponenta+75,comp[i].ycomponenta+40);

        rectangle(comp[i].xcomponenta,comp[i].ycomponenta+50,comp[i].xcomponenta+20,comp[i].ycomponenta+30);
        rectangle(comp[i].xcomponenta+20,comp[i].ycomponenta+50,comp[i].xcomponenta+40,comp[i].ycomponenta+30);
        rectangle(comp[i].xcomponenta+40,comp[i].ycomponenta+50,comp[i].xcomponenta+60,comp[i].ycomponenta+30);
        rectangle(comp[i].xcomponenta,comp[i].ycomponenta+50,comp[i].xcomponenta+20,comp[i].ycomponenta+70);
        rectangle(comp[i].xcomponenta+20,comp[i].ycomponenta+50,comp[i].xcomponenta+40,comp[i].ycomponenta+70);
        rectangle(comp[i].xcomponenta+40,comp[i].ycomponenta+50,comp[i].xcomponenta+60,comp[i].ycomponenta+70);
        rectangle(comp[i].xcomponenta+60,comp[i].ycomponenta+50,comp[i].xcomponenta+80,comp[i].ycomponenta+70);
        rectangle(comp[i].xcomponenta+60,comp[i].ycomponenta+50,comp[i].xcomponenta+80,comp[i].ycomponenta+30);
    }
}
void interfata()//ecranul unde se plaseaza componentele //Comun
{
    initwindow(1496,800,"Electron");
    while(meniu!=0)
    {
        while(meniu==ecranCircuit)
        {
            int x,y;
            setbkcolor(culoareBackg);
            setfillstyle(stilMeniu,culoareFundal);
            bar(0,0,1500,800);
            for(int i=1;i<=numarComponente;i++)
            {
                if(comp[i].plasat==1)
                {
                   settextstyle(13,HORIZ_DIR,20);
                   setbkcolor(culoareFundal);
                   setcolor(culoareText);
                   settextjustify(LEFT_TEXT,TOP_TEXT);
                   outtextxy(comp[i].xcomponenta-textwidth(comp[i].numeComponenta)/2,comp[i].ycomponenta-2*textheight(comp[i].numeComponenta),comp[i].numeComponenta);
                   outtextxy(comp[i].xcomponenta-textwidth(comp[i].rezistenta)/2,comp[i].ycomponenta+textheight(comp[i].rezistenta),comp[i].rezistenta);
                }
            }
            setfillstyle(1,culoareMeniu);
            bar(0,0,180,70);
            setcolor(RED);
            bar(0,70,180,100);
            rectangle(0,0,180,70);
            rectangle(0,70,180,100);
            setcolor(culoareComponente);
            setbkcolor(culoareMeniu);
            settextstyle(8,HORIZ_DIR, 7);
            outtextxy(15,5,"MENU");
            settextstyle(8,HORIZ_DIR, 2);
            outtextxy(5,75,"Sterge Circuit");
            int i=180;
            while(i<1500)
            {
                setfillstyle(SOLID_FILL,culoareMeniu);
                bar(i,0,i+(1500-180)/13,100);
                setcolor(RED);
                rectangle(i,0,i+(1500-180)/13,100);
                i=i+(1500-180)/13;
            }
            setcolor(culoareText);
            settextstyle(13,HORIZ_DIR,20);
            outtextxy(195,5,"Amplop");
            outtextxy(300,5,"Baterie");
            outtextxy(400,5,"Condens");
            outtextxy(511,5,"Dioda");
            outtextxy(612,5,"Nod");
            outtextxy(713,5,"Polariz");
            outtextxy(814,5,"Rezist");
            outtextxy(900,5,"Servomot");
            outtextxy(1016,5,"Sinu");
            outtextxy(1117,5,"Stop");
            outtextxy(1200,5,"TranzNPN");
            outtextxy(1300,5,"TranzPNP");
            outtextxy(1410,5,"Zenner");
            x = 230;
            y = 60;
            for(int i = 1; i <= 13; i++)
            {
                for(int j = 1; j <= tip_desen[i].nr_operatii; j++)
                {
                    setcolor(culoareComponente);
                    if(tip_desen[i].tip_linie[j] == 'L')
                    {
                        line(x + tip_desen[i].coordonate[j][1] * tip_desen[i].zoom, y + tip_desen[i].coordonate[j][2] * tip_desen[i].zoom, x + tip_desen[i].coordonate[j][3] * tip_desen[i].zoom, y + tip_desen[i].coordonate[j][4] * tip_desen[i].zoom);
                    }
                    else if(tip_desen[i].tip_linie[j] == 'R')
                    {
                        rectangle(x + tip_desen[i].coordonate[j][1] * tip_desen[i].zoom, y + tip_desen[i].coordonate[j][2] * tip_desen[i].zoom, x + tip_desen[i].coordonate[j][3] * tip_desen[i].zoom, y + tip_desen[i].coordonate[j][4] * tip_desen[i].zoom);
                    }
                    else if(tip_desen[i].tip_linie[j] == 'O')
                    {
                        ellipse(x,y,0,360,tip_desen[i].coordonate[j][3] * tip_desen[i].zoom,tip_desen[i].coordonate[j][4]*tip_desen[i].zoom);
                    }
                    else if(tip_desen[i].tip_linie[j] == 'C')
                    {
                        circle(x, y, 20);
                    }
                    else if(tip_desen[i].tip_linie[j] == 'A')
                    {
                        arc(x+tip_desen[i].coordonate[j][1]*tip_desen[i].zoom,y+tip_desen[i].coordonate[j][2]*tip_desen[i].zoom, 270, 90, tip_desen[i].coordonate[j][3]*tip_desen[i].zoom);
                    }
                }
                x+=101;
            }
            for(i=1;i<=numarComponente;i++)
            {
                if(comp[i].plasat==1)
                {
                   desenareComponenta(i, comp[i].xcomponenta, comp[i].ycomponenta,culoareComponente);
                }
            }
            for(i=1;i<=numarLegaturi;i++)
            {
                trasareCablu(leg[i].x1,leg[i].y1,leg[i].x2,leg[i].y2,culoareLegaturi,leg[i].tipLeg);
            }
            desenareMeniuComponenta(componentaSelectata);
            if(verifCircuit()==true)
            {
                settextstyle(BOLD_FONT, HORIZ_DIR,2);
                setbkcolor(GREEN);
                setcolor(BLACK);
                outtextxy(0,770,"VALID");
            }
            else if(verifCircuit()==false)
            {
                settextstyle(BOLD_FONT, HORIZ_DIR,2);
                setbkcolor(RED);
                setcolor(BLACK);
                outtextxy(0,770,"INVALID");
            }
            while(2>1)
            {
                userInput();
                if(executatInput==true)
                {
                    executatInput=false;
                    break;
                }
            }
        }
        while(meniu==meniuPrincipal)
        {
            setbkcolor(culoareBackg);
            setfillstyle(stilFundal,culoareMeniu);
            bar(0,0,1500,800);
            setcolor(culoareText);
            settextstyle(BOLD_FONT, HORIZ_DIR, 9);
            setbkcolor(culoareMeniu);
            outtextxy(560,30,"ELECTRON");
            settextstyle(8,HORIZ_DIR, 9) ;

            setbkcolor(culoareFundalText);
            outtextxy(575,200,"CIRCUIT");
            outtextxy(575,350,"OPTIUNI");
            outtextxy(575,500,"SALVARE");
            outtextxy(520,650,"INCARCARE");
            settextstyle(8,HORIZ_DIR, 4) ;
            outtextxy(0,750,"NECHIFOR ALEXANDRU");
            outtextxy(1225,750,"ANTOHI TUDOR");
            settextstyle(8,HORIZ_DIR, 1) ;
            outtextxy(1,1,"Professor of computer science");
            outtextxy(1,21,"Bogdan Patrut");
            outtextxy(1490-textwidth("Universitatea Alexandru Ioan Cuza din Iasi"),1,"Universitatea Alexandru Ioan Cuza din Iasi");
            settextstyle(8,HORIZ_DIR, 9);
            while(2>1)
            {
                userInput();
                if(executatInput==true)
                {
                    executatInput=false;
                    break;
                }
            }
        }
        while(meniu==meniuCustomizare)
        {
            setfillstyle(stilFundal,culoareMeniu);
            bar(0,0,1496,800);
            setbkcolor(culoareMeniu);
            setcolor(culoareText);
            settextstyle(BOLD_FONT,HORIZ_DIR,4);
            outtextxy(10,100,"Culoare fundal");
            outtextxy(10,200,"Culoare meniu");
            outtextxy(10,300,"Culoare componente");
            outtextxy(10,400,"Culoare legaturi");
            outtextxy(10,500,"Culoare text");
            settextstyle(BOLD_FONT,HORIZ_DIR,3);
            outtextxy(10,600,"Culoare fundal text meniu");
            for(int i=1;i<=6;i++)
            {
                for(int j=0;j<=15;j++)
                {
                    setfillstyle(SOLID_FILL,j);
                    bar(370+j*(1400-370)/16,100+(i-1)*(660-100)/6,370+(j+1)*(1400-370)/16,100+i*(660-100)/6);
                    setcolor(BLACK);
                    rectangle(370+j*(1400-370)/16,100+(i-1)*(660-100)/6,370+(j+1)*(1400-370)/16,100+i*(660-100)/6);
                    if(i==1&&culoareFundal==j)
                    {
                        if(j==0) setcolor(WHITE);
                        else setcolor(BLACK);
                        line(370+j*(1400-370)/16,100+(i-1)*(660-100)/6,370+(j+1)*(1400-370)/16,100+i*(660-100)/6);
                        line(370+j*(1400-370)/16,100+i*(660-100)/6,370+(j+1)*(1400-370)/16,100+(i-1)*(660-100)/6);
                    }
                    else if(i==2&&culoareMeniu==j)
                    {
                        if(j==0) setcolor(WHITE);
                        else setcolor(BLACK);
                        line(370+j*(1400-370)/16,100+(i-1)*(660-100)/6,370+(j+1)*(1400-370)/16,100+i*(660-100)/6);
                        line(370+j*(1400-370)/16,100+i*(660-100)/6,370+(j+1)*(1400-370)/16,100+(i-1)*(660-100)/6);
                    }
                    else if(i==3&&culoareComponente==j)
                    {
                        if(j==0) setcolor(WHITE);
                        else setcolor(BLACK);
                        line(370+j*(1400-370)/16,100+(i-1)*(660-100)/6,370+(j+1)*(1400-370)/16,100+i*(660-100)/6);
                        line(370+j*(1400-370)/16,100+i*(660-100)/6,370+(j+1)*(1400-370)/16,100+(i-1)*(660-100)/6);
                    }
                    else if(i==4&&culoareLegaturi==j)
                    {
                        if(j==0) setcolor(WHITE);
                        else setcolor(BLACK);
                        line(370+j*(1400-370)/16,100+(i-1)*(660-100)/6,370+(j+1)*(1400-370)/16,100+i*(660-100)/6);
                        line(370+j*(1400-370)/16,100+i*(660-100)/6,370+(j+1)*(1400-370)/16,100+(i-1)*(660-100)/6);
                    }
                    else if(i==5&&culoareText==j)
                    {
                        if(j==0) setcolor(WHITE);
                        else setcolor(BLACK);
                        line(370+j*(1400-370)/16,100+(i-1)*(660-100)/6,370+(j+1)*(1400-370)/16,100+i*(660-100)/6);
                        line(370+j*(1400-370)/16,100+i*(660-100)/6,370+(j+1)*(1400-370)/16,100+(i-1)*(660-100)/6);
                    }
                    else if(i==6&&culoareFundalText==j)
                    {
                        if(j==0) setcolor(WHITE);
                        else setcolor(BLACK);
                        line(370+j*(1400-370)/16,100+(i-1)*(660-100)/6,370+(j+1)*(1400-370)/16,100+i*(660-100)/6);
                        line(370+j*(1400-370)/16,100+i*(660-100)/6,370+(j+1)*(1400-370)/16,100+(i-1)*(660-100)/6);
                    }
                }
                setcolor(RED);
                bar(1400,700,1450,750);
                setcolor(BLACK);
                line(1410,725,1440,725);
                line(1430,710,1440,725);
                line(1430,740,1440,725);
            }
            while(2>1)
            {
                userInput();
                if(executatInput==true)
                {
                    executatInput=false;
                    break;
                }
            }
        }
    }
}
int main()
{
    initializare();
    interfata();
    return 0;
}
