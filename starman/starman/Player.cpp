#include "Player.h"
#include "SoundEffect.h"

Player::Player()
{
    D3DXVECTOR3 b = D3DXVECTOR3(0, 0, 0);
//    m_AnimMesh2 = new AnimMesh("res\\model\\RobotArm\\RobotArm.x", b, b, 0.5f);
    m_AnimMesh2 = new AnimMesh("res\\model\\hoshiman\\hoshiman.x", b, b, 0.5f);
    m_AnimMesh2->SetAnim("Walk");
    SoundEffect::get_ton()->load("res\\sound\\attack01.wav");
}

Player::~Player()
{
}

void Player::Update()
{
    if (m_bAttack)
    {
        ++m_attackTimeCounter;
    }
    if (m_attackTimeCounter >= 30)
    {
        m_attackTimeCounter = 0;
        m_bAttack = false;
        m_AnimMesh2->SetAnim("Walk");
    }
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

D3DXVECTOR3 Player::GetPos()
{
    return m_pos;
}

void Player::SetRotate(const D3DXVECTOR3& rotate)
{
    m_rotate = rotate;
}

D3DXVECTOR3 Player::GetRotate()
{
    return m_rotate;
}

void Player::SetAttack()
{
    if (!m_bAttack)
    {
        SoundEffect::get_ton()->play("res\\sound\\attack01.wav", 90);
        m_AnimMesh2->SetAnim("Attack");
        m_bAttack = true;
    }
}
