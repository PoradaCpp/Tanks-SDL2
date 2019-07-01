#ifndef SCORESCOUNTING_H
#define SCORESCOUNTING_H

#include <vector>

#include "text3d.h"

class ScoresCounting
{
public:
    ScoresCounting( ImgTextureInitData BackgroundTextureInitData, std::vector <Text3DInitData> Text3DVcInitData, Renderer renderer );
    ~ScoresCounting();

    void render();
    void changeSize();

private:
    Texture m_BackgroundTexture;
    std::vector <pSharedText3D> m_Text3DVc;
};

#endif // SCORESCOUNTING_H
