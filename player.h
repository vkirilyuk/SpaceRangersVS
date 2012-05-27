//---------------------------------------------------------------------------

#ifndef playerH
#define playerH

#include <d3dx9.h>
#include "types.h"
#include "texturemanager.h"
//---------------------------------------------------------------------------

//---------------------Below will be the main game class---------------------
class TPlayerManager
{
    private:
		LPD3DXSPRITE* SpritePointer;
		LPDIRECT3DTEXTURE9 Texture;
		RECT   GraphRect;
        RECT   PhysicRect;
        int SpriteCount;
    public:

        TPlayer Player;
        double PlayerParameters[15];
        double RangerWeapon[6][2][10];

        void __fastcall Init(string IniFile, TextureManager * textureManager);
        void __fastcall CreatePlayerShip(TPlayer Ship,double ShipParameters[15]);
        void __fastcall RepairPlayerShip();
        void __fastcall HealPlayerShip();
        void __fastcall MovePlayerShip();
        void ProceedPhysics();
        void __fastcall TryPlayerFire();
        void __fastcall PlayerDead();
        void __fastcall UpdatePlayerSpeedOverMouse();
        int  __fastcall NewPlayerArtefact();
        void RebuildParameters();
        TPoint __fastcall GetPlayerPos();
        RECT __fastcall GetPlayerRect();
        void __fastcall InitDrawingInterface(LPD3DXSPRITE* DestSprite);
        void __fastcall Draw();
        int __fastcall  GetPlayerExp();
        void Clear();
        void StopMoving();
};

class   TPlayerShotsManager : public TVisualManager
{
    protected:
        TRangerShot  Shots[100];
    public:
        void Clear();
        void Draw();
        void Add(TPoint ppos,int ID,int WeaponLevel,double pp4,double pp6);
        void Move();
        bool Test(int ID,RECT Rect,double* EnemyHP);
};

#endif
