#ifndef BONUS_H
#define BONUS_H

#include <vector>
#include <array>

#include "texture.h"
#include "audiochunk.h"

class Bonus
{
public:
    enum class BonusType
    {
        ENEMY_EXPLOSING = 0,
        TANK_UPGRADE    = 1,
        DEFENCE         = 2,
        ENEMY_TEMP_STOP = 3,
        EXTRA_LIFE      = 4,
        FORTIFICATION   = 5
    };

    Bonus( Texture texture, AudioChunk bonusAdd, AudioChunk bonusTaken, BonusType bonusType );
    ~Bonus();

    BonusType getBonusType() const;
    SDL_Rect getDestination() const;
    void setPosition( int nX, int nY );
    void playTakingSound();

    void changeSize();
    void render();

    static const size_t ROW_SIZE       = 3;
    static const size_t COLUMN_SIZE    = 2;
    static const size_t BONUS_QUANTITY = ROW_SIZE * COLUMN_SIZE;

private:
    Texture m_ImageTexture;
    AudioChunk m_BonusAddAudio;
    AudioChunk m_BonusTakenAudio;
    std::array <SDL_Rect, BONUS_QUANTITY> m_SourceRectArr;
    SDL_Rect m_DestImgRect = { 0, 0, 0, 0 };
    BonusType m_BonusType;

    void setRenderDimensions();
};

typedef std::shared_ptr <Bonus> pSharedBonus;

#endif // BONUS_H
