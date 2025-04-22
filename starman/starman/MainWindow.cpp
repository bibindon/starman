
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
    // �E�B���h�E���A�N�e�B�uor��A�N�e�B�u�ɂȂ����Ƃ��̏���
    // �J�����ƃJ�[�\���̃��[�h��ς���
    else if (mes == WM_ACTIVATE)
    {
        int lower = wParam & 0xFFFF;

        // �A�N�e�B�u�ɂȂ������̏���
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
        // ��A�N�e�B�u�ɂȂ������̏���
        else if (lower == WA_INACTIVE)
        {
            Common::SetCursorVisibility(true);
        }
    }
    // �E�B���h�E����鑀����s������Z�[�u���������s����
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
    // �E�B���h�E�N���X�̓o�^
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
        throw std::exception("RegisterClassEx�֐��̎��s�����s���܂����B");
    } 

    //-------------------------------------------------
    // �E�B���h�E�̍쐬
    //-------------------------------------------------
    {
        //-------------------------------------------------
        // �E�B���h�E����ʒ����ɕ\�������悤�ɐݒ�
        //-------------------------------------------------
        RECT rect { };
        SetRect(&rect, 0, 0, 1600, 900);

        // �^�C�g���o�[�A�E�B���h�E�t���[���̕����l�������T�C�Y���擾
        AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

        rect.right = rect.right - rect.left;
        rect.bottom = rect.bottom - rect.top;

        int monitorWidth = GetSystemMetrics(SM_CXSCREEN);
        int monitorHeight = GetSystemMetrics(SM_CYSCREEN);

        int startX = (monitorWidth / 2) - (1600 / 2);
        int startY = (monitorHeight / 2) - (900 / 2);

        m_hWnd = CreateWindow(TITLE.c_str(),
                              TITLE.c_str(),
                              /* �E�B���h�E�T�C�Y�̕ύX�������Ȃ��B�ŏ�����OK */
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
            throw std::exception("�E�B���h�E�̍쐬���ł��܂���ł���");
        }

        SharedObj::SetWindowHandle(m_hWnd);
    }

    //-------------------------------------------------
    // Direct3D�̏�����
    //-------------------------------------------------
    {
        m_D3D = Direct3DCreate9(D3D_SDK_VERSION);

        if (!m_D3D)
        {
            throw std::exception("DirectX�̏������Ɏ��s���܂���");
        }
    }

    HRESULT hResult = E_FAIL;

    //-------------------------------------------------
    // Direct3DDevice�̏�����
    //-------------------------------------------------
    {
        //-------------------------------------------------
        // DirectX9�ł̓v���O��������GPU�̐؂�ւ����ł��Ȃ��B
        // �g�p����GPU��ς������ꍇ�́A���[�U�[��Windows�̐ݒ��ʂŐݒ肷�邵���Ȃ��B
        //-------------------------------------------------

        // �O���t�B�b�N�{�[�h�̖��̂��擾
        D3DADAPTER_IDENTIFIER9 adapterInfo { };
        m_D3D->GetAdapterIdentifier(D3DADAPTER_DEFAULT, 0, &adapterInfo);

        std::string GPUName = adapterInfo.Description;
        Common::SetGPUName(GPUName);

        // TODO �t���X�N���[���Ή�

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
                throw std::exception("Direct3D::CreateDevice�֐������s���܂���");
            }
        }

        SharedObj::SetD3DDevice(D3DDevice);
    }

    //-------------------------------------------------
    // DirectInput�̏�����
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
            throw std::exception("DirectInput8Create�֐������s���܂���");
        }
    }

    //-------------------------------------------------
    // �t�H���g�̏�����
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
                                 "�l�r ����",
                                 &m_D3DFont);

        assert(hResult == S_OK);
    }

    //-------------------------------------------------
    // �L�[�{�[�h�A�}�E�X�A�Q�[���p�b�h�A�T�E���h�̏�����
    //-------------------------------------------------
    {
        keyboard->Init(m_directInput, m_hWnd);
        SharedObj::SetKeyBoard(keyboard);

        Mouse::Init(m_directInput, m_hWnd);
        GamePad::Init(m_directInput, m_hWnd);
        BGM::initialize(m_hWnd);
        SoundEffect::initialize(m_hWnd);
    }

    // PopUp�N���X�̏�����
    PopUp::Init();

    // PopUp2�N���X�̏�����
    IPopUpFont* popUpFont = NEW PopUpFont(SharedObj::GetD3DDevice());
    PopUp2::Init(popUpFont);

    //-------------------------------------------------
    // m_seqBattle�̏�����
    //
    // SeqBattle�N���X�̃R���X�g���N�^��DirectX���g�p����̂�
    // MainWindow�N���X�̃R���X�g���N�^�̍Ō�̂ق��Ő�������K�v������
    //-------------------------------------------------
    m_seqBattle = NEW SeqBattle();

    // Sleep�֐��̐��x���グ��
    timeBeginPeriod(1);

    // �E�B���h�E��\��
    ShowWindow(m_hWnd, SW_SHOW);
}

MainWindow::~MainWindow()
{
    // �t�̏��Ԃŉ������

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

        // 60FPS�ɂȂ�悤�ɃX���[�v���Ԃ𒲐�
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

        // ���ۂ�FPS�����߂�
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
        // �`�揈��
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
    // FPS�̕\��
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
    // �v���C���[�̍��W�\��
    //-------------------------------------------------------
    {
        D3DXVECTOR3 pos(0.f, 0.f, 0.f);

        if (SharedObj::GetPlayer() != nullptr)
        {
            pos = SharedObj::GetPlayer()->GetPos();
        }

        // X���W�̕\��
        SetRect(&rect, 0, 25, 100, 100);
        work = "x:" + std::to_string(pos.x);

        m_D3DFont->DrawText(NULL,
                            work.c_str(),
                            -1,
                            &rect,
                            DT_LEFT | DT_NOCLIP,
                            D3DCOLOR_ARGB(32, 255, 255, 255));

        // Y���W�̕\��
        SetRect(&rect, 0, 50, 100, 100);
        work = "y:" + std::to_string(pos.y);

        m_D3DFont->DrawText(NULL,
                            work.c_str(),
                            -1,
                            &rect,
                            DT_LEFT | DT_NOCLIP,
                            D3DCOLOR_ARGB(32, 255, 255, 255));

        // Z���W�̕\��
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
    // ���t���[���A���ݎ������L�^���A���X�g�̖����ɒǉ�����B
    // �擪�قǌÂ��������L�^�����B
    // ���X�g�̐擪����A�L�^���ꂽ�����ƌ��ݎ����̍����r���Ă����ƁA
    // �ŏ���1�b�ȏ�̍������邪�A�₪��1�b�ȉ��̗v�f��������B�iA�Ƃ���j
    // ���̂Ƃ��́AA�ȍ~�̗v�f�̑�����FPS�ł���
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

