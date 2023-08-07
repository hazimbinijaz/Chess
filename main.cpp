#include <iostream>
#include <windows.h>
#include <conio.h>
#include <fstream>

using namespace std;

int capital=0;
int Small=1;
void Save (char B[][8], int Turn);
void Load (char B[][8],int & Turn);
bool IsLegalMove (char B[][8], int Sr,int Sc,int dr,int dc,int turn);
void wait (int T)
{
    for (int t=0;t<T;t++)
    {
         for(int c=0; c<=99999;c++)
    {
    }
    }
}

void getRowColbyLeftClick(int &rpos,int &cpos)
{
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD Events;
    INPUT_RECORD InputRecord;
    SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
    do
    {
        ReadConsoleInput(hInput, &InputRecord, 1, &Events);
        if(InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
        {
            cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
            rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
            break;
        }
    }
    while(true);
}


void gotoRowCol(int rpos, int cpos)
{
    int xpos=cpos, ypos = rpos;
    COORD scrn;
    HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);

    scrn.X = xpos;
    scrn.Y = ypos;
    SetConsoleCursorPosition(hOuput,scrn);
}
void PrintBox (int Sr,int Sc, int cdim,int rdim,char S)
{
    for (int ri=Sr;ri<rdim+Sr;ri++)
    {
        for (int ci=Sc; ci<cdim+Sc;ci++)
        {
            if(ri==Sr|| ci==Sc||ri==Sr+rdim-1||ci==Sc+cdim-1)
            {
                gotoRowCol(ri,ci);
                cout<<S;
            }
        }
    }
}
void PrintBN(int BN,int rdim,int cdim,char S)
{
    int Sr=((BN-1)/8)*rdim;
    int Sc=(((BN-1)%8)*cdim);
    PrintBox(Sr,Sc,cdim,rdim,S);
}
void PrintInCenter (int BN, char S, int rdim, int cdim)
{
    int Cri=((BN-1)/8)*rdim+rdim/3+1;
    int Cci=(((BN-1)%8)*cdim)+cdim/2-1;

    gotoRowCol(Cri,Cci);
    cout<<S;
}
void PrintInCenter2 (int BN, char S, int rdim, int cdim)
{
    int Cri=((BN-1)/8)*rdim+rdim/3+1-2;
    int Cci=(((BN-1)%8)*cdim)+cdim/2-1+2;

    gotoRowCol(Cri,Cci);
    cout<<S;
}

void Init (char B[][8])
{
    B[0][0]=B[0][7]='R';
    B[0][1]=B[0][6]='H';
    B[0][2]=B[0][5]='B';
    B[0][3]='K';
    B[0][4]='Q';

    B[7][0]=B[7][7]='r';
    B[7][1]=B[7][6]='h';
    B[7][2]=B[7][5]='b';
    B[7][3]='k';
    B[7][4]='q';

    for (int i=0;i<8;i++)
    {
        B[1][i]='P';
        B[6][i]='p';

        B[2][i]='.';
        B[3][i]='.';
        B[4][i]='.';
        B[5][i]='.';
    }
}
void PrintGrid (int rdim, int cdim, char Board[][8])
{
    for (int c=1;c<=64;c++)
    {
        PrintBN(c,rdim,cdim,-37);
    }

    PrintBox(40,90,cdim,rdim,-37);

    gotoRowCol(34,100);
    cout<<"Click";

    gotoRowCol(38,90);
    cout<<"Here To";
    gotoRowCol(44,93);
    cout<<"Save";

    PrintBox(40,115,cdim,rdim,-37);
    gotoRowCol(38,115);
    cout<<"Here To";
    gotoRowCol(44,118);
    cout<<"Undo";
}
void Print_Board (int rdim, int cdim, char Board[][8])
{
    int p=1;
    for (int k=0;k<8;k++)
    {
        for (int i=0;i<8;i++)
        {
            PrintInCenter(p,Board[k][i],rdim,cdim);
            p++;
        }
    }
}
void Print_Board2 (int rdim, int cdim, char Board[][8])
{
    int p=1;
    for (int k=0;k<8;k++)
    {
        for (int i=0;i<8;i++)
        {
            PrintInCenter2(p,Board[k][i],rdim,cdim);
            p++;
        }
    }
}
bool IsCapital (char sym)
{
    if (sym>='A' && sym<='Z')
        return true;
    return false;
}
bool IsSmall (char sym)
{
    if (sym>='a' && sym<='z')
        return true;
    return false;
}

bool IsValidSelect (char B[][8],int Turn, int Sr, int Sc)
{
    if (Sr<0||Sr>7|| Sc<0||Sc>7)
        return false;
    char sym=B[Sr][Sc];
    if (capital==Turn && IsCapital(sym))
        return true;
    else if ((Small==Turn) && (IsSmall(sym)) )
        return true;
    return false;

}
bool IsValidDest (char B[][8], int Turn,int Sr, int Sc ,int dr, int dc)
{
    if (dr<0||dr>7|| dc<0||dc>7)
        return false;
    char sym=B[dr][dc];
    if (capital==Turn && !IsCapital(sym)  && (IsLegalMove(B,Sr,Sc,dr,dc,Turn)) )
        return true;
    else if ((Small==Turn) && !(IsSmall(sym)) && (IsLegalMove(B,Sr,Sc,dr,dc,Turn))  )
        return true;
    return false;

}

