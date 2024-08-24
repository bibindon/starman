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
    D3DXVECTOR3 b = D3DXVECTOR3(0, 0.f, 0);
    D3DXVECTOR3 c = D3DXVECTOR3(0, 0, 0);
    m_player = new Player();
    SharedObj::SetPlayer(m_player);

    D3DXVECTOR3 pos = D3DXVECTOR3(6.f, 0.f, 10.f);

    BGM::get_ton()->load("res\\sound\\letsgo.wav");
    BGM::get_ton()->play(10);

    m_spriteGameover = new Sprite("res\\image\\gameover.png");

    m_stage1 = new Stage1();
    m_stage1->Init();
}

SeqBattle::~SeqBattle()
{
}

void SeqBattle::Update(eSequence* sequence)
{
    if (m_nCurrentStage == 1)
    {
        m_stage1->Update();
    }
    else if (m_nCurrentStage == 2)
    {
        m_stage2->Update();
    }
    else if (m_nCurrentStage == 3)
    {
        m_stage3->Update();
    }
    D3DXVECTOR3 pos = m_player->GetPos();
    D3DXVECTOR3 rotate {0.f, 0.f, 0.f};
    float radian = Camera::GetRadian();
    float yaw = -1.f * (radian - (D3DX_PI / 2));
    if (KeyBoard::IsHold(DIK_W))
    {
        pos.x += -std::sin(radian + D3DX_PI / 2) / 5;
        pos.z += std::sin(radian + D3DX_PI) / 5;

        D3DXVECTOR3 rotate { 0.f, yaw, 0.f };
        m_player->SetRotate(rotate);
        m_player->SetWalk();
    }
    if (KeyBoard::IsHold(DIK_A))
    {
        pos.x += -std::sin(radian + D3DX_PI) / 5;
        pos.z += std::sin(radian + D3DX_PI * 3 / 2) / 5;

        D3DXVECTOR3 rotate { 0.f, yaw + D3DX_PI * 3 / 2, 0.f };
        m_player->SetRotate(rotate);
        m_player->SetWalk();
    }
    if (KeyBoard::IsHold(DIK_S))
    {
        pos.x += -std::sin(radian + D3DX_PI * 3 / 2) / 5;
        pos.z += std::sin(radian) / 5;

        D3DXVECTOR3 rotate { 0.f, yaw + D3DX_PI, 0.f };
        m_player->SetRotate(rotate);
        m_player->SetWalk();
    }
    if (KeyBoard::IsHold(DIK_D))
    {
        pos.x += -std::sin(radian) / 5;
        pos.z += std::sin(radian + D3DX_PI / 2) / 5;

        D3DXVECTOR3 rotate { 0.f, yaw + D3DX_PI / 2, 0.f };
        m_player->SetRotate(rotate);
        m_player->SetWalk();
    }
    if (KeyBoard::IsDown(DIK_F))
    {
        InputB(sequence);
    }
    if (KeyBoard::IsDown(DIK_SPACE))
    {
        InputA();
    }

    if (Mouse::IsDownLeft())
    {
        InputR1();
    }

    if (JoyStick::IsHold(eJoyStickButtonType::UP))
    {
        pos.x += -std::sin(radian + D3DX_PI / 2) / 5;
        pos.z += std::sin(radian + D3DX_PI) / 5;

        D3DXVECTOR3 rotate { 0.f, yaw, 0.f };
        m_player->SetRotate(rotate);
        m_player->SetWalk();
    }
    if (JoyStick::IsHold(eJoyStickButtonType::LEFT))
    {
        pos.x += -std::sin(radian + D3DX_PI) / 5;
        pos.z += std::sin(radian + D3DX_PI * 3 / 2) / 5;

        D3DXVECTOR3 rotate { 0.f, yaw + D3DX_PI * 3 / 2, 0.f };
        m_player->SetRotate(rotate);
        m_player->SetWalk();
    }
    if (JoyStick::IsHold(eJoyStickButtonType::DOWN))
    {
        pos.x += -std::sin(radian + D3DX_PI * 3 / 2) / 5;
        pos.z += std::sin(radian) / 5;

        D3DXVECTOR3 rotate { 0.f, yaw + D3DX_PI, 0.f };
        m_player->SetRotate(rotate);
        m_player->SetWalk();
    }
    if (JoyStick::IsHold(eJoyStickButtonType::RIGHT))
    {
        pos.x += -std::sin(radian) / 5;
        pos.z += std::sin(radian + D3DX_PI / 2) / 5;

        D3DXVECTOR3 rotate { 0.f, yaw + D3DX_PI / 2, 0.f };
        m_player->SetRotate(rotate);
        m_player->SetWalk();
    }
    if (JoyStick::IsDown(eJoyStickButtonType::R1))
    {
        InputR1();
    }
    if (JoyStick::IsDown(eJoyStickButtonType::B))
    {
        InputB(sequence);
    }
    if (JoyStick::IsDown(eJoyStickButtonType::A))
    {
        InputA();
    }

    m_player->SetPos(pos);
    Camera::SetPos(pos);
    m_player->Update();
    if (m_player->GetHP() <= 0)
    {
        m_player->SetDead();
        m_eState = eBattleState::GAMEOVER;
    }
    if (m_eState == eBattleState::GAMEOVER)
    {
        ++m_nGameoverCounter;
    }
    if (m_nCurrentStage == 1)
    {
        if (m_stage1->GetEnemy().size() == 0)
        {
            SAFE_DELETE(m_stage1);
            m_stage2 = new Stage2();
            m_stage2->Init();
            m_player->SetPos(D3DXVECTOR3 { 0.f, 0.f, 0.f });
            Camera::SetRadian(D3DX_PI);
            m_nCurrentStage = 2;
        }
    }
    else if (m_nCurrentStage == 2)
    {
        if (m_stage2->GetEnemy().size() == 0)
        {
            SAFE_DELETE(m_stage2);
            m_stage3 = new Stage3();
            m_stage3->Init();
            m_player->SetPos(D3DXVECTOR3 { 0.f, 0.f, 0.f });
            Camera::SetRadian(D3DX_PI);
            m_nCurrentStage = 3;
        }
    }
    else if (m_nCurrentStage == 3)
    {
        if (m_stage3->GetEnemy().size() == 0)
        {
            SAFE_DELETE(m_stage3);
            *sequence = eSequence::ENDING;
        }
    }
}

