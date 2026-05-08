#include "NPC.h"

using namespace Klein;

void NPC::increaseKarma(int k)
{
    karma += k;
    UpdateFeelingsTowardsPlayer();
}
void NPC::decreaseKarma(int k)
{
    karma -= k;
    UpdateFeelingsTowardsPlayer();
}

void NPC::UpdateFeelingsTowardsPlayer()
{
    if(karma > KARMA_THRESHOLD_HIGHEST) {feelingTowardsPlayer = AggroLevel::Protective; return;}
    if(karma > KARMA_THRESHOLD_HIGH)    {feelingTowardsPlayer = AggroLevel::Friendly; return;}
    if(karma > KARMA_THRESHOLD_MID)     {feelingTowardsPlayer = AggroLevel::Neutral; return;}
    if(karma > KARMA_THRESHOLD_LOW)     {feelingTowardsPlayer = AggroLevel::Unfriendly; return;}
    if(karma > KARMA_THRESHOLD_LOWEST)  {feelingTowardsPlayer = AggroLevel::Aggressive; return;}
}

void NPC::handleIA()
{
    switch(feelingTowardsPlayer)
    {
        case AggroLevel::Protective: executeProtectiveIA(); break;
        case AggroLevel::Friendly: executeFriendlyIA(); break;
        case AggroLevel::Neutral: executeNeutralIA(); break;
        case AggroLevel::Unfriendly: executeUnfriendlyIA(); break;
        case AggroLevel::Aggressive: executeAggressiveIA(); break;
    }
}

void NPC::executeProtectiveIA(){; /*Needs to be overwritten by child class*/}
void NPC::executeFriendlyIA(){; /*Needs to be overwritten by child class*/}
void NPC::executeNeutralIA(){; /*Needs to be overwritten by child class*/}
void NPC::executeUnfriendlyIA(){; /*Needs to be overwritten by child class*/}
void NPC::executeAggressiveIA(){; /*Needs to be overwritten by child class*/}