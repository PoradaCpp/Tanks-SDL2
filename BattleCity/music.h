#ifndef MUSIC_H
#define MUSIC_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <memory>

typedef std::shared_ptr <Mix_Music> pSharedSDLMusic;

class Music
{
public:
    Music();
    Music( std::string sMusicPath );
    Music( const Music &music );
    ~Music();

    static const int INFINITELY_PLAYING = -1;

    void createAudio( std::string &sMusicPath );
    void free();
    bool play( int nRepeatTimes = 1 );
    void pauseOrResume();
    void stop();

private:
    pSharedSDLMusic m_pMusic;

    operator Mix_Music* ();
};

#endif // MUSIC_H
