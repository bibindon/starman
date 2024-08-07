#include "SeqBattle.h"
#include "Common.h"
#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif
#include <dinput.h>
#include "KeyBoard.h"

SeqBattle::SeqBattle()
{
    D3DXVECTOR3 b = D3DXVECTOR3(0, 0, 0);
    m_mesh1 = new Mesh("res\\model\\cube2\\cube2.x", b, b, 5.0f);
    m_AnimMesh1 = new AnimMesh("res\\model\\RobotArm\\RobotArm.x", b, b, 10.0f);
}

SeqBattle::~SeqBattle()
{
}

void SeqBattle::Update(eSequence* sequence)
{
    if (KeyBoard::IsDown(DIK_W))
    {
    }
    else if (KeyBoard::IsDown(DIK_A))
    {
    }
    else if (KeyBoard::IsDown(DIK_S))
    {
    }
    else if (KeyBoard::IsDown(DIK_D))
    {
    }
}

void SeqBattle::Render()
{
    m_mesh1->Render();
    m_AnimMesh1->Render();
}
