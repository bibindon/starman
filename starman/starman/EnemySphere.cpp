#include "EnemySphere.h"
#include <ctime>
#include "SoundEffect.h"
#include "SharedObj.h"
#include "Sprite.h"
#include "Camera.h"
#include <random>

EnemySphere::EnemySphere()
{
}

EnemySphere::~EnemySphere()
{
}

bool EnemySphere::Init()
{
    AnimSetMap animSetMap;
    {
        AnimSetting animSetting { };
        animSetting.m_startPos = 0.5f;
        animSetting.m_duration = 0.5f;
        animSetting.m_loop = true;
        animSetMap["Idle"] = animSetting;
    }
    {
        AnimSetting animSetting { };
        animSetting.m_startPos = 0.f;
        animSetting.m_duration = 0.5f;
        animSetting.m_loop = true;
        animSetMap["Walk"] = animSetting;
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
        animSetting.m_duration = 1.f;
        animSetting.m_loop = false;
        animSetMap["Attack"] = animSetting;
    }
    m_AnimMesh = new AnimMesh("res\\model\\enemySphere\\enemySphere.x",
        m_pos, m_rotate, 0.5f, animSetMap);
    SoundEffect::get_ton()->load("res\\sound\\damage01.wav");
    
    m_spriteHP = new Sprite("res\\image\\hp_green.png");
    m_spriteHPBack = new Sprite("res\\image\\hp_black.png");

    m_AnimMesh->SetAnim("Idle", 0.f);
    return true;
}

void EnemySphere::Finalize()
{
    SAFE_DELETE(m_spriteHP);
    SAFE_DELETE(m_spriteHPBack);
    SAFE_DELETE(m_AnimMesh);
}

void EnemySphere::Update()
{
    if (m_state == eSphereState::DEAD)
    {
        ++m_deadTimeCounter;
        if (m_deadTimeCounter >= 60)
        {
            m_state = eSphereState::DISABLE;
        }
    }
    else if (m_state == eSphereState::IDLE)
    {
        Player* player = SharedObj::GetPlayer();
        D3DXVECTOR3 pos = player->GetPos();
        D3DXVECTOR3 enemyVector = pos - m_pos;
        FLOAT distance = D3DXVec3Length(&enemyVector);
        if (distance < 10.f)
        {
            m_state = eSphereState::WALK;
            m_AnimMesh->SetAnim("Walk", 0.f);
        }
    }
    else if (m_state == eSphereState::WALK)
    {
        Player* player = SharedObj::GetPlayer();
        D3DXVECTOR3 pos = player->GetPos();
        D3DXVECTOR3 enemyVector = pos - m_pos;
        FLOAT distance = D3DXVec3Length(&enemyVector);
        if (distance < 3.f)
        {
            int randNum = SharedObj::GetRandom();

            //std::string msg;
            //msg = "randNum: " + std::to_string(randNum) + "\n";
            //OutputDebugString(msg.c_str());
            if (randNum % 30 == 0)
            {
                m_state = eSphereState::ATTACK;
            }
        }
        else if (3.f <= distance && distance < 30.f)
        {
            D3DXVECTOR3 norm { 0.f, 0.f, 0.f };
            D3DXVec3Normalize(&norm, &enemyVector);
            m_pos += norm / 40;
            m_rotate.y = atan2(-enemyVector.x, -enemyVector.z);
        }
        else if (20.f <= distance)
        {
            m_state = eSphereState::IDLE;
            m_AnimMesh->SetAnim("Idle", 0.f);
        }
    }
    else if (m_state == eSphereState::DAMAGED)
    {
        m_damagedTimeCounter++;
        if (m_damagedTimeCounter >= 60)
        {
            m_damagedTimeCounter = 0;
            m_state = eSphereState::IDLE;
        }
    }
    else if (m_state == eSphereState::ATTACK)
    {
        ++m_attackTimeCounter;
        if (m_attackTimeCounter == 1)
        {
            m_AnimMesh->SetAnim("Attack", 0.f);
            Player* player = SharedObj::GetPlayer();
            D3DXVECTOR3 pos = player->GetPos();
            D3DXVECTOR3 rot = pos - m_pos;
            m_rotate.y = -atan2(rot.z, rot.x) + D3DX_PI*3/2;


            D3DXVECTOR3 attackPos { GetAttackPos() };
            D3DXVECTOR3 playerPos { 0.f, 0.f, 0.f };
            playerPos = player->GetPos();
            D3DXVECTOR3 subPos { attackPos - playerPos };
            FLOAT distance = D3DXVec3Length(&subPos);
            std::string msg;
            msg = "distance: " + std::to_string(distance) + "\n";
            OutputDebugString(msg.c_str());

            if (distance <= 1.0f)
            {
                player->SetDamaged();
                int hp = player->GetHP();
                player->SetHP(hp - 20);
            }

        }
        else if (m_attackTimeCounter >= 60)
        {
            m_attackTimeCounter = 0;
            m_state = eSphereState::IDLE;
        }
    }
}

void EnemySphere::Render()
{
    m_AnimMesh->SetPos(m_pos);
    m_AnimMesh->SetRotate(m_rotate);
    m_AnimMesh->Render();
    POINT screenPos = Camera::GetScreenPos(m_pos);
    m_spriteHPBack->Render(
        D3DXVECTOR3 { (FLOAT)screenPos.x - 64, (FLOAT)screenPos.y - 128, 0.f });
    if (m_HP >= 0)
    {
        m_spriteHP->Render(
            D3DXVECTOR3 { (FLOAT)screenPos.x - 64, (FLOAT)screenPos.y - 128, 0.f },
            255,
            (m_HP*128/100));
    }
}

void EnemySphere::SetPos(const D3DXVECTOR3& pos)
{
    m_pos = pos;
}

D3DXVECTOR3 EnemySphere::GetPos()
{
    return m_pos;
}

void EnemySphere::SetRotate(const D3DXVECTOR3& rotate)
{
    m_rotate = rotate;
}

D3DXVECTOR3 EnemySphere::GetRotate()
{
    return m_rotate;
}

void EnemySphere::SetHP(const int hp)
{
    m_HP = hp;
    if (m_HP <= 0)
    {
        m_state = eSphereState::DEAD;
    }
}

int EnemySphere::GetHP()
{
    return m_HP;
}

void EnemySphere::SetState(const eSphereState state)
{
    if (state == eSphereState::DAMAGED)
    {
        SoundEffect::get_ton()->play("res\\sound\\damage01.wav", 90);
        m_AnimMesh->SetAnim("Damaged", 0.f);
    }
    else if (state == eSphereState::DEAD)
    {
        SoundEffect::get_ton()->play("res\\sound\\damage01.wav", 90);
        m_AnimMesh->SetAnim("Damaged", 0.f);
    }
    m_state = state;
}

eSphereState EnemySphere::GetState()
{
    return m_state;
}

D3DXVECTOR3 EnemySphere::GetAttackPos()
{
    D3DXVECTOR3 pos { m_pos };
    D3DXVECTOR3 norm { 0.f, 0.f, 0.f };
    norm.x = std::sin(m_rotate.y + D3DX_PI);
    norm.z = std::sin(m_rotate.y + (D3DX_PI * 3 / 2));
    pos += norm * 2;
    return pos;
}
