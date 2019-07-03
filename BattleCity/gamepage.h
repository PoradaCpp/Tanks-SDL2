#ifndef GAMEPAGE_H
#define GAMEPAGE_H

#include "page.h"
#include "map.h"
#include "gameengine.h"

enum class CurrentState;

class GamePage: public Page
{
public:
    GamePage( InitContainers &&objContainers, pSharedGameEngine &&pGameEngine, State *pState, Renderer renderer );
    ~GamePage() override;

    void initButtons( State *pState ) override;
    void resize() override;
    void render() override;

    void renderMapBorder();
    void changeState( CurrentState State );
    void playStartSound();
    NumOfPlayers getNumOfPlayers() const;
    uint32_t getHighScore() const;
    void setCurrentScore( uint32_t nCurrentScore );

private:
    enum class Buttons
    {
        SET_PAUSE          = 0,
        SET_PREVIOUS_STATE = 1,
        BUTTONS_QUANTITY   = 2
    };

    class PauseKey
    {
    public:
        bool keyClick()
        {
            const Uint8* currentKeyStates = SDL_GetKeyboardState( nullptr );

            if( currentKeyStates[ Key_P ] )
            {
                m_fPressed = true;
            }
            else if( m_fPressed )
            {
                m_fPressed = false;
                return true;
            }
            return false;
        }
    private:
        SDL_Scancode Key_P { SDL_SCANCODE_P };
        bool m_fPressed { false };
    };

    State *m_pState;
    pSharedGameEngine m_pGameEngine;
    Renderer m_Renderer;
    CurrentState m_CurrentState;
    CurrentState m_PreviousState;
    SDL_Rect m_MapBordersRect;
    PauseKey m_PauseKey;
};

#endif // GAMEPAGE_H
