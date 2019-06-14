#ifndef TANKOFPLAYER_H
#define TANKOFPLAYER_H

#include "tank.h"

class TankOfPlayer: public Tank
{
public:
    TankOfPlayer( Animation TankMoveAnim, Animation TankExplosionAnim, Animation BombExplosionAnim, pSharedMap pMap,
                  RelativeRect RelativeBirthPos, GameEngine *pGameEngine,
                  CommonTanksProperties::MoveDirection moveDirection = CommonTanksProperties::MoveDirection::UP );
    ~TankOfPlayer() override;

    void changeSize() override;
    void render() override;

    void attachProperties( pSharedTankProperties pProperties ) override;
    void changePosition() override;
    pSharedTankShell tankShot() override;
    bool checkTankShellCollision( const SDL_Rect &Rect, CommonTanksProperties::TankOwnerIdentity tankOwnerIdentity ) override;

private:
    static const uint32_t DEFENSE_ON_TIME = 10000;

    Animation m_TankDefenseAnim;
    RelativeRect m_DefenseRelativeRect;
    uint32_t m_nDefenseTime;
    bool m_fDefense;
};

#endif // TANKOFPLAYER_H
