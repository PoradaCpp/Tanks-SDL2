#ifndef TANK_ARROW_H
#define TANK_ARROW_H

#include <array>

#include "texture.h"

struct TankArrowInitData
{
    ImgTextureInitData m_TextureInitData;
    RelativeRect m_TwoPlayersPos;
    SDL_Scancode m_Key1;
    SDL_Scancode m_Key2;

    TankArrowInitData( const char *pImgPath, RelativeRect StartPos, RelativeRect AnotherPos, RelativeRect BaseRect,
                       SDL_Scancode Key1 = SDL_SCANCODE_DOWN, SDL_Scancode Key2 = SDL_SCANCODE_UP ):
        m_TextureInitData( pImgPath, StartPos, BaseRect ), m_TwoPlayersPos( AnotherPos ), m_Key1( Key1 ), m_Key2( Key2 ) {}
};


enum class NumOfPlayers
{
    ONE_PLAYER  = 0,
    TWO_PLAYERS = 1
};

class TankArrow: public DisplayedObject
{
public:
    TankArrow( TankArrowInitData InitData, Renderer renderer, SDL_Scancode Key1 = SDL_SCANCODE_DOWN,
               SDL_Scancode Key2 = SDL_SCANCODE_UP );
    ~TankArrow() override;

    void changePosition();
    void changeSize() override;
    void render() override;

private:
    Texture m_Texture;
    RelativeRect m_OnePlayerPos;
    RelativeRect m_TwoPlayersPos;
    std::array < SDL_Scancode, 2 > m_ScancodeArr;
    NumOfPlayers m_CurNumOfPlayers;
    RelativeRect m_CurPos;
};

#endif // TANK_ARROW_H
