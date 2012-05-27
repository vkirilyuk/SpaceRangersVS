//---------------------------------------------------------------------------


#pragma hdrstop

#include "Types.h"
//#include <Classes.hpp>
#include "fmod/inc/fmod.h"
#include "fmod/inc/fmod_errors.h"

//---------------------------------------------------------------------------

TFontParameters __fastcall CreateFontParameters(string Name,int Size)
{
    TFontParameters Temp;
    Temp.Name=Name;
    Temp.Size=Size;
    return Temp;
}

//---------------------------------------------------------------------------
void __fastcall TStatistic::Clear()
{
    KilledShtips=0;
    KilledMenoks=0;
    KilledSmershes=0;
    KilledUrgants=0;
    KilledEcventors=0;
    ShotsFired=0;
    GameTime=0;
    DamageCause=0;
    DamageTaken=0;
    ArtefactsPickedUp=0;
    ArtefactsLoosed=0;
    ShotsHit=0;
    for(int i=0;i<5;i++)
        WeaponGameTimes[i]=0;
    ShowStatistic=false;
};
//---------------------------------------------------------------------------
void __fastcall TFPSShower::Init()
{
    FPS=0;
    LastTickCount=0;
    CurrentTickCount=0;
    FrameCount=0;
}
//---------------------------------------------------------------------------
int __fastcall TFPSShower::GetFPS()
{
    return FPS;
}
//---------------------------------------------------------------------------
void __fastcall TFPSShower::UpdateFPS()
{
    if(LastTickCount==0)
    {
        LastTickCount=GetTickCount();
        FrameCount=0;
        FPS=0;
    }
    CurrentTickCount=GetTickCount();
    FrameCount+=1;
    if(CurrentTickCount-LastTickCount>=500)
    {
        FPS=FrameCount*1000/(CurrentTickCount-LastTickCount);
        FrameCount=0;
        while(CurrentTickCount-LastTickCount>=500)
            LastTickCount+=500;
    }
}
//---------------------------------------------------------------------------
int TFontManager::AddFont(string Name,int Size)
{
    int Index=0;
    while(Exist[Index])
        Index++;

    D3DXCreateFont(*FontDevice,Size, 0, FW_DONTCARE, 1, FALSE, DEFAULT_CHARSET,
                    OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
                    Name.c_str(), &Fonts[Index]);
    Exist[Index]=true;
    FontParams[Index].Name=Name;
    FontParams[Index].Size=Size;

    return Index;
}
//---------------------------------------------------------------------------
void TFontManager::RestoreAll()
{
    for(int i=0;i<25;i++)
        if(Exist[i])
            D3DXCreateFont(*FontDevice,FontParams[i].Size, 0, FW_DONTCARE, 1, FALSE, DEFAULT_CHARSET,
                    OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
                    FontParams[i].Name.c_str(), &Fonts[i]);
}
//---------------------------------------------------------------------------
void TFontManager::AddFontEx(int Index,string Name,int Size)
{
    D3DXCreateFont(*FontDevice,Size, 0, FW_DONTCARE, 1, FALSE, DEFAULT_CHARSET,
                    OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
                    Name.c_str(), &Fonts[Index]);
    Exist[Index]=true;
    FontParams[Index].Name=Name;
    FontParams[Index].Size=Size;
}
//---------------------------------------------------------------------------
void TFontManager::Init(LPDIRECT3DDEVICE9* Device,LPD3DXSPRITE* Sprite)
{
    FontDevice=Device;
    SpritePointer=Sprite;
    for(int i=0;i<25;i++)
        Exist[i]=false;
}
//---------------------------------------------------------------------------
LPD3DXFONT TFontManager::GetFont(int Index)
{
    if(Exist[Index])
        return Fonts[Index];
    return NULL;
}
//---------------------------------------------------------------------------
void TFontManager::Draw(int Index,string Text,int Left,int Top, D3DCOLOR Color)
{
    Fonts[Index]->DrawTextA(*SpritePointer,Text.c_str(),-1,&CRect(Left, Top, 1000,1000),0,Color);
//    MainFont->DrawTextA(MainSprite,String(FPSShower.GetFPS()).c_str(),-1,&CRect(0,0,100,100),0,TCOLOR);

}
//---------------------------------------------------------------------------
void __fastcall TCornerTextManager::Init(LPD3DXFONT* FontPointer)
{
    Font=FontPointer;
    Count=0;
    for(int i=0;i<50;i++)
    {
        TextQueue[i].Text="";
        TextQueue[i].Time=0;
    }
}
//---------------------------------------------------------------------------
void __fastcall TCornerTextManager::Add(string Text,int Time)
{
    if(Count+1>MAX_CORNER_TEXTS)
        return;
    TextQueue[Count].Text=Text;
    TextQueue[Count].Time=Time;
    Count++;
}
//---------------------------------------------------------------------------
TCornerText __fastcall TCornerTextManager::GetCurrent()
{
    return TextQueue[0];
}
//---------------------------------------------------------------------------
TCornerText __fastcall TCornerTextManager::GetText(int Index)
{
    return TextQueue[Index];
}
//---------------------------------------------------------------------------
void __fastcall TCornerTextManager::Update()
{
    TextQueue[0].Time-=3;
    if(TextQueue[0].Time<=-255)
    {
        Count--;
        for(int i=0;i<Count;i++)
            TextQueue[i]=TextQueue[i+1];
    }
}
//---------------------------------------------------------------------------
bool __fastcall TCornerTextManager::TextExists()
{
    if(Count>0)
        return true;
    return false;
}
//---------------------------------------------------------------------------
void __fastcall TCornerTextManager::DrawText()
{
    if(TextExists())
    {
        TCornerText Text=GetCurrent();
        int Colour=255+Text.Time;
        if(Colour>=255)
            Colour=255;
        D3DCOLOR CurrColor=D3DCOLOR_ARGB(Colour,255,0,0);
        (*Font)->DrawTextA(NULL,Text.Text.c_str(),-1,&CRect(0,0,1000,1000),0,CurrColor);
    }
}
//---------------------------------------------------------------------------
void __fastcall TCornerTextManager::DrawTextEx(int HowMuch)
{
    int Top=0;
    int MyCount=min(Count,HowMuch);
    for(int i=0;i<MyCount;i++)
    {
        TCornerText Text=GetText(i);
        int Colour=255+Text.Time;
        if(Colour>=255)
            Colour=255;
        D3DCOLOR CurrColor=D3DCOLOR_ARGB(Colour,255,0,0);
        (*Font)->DrawTextA(NULL,Text.Text.c_str(),-1,&CRect(0,Top,1000,1000),0,CurrColor);
        Top+=20;
    }
}
//---------------------------------------------------------------------------
bool __fastcall TMusicManager::Init(HDC Handle)
{
    int Index;

    /* Check version numbers */

    if (FMOD_VERSION > FSOUND_GetVersion())
        return false;

  /* Initialize FSOUND */

    if (!FSOUND_SetOutput(FSOUND_OUTPUT_DSOUND))
        return false;
    if (!FSOUND_SetDriver(0))
        return false;
    if (!FSOUND_SetMixer(FSOUND_MIXER_QUALITY_AUTODETECT))
        return false;
    if (!FSOUND_SetHWND(Handle))
        return false;

    if (!FSOUND_Init(22050, 128, 0))
        return false;

  /* Initialize song list to empty */
    for (Index =0;Index<MAX_SONGS;Index++)
    {
        Songs[Index].Module = NULL;
        Songs[Index].Stream = NULL;
        Songs[Index].Channel= -1;
    }
    return true;
}
//---------------------------------------------------------------------------
int  __fastcall TMusicManager::OpenFile(string FileName)
{
    int  Index;
    FMUSIC_MODULE* Module;
    FSOUND_STREAM* Stream;

    Index=0;

    while(( ((Songs[Index].Module!=NULL)&&(FMUSIC_IsPlaying(Songs[Index].Module)))||
    ((Songs[Index].Channel>=0)&&(FSOUND_IsPlaying(Songs[Index].Channel))))
     &&(Index<=MAX_SONGS))
        Index++;

    Stream = NULL;
    Module = FMUSIC_LoadSong(FileName.c_str());
    if (Module ==NULL)
        Stream = FSOUND_Stream_Open(FileName.c_str(), FSOUND_NORMAL | FSOUND_LOOP_NORMAL, 0, 0);
    if ((Module == NULL) && (Stream ==NULL))
        return -1;

    if (Module != NULL)
    {
        FMUSIC_SetMasterVolume(Module, 255);
        if (((FMUSIC_GetType(Module) == FMUSIC_TYPE_MOD) || (FMUSIC_GetType(Module) == FMUSIC_TYPE_S3M)))
          FMUSIC_SetPanSeperation(Module, 0.15);  // 15% crossover
    }

    Songs[Index].Module = Module;
    Songs[Index].Stream = Stream;
    Songs[Index].Playing= false;
    Songs[Index].FileName=FileName;
    return Index;
}
//---------------------------------------------------------------------------
void __fastcall TMusicManager::OpenFileEx(string FileName,int Index)
{
    FMUSIC_MODULE* Module;
    FSOUND_STREAM* Stream;

    if(Songs[Index].Playing)
        StopFile(Index);

    Stream = NULL;
    Module = FMUSIC_LoadSong(FileName.c_str());
    if (Module ==NULL)
        Stream = FSOUND_Stream_Open(FileName.c_str(), FSOUND_NORMAL | FSOUND_LOOP_NORMAL, 0, 0);
    if ((Module == NULL) && (Stream ==NULL))
        return;

    if (Module != NULL)
    {
        FMUSIC_SetMasterVolume(Module, 255);
        if (((FMUSIC_GetType(Module) == FMUSIC_TYPE_MOD) || (FMUSIC_GetType(Module) == FMUSIC_TYPE_S3M)))
          FMUSIC_SetPanSeperation(Module, 0.15);  // 15% crossover
    }

    Songs[Index].Module = Module;
    Songs[Index].Stream = Stream;
    Songs[Index].Playing= false;
    Songs[Index].FileName=FileName;
}
//---------------------------------------------------------------------------
void __fastcall TMusicManager::PlayFile(int Index)
{
    if(Index < 0)
        return;
    if (Songs[Index].Playing)
        StopFile(Index);

    if (Songs[Index].Module != NULL)
    {
        Songs[Index].Playing= FMUSIC_PlaySong(Songs[Index].Module);
        if(!Songs[Index].Playing)
            return;
    }
    else if (Songs[Index].Stream !=NULL)
    {
        Songs[Index].Channel = FSOUND_Stream_Play(FSOUND_FREE, Songs[Index].Stream);
        Songs[Index].Playing = Songs[Index].Channel >= 0;
        if (! Songs[Index].Playing)
        {
            return;
        }
        else
        {
            FSOUND_SetPan(Songs[Index].Channel, FSOUND_STEREOPAN);
            FSOUND_SetVolume(Songs[Index].Channel, 255);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TMusicManager::StopFile(int Index)
{
    if(Index<0)
        return;
    if (Songs[Index].Module !=NULL)
        FMUSIC_StopSong(Songs[Index].Module);
    else if (Songs[Index].Stream !=NULL)
        FSOUND_Stream_Stop(Songs[Index].Stream);
    Songs[Index].Channel = -1;
    Songs[Index].Playing = false;
}
//---------------------------------------------------------------------------
void __fastcall TMusicManager::PauseFile(int Index)
{
}
//---------------------------------------------------------------------------
void __fastcall TMusicManager::StopAll()
{
}
//---------------------------------------------------------------------------
int  __fastcall TMusicManager::OpenAndPlay(string FileName)
{
    int Index=OpenFile(FileName);
    PlayFile(Index);

    SetLoop(Index,false);

    return Index;
}
//---------------------------------------------------------------------------
void __fastcall TMusicManager::SetLoop(int Index,bool Loop)
{
    if(Songs[Index].Module!=NULL)
        FMUSIC_SetLooping(Songs[Index].Module,Loop);
    else if(Songs[Index].Channel>=0)
        FSOUND_SetLoopMode(Songs[Index].Channel,Loop+1);
}
//---------------------------------------------------------------------------
void __fastcall TMusicManager::SetGlobalLoop(bool Loop)
{
    if(Loop)
        FSOUND_SetLoopMode(FSOUND_ALL,FSOUND_LOOP_NORMAL);
    else
        FSOUND_SetLoopMode(FSOUND_ALL,FSOUND_LOOP_OFF);
}
//---------------------------------------------------------------------------
int  __fastcall TMusicManager::OpenAndPlayEx(string FileName,int FIndex,int SIndex,bool Loop)
{
    int Index=FIndex;
    while(( ((Songs[Index].Module!=NULL)&&(FMUSIC_IsPlaying(Songs[Index].Module)))||
    ((Songs[Index].Channel>=0)&&(FSOUND_IsPlaying(Songs[Index].Channel))))
     &&(Index<=SIndex))
        Index++;
    if(Index>SIndex)
        return -1;

    OpenFileEx(FileName,Index);
    PlayFile(Index);
    SetLoop(Index,Loop);
    return Index;
}
//---------------------------------------------------------------------------
void __fastcall TMusicManager::SetGlobalVolume(int Volume)
{
    FSOUND_SetSFXMasterVolume(Volume*2.55);
    for(int i=0;i<MAX_SONGS;i++)
        if(Songs[i].Playing)
        {
            if(Songs[i].Module!=NULL)
                FMUSIC_SetMasterVolume(Songs[i].Module,Volume*2.55);
        }
}
//---------------------------------------------------------------------------
void __fastcall TMusicManager::SetVolume(int Index,int Volume)
{
    if(Songs[Index].Module!=NULL)
        FMUSIC_SetMasterVolume(Songs[Index].Module,Volume*2.55);
    else if(Songs[Index].Stream!=NULL)
        FSOUND_SetVolume(Songs[Index].Channel,Volume*2.55);
}
//---------------------------------------------------------------------------
void __fastcall TTimerManager::AddTimerEx(int ID,int Interval, TMyCallBack CallBack, bool Enabled)
{
    TimerSets[ID].Interval=Interval;
    TimerSets[ID].CallBack=CallBack;
    TimerSets[ID].Enabled=Enabled;
    TimerSets[ID].LastTime=Ticks;
    TimerSets[ID].Exists=true;
}
//---------------------------------------------------------------------------
int  __fastcall TTimerManager::AddTimer(int Interval, TMyCallBack CallBack, bool Enabled)
{
    int ID=0;
    while(ID<100 && !TimerSets[ID].Exists)
        ID++;
    if(ID==100)
        return -1;
    AddTimerEx(ID,Interval,CallBack,Enabled);
    return ID;
}
//---------------------------------------------------------------------------
void TTimerManager::Update(double Delta)
{
    Ticks+=Delta;
    int i;
    for(i=0;i<100;i++)
        if((TimerSets[i].Exists)&&(TimerSets[i].Enabled))
            if(TimerSets[i].LastTime+TimerSets[i].Interval<Ticks)
            {
                TimerSets[i].LastTime=Ticks;
                TimerSets[i].CallBack();
            }
}
//---------------------------------------------------------------------------
void __fastcall TTimerManager::Remove(int ID)
{
    TimerSets[ID].Exists=false;
}
//---------------------------------------------------------------------------
void __fastcall TTimerManager::SetStatus(int ID, bool Enabled)
{
    bool Was=TimerSets[ID].Enabled;
    TimerSets[ID].Enabled=Enabled;
    if((!Was)&&(Enabled))
        TimerSets[ID].LastTime=Ticks;
}
//---------------------------------------------------------------------------
bool __fastcall TTimerManager::GetStatus(int ID)
{
    return TimerSets[ID].Enabled;
}

//---------------------------------------------------------------------------
void __fastcall TTimerManager::Init()
{
    int i;
    for(i=0;i<100;i++)
        TimerSets[i].Exists=false;
}
//---------------------------------------------------------------------------
TTimerManager::TTimerManager()
{
    Ticks=0;
}

void TVisualManager::InitDrawingInterface(LPD3DXSPRITE* pSprite)
{
    Sprite=pSprite;
}

