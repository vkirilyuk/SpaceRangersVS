//---------------------------------------------------------------------------

#ifndef TriggersH
#define TriggersH

#include "Scripting.h"
//---------------------------------------------------------------------------

struct TTrigger
{
    bool Exists;

    int Reason_type;

    //-------
    int Reason_time;
    int Reason_score;
    char Reason_ship_type[35];
    int Reason_ship_killer;
    char Reason_artefact_type[35];

    string Script[100];
    int Script_length;
};
//---------------------------------------------------------------------------

class TTriggerManager
{
    private:
        int Count;
        TTrigger Triggers[100];
    //-----------------------------------------------
    public:
        void __fastcall AddTrigger(TTrigger Trigger);
        void __fastcall RemoveTrigger(int Index);
        void __fastcall UpdateTriggers();
        void __fastcall AssemblyTrigger(int Index);
//-----------End of functions--------------------
};

#endif
