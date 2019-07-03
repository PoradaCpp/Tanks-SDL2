#include <iostream>
#include <fstream>
#include <iterator>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "state.h"

State::State( std::string sHighScoreInfoPath ): m_sHighScoreInfoPath( sHighScoreInfoPath )
{    
    initSdlAndUnits();
    loadHighScore();
}

State::~State()
{
    //Destroy window
    m_Window.free();

    //Quit SDL subsystems
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void State::initSdlAndUnits()
{
    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
    {
        std::cerr<< "SDL could not initialize! SDL Error: " <<  SDL_GetError() << '\n';
    }
    else
    {
        //Initialize PNG loading
        int imgFlags = IMG_INIT_PNG;
        if( !( IMG_Init( imgFlags ) & imgFlags ) )
        {
            std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << '\n';
            SDL_Quit();
            std::exit(-1);
        }

        //Initialize SDL_ttf
        if( TTF_Init() == -1 )
        {
            std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << '\n';

            IMG_Quit();
            SDL_Quit();
            std::exit(-1);
        }

        //Initialize SDL_mixer
        if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
        {
            std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: %s\n" << Mix_GetError() << '\n';

            IMG_Quit();
            TTF_Quit();
            SDL_Quit();
            std::exit(-1);
        }
    }
}

void State::loadHighScore()
{
    std::ifstream dataFile( m_sHighScoreInfoPath );

    if( !dataFile )
    {
        std::cerr << "File opening error! Can't open file " << m_sHighScoreInfoPath << '\n';
        exit(-1);
    }
    std::istream_iterator <uint32_t> dataBegin( dataFile ), dataEnd;

    if( dataBegin != dataEnd )
    {
        m_nHighScore = *dataBegin;
    }
}

void State::attachWindow(MainWindow window )
{
    m_Window = window;
}

void State::mainAction()
{
    //Main loop flag
    bool fQuit = false;

    //Event handler
    SDL_Event event;

    while( !fQuit )
    {
         while( SDL_PollEvent( &event ) != 0 )
         {
             //User requests quit
             if( event.type == SDL_QUIT )
             {
                 fQuit = true;
             }
         }

         if( m_Window )
         {
             m_Window.windowProcessing( this );
         }
    }
}

Renderer State::getRenderer() const
{
    return m_Window.getRenderer();
}

void State::changeState( CurrentState State )
{
    m_PreviousState = m_State;
    m_State = State;
}

CurrentState State::getState() const
{
    return m_State;
}

CurrentState State::getPreviousState() const
{
    return m_PreviousState;
}

NumOfPlayers State::getNumOfPlayers() const
{
    return m_NumOfPlayers;
}

void State::setPlayersQuantity( NumOfPlayers numOfPlayers )
{
    if( !m_fLockPlayersQuantity )
    {
        m_NumOfPlayers = numOfPlayers;
    }
}

void State::lockPlayersQuantity()
{
    m_fLockPlayersQuantity = true;
}

void State::unlockPlayersQuantity()
{
    m_fLockPlayersQuantity = false;
}

bool State::isPlayersQuantityLocked() const
{
    return m_fLockPlayersQuantity;
}

uint32_t State::getHighScore() const
{
    return m_nHighScore;
}

uint32_t State::getCurrentScore() const
{
    return m_nCurrentScore;
}

void State::setHighScore( uint32_t nHighScore )
{
    if( nHighScore > m_nHighScore )
    {
        m_nHighScore = nHighScore;
        std::ofstream writeNewHighScore( m_sHighScoreInfoPath );

        if( !writeNewHighScore )
        {
            std::cerr << "File opening error! Can't open file " << m_sHighScoreInfoPath << '\n';
            exit(-1);
        }
        writeNewHighScore << m_nHighScore;
        writeNewHighScore.close();
    }
}

void State::setCurrentScore( uint32_t nCurrentScore )
{
    m_nCurrentScore = nCurrentScore;
    setHighScore( nCurrentScore );
}
