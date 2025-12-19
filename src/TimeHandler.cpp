#include "TimeHandler.h"

using namespace Klein;

float TimeHandler::getRealDelta(){ return REAL.delta;}

float TimeHandler::getRealTime(){ return REAL.time;}

float TimeHandler::getGameDelta(){ return GAME.delta;}

float TimeHandler::getGameTime(){ return GAME.time;}

float TimeHandler::getGameSpeed(){ return GAME.speed;}

void TimeHandler::ChangeSpeed(float s){ GAME.speed = s;}

bool TimeHandler::WaitUntil(float millis)
{ 
    timespec temp;
    clock_gettime(CLOCK_REALTIME, &temp);
    float new_time = (float)(temp.tv_sec - REAL.value.tv_sec) + (temp.tv_nsec - REAL.value.tv_nsec)/1000000000.0;
    return (new_time - REAL.time) > millis;
}

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



