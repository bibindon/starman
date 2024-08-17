#include "Enemy.h"
#include <ctime>
#include "SoundEffect.h"
#include "SharedObj.h"
#include "Sprite.h"
#include "Camera.h"
#include <random>

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
    SAFE_DELETE(m_spriteHP);
    SAFE_DELETE(m_spriteHPBack);
    SAFE_DELETE(m_AnimMesh);
}

bool Enemy::Init()
{
    AnimSetMap animSetMap;
    {
        AnimSetting animSetting { };
        animSetting.m_startPos = 0.f;
        animSetting.m_duration = 1.f;
        animSetting.m_loop = true;
        animSetMap["Idle"] = animSetting;
    }
    {
        AnimSetting animSetting { };
        animSetting.m_startPos = 1.f;
        animSetting.m_duration = 1.f;
        animSetting.m_loop = false;
        animSetMap["Damaged"] = animSetting;
    }
    {
        AnimSetting animSetting { };
        animSetting.m_startPos = 2.f;
        animSetting.m_duration = 0.5f;
        animSetting.m_loop = false;
        animSetMap["Attack"] = animSetting;
    }
    m_pos.z = 10.f;
    m_AnimMesh = new AnimMesh("res\\model\\rippoutai\\rippoutai.x",
        m_pos, m_rotate, 0.5f, animSetMap);
    SoundEffect::get_ton()->load("res\\sound\\damage01.wav");
    
    m_spriteHP = new Sprite("res\\image\\hp_green.png");
    m_spriteHPBack = new Sprite("res\\image\\hp_black.png");
    return true;
}

void Enemy::Update()
{
    if (m_state == eState::DEAD)
    {
        ++m_deadTimeCounter;
        if (m_deadTimeCounter >= 60)
        {
            m_state = eState::DISABLE;
        }
    }
    else if (m_state == eState::IDLE)
    {
        int randNum = SharedObj::GetRandom();

        std::string msg;
        msg = "randNum: " + std::to_string(randNum) + "\n";
        OutputDebugString(msg.c_str());
        if (randNum % 120 == 0)
        {
            m_state = eState::ATTACK;
        }
    }
    else if (m_state == eState::ATTACK)
    {
        ++m_attackTimeCounter;
        if (m_attackTimeCounter == 1)
        {
            m_AnimMesh->SetAnim("Attack", 0.f);
            Player* player = SharedObj::GetPlayer();
            D3DXVECTOR3 pos = player->GetPos();
            D3DXVECTOR3 rot = pos - m_pos;
            m_rotate.y = -atan2(rot.z, rot.x) + D3DX_PI*3/2;
        }
        else if (m_attackTimeCounter >= 60)
        {
            m_attackTimeCounter = 0;
            m_state = eState::IDLE;
        }
    }
}

void Enemy::Render()
{
    m_AnimMesh->SetPos(m_pos);
    m_AnimMesh->SetRotate(m_rotate);
    m_AnimMesh->Render();
    POINT screenPos = Camera::GetScreenPos(m_pos);
    m_spriteHPBack->Render(
        D3DXVECTOR3 { (FLOAT)screenPos.x - 64, (FLOAT)screenPos.y - 128, 0.f });
    m_spriteHP->Render(
        D3DXVECTOR3 { (FLOAT)screenPos.x - 64, (FLOAT)screenPos.y - 128, 0.f },
        255,
        (m_HP*128/100));
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
        m_AnimMesh->SetAnim("Damaged", 0.f);
    }
    else if (state == eState::DEAD)
    {
        SoundEffect::get_ton()->play("res\\sound\\damage01.wav", 90);
        m_AnimMesh->SetAnim("Damaged", 0.f);
    }
    m_state = state;
}

eState Enemy::GetState()
{
    return m_state;
}

