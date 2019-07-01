#include <algorithm>

#include "scorescounting.h"

ScoresCounting::ScoresCounting( ImgTextureInitData BackgroundTextureInitData, std::vector <Text3DInitData> Text3DVcInitData,
                                Renderer renderer): m_BackgroundTexture( BackgroundTextureInitData, renderer )

{
    std::for_each( Text3DVcInitData.begin(), Text3DVcInitData.end(), [this, renderer] ( Text3DInitData &InitData )
    {
        m_Text3DVc.push_back( std::make_shared <Text3D> ( InitData, renderer ) );
    });
}

ScoresCounting::~ScoresCounting() {}

void ScoresCounting::render()
{
    m_BackgroundTexture.render();

    std::for_each( m_Text3DVc.begin(), m_Text3DVc.end(), [] ( pSharedText3D &pText3D )
    {
        if( pText3D )
        {
            pText3D->render();
        }
    });
}

void ScoresCounting::changeSize()
{
    m_BackgroundTexture.changeSize();

    std::for_each( m_Text3DVc.begin(), m_Text3DVc.end(), [] ( pSharedText3D &pText3D )
    {
        if( pText3D )
        {
            pText3D->changeSize();
        }
    });
}
