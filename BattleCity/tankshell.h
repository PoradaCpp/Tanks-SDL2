#ifndef TANKSHELL_H
#define TANKSHELL_H

#include "animation.h"
#include "tank.h"


class TankShell
{
public:
    TankShell( Animation ShellExplosionAnim, RelativeRect RelativePos, CommonTanksProperties::MoveDirection moveDirection,
               CommonTanksProperties::TankOwnerIdentity tankOwnerIdentity, double dRelativeXDestroyingForce );
    ~TankShell();

    void changeSize();
    void render();

    void setExplosion();
    bool getExplosion();
    bool isExplosed();
    void changePosition();
    void setPosition( int nX, int nY );
    SDL_Rect getPosition() const;
    SDL_Rect getCollisionPosition() const;
    SDL_Rect getDestoyingVolume() const;
    CommonTanksProperties::TankOwnerIdentity getOwnerIdentity() const;
    CommonTanksProperties::MoveDirection getMoveDirection() const;

private:
    Animation m_ShellAnim;
    SDL_Rect m_CurPos;
    SDL_Rect m_CollisionSize;
    SDL_Rect m_HorizShellDestroyingVolume;
    SDL_Rect m_VertShellDestroyingVolume;
    double m_dRelativeXDestroyingForce;
    CommonTanksProperties::MoveDirection m_MoveDirection;
    CommonTanksProperties::TankOwnerIdentity m_TankOwnerIdentity;
    uint32_t m_nTankShellSpeed;
    bool m_fExplosion;
    bool m_fExplosed;

    void recalcPosition();
    void calcDestroyingVolume();
    void calcDestroyingVolumePos();
};

#endif // TANKSHELL_H
