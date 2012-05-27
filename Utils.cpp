
#include "Utils.h"
//---------------------------------------------------------------------------
RECT GetFourParametersFromString(string String)
{
  /*  int Param[4];
    String+=" ";
    for(int i=0;i<4;i++)
    {
        string Temp="";
        int Pos=String.pos(" ");
        for(int a=1;a<Pos;a++)
            Temp+=String[a];
        Param[i]=StrToInt(Temp);
        String.Delete(1,Pos);
    }
    RECT Ret;
    Ret.left=Param[0];
    Ret.top=Param[1];
    Ret.right=Param[2];
    Ret.bottom=Param[3];
    return Ret;     */
	RECT ret;
	return ret;
}
//---------------------------------------------------------------------------
string GetCurrentDateTime()
{
/*    int i;
    string CurrentTime;
    string CurrentDate;
    string CurrentDateTime;

    CurrentTime=TimeToStr(Time() ).c_str();
    CurrentDate=DateToStr(Date() ).c_str();

    while(CurrentTime.Pos(":")!=0)
    {
        i=CurrentTime.Pos(":");
        CurrentTime.Delete(i,1);
        CurrentTime.Insert(".",i);
    }

    while(CurrentDate.Pos(".")!=0)
    {
        i=CurrentDate.Pos(".");
        CurrentDate.Delete(i,1);
        CurrentDate.Insert("-",i);
    }

    CurrentDateTime=CurrentDate+" "+CurrentTime;
    return CurrentDateTime;           */
	return "";
}
//---------------------------------------------------------------------------
bool TestRects(RECT FirsRECT,RECT SecondRect)
{
    int a=FirsRECT.left;
    int b=FirsRECT.top;
    int c=FirsRECT.right;
    int d=FirsRECT.bottom;

    if(SecondRect.left>a) a=SecondRect.left;
    if(SecondRect.top>b) b=SecondRect.top;
    if(SecondRect.right<c) c=SecondRect.right;
    if(SecondRect.bottom<d) d=SecondRect.bottom;

    if(FirsRECT.right<=SecondRect.left) return false;
    if(FirsRECT.left>=SecondRect.right) return false;
    if(FirsRECT.top>=SecondRect.bottom) return false;
    if(FirsRECT.bottom<=SecondRect.top) return false;

    if((c-a)*(d-b)>0) return true;
        else return false;
}
//---------------------------------------------------------------------------
D3DXMATRIX Translate(const float dx, const float dy, const float dz,D3DXMATRIX Start)
{
    D3DXMATRIX ret;
    ret=Start;

    ret(3, 0) = dx;
    ret(3, 1) = dy;
    ret(3, 2) = dz;
    return ret;
}
//---------------------------------------------------------------------------
D3DXVECTOR3 CVector(int x,int y,int z)
{
    D3DXVECTOR3 Vec;
    Vec.x=x;
    Vec.y=y;
    Vec.z=z;
    return Vec;
}
//---------------------------------------------------------------------------
RECT UpdateRect(RECT Rect,int X,int Y)
{
    RECT Rect1;
    Rect1=Rect;
    Rect1.left+=X;
    Rect1.right+=X;
    Rect1.top+=Y;
    Rect1.bottom+=Y;
    return Rect1;
}
//---------------------------------------------------------------------------
vector<string> GetFileList(string What)
{
    vector<string> result;
    WIN32_FIND_DATA Data;
    HANDLE  Hnd;
    Hnd=FindFirstFile(What.c_str(),&Data);
    do {
        result.push_back(Data.cFileName);
    } while(FindNextFile(Hnd,&Data));
    FindClose(Hnd);
	return result;
}
//---------------------------------------------------------------------------
bool TestForCursorPosition(RECT rect,int x,int y)
{
    return (rect.left <= x && rect.right >= x && rect.top <= y && rect.bottom >= y);
}
//---------------------------------------------------------------------------
