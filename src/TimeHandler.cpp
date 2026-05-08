#include "TimeHandler.h"

namespace Klein
{
    /*Costruttore*/
    TimeHandler::TimeHandler()
    {
        clock_gettime(CLOCK_REALTIME, &m_real.value);
        m_real.time  = 0.f;
        m_real.delta = 0.f;
        m_game.time  = 0.f;
        m_game.delta = 0.f;
        m_game.speed = 1.f;
    }

  
    float TimeHandler::readRawTime() const
    {
        timespec now;
        clock_gettime(CLOCK_REALTIME, &now);
        return static_cast<float>(now.tv_sec  - m_real.value.tv_sec)
             + static_cast<float>(now.tv_nsec - m_real.value.tv_nsec) * 1e-9f;
    }


    float TimeHandler::getRealDelta() const { return m_real.delta; }
    float TimeHandler::getRealTime()  const { return m_real.time;  }
    float TimeHandler::getGameDelta() const { return m_game.delta; }
    float TimeHandler::getGameTime()  const { return m_game.time;  }
    float TimeHandler::getGameSpeed() const { return m_game.speed; }


    void TimeHandler::setGameSpeed(float speed) { m_game.speed = speed; }


    void TimeHandler::tick()
    {
        const float now = readRawTime();
        m_real.delta    = now - m_real.time;
        m_real.time     = now;

        m_game.delta    = m_game.speed * m_real.delta;
        m_game.time    += m_game.delta;
    }

    bool TimeHandler::hasElapsed(float millis) const
    {
        return (readRawTime() - m_real.time) > millis;
    }

} // namespace Klein