#include "SeqBattle.h"
#include "Common.h"
#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif
#include <dinput.h>
#include "KeyBoard.h"
#include "Mouse.h"
#include "Camera.h"
#include "JoyStick.h"
#include "Light.h"
#include "BGM.h"
#include "SharedObj.h"

SeqBattle::SeqBattle()
{
    D3DXVECTOR3 b = D3DXVECTOR3(0, -9.f, 0);
    D3DXVECTOR3 c = D3DXVECTOR3(0, 0, 0);
    m_mesh1 = new Mesh("res\\model\\dessert\\dessert.x", b, c, 100.0f);
    b.x = 15.f;
    b.y = -80.f;
    m_meshSky = new Mesh("res\\model\\hemisphere\\hemisphere.x", b, c, 1000.0f);
    c.y = D3DX_PI;
    m_meshSky2 = new Mesh("res\\model\\hemisphere\\hemisphere.x", b, c, 1000.0f);
    b.y = 0.f;

    m_player = new Player();
    SharedObj::SetPlayer(m_player);
    m_enemy = new Enemy();
    m_enemy->Init();

    BGM::get_ton()->load("res\\sound\\letsgo.wav");
    BGM::get_ton()->play(10);
}

SeqBattle::~SeqBattle()
{
}

void SeqBattle::Update(eSequence* sequence)
{
    D3DXVECTOR3 pos = m_player->GetPos();
    D3DXVECTOR3 rotate {0.f, 0.f, 0.f};
    float radian = Camera::GetRadian();
    float yaw = -1.f * (radian - (D3DX_PI / 2));
    if (KeyBoard::IsHold(DIK_W))
    {
        pos.x += -std::sin(radian + D3DX_PI / 2) / 10;
        pos.z += std::sin(radian + D3DX_PI) / 10;

        D3DXVECTOR3 rotate { 0.f, yaw, 0.f };
        m_player->SetRotate(rotate);
        m_player->SetWalk();
    }
    if (KeyBoard::IsHold(DIK_A))
    {
        pos.x += -std::sin(radian + D3DX_PI) / 10;
        pos.z += std::sin(radian + D3DX_PI * 3 / 2) / 10;

        D3DXVECTOR3 rotate { 0.f, yaw + D3DX_PI * 3 / 2, 0.f };
        m_player->SetRotate(rotate);
        m_player->SetWalk();
    }
    if (KeyBoard::IsHold(DIK_S))
    {
        pos.x += -std::sin(radian + D3DX_PI * 3 / 2) / 10;
        pos.z += std::sin(radian) / 10;

        D3DXVECTOR3 rotate { 0.f, yaw + D3DX_PI, 0.f };
        m_player->SetRotate(rotate);
        m_player->SetWalk();
    }
    if (KeyBoard::IsHold(DIK_D))
    {
        pos.x += -std::sin(radian) / 10;
        pos.z += std::sin(radian + D3DX_PI / 2) / 10;

        D3DXVECTOR3 rotate { 0.f, yaw + D3DX_PI / 2, 0.f };
        m_player->SetRotate(rotate);
        m_player->SetWalk();
    }

    if (Mouse::IsDownLeft())
    {
        InputR1();
    }

    if (JoyStick::IsHold(eJoyStickButtonType::UP))
    {
        pos.x += std::sin(radian+D3DX_PI)/10;
        pos.z += std::sin(radian+D3DX_PI*3/2)/10;

        D3DXVECTOR3 rotate {0.f, radian+D3DX_PI/2, 0.f};
        m_player->SetRotate(rotate);
    }
    if (JoyStick::IsHold(eJoyStickButtonType::LEFT))
    {
        pos.x += std::sin(radian+D3DX_PI/2)/10;
        pos.z += std::sin(radian+D3DX_PI)/10;

        D3DXVECTOR3 rotate {0.f, radian, 0.f};
        m_player->SetRotate(rotate);
    }
    if (JoyStick::IsHold(eJoyStickButtonType::DOWN))
    {
        pos.x += std::sin(radian)/10;
        pos.z += std::sin(radian+D3DX_PI/2)/10;

        D3DXVECTOR3 rotate {0.f, radian+D3DX_PI, 0.f};
        m_player->SetRotate(rotate);
    }
    if (JoyStick::IsHold(eJoyStickButtonType::RIGHT))
    {
        pos.x += std::sin(radian+D3DX_PI*3/2)/10;
        pos.z += std::sin(radian)/10;

        D3DXVECTOR3 rotate {0.f, radian+D3DX_PI*3/2, 0.f};
        m_player->SetRotate(rotate);
    }
    if (JoyStick::IsDown(eJoyStickButtonType::R1))
    {
        InputR1();
    }
    m_player->SetPos(pos);
    Camera::SetPos(pos);
    m_player->Update();
    if (m_enemy != nullptr)
    {
        m_enemy->Update();
        if (m_enemy->GetState() == eState::DISABLE)
        {
            SAFE_DELETE(m_enemy);
    //        *sequence = eSequence::ENDING;
        }
    }
}

void SeqBattle::Render()
{
    m_mesh1->Render();
    D3DXVECTOR4 norm = Light::GetLightNormal();
    D3DXVECTOR4 norm2 { norm };
    // “V‹…—p‚É‰º‚©‚çŒõ‚ð“–‚Ä‚Ä‚¢‚é‚±‚Æ‚É‚·‚é
    // ³‚µ‚­‚È‚¢‚â‚è•ûB
    norm2.x = 0.f;
    norm2.y = -1.f;
    Light::SetLightNormal(norm2);
    m_meshSky->Render();
    m_meshSky2->Render();
    Light::SetLightNormal(norm);
    m_player->Render();
    if (m_enemy != nullptr)
    {
        m_enemy->Render();
    }
}

void SeqBattle::InputR1()
{
    bool ret { m_player->SetAttack() };
    if (ret == false)
    {
        return;
    }
    D3DXVECTOR3 attackPos { m_player->GetAttackPos() };
    D3DXVECTOR3 enemyPos { 0.f, 0.f, 0.f };
    if (m_enemy != nullptr)
    {
        enemyPos = m_enemy->GetPos();
    }
    else
    {
        return;
    }
    D3DXVECTOR3 subPos { attackPos - enemyPos };
    FLOAT distance = D3DXVec3Length(&subPos);

    if (distance <= 1.0f)
    {
        m_enemy->SetState(eState::DAMAGED);
        int hp = m_enemy->GetHP();
        m_enemy->SetHP(hp - 10);
    }
}
