#ifndef MAINSTATE_H
#define MAINSTATE_H

#include "mainwindow.h"


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

private:
    CurrentState m_State;
    MainWindow m_Window;
};

#endif // MAINSTATE_H
