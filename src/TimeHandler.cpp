#include "TimeHandler.h"

namespace Klein
{
    TimeHandler     g_timer;
    
    /*Costruttore*/
    TimeHandler::TimeHandler()
    {
        clock_gettime(CLOCK_REALTIME, &m_real.value);
        m_real.time  = m_real.value.tv_sec * 1E9 + m_real.value.tv_nsec;
        m_real.delta = 0;
        m_game.time  = 0;
        m_game.delta = 0;
        m_game.speed = 1.f;

        KLEIN_DEBUG("TimeHandler", "initialized");
    }

  
    long TimeHandler::readRawTime() const
    {
        timespec now;
        clock_gettime(CLOCK_REALTIME, &now);
        return now.tv_sec * 1E9  + now.tv_nsec;
    }


    long  TimeHandler::getRealDelta() const { return m_real.delta; }
    long  TimeHandler::getRealTime()  const { return m_real.time;  }
    long  TimeHandler::getGameDelta() const { return m_game.delta; }
    long  TimeHandler::getGameTime()  const { return m_game.time;  }
    float TimeHandler::getGameSpeed() const { return m_game.speed; }


    void TimeHandler::setGameSpeed(float speed) { m_game.speed = speed; }


    void TimeHandler::tick()
    {
        clock_gettime(CLOCK_REALTIME, &m_real.value);
        m_real.delta    = m_real.value.tv_sec * 1E9 + m_real.value.tv_nsec - m_real.time;
        m_real.time     = m_real.value.tv_sec * 1E9 + m_real.value.tv_nsec;

        // Clamp per evitare salti enormi dovuti a non so cosa
        const long maxDelta = static_cast<long>(50 * 1E6);  // 50ms in nanosecondi
        if (m_real.delta > maxDelta) m_real.delta = maxDelta;

        m_game.delta    = static_cast<long>(m_game.speed * m_real.delta);
        m_game.time    += m_game.delta;

        //assert(m_real.delta < 1e9);
        //printf("m_real: %f (%f), m_game: %f (%f)\n", m_real.time/1E6, m_real.delta/1E6, m_game.time/1E6, m_game.delta/1E6);
    }

    bool TimeHandler::hasElapsed(unsigned int millis) const
    {
        return (readRawTime() - m_real.time) > (millis * 1E6);
    }


    void Tick() {g_timer.tick();}
    long GetGameTime() {return g_timer.getGameTime();}
    void SetGameSpeed(float s) {g_timer.setGameSpeed(s);}
    void Wait(unsigned int millis) {while(!g_timer.hasElapsed(millis)){;}}
} // namespace Klein