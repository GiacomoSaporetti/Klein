#include "TimeHandler.h"

using namespace Klein;

float TimeHandler::GetRealDelta()
{ return REAL.delta;}

float TimeHandler::GetRealTime()
{

    return REAL.time;
}

float TimeHandler::GetGameDelta()
{ return GAME.delta;}

float TimeHandler::GetGameTime()
{ return GAME.time;}

float TimeHandler::GetGameSpeed()
{ return GAME.speed;}

void TimeHandler::ChangeSpeed(float s)
{ GAME.speed = s;}

void TimeHandler::Run()
{
    timespec temp;
    //Update real time 
    clock_gettime(CLOCK_REALTIME, &temp);
    float new_time = (float)(temp.tv_sec - REAL.value.tv_sec) + (temp.tv_nsec - REAL.value.tv_nsec)/1000000000.0;
    REAL.delta = new_time - REAL.time;
    REAL.time = new_time;

    //Compute game time
    GAME.delta = GAME.speed*REAL.delta;
    GAME.time += GAME.delta;

}



