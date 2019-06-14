#ifndef BUTTON_NAMED_H
#define BUTTON_NAMED_H

#include "button.h"

class ButtonNamed: public Button
{
public:
    ButtonNamed( ButtonInitData InitData, Renderer renderer );
    ~ButtonNamed() override;

    void changeSize() override;
    void render() override;

protected:
    RelativeRect m_RelativeDestRect;
    RelativeRect m_RelativeDestTxtRect;
    Text3D m_NameText3D;

    void setRenderDimensions();
};

#endif // BUTTON_NAMED_H
