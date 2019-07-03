#ifndef TEXTTEXTURE_H
#define TEXTTEXTURE_H

#include "texture.h"

class TextTexture: public Texture
{
public:
    enum class TextAlignment
    {
        LEFT_SIDE,
        RIGHT_SIDE,
        CENTER
    };

    TextTexture( TextTextureInitData TextInitData, Renderer renderer );
    ~TextTexture() override;

    void setText( std::string sText );
    void setAlignment( TextAlignment textAlignment );
    size_t getTextSize() const;

private:
    Font m_Font;
    std::string m_sText;
    SDL_Color m_TextColor;
    TextAlignment m_TextAlignment = TextAlignment::LEFT_SIDE;
};

#endif // TEXTTEXTURE_H
