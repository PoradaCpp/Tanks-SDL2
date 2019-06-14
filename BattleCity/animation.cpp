
#include <algorithm>
#include <iostream>

#include "animation.h"


Animation::Animation(  AnimationInitData InitData, Renderer renderer  ): DisplayedObject(),
    m_BaseTexture( InitData.m_TextureInitData, renderer ), m_nFrameRate( InitData.m_nFrameRate ), m_nFrame( 0 ),
    m_nNumOfPictures( InitData.m_nRowSize * InitData.m_nColumnSize ), m_nAnimFirstFrame( 0 ),
    m_nAnimLastFrame( InitData.m_nRowSize * InitData.m_nColumnSize ), m_fAnimation( false ), m_fAnimPlayed( false ),
    m_fSingleAnimation( false )
{
    changeSize();
    setRenderDimensions( InitData.m_nRowSize, InitData.m_nColumnSize );
}

Animation::~Animation() {}

void Animation::setRenderDimensions( size_t nRowSize, size_t nColumnSize )
{
    int nWidth = m_BaseTexture.getWidth();
    int nHeight = m_BaseTexture.getHeight();
    int nSrcRectHeight = nHeight / static_cast <int> ( nColumnSize );
    int nSrcRectWidth = nWidth / static_cast <int> ( nRowSize );
    int width( 0 ) , height( 0 );

    m_SourceRectVc.reserve( m_nNumOfPictures );
    for( size_t i = 0; i < m_nNumOfPictures; ++i , ++width )
    {
        if( width >= static_cast <int> ( nRowSize ) )
        {
            width = 0;
            ++height;
        }
        m_SourceRectVc.push_back({ nSrcRectWidth *width, nSrcRectHeight * height, nSrcRectWidth, nSrcRectHeight });
    }
}

void Animation::setBlendMode( SDL_BlendMode BlendMode )
{
    m_BaseTexture.setBlendMode( BlendMode );
}

void Animation::setAlpha( Uint8 Alpha )
{
    m_BaseTexture.setAlpha( Alpha );
}

void Animation::changeAlpha( Sint8 dAlpha )
{
    m_BaseTexture.changeAlpha( dAlpha );
}

Uint8 Animation::getAlpha()
{
    return m_BaseTexture.getAlpha();
}

SDL_Rect Animation::getDestination() const
{
    return m_BaseTexture.getDestination();
}

SDL_Point Animation::getPosition() const
{
    return m_BaseTexture.getPosition();
}

RelativeRect Animation::getRelativeBase() const
{
    return m_BaseTexture.getRelativeBase();
}

RelativeRect Animation::getRelativeDestination() const
{
    return m_BaseTexture.getRelativeDestination();
}

void Animation::setDestination( SDL_Rect DestRect )
{
    m_BaseTexture.setDestination( DestRect );
}

void Animation::setPosition( int nX, int nY )
{
    m_BaseTexture.setPosition( nX, nY );
}

void Animation::setRelativeDestination( RelativeRect RelativeDestRect, RelativeRect RelativeBaseRect )
{
    m_BaseTexture.setRelativeDestination( RelativeDestRect, RelativeBaseRect );
}

void Animation::Animation::setRelativePosition( double dX, double dY )
{
    m_BaseTexture.setRelativePosition( dX, dY );
}

int Animation::getPageHeight()
{
    return m_BaseTexture.getPageHeight();
}

int Animation::getPageWidth()
{
    return m_BaseTexture.getPageWidth();
}

void Animation::setAnimFrames( size_t nAnimFirstFrame, size_t nAnimLastFrame )
{
    if( m_nAnimFirstFrame != nAnimFirstFrame || m_nAnimLastFrame != nAnimLastFrame )
    {
        m_nAnimFirstFrame = nAnimFirstFrame;

        if( nAnimLastFrame )
        {
            m_nAnimLastFrame = nAnimLastFrame;
        }
        m_nFrame = m_nAnimFirstFrame * m_nFrameRate;
    }
}

void Animation::startAnimation()
{
    m_fAnimation = true;
}

bool Animation::startAnimation( size_t nAnimFirstFrame, size_t nAnimLastFrame )
{
    if( m_nAnimFirstFrame != nAnimFirstFrame || m_nAnimLastFrame != nAnimLastFrame )
    {
        m_nAnimFirstFrame = nAnimFirstFrame;
        m_nAnimLastFrame = nAnimLastFrame ? nAnimLastFrame : m_SourceRectVc.size() - 1;
        m_nFrame = m_nAnimFirstFrame * m_nFrameRate;
    }
    m_fAnimation = true;
    return m_fAnimPlayed;
}

bool Animation::singleAnimation( size_t nAnimFirstFrame, size_t nAnimLastFrame )
{
    if( !m_fSingleAnimation && !m_fAnimPlayed )
    {
        if( m_nAnimFirstFrame != nAnimFirstFrame || m_nAnimLastFrame != nAnimLastFrame )
        {
            m_nAnimFirstFrame = nAnimFirstFrame;
            m_nAnimLastFrame = nAnimLastFrame ? nAnimLastFrame : m_SourceRectVc.size() - 1;
            m_nFrame = m_nAnimFirstFrame * m_nFrameRate;
        }
        m_fSingleAnimation = true;
    }

    return m_fAnimPlayed;
}

void Animation::stopAnimation()
{
    m_fAnimation = false;
}

void Animation::changeSize()
{
    m_BaseTexture.changeSize();
}

void Animation::render()
{
    if( m_fAnimation || m_fSingleAnimation )
    {
        m_BaseTexture.render( &m_SourceRectVc [m_nFrame / m_nFrameRate] );

        ++m_nFrame;
        if( m_nFrame / m_nFrameRate >= m_nAnimLastFrame )
        {
            m_nFrame = m_nAnimFirstFrame * m_nFrameRate;
            m_fAnimPlayed = true;
            m_fSingleAnimation = false;
        }
        else
        {
            m_fAnimPlayed = false;
        }
    }
    else
    {
        m_BaseTexture.render( &m_SourceRectVc.at( m_nAnimFirstFrame ) );
    }
}

void Animation::render( double dAngle )
{
    if( m_fAnimation || m_fSingleAnimation )
    {
        m_BaseTexture.render( &m_SourceRectVc [m_nFrame / m_nFrameRate], nullptr, dAngle );
        ++m_nFrame;
        if( m_nFrame / m_nFrameRate >= m_nAnimLastFrame + 1 )
        {
            m_nFrame = m_nAnimFirstFrame * m_nFrameRate;
            m_fAnimPlayed = true;
            m_fSingleAnimation = false;
        }
        else
        {
            m_fAnimPlayed = false;
        }
    }
    else
    {
        m_BaseTexture.render( &m_SourceRectVc.at( m_nAnimFirstFrame ), nullptr, dAngle );
    }
}
