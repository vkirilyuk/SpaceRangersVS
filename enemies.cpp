//---------------------------------------------------------------------------


#pragma hdrstop

#include "enemies.h"
#include "main.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
float __fastcall TEnemiesManager::GetRocketSpeed(int RocketX,int RocketY,bool LargestShip)
{
    int i,q,r,ta,locked;

    double e;
    locked=-1;
    ta=10000;

    for(i=0;i<5;i++)
        for(q=0;q<25;q++)
            if((Enemies[i][q].Exists)&&(Enemies[i][q].ypos>=0)&&(!Enemies[i][q].Locked))
                if(abs(Enemies[i][q].xpos-RocketX)*1.5<RocketY-Enemies[i][q].ypos)
                    if((Enemies[i][q].xpos-RocketX)/(RocketY-Enemies[i][q].ypos)<ta)
                    {
                        e=Enemies[i][q].xpos-RocketX;
                        ta=(Enemies[i][q].xpos-RocketX)/(RocketY-Enemies[i][q].ypos);
                        if(i==0)
                            locked=q;
                        else
                            locked=-1;
                    }

    if(locked!=-1)
        Enemies[0][locked].Locked=true;

    e=e/50;
    if(ta!=10000)
        return e;
    return 0;
}
//---------------------------------------------------------------------------
void __fastcall TEnemiesManager::EnemyNew(int Type,int Index)
{
    Enemies[Type-1][Index].Exists=true;
    Enemies[Type-1][Index].xpos=random(700);

    Enemies[Type-1][Index].ypos=random(350)-700;
    Enemies[Type-1][Index].ShotsMade=0;
    Enemies[Type-1][Index].Locked=false;
    Enemies[Type-1][Index].CurrentFrame=0;;
    Enemies[Type-1][Index].MaxFrames=25;
    switch(Type)
    {
        case 1:   Enemies[Type-1][Index].HP=10;    Enemies[Type-1][Index].MaxShots=0;   break;
        case 2:   Enemies[Type-1][Index].HP=20;    Enemies[Type-1][Index].MaxShots=5;   break;
        case 3:   Enemies[Type-1][Index].HP=50;    Enemies[Type-1][Index].MaxShots=10;  break;
        case 4:   Enemies[Type-1][Index].HP=250;   Enemies[Type-1][Index].MaxShots=25;  break;
        case 5:   Enemies[Type-1][Index].HP=750;   Enemies[Type-1][Index].MaxShots=100; break;
    }
}
//---------------------------------------------------------------------------
void __fastcall TEnemiesManager::EnemyDead(int Type,int Index)
{
    int X=Enemies[Type-1][Index].xpos;
    int Y=Enemies[Type-1][Index].ypos;
    if(Enemies[Type-1][Index].ypos>-100)
        switch(Type)
        {
            case 1:
            case 2:
            case 3: Exploding(1,X-10,Y-10); break;
            case 4:
            case 5: Exploding(2,X-65/2,Y-65/2);               break;
        }
    EnemyNew(Type,Index);
    switch(Type)
    {
        case 1: PlayerManager.Player.Experience+=9+random(3);  PlayerStatistic.KilledShtips+=1;    break;
        case 2: PlayerManager.Player.Experience+=18+random(5); PlayerStatistic.KilledMenoks+=1;    break;
        case 3: PlayerManager.Player.Experience+=27+random(7); PlayerStatistic.KilledSmershes+=1;  break;
        case 4: PlayerManager.Player.Experience+=45+random(11);PlayerStatistic.KilledUrgants+=1;   break;
        case 5: PlayerManager.Player.Experience+=90+random(21);PlayerStatistic.KilledEcventors+=1; break;
    }

        int a=random(50*(6-Type)/PlayerManager.PlayerParameters[8]);
        if(a==1)
            NewArtefact(X,Y);
}
//---------------------------------------------------------------------------
void __fastcall TEnemiesManager::MoveEnemies()
{
    for(int g=0;g<5;g++)
        for(int i=0;i<EnemiesNow[g];i++)
        {
            Enemies[g][i].ypos+=3-g/2;
            Enemies[g][i].CurrentFrame+=0.2;
            if(Enemies[g][i].CurrentFrame>=Enemies[g][i].MaxFrames)
                Enemies[g][i].CurrentFrame=1;
            if(Enemies[g][i].ypos>600)
                EnemyNew(g+1,i);
            if(g!=0)
                if((Enemies[g][i].ShotsMade<Enemies[g][i].MaxShots)&&(random((5-g)*10)==0)&&(Enemies[g][i].ypos>-50))
                {
                    Enemies[g][i].ShotsMade++;
                    EnemyShotsManager->Create(g+1,Enemies[g][i].xpos,Enemies[g][i].ypos);
                }
        }
}
//---------------------------------------------------------------------------
TPoint __fastcall TEnemiesManager::GetNearestEnemy(int X,int Y)
{
    double LengthMin=2000;
    TPoint v(-1000,-1000);
    for(int j=0;j<5;j++)
        for(int i=0;i<EnemiesNow[j];i++)
        {
            double CurrentLength=sqrt(pow(X-Enemies[j][i].xpos,2)+pow(Y-Enemies[j][i].ypos,2));
            if(CurrentLength<LengthMin)
            {
                LengthMin=CurrentLength;
                v.x=j;
                v.y=i;
            }
        }
    if(LengthMin<225)
        return v;
    return v;
}
//---------------------------------------------------------------------------
void __fastcall TEnemiesManager::Init()
{
    ZeroMemory(Enemies,sizeof(TEnemyShip)*6*50);
    ZeroMemory(EnemiesNow,sizeof(int)*6);
    ZeroMemory(EnemiesMax,sizeof(int)*6);

    DominatorDrawingRect=CRect(0,0,64,64);

    DominatorRects[0][0]=CRect(0,0,16,32);
    DominatorRects[0][1]=CRect(0,0,32,35);
    DominatorRects[0][2]=CRect(0,0,55,42);
    DominatorRects[0][3]=CRect(0,0,70,76);
    DominatorRects[0][4]=CRect(0,0,73,117);

    DominatorRects[1][0]=CRect(0,0,16,32);
    DominatorRects[1][1]=CRect(0,0,37,29);
    DominatorRects[1][2]=CRect(0,0,57,55);
    DominatorRects[1][3]=CRect(0,0,81,75);
    DominatorRects[1][4]=CRect(0,0,118,120);

    DominatorRects[2][0]=CRect(0,0,16,32);
    DominatorRects[2][1]=CRect(0,0,30,24);
    DominatorRects[2][2]=CRect(0,0,58,57);
    DominatorRects[2][3]=CRect(0,0,47,75);
    DominatorRects[2][4]=CRect(0,0,118,118);

    DrawingRects[8]=CRect(0,0,10,12);
    DrawingRects[9]=CRect(0,0,6,18);
    DrawingRects[10]=CRect(0,0,10,12);
    DrawingRects[11]=CRect(0,0,15,15);
    DrawingRects[12]=CRect(0,0,1,2);
    DrawingRects[13]=CRect(0,0,100,100);
    DrawingRects[14]=CRect(0,0,6,32);

    DominatorScalingFactor[0]=0.4*2;
    DominatorScalingFactor[1]=0.45*2;
    DominatorScalingFactor[2]=0.6*2;
    DominatorScalingFactor[3]=0.75*2;
    DominatorScalingFactor[4]=1*2;
}
//---------------------------------------------------------------------------
void __fastcall TEnemiesManager::EnemySetMax(int Type,int MaxDom)
{
    int WasMax=EnemiesMax[Type-1];
    EnemiesMax[Type-1]=MaxDom;
    if(MaxDom>WasMax)
    {
        for(int i=WasMax;i<MaxDom;i++)
        {
            EnemyNew(Type,EnemiesNow[Type-1]);
            EnemiesNow[Type-1]++;
        }
    }
    else
    {
//        EnemiesNow(Type-1]=MaxDom;
        for(int i=WasMax;i>MaxDom;i--)
        {
            EnemyDelete(Type,EnemiesNow[Type-1]);
            EnemiesNow[Type-1]--;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TEnemiesManager::EnemyDelete(int Type,int Index)
{
    ZeroMemory(&Enemies[Type-1][Index],sizeof(TEnemyShip));
}
//---------------------------------------------------------------------------
TPoint __fastcall TEnemiesManager::GetEnemyPosition(int Type,int Index)
{
    TPoint Result;
    Result.x=Enemies[Type-1][Index].xpos;
    Result.y=Enemies[Type-1][Index].ypos;
    return Result;
}
//---------------------------------------------------------------------------
RECT __fastcall TEnemiesManager::GetEnemyRect(int Type,int Index)
{
    RECT Result;
    Result=UpdateRect(DominatorRects[EnemySeries][Type-1],
    Enemies[Type-1][Index].xpos-DominatorRects[EnemySeries][Type-1].right/2,
    Enemies[Type-1][Index].ypos-DominatorRects[EnemySeries][Type-1].bottom/2);
    return Result;
}
//---------------------------------------------------------------------------
void __fastcall TEnemiesManager::InitDrawingInterface(LPD3DXSPRITE DestSprite)
{
    SpritePointer=DestSprite;
}
//---------------------------------------------------------------------------
void __fastcall TEnemiesManager::Draw()
{
    D3DXMATRIXA16 Matrix;
    for(int wq=5;wq>=0;wq--)
    {
        D3DXMatrixScaling(&Matrix,DominatorScalingFactor[wq],DominatorScalingFactor[wq],1);
        SpritePointer->SetTransform(&Matrix);
        for(int i=0;i<EnemiesNow[wq];i++)
            if(Enemies[wq][i].Exists)
                if(Enemies[wq][i].ypos>-125)
                {
                    int number=Enemies[wq][i].CurrentFrame;
                    RECT Rect1=UpdateRect(DominatorDrawingRect,Enemies[wq][i].xpos/DominatorScalingFactor[wq]-32,Enemies[wq][i].ypos/DominatorScalingFactor[wq]-32);
                    RECT Rect2=UpdateRect(DominatorDrawingRect,number* 64,0);
                    SpritePointer->Draw(DominatorUno[EnemySeries][wq],&CRect(Rect2.left,Rect2.top,Rect2.right,Rect2.bottom),NULL,
                    &CVector(Rect1.left,Rect1.top,0),FCOLOR);
                }
        SpritePointer->SetTransform(&IdentityMatrix);
    }
}
void __fastcall TEnemyShotsManager::Init(TPlayer* RangerPointer)
{
    for(int i=0;i<300;i++)
        Shots[i].Exists=false;
    Ranger=RangerPointer;
}
//---------------------------------------------------------------------------
void __fastcall TEnemyShotsManager::Create(int Type,int X,int Y)
{
    int i=0;
    while (Shots[i].Exists)
        i++;
    Shots[i].Exists=true;
    Shots[i].xpos=X;
    Shots[i].ypos=Y;
    Shots[i].XSpeed=(-X-2+Ranger->xpos+20)/40;
    Shots[i].YSpeed=(-Y-2+Ranger->ypos+20)/40;
    Shots[i].Damage=5*Type;
    Shots[i].ShoRECTDef=CRect(0,0,10,10);
    Shots[i].DrawingRectDef=CRect(0,0,10,10);
    Shots[i].ShoRECT=Shots[i].ShoRECTDef;
    Shots[i].DrawingRect=Shots[i].DrawingRectDef;
    if(Type==6)
        Shots[i].Damage=50;
}
//---------------------------------------------------------------------------
void __fastcall TEnemyShotsManager::Move()
{
    int i;

    for(i=0;i<300;i++)
        if(Shots[i].Exists)
        {
            Shots[i].xpos+=Shots[i].XSpeed;
            Shots[i].ypos+=Shots[i].YSpeed;
            Shots[i].ShoRECT=UpdateRect(Shots[i].ShoRECTDef,Shots[i].xpos,Shots[i].ypos);
            Shots[i].DrawingRect=UpdateRect(Shots[i].DrawingRectDef,Shots[i].xpos,Shots[i].ypos);
            if(!TestRects(CRect(0,0,800,600),Shots[i].ShoRECT))
                Shots[i].Exists=false;
        }
}
//---------------------------------------------------------------------------
void TEnemyShotsManager::Draw()
{
    for (int i=0;i<300;i++)
        if(Shots[i].Exists)
            (*Sprite)->Draw(DominatorShot,NULL,NULL,&CVector(Shots[i].xpos,Shots[i].ypos,0),FCOLOR);
}

void TEnemyShotsManager::Clear()
{
    for(int i=0;i<300;i++)
        Shots[i].Exists=false;
}

bool TEnemyShotsManager::Test(int ID,RECT Rect,double* EnemyHP)
{
    if(!Shots[ID].Exists)
        return false;
    if( Shots[ID].xpos>=Rect.left && Shots[ID].xpos<=Rect.right &&
    Shots[ID].ypos>=Rect.top && Shots[ID].ypos<=Rect.bottom)
    {
        if(EnemyHP!=NULL)
        {
            *EnemyHP-=Shots[ID].Damage;
            Shots[ID].Exists=false;
        }
        return true;
    }
    return false;
}

TPoint TEnemyShotsManager::GetPos(int ID)
{
    return TPoint(Shots[ID].xpos, Shots[ID].ypos);
}

int TEnemyShotsManager::GetDamage(int ID)
{
    return Shots[ID].Damage;
}

