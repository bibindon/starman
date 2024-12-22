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

    m_spriteTimer = new Sprite("res\\image\\command_cursor.png");

    // セーブデータがあるか否か
    int saveExist = 0;
    if (Common::DebugMode())
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

    Common::SetCursorVisibility(true);

    BGM::get_ton()->load("res\\sound\\title.wav");
    BGM::get_ton()->play(10);

    HRESULT ret = D3DXCreateFont(SharedObj::GetD3DDevice(),
                                 20,
                                 0,
                                 FW_NORMAL,
                                 1,
                                 false,
                                 SHIFTJIS_CHARSET,
                                 OUT_TT_ONLY_PRECIS,
                                 ANTIALIASED_QUALITY,
                                 FF_DONTCARE,
                                 "ＭＳ 明朝",
                                 &m_font);
}

SeqTitle::~SeqTitle()
{
    SAFE_DELETE(m_sprite1);
    SAFE_DELETE(m_sprite3);
    SAFE_DELETE(m_spriteBlack);
    SAFE_DELETE(m_spriteTimer);
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
            m_thread = new std::thread(
                [&]
                {
                    SaveManager::Get()->LoadOrigin();
                    m_loaded.store(true);
                });
        }
        else if (result == "Continue")
        {
            m_eMenu = eMenu::CONTINUE;
            m_thread = new std::thread(
                [&]
                {
                    SaveManager::Get()->Load();
                    m_loaded.store(true);
                });
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

    if (m_loaded.load() == true)
    {
        m_bFadeOut = true;
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

    if (m_thread != nullptr && m_loaded.load() == false)
    {

        static float counter = 0;
        counter += 0.05f;

        D3DXVECTOR3 pos { 0.0f, 0.0f, 0.0f };
        pos.x = std::sin(counter) * 100 + 800;
        pos.y = std::cos(counter) * 60 + 450;
        m_spriteTimer->Render(pos);

        static float counter2 = 0;
        counter2 += 0.04f;

        pos.x = std::sin(counter2) * 60 + 800;
        pos.y = std::cos(counter2) * 90 + 450;
        m_spriteTimer->Render(pos);

        static int counter3 = 0;

        counter3 += 2;
        if (counter3 >= 256)
        {
            counter3 = 0;
        }

        int temp = 0;
        if (counter3 <= 127)
        {
            temp = counter3;
        }
        else
        {
            temp = 256 - counter3;
        }

        // 右方向に8ピクセル、下方向に31ピクセル移動すればクライアント領域
        // 本当はちゃんとやらないといけない。

        RECT rect { };
        rect.left = 800 - 100;
        rect.top = 450 - 100;
        rect.right = 800 + 100;
        rect.bottom = 450 + 100;

        rect.left += 8;
//        rect.top += 31;
        rect.right += 8;
//        rect.bottom += 31;

        m_font->DrawText(NULL,
                         "Loading...",
                         -1,
                         &rect,
                         DT_CENTER | DT_VCENTER,
                         D3DCOLOR_ARGB(temp, 255, 255, 255));
    }
}
