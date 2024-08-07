#include "SeqBattle.h"
#include "Common.h"
#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif
#include <dinput.h>
#include "KeyBoard.h"
#include "Camera.h"

SeqBattle::SeqBattle()
{
    D3DXVECTOR3 b = D3DXVECTOR3(0, -1, 0);
    D3DXVECTOR3 c = D3DXVECTOR3(0, 0, 0);
    m_mesh1 = new Mesh("res\\model\\cube2\\cube2.x", b, c, 1.0f);
    b.y = 0.f;
//    m_AnimMesh1 = new AnimMesh("res\\model\\RobotArm\\RobotArm.x", b, c, 1.0f);
    m_player = new Player();
}

SeqBattle::~SeqBattle()
{
}

void SeqBattle::Update(eSequence* sequence)
{
    D3DXVECTOR3 pos = m_player->GetPos();
    float radian = Camera::GetRadian();
    if (KeyBoard::IsHold(DIK_W))
    {
        pos.x -= std::sin(radian)/10;
        pos.z -= std::cos(radian)/10;
    }
    if (KeyBoard::IsHold(DIK_A))
    {
        pos.x -= std::sin(radian)/10;
        pos.z += std::cos(radian)/10;
    }
    if (KeyBoard::IsHold(DIK_S))
    {
        pos.x += std::sin(radian)/10;
        pos.z += std::cos(radian)/10;
    }
    if (KeyBoard::IsHold(DIK_D))
    {
        pos.x += std::sin(radian)/10;
        pos.z -= std::cos(radian)/10;
    }
    m_player->SetPos(pos);
    Camera::SetPos(pos);
}

void SeqBattle::Render()
{
    m_mesh1->Render();
    m_player->Render();
}
