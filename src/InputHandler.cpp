#include "InputHandler.h"
#include "SDL3/SDL.h"

namespace Klein
{
    InputHandler g_input_handler;

    void updateKey(key_info_t& key, bool is_pressed);

    InputHandler::InputHandler() {}


    void InputHandler::pollUserInput()
    {
        const bool * keys = SDL_GetKeyboardState(nullptr);

        updateKey(m_keys_state.left,   keys[SDL_SCANCODE_A]);
        updateKey(m_keys_state.right,  keys[SDL_SCANCODE_D]);
        updateKey(m_keys_state.jump,   keys[SDL_SCANCODE_SPACE]);
        /*Mancano dei tasti da assegnare*/

        Uint32 mouse_mask = SDL_GetMouseState(&m_mouse_state.x, &m_mouse_state.y);
        m_mouse_state.mask = mouse_mask;
    }

    /**
     * @brief Aggiorna lo stato del pulsante in base al suo stato attuale e a quello precendente
     */
    void updateKey(key_info_t& key, bool is_pressed)
    {
        if(is_pressed) 
        {
            /*Primo frame che viene premuto*/
            if(key.state == KeyState::IDLE || key.state == KeyState::RELEASED)  key.state = KeyState::PRESSED; 
            /*Frame successivi*/
            else                key.state = KeyState::HELD; 
            
            key.ns_pressed += g_timer.getRealDelta(); 
        } 
        else 
        {
            /*Frame di rilascio*/
            if(key.state == KeyState::PRESSED || key.state == KeyState::HELD)   key.state = KeyState::RELEASED;
            /*Frame successivi*/
            else                key.state     = KeyState::IDLE;
            
            key.ns_pressed = 0;
        }
    }

    const keys_state_t& InputHandler::getKeysState()    const   {return m_keys_state;}
    const mouse_state_t& InputHandler::getMouseState()  const   {return m_mouse_state;}
} //namespace Klein
