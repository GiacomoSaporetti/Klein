#pragma once

#include "Klein.h"

namespace Klein
{
    class TimeHandler
    {
        private:
            time_profile_t REAL;
            time_profile_t GAME;
        public:
            TimeHandler()
            {
                clock_gettime(CLOCK_REALTIME, &REAL.value);
            }
            ~TimeHandler() = default;   

            float getRealDelta();
            float getRealTime();
         
            float getGameDelta();
            float getGameTime();
            float getGameSpeed();

            void ChangeSpeed(float s);

            void Run();

            bool WaitUntil(float millis);
    };
}