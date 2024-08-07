#include "SeqBattle.h"
#include "Common.h"
#include <dinput.h>
#include "KeyBoard.h"

SeqBattle::SeqBattle(LPDIRECT3DDEVICE9 D3DDevice)
    : m_D3DDevice { D3DDevice }
{
    D3DXVECTOR3 b = D3DXVECTOR3(0, 0, 0);
    m_mesh1 = new Mesh(m_D3DDevice, "res\\model\\cube2\\cube2.x", b, b, 5.0f);
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


void SeqBattle::Render(const D3DXMATRIX& viewMatrix, const D3DXMATRIX& projMatrix)
{
    m_mesh1->Render(viewMatrix, projMatrix);
}
