#include "Player.h"
#include "SoundEffect.h"

Player::Player()
{
    D3DXVECTOR3 b = D3DXVECTOR3(0, 0, 0);
    m_AnimMesh2 = new AnimMesh("res\\model\\hoshiman\\hoshiman.x", b, b, 0.5f);
    m_AnimMesh2->SetAnim("Idle");
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
       // m_AnimMesh2->SetAnim("Walk");
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

bool Player::SetAttack()
{
    bool ret { false };
    if (!m_bAttack)
    {
        SoundEffect::get_ton()->play("res\\sound\\attack01.wav", 90);
        m_AnimMesh2->SetAnim("Attack", 0.f);
        m_bAttack = true;
        ret = true;
    }
    return ret;
}

void Player::SetWalk()
{
    m_AnimMesh2->SetAnim("Walk");
}

D3DXVECTOR3 Player::GetAttackPos()
{
    D3DXVECTOR3 pos { m_pos };
    D3DXVECTOR3 norm { 0.f, 0.f, 0.f };
    norm.x = std::sin(m_rotate.y);
    norm.z = std::sin(m_rotate.y + (D3DX_PI / 2));
    pos += norm*2;
    return pos;
}
