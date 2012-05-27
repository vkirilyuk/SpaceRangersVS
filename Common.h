//---------------------------------------------------------------------------

#ifndef CommonH
#define CommonH

#include <vector>
#include <stack>
#include <string>
#include <sstream>

#define _USE_MATH_DEFINES
#include <math.h>

#include <windows.h>
using namespace std;

//#include <SysUtils.hpp>

//---------------------------------------------------------------------------

typedef void __fastcall TMyCallBack(void);

const string strlist[12]=
{"Пси-ускоритель","Дроид младший","Навинт","Пятерик",
"Проглот","Отморозки","ЛВВ","Антигравитатор",
"Нанитоиды","Поляризатор","Вжик","Кварковая бомба"};

const string strlists[12]=
{"Пси-ускоритель материи.","Дроида младшего.","Навинт.","Пятерик.",
"Проглот.","Отморозки.","ЛВВ.","Антигравитатор.",
"Нанитоидов.","Поляризатор.","Вжик.","Кварковую бомбу."};

const int FWCount[7]={7,14,7,6,0,0,6};
const int FWACount[5]={7,23,0,5,0};

const int AIPath=10;
const int AIT=11;
const int END_GAME_TIMER=12;
const int DEAD_TIMER=13;
const int RENDERING_TIMER=14;
const int ENGINE_TIMER=15;

/* Keys for Direct Input */

const int   BT_UP                   =200;
const int   BT_LEFT                 =203;
const int   BT_RIGHT                =205;
const int   BT_DOWN                 =208;
const int   BT_ALT                  =56;
const int   BT_CTRL                 =29;
const int   BT_SHIFT                =42;
const int   BT_SPACE                =47;
const int   BT_A                    =30;
const int   BT_S                    =31;
const int   BT_P                    =25;
const int   BT_O                    =24;
const int   BT_F                    =33;
const int   BT_M                    =50;
const int   BT_1                    =2;
const int   BT_2                    =3;
const int   BT_3                    =4;
const int   BT_4                    =5;
const int   BT_5                    =6;
const int   BT_NUMPLUS              =78;
const int   BT_NUMMINUS             =74;
const int   BT_ESC                  =1;
const int   BT_ENTER                =28;
const int   BT_LEFTBRACKET          =51;
const int   BT_RIGHTBRACKET         =52;
//const int   BT_BIGGER               =52;
//const int   BT_SMALLER              =51;
const int   BT_PAUSE                =197;



const int   MAX_TEXTURES            =250;
const int   MAX_SONGS               =512;
const int   MAX_CORNER_TEXTS        =50;
const int   MAX_MESSAGES            =50;

const int   MusicID=5;

const int   TR_TIME                     =1;
const int   TR_SCORE                    =2;
const int   TR_SHIPS_KILLED             =3;
const int   TR_ARTEFACT                 =4;
const int   TR_END_LEVEL                =5;

const int   Trigger_reason_time         =1;
const int   Trigger_reason_score        =2;
const int   Trigger_reason_ships_killed =3;
const int   Trigger_reason_artefact     =4;
const int   Trigger_reason_end_level    =5;

const int   GS_MAINMENU     =1;
const int   GS_BRIEFINGMENU =2;
const int   GS_AUTHORSMENU  =3;
const int   GS_CONTROLSMENU =4;
const int   GS_RECORDSMENU  =5;
const int   GS_GAME         =6;
const int   GS_ENTERINGNAME =7;
const int   GS_SAVINGMENU   =8;

const int   GS_PAUSEFULL    =10;
const int   GS_PAUSEDEAD    =20;
const int   GS_PAUSEWIN     =30;

const int   GS_MESSAGEBOX   =100;
const int   GS_ARTPANEL     =200;
const int   GS_FUCKINGPLACE =300;

const int   WP_OSCOL_MAIN   =1 ;
const int   WP_OSCOL_ALT    =10;
const int   WP_ROCKETS_MAIN =2 ;
const int   WP_ROCKETS_ALT  =20;
const int   WP_MINIGUN_MAIN =3 ;
const int   WP_MINIGUN_ALT  =30;
const int   WP_ELECTRO_MAIN =4 ;
const int   WP_ELECTRO_ALT  =40;
const int   WP_LASER_MAIN   =5 ;
const int   WP_LASER_ALT    =50;

const int   RACE_BLAZER     =0;
const int   RACE_KELLER     =1;
const int   RACE_TERRON     =2;

const int   FT_MAINMENU     =1;
const int   FT_GAME         =2;
const int   FT_AUTHORSMENU  =3;
const int   FT_RECORDSMENU  =4;
const int   FT_JUSTFUCKING  =25;


#define FCOLOR D3DCOLOR_ARGB(255,255,255,255)
#define TCOLOR D3DCOLOR_ARGB(255,255,0,255)

const int   ARTPANEL_HIDDEN     =0;
const int   ARTPANEL_SHOWING    =1;
const int   ARTPANEL_VISIBLE    =2;
const int   ARTPANEL_HIDING     =3;

const int   MESSAGETYPE_QUIT            =1;
const int   MESSAGETYPE_INFORMATION     =2;
const int   MESSAGETYPE_WINGAME         =3;
const int   MESSAGETYPE_LOSEGAME        =4;
const int   MESSAGETYPE_SCOREQUESTION   =5;

const int   MAX_GAME_BUTTONS        =50;

const double FON_SCALING       =1.96;
const int   EXPL_SCALING            =2;
const int   WEFFECT_SCALING         =2;

const int   SCREEN_WIDTH            = 800;
const int   SCREEN_HEIGHT           = 600;

const int   CS_KEYBOARD             =1;
const int   CS_MOUSE                =2;

const int   IC_INFORMATION          =1;
const int   IC_WARNING              =2;
const int   IC_ERROR                =3;
const int   IC_QUESTION             =4;

const int   LIGHT_BLUE              =10;
const int   LIGHT_RED               =20;

const int   GB_MESSAGE1             =1;
const int   GB_MESSAGE2             =2;
const int   GB_BRIEFING             =5;

const int   MBS_MESSAGENOW          =0;
const int   MBS_CANCREATE           =1;
const int   MBS_NOTHINGACCESS       =2;

const int   ST_SHOW                 =1;
const int   ST_DONTSHOW             =0;

const int   WEAPON_SCORE[5]     ={0,750,3000,10000,25000};

//---------------------------------------------------------------------------

  struct TPoint
  {
    TPoint() {}
    TPoint(int _x, int _y) { x=_x; y=_y; }
    TPoint(TPoint& pt)
    {
      x = pt.x;
      y = pt.y;
    }
        int x;
        int y;
  };
  


enum TMouseButton { mbLeft, mbRight, mbMiddle };

string str(int x);
int random(int n);
RECT CRect(int Left,int Top,int Right,int Bottom);

#endif
