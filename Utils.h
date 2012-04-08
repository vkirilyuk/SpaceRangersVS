//---------------------------------------------------------------------------

#ifndef UtilsH
#define UtilsH
#include <d3dx9.h>
#include "Common.h"
//#include <classes.hpp>
//---------------------------------------------------------------------------
//RECT __fastcall GetFourParametersFromString(string String);
string GetCurrentDateTime();
D3DXVECTOR3 CVector(int x,int y,int z);
D3DXMATRIX Translate(const float dx, const float dy, const float dz,D3DXMATRIX Start);
bool  TestRects(RECT FirstRect,RECT SecondRect);
RECT  UpdateRect(RECT Rect,int X,int Y);
vector<string> GetFileList(string What);
bool  TestForCursorPosition(RECT Rect,int X,int Y);
//---------------------------------------------------------------------------

#endif
