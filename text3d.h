#ifndef TEXT3D
#define TEXT3D

#include "texture.h"

struct Text3DInitData
{
    TextTextureInitData TopTextInitData;
    TextTextureInitData BottomTextInitData;

    Uint8 m_nTopTextureAlpha;
    Uint8 m_nBottomTextureAlpha;
    int m_nTopTextureShiftX;
    int m_nTopTextureShiftY;

    Text3DInitData( Font font, std::string sText, SDL_Color TopTextColor, SDL_Color BottomTextColor,
                    Uint8 nTopTextureAlpha, Uint8 nBottomTextureAlpha, int nTopTextureShiftX = 1, int nTopTextureShiftY = 1,
                    RelativeRect DestRect = { 0, 0, 0, 0 }, RelativeRect BaseRect = { 0, 0, 0, 0 } ):
        TopTextInitData   ( font, sText, TopTextColor,    DestRect, BaseRect ),
        BottomTextInitData( font, sText, BottomTextColor, DestRect, BaseRect ),
        m_nTopTextureAlpha( nTopTextureAlpha ), m_nBottomTextureAlpha( nBottomTextureAlpha ),
        m_nTopTextureShiftX( nTopTextureShiftX ), m_nTopTextureShiftY( nTopTextureShiftY ) {}

    Text3DInitData() {}
};


class Text3D: public DisplayedObject
{
public:
    Text3D( Text3DInitData text3DInitData, Renderer renderer );

    ~Text3D();

    void setTopTextureAlpha( Uint8 Alpha );
    void setBottomTextureAlpha( Uint8 Alpha );
    void setTopTextureShift( int dX, int dY );

    void setRelativeDestination( RelativeRect RelativeDestRect, RelativeRect RelativeBaseRect = { 0, 0, 0, 0 } );

    void changeSize() override;
    void render() override;

private:
    Texture m_TopTexture;
    Texture m_BottomTexture;

    int m_nTopTextureShiftX;
    int m_nTopTextureShiftY;
};

#endif // TEXT3D
