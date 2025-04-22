
#include "MainWindow.h"

#include <exception>
#include <chrono>
#include <random>
#include <cassert>

#include "KeyBoard.h"
#include "Mouse.h"
#include "GamePad.h"
#include "BGM.h"
#include "SoundEffect.h"
#include "Common.h"
#include "Camera.h"
#include "SharedObj.h"
#include "PopUp.h"
#include "PopUp2.h"
#include "SaveManager.h"
#include "resource.h"
#include "QuestManager.h"

#include "../../StarmanLib/StarmanLib/StarmanLib/HumanInfoManager.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/MapInfoManager.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/ItemManager.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/Inventory.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/Storehouse.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/WeaponManager.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/EnemyInfoManager.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/SkillManager.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/StatusManager.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/Guide.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/PowereggDateTime.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/MapObjManager.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/Rynen.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/RainModel.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/CraftInfoManager.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/CraftSystem.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/Help.h"

SeqBattle* MainWindow::m_seqBattle = nullptr;

using std::chrono::system_clock;

static LRESULT CALLBACK WndProc(HWND hWnd, UINT mes, WPARAM wParam, LPARAM lParam)
{
    if (mes == WM_DESTROY)
    {
        PostQuitMessage(0);
        return 0;
    }
    // ウィンドウがアクティブor非アクティブになったときの処理
    // カメラとカーソルのモードを変える
    else if (mes == WM_ACTIVATE)
    {
        int lower = wParam & 0xFFFF;

        // アクティブになった時の処理
        if (lower == WA_ACTIVE || lower == WA_CLICKACTIVE)
        {
            auto seq = MainWindow::GetBattleSequence();
            if (seq != nullptr)
            {
                if (seq->GetState() == eBattleState::NORMAL)
                {
                    Camera::SetCameraMode(eCameraMode::BATTLE);
                    Common::SetCursorVisibility(false);
                }
                if (seq->GetState() == eBattleState::LOAD)
                {
                    Common::SetCursorVisibility(false);
                }
            }
        }
        // 非アクティブになった時の処理
        else if (lower == WA_INACTIVE)
        {
            Common::SetCursorVisibility(true);
        }
    }
    // ウィンドウを閉じる操作を行ったらセーブを自動実行する
    else if (mes == WM_CLOSE)
    {
        auto seq = MainWindow::GetBattleSequence();
        if (seq != nullptr)
        {
            if (seq->GetState() == eBattleState::NORMAL ||
                seq->GetState() == eBattleState::MENU)
            {
                if (Common::DeployMode() || Common::DeployEncryptMode())
                {
                    SaveManager::Get()->Save();
                }
            }
        }
    }

    return DefWindowProc(hWnd, mes, wParam, lParam);
}

