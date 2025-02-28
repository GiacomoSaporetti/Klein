#pragma once

#include "Klein.h"

namespace Klein
{
    class TimeHandler
    {
        private:
            time_profile REAL;
            time_profile GAME;
        public:
            TimeHandler()
            {
                clock_gettime(CLOCK_REALTIME, &REAL.value);
            }
            ~TimeHandler() = default;   

            float GetRealDelta();
            float GetRealTime();
         
            float GetGameDelta();
            float GetGameTime();
            float GetGameSpeed();

            void ChangeSpeed(float s);

            void Run();
    };
}