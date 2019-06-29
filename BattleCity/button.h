#ifndef BUTTON_H
#define BUTTON_H

#include <string>
#include <array>
#include <functional>

#include "text3d.h"
#include "displayedobject.h"
#include "buttonbase.h"

struct ButtonInitData
{
    ImgTextureInitData m_ImgInitData;
    Text3DInitData m_Text3DInitData;

    ButtonInitData( ImgTextureInitData ImgInitData, Text3DInitData TextInitData ):
        m_ImgInitData( ImgInitData ), m_Text3DInitData( TextInitData ) {}

    ButtonInitData( ImgTextureInitData ImgInitData ):
        m_ImgInitData( ImgInitData ) {}
};

class Button: public ButtonBase
{
public:
    Button( ButtonInitData InitData, Renderer renderer );
    Button( ImgTextureInitData InitData, Renderer renderer );
    ~Button() override;

    ButtonState getState();
    Uint32 inFocus();

    void setAction( Action action ) override;
    void execute() override;
    void lock() override;
    void unlock() override;

    void changeSize() override;
    void render() override;

protected:
    Texture m_ImageTexture;
    SDL_Rect m_DestImgRect;
    ButtonState m_State;
    std::array < SDL_Rect, 3 > m_SourceRectArr;
    SDL_Point m_CursorPos;
    bool m_fLock;

    void setRenderDimensions();
};

#endif // BUTTON_H
