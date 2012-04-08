//---------------------------------------------------------------------------

#ifndef TypesH
#define TypesH
#include <d3dx9.h>
#include "fmod/inc/fmod.h"
#include "fmod/inc/fmod_errors.h"
#include "Common.h"
#include "Utils.h"
//---------------------------------------------------------------------------
class GObject
{
    public:
        double xpos;
        double ypos;
        bool Exists;
};
//---------------------------------------------------------------------------

class TVisualManager
{
    protected:
        LPD3DXSPRITE* Sprite;
    public:
        void InitDrawingInterface(LPD3DXSPRITE* pSprite);
};

struct TLaserParam
{
    string TexturePath;
    int DeltaX;
    int DeltaY;
    float ScalingX;
    float ScalingY;
};
//---------------------------------------------------------------------------
struct TRangerShot : public GObject
{
    double XSpeed,YSpeed;
    int Damage;
    int Type;
    bool Second;
    RECT ShoRECT;
    RECT ShoRECTDef;
    RECT DrawingRect;
    RECT DrawingRectDef;
    int MaxFrames;
    int CurrentFrame;
    int AnimationLevel;
    int AnimationProgress;
    int AnimationEnd;
    int a,b,c,d,e,f;
    int ID;
};
//---------------------------------------------------------------------------
struct TEnemyShot : public GObject
{
    double XSpeed,YSpeed;
    int Damage;
    RECT ShoRECT;
    RECT ShoRECTDef;
    RECT DrawingRect;
    RECT DrawingRectDef;
};
//---------------------------------------------------------------------------
class TPlayer : public GObject
{
    public:
        double LeftSpeed;
        double TopSpeed;
        double RightSpeed;
        double BottomSpeed;
        double Experience;
        double HP;
        double MaxHP;
        int Lifes;
        int CurrentWeapon;
        double DroidRepair;
        int WeaponPower;
        bool MovingLeft;
        bool MovingRight;
        bool MovingUp;
        bool MovingDown;

        bool EnabledNewLifes;

        bool Visible;
};
//---------------------------------------------------------------------------
// just a bit of this and a bit of that
/*
class TEnemyShip : public GObject
{

};
class EnemyShipBase : public GObject
{
public:
    virtual void draw() {
    }
    virtual RECT getBounds() {
        return shipRect;
    };       */
struct TEnemyShip: public GObject {
    double HP;
    double XSpeed;
    double YSpeed;
    int WeaponPower;
    int ExperienceForMurder;
    RECT DrawingRect;
    RECT DrawingRectDef;
    RECT ShipRect;
    RECT ShipRectDef;
    double CurrentFrame;
    int MaxFrames;
    int MaxShots;
    int ShotsMade;
    bool Locked;
};

//---------------------------------------------------------------------------
struct TRangerParameters
{
    int Kvarks;
    int WeaponPowerPlus;
    double WeaponPowerIncrease;
    double SpeedIncrease;
    double WeaponFireSpeedIncrease;
    int DroidRepairPlus;
    int ProtectionUp;
    bool BigChanceForArtefact;
};
//---------------------------------------------------------------------------

struct TTeslaWeapon
{
    int Power;
    int Radius;
    int ChargeMax;
    int ChargeLast;
    bool Exists;
};
//---------------------------------------------------------------------------
struct TLightning : public GObject
{
    int Power;
    int JumpesLeft;
};
//---------------------------------------------------------------------------
struct TFontParameters
{
    string Name;
    int Size;
};
//---------------------------------------------------------------------------
TFontParameters __fastcall CreateFontParameters(string Name,int Size);
//---------------------------------------------------------------------------
struct TStatistic
{
    int KilledShtips;
    int KilledMenoks;
    int KilledSmershes;
    int KilledUrgants;
    int KilledEcventors;
    int ShotsFired;
    float GameTime;
    int DamageCause;
    int DamageTaken;
    int ArtefactsPickedUp;
    int ArtefactsLoosed;
    int ShotsHit;
    int WeaponGameTimes[5];
    bool ShowStatistic;
    void __fastcall Clear();
};
//---------------------------------------------------------------------------
class TFPSShower
{
    private:
        long FPS;
        long LastTickCount;
        long CurrentTickCount;
        long FrameCount;
//----------Functions----------------------------
    public:
        void __fastcall Init();
        int __fastcall GetFPS();
        void __fastcall UpdateFPS();
};
//---------------------------------------------------------------------------
class TFontManager
{
    private:
        LPD3DXFONT Fonts[25];
        bool        Exist[25];
        LPDIRECT3DDEVICE9* FontDevice;
        LPD3DXSPRITE*      SpritePointer;
        TFontParameters FontParams[25];

