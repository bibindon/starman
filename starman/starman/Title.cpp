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

    m_fadeInCount = 0;
    m_fadeInAlpha = 255;

    m_bFadeIn = true;

    m_eMenu = eTitleMenu::NOT_DECIDE;
    m_cameraFadeCount = 0;
    m_bCameraFadeOut = false;
    m_bLoading = false;
    m_bFadeOut = false;
    m_fadeOutCount = 0;

    if (blackFadeIn)
    {
        m_bCameraFadeIn = false;
    }
    else
    {
        m_bCameraFadeIn = true;
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
        if (m_bCameraFadeIn == false)
        {
            ++m_fadeInCount;
            m_fadeInAlpha = 255 - (m_fadeInCount * 255 / 60);
            if (m_fadeInCount >= FADE_IN)
            {
                m_bFadeIn = false;
                m_fadeInAlpha = 0;
            }
        }
        else
        {
            ++m_fadeInCount;
            if (m_fadeInCount >= Camera::MOVE_COUNT_MAX)
            {
                m_bFadeIn = false;
                Camera::SetCameraMode(eCameraMode::TITLE);
            }
        }
    }
    else
    {
        // ローディング中、カメラ移動中、フェードアウト中は操作不能にする
        if (m_bLoading == false && m_bCameraFadeOut == false && m_bFadeOut == false)
        {
            std::string result = m_titleCommand->Operate();

            if (result == "Start")
            {
                m_eMenu = eTitleMenu::START;
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
                else
                {
                    *eState = eBattleState::OPENING;
                }
            }
            else if (result == "Continue")
            {
                m_eMenu = eTitleMenu::CONTINUE;
                m_bCameraFadeOut = true;
                Camera::SetCameraMode(eCameraMode::TITLE_TO_BATTLE);
                m_cameraFadeCount = 0;
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
        else
        {
            if (m_bCameraFadeOut)
            {
                // カメラをプレイヤーの位置に向かって60フレームで到達するように移動させる。
                ++m_cameraFadeCount;
                if (m_cameraFadeCount >= Camera::MOVE_COUNT_MAX)
                {
                    *eState = eBattleState::NORMAL;
                }
            }
            else if (m_bFadeOut)
            {
                ++m_fadeOutCount;
                if (m_fadeOutCount >= FADE_OUT)
                {
                    *eState = eBattleState::OPENING;
                }
            }
            else if (m_bLoading)
            {
                // 即座にオープニングが始まるのではなく、
                // フェードアウトを描画し、
                // フェードアウトが完了したらオープニングが始まるようにする。
                if (m_loaded.load() == true)
                {
                    m_bFadeOut = true;
                    m_fadeOutCount = 0;
                    m_bLoading = false;
                }
            }
        }
    }
}

void Title::Render()
{
    // 1フレーム目は、Update関数が呼ばれる前に呼ばれることに注意する

    D3DXVECTOR3 pos(0.f, 0.f, 0.f);

    if (m_bFadeIn && (m_bCameraFadeIn == false))
    {
        m_sprBack->Render(pos, m_fadeInAlpha);
    }

    pos.x = 630.f;
    pos.y = 200.f;

    m_sprLogo->Render(pos);

    if (m_bFadeIn == false && m_bFadeOut == false && m_bCameraFadeOut == false)
    {
        m_titleCommand->Draw();
    }

    // ロードするならくるくるを表示
    if (m_thread != nullptr && m_loaded.load() == false)
    {

        static float counter = 0;
        counter += 0.05f;
        int temp1 = 0;

        D3DXVECTOR3 pos { 0.0f, 0.0f, 0.0f };
        pos.x = std::sin(counter) * 100 + 800;
        pos.y = std::cos(counter) * 60 + 450;
        temp1 = (int)std::sin(counter * 0.2) * 255;
        m_sprClock->Render(pos, temp1);

        static float counter2 = 0;
        counter2 += 0.04f;

        pos.x = std::sin(counter2) * 60 + 800;
        pos.y = std::cos(counter2) * 90 + 450;
        temp1 = (int)std::sin(counter2 * 0.2) * 192;
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

        pos = D3DXVECTOR3(800 - 64, 450 - 64, 0.0f);
        m_sprLoading->Render(pos, temp);
    }
}

bool Title::GetLoading() const
{
    return m_bLoading;
}
