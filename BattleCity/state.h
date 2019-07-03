#ifndef MAINSTATE_H
#define MAINSTATE_H

#include "mainwindow.h"
#include "tankarrow.h"

enum class CurrentState
{
    START_PAGE  = 0,
    CREATE_MAP = 1,
    GAME  = 2,
};

class State
{
public:
    State( std::string sHighScoreInfoPath );
    ~State();

    void attachWindow( MainWindow window );
    void mainAction();
    Renderer getRenderer() const;
    void changeState( CurrentState State );
    CurrentState getState() const;
    CurrentState getPreviousState() const;
    NumOfPlayers getNumOfPlayers() const;
    void setPlayersQuantity( NumOfPlayers numOfPlayers );
    void lockPlayersQuantity();
    void unlockPlayersQuantity();
    bool isPlayersQuantityLocked() const;
    uint32_t getHighScore() const;
    uint32_t getCurrentScore() const;
    void setHighScore( uint32_t nHighScore );
    void setCurrentScore( uint32_t nHighScore );

private:
    CurrentState m_State                = CurrentState::START_PAGE;
    CurrentState m_PreviousState;
    MainWindow   m_Window;
    NumOfPlayers m_NumOfPlayers         = NumOfPlayers::TWO_PLAYERS;
    bool         m_fLockPlayersQuantity = false;
    std::string  m_sHighScoreInfoPath;
    uint32_t     m_nHighScore           = 0;
    uint32_t     m_nCurrentScore        = 0;

    void initSdlAndUnits();
    void loadHighScore();
};

#endif // MAINSTATE_H
