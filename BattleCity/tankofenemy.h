#ifndef TANKOFENEMY_H
#define TANKOFENEMY_H

#include "tank.h"

class TankOfEnemy: public Tank
{
public:
    TankOfEnemy( Animation TankMoveAnim, Animation TankExplosionAnim, Animation BombExplosionAnim, std::vector <AudioChunk> audioVc,
                 pSharedMap pMap, RelativeRect RelativeBirthPos, std::shared_ptr <ObjectsManagement> pObjectManagement,
                 CommonTanksProperties::MoveDirection moveDirection = CommonTanksProperties::MoveDirection::UP );
    ~TankOfEnemy() override;

    void render() override;
    void changePosition() override;

private:
    uint32_t m_nDeadLockChangeDirectionTime;

    void bonusAnimControl();
    void newRandomDirection();
};

#endif // TANKOFENEMY_H
