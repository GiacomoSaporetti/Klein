#include "NPC.h"

using namespace Klein;

void NPC::IncreaseKarma(int k)
{
    Karma += k;
    if(Karma > KARMA_THRESHOLD_0)
    {feeling = PROTECTIVE; return;}
    if(Karma > KARMA_THRESHOLD_1)
    {feeling = FRIENDLY; return;}
    if(Karma > KARMA_THRESHOLD_2)
    {feeling = NEUTRAL; return;}
    if(Karma > KARMA_THRESHOLD_3)
    {feeling = UNFRIENDLY; return;}
    if(Karma > KARMA_THRESHOLD_4)
    {feeling = AGGRESSIVE; return;}
}
void NPC::DecreaseKarma(int k)
{
    Karma -= k;
    if(Karma > KARMA_THRESHOLD_0)
    {feeling = PROTECTIVE; return;}
    if(Karma > KARMA_THRESHOLD_1)
    {feeling = FRIENDLY; return;}
    if(Karma > KARMA_THRESHOLD_2)
    {feeling = NEUTRAL; return;}
    if(Karma > KARMA_THRESHOLD_3)
    {feeling = UNFRIENDLY; return;}
    if(Karma > KARMA_THRESHOLD_4)
    {feeling = AGGRESSIVE; return;}
}

void NPC::HandleIA()
{
    switch(feeling)
    {
        case PROTECTIVE: ProtectiveIA(); break;
        case FRIENDLY: FriendlyIA(); break;
        case NEUTRAL: NeutralIA(); break;
        case UNFRIENDLY: UnfriendlyIA(); break;
        case AGGRESSIVE: AggressiveIA(); break;
    }
}

void NPC::ProtectiveIA(){; /*Needs to be overwritten by child*/}
void NPC::FriendlyIA(){; /*Needs to be overwritten by child*/}
void NPC::NeutralIA(){; /*Needs to be overwritten by child*/}
void NPC::UnfriendlyIA(){; /*Needs to be overwritten by child*/}
void NPC::AggressiveIA(){; /*Needs to be overwritten by child*/}