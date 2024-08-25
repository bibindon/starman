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
    else if (m_nCurrentStage == 4)
    {
        m_stage4->Update();
    }
    else if (m_nCurrentStage == 5)
    {
        m_stage5->Update();
    }
    else if (m_nCurrentStage == 6)
    {
        m_stage6->Update();
    }
    else if (m_nCurrentStage == 7)
    {
        m_stage7->Update();
    }
    else if (m_nCurrentStage == 8)
    {
        m_stage8->Update();
    }
    else if (m_nCurrentStage == 9)
    {
        m_stage9->Update();
    }
    else if (m_nCurrentStage == 10)
    {
        m_stage10->Update();
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

    if (JoyStick::IsLeftStickUsed())
    {
        float joyRadian = JoyStick::GetLeftRadian();
        float cameRadian = Camera::GetRadian();
        float radian = joyRadian + (cameRadian - D3DX_PI * 3 / 2);
        pos.x += std::cos(radian) / 5;
        pos.z += std::sin(radian) / 5;

        yaw = -1.f * (radian + D3DX_PI / 2);
        D3DXVECTOR3 rotate { 0.f, yaw, 0.f };
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
        if (m_nGameoverCounter >= 120)
        {
            *sequence = eSequence::TITLE;
        }
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
            m_stage4 = new Stage4();
            m_stage4->Init();
            m_player->SetPos(D3DXVECTOR3 { 0.f, 0.f, 0.f });
            Camera::SetRadian(D3DX_PI);
            m_nCurrentStage = 4;
        }
    }
    else if (m_nCurrentStage == 4)
    {
        if (m_stage4->GetEnemy().size() == 0)
        {
            SAFE_DELETE(m_stage4);
            m_stage5 = new Stage5();
            m_stage5->Init();
            m_player->SetPos(D3DXVECTOR3 { 0.f, 0.f, 0.f });
            Camera::SetRadian(D3DX_PI);
            m_nCurrentStage = 5;
        }
    }
    else if (m_nCurrentStage == 5)
    {
        if (m_stage5->GetEnemy().size() == 0)
        {
            SAFE_DELETE(m_stage5);
            m_stage6 = new Stage6();
            m_stage6->Init();
            m_player->SetPos(D3DXVECTOR3 { 0.f, 0.f, 0.f });
            Camera::SetRadian(D3DX_PI);
            m_nCurrentStage = 6;
        }
    }
    else if (m_nCurrentStage == 6)
    {
        if (m_stage6->GetEnemy().size() == 0)
        {
            SAFE_DELETE(m_stage6);
            m_stage7 = new Stage7();
            m_stage7->Init();
            m_player->SetPos(D3DXVECTOR3 { 0.f, 0.f, 0.f });
            Camera::SetRadian(D3DX_PI);
            m_nCurrentStage = 7;
        }
    }
    else if (m_nCurrentStage == 7)
    {
        if (m_stage7->GetEnemy().size() == 0)
        {
            SAFE_DELETE(m_stage7);
            m_stage8 = new Stage8();
            m_stage8->Init();
            m_player->SetPos(D3DXVECTOR3 { 0.f, 0.f, 0.f });
            Camera::SetRadian(D3DX_PI);
            m_nCurrentStage = 8;
        }
    }
    else if (m_nCurrentStage == 8)
    {
        if (m_stage8->GetEnemy().size() == 0)
        {
            SAFE_DELETE(m_stage8);
            m_stage9 = new Stage9();
            m_stage9->Init();
            m_player->SetPos(D3DXVECTOR3 { 0.f, 0.f, 0.f });
            Camera::SetRadian(D3DX_PI);
            m_nCurrentStage = 9;
        }
    }
    else if (m_nCurrentStage == 9)
    {
        if (m_stage9->GetEnemy().size() == 0)
        {
            SAFE_DELETE(m_stage9);
            m_stage10 = new Stage10();
            m_stage10->Init();
            m_player->SetPos(D3DXVECTOR3 { 0.f, 0.f, 0.f });
            Camera::SetRadian(D3DX_PI);
            m_nCurrentStage = 10;
        }
    }
    else if (m_nCurrentStage == 10)
    {
        if (m_stage10->GetEnemy().size() == 0)
        {
            SAFE_DELETE(m_stage10);
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
    else if (m_nCurrentStage == 4)
    {
        m_stage4->Render();
    }
    else if (m_nCurrentStage == 5)
    {
        m_stage5->Render();
    }
    else if (m_nCurrentStage == 6)
    {
        m_stage6->Render();
    }
    else if (m_nCurrentStage == 7)
    {
        m_stage7->Render();
    }
    else if (m_nCurrentStage == 8)
    {
        m_stage8->Render();
    }
    else if (m_nCurrentStage == 9)
    {
        m_stage9->Render();
    }
    else if (m_nCurrentStage == 10)
    {
        m_stage10->Render();
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
    std::vector<EnemySphere> vecEnemySphere;
    std::vector<EnemyDisk> vecEnemyDisk;
    if (m_nCurrentStage == 1)
    {
        vecEnemy = m_stage1->GetEnemy();
    }
    else if (m_nCurrentStage == 2)
    {
        vecEnemy = m_stage2->GetEnemy();
        vecEnemySphere = m_stage2->GetEnemySphere();
    }
    else if (m_nCurrentStage == 3)
    {
        vecEnemy = m_stage3->GetEnemy();
        vecEnemyDisk = m_stage3->GetEnemyDisk();
    }
    else if (m_nCurrentStage == 4)
    {
        vecEnemy = m_stage4->GetEnemy();
    }
    else if (m_nCurrentStage == 5)
    {
        vecEnemy = m_stage5->GetEnemy();
    }
    else if (m_nCurrentStage == 6)
    {
        vecEnemy = m_stage6->GetEnemy();
    }
    else if (m_nCurrentStage == 7)
    {
        vecEnemy = m_stage7->GetEnemy();
    }
    else if (m_nCurrentStage == 8)
    {
        vecEnemy = m_stage8->GetEnemy();
    }
    else if (m_nCurrentStage == 9)
    {
        vecEnemy = m_stage9->GetEnemy();
    }
    else if (m_nCurrentStage == 10)
    {
        vecEnemy = m_stage10->GetEnemy();
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
    for (std::size_t i = 0; i < vecEnemySphere.size(); i++)
    {
        D3DXVECTOR3 enemyPos { 0.f, 0.f, 0.f };
        enemyPos = vecEnemySphere.at(i).GetPos();

        D3DXVECTOR3 subPos { attackPos - enemyPos };
        FLOAT distance = D3DXVec3Length(&subPos);

        if (distance <= 1.5f)
        {
            vecEnemySphere.at(i).SetState(eSphereState::DAMAGED);
            int hp = vecEnemySphere.at(i).GetHP();
            vecEnemySphere.at(i).SetHP(hp - 40);
        }
    }
    for (std::size_t i = 0; i < vecEnemyDisk.size(); i++)
    {
        D3DXVECTOR3 enemyPos { 0.f, 0.f, 0.f };
        enemyPos = vecEnemyDisk.at(i).GetPos();

        D3DXVECTOR3 subPos { attackPos - enemyPos };
        FLOAT distance = D3DXVec3Length(&subPos);

        if (distance <= 1.5f)
        {
            vecEnemyDisk.at(i).SetState(eDiskState::DAMAGED);
            int hp = vecEnemyDisk.at(i).GetHP();
            vecEnemyDisk.at(i).SetHP(hp - 40);
        }
    }
    if (m_nCurrentStage == 1)
    {
        m_stage1->SetEnemy(vecEnemy);
    }
    else if (m_nCurrentStage == 2)
    {
        m_stage2->SetEnemy(vecEnemy);
        m_stage2->SetEnemySphere(vecEnemySphere);
    }
    else if (m_nCurrentStage == 3)
    {
        m_stage3->SetEnemy(vecEnemy);
        m_stage3->SetEnemyDisk(vecEnemyDisk);
    }
    else if (m_nCurrentStage == 4)
    {
        m_stage4->SetEnemy(vecEnemy);
    }
    else if (m_nCurrentStage == 5)
    {
        m_stage5->SetEnemy(vecEnemy);
    }
    else if (m_nCurrentStage == 6)
    {
        m_stage6->SetEnemy(vecEnemy);
    }
    else if (m_nCurrentStage == 7)
    {
        m_stage7->SetEnemy(vecEnemy);
    }
    else if (m_nCurrentStage == 8)
    {
        m_stage8->SetEnemy(vecEnemy);
    }
    else if (m_nCurrentStage == 9)
    {
        m_stage9->SetEnemy(vecEnemy);
    }
    else if (m_nCurrentStage == 10)
    {
        m_stage10->SetEnemy(vecEnemy);
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

