//---------------------------------------------------------------------------

#ifndef mesmanH
#define mesmanH

#include "DirectX/d3dx9.h"
#include "common.h"
#include "GUI.h"
//---------------------------------------------------------------------------
class TMessageBoxManager
{
    private:
        TMyMessageBox Messages[MAX_MESSAGES];
        TMyMessageBox Temp;
        LPD3DXSPRITE* Sprite;
        int Count;

        AnsiString MessText;
        int MessType;
        int MessButtons;
//    int MeType;
        TStringList* MessTextList;

        int  __fastcall TestForAccess();
        void __fastcall CreateMessageBox();
        void __fastcall DrawMessageBox();

    public:
        void __fastcall Init(LPD3DXSPRITE* SpritePointer);
        void __fastcall AddMessageBox(AnsiString Text, int Type,int Buttons,int MessageID);
        void __fastcall Update();
};
//---------------------------------------------------------------------------
#endif
