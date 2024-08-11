#include "SeqOpening.h"
#include "KeyBoard.h"
#include "Mouse.h"
#include "JoyStick.h"
#include "Common.h"
#include "SharedObj.h"

SeqOpening::SeqOpening()
{
    m_sprite1 = new Sprite("res\\image\\opening01.png");
    m_sprite1->SetFill(true);

    m_sprite2 = new Sprite("res\\image\\opening02.png");
    m_sprite2->SetFill(true);

    m_sprite3 = new Sprite("res\\image\\opening03.png");
    m_sprite3->SetFill(true);
}

SeqOpening::~SeqOpening()
{
    SAFE_DELETE(m_sprite1);
    SAFE_DELETE(m_sprite2);
    SAFE_DELETE(m_sprite3);
}

void SeqOpening::Update(eSequence* sequence)
{
    if (KeyBoard::IsDown(DIK_RETURN))
    {
        if (m_nIndex == 0)
        {
            m_nIndex = 1;
        }
        else if (m_nIndex == 1)
        {
            m_nIndex = 2;
        }
        else if (m_nIndex == 2)
        {
            *sequence = eSequence::BATTLE;
        }
    }

}

void SeqOpening::Render()
{
    D3DXVECTOR3 pos { 0.0f, 0.0f, 0.0f };
    if (m_nIndex == 0)
    {
        m_sprite1->Render(pos);
    }
    else if (m_nIndex == 1)
    {
        m_sprite2->Render(pos);
    }
    else if (m_nIndex == 2)
    {
        m_sprite3->Render(pos);
    }
}