MainWindow::MainWindow(const HINSTANCE& hInstance, IKeyBoard* keyboard)
{
    SharedObj::Init();

    //-------------------------------------------------
    // ウィンドウクラスの登録
    //-------------------------------------------------
    HICON hIcon = (HICON)LoadImage(hInstance,
                                   MAKEINTRESOURCE(IDI_ICON1),
                                   IMAGE_ICON,
                                   GetSystemMetrics(SM_CXICON),
                                   GetSystemMetrics(SM_CYICON),
                                   0);

    WNDCLASSEX wcex { };

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = hIcon;
    wcex.hCursor = nullptr;
    wcex.hbrBackground = GetSysColorBrush(COLOR_WINDOW);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = TITLE.c_str();
    wcex.hIconSm = nullptr;

    auto result = RegisterClassEx(&wcex);
    if (!result)
    {
        DWORD err = GetLastError();
        throw std::exception("RegisterClassEx関数の実行が失敗しました。");
    } 

    //-------------------------------------------------
    // ウィンドウの作成
    //-------------------------------------------------
    {
        //-------------------------------------------------
        // ウィンドウが画面中央に表示されるように設定
        //-------------------------------------------------
        RECT rect { };
        SetRect(&rect, 0, 0, 1600, 900);

        // タイトルバー、ウィンドウフレームの幅を考慮したサイズを取得
        AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

        rect.right = rect.right - rect.left;
        rect.bottom = rect.bottom - rect.top;

        int monitorWidth = GetSystemMetrics(SM_CXSCREEN);
        int monitorHeight = GetSystemMetrics(SM_CYSCREEN);

        int startX = (monitorWidth / 2) - (1600 / 2);
        int startY = (monitorHeight / 2) - (900 / 2);

        m_hWnd = CreateWindow(TITLE.c_str(),
                              TITLE.c_str(),
                              /* ウィンドウサイズの変更をさせない。最小化はOK */
                              WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME | WS_VISIBLE,
                              startX,
                              startY,
                              rect.right,
                              rect.bottom,
                              NULL,
                              NULL,
                              hInstance,
                              NULL);

        if (!m_hWnd)
        {
            throw std::exception("ウィンドウの作成ができませんでした");
        }

        SharedObj::SetWindowHandle(m_hWnd);
    }

    //-------------------------------------------------
    // Direct3Dの初期化
    //-------------------------------------------------
    {
        m_D3D = Direct3DCreate9(D3D_SDK_VERSION);

        if (!m_D3D)
        {
            throw std::exception("DirectXの初期化に失敗しました");
        }
    }

    HRESULT hResult = E_FAIL;

    //-------------------------------------------------
    // Direct3DDeviceの初期化
    //-------------------------------------------------
    {
        //-------------------------------------------------
        // DirectX9ではプログラムからGPUの切り替えができない。
        // 使用するGPUを変えたい場合は、ユーザーがWindowsの設定画面で設定するしかない。
        //-------------------------------------------------

        // グラフィックボードの名称を取得
        D3DADAPTER_IDENTIFIER9 adapterInfo { };
        m_D3D->GetAdapterIdentifier(D3DADAPTER_DEFAULT, 0, &adapterInfo);

        std::string GPUName = adapterInfo.Description;
        Common::SetGPUName(GPUName);

        // TODO フルスクリーン対応

        D3DPRESENT_PARAMETERS d3dpp { };
        d3dpp.BackBufferWidth = 0;
        d3dpp.BackBufferHeight = 0;
        d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
        d3dpp.BackBufferCount = 0;

        d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
        d3dpp.MultiSampleQuality = 0;

        d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
        d3dpp.hDeviceWindow = nullptr;
        d3dpp.Windowed = TRUE;
        d3dpp.EnableAutoDepthStencil = TRUE;
        d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
        d3dpp.Flags = 0;

        d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
        d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

        LPDIRECT3DDEVICE9 D3DDevice = nullptr;

        hResult = m_D3D->CreateDevice(D3DADAPTER_DEFAULT,
                                      D3DDEVTYPE_HAL,
                                      m_hWnd,
                                      D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
                                      &d3dpp,
                                      &D3DDevice);

        if (FAILED(hResult))
        {
            hResult = m_D3D->CreateDevice(D3DADAPTER_DEFAULT,
                                          D3DDEVTYPE_HAL,
                                          m_hWnd,
                                          D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
                                          &d3dpp,
                                          &D3DDevice);
            if (FAILED(hResult))
            {
                throw std::exception("Direct3D::CreateDevice関数が失敗しました");
            }
        }

        SharedObj::SetD3DDevice(D3DDevice);
    }

    //-------------------------------------------------
    // DirectInputの初期化
    //-------------------------------------------------
    {
        // directinput
        hResult = DirectInput8Create(hInstance,
                                     DIRECTINPUT_VERSION,
                                     IID_IDirectInput8,
                                     (LPVOID*)&m_directInput,
                                     NULL);

        if (FAILED(hResult))
        {
            throw std::exception("DirectInput8Create関数が失敗しました");
        }
    }

    //-------------------------------------------------
    // フォントの初期化
    //-------------------------------------------------
    {
        hResult = D3DXCreateFont(SharedObj::GetD3DDevice(),
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
                                 &m_D3DFont);

        assert(hResult == S_OK);
    }

    //-------------------------------------------------
    // キーボード、マウス、ゲームパッド、サウンドの初期化
    //-------------------------------------------------
    {
        keyboard->Init(m_directInput, m_hWnd);
        SharedObj::SetKeyBoard(keyboard);

        Mouse::Init(m_directInput, m_hWnd);
        GamePad::Init(m_directInput, m_hWnd);
        BGM::initialize(m_hWnd);
        SoundEffect::initialize(m_hWnd);
    }

    // PopUpクラスの初期化
    PopUp::Init();

    // PopUp2クラスの初期化
    IPopUpFont* popUpFont = NEW PopUpFont(SharedObj::GetD3DDevice());
    PopUp2::Init(popUpFont);

    //-------------------------------------------------
    // m_seqBattleの初期化
    //
    // SeqBattleクラスのコンストラクタはDirectXを使用するので
    // MainWindowクラスのコンストラクタの最後のほうで生成する必要がある
    //-------------------------------------------------
    m_seqBattle = NEW SeqBattle();

    // Sleep関数の精度を上げる
    timeBeginPeriod(1);

    // ウィンドウを表示
    ShowWindow(m_hWnd, SW_SHOW);
}

