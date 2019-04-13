#ifndef BUTTON_H
#define BUTTON_H

#include <string>
#include <array>

#include "text3d.h"
#include "displayedobject.h"

struct ButtonInitData
{
    ImgTextureInitData m_ImgInitData;
    Text3DInitData m_Text3DInitData;

    ButtonInitData( ImgTextureInitData ImgInitData, Text3DInitData TextInitData ):
        m_ImgInitData( ImgInitData ), m_Text3DInitData( TextInitData ) {}
};

struct ButtonDestinationData
{
    RelativeRect m_RelativeDestRect;
    RelativeRect m_RelativeBaseRect;

    ButtonDestinationData( RelativeRect DestRect, RelativeRect BaseRect = { 0, 0, 0, 0} ):
        m_RelativeDestRect( DestRect ), m_RelativeBaseRect( BaseRect ) {}
};

enum class ButtonState
{
    DEFAULT  = 0,
    IN_FOCUS = 1,
    PRESSED  = 2,
    RELEASED = 3
};

class Button: public DisplayedObject
{
public:
    Button( ButtonInitData InitData, Renderer renderer );
    Button( Texture ImageTexture, Text3D ButtonName, RelativeRect DestRect, RelativeRect BaseRect = { 0, 0, 0, 0} );
    Button( Texture ImageTexture, Text3D ButtonName, ButtonDestinationData InitData );
    ~Button();

    ButtonState getState();
    Uint32 inFocus(int &nCursorX, int &nCursorY);
    void changeSize() override;
    void render() override;

private:
    RelativeRect m_RelativeDestRect;
    SDL_Rect m_DestImgRect;
    RelativeRect m_RelativeDestTxtRect;
    Texture m_ImageTexture;
    Text3D m_NameText3D;
    ButtonState m_State;
    std::array < SDL_Rect, 3 > m_RectArr;

    void setRenderDimensions();
};

typedef std::shared_ptr < Button > pSharedButton;

#endif // BUTTON_H
