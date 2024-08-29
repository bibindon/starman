#pragma once

#include <d3d9.h>
#include "AnimMesh.h"
#include "Sprite.h"
#include "Stage1.h"

class Player
{
public:
    Player();
    ~Player();
    void Render();
    void Update(Stage1* stage1);
    void SetPos(const D3DXVECTOR3& pos);
    D3DXVECTOR3 GetPos();
    void SetMove(const D3DXVECTOR3& move);
    D3DXVECTOR3 GetMove();
    void SetRotate(const D3DXVECTOR3& rotate);
    D3DXVECTOR3 GetRotate();
    bool SetAttack();
    void SetWalk();
    void SetDamaged();
    void SetDead();
    bool GetDead();
    D3DXVECTOR3 GetAttackPos();
    void SetJump();

    void SetHP(const int hp);
    int GetHP();
private:
    AnimMesh* m_AnimMesh2 { nullptr };

    D3DXVECTOR3 m_pos { 0.f, 0.1f, 0.f }; // ‚¿‚å‚Á‚Æ•‚‚©‚¹‚é
    D3DXVECTOR3 m_rotate { 0.f, D3DX_PI, 0.f };
    D3DXVECTOR3 m_dir { 0.f, 0.f, 1.f };

    // ˆÚ“®ƒxƒNƒgƒ‹
    D3DXVECTOR3 m_move { 0.f, 0.f, 0.f };

    bool m_bAttack { false };
    int m_attackTimeCounter { 0 };

    bool m_bDamaged { false };
    int m_damagedTimeCounter { 0 };

    int m_HP { 100 };

    Sprite* m_spriteHP { nullptr };
    Sprite* m_spriteHPBack { nullptr };
    bool m_bDead { false };
    bool m_bJump { false };
    int m_jumpTimeCounter { 0 };
    float m_jumpVelocity { 0.f };
    const float JUMP_INITIAL_VELOCITY { 0.25f };
};

