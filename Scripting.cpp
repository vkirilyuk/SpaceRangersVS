//---------------------------------------------------------------------------


#pragma hdrstop

#include "Scripting.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
string __fastcall ZeroRegister(string Str)
{
    string TempString=Str;
    for(int i=1;i<=TempString.length();i++)
    if(((int)TempString[i]>=65)&&((int)TempString[i]<=90))
        TempString[i]+=32;
    return TempString;
}
//---------------------------------------------------------------------------
int __fastcall GetSomeSymbolCount(string String,int Index,char Symbol)
{
    int Count=0;
    for(int i=1;i<Index;i++)
        if(String[i]==Symbol)
            Count++;
    return Count;
}
//---------------------------------------------------------------------------
TScriptParameters __fastcall GetScriptParameters(string Script)
{
/*    string Parameters;
    string CurrentParameters;
    int Pos;
    int a=-1;
    TScriptParameters Param;
    char SomeSymbol=127;
    //char X=34;// 34="
    char X=39;  // 39='

    Param.Count=0;

    Parameters=Script;

    Pos=Parameters.Pos(")");
    Parameters.Delete(Pos,1);
    Parameters.Insert(",",Pos);

    for(int i=1;i<Parameters.Length();i++)
        if(Parameters[i]==',')
            if(GetSomeSymbolCount(Parameters,i,X)%2==1)
            {
            Parameters.Delete(i,1);
            Parameters.Insert(SomeSymbol,i);
            }

    while(Parameters.Pos(",")!=0)
    {
        a++;
        Pos=Parameters.Pos(",");
        CurrentParameters=Parameters;
        CurrentParameters.SetLength(Pos-1);
        Parameters.Delete(1,Pos);

        Param.Parameters[a]=CurrentParameters;
        Param.Count+=1;
    }

    for(int i=0;i<Param.Count;i++)
    {
        while(Param.Parameters[i].Pos(SomeSymbol)!=0)
        {
            int x=Param.Parameters[i].Pos(SomeSymbol);
            Param.Parameters[i].Delete(x,1);
            Param.Parameters[i].Insert(',',x);
        }
        while(Param.Parameters[i].Pos(X)!=0)
        {
            int x=Param.Parameters[i].Pos(X);
            Param.Parameters[i].Delete(x,1);
        }
    }

    return Param;     */
	return TScriptParameters();
}
//---------------------------------------------------------------------------
void __fastcall DoScript(string Script)
{
/*    //char a=34;  // 34=" You can't write it from keyboard...
    char a=39;  // 39=' You also can't write it from keyboard...

    for(int x=1;x<=Script.length();x++)
        if(Script[x]==' ')
        {
            int Count=0;
            for(int i=1;i<x;i++)
                if(Script[i]==a)
                    Count++;
            if(Count%2==0)
                while(Script[x]==' ')
                    Script.Delete(x,1);
        }

    int FirstBracket=Script.Pos("(");

    string FunctionName=Script;
    FunctionName.SetLength(FirstBracket-1);

    string Parameters=Script;
    Parameters.Delete(1,FirstBracket);

    TScriptParameters ScriptParameters=GetScriptParameters(Parameters);

    FunctionName=ZeroRegister(FunctionName);
    if(FunctionName==ZeroRegister("Close"))
    {
        exit(0);
    }
    if(FunctionName==ZeroRegister("RunApplication"))
    {
        char* Line=ScriptParameters.Parameters[0].c_str();
        int Param=StrToInt(ScriptParameters.Parameters[1]);
        WinExec(Line,Param);
    }
    if(FunctionName==ZeroRegister("CloseWindow"))
    {
        string WinName=ScriptParameters.Parameters[0];
        HDC hwnd=FindWindow(NULL,WinName.c_str());
        if(hwnd!=NULL)
            PostMessage(hwnd,WM_QUIT,0,0);
    }
    if(FunctionName==ZeroRegister("CreateMessageBox"))
    {
        string Text=ScriptParameters.Parameters[0];
        int FParam=StrToInt(ScriptParameters.Parameters[1]);
        int SParam=StrToInt(ScriptParameters.Parameters[2]);
        int TParam=StrToInt(ScriptParameters.Parameters[3]);

        //suck dicks

//        MessageBoxManager.AddMessageBox(Text,FParam,SParam,TParam);
    }
    if(FunctionName==ZeroRegister("UpgradeWeaponPower"))
    {
        int FParam=StrToInt(ScriptParameters.Parameters[0]);
        PlayerManager.Player.WeaponPower=FParam;
    }
    if(FunctionName==ZeroRegister("UnlockLifes"))
    {
        PlayerManager.Player.EnabledNewLifes=true;
    }
    if(FunctionName==ZeroRegister("UnlockWeapon"))
    {
        int FParam=StrToInt(ScriptParameters.Parameters[0]);
        int SParam=StrToInt(ScriptParameters.Parameters[1]);
        PlayerManager.RangerWeapon[FParam][SParam][1]=1;
    }
    if(FunctionName==ZeroRegister("AddCornerText"))
    {
        string Text=ScriptParameters.Parameters[0];
        int FParam=StrToInt(ScriptParameters.Parameters[1]);
        CornerTextManager.Add(Text,FParam);
    } */
}
//---------------------------------------------------------------------------
