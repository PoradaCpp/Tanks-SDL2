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

    Text3DInitData( Font font, std::string sText, SDL_Color TopTextColor = { 40, 200, 40, 40 },
                    SDL_Color BottomTextColor = { 200, 40, 40, 40 }, Uint8 nTopTextureAlpha = 255, Uint8 nBottomTextureAlpha = 255,
                    int nTopTextureShiftX = 1, int nTopTextureShiftY = 1, RelativeRect DestRect = { 0, 0, 0, 0 },
                    RelativeRect BaseRect = { 0, 0, 0, 0 } ):
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
    ~Text3D() override;

    void setTopTextureAlpha( Uint8 Alpha );
    void setBottomTextureAlpha( Uint8 Alpha );
    void setTopTextureShift( int nDx, int nDy );

    void setRelativeDestination( RelativeRect RelativeDestRect, RelativeRect RelativeBaseRect = { 0, 0, 0, 0 } );
    void setDestination( SDL_Rect DestRect );
    void setPosition(int nX, int nY );

    void changeSize() override;
    void render() override;

private:
    Texture m_TopTexture;
    Texture m_BottomTexture;

    int m_nTopTextureShiftX;
    int m_nTopTextureShiftY;
};

typedef std::shared_ptr <Text3D> pSharedText3D;

#endif // TEXT3D
