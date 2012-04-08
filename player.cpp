//---------------------------------------------------------------------------


#pragma hdrstop

#include "player.h"
#include "main.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
void TPlayerManager::StopMoving()
{
    Player.MovingDown=0;
    Player.MovingLeft=0;
    Player.MovingRight=0;
    Player.MovingUp=0;
}
//---------------------------------------------------------------------------
void TPlayerManager::Clear()
{
    memset(&Player,0,sizeof(Player));
    memset(&PlayerParameters,0,sizeof(PlayerParameters));
    
}
//---------------------------------------------------------------------------
void TPlayerManager::RebuildParameters()
{
    FireInterval=1;
    PlayerParameters[1]=1;
    PlayerParameters[2]=0;
    PlayerParameters[4]=0;
    PlayerParameters[5]=0;
    PlayerParameters[6]=1;
    PlayerParameters[8]=1;
    PlayerParameters[14]=1;

    int i;

    for(i=0;i<11;i++)
        if(RangerArtefacts[i]==1)
            switch(i)
            {
                case 0: PlayerParameters[1]*=1.25; PlayerParameters[14]/=1.07;    break;
                case 1: PlayerParameters[2]+=10;   PlayerParameters[1]/=1.1;      break;
                case 2: PlayerParameters[6]*=1.25; PlayerParameters[2]-=4;        break;
                case 3: PlayerParameters[6]*=1.25; PlayerParameters[1]/=1.1;      break;
                case 4: PlayerParameters[5]+=10;   PlayerParameters[6]/=1.15;     break;
                case 5: PlayerParameters[14]*=1.3; PlayerParameters[5]-=5;        break;
                case 6: PlayerParameters[8]=2;     PlayerParameters[4]-=4;        break;
                case 7: PlayerParameters[1]*=1.1;                                 break;
                case 8: PlayerParameters[2]+=5;                                   break;
                case 9: PlayerParameters[5]+=5;                                   break;
                case 10:PlayerParameters[4]+=8;    PlayerParameters[14]/=1.08;    break;
            }
    for(i=1;i<6;i++)
        for(int h=0;h<2;h++)
            RangerWeapon[i][h][3]*=FireInterval;
}

