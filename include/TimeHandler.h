#pragma once

#include "Klein.h"
#include "Entity.h"
#include <stdint.h>
#include <stdio.h>
#include <time.h>

namespace Klein
{
    class Entity;
    /**
     * @brief Struttura per la gestione del tempo
     */
    struct time_profile_t
    {
        timespec value{};      
        long     delta = 0;     // Tempo trascorso dall'ultimo tick [nanosecondi]
        long     time  = 0;     // Tempo accumulato dall'inizio [nanosecondi]
        float    speed = 1.f;   // Moltiplicatore di velocità (1.0 = tempo reale)
    };

    void Tick();
    long GetGameTime();
    void SetGameSpeed(float s);
    void Wait(unsigned int millis);

    /**
     * @brief Gestisce il tempo reale e il tempo di gioco, con supporto a velocità variabile.
     *
     * Il tempo di gioco scorre a una velocità proporzionale a 'speed' rispetto al tempo reale.
     * Chiamare 'tick()' ogni frame per aggiornare entrambi i clock.
     */
    class TimeHandler
    {
    public:
        TimeHandler();
        ~TimeHandler() = default;

        /*Getters*/

        long getRealDelta() const;
        long getRealTime()  const;
        long getGameDelta() const;
        long getGameTime()  const;
        float getGameSpeed() const;

        /*Imposta il moltiplicatore di velocità del tempo di gioco*/
        void setGameSpeed(float speed);

        /*Aggiorna i clock. Da chiamare una volta per frame*/
        void tick();

        /*Restituisce 'true' se sono trascorsi almeno 'millis' secondi dall'ultimo tick*/
        bool hasElapsed(unsigned int millis) const;


    private:
        time_profile_t m_real; // Clock del tempo reale
        time_profile_t m_game; // Clock del tempo di gioco

        /*Legge il tempo reale trascorso dall'origine [secondi]*/
        long readRawTime() const;
    };

} //namespace Klein