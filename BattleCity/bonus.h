#ifndef BONUS_H
#define BONUS_H

#include <vector>
#include <array>

#include "texture.h"

class Bonus
{
public:
    enum class BonusType
    {
        DEFENCE         = 0,
        ENEMY_EXPLOSING = 1,
        ENEMY_TEMP_STOP = 2,
        EXTRA_LIFE      = 3,
        FORTIFICATION   = 4,
        TANK_UPGRADE    = 5
    };

    Bonus( Texture texture, BonusType bonusType );
    ~Bonus();

    BonusType getBonusType() const;
    void setPosition( int nX, int nY );

    void changeSize();
    void render();
    bool checkCollision( const SDL_Rect &CheckingRect );

    static const size_t ROW_SIZE       = 3;
    static const size_t COLUMN_SIZE    = 2;
    static const size_t BONUS_QUANTITY = ROW_SIZE * COLUMN_SIZE;

private:
    Texture m_ImageTexture;
    Renderer m_Renderer;
    std::array <SDL_Rect, BONUS_QUANTITY> m_SourceRectArr;
    SDL_Rect m_DestImgRect = { 0, 0, 0, 0 };
    BonusType m_BonusType;

    void setRenderDimensions();
};

typedef std::shared_ptr <Bonus> pSharedBonus;

#endif // BONUS_H
