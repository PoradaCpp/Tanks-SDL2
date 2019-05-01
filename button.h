#ifndef BUTTON_H
#define BUTTON_H

#include <string>
#include <array>
#include <functional>

#include "text3d.h"
#include "displayedobject.h"

struct ButtonInitData
{
    ImgTextureInitData m_ImgInitData;
    Text3DInitData m_Text3DInitData;

    ButtonInitData( ImgTextureInitData ImgInitData, Text3DInitData TextInitData ):
        m_ImgInitData( ImgInitData ), m_Text3DInitData( TextInitData ) {}

    ButtonInitData( ImgTextureInitData ImgInitData ):
        m_ImgInitData( ImgInitData ) {}
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
    typedef std::function <void()> Action;

    Button( ButtonInitData InitData, Renderer renderer );
    Button( ImgTextureInitData InitData, Renderer renderer );
    ~Button();

    virtual Uint32 inFocus();
    virtual ButtonState getState();

    virtual void setAction( Action action );
    virtual void execute();

    void changeSize() override;
    void render() override;

protected:
    Texture m_ImageTexture;
    SDL_Rect m_DestImgRect;
    ButtonState m_State;
    std::array < SDL_Rect, 3 > m_SourceRectArr;
    SDL_Point m_CursorPos;

    Action m_Action;

    void setRenderDimensions();
};

typedef std::shared_ptr <Button> pSharedButton;

#endif // BUTTON_H
