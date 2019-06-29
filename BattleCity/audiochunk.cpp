#include <iostream>

#include "audiochunk.h"

AudioChunk::AudioChunk(): m_pAudioChunk( nullptr ) {}

AudioChunk::AudioChunk( std::string sAudioChunkPath ): m_pAudioChunk( nullptr ), m_nChannelNumber( -1 )
{
    createAudio( sAudioChunkPath );
}

AudioChunk::AudioChunk( const AudioChunk &audioChunk ): m_pAudioChunk( audioChunk.m_pAudioChunk ), m_nChannelNumber( -1 ) {}

AudioChunk::~AudioChunk()
{
    free();
}

void AudioChunk::createAudio( std::string &sAudioChunkPath )
{
    free();
    pSharedSDLAudioChunk temp_pAudioChunk( Mix_LoadWAV( sAudioChunkPath.c_str() ), [] ( Mix_Chunk *chunk ) { Mix_FreeChunk( chunk ); });

    if( temp_pAudioChunk )
    {
        m_pAudioChunk = temp_pAudioChunk;
    }
    else
    {
        std::cerr << "Unable to load audio \'" << sAudioChunkPath << "\'! SDL_mixer Error: " << Mix_GetError() << '\n';
        std::exit(-1);
    }
}

void AudioChunk::free()
{
    if( m_pAudioChunk )
    {
        m_pAudioChunk.reset();
    }
}

void AudioChunk::play( int nRepeatTimes )
{
    m_nChannelNumber = -1;

    if( m_pAudioChunk )
    {
        m_nChannelNumber = Mix_PlayChannel( -1, m_pAudioChunk.get(), nRepeatTimes );
    }
}

void AudioChunk::pauseOrResume()
{
    if( Mix_Paused( m_nChannelNumber ) )
    {
        Mix_Resume( m_nChannelNumber );
    }
    else
    {
        Mix_Pause( m_nChannelNumber );
    }
}

void AudioChunk::stop()
{
    Mix_HaltChannel( m_nChannelNumber );
}

AudioChunk::operator Mix_Chunk* ()
{
    return m_pAudioChunk.get();
}
