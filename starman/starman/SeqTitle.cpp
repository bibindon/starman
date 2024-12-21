#include "SeqTitle.h"
#include "KeyBoard.h"
#include "Mouse.h"
#include "GamePad.h"
#include "Common.h"
#include "SharedObj.h"
#include "SoundEffect.h"
#include "BGM.h"
#include "SaveManager.h"
#include <Shlwapi.h>

SeqTitle::SeqTitle()
{
    m_sprite1 = new Sprite("res\\image\\title01.png");

    m_sprite3 = new Sprite("res\\image\\title03.png");
    m_sprite3->SetFill(true);

    m_spriteBlack = new Sprite("res\\image\\black.png");
    m_spriteBlack->SetFill(true);

    // セーブデータがあるか否か
    int saveExist = 0;
    if (SharedObj::DebugMode())
    {
        saveExist = PathFileExists("res\\script\\save_debug");
    }
    else
    {
        saveExist = PathFileExists("res\\script\\save");
    }



    {
        std::vector<std::string> vs;
        std::vector<bool> vb;

        vs.push_back("Start");
        vb.push_back(true);

        vs.push_back("Continue");
        if (saveExist == 1)
        {
            vb.push_back(true);
        }
        else
        {
            vb.push_back(false);
        }

        vs.push_back("Exit");
        vb.push_back(true);

        m_commandManager.Init(vs, vb);
    }

    ShowCursor(true);
    ClipCursor(NULL);
}

SeqTitle::~SeqTitle()
{
    SAFE_DELETE(m_sprite1);
    SAFE_DELETE(m_sprite3);
    SAFE_DELETE(m_spriteBlack);
}

void SeqTitle::Update(eSequence* sequence)
{
    if (m_bFadeIn == false && m_bFadeOut == false)
    {
        std::string result = m_commandManager.Operate();

        if (result == "Start")
        {
            m_eMenu = eMenu::START;
            m_bFadeOut = true;
        }
        else if (result == "Continue")
        {
            m_eMenu = eMenu::CONTINUE;
            m_bFadeOut = true;
        }
        else if (result == "Exit")
        {
            m_eMenu = eMenu::EXIT;
            m_bFadeOut = true;
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

    D3DXVECTOR3 pos1 { 500.0f, 100.0f, 0.0f };
    m_sprite1->Render(pos1);

    m_commandManager.Draw();

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
