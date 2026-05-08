#pragma once

#include "Klein.h"

namespace Klein
{
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

        /*Tempo trascorso dall'ultimo tick nel clock reale [secondi]*/
        float getRealDelta() const;

        /*Tempo totale accumulato nel clock reale [secondi]*/
        float getRealTime()  const;

        /*Tempo trascorso dall'ultimo tick nel clock di gioco [secondi]*/
        float getGameDelta() const;

        /*Tempo totale accumulato nel clock di gioco [secondi]*/
        float getGameTime()  const;

        /*Moltiplicatore di velocità del tempo di gioco (1.0 = tempo reale)*/
        float getGameSpeed() const;

        /*Imposta il moltiplicatore di velocità del tempo di gioco*/
        void setGameSpeed(float speed);

        /*Aggiorna entrambi i clock. Da chiamare una volta per frame*/
        void tick();

        /*Restituisce 'true' se sono trascorsi almeno 'millis' secondi dall'ultimo tick*/
        bool hasElapsed(float millis) const;

    private:
        time_profile_t m_real; // Clock del tempo reale
        time_profile_t m_game; // Clock del tempo di gioco

        /*Legge il tempo reale trascorso dall'origine [secondi]*/
        float readRawTime() const;
    };

} //namespace Klein