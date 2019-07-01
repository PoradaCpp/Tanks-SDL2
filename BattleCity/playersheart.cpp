#include <cmath>

#include "playersheart.h"

const RelativeRect PlayersHeart::RELATIVE_SOLID_POS = { 0, 0, 69, 58 };

PlayersHeart::PlayersHeart( AnimationInitData AnimInitData, std::string sAudioChunkPath, Renderer renderer ):
    m_HeartAnim( AnimInitData, renderer ), m_AudioChunk( sAudioChunkPath )
{
    calcNewSize();
    m_HeartAnim.startAnimation( NOLMAL_HEART_STATE_ANIM_BEG, NOLMAL_HEART_STATE_ANIM_END );
}

PlayersHeart::PlayersHeart( PlayersHeartInitData InitData, Renderer renderer ): m_HeartAnim( InitData.m_AnimInitData, renderer ),
    m_AudioChunk( InitData.m_sAudioChunkPath )
{
    calcNewSize();
    m_HeartAnim.startAnimation( NOLMAL_HEART_STATE_ANIM_BEG, NOLMAL_HEART_STATE_ANIM_END );
}

PlayersHeart::~PlayersHeart() {}

void PlayersHeart::calcNewSize()
{
    m_AnimCurPos = m_HeartAnim.getDestination();

    m_RealSizeRect = { 0, 0, lround( m_AnimCurPos.w * RELATIVE_SOLID_POS.m_dWidth  / 100 ),
                             lround( m_AnimCurPos.h * RELATIVE_SOLID_POS.m_dHeight / 100 )};

    m_RealSizeRect.x = m_AnimCurPos.x + ( m_AnimCurPos.w - m_RealSizeRect.w ) / 2;
    m_RealSizeRect.y = m_AnimCurPos.y + ( m_AnimCurPos.h - m_RealSizeRect.h ) / 2;
}

void PlayersHeart::render()
{
    m_HeartAnim.render();

    if( m_fDestroying && m_HeartAnim.singleAnimation( HEART_DESTROYING_STATE_ANIM_BEG, HEART_DESTROYING_STATE_ANIM_END ) )
    {
        m_HeartAnim.setAnimFrames( HEART_DESTROYING_STATE_ANIM_END );
        m_fDestroyed = true;
    }
}

void PlayersHeart::resize()
{
    m_HeartAnim.changeSize();
    calcNewSize();
}

void PlayersHeart::init()
{
    m_fDestroying = false;
    m_fDestroyed  = false;
    calcNewSize();
    m_HeartAnim.startAnimation( NOLMAL_HEART_STATE_ANIM_BEG, NOLMAL_HEART_STATE_ANIM_END );
}

void PlayersHeart::destroy()
{
    m_HeartAnim.stopAnimation();
    m_fDestroying = true;
    m_AudioChunk.play();
}

bool PlayersHeart::isDestroying() const
{
    return m_fDestroying;
}

bool PlayersHeart::isDestroyed() const
{
    return m_fDestroyed;
}

SDL_Rect PlayersHeart::getCollisionRect() const
{
    return m_RealSizeRect;
}
