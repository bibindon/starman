// TODO カメラ移動時、ちょっとビクンと動く。

#include "Title.h"
#include "KeyBoard.h"
#include "Mouse.h"
#include "GamePad.h"
#include "Common.h"
#include "SharedObj.h"
#include "SoundEffect.h"
#include "BGM.h"
#include "SaveManager.h"
#include <Shlwapi.h>
#include "Camera.h"

Title::Title(const bool blackFadeIn)
{
    {
        std::vector<std::string> vs;
        std::vector<bool> vb;

        vs.push_back("Start");
        vb.push_back(true);

        vs.push_back("Continue");

        // セーブデータがあるか否か
        BOOL saveExist = 0;
        if (Common::DebugMode())
        {
            saveExist = PathFileExists("res\\script\\save_debug");
        }
        else
        {
            saveExist = PathFileExists("res\\script\\save");
        }
        if (saveExist == TRUE)
        {
            m_bSavedataExists = true;
            vb.push_back(true);
        }
        else
        {
            m_bSavedataExists = false;
            vb.push_back(false);
        }

        vs.push_back("Exit");
        vb.push_back(true);

        m_titleCommand = new CommandManager();
        m_titleCommand->Init(vs, vb);
    }

    m_sprBack = new Sprite("res\\image\\black.png");
    m_sprLogo = new Sprite("res\\image\\title01.png");
    m_sprClock = new Sprite("res\\image\\load_clock.png");
    m_sprLoading = new Sprite("res\\image\\loading.png");

    Common::SetCursorVisibility(true);

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

    if (blackFadeIn)
    {
        m_bFadeIn = true;
        m_bCameraFadeIn = false;

        m_fadeInAlpha = 255;
    }
    else
    {
        m_bFadeIn = false;
        m_bCameraFadeIn = true;

        m_fadeInAlpha = 0;
    }
}

Title::~Title()
{
    SAFE_DELETE(m_titleCommand);
    SAFE_DELETE(m_sprBack);
    SAFE_DELETE(m_sprLogo);
    SAFE_DELETE(m_sprClock);
    SAFE_DELETE(m_sprLoading);
    SAFE_RELEASE(m_font);
}

void Title::Update(eSequence* sequence, eBattleState* eState)
{
    if (m_bFadeIn)
    {
        ++m_fadeInCount;
        m_fadeInAlpha = 255 - (m_fadeInCount * 255 / 60);
        if (m_fadeInCount >= FADE_IN)
        {
            m_bFadeIn = false;
            m_fadeInAlpha = 0;
        }
    }
    else if (m_bFadeOut)
    {
        ++m_fadeOutCount;
        m_fadeOutAlpha = m_fadeOutCount * 255 / 60;
        if (m_fadeOutCount >= FADE_OUT)
        {
            m_bFadeOut = false;
            m_fadeOutAlpha = 255;
            if (m_bLoading == false)
            {
                *eState = eBattleState::OPENING;
            }
        }
    }
    else if (m_bCameraFadeIn)
    {
        ++m_cameraFadeInCount;
        if (m_cameraFadeInCount >= Camera::MOVE_COUNT_MAX)
        {
            m_bCameraFadeIn = false;
            Camera::SetCameraMode(eCameraMode::TITLE);
        }
    }
    else if (m_bCameraFadeOut)
    {
        ++m_cameraFadeOutCount;
        if (m_cameraFadeOutCount >= Camera::MOVE_COUNT_MAX)
        {
            m_bCameraFadeOut = false;
            Camera::SetCameraMode(eCameraMode::TITLE_TO_BATTLE);
            *eState = eBattleState::NORMAL;
        }
    }
    else if (m_bLoading == true)
    {
        if (m_loaded.load() == true)
        {
            m_bLoading = false;
            *eState = eBattleState::OPENING;
        }
    }
    else
    {
        std::string result = m_titleCommand->Operate();

        if (result == "Start")
        {
            // 即座にオープニングが始まるのではなく、
            // フェードアウトを描画し、
            // フェードアウトが完了したらオープニングが始まるようにする。
            m_eMenu = eTitleMenu::START;
            m_bFadeOut = true;
            m_fadeOutCount = 0;
            Common::SetCursorVisibility(false);

            if (m_bSavedataExists)
            {
                m_bLoading = true;
                m_thread = new std::thread(
                    [&]
                    {
                        SaveManager::Get()->LoadOrigin();
                        m_loaded.store(true);
                    });
            }
        }
        else if (result == "Continue")
        {
            m_eMenu = eTitleMenu::CONTINUE;
            m_bCameraFadeOut = true;
            m_cameraFadeOutCount = 0;
            Common::SetCursorVisibility(false);

            Camera::SetCameraMode(eCameraMode::TITLE_TO_BATTLE);
            auto ppos = SharedObj::GetPlayer()->GetPos();
            ppos.y += 1.0f;
            Camera::SetLookAtPos(ppos);
        }
        else if (result == "Exit")
        {
            m_eMenu = eTitleMenu::EXIT;
            *sequence = eSequence::EXIT;
        }
    }
}

void Title::Render()
{
    // 1フレーム目は、Update関数が呼ばれる前に呼ばれることに注意する

    D3DXVECTOR3 pos(0.f, 0.f, 0.f);

    if (m_bFadeIn)
    {
        m_sprBack->Render(pos, m_fadeInAlpha);
    }
    else if (m_bFadeOut)
    {
        m_sprBack->Render(pos, m_fadeOutAlpha);
    }
    else if (m_bLoading)
    {
        m_sprBack->Render(pos, m_fadeOutAlpha);
    }

    pos.x = 630.f;
    pos.y = 200.f;

    if (m_bLoading == false && m_bCameraFadeOut == false)
    {
        m_sprLogo->Render(pos);
    }

    if (m_bFadeIn == false &&
        m_bFadeOut == false &&
        m_bCameraFadeIn == false &&
        m_bCameraFadeOut == false &&
        m_bLoading == false)
    {
        m_titleCommand->Draw();
    }

    // ロード中ならくるくるを表示
    if (m_bLoading && m_loaded.load() == false)
    {

        static float counter = 0;
        counter += 0.05f;
        int temp1 = 0;

        D3DXVECTOR3 pos { 0.0f, 0.0f, 0.0f };
        pos.x = std::sin(counter) * 100 + 800;
        pos.y = std::cos(counter) * 60 + 450;
        temp1 = (int)((std::sin(counter * 0.2) + 1) * 64);
        m_sprClock->Render(pos, temp1);

        static float counter2 = 0;
        counter2 += 0.04f;

        pos.x = std::sin(counter2) * 60 + 800;
        pos.y = std::cos(counter2) * 90 + 450;
        temp1 = (int)((std::sin(counter2 * 0.2) + 1) * 128);
        m_sprClock->Render(pos, temp1);

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

        // 右方向に8ピクセル、下方向に31ピクセル移動すればクライアント領域？
        // 本当はちゃんとやらないといけない。

        pos = D3DXVECTOR3(800 - 40, 450 - 10, 0.0f);
        m_sprLoading->Render(pos, temp);
    }
}

bool Title::GetLoading() const
{
    return m_bLoading;
}
