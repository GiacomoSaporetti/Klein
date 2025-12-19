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
    if(karma > KARMA_THRESHOLD_HIGHEST)   {feelingTowardsPlayer = AGGRO_PROTECTIVE; return;}
    if(karma > KARMA_THRESHOLD_HIGH)   {feelingTowardsPlayer = AGGRO_FRIENDLY; return;}
    if(karma > KARMA_THRESHOLD_MID)   {feelingTowardsPlayer = AGGRO_NEUTRAL; return;}
    if(karma > KARMA_THRESHOLD_LOW)   {feelingTowardsPlayer = AGGRO_UNFRIENDLY; return;}
    if(karma > KARMA_THRESHOLD_LOWEST)   {feelingTowardsPlayer = AGGRO_AGGRESSIVE; return;}
}

void NPC::handleIA()
{
    switch(feelingTowardsPlayer)
    {
        case AGGRO_PROTECTIVE: executeProtectiveIA(); break;
        case AGGRO_FRIENDLY: executeFriendlyIA(); break;
        case AGGRO_NEUTRAL: executeNeutralIA(); break;
        case AGGRO_UNFRIENDLY: executeUnfriendlyIA(); break;
        case AGGRO_AGGRESSIVE: executeAggressiveIA(); break;
    }
}

void NPC::executeProtectiveIA(){; /*Needs to be overwritten by child class*/}
void NPC::executeFriendlyIA(){; /*Needs to be overwritten by child class*/}
void NPC::executeNeutralIA(){; /*Needs to be overwritten by child class*/}
void NPC::executeUnfriendlyIA(){; /*Needs to be overwritten by child class*/}
void NPC::executeAggressiveIA(){; /*Needs to be overwritten by child class*/}