MainWindow::~MainWindow()
{
    // 逆の順番で解放する

    SAFE_DELETE(m_seqEnding);
    SAFE_DELETE(m_seqBattle);

    SaveManager::Destroy();

    NSStarmanLib::Voyage::Destroy();
    NSStarmanLib::PatchTestManager::Destroy();
    NSStarmanLib::MapObjManager::Destroy();
    NSStarmanLib::PowereggDateTime::Destroy();
    NSStarmanLib::Guide::Destroy();
    NSStarmanLib::StatusManager::Destroy();
    NSStarmanLib::SkillManager::Destroy();
    NSStarmanLib::EnemyInfoManager::Destroy();
    NSStarmanLib::WeaponManager::Destroy();
    NSStarmanLib::StorehouseManager::Destroy();
    NSStarmanLib::Inventory::Destroy();
    NSStarmanLib::ItemManager::Destroy();
    NSStarmanLib::MapInfoManager::Destroy();
    NSStarmanLib::HumanInfoManager::Destroy();
    NSStarmanLib::Rynen::Destroy();
    NSStarmanLib::RainModel::Get()->Destroy();
    NSStarmanLib::CraftInfoManager::Destroy();
    NSStarmanLib::CraftSystem::Destroy();
    NSStarmanLib::Help::Destroy();

    QuestManager::Finalize();

    PopUp2::Finalize();
    PopUp::Finalize();

    SoundEffect::finalize();
    BGM::finalize();
    GamePad::Finalize();
    Mouse::Finalize();
    SharedObj::KeyBoard()->Finalize();

    SAFE_RELEASE(m_D3DFont);
    SAFE_RELEASE(m_directInput);

    SharedObj::Finalize();

    SAFE_RELEASE(m_D3D);

    timeEndPeriod(1);
}

int MainWindow::MainLoop()
{
    do
    {
        if (PeekMessage(&m_msg, NULL, 0, 0, PM_REMOVE))
        {
            DispatchMessage(&m_msg);
        }

        // 60FPSになるようにスリープ時間を調節
        {
            static system_clock::time_point tpStart = system_clock::now();
            static system_clock::time_point tpEnd = system_clock::now();
            tpEnd = system_clock::now();

            std::chrono::milliseconds dura =
                std::chrono::duration_cast<std::chrono::milliseconds>(tpEnd - tpStart);
            int dura_i = (int)dura.count();

            if (dura_i >= 16 || dura_i <= -1)
            {
                // Do nothing
            }
            else
            {
                Sleep(16 - dura_i);
            }

            tpStart = system_clock::now();
        }

        // 実際のFPSを求める
        int fps = 0;
        if (Common::DebugMode() || Common::ReleaseMode())
        {
            fps = CalcFPS();
        }

        SharedObj::KeyBoard()->Update();
        Mouse::Update();
        GamePad::Update();

        if (m_sequence == eSequence::BATTLE)
        {
            m_seqBattle->Update(&m_sequence);
            if (m_sequence == eSequence::ENDING)
            {
                SAFE_DELETE(m_seqBattle);
                m_seqEnding = NEW SeqEnding();
            }
        }
        else if (m_sequence == eSequence::ENDING)
        {
            m_seqEnding->Update(&m_sequence);
        }
        else if (m_sequence == eSequence::EXIT)
        {
            PostMessage(m_hWnd, WM_CLOSE, 0, 0);
        }

        //------------------------------------------------------
        // 描画処理
        //------------------------------------------------------
        LPDIRECT3DDEVICE9 D3DDevice = SharedObj::GetD3DDevice();

        D3DDevice->Clear(0,
                         NULL,
                         D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                         D3DCOLOR_XRGB(40, 40, 80),
                         1.0f, 0);

        D3DDevice->BeginScene();

        if (m_sequence == eSequence::BATTLE)
        {
            m_seqBattle->Render();
        }
        else if (m_sequence == eSequence::ENDING)
        {
            m_seqEnding->Render();
        }

        if (Common::DebugMode() || Common::ReleaseMode())
        {
            ShowDebugInfo(fps);
        }

        D3DDevice->EndScene();
        D3DDevice->Present(NULL, NULL, NULL, NULL);
    }
    while (m_msg.message != WM_QUIT);

    return 0;
}