void MakeMoveOnBoard(char B[][8],int Sri,int Sci,int dri,int dci)
{
    B[dri][dci]=B[Sri][Sci];
    B[Sri][Sci]='.';
}
void TurnChanger (int & Turn)
{
    Turn=(Turn+1)%2;
}
void AskForPoints (char B[][8],int & r, int & c,int rdim,int cdim,int & Turn)
{
//    cin>>r;
//    cin>>c;

    getRowColbyLeftClick(r,c);
    r=r/rdim;
    c=c/cdim;

    if (r==4 && c==11)
    {
        Load(B,Turn);
//        TurnChanger(Turn);
        Print_Board(rdim,cdim,B);
    }
    Save(B,Turn);

    if (r==3 && c==10)
    {
        exit(1);
    }
}

bool Is_NS_move(int Sr, int Sc, int dr, int dc)
{
    if (Sc==dc)
        return true;
    return false;
}
bool Is_EW_move (int Sr, int Sc, int dr, int dc)
{
    if (Sr==dr)
        return true;
    return false;

}
bool Is_Sloppy_move (int Sr, int Sc, int dr, int dc)
{
    int Rchange=dr-Sr;
    int Cchange=dc-Sc;

    return (Rchange==Cchange || Rchange==-Cchange);
}

bool Is_NS_pathClear (char B[][8], int Sr, int Sc, int dr, int dc)
{
    if (Sr<dr)
    {
        for(int i=Sr+1; i<dr; i++)
        {
            if ( B[i][Sc]!=' ')
                return false;
        }
    }
    else if (Sr>dr)
    {
        for (int i=Sr-1; i>dr;i--)
        {
            if ( B[i][Sc]!=' ')
                return false;
        }
    }
    return true;
}

bool Is_EW_pathClear (char B[][8], int Sr, int Sc, int dr, int dc)
{
    if (Sc<dc)
    {
        for(int i=Sc+1; i<dc; i++)
        {
            if ( B[Sr][i]!=' ')
                return false;
        }
    }
    if (Sc>dc)
    {
        for (int i=Sc-1; i>dc;i--)
        {
            if ( B[Sr][i]!=' ')
                return false;
        }
    }
    return true;
}
bool Is_Sloppy_PathClear (char B[][8], int Sr, int Sc, int dr, int dc)
{
    if (Sr<dr&&Sc<dc)
    {
        for (int r=Sr+1, c=Sc+1;r<dr; r++,c++)
        {
            if ( B[r][c]!=' ')
                return false;
        }
    }

    else if (Sr>dr&&Sc>dc)
    {
        for (int r=Sr-1, c=Sc-1;r>dr; r--,c--)
        {
            if ( B[r][c]!=' ')
                return false;
        }
    }

    else if (Sr<dr&&Sc>dc)
    {
        for (int r=Sr+1, c=Sc-1;r<dr; r++,c--)
        {
            if ( B[r][c]!=' ')
                return false;
        }
    }

    else
    {
         for (int r=Sr-1, c=Sc+1;r>dr; r--,c++)
        {
            if ( B[r][c]!=' ')
                return false;
        }
    }
    return true;
}
bool IsLegal_Rook_Move (char B[][8], int Sr,int Sc,int dr,int dc,int turn)
{
    return (((Is_NS_move(Sr,Sc,dr,dc)) && (Is_NS_pathClear(B,Sc,Sc,dr,dc)) )
            || ((Is_EW_move(Sr,Sc,dr,dc)) && (Is_EW_pathClear(B,Sr,Sc,dr,dc)) ) );
}
bool IsLegal_Bishop_Move (char B[][8], int Sr,int Sc,int dr,int dc,int turn)
{
    return ( (Is_Sloppy_move(Sr,Sc,dr,dc) ) && (Is_Sloppy_PathClear(B,Sr,Sc,dr,dc)) );
}
bool IsKill (char B[][8], int Sr,int Sc,int dr,int dc,int turn)
{
    char sym=B[dr][dc];
    if (turn==capital && IsSmall(sym))
        return true;
    if (turn==Small && IsCapital(sym))
        return true;
    return false;

}

