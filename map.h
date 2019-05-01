#ifndef MAP_H
#define MAP_H

#include <vector>

#include "mapobject.h"

struct MapInitData
{
    ImgTextureInitData m_ImgTextureInitData;
    std::string m_sPresentMapsPath;

    MapInitData( ImgTextureInitData imgTextureInitData, std::string PresentMapsPath ):
        m_ImgTextureInitData( imgTextureInitData ), m_sPresentMapsPath( PresentMapsPath ) {}
};

class CreateMapPage;
class MapSquareButton;

class Map
{
public:
    friend class CreateMapPage;
    friend class MapSquareButton;

    Map( MapInitData InitData, Renderer renderer );
    ~Map();

    void changeSize();
    void render();
    Texture getTilesTexture();
    void saveCurrentMap();
    void loadDefaultMap();
    void loadMap();
    SDL_Rect getMapBordersRect() const;

    static const size_t MAP_SIZE    = 504;
    static const size_t COLUMN_SIZE = 28;
    static const size_t ROW_SIZE    = 18;

    static constexpr double TILE_WIDTH   = 2.9;
    static constexpr double TILE_HEIGHT  = 5.2;
    static constexpr double TILE_X_BEGIN = 1.2;
    static constexpr double TILE_Y_BEGIN = 2;

    static constexpr SDL_Color BLACK = { 0,  0,  0,  255 };
    static constexpr SDL_Color BROWN = { 50, 40, 40, 255 };
    static constexpr SDL_Color GREY  = { 80, 80, 80, 255 };

    static const size_t VECTOR_TILE_SIZE   = 20;
    static const size_t VECTOR_TILE_WIDTH  = 2;
    static const size_t VECTOR_TILE_HEIGHT = 10;

private:
    Texture m_TilesTexture;
    Renderer m_Renderer;
    std::vector <pSharedMapObject> m_MapVc;
    std::vector <std::string> m_MapsPathsVc;
    std::vector <SDL_Rect> m_SourceRectVc;
    SDL_Rect m_TileBeginRect;
    SDL_Rect m_MapBordersRect;
    std::string m_sPresentMapsPath;
    size_t m_nCurrentMapIndex;

    void setRenderDimensions();
    void readMapFromFile( std::string sPath );
};

typedef std::shared_ptr <Map> pSharedMap;

#endif // MAP_H
