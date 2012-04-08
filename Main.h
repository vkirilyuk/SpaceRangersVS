//---------------------------------------------------------------------------

#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#include <windows.h>

#include <d3dx9.h>
#include <d3d9types.h>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#include "Utils.h"
#include "GUI.h"

#include "Common.h"
#include "Types.h"
#include "Scripting.h"
#include "Triggers.h"
#include "enemies.h"
#include "player.h"
#include "texturemanager.h"

//---------------------------------------------------------------------------
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

void __fastcall MoveShleif();

void __fastcall KeyDown( int Key, LPARAM lParam);
void __fastcall KeyUp( int Key, LPARAM lParam);

void __fastcall MouseMove( WPARAM wParam, LPARAM lParam);
void __fastcall MouseDown( WPARAM wParam, LPARAM lParam, int Button);
void __fastcall MouseWheel(WPARAM wParam, LPARAM lParam);

void __fastcall KeyPress(WPARAM wParam, LPARAM lParam);

void __fastcall End_game_timer();
void __fastcall DeadTimer();

//--------------Here I'm trying to extern function without new errors--------
void __fastcall StartBriefing(int Number);
void __fastcall ShowArtefactPanel();
void __fastcall HideArtefactPanel();
void __fastcall StartTimers(bool Start);
void __fastcall UpdateMusicChannels();
void __fastcall BackToMainMenu();
void __fastcall RenderingTimer();
void __fastcall EngineTimer();

//-----------------------Other eugine functions------------------------------
void __fastcall MoveArtefacts();
void __fastcall MoveStars();
void __fastcall MoveShleif();
void __fastcall FirstWeaponShieldExplode(int X,int Y,int Power);
void __fastcall ShipShleif(int X,int Y);
extern void __fastcall Pause(int Type);
void __fastcall SuperWeapon();
void __fastcall ActivateCheat(int Cheat);
void __fastcall Exploding(int type,int X,int Y);
void __fastcall NewArtefact(int X,int Y);
void __fastcall UpdateGameOverNewScore();
void __fastcall MoveRocketShleif();

void __fastcall ShowScreen();

//----------------------------AI functions-----------------------------------
TPoint __fastcall SetNewCource();
void __fastcall AITimer();
void __fastcall AIRepathTimer();

//---------------------------Controls events---------------------------------
void __fastcall MessageButton1Event();
void __fastcall MessageButton2Event();
void __fastcall BriefingButtonEvent();
void __fastcall RecordButton();
void __fastcall ControlsButton();
void __fastcall DevelopersButton();
void __fastcall HideButtons(bool Hide);

//--------------Initialization game, DirectX and textures--------------------
void __fastcall LoadResources();
void __fastcall TestForCrash();
void __fastcall SetConstants();
HRESULT InitD3D(HWND hWnd );
bool __fastcall InitConfig();
void __fastcall LoadD3DXResources();
void __fastcall RestoreMyD3D();

void __fastcall InitManagers();
void __fastcall InitControls();
void __fastcall InitTriggers();

//----------------------------Drawing something------------------------------
void __fastcall DrawLighting(int X1,int Y1,int X2,int Y2,D3DCOLOR Color);

void __fastcall DrawSavingMenu();
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

void __fastcall Time9();
void __fastcall Time18();


void __fastcall PlayerShipFire(int WeaponType,int WeaponLevel,bool SecondWeapon);
void ProceedPhysics();


extern void PlayerFire(int xpos, int ypos);

//---------------------------------------------------------------------------
//extern  TTriggerManager     TriggerManager;
extern  TFontManager        FontManager;
extern  TCornerTextManager  CornerTextManager;
extern  TNumberShower       NumberShower;
extern  TButtonsManager     ButtonsManager;
extern  TPlayerManager      PlayerManager;
extern  TEnemiesManager     EnemiesManager;
extern  TTextureManager     TextureManager;
extern  TMusicManager       MusicManager;
extern  TTimerManager      *TimerManager;
extern  TFPSShower          FPSShower;
extern  TStatistic          PlayerStatistic;
extern  TEnemyShotsManager *EnemyShotsManager;


extern D3DXMATRIXA16 IdentityMatrix;
extern RECT DrawingRects[15];
extern RECT DominatorRects[3][5];
extern RECT DominatorDrawingRect;
extern float DominatorScalingFactor[5];
extern LPDIRECT3DTEXTURE9 DominatorUno[3][5];

extern int Lines[100][10];
extern int RocketShleif[150][5];
extern double Boss[15];


extern LPDIRECT3DTEXTURE9 Angle[4];
extern LPDIRECT3DTEXTURE9 Line[4];
extern LPDIRECT3DTEXTURE9 Icons[4];
extern LPDIRECT3DTEXTURE9 Light[2];
extern LPDIRECT3DTEXTURE9 Centre;

extern LPDIRECT3DTEXTURE9 DominatorShot;

extern LPD3DXFONT   MainFont;
extern TMyD3DButton MessageButton1;
extern TMyD3DButton MessageButton2;

extern int Laser[10];

extern string TempFolder;

extern bool Fire,FireAlt,Undamager,PauseNow,CanShot,AlreadyPressed;
extern int Artefacts[100][3];
extern bool RangerDeads;
extern int Shleif[100][6];
extern int SomeShit;
extern TPoint PPos;
extern int RangerArtefacts[15];
extern double FireInterval;
extern TTeslaWeapon RangerTesla;
extern TLightning Lightnings[100];
extern LPDIRECT3DTEXTURE9 WeaponImages[5][3];
//---------------------------------------------------------------------------
#endif
