#include "SeqTitle.h"
#include "KeyBoard.h"
#include "Mouse.h"
#include "GamePad.h"
#include "Common.h"
#include "SharedObj.h"
#include "SoundEffect.h"
#include "BGM.h"
#include "SaveManager.h"

SeqTitle::SeqTitle()
{
    m_sprite1 = new Sprite("res\\image\\title01.png");
    m_sprite2 = new Sprite("res\\image\\title02.png");

    m_sprite3 = new Sprite("res\\image\\title03.png");
    m_sprite3->SetFill(true);

    m_spriteBlack = new Sprite("res\\image\\black.png");
    m_spriteBlack->SetFill(true);

    m_spriteCursor = new Sprite("res\\image\\cursor.png");

    SoundEffect::get_ton()->load("res\\sound\\cursor_move.wav");
    SoundEffect::get_ton()->load("res\\sound\\cursor_confirm.wav");
    BGM::get_ton()->load("res\\sound\\title.wav");
    BGM::get_ton()->play(10);
}

SeqTitle::~SeqTitle()
{
    SAFE_DELETE(m_sprite1);
    SAFE_DELETE(m_sprite2);
    SAFE_DELETE(m_sprite3);
    SAFE_DELETE(m_spriteBlack);
    SAFE_DELETE(m_spriteCursor);
}

void SeqTitle::Update(eSequence* sequence)
{
    if (m_bFadeIn == false && m_bFadeOut == false)
    {
        if (KeyBoard::IsDownFirstFrame(DIK_UP))
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
                SoundEffect::get_ton()->play("res\\sound\\cursor_move.wav", 90);
                break;
            }
            case eMenu::EXIT:
            {
                m_eMenu = eMenu::CONTINUE;
                SoundEffect::get_ton()->play("res\\sound\\cursor_move.wav", 90);
                break;
            }
            }
        }
        else if (KeyBoard::IsDownFirstFrame(DIK_DOWN))
        {
            switch (m_eMenu)
            {
            case eMenu::START:
            {
                m_eMenu = eMenu::CONTINUE;
                SoundEffect::get_ton()->play("res\\sound\\cursor_move.wav", 90);
                break;
            }
            case eMenu::CONTINUE:
            {
                m_eMenu = eMenu::EXIT;
                SoundEffect::get_ton()->play("res\\sound\\cursor_move.wav", 90);
                break;
            }
            case eMenu::EXIT:
            {
                // do nothing
                break;
            }
            }
        }
        else if (KeyBoard::IsDownFirstFrame(DIK_RETURN))
        {
            m_bFadeOut = true;
            SoundEffect::get_ton()->play("res\\sound\\cursor_confirm.wav", 90);
        }
        if (GamePad::IsDown(eGamePadButtonType::UP))
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
                SoundEffect::get_ton()->play("res\\sound\\cursor_move.wav", 90);
                break;
            }
            case eMenu::EXIT:
            {
                m_eMenu = eMenu::CONTINUE;
                SoundEffect::get_ton()->play("res\\sound\\cursor_move.wav", 90);
                break;
            }
            }
        }
        else if (GamePad::IsDown(eGamePadButtonType::DOWN))
        {
            switch (m_eMenu)
            {
            case eMenu::START:
            {
                m_eMenu = eMenu::CONTINUE;
                SoundEffect::get_ton()->play("res\\sound\\cursor_move.wav", 90);
                break;
            }
            case eMenu::CONTINUE:
            {
                m_eMenu = eMenu::EXIT;
                SoundEffect::get_ton()->play("res\\sound\\cursor_move.wav", 90);
                break;
            }
            case eMenu::EXIT:
            {
                // do nothing
                break;
            }
            }
        }
        else if (GamePad::IsDown(eGamePadButtonType::A))
        {
            m_bFadeOut = true;
            SoundEffect::get_ton()->play("res\\sound\\cursor_confirm.wav", 90);
        }
    }
    if (m_bFadeIn)
    {
        ++m_fadeInCount;
        if (m_fadeInCount >= FADE_IN_COUNT)
        {
            m_fadeInCount = FADE_IN_COUNT;
            m_bFadeIn = false;
        }
    }
    if (m_bFadeOut)
    {
        ++m_fadeOutCount;
        if (m_fadeOutCount >= FADE_OUT_COUNT)
        {
            m_fadeOutCount = FADE_OUT_COUNT;
            m_bFadeOut = false;
            switch (m_eMenu)
            {
            case eMenu::START:
            {
                *sequence = eSequence::OPENING;
                SaveManager::Get()->LoadOrigin();
                break;
            }
            case eMenu::CONTINUE:
            {
                *sequence = eSequence::BATTLE;
                SaveManager::Get()->Load();
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

    if (m_bFadeIn)
    {
        D3DXVECTOR3 pos { 0.0f, 0.0f, 0.0f };
        m_spriteBlack->Render(pos, 255 - (int)(255.f * m_fadeInCount / FADE_IN_COUNT));
    }
    if (m_bFadeOut)
    {
        D3DXVECTOR3 pos { 0.0f, 0.0f, 0.0f };
        m_spriteBlack->Render(pos, (int)(255.f * m_fadeOutCount / FADE_OUT_COUNT));
    }
}
