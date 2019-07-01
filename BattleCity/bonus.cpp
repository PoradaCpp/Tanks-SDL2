#include "bonus.h"

Bonus::Bonus( Texture texture, AudioChunk bonusAdd, AudioChunk bonusTaken, BonusType bonusType ): m_ImageTexture( texture ),
    m_BonusAddAudio( bonusAdd ), m_BonusTakenAudio( bonusTaken ), m_BonusType( bonusType )
{
    m_DestImgRect = m_ImageTexture.getDestination();
    setRenderDimensions();
    m_BonusAddAudio.play();
}

Bonus::~Bonus() {}

void Bonus::setRenderDimensions()
{
    int nWidth = m_ImageTexture.getWidth();
    int nHeight = m_ImageTexture.getHeight();
    int nSrcRectHeight = nHeight / static_cast <int> ( COLUMN_SIZE );
    int nSrcRectWidth = nWidth / static_cast <int> ( ROW_SIZE );
    int width( 0 ) , height( 0 );

    for( size_t i = 0 ; i < BONUS_QUANTITY; ++i, ++width )
    {
        if( width >= static_cast <int> ( ROW_SIZE ) )
        {
            width = 0;
            ++height;
        }
        m_SourceRectArr.at( i ) = { nSrcRectWidth * width, nSrcRectHeight * height, nSrcRectWidth, nSrcRectHeight };
    }
}

Bonus::BonusType Bonus::getBonusType() const
{
    return m_BonusType;
}

SDL_Rect Bonus::getDestination() const
{
    return m_ImageTexture.getDestination();
}

void Bonus::setPosition( int nX, int nY )
{
    m_ImageTexture.setPosition( nX, nY );
    m_DestImgRect = m_ImageTexture.getDestination();
}

void Bonus::playTakingSound()
{
    m_BonusTakenAudio.play();
}

void Bonus::changeSize()
{
    m_ImageTexture.changeSize();
    m_DestImgRect = m_ImageTexture.getDestination();
}

void Bonus::render()
{
    m_ImageTexture.render( &m_SourceRectArr.at( static_cast <size_t> ( m_BonusType ) ));
}
