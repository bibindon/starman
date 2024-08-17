#pragma once

#include <d3d9.h>
#include "AnimMesh.h"

class Player
{
public:
    Player();
    ~Player();
    void Render();
    void Update();
    void SetPos(const D3DXVECTOR3& pos);
    D3DXVECTOR3 GetPos();
    void SetRotate(const D3DXVECTOR3& rotate);
    D3DXVECTOR3 GetRotate();
    bool SetAttack();
    void SetWalk();
    void SetDamaged();
    D3DXVECTOR3 GetAttackPos();
private:
    AnimMesh* m_AnimMesh2 { nullptr };

    D3DXVECTOR3 m_pos { 0.f, 0.f, 0.f };
    D3DXVECTOR3 m_rotate { 0.f, 0.f, 0.f };

    bool m_bAttack { false };
    int m_attackTimeCounter { 0 };

    bool m_bDamaged { false };
    int m_damagedTimeCounter { 0 };
};