void SeqBattle::Render()
{
    m_player->Render();
    D3DXVECTOR3 pos { 0.f, 0.f, 0.f };
    if (m_player->GetDead())
    {
        m_spriteGameover->Render(pos);
    }
    if (m_nCurrentStage == 1)
    {
        m_stage1->Render();
    }
    else if (m_nCurrentStage == 2)
    {
        m_stage2->Render();
    }
    else if (m_nCurrentStage == 3)
    {
        m_stage3->Render();
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
    std::vector<Enemy> vecEnemy;
    if (m_nCurrentStage == 1)
    {
        vecEnemy = m_stage1->GetEnemy();
    }
    else if (m_nCurrentStage == 2)
    {
        vecEnemy = m_stage2->GetEnemy();
    }
    else if (m_nCurrentStage == 3)
    {
        vecEnemy = m_stage3->GetEnemy();
    }
    for (std::size_t i = 0; i < vecEnemy.size(); i++)
    {
        D3DXVECTOR3 enemyPos { 0.f, 0.f, 0.f };
        enemyPos = vecEnemy.at(i).GetPos();

        D3DXVECTOR3 subPos { attackPos - enemyPos };
        FLOAT distance = D3DXVec3Length(&subPos);

        if (distance <= 1.5f)
        {
            vecEnemy.at(i).SetState(eState::DAMAGED);
            int hp = vecEnemy.at(i).GetHP();
            vecEnemy.at(i).SetHP(hp - 50);
        }
    }
    if (m_nCurrentStage == 1)
    {
        m_stage1->SetEnemy(vecEnemy);
    }
    else if (m_nCurrentStage == 2)
    {
        m_stage2->SetEnemy(vecEnemy);
    }
    else if (m_nCurrentStage == 3)
    {
        m_stage3->SetEnemy(vecEnemy);
    }
}

void SeqBattle::InputA()
{
    m_player->SetJump();
}

void SeqBattle::InputB(eSequence* sequence)
{
    if (m_eState == eBattleState::GAMEOVER)
    {
        if (m_nGameoverCounter >= 60)
        {
            *sequence = eSequence::TITLE;
        }
    }
}

