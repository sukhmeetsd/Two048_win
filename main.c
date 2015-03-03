#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include <string.h>
#include<windows.h>
#define MAXSAVE 20
#define MAXHALL 10
#define MAXNODES 15
typedef struct attributes attrib;
typedef struct hof hall;
typedef struct node lnode;
int **a, **b, **r, x, y, n, flag, won, cheated=0, lost, checkedHOF, highestOnBoard;
double sec;
long int score;
struct attributes
{
    int a[20][20], b[20][20], r[20][20], x, y, n, flag, won, cheated, lost;
    long int score;
    char tim[40];
};
struct hof
{
    int score,n,lost;
    char tim[40];
    char name[16];
};
struct node
{
    attrib att;
    lnode *next;
    lnode *prev;
};
lnode *rear;
lnode *front;
lnode *present;
int nodeCount, superCheater, smthgWzMoved;
void redoUndo(char);
char ch;
time_t start_t,end_t;
double diff_t;
//int compareMat(attrib);
void reserveR(char);
hall punchTime(hall);
attrib punchTimeAT(attrib);
void pushElements(char,int);
void prnmat();
char *strtrim(char *);
void prnSavedGames(FILE *);
void cheat();
int filled();
void setAttributes(attrib);
void play(int);
void getControl();
void initialize();
void showControls();
char getEqChar(char);
void savePresentStatus(FILE *);
void revokeLastSaved();
void saveGame();
int loadGame();
void freeall();
void freeList();
int checkWinOrLoss();
attrib getStruct();
void checkAndWriteHOF();
void reserveNode();
void prnTimeElapsed(double);



