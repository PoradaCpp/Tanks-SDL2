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
    State();
    ~State();

    void attachWindow( MainWindow window );
    void mainAction();
    Renderer getRenderer();
    void changeState( CurrentState State );
    CurrentState getState();
    NumOfPlayers getNumOfPlayers();
    void setPlayersQuantity( NumOfPlayers numOfPlayers );
    void lockPlayersQuantity();
    void unlockPlayersQuantity();
    bool isPlayersQuantityLocked();

private:
    CurrentState m_State;
    MainWindow m_Window;
    NumOfPlayers m_NumOfPlayers;
    bool m_fLockPlayersQuantity;
};

#endif // MAINSTATE_H