void __fastcall TPlayerManager::HealPlayerShip()
{
    Player.HP=Player.MaxHP;
}
//---------------------------------------------------------------------------
void __fastcall TPlayerManager::RepairPlayerShip()
{
    Player.HP+=(5*Player.DroidRepair+PlayerParameters[2])/100;
        if(Player.HP>Player.MaxHP)
            Player.HP=Player.MaxHP;
}
//---------------------------------------------------------------------------
void __fastcall TPlayerManager::Init(string IniFile, TTextureManager* TexMan)
{
    ZeroMemory(&Player,sizeof(TPlayer));
    ZeroMemory(PlayerParameters,sizeof(double)*15);

    string Loc = "graphresource.dll";
    string FileName = "Ranger";
    SpriteCount = 5;
    int X = 64;
    int Y = 64;
    GraphRect=CRect(0,0,X,Y);

    bool Alpha=false;
    D3DCOLOR Color=FCOLOR;
    if(!Alpha)
    {
        int Red=255;
        int Green=0;
        int Blue=255;
        Color=D3DCOLOR_ARGB(255,Red,Green,Blue);
    }
    Texture = new LPDIRECT3DTEXTURE9;
    TexMan->CreateEx(FileName,(Loc==""),0,0,Color,Texture);

    Player.Visible=true;
}
//---------------------------------------------------------------------------
void __fastcall TPlayerManager::MovePlayerShip()
{
    int i;

    if((Player.MovingLeft)&&(Player.xpos>0)&&(Player.LeftSpeed<3))
        Player.LeftSpeed+=PlayerParameters[1]/2.5;
    else if(Player.LeftSpeed>0)
        Player.LeftSpeed-=0.1;

    if((Player.MovingRight)&&(Player.xpos<750)&&(Player.RightSpeed<3))
        Player.RightSpeed+=PlayerParameters[1]/2.5;
    else if(Player.RightSpeed>0)
        Player.RightSpeed-=0.1;

    if((Player.MovingUp)&&(Player.ypos>10)&&(Player.TopSpeed<3))
        Player.TopSpeed+=PlayerParameters[1]/2.5;
    else if(Player.TopSpeed>0)
        Player.TopSpeed-=0.1;

    if((Player.MovingDown)&&(Player.ypos<540)&&(Player.BottomSpeed<2.5))
        Player.BottomSpeed+=PlayerParameters[1]/2.5;
    else if(Player.BottomSpeed>0)
        Player.BottomSpeed-=0.1;

    Player.xpos+=Player.RightSpeed-Player.LeftSpeed;
    Player.ypos+=Player.BottomSpeed-Player.TopSpeed;
    if(Player.xpos<0)    Player.xpos=0;
    if(Player.xpos>750)  Player.xpos=750;
    if(Player.ypos<1)    Player.ypos=1;
    if(Player.ypos>509)  Player.ypos=509;
}
//---------------------------------------------------------------------------
void TPlayerManager::ProceedPhysics()
{
}
//---------------------------------------------------------------------------
void __fastcall TPlayerManager::TryPlayerFire()
{
    int x;
    for(int i=1;i<6;i++)
        for(x=0;x<2;x++)
        {
            if(RangerWeapon[i][x][1]==1)
                if(RangerWeapon[i][x][2]<RangerWeapon[i][x][3])
                    RangerWeapon[i][x][2]+=1*PlayerParameters[14];
        }
    if(Fire)
    {
        int WeaponType=Player.CurrentWeapon;
        if(RangerWeapon[WeaponType][0][2]>=RangerWeapon[WeaponType][0][3])
        {
            PlayerShipFire(WeaponType,Player.WeaponPower,false);
            string FolderName=TempFolder;
            FolderName+="sound//1.wav";
            MusicManager.OpenAndPlayEx(FolderName,60,100,false);
            RangerWeapon[WeaponType][0][2]=0;
        }
    }
    if(FireAlt)
    {
        int WeaponType=Player.CurrentWeapon;
        if(RangerWeapon[WeaponType][1][2]>=RangerWeapon[WeaponType][1][3])
        {
            RangerWeapon[WeaponType][1][2]=0;
            PlayerShipFire(WeaponType,Player.WeaponPower,true);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TPlayerManager::PlayerDead()
{
    Laser[1]=0;
    Player.HP=Player.MaxHP;
    Player.Lifes-=1;
    Player.Visible=false;
    RangerDeads=true;
    for(int i=0;i<100;i++)
        Shleif[i][1]=0;
    Pause(1);
    SomeShit=0;

    if((Player.Lifes>=0)&&(Player.EnabledNewLifes))
        Exploding(2,Player.xpos-20,Player.ypos-20);

    if((Player.Lifes<0)||(!Player.EnabledNewLifes))
        Exploding(3,Player.xpos-50,Player.ypos-50);

    TimerManager->SetStatus(DEAD_TIMER,false);
    TimerManager->SetStatus(DEAD_TIMER,true);
}
//---------------------------------------------------------------------------
void __fastcall TPlayerManager::UpdatePlayerSpeedOverMouse()
{
    double Diff;
	Diff=(SCREEN_HEIGHT/2-PPos.y);
	double XSpeed,YSpeed;
	if(abs(Diff)<50)
		YSpeed=Diff/13;
	if(abs(Diff)>=50)
		YSpeed=4*Diff/abs(Diff);

	Diff=(SCREEN_WIDTH/2-PPos.x);
	if(abs(Diff)<50)
		XSpeed=Diff/13;
	if(abs(Diff)>=50)
		XSpeed=4*Diff/abs(Diff);

	Player.MovingUp=false;
	Player.MovingDown=false;
	Player.MovingRight=false;
	Player.MovingLeft=false;

	if(XSpeed<0)
	{
		Player.RightSpeed+=-XSpeed/15;
		Player.LeftSpeed+=XSpeed/30;
	}
	if(XSpeed>0)
	{
		Player.LeftSpeed+=XSpeed/15;
		Player.RightSpeed-=XSpeed/30;
	}
	if(YSpeed>0)
	{
		Player.TopSpeed+=YSpeed/15;
		Player.BottomSpeed-=YSpeed/30;
	}
	if(YSpeed<0)
	{
		Player.BottomSpeed+=-YSpeed/15;
		Player.TopSpeed+=YSpeed/30;
	}

	if((Player.RightSpeed>-XSpeed)&&(XSpeed<0))   Player.RightSpeed=-XSpeed;
	if((Player.TopSpeed>YSpeed)&&(YSpeed>0))      Player.TopSpeed=YSpeed;
	if((Player.LeftSpeed>XSpeed)&&(XSpeed>0))     Player.LeftSpeed=XSpeed;
	if((Player.BottomSpeed>-YSpeed)&&(YSpeed<0))  Player.BottomSpeed=-YSpeed;

	if(Player.RightSpeed<0)   Player.RightSpeed=0;
	if(Player.TopSpeed<0)     Player.TopSpeed=0;
	if(Player.LeftSpeed<0)    Player.LeftSpeed=0;
	if(Player.BottomSpeed<0)  Player.BottomSpeed=0;
}
//---------------------------------------------------------------------------
int  __fastcall TPlayerManager::NewPlayerArtefact()
{
	int i;
	do
		i=random(16);
	while(RangerArtefacts[i]!=0 && i<11);
	RangerArtefacts[i]=1;
	switch(i)
	{
		case 0: PlayerParameters[1]*=1.25;    FireInterval*=1.07;        break;
		case 1: PlayerParameters[2]+=10;      PlayerParameters[1]/=1.1;  break;
		case 2: PlayerParameters[6]*=1.25;    PlayerParameters[2]-=4;    break;
		case 3: PlayerParameters[6]*=1.25;    PlayerParameters[1]/=1.1;  break;
		case 4: PlayerParameters[5]+=10;      PlayerParameters[6]/=1.15; break;
		case 11: case 12:                     PlayerParameters[7]+=1;    break;
		case 6: PlayerParameters[8]=2;        PlayerParameters[4]-=4;    break;
		case 7: PlayerParameters[1]*=1.1;                                break;
		case 8: PlayerParameters[2]+=5;                                  break;
		case 9: PlayerParameters[5]+=5;                                  break;
		case 10:PlayerParameters[4]+=8;       FireInterval*=1.08;        break;
		case 5: FireInterval/=1.3;            PlayerParameters[5]-=5;    break;
		case 13: case 14:                     Player.HP=Player.MaxHP;    break;
        case 15:Player.Lifes++;                                          break;
    }

    if((i>=11)&&(i<=12))
        i=11;

    for(i=1;i<6;i++)
        for(int h=0;h<2;h++)
            RangerWeapon[i][h][3]=FireInterval*RangerWeapon[i][h][5];
    return i;
}
//---------------------------------------------------------------------------
void __fastcall TPlayerManager::CreatePlayerShip(TPlayer Ship,double ShipParameters[15])
{
    Player=Ship;
    for(int i=0;i<15;i++)
        PlayerParameters[i]=ShipParameters[i];
}
//---------------------------------------------------------------------------
TPoint __fastcall TPlayerManager::GetPlayerPos()
{
    TPoint Rect(Player.xpos,Player.ypos);
    return Rect;
}
//---------------------------------------------------------------------------
RECT __fastcall TPlayerManager::GetPlayerRect()
{
    return UpdateRect(CRect(10,5,30,45), Player.xpos, Player.ypos);
}
//---------------------------------------------------------------------------
void __fastcall TPlayerManager::InitDrawingInterface(LPD3DXSPRITE* DestSprite)
{
    SpritePointer=DestSprite;
}
//---------------------------------------------------------------------------
void __fastcall TPlayerManager::Draw()
{
    if(!Player.Visible)
        return;
    int NumImage=(Player.LeftSpeed-Player.RightSpeed)/6.0*SpriteCount+(SpriteCount-1)/2.0;
    NumImage=NumImage>4?4:NumImage;
    NumImage=NumImage<0?0:NumImage;
    (*SpritePointer)->Draw(*Texture,&CRect(NumImage*GraphRect.right,0,(NumImage+1)*GraphRect.right,GraphRect.bottom),
                            NULL,&CVector(Player.xpos,Player.ypos,0),FCOLOR);
}
//---------------------------------------------------------------------------



void TPlayerShotsManager::Clear()
{
    for(int i=0;i<100;i++)
        Shots[i].Exists=false;
}

void TPlayerShotsManager::Draw()
{
    for (int i=0;i<100;i++)
        if(Shots[i].Exists)
        {
            int qw=Shots[i].CurrentFrame;
            int Type,Second;

            if(Shots[i].MaxFrames==7)                              Type=0;
            if(Shots[i].MaxFrames==14)                             Type=1;
            if((Shots[i].MaxFrames==5)||(Shots[i].MaxFrames==6))  Type=3;
            if(Shots[i].MaxFrames==23)                             Type=6;

            RECT Rect1=UpdateRect(DrawingRects[8+Type], Shots[i].xpos, Shots[i].ypos);
            RECT Rect2=UpdateRect(DrawingRects[8+Type], (DrawingRects[8+Type].right - DrawingRects[8+Type].left)*qw,   0);

            if(Type!=6)
                (*Sprite)->Draw(WeaponImages[Type][0],&CRect(Rect2.left,Rect2.top,Rect2.right,Rect2.bottom),NULL,
                &CVector(Rect1.left,Rect1.top,0),FCOLOR);
            else
                (*Sprite)->Draw(WeaponImages[1][1],&CRect(Rect2.left,Rect2.top,Rect2.right,Rect2.bottom),NULL,
                &CVector(Rect1.left,Rect1.top,0),FCOLOR);
        }
}

void TPlayerShotsManager::Add(TPoint ppos,int ID,int WeaponLevel,double pp4,double pp6)
{
    int count=0;
    bool SecondWeapon=(ID%10)-1;
    int WeaponType=ID/10;
    if(!SecondWeapon)
    {
        switch(WeaponType)
        {
            case 1:
            case 3: count=WeaponLevel; break;
            case 2: count=2; break;
            case 4: count=1; break;
            case 5: count=0; break;
            case 7: count=3; break;
        }
    }
    if(SecondWeapon)
    {
        switch(WeaponType)
        {
            case 1:
            case 2:
            case 4: count=1; break;
            case 3:
            case 5: count=0; break;
        }
    }



    for(int x=0;x<count;x++)
    {
        int i=0;
        while(i<100 && Shots[i].Exists)
            i++;
        if(i==100)
            return;
        Shots[i].Exists=true;
        Shots[i].ID=ID;

        Shots[i].CurrentFrame=0;
        if(!SecondWeapon)
            Shots[i].MaxFrames=FWCount[WeaponType-1];
        else
            Shots[i].MaxFrames=FWACount[WeaponType-1];

        PlayerStatistic.ShotsFired+=1;


        if(!SecondWeapon)
        {
            switch(WeaponType)
            {
                case 1:
                case 3:
                {

                    Shots[i].Damage=10*WeaponLevel*pp6+pp4;

                    int y=x+1;
                    if(WeaponLevel==2)
                        y++;
                    Shots[i].YSpeed=5;
                    Shots[i].XSpeed=0;
                    if(y>3)
                    {
                        Shots[i].YSpeed=4;
                        Shots[i].XSpeed=10*y-45;
                    }
                    if(y>1)
                    {
                        Shots[i].xpos=ppos.x+40-40*(y%2);
                        Shots[i].ypos=ppos.y+20;
                    }
                    if(y==1)
                    {
                        Shots[i].xpos=ppos.x+20;
                        Shots[i].ypos=ppos.y;
                    }
                    if(WeaponType==3)
                    {
                        Shots[i].Damage/=5;
                        Shots[i].XSpeed*=3;
                        Shots[i].YSpeed*=3;
                    }
                }
                break;
                case 2:
                {
                    Shots[i].xpos=ppos.x+23;
                    Shots[i].ypos=ppos.y+30;
                    Shots[i].YSpeed=0;
                    Shots[i].XSpeed=2-4*x;
                    Shots[i].Damage=10*WeaponLevel*pp6+pp4;
                    Shots[i].f=12;
                    Shots[i].a=1;
                }
                break;
                case 4:
                case 7:
                {
                    Shots[i].xpos=ppos.x+18;
                    Shots[i].ypos=ppos.y;
                    Shots[i].YSpeed=2;
                    Shots[i].XSpeed=0;
                    Shots[i].Damage=200*WeaponLevel*pp6+pp4;
                    Shots[i].f=10;
                    Shots[i].a=1;

                    if(WeaponType==7)
                    {
                        Shots[i].f=30;
                        Shots[i].a=2;
                        Shots[i].b=120*i;
                        Shots[i].c=ppos.x;
                        Shots[i].d=ppos.y;
                        Shots[i].e=0;
                        Shots[i].ID=41;
                    }
                }
                break;
            }
        }
        else
        {
            switch(WeaponType)
            {
                case 1:
                {
                    Shots[i].xpos=ppos.x+20;
                    Shots[i].ypos=ppos.y;
                    Shots[i].Damage=100*WeaponLevel*pp6+pp4;
                    Shots[i].YSpeed=4;
                    Shots[i].XSpeed=0;
                }
                break;
                case 2:
                {
                    Shots[i].xpos=ppos.x+23;
                    Shots[i].ypos=ppos.y+30;
                    Shots[i].YSpeed=0;
                    Shots[i].XSpeed=0;
                    Shots[i].Damage=150*WeaponLevel*pp6+pp4*3;
                }
                break;
                case 4:
                {
                    Shots[i].xpos=ppos.x+20;
                    Shots[i].ypos=ppos.y;
                    Shots[i].Damage=100*WeaponLevel*pp6+pp4;
                    Shots[i].YSpeed=4;
                    Shots[i].XSpeed=0;
                }
                break;
            }
        }
    }

    if(WeaponType==5 && !SecondWeapon)
    {
        Laser[1]=1;
        Laser[2]=ppos.x-7;
        Laser[3]=ppos.y-470;
        Laser[4]=5*WeaponLevel*pp6;
        Laser[5]=50;
    }

    if(SecondWeapon)
    {
        if(WeaponType==3)
        {
            int i=1;
            while(Lightnings[i].Exists)
                i++;
            Lightnings[i].Exists=true;
            Lightnings[i].xpos=ppos.x+20;
            Lightnings[i].ypos=ppos.y+20;
            Lightnings[i].Power=50*WeaponLevel*pp6+pp4*2;
            Lightnings[i].JumpesLeft=5;
        }
        if(WeaponType==5)
        {
            if(!RangerTesla.Exists)
            {
                RangerTesla.Exists=true;
                RangerTesla.Power=1.5*WeaponLevel*pp6;
                RangerTesla.Radius=100+20*WeaponLevel*pp6;
                RangerTesla.ChargeMax=200;
                RangerTesla.ChargeLast=200;
            }
        }
    }
}


void TPlayerShotsManager::Move()
{
    int WeaponPower=PlayerManager.Player.WeaponPower;
    int xpos=PlayerManager.Player.xpos;
    int ypos=PlayerManager.Player.ypos;
    int pp4=PlayerManager.PlayerParameters[4];
    int pp6=PlayerManager.PlayerParameters[6];

    int i,q;
    for (i=0;i<100;i++)
    if(Shots[i].Exists)
    {
        if(Shots[i].ID==41)
        {
            if(Shots[i].a==3)
            {
                Shots[i].b+=30;
                Shots[i].d-=2*Shots[i].YSpeed;
                Shots[i].xpos=Shots[i].c+sin(Shots[i].b*M_PI/180)*Shots[i].e;
                Shots[i].ypos=Shots[i].d+cos(Shots[i].b*M_PI/180)*Shots[i].e;
            }
            if(Shots[i].a==2)
            {
                if(Shots[i].e==30)
                    Shots[i].a=3;
                if(Shots[i].e<30)
                {
                    Shots[i].e+=1;
                    Shots[i].d-=Shots[i].YSpeed;
                    Shots[i].xpos=Shots[i].c+sin(Shots[i].b*M_PI/180)*Shots[i].e;
                    Shots[i].ypos=Shots[i].d+cos(Shots[i].b*M_PI/180)*Shots[i].e;
                }
            }
            if(Shots[i].a==1)
            {
                if(Shots[i].f==0)
                {
                    Shots[i].Exists=false;

                    Add(TPoint(Shots[i].xpos,Shots[i].ypos),71,WeaponPower,pp4,pp6);

                }
                if(Shots[i].f>0)
                    Shots[i].f-=1;
            }
        }

        if(Shots[i].ID==21)
        {
            if(Shots[i].a==1)
            {
                if(Shots[i].f!=0)
                {
                    Shots[i].xpos-=Shots[i].XSpeed;
                    Shots[i].f-=1;
                    if(Shots[i].f==0)
                    {
                        Shots[i].XSpeed=0;
                        Shots[i].YSpeed=0;
                    }
                }
                if(Shots[i].f==0)
                {
                    Shots[i].a=2;
                    Shots[i].XSpeed=-EnemiesManager.GetRocketSpeed(Shots[i].xpos+5,Shots[i].ypos+5,false);
                    Shots[i].f=50;
                    Shots[i].YSpeed=2;
                }
            }
            if(Shots[i].a==2)
                if(Shots[i].f>0)
                {
                    Shots[i].xpos-=Shots[i].XSpeed;
                    Shots[i].f-=1;
                }
            Shots[i].YSpeed+=0.1;
        }

        if(Shots[i].ID==42)
        {
            for(int k=0;k<5;k++)
                for(int z=0;z<EnemiesManager.EnemiesNow[k];z++)
                {
                    int X,Y;
                    X=EnemiesManager.Enemies[k][z].xpos-Shots[i].xpos;
                    Y=EnemiesManager.Enemies[k][z].ypos-Shots[i].ypos;
                    if(X*X+Y*Y<(100+5*WeaponPower)*(100+5*WeaponPower))
                    {
                        int h=1;
                        while(Lines[h][1]!=0)
                            h++;
                        Lines[h][1]=1;
                        Lines[h][2]=Shots[i].xpos+5;
                        Lines[h][3]=Shots[i].ypos+5;
                        Lines[h][4]=EnemiesManager.Enemies[k][z].xpos;
                        Lines[h][5]=EnemiesManager.Enemies[k][z].ypos;
                        int Damage=WeaponPower*pp6/2;

                        if(k==0)
                            EnemiesManager.Enemies[k][z].HP-=1;
                        else
                            EnemiesManager.Enemies[k][z].HP-=Damage;
                        if(EnemiesManager.Enemies[k][z].HP<=0)
                            EnemiesManager.EnemyDead(k+1,z);
                    }
                }
        }

        if((Shots[i].ID==21)||(Shots[i].ID==22))
        {
            int we=0;
            while (RocketShleif[we][1]!=0)
                we++;
            RocketShleif[we][1]=1;
            RocketShleif[we][2]=Shots[i].xpos+2;
            if(Shots[i].MaxFrames==14)
                RocketShleif[we][3]=Shots[i].ypos+17;
            if(Shots[i].MaxFrames==23)
                RocketShleif[we][3]=Shots[i].ypos+32;
            RocketShleif[we][4]=10;
        }

        if(Shots[i].ID==22)
            Shots[i].YSpeed+=0.15;
        if(Shots[i].ID!=41)
            Shots[i].ypos-=Shots[i].YSpeed;
        if(Shots[i].ID==11|31)
            Shots[i].xpos-=Shots[i].XSpeed;
        if((Shots[i].xpos>800)||(Shots[i].xpos<0)||(Shots[i].ypos>600)||(Shots[i].ypos<0))
            Shots[i].Exists=false;
        Shots[i].CurrentFrame+=1;
        if(Shots[i].CurrentFrame==Shots[i].MaxFrames)
            Shots[i].CurrentFrame=0;

        RECT ShieldRect;
        ShieldRect.left=Shots[i].xpos+2;
        ShieldRect.right=Shots[i].xpos+10;
        ShieldRect.top=Shots[i].ypos+2;
        ShieldRect.bottom=Shots[i].ypos+10;

        for(q=0;q<5;q++)
            for(int l=0;l<EnemiesManager.EnemiesNow[q];l++)
            {
                RECT FirstShip=EnemiesManager.GetEnemyRect(q+1,l);
                if(TestRects(FirstShip,ShieldRect))
                {
                    if((Shots[i].MaxFrames==7)&&(Shots[i].YSpeed==4)&&(Shots[i].XSpeed==0))
                        FirstWeaponShieldExplode(Shots[i].xpos,Shots[i].ypos,Shots[i].Damage);

                    EnemiesManager.Enemies[q][l].HP-=Shots[i].Damage;
                        if(Shots[i].MaxFrames!=23)
                        {
                            Shots[i].Exists=false;
                            PlayerStatistic.ShotsHit++;
                        }
                        else
                        {
                            Shots[i].Damage-=EnemiesManager.Enemies[q][l].HP;
                            if(Shots[i].Damage<=0)
                            {
                                Shots[i].Exists=false;
                                PlayerStatistic.ShotsHit++;
                            }
                        }
                    if(EnemiesManager.Enemies[q][l].HP<=0)
                    {
                        MusicManager.OpenAndPlayEx(TempFolder+"sound//expl0.wav",10,59,false);
                        EnemiesManager.Enemies[q][l].Exists=false;
                        EnemiesManager.EnemyDead(q+1,l);
                    }
                    else Exploding(1,Shots[i].xpos,Shots[i].ypos);
                }
            }
    }

    if(Laser[1]==1)
    {
        RECT (* DominatorRects)[5] = EnemiesManager.DominatorRects;

        Laser[2]=xpos-7;
        Laser[3]=ypos-470;

        RECT SecondShip1=UpdateRect(CRect(15,10,50,385), Laser[2], Laser[3]);
        RECT SecondShip2=UpdateRect(CRect(5,386,57,475), Laser[2], Laser[3]);

        for(int v=0;v<5;v++)
            for (i=0;i<EnemiesManager.EnemiesNow[v];i++)
            {
                RECT FirstShip=UpdateRect(DominatorRects[EnemiesManager.EnemySeries][v],
                EnemiesManager.Enemies[v][i].xpos-DominatorRects[EnemiesManager.EnemySeries][v].right/2,
                EnemiesManager.Enemies[v][i].ypos-DominatorRects[EnemiesManager.EnemySeries][v].bottom/2);
                if(TestRects(FirstShip,SecondShip1)||TestRects(FirstShip,SecondShip2))
                {
                    EnemiesManager.Enemies[v][i].HP-=Laser[4];
                    if(EnemiesManager.Enemies[v][i].HP<=0)
                    {
                        MusicManager.OpenAndPlayEx(TempFolder+"sound//expl0.wav",10,59,false);
                        EnemiesManager.EnemyDead(v+1,i);
                    }
                }
            }
        Laser[5]-=1;
        if(Laser[5]==0)
            Laser[1]=0;
        if(Boss[1]==1)
        {
            RECT Rect3=CRect(30,30,170,150);
            Rect3=UpdateRect(Rect3,Boss[2],Boss[3]);
            if(TestRects(Rect3,SecondShip1)||TestRects(Rect3,SecondShip2))
            {
                Boss[5]-=Laser[4];
                Exploding(1,random(SecondShip1.right - SecondShip1.left)+SecondShip1.left,random(Rect3.bottom - Rect3.top)+Rect3.top);
            }
            if(Boss[5]<=0)
            {
                Boss[1]=2;
                Exploding(3,Boss[2]+25,Boss[3]+25);
                SomeShit=0;
                TimerManager->SetStatus(END_GAME_TIMER,true);
                TimerManager->SetStatus(T18,false);
            }
        }
    }
    if(RangerTesla.Exists)
    {
        for(int k=0;k<5;k++)
            for(int z=0;z<EnemiesManager.EnemiesNow[k];z++)
            {
                int X,Y;
                X=EnemiesManager.Enemies[k][z].xpos-xpos+20;
                Y=EnemiesManager.Enemies[k][z].ypos-ypos+20;
                if(X*X+Y*Y<(RangerTesla.Radius)*(RangerTesla.Radius))
                {
                    int h=1;
                    while(Lines[h][1]!=0)
                        h++;
                    Lines[h][1]=1;
                    Lines[h][2]=xpos+20;
                    Lines[h][3]=ypos+20;
                    Lines[h][4]=EnemiesManager.Enemies[k][z].xpos;
                    Lines[h][5]=EnemiesManager.Enemies[k][z].ypos;
                    int Damage=RangerTesla.Power;

                    if(k==0)
                        EnemiesManager.Enemies[k][z].HP-=1;
                    else
                        EnemiesManager.Enemies[k][z].HP-=Damage;

                    if(EnemiesManager.Enemies[k][z].HP<=0)
                        EnemiesManager.EnemyDead(k+1,z);
                }
            }
            RangerTesla.ChargeLast-=1;
            if(RangerTesla.ChargeLast<=0)
                RangerTesla.Exists=false;
    }
    for(i=1;i<100;i++)
        if(Lightnings[i].Exists)
        {
            int x1,y1;
            TPoint c=EnemiesManager.GetNearestEnemy(Lightnings[i].xpos,Lightnings[i].ypos);
            if((c.x!=-1000)&&(c.y!=-1000))
            {
                x1=c.x;
                y1=c.y;
                int h=1;
                while(Lines[h][1]!=0)
                    h++;
                Lines[h][1]=1;
                Lines[h][2]=Lightnings[i].xpos;
                Lines[h][3]=Lightnings[i].ypos;
                Lines[h][4]=EnemiesManager.Enemies[x1][y1].xpos;
                Lines[h][5]=EnemiesManager.Enemies[x1][y1].ypos;

                Lightnings[i].xpos=EnemiesManager.Enemies[x1][y1].xpos;
                Lightnings[i].ypos=EnemiesManager.Enemies[x1][y1].ypos;
                Lightnings[i].JumpesLeft-=1;
                if(Lightnings[i].JumpesLeft<=0)
                    Lightnings[i].Exists=false;
                EnemiesManager.Enemies[x1][y1].HP-=Lightnings[i].Power;
                if(EnemiesManager.Enemies[x1][y1].HP<=0)
                    EnemiesManager.EnemyDead(x1+1,y1);
            }
            if((c.x==-1000)&&(c.y==-1000))
                Lightnings[i].Exists=false;
        }
}

bool TPlayerShotsManager::Test(int ID,RECT Rect,double* EnemyHP)
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
