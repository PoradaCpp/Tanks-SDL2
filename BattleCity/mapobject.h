#ifndef MAPOBJECT_H
#define MAPOBJECT_H

#include <vector>
#include <array>

#include "texture.h"
#include "tankproperties.h"

class MapCollisionInfo
{
public:
    friend class MapObject;

    MapCollisionInfo();

private:
    static const RelativeRect FULL_SIZE;
    static const RelativeRect HALF_LEFT_SIZE;
    static const RelativeRect HALF_RIGHT_SIZE;
    static const RelativeRect HALF_UPPER_SIZE;
    static const RelativeRect HALF_DOWN_SIZE;
    static const RelativeRect QUARTER_DOWN_RIGHT_SIZE;
    static const RelativeRect QUARTER_DOWN_LEFT_SIZE;
    static const RelativeRect EMPTY;

    std::array <const RelativeRect*, 16> m_SizeCoeffArr;
};

class Map;

class MapObject
{
public:
    friend class Map;

    enum class DisplayingLayer
    {
        UPPER_LAYER,
        LOWER_LAYER
    };

    MapObject( Texture texture, Renderer renderer );
    ~MapObject();

    void setSourceRectNumber( size_t nTileNumber );
    size_t getTileNumber() const;
    void setRelativePosition( double dX, double dY );
    void setDestination( SDL_Rect DestRect );
    SDL_Point getPosition() const;
    SDL_Rect getDestination() const;
    SDL_Rect getBase() const;
    RelativeRect getRelativeDestination() const;
    RelativeRect getRelativeBase() const;

    void changeSize();
    void render (SDL_Rect SourceRect );
    void renderLowerLayer( SDL_Rect SourceRect );
    void renderUpperLayer( SDL_Rect SourceRect );
    void renderFillRect();
    bool checkCollision( const SDL_Rect &CheckingRect, bool isShell = false );
    bool checkCollisionWithLine( SDL_Point Point1, SDL_Point Point2 );
    void destroy( SDL_Rect DestroyingRect, CommonTanksProperties::MoveDirection moveDirection );
    void clear();
    DisplayingLayer getDisplayingLayer() const;

    static const size_t TILE_DEFAULT_NUMBER = 99;

private:
    static const RelativeRect DEFAULT_TILE_SIZE;
    static const RelativeRect TILES_BASE_RECT;
    static const size_t TILE_ATOMS_NUMBER = 64;
    static const size_t TILE_ATOMS_IN_DIMENSION = 8;
    static const MapCollisionInfo COLLISION_INFO;

    enum class TileType
    {
        BRICK           = 0,
        GRANITE         = 1,
        FOREST          = 10,
        WATER           = 11,
        ICE             = 12,
        EMPTY           = 99,
        WHITE_SQUARE    = 15,
        SOLID_WALLS_END = 9
    };

    enum class TileCollisionProperties
    {
        FULL_COLLISION,
        TANK_ONLY_COLLISION,
        NO_COLLISION
    };

    Texture m_Texture;
    Renderer m_Renderer;
    std::vector <std::pair <bool, SDL_Rect>> m_TileAtomsVc;
    RelativeRect m_TileRelativeRealPart;
    SDL_Rect m_TileRealPart;
    SDL_Rect m_TileRect;
    size_t m_nTileNumber;
    TileType m_TileType;
    TileCollisionProperties m_TileCollisionProperties;
    DisplayingLayer m_DisplayingLayer;

    void determineTileType();
    void calcTileRealSize();
    void fillTileAtomsArr();
    bool SDL_RectContains( SDL_Rect &MoreLargeRect, SDL_Rect &MoreSmallRect );
    void destroyingRectAlignment( SDL_Rect &DestroyingRect, CommonTanksProperties::MoveDirection moveDirection );
};

typedef std::shared_ptr < MapObject > pSharedMapObject;
typedef std::unique_ptr < MapObject > pUniqueMapObject;

#endif // MAPOBJECT_H
