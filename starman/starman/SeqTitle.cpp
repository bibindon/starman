#include "SeqTitle.h"
#include "KeyBoard.h"
#include "Mouse.h"
#include "JoyStick.h"
#include "Common.h"

SeqTitle::SeqTitle(LPDIRECT3DDEVICE9 D3DDevice)
{
    m_D3DDevice = D3DDevice;
    m_sprite1 = new Sprite(m_D3DDevice, "res\\image\\title01.png");
    m_sprite2 = new Sprite(m_D3DDevice, "res\\image\\title02.png");
    m_spriteCursor = new Sprite(m_D3DDevice, "res\\image\\cursor.png");
}

SeqTitle::~SeqTitle()
{
    SAFE_DELETE(m_sprite1);
    SAFE_DELETE(m_sprite2);
    SAFE_DELETE(m_spriteCursor);
}

void SeqTitle::Update()
{
    if (KeyBoard::IsDown(DIK_UP))
    {

    }
    else if (KeyBoard::IsDown(DIK_DOWN))
    {

    }
    else if (KeyBoard::IsDown(DIK_RETURN))
    {

    }
    if (JoyStick::IsDown(eJoyStickButtonType::UP))
    {

    }
    else if (JoyStick::IsDown(eJoyStickButtonType::DOWN))
    {

    }
    else if (JoyStick::IsDown(eJoyStickButtonType::A))
    {

    }
}

void SeqTitle::Render()
{
    D3DXVECTOR3 pos { 0.0f, 0.0f, 0.0f };
    m_sprite1->Render(pos);
    m_sprite2->Render(pos);
    m_spriteCursor->Render(pos);
}
