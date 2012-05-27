//---------------------------------------------------------------------------

#ifndef TypesH
#define TypesH
#include <d3dx9.h>
#include "fmod/inc/fmod.h"
#include "fmod/inc/fmod_errors.h"
#include "Common.h"
#include "Utils.h"

class Rect: public RECT {
public:
	Rect() {
		left = 0, top = 0, right = 0, bottom = 0;
	}
	Rect(const RECT & rect) {
		left = rect.left;
		top = rect.top;
		right = rect.right;
		bottom = rect.bottom;
	}
	Rect(int left, int top, int right, int bottom) {
		this->left = left;
		this->top = top;
		this->right = right;
		this->bottom = bottom;
	}
	int width() {
		return this->right - this->left;
	}
	int height() {
		return this->bottom - this->top;
	}
	Rect shift(int x, int y) {
		return Rect(left + x, top + y, right + x, bottom + y);
	}
};

class Particle {
public:
	double x, y;
};

class Movable: public Particle {
public:
	double speed, dx, dy;
	virtual void move(double time) {
		x += speed * dx * time;
		y += speed * dy * time;
	}
};

class TexturedObject {
protected:
	Rect rect;
	LPDIRECT3DTEXTURE9 texture;
	D3DXVECTOR3 center;
	D3DXVECTOR3 position;
	D3DCOLOR color;
	bool hasRect, hasTexture, hasCenter, hasPosition, hasColor;
public:
	TexturedObject() {
		color = FCOLOR;
		hasRect = hasTexture = hasCenter = hasPosition = hasColor = NULL;
	}
	virtual LPDIRECT3DTEXTURE9 getTexture() { if (hasTexture) return texture; return NULL; }
	virtual RECT * getRect() { if (hasRect) return &rect; return NULL; }
	virtual D3DXVECTOR3 * getCenter() { if (hasCenter) return &center; return NULL; }
	virtual D3DXVECTOR3 * getPosition() { if (hasPosition) return &position; return NULL; }
	virtual D3DCOLOR getColor() { return color; }
	virtual void setTexture(LPDIRECT3DTEXTURE9 texture) {this->texture = texture; hasTexture = true; }
	virtual void setRect(RECT rect) { this->rect = rect; hasRect = true; }
	virtual void setCenter(D3DXVECTOR3 center) { this->center = center; hasCenter = true; }
	virtual void setPosition(D3DXVECTOR3 position) { this->position = position; hasPosition = true; }
	virtual void setColor(D3DCOLOR color) { this->color = color; hasColor = true; }
/*	void draw() {
		MainSprite->Draw(getTexture(), getRect(), getCenter(), getPosition(), getColor());
	} */
};

class AnimatedObject: public TexturedObject {
public:
	Rect animationRect;
	int animationCount;
	int animationStep;
public:
	AnimatedObject() {
		animationStep = 0;
		animationCount = 1;
	}
	void nextAnimation() {
		animationStep = (animationStep + 1) % animationCount;
	}
	RECT *getRect() {
		rect = animationRect.shift( animationRect.width() * animationStep, 0);
		return &rect;
	}
};

class MovableAnimatedObject: public AnimatedObject, public Movable {
public:
	D3DXVECTOR3 * getCenter() {
		center = CVector( animationRect.width() / 2, animationRect.height() / 2, 0);
		return &center;
	}
	D3DXVECTOR3 * getPosition() {
		position = CVector(x, y, 0);
		return &position;
	}
};

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

class GObject {
public:
	int xpos, ypos;
	bool Exists;
};

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

