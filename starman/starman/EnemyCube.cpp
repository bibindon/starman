#include "EnemyCube.h"
#include <ctime>
#include "SoundEffect.h"
#include "SharedObj.h"
#include "Sprite.h"
#include "Camera.h"
#include <random>

EnemyCube::EnemyCube()
{
}

EnemyCube::~EnemyCube()
{
}

bool EnemyCube::Init()
{
    m_thread = new std::thread(
        [&]
        {
            std::lock_guard<std::mutex> lock(s_mutex);
            OutputDebugString("@@@@start\n");
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
                animSetting.m_duration = 0.5f;
                animSetting.m_loop = false;
                animSetMap["Attack"] = animSetting;
            }
            m_AnimMesh = new AnimMesh("res\\model\\rippoutai\\rippoutai.x",
                m_loadingPos, m_rotate, 0.5f, animSetMap);
            SoundEffect::get_ton()->load("res\\sound\\damage01.wav");
            
            m_spriteHP = new Sprite("res\\image\\hp_green.png");
            m_spriteHPBack = new Sprite("res\\image\\hp_black.png");

            m_AnimMesh->SetAnim("Idle", 0.f);
            m_loaded.store(true);
            OutputDebugString("@@@@end\n");
        }
        );
    return true;
}

void EnemyCube::Update()
{
    if (m_loaded.load() == false)
    {
        return;
    }

    if (m_state == eEnemyState::DEAD)
    {
        ++m_deadTimeCounter;
        if (m_deadTimeCounter >= 60)
        {
            m_state = eEnemyState::DISABLE;
        }
    }
    else if (m_state == eEnemyState::IDLE)
    {
        Player* player = SharedObj::GetPlayer();
        D3DXVECTOR3 pos = player->GetPos();
        D3DXVECTOR3 enemyVector = pos - m_loadingPos;
        FLOAT distance = D3DXVec3Length(&enemyVector);
        if (distance < 10.f)
        {
            m_state = eEnemyState::WALK;
            m_AnimMesh->SetAnim("Walk", 0.f);
        }
    }
    else if (m_state == eEnemyState::WALK)
    {
        Player* player = SharedObj::GetPlayer();
        D3DXVECTOR3 pos = player->GetPos();
        D3DXVECTOR3 enemyVector = pos - m_loadingPos;
        FLOAT distance = D3DXVec3Length(&enemyVector);
        if (distance < 3.f)
        {
            int randNum = SharedObj::GetRandom();

            //std::string msg;
            //msg = "randNum: " + std::to_string(randNum) + "\n";
            //OutputDebugString(msg.c_str());
            if (randNum % 30 == 0)
            {
                m_state = eEnemyState::ATTACK;
            }
        }
        else if (3.f <= distance && distance < 20.f)
        {
            D3DXVECTOR3 norm { 0.f, 0.f, 0.f };
            D3DXVec3Normalize(&norm, &enemyVector);
            // •Ç‚¸‚è
            Map* map = SharedObj::GetMap();
            D3DXVECTOR3 move = norm / 50;
            bool bHit = false;
            move = map->WallSlide(m_loadingPos, move, &bHit);
            m_loadingPos += move;
            m_rotate.y = atan2(-enemyVector.x, -enemyVector.z);
        }
        else if (20.f <= distance)
        {
            m_state = eEnemyState::IDLE;
            m_AnimMesh->SetAnim("Idle", 0.f);
        }
    }
    else if (m_state == eEnemyState::DAMAGED)
    {
        m_damagedTimeCounter++;
        if (m_damagedTimeCounter >= 60)
        {
            m_damagedTimeCounter = 0;
            m_state = eEnemyState::IDLE;
        }
    }
    else if (m_state == eEnemyState::ATTACK)
    {
        ++m_attackTimeCounter;
        if (m_attackTimeCounter == 1)
        {
            m_AnimMesh->SetAnim("Attack", 0.f);
            Player* player = SharedObj::GetPlayer();
            D3DXVECTOR3 pos = player->GetPos();
            D3DXVECTOR3 rot = pos - m_loadingPos;
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
                player->SetHP(hp - 10);
            }

        }
        else if (m_attackTimeCounter >= 60)
        {
            m_attackTimeCounter = 0;
            m_state = eEnemyState::IDLE;
        }
    }
}

