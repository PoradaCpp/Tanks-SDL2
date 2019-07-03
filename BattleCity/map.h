#ifndef MAP_H
#define MAP_H

#include <vector>

#include "mapobject.h"
#include "displayedobject.h"
#include "tankproperties.h"

struct MapInitData
{
    ImgTextureInitData m_ImgTextureInitData;
    std::string m_sPresentMapsPath;

    MapInitData( ImgTextureInitData imgTextureInitData, std::string PresentMapsPath ):
        m_ImgTextureInitData( imgTextureInitData ), m_sPresentMapsPath( PresentMapsPath ) {}
};

class CreateMapPage;
class MapSquareButton;

class Map: public DisplayedObject
{
public:
    friend class CreateMapPage;
    friend class MapSquareButton;
    friend class Tank;
    friend class TankOfPlayer;

    Map( MapInitData InitData, Renderer renderer );
    ~Map() override;

    void changeSize() override;
    void render() override;

    void renderLowerLayer();
    void renderUpperLayer();

    Texture getTilesTexture();
    void saveCurrentMap();
    void loadDefaultMap();
    void loadMap();
    SDL_Rect getMapBordersRect() const;
    RelativeRect getRelativeBase() const;
    int getPageHeight() const;
    int getPageWidth() const;
    bool checkCollision( const SDL_Rect &CheckingRect, bool isShell = false );
    bool checkBorderCollision( const SDL_Rect &CheckingRect );
    void destroy( const SDL_Rect &CheckingRect, const SDL_Rect &DestroyingRect, CommonTanksProperties::MoveDirection moveDirection );
    void clear();
    void resetCurrentMapIndex();
    bool isMapsStillPresent() const;
    bool isMapIndexDefault() const;
    uint32_t getStageNumber() const;
    void setBaseGraniteDefense();
    void resetBaseGraniteDefense();

    static const size_t MAP_SIZE    = 504;
    static const size_t ROW_SIZE    = 28;
    static const size_t COLUMN_SIZE = 18;

    static const size_t BASE_TOP_POS1   = 433;
    static const size_t BASE_TOP_POS2   = 434;
    static const size_t BASE_LEFT_POS1  = 460;
    static const size_t BASE_LEFT_POS2  = 488;
    static const size_t BASE_RIGHT_POS1 = 463;
    static const size_t BASE_RIGHT_POS2 = 491;

    static const size_t GRANITE_TOP_TILE   = 9;
    static const size_t GRANITE_LEFT_TILE  = 7;
    static const size_t GRANITE_RIGHT_TILE = 3;
    static const size_t BRICK_TOP_TILE     = 8;
    static const size_t BRICK_LEFT_TILE    = 6;
    static const size_t BRICK_RIGHT_TILE   = 2;

    static constexpr double TILE_WIDTH   = 2.9;
    static constexpr double TILE_HEIGHT  = 5.2;
    static constexpr double TILE_X_BEGIN = 1.3;
    static constexpr double TILE_Y_BEGIN = 2.3;

    static constexpr SDL_Color BLACK = { 0,  0,  0,  255 };
    static constexpr SDL_Color BROWN = { 50, 40, 40, 255 };
    static constexpr SDL_Color GREY  = { 80, 80, 80, 255 };

    static const size_t VECTOR_TILE_WIDTH  = 2;
    static const size_t VECTOR_TILE_HEIGHT = 8;
    static const size_t VECTOR_TILE_SIZE   = VECTOR_TILE_WIDTH * VECTOR_TILE_HEIGHT;

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
    size_t tileNumberForPoint( int nX, int nY );
};

typedef std::shared_ptr <Map> pSharedMap;

#endif // MAP_H
