#pragma once

#include <d3d9.h>
#include "AnimMesh.h"
#include "Sprite.h"

enum class eDiskState
{
    IDLE,
    WALK,
    DAMAGED,
    DEAD,
    DISABLE,
    ATTACK,
};
class EnemyDisk
{
public:
    EnemyDisk();
    ~EnemyDisk();
    bool Init();
    void Finalize();
    void Render();
    void Update();
    void SetPos(const D3DXVECTOR3& pos);
    D3DXVECTOR3 GetPos();
    void SetRotate(const D3DXVECTOR3& rotate);
    D3DXVECTOR3 GetRotate();
    void SetHP(const int hp);
    int GetHP();
    void SetState(const eDiskState state);
    eDiskState GetState();
private:
    AnimMesh* m_AnimMesh { nullptr };

    D3DXVECTOR3 m_loadingPos { 0.f, 0.f, 0.f };
    D3DXVECTOR3 m_rotate { 0.f, 0.f, 0.f };

    bool m_bAttack { false };
    int m_attackTimeCounter { 0 };
    int m_HP { 100 };
    Sprite* m_spriteHP { nullptr };
    Sprite* m_spriteHPBack { nullptr };
    eDiskState m_state { eDiskState::IDLE };
    int m_deadTimeCounter { 0 };
    int m_damagedTimeCounter { 0 };
    D3DXVECTOR3 GetAttackPos();
};

