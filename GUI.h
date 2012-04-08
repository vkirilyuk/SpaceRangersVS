//---------------------------------------------------------------------------

#ifndef GUIH
#define GUIH

#include "d3dx9.h"
#include "Utils.h"
#include "Common.h"
//---------------------------------------------------------------------------
struct TMyD3DButton
{
    LPDIRECT3DTEXTURE9  Texture;
    LPD3DXFONT          Font;
    RECT                Position;
    RECT                States[3];
    int                 CurrentState;
    bool                Visible;
    int                 ID;
    string              Caption;
};
//---------------------------------------------------------------------------
struct TMyD3DCheckListBoxItem
{
    string Name;
    bool Checked;
};
//---------------------------------------------------------------------------
struct TMyD3DCheckListBox
{
    bool Visible;
    bool VisibleMainForm;
    LPDIRECT3DTEXTURE9 Texture;
    LPD3DXFONT Font;
    RECT Position;
    RECT CheckedItem;
    RECT UncheckedItem;
    RECT MainForm;
    int ItemCount;
    int CheckedItemNumber;
    TMyD3DCheckListBoxItem Items[50];
};
//---------------------------------------------------------------------------
struct TMyMessageBox
{
    string      Text;
    int         Type;
    int         Buttons;
    int         MessageID;
};
//---------Below will be VERY important class--------------------------------
class TButtonsManager
{
    private:
        TMyD3DButton Buttons[100];
        TMyCallBack* CallBacks[100];
        LPD3DXSPRITE* SpritePointer;
    public:
        void __fastcall Add(TMyD3DButton Button,int Index,TMyCallBack* CallBack);
        void __fastcall MouseDown(int X,int Y);
        void __fastcall MouseMove(int X,int Y);
        void __fastcall MouseUp(int X,int Y);
        void __fastcall Draw();
        void __fastcall Init();
        void __fastcall InitDrawingInterface(LPD3DXSPRITE* DestSprite);
        void __fastcall ChangeVisible(int Index,bool Visible);
};
//---------------------------------------------------------------------------
class TNumberShower
{
    private:
        LPD3DXFONT* Font;
        int* Q[1000];
        TPoint Pos[1000];
        int States[1000];
        int Count;
    public:
        void __fastcall Init(LPD3DXFONT* FontPointer);
        void __fastcall Draw();
        void __fastcall Add(int* Numbet, int X, int Y, int State);
        void __fastcall ChangeState(int* Numbet, int State);
};
//---------------------------------------------------------------------------


#endif