void printMenu()
{
    int i;
    char choice;
    do
    {
        system("cls");
        printf("\n>>>>>>>>>>>> Welcome to SSD\'s 2048 <<<<<<<<<<<<\n\n");
        printf("Select from the following menu : \n\n");
        printf("1.) New Game\n");
        printf("2.) Load Last Saved\n");
        printf("3.) Save Game\n");
        printf("4.) Load Game\n");
        printf("5.) Show Controls\n");
        printf("6.) Hall of Fame\n");
        printf("8.) Resume Game\n");
        printf("9.) Exit\n");
        fflush(stdin);
        choice = getch();
        switch(choice)
        {
        case '1':
            printf("\n\nPress m to see controls at anytime\n");
            getch();
            system("cls");
            printf("\n>>>>>>>>>>>> Welcome to SSD\'s 2048 <<<<<<<<<<<<\n\n");
            printf("Enter the order of matrix : ");
            scanf("%d",&n);
            printf("\n");
            if(n<2||n>20)
            {
                printf("\nInvalid Input!!\n");
                getch();
                continue;
            }
            initialize();
            /*FILE *fp;
            fp = fopen("lastState","wb");
            if(score>50) savePresentStatus(fp);
            freeall();
            initialize();*/
            play(0);
            break;
        case '2':
            revokeLastSaved();
            break;
        case '3':
            if(n==0)
            {
                printf("\n\nNo game in Progress!!\n");
                getch();
                break;
            }
            saveGame();
            break;
        case '4':
            initialize();
            i=loadGame();
            if(i==1) break;
            play(1);
            break;
        case '5':
            showControls();
            break;
        case '6':
            showHOF();
            break;
        case '8':
            if(n!=0) play(1);
            else
            {
                printf("\n\nAt least start a Game first!!\n\n");
                getch();
            }
            break;
        case '9':
            printf("\n\nThanx for playing!!\n\n");
            getch();
            if(lost!=1&&!checkedHOF&&score>0) checkAndWriteHOF();
            checkedHOF=1;
            exit(0);
        }
    } //DO ends here
    while(choice!='9');
}
void showHOF()
{
    printf("\n\n>>>>>  H A L L   O F   F A M E  <<<<<\n\n");
    FILE *fp=fopen("HOF","rb");
    if(fp==NULL)
    {
        printf("\n\nThere's no one in the HOF yet. Play and be the first one to enter!!\n");
        getch();
        return;
    }
    int hs;
    hall h[MAXHALL];
    fread(&hs,sizeof(int),1,fp);
    fread(h,sizeof(h),1,fp);
    char strr[10];
    strcpy(strr,"Player");
    printf("\n\nSe.No.\t%-16s\tOrder\tScore\tMaxOnBoard\tTime Saved\n\n\n\n",strr);
    int i;
    for(i=0; i<hs; i++) printf("%d\t%-16s\t%d\t%d\t%d\t\t%s\n",i+1,strtrim(h[i].name),h[i].n,h[i].score,h[i].lost,strtrim(h[i].tim));
    getch();
    fclose(fp);
}
char *strtrim(char *str)
{
    int i=0,ctr,flag=0,index=-1;
    char *s;
    for(i=0; i<40; i++)
    {
        if(!flag&&str[i]!=' ')
        {
            flag=1;
            index=i;
        }
        if(flag) ctr++;
        if(flag&&str[i]=='\0') break;
    }
    s=(char *)malloc(ctr*sizeof(char));
    for(i=index; str[i]!='\0'; i++) s[i-index]=str[i];
    s[i]='\0';
    return s;
}
void checkAndWriteHOF()
{
    if(cheated)
    {
        printf("\nYou have cheated %d times and will not be allowed to enter the HOF!!\n",cheated);
        char c=getch();
        if(c!='x') return;
        else printf("\n\nWelcome ji, thonu, maha-cheater nu, agge wadhan da mauka ditta javega\n");
        getch();
    }
    FILE *fp=fopen("HOF","rb");
    hall h[MAXHALL];
    int hallStrength=0;
    rewind(fp);
    fread(&hallStrength,sizeof(int),1,fp);
    fread(h,sizeof(h),1,fp);
    //printf("HallStrength = %d",hallStrength);getch();
    int pos=updateArrayHOF(hallStrength,&h);
    //printf("hallStrength ehe a = %d. %d ehe pos a, karda phos aa :D",hallStrength,pos);getch();
    if(pos!=-1)
    {
        fclose(fp);
        FILE *fp=fopen("HOF","wb");
        fwrite(&pos,sizeof(pos),1,fp);
        fwrite(h,sizeof(h),1,fp);
        printf("\nCongratulations you made it to the H A L L   O F   F A M E!!\n");
        getch();
        fclose(fp);
        showHOF();
    }
}
int updateArrayHOF(int hs, hall *h)
{
    int i=0;
    hall temph;
    temph.lost=highestOnBoard;
    temph.score=score;
    temph.n=n;
    temph=punchTime(temph);
    for(i=0; i<hs; i++)
    {
        if((h[i].score<score)||((h[i].score==score)&&(h[i].lost<highestOnBoard)))
        {
            if(!won)
            {
                printf("\n\nYou have got a good score, but to make it to the revered HOF, you need to at least get 2048 on the board....\n");
                char c=getch();
                if(c!='x') return -1;
                else printf("\n\nBai ji, kinnik cheating karonge!? Chalo, allow kar dinne a ;) !!\n");
            }
            int j=0;
            if(hs==MAXHALL) j=--hs;
            else j=hs;
            for(; j>=i+1; j--) h[j]=h[j-1];
            printf("\n\nEnter Your name(Better keep it less than 16 letters) :");
            gets(temph.name);
            printf("\n");
            h[i]=temph;
            return ++hs;
        }
    }
    if(hs<MAXHALL)
    {
        if(!won)
        {
            printf("\n\nYou have got a good score, but to make it to the revered HOF, you need to at least get 2048 on the board\n");
            char c=getch();
            if(c!='x') return -1;
            else printf("\n\nBai ji, kinnik cheating karonge!? Chalo, allow kar dinne a ;) !!\n");
        }
        printf("\n\nEnter Your name :");
        gets(temph.name);
        printf("\n");
        h[i]=temph;
        return ++hs;
    }
    else return -1;
}
int loadGame()
{
    FILE *fp = fopen("Attribs","r+b");
    if(fp==NULL)
    {
        printf("File Missing! Reinstall the game!");
        getch();
        return 1;
    }
    prnSavedGames(fp);
    fflush(stdin);
    printf("\nChoose which one to load : ");
    char *cho;
    gets(cho);
    if(cho[0]<'0'||cho[0]>'9')
    {
        printf("\nInvalid input!!\n");
        getch();
        return 1;
    }
    int x=atoi(cho);
    rewind(fp);
    fseek(fp,(x-1)*sizeof(attrib),SEEK_SET);
    attrib loadAttrib;
    fread(&loadAttrib,sizeof(attrib),1,fp);
    if(loadAttrib.score==-1)
    {
        printf("\n\nSlot no. %d is empty!\n\n",x);
        getch();
        return 1;
    }
    freeList();
    setAttributes(loadAttrib);
    fclose(fp);
    return 0;
}
void saveGame()
{

    //printf("Current local time and date : %s", asctime(timeinfo));getch();
    attrib toPunch = getStruct();
    FILE *fp = fopen("Attribs","r+b");
    //prnSavedGames(fp);
    //printf("Print kareyaya\n");getch();
    rewind(fp);
    if(fp!=NULL)
    {
        attrib temp;
        int ctr=0;
        while(fread(&temp, sizeof(attrib),1,fp)==1) if(temp.score!=-1) ctr++;
            else break;
        //printf("Counted fills ctr=%d\n",ctr);getch();
        if(ctr==MAXSAVE)
        {
            printf("All slots occupied!\n");
            getch();
            prnSavedGames(fp);
            printf("\nChoose which slot to overwrite(Enter Return if you don't want to overwrite) : \n");
            /*char *c;
            //fgets(c,20,stdin);
            //fflush(stdin);
            gets(c);*/
            char c[20];
            scanf("%19s",c);
            int topun;
            if(c[0]=='r'||c[0]=='R') return;
            else topun=atoi(c);
            if(topun>MAXSAVE)
            {
                printf("Invalid Input!\n");
                getch();
                return;
            }
            fseek(fp, (topun-1)*sizeof(attrib), SEEK_SET);
            toPunch=punchTimeAT(toPunch);
            fwrite(&toPunch,sizeof(attrib),1,fp);
        }
        else
        {
            fseek(fp,-sizeof(attrib),SEEK_CUR);
            toPunch=punchTimeAT(toPunch);
            if(fwrite(&toPunch,sizeof(attrib),1,fp)<1)
            {
                printf("Some Error occured, game not saved!! :(\n");
                getch();
            }
            else printf("Game Saved!! :)\n");
            getch();
        }
    }
    else
    {
        printf("File Missing! Reinstall the game!\n");
        getch();
    }
    fclose(fp);
    //printf("File Closed!!");getch();
}
hall punchTime(hall obj)
{
    time_t rawtime;
    struct tm* timeinfo;
    time(&rawtime);
    timeinfo=localtime(&rawtime);
    strcpy(obj.tim, asctime(timeinfo));
    return obj;
}
attrib punchTimeAT(attrib obj)
{
    time_t rawtime;
    struct tm* timeinfo;
    time(&rawtime);
    timeinfo=localtime(&rawtime);
    strcpy(obj.tim, asctime(timeinfo));
    return obj;
}
void prnSavedGames(FILE *fp)
{
    rewind(fp);
    printf("\nNo.\tOrder\tScore\tTime\n\n");
    attrib temp;
    int ctr=1;
    fread(&temp,sizeof(attrib),1,fp);
    while(!feof(fp)&&!ferror(fp))
    {
        if(temp.score==-1) printf("%d.) \tEmpty Slot\n", ctr++);
        else
        {
            fflush(stdin);
            printf("%d.)\t%d\t%ld\t",ctr++,temp.n,temp.score);
            int i=0;
            for(i=0; i<strlen(temp.tim); i++)
                if(temp.tim[i]!='\0') printf("%c",temp.tim[i]);
                else
                {
                    printf("\n");
                    break;
                }
        }
        fread(&temp,sizeof(attrib),1,fp);
    }
    rewind(fp);
    //printf("While tobbar\n"); getch();
}
attrib getStruct()
{
    //printf("ingetStruct");getch();
    attrib curr;
    int i,j;
    for(i=0; i<n; i++) for(j=0; j<n; j++) curr.a[i][j]=a[i][j];
    for(i=0; i<n; i++) for(j=0; j<n; j++) curr.b[i][j]=b[i][j];
    for(i=0; i<n; i++) for(j=0; j<n; j++) curr.r[i][j]=r[i][j];
    curr.x=x;
    curr.y=y;
    curr.n=n;
    curr.flag=flag;
    curr.won=won;
    curr.cheated=cheated;
    curr.score=score;
    curr.lost=lost;
    return curr;
}
void revokeLastSaved()
{
    FILE *fp;
    attrib lastSt;
    fflush(stdin);
    fp=fopen("lastState", "rb");
    if(fp!=NULL)
    {
        //printf("Inside\n");getch();
        if(fread(&lastSt,sizeof(lastSt),1,fp)!=1) perror("Ehe ho geya bai");
        if(ferror(fp)) printf("Error");
        //exit(0);
        //printf("a and b's first value = %d and %d",a[0][0],b[0][0]);getch();
        printf("Time Siga : %s", lastSt.tim);
        getch();
        setAttributes(lastSt);
    }
    else
    {
        printf("You haven't played even once yet!!");
        getch();
        system("cls");
        printMenu();
    }
    play(1);
}
void setAttributes(attrib sts)         //Sets the attributes given to it
{
    n = sts.n;
    freeall();
    a=malloc(n*sizeof(int*));
    if(a==NULL) printf("Jagah Koni");
    b=malloc(n*sizeof(int*));
    if(b==NULL) printf("Jagah Koni");
    r=malloc(n*sizeof(int*));
    if(r==NULL) printf("Jagah Koni");
    int i=0, j=0;
    for(i=0; i<n; i++)
    {
        a[i]=malloc(n*sizeof(int));
        b[i]=malloc(n*sizeof(int));
        r[i]=malloc(n*sizeof(int));
    }
    for(i=0; i<n; i++) for(j=0; j<n; j++)
        {
            a[i][j]=sts.a[i][j];
            b[i][j]=sts.b[i][j];
            r[i][j]=sts.r[i][j];
        }
    x = sts.x;
    y = sts.y;
    flag = sts.flag;
    won = sts.won;
    cheated = sts.cheated;
    score = sts.score;
    lost = sts.lost;
}
void play(int loaded)
{
    if(!loaded)
    {
        checkedHOF=0;
        highestOnBoard=0;
        int x=rand()%n;
        int y=rand()%n;
        a[x][y]=2;
        b[x][y]=1;
        do
        {
            x=rand()%n;
            y=rand()%n;
        }
        while(b[x][y]);
        a[x][y]=2;
        b[x][y]=1;
        cheated=0;
    }
    prnmat();
    char c='\0';
    time(&start_t);
    do
    {
        FILE *fp = fopen("lastState","wb");
        savePresentStatus(fp);
        fclose(fp);
        if(((c=='a'||c=='w'||c=='s'||c=='d'||c=='A'||c=='W'||c=='S'||c=='D')&&smthgWzMoved)||loaded) reserveNode();
        loaded=0;               //Loaded set to 0 after the first use itself
        //printf("Lost = %d",lost);getch();
        printf("Enter response (W/A/S/D) : ");
        fflush(stdin);
        c=getch();
        if((int)c==-32) c=getEqChar(c);
        if(c=='u'||c=='U'||c=='r'||c=='R'||c=='7')
            redoUndo(c);
        if(c=='q')
        {
            printf("\n");
            int i=0,j;
            for(i=0; i<n; i++)
            {
                for(j=0; j<n; j++) printf("%d ",r[i][j]);
                printf("\n");
            }
            printf("\nThe direction was %c\n",ch);
            printf("The No. of times you have cheated  = %d", ++cheated);
            getch();
        }
        reserveR(c);
        if(c=='o')
        {
            FILE *fp;
            fp = fopen("lastState","wb");
            savePresentStatus(fp);
            fclose(fp);
            if(!checkedHOF) checkAndWriteHOF();
            checkedHOF=1;
            freeall();
            //freeList();
            n=0;
            //printf("LOST = %d",lost);
            continue;
        }
        smthgWzMoved=0;
        if(c=='a'||c=='w'||c=='s'||c=='d'||c=='A'||c=='W'||c=='S'||c=='D'||c=='c'||c=='x'||c=='m') pushElements(c,0);
        if(flag) smthgWzMoved=1;
        prnmat();
        if(c=='a'||c=='w'||c=='s'||c=='d'||c=='A'||c=='W'||c=='S'||c=='D'||c=='c'||c=='x'||c=='m') lost=checkWinOrLoss();
        if(lost==1)
        {
            printf("\n\n\tY O U    L O S E!!\n\n");
            time(&end_t);
            diff_t=difftime(end_t,start_t);
            printf("Time played : ");
            prnTimeElapsed(diff_t);
            printf("\n\n");
            getch();
            freeall();
            //freeList();
            n=0;
            if(!checkedHOF) checkAndWriteHOF();
            checkedHOF=1;
            return;
        }
        else if(lost>=2048&&!won)
        {
            printf("You made it to %d. Congratulations!! \n",lost);
            getch();
            won=1;
        }
    }
    while(c!='o');
    printf("Thanx for Playing!!\n");
    getch();
    if(!checkedHOF) checkAndWriteHOF();
    checkedHOF=1;
}
void prnTimeElapsed(double s)
{
    if(s==0) return;
    int m=(int)s/60;
    if(m) printf("%d Min ",m);
    int se=(int)s%60;
    if(se) printf("%d Sec",se);
}
void redoUndo(char c)
{
    if(c=='u'||c=='U'||c=='7')
    {
        if(c=='7'&&!superCheater)
        {
            superCheater=1;
            printf("\nYou are now a superCheater. Undo as many times as possible.\n");
            cheated++;
            getch();
        }
        else if(c=='7'&&superCheater)
        {
            printf("\nYou are already a superCheater man, why do you want to prove it?!?\n");
            getch();
        }
        if(present->prev!=NULL)
        {
            setAttributes(present->prev->att);
            present=present->prev;
        }
        else
        {
            printf("\nThats all the undoing possible!! ;)\n");
            getch();
        }
    }
    else
    {
        if(present->next!=NULL)
        {
            setAttributes(present->next->att);
            present=present->next;
        }
        else
        {
            printf("Thats all the playing you had done!!");
            getch();
        }
    }
}
void reserveNode()
{
    if(rear==NULL&&front==NULL)
    {
        rear=malloc(sizeof(lnode));
        front=present=rear;
        nodeCount++;
        front->att=getStruct();
        front->next=front->prev=NULL;
        return;
    }
    //int te;
    //if((te=compareMat(present->att))) {printf("%d",te);getch();return;}
    if(front!=present)
    {
        while(front!=present)
        {
            lnode *t;
            t=front;
            front=front->prev;
            free(t);
            nodeCount--;
        }
        front->next=NULL;
    }
    lnode *t=(lnode *) malloc(sizeof(lnode));
    front->next=t;
    t->prev=front;
    front=t;
    front->next=NULL;
    front->att=getStruct();
    present=front;
    nodeCount++;
    if(nodeCount>MAXNODES&&!superCheater)
    {
        lnode *tt;
        tt=rear;
        rear=rear->next;
        rear->prev=NULL;
        free(tt);
        nodeCount--;
    }
}
/*int compareMat(attrib aa)
{
    int i=0,j=0;
    for(i=0;i<20;i++) for(j=0;j<20;j++) if(a[i][j]!=aa.a[i][j]) return 0;
    return 1;
}*/
void savePresentStatus(FILE *fp)
{
    attrib presentStatus = getStruct();
    presentStatus=punchTimeAT(presentStatus);
    //printf("Time = %s",presentStatus.tim);
    fwrite(&presentStatus, sizeof(presentStatus), 1, fp);
}
char getEqChar(char c)
{
    if((int)c!=-32) return 'o';
    char c1=getch();
    switch((int)c1)
    {
    case 77:
        return 'd';
    case 80:
        return 's';
    case 75:
        return 'a';
    case 72:
        return 'w';
    default:
        return 'o';
    }
    return 'o';
}
int checkWinOrLoss()
{
    int i=0, j, resp[4], r;
    for(i=0; i<n; i++) for(j=0; j<n; j++) if(a[i][j]>=highestOnBoard&&b[i][j]==1) highestOnBoard = a[i][j];
    //printf("HIGH = %d\n",high);getch();
    for(i=0; i<4; i++)
    {
        r=flag=0;
        switch(i)
        {
        case 0:
            pushElements('w',1);
            r=flag;
            break;
        case 1:
            pushElements('a',1);
            r=flag;
            break;
        case 2:
            pushElements('s',1);
            r=flag;
            break;
        case 3:
            pushElements('d',1);
            r=flag;
            break;
        }
        resp[i]=r;
    }
    for(i=0; i<4; i++) if(resp[i]) return highestOnBoard;
    return 1;
}
void reserveR(char c)
{
    ch=c;
    int i=0,j=0;
    for(i=0; i<n; i++) for(j=0; j<n; j++) r[i][j]=a[i][j];
}
void pushElements(char c, int checking)
{
    int i=0;
    flag=0;
    for(i=0; i<2; i++)
    {
        switch(c)
        {
            int j=0;
        case 'w':

            if(i==0)
            {
                int k=0;
                for(k=0; k<n; k++)
                    for(j=0; j<(n-1); j++)
                        if(b[j][k])
                        {
                            int l=0;
                            for(l=j+1; l<n; l++)
                                if(b[l][k])
                                    if(a[j][k]==a[l][k])
                                        if(checking)
                                        {
                                            flag=1;
                                            return;
                                        }
                                        else
                                        {
                                            a[j][k]*=2;
                                            score+=a[j][k];
                                            b[l][k]=a[l][k]=0;
                                            flag=1;         //Flag serving the purpose of telling that a cell has been emptied
                                            j=l;
                                            break;
                                        }
                                    else break;
                        }
            }
            else
            {
                int k=0,l;
                for(k=0; k<n; k++)
                    for(j=0; j<n; j++)
                        if(b[j][k])
                        {
                            for(l=j-1; l>-1; l--)
                                if(b[l][k]) break;
                            if(l+1!=j)
                            {
                                flag=1;                     //Flag to show that things were moved
                                if(checking) return;
                                else
                                {
                                    a[l+1][k]=a[j][k];
                                    b[l+1][k]=1;
                                    a[j][k]=b[j][k]=0;
                                }
                            }
                        }
            }
            if(i==1&&!checking&&!filled()&&flag)
            {
                do
                {
                    x=rand()%n;
                    y=rand()%n;
                }
                while(b[x][y]);
                a[x][y]=2*(rand()%2+1);
                b[x][y]=1;
            }
            //int g=0;
            //for(g=0;g<10;g++) printf("%d\n",rand()%16);
            //getch();
            break;
        case 's':
            if(i==0)
            {
                //To club similar ones
                int k=0;
                for(k=0; k<n; k++)
                    for(j=n-1; j>0; j--)
                        if(b[j][k])
                        {
                            int l=0;
                            for(l=j-1; l>-1; l--)
                                if(b[l][k])
                                    if(a[j][k]==a[l][k])
                                        if(checking)
                                        {
                                            flag=1;
                                            return;
                                        }
                                        else
                                        {
                                            a[j][k]*=2;
                                            score+=a[j][k];
                                            flag=1;
                                            b[l][k]=a[l][k]=0;
                                            j=l;
                                            break;
                                        }
                                    else break;
                        }
            }
            else
            {
                int k=0,l;
                for(k=0; k<n; k++)
                    for(j=n-1; j>-1; j--)
                        if(b[j][k])
                        {
                            l=0;
                            for(l=j+1; l<n; l++)
                                if(b[l][k]) break;
                            if(l-1!=j)
                            {
                                flag=1;
                                if(checking) return;
                                else
                                {
                                    a[l-1][k]=a[j][k];
                                    b[l-1][k]=1;
                                    a[j][k]=b[j][k]=0;
                                }
                            }
                        }
            }
            if(i==1&&!checking&&!filled()&&flag)
            {
                do
                {
                    x=rand()%n;
                    y=rand()%n;
                }
                while(b[x][y]);
                a[x][y]=2*(rand()%2+1);
                b[x][y]=1;
            }
            break;
        case 'a':
            if(i==0)
            {
                int k=0;
                for(k=0; k<n; k++)
                    for(j=0; j<(n-1); j++)
                        if(b[k][j])
                        {
                            int l=0;
                            for(l=j+1; l<n; l++)
                                if(b[k][l])
                                    if(a[k][j]==a[k][l])
                                        if(checking)
                                        {
                                            flag=1;
                                            return;
                                        }
                                        else
                                        {
                                            a[k][j]*=2;
                                            score+=a[k][j];
                                            flag=1;
                                            b[k][l]=a[k][l]=0;
                                            j=l;
                                            break;
                                        }
                                    else break;
                        }
            }
            //To club them all
            else
            {
                int k=0,l;
                for(k=0; k<n; k++)
                    for(j=0; j<n; j++)
                        if(b[k][j])
                        {
                            for(l=j-1; l>-1; l--)
                                if(b[k][l]) break;
                            if(l+1!=j)
                            {
                                flag=1;
                                if(checking) return;
                                else
                                {
                                    a[k][l+1]=a[k][j];
                                    b[k][l+1]=1;
                                    a[k][j]=b[k][j]=0;
                                }
                            }
                        }
            }
            if(i==1&&!checking&&!filled()&&flag)
            {
                do
                {
                    x=rand()%n;
                    y=rand()%n;
                }
                while(b[x][y]);
                a[x][y]=2*(rand()%2+1);
                b[x][y]=1;
            }
            break;
        case 'd':
            if(i==0)
            {
                //To club similar ones
                int k=0;
                for(k=0; k<n; k++)
                    for(j=n-1; j>0; j--)
                        if(b[k][j])
                        {
                            int l=0;
                            for(l=j-1; l>-1; l--)
                                if(b[k][l])
                                    if(a[k][j]==a[k][l])
                                        if(checking)
                                        {
                                            flag=1;
                                            return;
                                        }
                                        else
                                        {
                                            a[k][j]*=2;
                                            score+=a[k][j];
                                            flag=1;
                                            b[k][l]=a[k][l]=0;
                                            j=l;
                                            break;
                                        }
                                    else break;
                        }
            }
            else
            {
                int k=0,l;
                for(k=0; k<n; k++)
                    for(j=n-1; j>-1; j--)
                        if(b[k][j])
                        {
                            l=0;
                            for(l=j+1; l<n; l++)
                                if(b[k][l]) break;
                            if(l-1!=j)
                            {
                                flag=1;
                                if(checking) return;
                                else
                                {
                                    a[k][l-1]=a[k][j];
                                    b[k][l-1]=1;
                                    a[k][j]=b[k][j]=0;
                                }
                            }
                        }
            }
            if(i==1&&!checking&&!filled()&&flag)
            {
                do
                {
                    x=rand()%n;
                    y=rand()%n;
                }
                while(b[x][y]);
                a[x][y]=2*(rand()%2+1);
                b[x][y]=1;
            }
            break;
        case 'c':
            printf("The No. of times you have cheated = %d\n", ++cheated);
            printf("x = %d y = %d. Value = %d",x,y,a[x][y]);
            int c=0;
            scanf("%d",&c);
            c = getNearestPowerOf2(c);
            a[x][y]=c;
            b[x][y]=1;
            i=2;
            break;
        case 'x':
            printf("The No. of times you have cheated = %d\n", ++cheated);
            getControl();
            i=2;
            break;
        case 'm':
            printMenu();
            i=2;
            break;
        default:
            printf("Wrong char given to pushElements!!");
            getch();
        }
    }
}
void showControls()
{
    printf("\nAll the possible actions are listed here against the character to be pressed: \n\n");
    printf("w/a/s/d\t-\tDirections\n");
    printf("q\t-\tTells about last state and what was pressed\n");
    printf("r/u\t-\tRedo/Undo while playing [15 undos allowed]\n");
    printf("o\t-\tExit (Syshtem \"O\"bliterate)\n");
    char che=getch();
    if(che=='x')
    {
        printf("c\t-\tCheat Mode (waits for you to enter a value to change the presently added number at random position)\n");
        printf("x\t-\tPressing x as a response for any prompt which tells you that you cannot enter hall of fame, will let you enter it");
        printf("q\t-\tShows you previous situation of the board and what was entered in the last turn");
        printf("x\t-\tPressing x while playing gives you total control. Enter <x> <y> <val> to change value at position (x,y)");
        getch();
    }
}
void prnmat()
{
    system("cls");
    printf("\n>>>>>>>>>>>> Welcome to SSD\'s 2048 <<<<<<<<<<<<\n\n");
    printf("\'m\' - Show Menu\t\t\'r\'\/\'u\' - Redo\/Undo\n\n");
    printf("Score : %d\n\n",score);
    int i=0,j=0,k=0;
    for(i=0; i<n; i++) printf("\t------\t");
    printf("\n");
    for(i=0; i<n; i++)
    {
        printf("|");
        for(j=0; j<n; j++)
            if(b[i][j]) printf("\t%4d\t|",a[i][j]);
            else printf("\t     \t|");
        printf("\n");
        for(k=0; k<n; k++) printf("\t------\t");
        printf("\n");
    }
    printf("\n");
    /*for(i=0;i<n;i++)
    {
        for(j=0;j<n;j++) printf("%d ",b[i][j]);
        printf("\n");
    }*/
}
int getNearestPowerOf2(int c)
{
    if(c==1) return 2;
    int pwr=10;
    while(c>pow(2,pwr)) pwr+=5;
    int i=pwr>10?(pwr-5):1;
    while(pow(2,i++)<=c);
    i-=2;
    if(pow(2,i)==c) return c;
    else return((c-pow(2,i))<(pow(2,(i+1))-c)? pow(2,i):pow(2,(i+1)));
}
int filled()
{
    int i,j;
    for(i=0; i<n; i++) for(j=0; j<n; j++) if(!b[i][j]) return 0;
    return 1;
}
void initialize()
{
    won=score=flag=lost=checkedHOF=cheated=0;
    a=malloc(n*sizeof(int*));
    if(a==NULL) printf("Jagah Koni");
    b=malloc(n*sizeof(int*));
    if(b==NULL) printf("Jagah Koni");
    r=malloc(n*sizeof(int*));
    if(r==NULL) printf("Jagah Koni");
    int i=0, j=0;
    for(i=0; i<n; i++)
    {
        a[i]=malloc(n*sizeof(int));
        b[i]=malloc(n*sizeof(int));
        r[i]=malloc(n*sizeof(int));
    }
    for(i=0; i<n; i++) for(j=0; j<n; j++)
        {
            a[i][j]=0;
            b[i][j]=0;
            r[i][j]=0;
        }
    rear=front=present=NULL;
    superCheater=0;
    smthgWzMoved=0;
}
void getControl()
{
    printf("\nYou have total control sir!!\n");
    printf("Until you press e on your keyboard, I will keep asking for response.\n\n");
    int xx, yy, val;
    char entry;
    while((entry=getch())!='e')
    {
        printf("Enter your response sir : ");
        scanf("%d%d%d",&xx,&yy,&val);
        if(xx>-1&&xx<n&&yy>-1&&yy<n)
        {
            if(val==0)
            {
                a[xx][yy]=val;
                b[xx][yy]=0;
            }
            else
            {
                a[xx][yy]=getNearestPowerOf2(val);
                b[xx][yy]=1;
            }
            prnmat();
        }
    }
    return;
}
void freeall()
{
    free(a);
    free(b);
    free(r);
}
void freeList()
{
    if((front==NULL)&&(present==NULL)) return;
    //printf("Reached 1");getch();
    while(front->prev!=NULL)
    {
        //printf("Reached 2");getch();
        lnode *tempor;
        tempor=front;
        front=front->prev;
        free(tempor);
    }
    //printf("Reached 3");getch();
    rear=present=front;
    //printf("Reached 4");getch();
    free(rear);
    front=rear=present=NULL;
}
void main()
{
    srand(time(NULL));
    printf("\n>>>>>>>>>>>> Welcome to SSD\'s 2048 <<<<<<<<<<<<\n\n");
    printf("Welcome to version 2.9\n\n");
    printf("Written by :\n\n");
    char author[40]="S U K H M E E T   S I N G H";
    int i=0;
    while(author[i]!='\0')
    {
        printf("%c",author[i++]);
        Sleep(100);
    }
    printf("\n\n");
    getch();
    printf("\nRecommendation : Please right click on the title bar of this command window, go to properties -> Layout Tab -> Window Size -> Width = 168 and Height = 55. \n\nAppropriate size may vary with screen resolution.");
    getch();
    printf("\n\nThis will help display the Board better on the screen, upto the order of 10.\n\nThank You!!");
    getch();
    printMenu();
    freeall();
}
