//---------------------------------------------------------------------------

#ifndef ScriptingH
#define ScriptingH
#include "Main.h"
//---------------------------------------------------------------------------
struct TScriptParameters
{
    int Count;
    string Parameters[10];
};
//--------------------------------------------------------------------------

 //----------Begin-script-of-system---------------
void __fastcall DoScript(string Script);
TScriptParameters __fastcall GetScriptParameters(string Script);
int __fastcall GetSomeSymbolCount(string String,int Index,char Symbol);
string __fastcall ZeroRegister(string Str);
//-----------End-script-of-system----------------

#endif
