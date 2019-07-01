#ifndef PLAYERSHEART_H
#define PLAYERSHEART_H

#include "animation.h"
#include "audiochunk.h"

struct PlayersHeartInitData
{
    AnimationInitData m_AnimInitData;
    std::string m_sAudioChunkPath;

    PlayersHeartInitData( AnimationInitData AnimInitData, std::string sAudioChunkPath ):
        m_AnimInitData( AnimInitData ), m_sAudioChunkPath( sAudioChunkPath ) {}
};

class PlayersHeart
{
public:
    PlayersHeart( AnimationInitData AnimInitData, std::string sAudioChunkPath, Renderer renderer );
    PlayersHeart( PlayersHeartInitData InitData, Renderer renderer  );
    ~PlayersHeart();

    void render();
    void resize();
    void init();
    void destroy();
    bool isDestroying() const;
    bool isDestroyed() const;
    SDL_Rect getCollisionRect() const;

private:
    static const RelativeRect RELATIVE_SOLID_POS;
    static const size_t NOLMAL_HEART_STATE_ANIM_BEG = 0;
    static const size_t NOLMAL_HEART_STATE_ANIM_END = 15;
    static const size_t HEART_DESTROYING_STATE_ANIM_BEG = 16;
    static const size_t HEART_DESTROYING_STATE_ANIM_END = 31;

    Animation m_HeartAnim;
    AudioChunk m_AudioChunk;
    SDL_Rect m_AnimCurPos   = { 0, 0, 0, 0 };
    SDL_Rect m_RealSizeRect = { 0, 0, 0, 0 };
    bool m_fDestroying      = false;
    bool m_fDestroyed       = false;

    void calcNewSize();
};

#endif // PLAYERSHEART_H
