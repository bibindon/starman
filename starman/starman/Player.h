#pragma once

#include "AnimMesh.h"

class Player
{
public:
    Player();
    ~Player();
    void Render();
    void SetPos(const D3DXVECTOR3& pos);
    void SetRotate(const D3DXVECTOR3& rotate);
private:
    AnimMesh* m_AnimMesh2 { nullptr };

    D3DXVECTOR3 m_pos { 0.f, 0.f, 0.f };
    D3DXVECTOR3 m_rotate { 0.f, 0.f, 0.f };
};

