//---------------------------------------------------------------------------


#pragma hdrstop

#include "Triggers.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

#define Ranger          GetRangerPointer()
#define RangerStatistic GetStatisticPointer()
//---------------------------------------------------------------------------
void __fastcall TTriggerManager::AddTrigger(TTrigger Trigger)
{
    Triggers[Count]=Trigger;
    Count++;
}
//---------------------------------------------------------------------------
void __fastcall TTriggerManager::RemoveTrigger(int Index)
{
    for(int i=Index;i<Count-1;i++)
        Triggers[i]=Triggers[i+1];
    Count--;
}
//---------------------------------------------------------------------------
void __fastcall TTriggerManager::UpdateTriggers()
{
    for(int i=0;i<Count;i++)
    {
        switch(Triggers[i].Reason_type)
        {
            case Trigger_reason_time:
            {
                if(PlayerStatistic.GameTime>=Triggers[i].Reason_time)
                {
                    AssemblyTrigger(i);
                    i--;
                }
            }
            break;
            case Trigger_reason_score:
            {
                if(PlayerManager.Player.Experience>=Triggers[i].Reason_score)
                {
                    AssemblyTrigger(i);
                    i--;
                }
            }
            break;
            case Trigger_reason_ships_killed:
            {
//                i++;
            }
            break;
            case Trigger_reason_artefact:
            {
//                i++;
            }
            break;
            case Trigger_reason_end_level:
            {
//                i++;
            }
            break;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TTriggerManager::AssemblyTrigger(int Index)
{
    for(int j=0;j<Triggers[Index].Script_length;j++)
        DoScript(Triggers[Index].Script[j].c_str());
    RemoveTrigger(Index);
}
//---------------------------------------------------------------------------
