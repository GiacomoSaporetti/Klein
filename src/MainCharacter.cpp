#include "MainCharacter.h"

namespace Klein
{
    MainCharacter g_main_char;
    extern InputHandler g_input_handler;


    void MainCharacter::Jump(const key_info_t& jump_key) {}

    void MainCharacter::Move(const keys_state_t&  keys)
    {
        /*Fermo il personaggio se nessun tasto è premuto*/
        m_speed = {0, 0};

        /*Muovo a destra*/
        if(keys.right.state == KeyState::PRESSED || keys.right.state == KeyState::HELD)
            m_speed += {10, 0};

        /*Muovo a sinistra*/
        if(keys.left.state == KeyState::PRESSED || keys.left.state == KeyState::HELD)
            m_speed += {-10, 0};

        /*Gestisco la logica del salto*/
        if(keys.jump.state == KeyState::PRESSED || keys.jump.state == KeyState::HELD)
            Jump(keys.jump);

    }


    void MainCharacter::interact()
    {

    }


    void MainCharacter::tick()
    {
        const keys_state_t&    keys  = g_input_handler.getKeysState();
        const mouse_state_t&   mouse = g_input_handler.getMouseState();

        Move(keys);

        if(keys.interact.state == KeyState::PRESSED)
            interact();
    }
}