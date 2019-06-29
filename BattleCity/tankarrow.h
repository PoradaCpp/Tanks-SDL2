#ifndef TANK_ARROW_H
#define TANK_ARROW_H

#include <array>

#include "texture.h"
#include "buttonbase.h"
#include "audiochunk.h"

struct TankArrowInitData
{
    ImgTextureInitData m_TextureInitData;
    RelativeRect m_TwoPlayersPos;
    SDL_Scancode m_Key1;
    SDL_Scancode m_Key2;
    std::string  m_sAudioPath;

    TankArrowInitData( const char *pImgPath, std::string sAudioPath, RelativeRect StartPos, RelativeRect AnotherPos,
                       RelativeRect BaseRect, SDL_Scancode Key1 = SDL_SCANCODE_DOWN, SDL_Scancode Key2 = SDL_SCANCODE_UP ):
    m_TextureInitData( pImgPath, StartPos, BaseRect ), m_TwoPlayersPos( AnotherPos ), m_Key1( Key1 ), m_Key2( Key2 ),
    m_sAudioPath( sAudioPath ) {}
};


enum class NumOfPlayers
{
    ONE_PLAYER  = 0,
    TWO_PLAYERS = 1
};

class TankArrow: public ButtonBase
{
public:
    TankArrow( TankArrowInitData InitData, Renderer renderer, SDL_Scancode Key1 = SDL_SCANCODE_DOWN,
               SDL_Scancode Key2 = SDL_SCANCODE_UP );
    ~TankArrow() override;

    void setAction( Action action ) override;
    void execute() override;
    void lock() override;
    void unlock() override;

    void changePosition();
    void changeSize() override;
    void render() override;

private:
    Texture m_Texture;
    AudioChunk m_AudioChunk;
    RelativeRect m_OnePlayerPos;
    RelativeRect m_TwoPlayersPos;
    std::array < SDL_Scancode, 2 > m_ScancodeArr;
    NumOfPlayers m_CurNumOfPlayers;
    RelativeRect m_CurPos;
    bool m_fNumOfPlayersChanged;
    bool m_fLock;
};

#endif // TANK_ARROW_H
