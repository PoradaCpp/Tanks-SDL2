#ifndef AUDIO_CHUNK_H
#define AUDIO_CHUNK_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <memory>

typedef std::shared_ptr <Mix_Chunk> pSharedSDLAudioChunk;

class AudioChunk
{
public:
    AudioChunk();
    AudioChunk( std::string sAudioChunkPath );
    AudioChunk( const AudioChunk &audioChunk );
    ~AudioChunk();

    static const int INFINITELY_PLAYING = -1;

    void createAudio( std::string &sAudioChunkPath );
    void free();
    void play( int nRepeatTimes = 0 );
    void pauseOrResume();
    void stop();

private:
    pSharedSDLAudioChunk m_pAudioChunk;
    int m_nChannelNumber;

    operator Mix_Chunk* ();
};

#endif // AUDIO_CHUNK_H
