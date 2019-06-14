#ifndef ANIMATION_H
#define ANIMATION_H

#include <memory>
#include <vector>

#include "surface.h"
#include "renderer.h"
#include "displayedobject.h"
#include "texture.h"

struct AnimationInitData
{
    ImgTextureInitData m_TextureInitData;
    uint16_t m_nFrameRate;
    size_t m_nRowSize;
    size_t m_nColumnSize;

    AnimationInitData( ImgTextureInitData TexureInitData, size_t nRowSize, size_t nColumnSize = 1, uint16_t nFrameRate = 3 ):
        m_TextureInitData( TexureInitData ), m_nFrameRate( nFrameRate ), m_nRowSize( nRowSize ), m_nColumnSize( nColumnSize ) {}
};

class Animation: public DisplayedObject
{
public:
    Animation( AnimationInitData InitData, Renderer renderer );
    ~Animation() override;

    void setBlendMode( SDL_BlendMode BlendMode = SDL_BLENDMODE_BLEND );
    void setAlpha( Uint8 Alpha );
    void changeAlpha( Sint8 dAlpha );
    Uint8 getAlpha();

    SDL_Rect getDestination() const;
    SDL_Point getPosition() const;
    RelativeRect getRelativeBase() const;
    RelativeRect getRelativeDestination() const;
    void setDestination( SDL_Rect DestRect );
    void setPosition( int nX, int nY );
    void setRelativeDestination( RelativeRect RelativeDestRect, RelativeRect RelativeBaseRect = { 0, 0, 0, 0 } );
    void setRelativePosition( double dX, double dY );

    int getPageHeight();
    int getPageWidth();

    void setAnimFrames( size_t nAnimFirstFrame, size_t nAnimLastFrame = 0 );
    void startAnimation();
    bool startAnimation( size_t nAnimFirstFrame, size_t nAnimLastFrame = 0 );
    bool singleAnimation( size_t nAnimFirstFrame = 0, size_t nAnimLastFrame = 0 );
    void stopAnimation();

    void changeSize() override;
    void render() override;
    void render( double dAngle );

private:
    Texture m_BaseTexture;
    std::vector <SDL_Rect> m_SourceRectVc;
    uint32_t m_nFrameRate;
    uint32_t m_nFrame;
    size_t m_nNumOfPictures;
    size_t m_nAnimFirstFrame;
    size_t m_nAnimLastFrame;
    bool m_fAnimation;
    bool m_fAnimPlayed;
    bool m_fSingleAnimation;

    void setRenderDimensions( size_t nRowSize, size_t nColumnSize );
};

typedef std::shared_ptr <Animation> pSharedAnimation;

#endif // ANIMATION_H
