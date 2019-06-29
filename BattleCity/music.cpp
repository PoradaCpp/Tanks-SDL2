#include <iostream>

#include "music.h"

Music::Music(): m_pMusic( nullptr ) {}

Music::Music( std::string sAudioChunkPath ): m_pMusic( nullptr )
{
    createAudio( sAudioChunkPath );
}

Music::Music( const Music &music ): m_pMusic( music.m_pMusic ) {}

Music::~Music()
{
    free();
}

void Music::createAudio( std::string &sMusicPath )
{
    free();
    pSharedSDLMusic temp_pMusic( Mix_LoadMUS( sMusicPath.c_str() ), [] ( Mix_Music *mus ) { Mix_FreeMusic( mus ); });

    if( temp_pMusic )
    {
        m_pMusic = temp_pMusic;
    }
    else
    {
        std::cerr << "Unable to load audio \'" << sMusicPath << "\'! SDL_mixer Error: " << Mix_GetError() << '\n';
        std::exit(-1);
    }
}

void Music::free()
{
    if( m_pMusic )
    {
        m_pMusic.reset();
    }
}

bool Music::play( int nRepeatTimes )
{
    if( m_pMusic && !Mix_PlayingMusic() )
    {
        return ( 0 == Mix_PlayMusic( m_pMusic.get(), nRepeatTimes ) ? true : false );
    }
    return false;
}

void Music::pauseOrResume()
{
    if( Mix_PausedMusic() )
    {
        Mix_ResumeMusic();
    }
    else
    {
        Mix_PauseMusic();
    }
}

void Music::stop()
{
    Mix_HaltMusic();
}

Music::operator Mix_Music* ()
{
    return m_pMusic.get();
}
