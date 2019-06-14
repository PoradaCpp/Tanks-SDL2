#ifndef TANKOFENEMY_H
#define TANKOFENEMY_H

#include "tank.h"

class TankOfEnemy: public Tank
{
public:
    TankOfEnemy( Animation TankMoveAnim, Animation TankExplosionAnim, Animation BombExplosionAnim, pSharedMap pMap,
                 RelativeRect RelativeBirthPos, GameEngine *pGameEngine, MoveDirection moveDirection = MoveDirection::UP );
    ~TankOfEnemy() override;

    void changePosition() override;

private:
    uint32_t m_nDeadLockChangeDirectionTime;

    void bonusAnimControl();
    void newRandomDirection();
};

#endif // TANKOFENEMY_H
