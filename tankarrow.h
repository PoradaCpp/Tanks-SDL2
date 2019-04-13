#ifndef TANK_ARROW_H
#define TANK_ARROW_H

#include <array>

#include "dynamicobject.h"


struct TankArrowInitData
{
    ImgTextureInitData m_TextureInitData;
    RelativeRect m_AnotherPos;
    SDL_Scancode m_Key1;
    SDL_Scancode m_Key2;

    TankArrowInitData( const char *pImgPath, RelativeRect StartPos, RelativeRect AnotherPos, RelativeRect BaseRect,
                       SDL_Scancode Key1 = SDL_SCANCODE_DOWN, SDL_Scancode Key2 = SDL_SCANCODE_UP ):
        m_TextureInitData( pImgPath, StartPos, BaseRect ), m_AnotherPos( AnotherPos ), m_Key1( Key1 ), m_Key2( Key2 ) {}
};


enum class CurrentPosition
{
    START   = 0,
    ANOTHER = 1
};


class TankArrowMoveAlgo: public MoveAlgorithm
{
public:
    TankArrowMoveAlgo( RelativeRect StartPos, RelativeRect AnotherPos, SDL_Scancode Key1 = SDL_SCANCODE_DOWN,
                       SDL_Scancode Key2 = SDL_SCANCODE_UP );
    ~TankArrowMoveAlgo();

    void move( RelativeRect &CurPos ) override;
    void move( SDL_Rect &CurPos ) override;

private:
    RelativeRect m_StartPos;
    RelativeRect m_AnotherPos;
    std::array < SDL_Scancode, 2 > m_ScancodeArr;
    CurrentPosition m_CurPos;
};


class TankArrow: public DynamicObject
{
public:
    TankArrow( TankArrowInitData InitData, Renderer renderer );
    ~TankArrow() override;

    void changePosition() override;
    void changeSize() override;
    void render() override;

private:
    Texture m_Texture;
    RelativeRect m_CurPos;
};

#endif // TANK_ARROW_H
