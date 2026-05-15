#pragma once

#include "Klein.h"
#include "TimeHandler.h"

namespace Klein
{
  extern TimeHandler g_timer;

  enum class KeyState 
  {
    IDLE,       //Non premuto
    PRESSED,    //Primo frame in cui viene premuto
    HELD,       //Tenuto premuto
    RELEASED    //Primo frame in cui viene rilasciato
  };

  struct key_info_t
  {
    unsigned int  key_id = 0; //Inutile?
    KeyState      state = KeyState::IDLE;
    unsigned int  ns_pressed = 0;
  };

  struct keys_state_t
  {
    key_info_t left, right; //Movimento
    key_info_t jump;        //Salto
    key_info_t attack;      //Attacco
    key_info_t dash;        //Schivata
    key_info_t interact;    //Interazione

    /*Abilità temporali*/

    key_info_t rewind;         
    key_info_t freeze;
    key_info_t fastforward;  
    key_info_t slowforward;
    key_info_t echo;                                  
  };

  struct mouse_state_t
  {
    float x, y;     //Coordinate del mouse
    uint32_t mask;  
  };

  class InputHandler
  {
    public:
      InputHandler();
      ~InputHandler() = default;

      void pollUserInput();

      const keys_state_t&  getKeysState()   const;
      const mouse_state_t& getMouseState()  const;

    private:
      keys_state_t  m_keys_state;
      mouse_state_t m_mouse_state;

  };

}//namespace Klein