bool IsLegalMove (char B[][8], int Sr,int Sc,int dr,int dc,int turn)
{
    char sym=B[Sr][Sc];

    switch(sym)
    {
        case 'R':
        case 'r':
            {
                return IsLegal_Rook_Move(B,Sr,Sc,dr,dc,turn);
            }
        case 'B':
        case 'b':
            {
                return IsLegal_Bishop_Move(B,Sr,Sc,dr,dc,turn);
            }
        case 'Q':
        case 'q':
            {
                return (IsLegal_Bishop_Move(B,Sr,Sc,dr,dc,turn)||IsLegal_Rook_Move(B,Sr,Sc,dr,dc,turn));
            }
        case 'H':
        case 'h':
            {
                int deltaR= abs(dr-Sr);
                int deltaC= abs(dc-Sc);
                if ( (deltaR==1&&deltaC==2) ||  (deltaR==2&&deltaC==1) )
                    return true;
                return false;
            }
        case 'K':
        case 'k':
            {
                int deltaR= abs(dr-Sr);
                int deltaC= abs(dc-Sc);
                if (deltaR==1 || deltaC==1)
                {
                    return (IsLegal_Bishop_Move(B,Sr,Sc,dr,dc,turn)||IsLegal_Rook_Move(B,Sr,Sc,dr,dc,turn));
                }
                else
                    return false;
            }
        case 'P':
            {
                int deltaR=(dr-Sr);
                int deltaC= abs(dc-Sc);
                if ((deltaC==0 && !IsKill(B,Sr,Sc,dr,dc,turn)) || (deltaC==1 && IsKill(B,Sr,Sc,dr,dc,turn)))
                {
                    if ( Sr==1 && (deltaR<=2 && deltaR>0 && !IsKill(B,Sr,Sc,dr,dc,turn)) )
                    {
                        return true;
                    }
                    else if ( deltaR==1)
                    {
                        return true;
                    }

                    return false;
                }
            }
        case 'p':
            {
                int deltaR= (dr-Sr);
                int deltaC= (dc-Sc);
                if ( (deltaC==0 && !IsKill(B,Sr,Sc,dr,dc,turn)) ||(deltaC==1 && IsKill(B,Sr,Sc,dr,dc,turn)))
                {
                    if (Sr==6 && deltaR>=-2 && !IsKill(B,Sr,Sc,dr,dc,turn))
                        return true;
                    else if (deltaR==-1 )
                        return true;
                    return false;
                }
            }
    }
    return false;
}
void Highlight (char C[][8],char B[][8],int Sr,int Sc,int dr,int dc,int Turn)
{
    for (int i=0;i<8;i++)
    {
        for (int k=0;k<8;k++)
        {
            if (IsValidDest(B,Turn,Sr,Sc,i,k))
                C[i][k]='*';
        }
    }
}
void Save (char B[][8], int Turn)
{
    ofstream GivingTo ("file.txt");
    GivingTo<<Turn<<" ";
    for (int i=0; i<8;i++)
    {
        for (int c=0;c<8;c++)
        {
            GivingTo<<B[i][c]<<" ";
        }
    }
}
void Load (char B[][8],int & Turn)
{
    ifstream Take ("file.txt");
    Take>>Turn;
    for (int i=0; i<8;i++)
    {
        for (int c=0;c<8;c++)
        {
            Take>>B[i][c];
        }
    }
}

int main()
{
    int Turn=0,rdim=10,cdim=10;
    char B [8][8],choice;
    int Sr,Sc,dr,dc;
    cout<<"New Game Or Load game (N/L) : ";
    cin>>choice;
    if (choice=='N' || choice =='n')
    {
        Init(B);
    }
    else if (choice=='L' || choice =='l')
    {
        Load(B,Turn);
    }
//    getch();
    PrintGrid(rdim,cdim,B);
    Print_Board(rdim,cdim,B);
    gotoRowCol(20,83);
    cout<<"Select Again To Unselect!";
    do
    {
        char C [8][8];
        for (int i=0;i<8;i++)
        {
            for (int c=0;c<8;c++)
            {
                C[i][c]=' ';
            }
        }
        Print_Board2(rdim,cdim,C);
        do
        {
            do
            {
                gotoRowCol(5,83);
                cout<<"P"<<Turn+1<<"'s Turn : ";
                cout<<"Selection Coordinates (R , C) : ";
                AskForPoints(B,Sr,Sc,rdim,cdim,Turn);
//                if (Sr==3 && Sc==10)
//                {
//                    Save(B,Turn);
//                    exit(1);
//                }

            }
            while(IsValidSelect(B,Turn,Sr,Sc)!=true);

            for (int i=0;i<8;i++)
            {
                for (int c=0;c<8;c++)
                {
                    C[i][c]=' ';
                }
            }
            Highlight(C,B,Sr,Sc,dr,dc,Turn);
            Print_Board2(rdim,cdim,C);

            gotoRowCol(10,83);
            cout<<"P"<<Turn+1<<"'s Turn : ";
            cout<<"Destination Coordinates (R , C) : ";
            AskForPoints(B,dr,dc,rdim,cdim,Turn);
        }
        while(IsValidDest(B,Turn,Sr,Sc,dr,dc)!=true);


        MakeMoveOnBoard(B,Sr,Sc,dr,dc);
        TurnChanger(Turn);
        Print_Board(rdim,cdim,B);
    }
    while (true);


    cout << "\n\n\n\n\n\n";
    getch();
    return 0;
}