SeqBattle* MainWindow::GetBattleSequence()
{
    return m_seqBattle;
}

void MainWindow::ShowDebugInfo(const int fps)
{
    std::string work;

    RECT rect { };

    //-------------------------------------------------------
    // FPSの表示
    //-------------------------------------------------------
    {
        SetRect(&rect, 0, 0, 50, 50);

        work = "fps:" + std::to_string(fps);
        m_D3DFont->DrawText(NULL,
                            work.c_str(),
                            -1,
                            &rect,
                            DT_LEFT | DT_NOCLIP,
                            D3DCOLOR_ARGB(32, 255, 255, 255));
    }

    //-------------------------------------------------------
    // プレイヤーの座標表示
    //-------------------------------------------------------
    {
        D3DXVECTOR3 pos(0.f, 0.f, 0.f);

        if (SharedObj::GetPlayer() != nullptr)
        {
            pos = SharedObj::GetPlayer()->GetPos();
        }

        // X座標の表示
        SetRect(&rect, 0, 25, 100, 100);
        work = "x:" + std::to_string(pos.x);

        m_D3DFont->DrawText(NULL,
                            work.c_str(),
                            -1,
                            &rect,
                            DT_LEFT | DT_NOCLIP,
                            D3DCOLOR_ARGB(32, 255, 255, 255));

        // Y座標の表示
        SetRect(&rect, 0, 50, 100, 100);
        work = "y:" + std::to_string(pos.y);

        m_D3DFont->DrawText(NULL,
                            work.c_str(),
                            -1,
                            &rect,
                            DT_LEFT | DT_NOCLIP,
                            D3DCOLOR_ARGB(32, 255, 255, 255));

        // Z座標の表示
        SetRect(&rect, 0, 75, 100, 100);
        work = "z:" + std::to_string(pos.z);

        m_D3DFont->DrawText(NULL,
                            work.c_str(),
                            -1,
                            &rect,
                            DT_LEFT | DT_NOCLIP,
                            D3DCOLOR_ARGB(32, 255, 255, 255));
    }
}

int MainWindow::CalcFPS()
{
    //--------------------------------------------------------
    // 毎フレーム、現在時刻を記録し、リストの末尾に追加する。
    // 先頭ほど古い時刻が記録される。
    // リストの先頭から、記録された時刻と現在時刻の差を比較していくと、
    // 最初は1秒以上の差があるが、やがて1秒以下の要素が見つかる。（Aとする）
    // そのときの、A以降の要素の総数がFPSである
    //--------------------------------------------------------

    int fps = 0;

    const int TIME_RECORD_LENGTH = 150;

    system_clock::time_point tempTime = system_clock::now();

    m_vecTime.push_back(tempTime);
    const int removeCnt = (int)m_vecTime.size() - TIME_RECORD_LENGTH;
    if (removeCnt >= 1)
    {
        m_vecTime.erase(m_vecTime.begin(), m_vecTime.begin() + removeCnt);
    }

    tempTime = tempTime - std::chrono::seconds(1);

    for (int i = 0; i < TIME_RECORD_LENGTH; ++i)
    {
        if (m_vecTime.size() == TIME_RECORD_LENGTH)
        {
            if (tempTime < m_vecTime.at(i))
            {
                fps = TIME_RECORD_LENGTH - i;
                break;
            }
        }
    }

    return fps;
}

