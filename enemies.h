//---------------------------------------------------------------------------

#ifndef enemiesH
#define enemiesH

#include <d3dx9.h>
#include "types.h"

//---------------------Very important class----------------------------------
class TEnemiesManager
{
    private:
        LPD3DXSPRITE SpritePointer;


        float DominatorScalingFactor[5];
        RECT DominatorDrawingRect;
    public:
        RECT DominatorRects[3][5];
        int EnemiesMax[6];
        int EnemiesNow[6];
        TEnemyShip  Enemies[6][50];
        int EnemySeries;
        void __fastcall Init();
        void __fastcall EnemyNew(int Type,int Index);
        void __fastcall EnemyDead(int Type,int Index);
        void __fastcall EnemySetMax(int Type,int MaxDom);
        void __fastcall EnemyDelete(int Type,int Index);
        void __fastcall MoveEnemies();
        TPoint __fastcall GetNearestEnemy(int X,int Y);
        TPoint __fastcall GetEnemyPosition(int Type,int Index);
        RECT  __fastcall GetEnemyRect(int Type,int Index);
        float __fastcall GetRocketSpeed(int RocketX,int RocketY,bool LargestShip);

        void __fastcall InitDrawingInterface(LPD3DXSPRITE DestSprite);
        void __fastcall Draw();
};

//---------------------------------------------------------------------------
class   TEnemyShotsManager : public TVisualManager
{
    private:
        TEnemyShot Shots[300];
        TPlayer* Ranger;
    public:
        void __fastcall Init(TPlayer* RangerPointer);
        void __fastcall Create(int Type,int X,int Y);
        void __fastcall Move();
        void Draw();
        void Clear();
        bool Test(int ID,RECT Rect,double* EnemyHP);
        TPoint GetPos(int ID);
        int GetDamage(int ID);

};
//---------------------------------------------------------------------------
#endif
