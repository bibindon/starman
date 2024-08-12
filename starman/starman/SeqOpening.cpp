#include "SeqOpening.h"
#include "KeyBoard.h"
#include "Mouse.h"
#include "JoyStick.h"
#include "Common.h"
#include "SharedObj.h"
#include "BGM.h"

SeqOpening::SeqOpening()
{
    m_sprite1 = new Sprite("res\\image\\opening01.png");
    m_sprite2 = new Sprite("res\\image\\opening02.png");
    m_sprite3 = new Sprite("res\\image\\opening03.png");
    m_spriteBlack = new Sprite("res\\image\\black.png");

    BGM::get_ton()->load("res\\sound\\novel.wav");
    BGM::get_ton()->play(10);
}

SeqOpening::~SeqOpening()
{
    SAFE_DELETE(m_sprite1);
    SAFE_DELETE(m_sprite2);
    SAFE_DELETE(m_sprite3);
    SAFE_DELETE(m_spriteBlack);
}

void SeqOpening::Update(eSequence* sequence)
{
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
            if (m_nIndex == 2)
            {
                *sequence = eSequence::BATTLE;
            }
            else
            {
                m_bFadeIn = true;
                ++m_nIndex;
                m_fadeInCount = 0;
            }
        }
    }
    if (m_bFadeIn || m_bFadeOut)
    {
        return;
    }
    if (KeyBoard::IsDown(DIK_RETURN))
    {
        if (m_nIndex == 0)
        {
            m_bFadeOut = true;
            m_fadeOutCount = 0;
        }
        else if (m_nIndex == 1)
        {
            m_bFadeOut = true;
            m_fadeOutCount = 0;
        }
        else if (m_nIndex == 2)
        {
            m_bFadeOut = true;
            m_fadeOutCount = 0;
        }
    }
    if (JoyStick::IsDown(eJoyStickButtonType::A))
    {
        if (m_nIndex == 0)
        {
            m_bFadeOut = true;
            m_fadeOutCount = 0;
        }
        else if (m_nIndex == 1)
        {
            m_bFadeOut = true;
            m_fadeOutCount = 0;
        }
        else if (m_nIndex == 2)
        {
            m_bFadeOut = true;
            m_fadeOutCount = 0;
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
