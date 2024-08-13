#include "Enemy.h"
#include "SoundEffect.h"

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

void Enemy::Init()
{
    m_pos.x = -10.f;
    m_AnimMesh = new AnimMesh("res\\model\\cube4\\cube4.x", m_pos, m_rotate, 0.5f);
    SoundEffect::get_ton()->load("res\\sound\\damage01.wav");
}

void Enemy::Update()
{
}

void Enemy::Render()
{
    m_AnimMesh->SetPos(m_pos);
    m_AnimMesh->SetRotate(m_rotate);
    m_AnimMesh->Render();
}

void Enemy::SetPos(const D3DXVECTOR3& pos)
{
    m_pos = pos;
}

D3DXVECTOR3 Enemy::GetPos()
{
    return m_pos;
}

void Enemy::SetRotate(const D3DXVECTOR3& rotate)
{
    m_rotate = rotate;
}

D3DXVECTOR3 Enemy::GetRotate()
{
    return m_rotate;
}

void Enemy::SetAttack()
{
}

void Enemy::SetHP(const int hp)
{
    m_HP = hp;
    if (m_HP <= 0)
    {
        m_state = eState::DEAD;
    }
}

int Enemy::GetHP()
{
    return m_HP;
}

void Enemy::SetState(const eState state)
{
    if (state == eState::DAMAGED)
    {
        SoundEffect::get_ton()->play("res\\sound\\damage01.wav", 90);
    }
    m_state = state;
}

eState Enemy::GetState()
{
    return m_state;
}

