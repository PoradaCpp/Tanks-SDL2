#ifndef MAPOBJECT_H
#define MAPOBJECT_H

#include <vector>
#include <array>

#include "texture.h"

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
    static const RelativeRect QUARTER_UPPER_RIGHT_SIZE;
    static const RelativeRect QUARTER_UPPER_LEFT_SIZE;
    static const RelativeRect EMPTY;

    std::array <const RelativeRect*, 20> m_SizeCoeffArr;
};

class Map;

class MapObject
{
public:
    friend class Map;

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
    void renderTexture( SDL_Rect SourceRect );
    void renderFillRect();
    bool checkCollision( const SDL_Rect &CheckingRect );
    void destroy( const SDL_Rect &DestroyingRect );

    static const size_t TILE_DEFAULT_NUMBER = 99;

private:
    static const RelativeRect DEFAULT_TILE_SIZE;
    static const RelativeRect TILES_BASE_RECT;
    static const size_t TILE_ATOMS_NUMBER = 64;
    static const size_t TILE_ATOMS_IN_DIMENSION = 8;
    static const MapCollisionInfo COLLISION_INFO;

    Texture m_Texture;
    Renderer m_Renderer;
    std::vector <std::pair <bool, SDL_Rect>> m_TileAtomsVc;
    RelativeRect m_TileRelativeRealPart;
    SDL_Rect m_TileRealPart;
    SDL_Rect m_TileRect;
    size_t m_nTileNumber;

    void calcTileRealSize();
    void fillTileAtomsArr();
    bool SDL_RectContains( SDL_Rect &MoreLargeRect, SDL_Rect &MoreSmallRect );
};

typedef std::shared_ptr < MapObject > pSharedMapObject;
typedef std::unique_ptr < MapObject > pUniqueMapObject;

#endif // MAPOBJECT_H