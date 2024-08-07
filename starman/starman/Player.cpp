#include "Player.h"

Player::Player()
{
    D3DXVECTOR3 b = D3DXVECTOR3(0, 0, 0);
    m_AnimMesh2 = new AnimMesh("res\\model\\RobotArm\\RobotArm.x", b, b, 10.0f);
}

Player::~Player()
{
}

void Player::Render()
{
    m_AnimMesh2->SetPos(m_pos);
    m_AnimMesh2->SetRotate(m_rotate);
    m_AnimMesh2->Render();
}

void Player::SetPos(const D3DXVECTOR3& pos)
{
    m_pos = pos;
}

void Player::SetRotate(const D3DXVECTOR3& rotate)
{
    m_rotate = rotate;
}
