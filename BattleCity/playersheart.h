#ifndef PLAYERSHEART_H
#define PLAYERSHEART_H

#include "animation.h"

class PlayersHeart
{
public:
    PlayersHeart( AnimationInitData AnimInitData, Renderer renderer );
    ~PlayersHeart();

    void render();
    void resize();
    void destroy();
    bool isDestroying();
    bool isDestroyed();
    SDL_Rect getCollisionRect();

private:
    static const RelativeRect RELATIVE_SOLID_POS;
    static const size_t NOLMAL_HEART_STATE_ANIM_BEG = 0;
    static const size_t NOLMAL_HEART_STATE_ANIM_END = 7;
    static const size_t HEART_DESTROYING_STATE_ANIM_BEG = 8;
    static const size_t HEART_DESTROYING_STATE_ANIM_END = 23;

    Animation m_HeartAnim;
    SDL_Rect m_AnimCurPos;
    SDL_Rect m_RealSizeRect;
    bool m_fDestroying;
    bool m_fDestroyed;

    void calcNewSize();
};

#endif // PLAYERSHEART_H
