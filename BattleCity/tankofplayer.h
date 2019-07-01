#ifndef TANKOFPLAYER_H
#define TANKOFPLAYER_H

#include "tank.h"

class TankOfPlayer: public Tank
{
public:
    TankOfPlayer( Animation TankMoveAnim, Animation TankExplosionAnim, Animation BombExplosionAnim, std::vector <AudioChunk> audioVc,
                  pSharedMap pMap, RelativeRect RelativeBirthPos, std::shared_ptr <ObjectsManagement> pObjectManagement,
                  CommonTanksProperties::MoveDirection moveDirection = CommonTanksProperties::MoveDirection::UP );
    ~TankOfPlayer() override;

    void changeSize() override;
    void render() override;

    void attachProperties( pSharedTankProperties pProperties ) override;
    void changePosition() override;
    bool shotCondition() override;

private:
    static const uint32_t DEFENSE_ON_TIME = 10000;

    Animation m_TankDefenseAnim;
    RelativeRect m_DefenseRelativeRect = { 0, 0, 0, 0 };
    uint32_t m_nDefenseTime            = 0;
};

#endif // TANKOFPLAYER_H
