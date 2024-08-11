#include "SeqTitle.h"
#include "KeyBoard.h"
#include "Mouse.h"
#include "JoyStick.h"
#include "Common.h"
#include "SharedObj.h"

SeqTitle::SeqTitle()
{
    m_sprite1 = new Sprite("res\\image\\title01.png");
    m_sprite2 = new Sprite("res\\image\\title02.png");
    m_sprite3 = new Sprite("res\\image\\title03.png");
    m_sprite3->SetFill(true);
    m_spriteCursor = new Sprite("res\\image\\cursor.png");
}

SeqTitle::~SeqTitle()
{
    SAFE_DELETE(m_sprite1);
    SAFE_DELETE(m_sprite2);
    SAFE_DELETE(m_sprite3);
    SAFE_DELETE(m_spriteCursor);
}

void SeqTitle::Update(eSequence* sequence)
{
    if (KeyBoard::IsDown(DIK_UP))
    {
        switch (m_eMenu)
        {
        case eMenu::START:
        {
            // do nothing
            break;
        }
        case eMenu::CONTINUE:
        {
            m_eMenu = eMenu::START;
            break;
        }
        case eMenu::EXIT:
        {
            m_eMenu = eMenu::CONTINUE;
            break;
        }
        }
    }
    else if (KeyBoard::IsDown(DIK_DOWN))
    {
        switch (m_eMenu)
        {
        case eMenu::START:
        {
            m_eMenu = eMenu::CONTINUE;
            break;
        }
        case eMenu::CONTINUE:
        {
            m_eMenu = eMenu::EXIT;
            break;
        }
        case eMenu::EXIT:
        {
            // do nothing
            break;
        }
        }
    }
    else if (KeyBoard::IsDown(DIK_RETURN))
    {
        switch (m_eMenu)
        {
        case eMenu::START:
        {
            *sequence = eSequence::OPENING;
            break;
        }
        case eMenu::CONTINUE:
        {
            *sequence = eSequence::BATTLE;
            break;
        }
        case eMenu::EXIT:
        {
            *sequence = eSequence::EXIT;
            break;
        }
        }
    }
    if (JoyStick::IsDown(eJoyStickButtonType::UP))
    {

    }
    else if (JoyStick::IsDown(eJoyStickButtonType::DOWN))
    {

    }
    else if (JoyStick::IsDown(eJoyStickButtonType::A))
    {
        switch (m_eMenu)
        {
        case eMenu::START:
        {
            *sequence = eSequence::OPENING;
            break;
        }
        case eMenu::CONTINUE:
        {
            *sequence = eSequence::BATTLE;
            break;
        }
        case eMenu::EXIT:
        {
            *sequence = eSequence::EXIT;
            break;
        }
        }
    }
}

void SeqTitle::Render()
{
    D3DXVECTOR3 pos4 { 0.0f, 0.0f, 0.0f };
    m_sprite3->Render(pos4);

    D3DXVECTOR3 pos1 { 0.0f, 0.0f, 0.0f };
    m_sprite1->Render(pos1);

    D3DXVECTOR3 pos2 { 800.0f, 200.0f, 0.0f };
    m_sprite2->Render(pos2);

    D3DXVECTOR3 pos3 { 900.0f, 220.0f, 0.0f };
    switch (m_eMenu)
    {
    case eMenu::START:
    {
        pos3.y = 220.f;
        break;
    }
    case eMenu::CONTINUE:
    {
        pos3.y = 280.f;
        break;
    }
    case eMenu::EXIT:
    {
        pos3.y = 340.f;
        break;
    }
    }
    m_spriteCursor->Render(pos3);
}
