#ifndef MAPOBJECT_H
#define MAPOBJECT_H

#include <array>

#include "texture.h"

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
    SDL_Rect getDestination() const;

    void changeSize();
    void renderTexture( SDL_Rect SourceRect );
    void renderFillRect();

    static const size_t TILE_DEFAULT_NUMBER = 99;

private:
    static const RelativeRect DEFAULT_TILE_SIZE;
    static const RelativeRect TILES_BASE_RECT;

    static constexpr SDL_Color BLACK = { 0,  0,  0,  255 };

    Texture m_Texture;
    Renderer m_Renderer;
    RelativeRect m_TileRelativeRect;
    SDL_Rect m_TileRect;
    size_t m_nTileNumber;

};

typedef std::shared_ptr < MapObject > pSharedMapObject;
typedef std::unique_ptr < MapObject > pUniqueMapObject;

#endif // MAPOBJECT_H