    public:
        void            Init(LPDIRECT3DDEVICE9* Device,LPD3DXSPRITE* Sprite);
        void            RestoreAll();
        LPD3DXFONT      GetFont(int Index);
        int             AddFont(string Name,int Size);
        void            AddFontEx(int Index, string Name,int Size);
        void            Draw(int Index, string String, int Left, int Top, D3DCOLOR Color);
};
//---------------------------------------------------------------------------
struct TCornerText
{
    string  Text;
    int         Time;
};
//---------------------------------------------------------------------------
class TCornerTextManager
{
    private:
        TCornerText TextQueue[50];
        int         Count;
        LPD3DXFONT* Font;

    public:
        void __fastcall Init(LPD3DXFONT* FontPointer);
        TCornerText __fastcall GetCurrent();
        void __fastcall Add(string Text,int Time);
        void __fastcall Update();
        bool __fastcall TextExists();
        TCornerText __fastcall GetText(int Index);
        void __fastcall DrawText();
        void __fastcall DrawTextEx(int HowMuch);
};
//---------------------------------------------------------------------------
struct TSongType
{
    FMUSIC_MODULE*  Module;
    FSOUND_STREAM*  Stream;
    int             Channel;
    bool            Playing;
    string          FileName;
};
//---------------------------------------------------------------------------
class TMusicManager
{
    private:
    public:
        TSongType Songs[MAX_SONGS];
        bool __fastcall Init(HDC Handle);
        void __fastcall SetGlobalVolume(int Volume);
        void __fastcall SetVolume(int Index,int Volume);
        int  __fastcall OpenFile(string FileName);
        void __fastcall OpenFileEx(string FileName,int Index);
        void __fastcall PlayFile(int Index);
        void __fastcall StopFile(int Index);
        void __fastcall PauseFile(int Index);
        void __fastcall StopAll();
        int  __fastcall OpenAndPlay(string FileName);
        void __fastcall SetLoop(int Index,bool Loop);
        void __fastcall SetGlobalLoop(bool Loop);
        int  __fastcall OpenAndPlayEx(string FileName,int FIndex,int SIndex,bool Loop);
};
//---------------------------------------------------------------------------
struct TTextureParam
{
    string TextureName;
    LPDIRECT3DTEXTURE9* Texture;
    bool InFile;
    int Height;
    int Width;
    D3DCOLOR Color;
    bool Ex;
};
//---------------------------------------------------------------------------

class TTextureManager
{
    private:
        int Count;
        TTextureParam Textures[MAX_TEXTURES];
        LPDIRECT3DDEVICE9* OurDevice;
        HMODULE Source;
    public:
        void __fastcall Init(LPDIRECT3DDEVICE9* Device, HMODULE TexSource);
        int  __fastcall Create(string TexName,bool InFile,LPDIRECT3DTEXTURE9* Texture);
        int  __fastcall CreateEx(string TexName,bool InFile,int Width,int Height,D3DCOLOR Color,LPDIRECT3DTEXTURE9* Texture);
        void __fastcall FreeAll();
        void __fastcall ReloadAll();
        void __fastcall Free(int Index);
        int  __fastcall FindByName(string TexName);
        int  __fastcall FindByTexture(LPDIRECT3DTEXTURE9* Texture);
};
//---------------------------------------------------------------------------
struct  TTimerSet
{
    int Interval;
    double LastTime;
    bool Enabled;
    TMyCallBack* CallBack;
    bool Exists;
};
//---------------------------------------------------------------------------
class   TTimerManager
{
    private:
        TTimerSet   TimerSets[100];
        double Ticks;
    public:
        TTimerManager();
        void __fastcall Init();
        void __fastcall AddTimerEx(int ID,int Interval, TMyCallBack* CallBack, bool Enabled);
        int  __fastcall AddTimer(int Interval, TMyCallBack* CallBack, bool Enabled);
        void Update(double Delta);
        void __fastcall Remove(int ID);
        void __fastcall SetStatus(int ID, bool Enabled);
        bool __fastcall GetStatus(int ID);
};


#endif

