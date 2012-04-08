//---------------------------------------------------------------------------


#pragma hdrstop

#include "GUI.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
void __fastcall TButtonsManager::Init()
{
    ZeroMemory(Buttons,sizeof(TMyD3DButton)*100);
    ZeroMemory(CallBacks,sizeof(TMyCallBack*)*100);
    ZeroMemory(&SpritePointer,sizeof(LPD3DXSPRITE*));
}
//---------------------------------------------------------------------------
void __fastcall TButtonsManager::InitDrawingInterface(LPD3DXSPRITE* DestSprite)
{
    SpritePointer=DestSprite;
}
//---------------------------------------------------------------------------
void __fastcall TButtonsManager::Add(TMyD3DButton Button,int Index,TMyCallBack* CallBack)
{
    Buttons[Index]=Button;
    CallBacks[Index]=CallBack;
}
//---------------------------------------------------------------------------
void __fastcall TButtonsManager::MouseDown(int X,int Y)
{
    for(int i=0;i<100;i++)
        if(Buttons[i].Visible)
        {
            if(TestForCursorPosition(Buttons[i].Position,X,Y))
            {
                if (Buttons[i].CurrentState == 1)
                    Buttons[i].CurrentState=3;
            }
            else
                Buttons[i].CurrentState=1;
        }
}
//---------------------------------------------------------------------------
void __fastcall TButtonsManager::MouseMove(int X,int Y)
{
    for(int i=0;i<100;i++)
        if(Buttons[i].Visible)
            Buttons[i].CurrentState = TestForCursorPosition(Buttons[i].Position,X,Y) ? 2 : 1;
}
//---------------------------------------------------------------------------
void __fastcall TButtonsManager::MouseUp(int X,int Y)
{
    for(int i=0;i<100;i++)
        if(Buttons[i].Visible)
            if(TestForCursorPosition(Buttons[i].Position,X,Y))
                (*CallBacks[i])();
}
//---------------------------------------------------------------------------
void __fastcall TButtonsManager::ChangeVisible(int Index,bool Visible)
{
    Buttons[Index].Visible=Visible;
}
//---------------------------------------------------------------------------
void __fastcall TButtonsManager::Draw()
{
    for(int i=0;i<100;i++)
        if(Buttons[i].Visible)
        {
            (*SpritePointer)->Draw(Buttons[i].Texture,
                                    &Buttons[i].States[Buttons[i].CurrentState-1],
                                    NULL,&CVector(Buttons[i].Position.left,Buttons[i].Position.top,0),
                                    FCOLOR);
        }
}
//---------------------------------------------------------------------------
void __fastcall TNumberShower::Init(LPD3DXFONT* FontPointer)
{
    Count=0;
    Font=FontPointer;
}
//---------------------------------------------------------------------------
void __fastcall TNumberShower::Add(int* Number, int X, int Y, int State)
{
    Q[Count]=Number;
    Pos[Count].x=X;
    Pos[Count].y=Y;
    States[Count]=State;
    Count++;
}
//---------------------------------------------------------------------------
void __fastcall TNumberShower::ChangeState(int* Number, int State)
{
    for(int i=0;i<Count;i++)
        if(Q[i]==Number)
            States[i]=State;
}
//---------------------------------------------------------------------------
void __fastcall TNumberShower::Draw()
{
    for(int i=0;i<Count;i++)
        if(States[i]==ST_SHOW)
        {
            D3DCOLOR CurrColor=D3DCOLOR_XRGB(255,0,0);
            (*Font)->DrawTextA(NULL,str(*Q[i]).c_str(),-1,&CRect(Pos[i].x,Pos[i].y,1000,1000),0,CurrColor);
        }
}
//---------------------------------------------------------------------------
