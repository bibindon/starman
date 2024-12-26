#include "MainWindow.h"

#include <exception>
#include <chrono>
#include <random>

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

SeqBattle* MainWindow::m_seqBattle = nullptr;

using std::chrono::system_clock;

static LRESULT CALLBACK WndProc(HWND hWnd, UINT mes, WPARAM wParam, LPARAM lParam)
{
    if (mes == WM_DESTROY)
    {
        PostQuitMessage(0);
        return 0;
    }
    else if (mes == WM_ACTIVATE)
    {
        int lower = wParam & 0xFFFF;
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
        else if (lower == WA_INACTIVE)
        {
//            Camera::SetCameraMode(eCameraMode::SLEEP);
            Common::SetCursorVisibility(true);
        }
    }
    else if (mes == WM_CLOSE)
    {
        auto seq = MainWindow::GetBattleSequence();
        if (seq != nullptr)
        {
            if (seq->GetState() == eBattleState::NORMAL ||
                seq->GetState() == eBattleState::MENU)
            {
                if (Common::DeployMode())
                {
                    SaveManager::Get()->Save();
                }
            }
        }
    }

    return DefWindowProc(hWnd, mes, wParam, lParam);
}

MainWindow::MainWindow(const HINSTANCE& hInstance)
{
    SharedObj::Init();

    HICON hIcon = (HICON)LoadImage(hInstance,
                                   MAKEINTRESOURCE(IDI_ICON1),
                                   IMAGE_ICON,
                                   GetSystemMetrics(SM_CXICON),
                                   GetSystemMetrics(SM_CYICON),
                                   0);

    WNDCLASSEX wcex = {
        sizeof(WNDCLASSEX),
        CS_HREDRAW | CS_VREDRAW,
        WndProc,
        0,
        0,
        hInstance,
        hIcon,
        NULL,
        (HBRUSH)(COLOR_WINDOW + 1),
        NULL,
        TITLE.c_str(), NULL
    };

    if (!RegisterClassEx(&wcex))
    {
        throw std::exception("");
    } 

    RECT rect;
    SetRect(&rect, 0, 0, 1600, 900);
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
    rect.right = rect.right - rect.left;
    rect.bottom = rect.bottom - rect.top;

    m_hWnd = CreateWindow(
        TITLE.c_str(),
        TITLE.c_str(),
        WS_OVERLAPPEDWINDOW,
        100,
        100,
        rect.right,
        rect.bottom,
        NULL,
        NULL,
        hInstance,
        NULL);
    if (!m_hWnd)
    {
        throw std::exception("");
    }

    // Direct3D�̏�����
    if (!(m_D3D = Direct3DCreate9(D3D_SDK_VERSION)))
    {
        throw std::exception("");
    }
    auto adaptorNum = m_D3D->GetAdapterCount();

    // Intel HD�ƃO���{����Ȃ�PC�̂Ƃ�DirectX 9�ł�Intel HD����������Ȃ��B
    // ���[�U�[��Windows�̐ݒ��ʂŐݒ肷��K�v������B
    //
    // TODO ����GPU�ƊO�t��GPU�̂��낢��ȑg�ݍ��킹�Ŏ���
    int adaptor = D3DADAPTER_DEFAULT;

    std::vector<std::string> vs;
    for (int i = 0; i < (int)adaptorNum; ++i)
    {
        D3DADAPTER_IDENTIFIER9 adapterInfo;
        m_D3D->GetAdapterIdentifier(i, 0, &adapterInfo);
        vs.push_back(adapterInfo.Description);
    }

    m_GPUName = vs.at(0);

    // TODO �t���X�N���[���Ή�
    D3DPRESENT_PARAMETERS d3dpp = {
        0,
        0,
        D3DFMT_UNKNOWN,
        0,
        D3DMULTISAMPLE_NONE,
        0,
        D3DSWAPEFFECT_DISCARD,
        NULL,
        TRUE,
        TRUE,
        D3DFMT_D24S8,
        0,
        D3DPRESENT_RATE_DEFAULT,
        D3DPRESENT_INTERVAL_DEFAULT
    };

    SharedObj::SetWindowHandle(m_hWnd);

    LPDIRECT3DDEVICE9 D3DDevice;

    if (FAILED(m_D3D->CreateDevice(adaptor, D3DDEVTYPE_HAL, m_hWnd,
        D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &d3dpp, &D3DDevice)))
    {
        if (FAILED(m_D3D->CreateDevice(adaptor, D3DDEVTYPE_HAL, m_hWnd,
            D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &d3dpp, &D3DDevice)))
        {
            m_D3D->Release();
            throw std::exception("");
        }
    }

    SharedObj::SetD3DDevice(D3DDevice);

    ZeroMemory(&light, sizeof(D3DLIGHT9));
    light.Direction = D3DXVECTOR3(-1, -20, 0);
    light.Type = D3DLIGHT_DIRECTIONAL;
    light.Diffuse.r = 1.0f;
    light.Diffuse.g = 1.0f;
    light.Diffuse.b = 1.0f;
    light.Ambient.r = 0.5f;
    light.Ambient.g = 0.5f;
    light.Ambient.b = 0.5f;
    light.Range = 1000;

    D3DDevice->SetLight(0, &light);
    D3DDevice->LightEnable(0, true);
    D3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
    D3DDevice->SetRenderState(D3DRS_AMBIENT, 0x00808080);   // �A���r�G���g���C�g

    // directinput
    HRESULT ret = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
        (LPVOID*)&m_directInput, NULL);

    KeyBoard::Init(m_directInput, m_hWnd);
    Mouse::Init(m_directInput, m_hWnd);
    GamePad::Init(m_directInput, m_hWnd);
    BGM::initialize(m_hWnd);
    SoundEffect::initialize(m_hWnd);
    PopUp::Init();
    PopUp2::Init();

    m_sequence = eSequence::BATTLE;
    m_seqBattle = new SeqBattle();

    ret = D3DXCreateFont(
        D3DDevice,
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

    timeBeginPeriod(1);

    // �E�B���h�E�\��
    ShowWindow(m_hWnd, SW_SHOW);
}

MainWindow::~MainWindow()
{
    BGM::finalize();
    SoundEffect::finalize();
    SAFE_DELETE(m_sprite);
    SharedObj::Finalize();
    m_D3D->Release();
    timeEndPeriod(1);
}

int MainWindow::MainLoop()
{
    // TODO ���`
    LPDIRECT3DDEVICE9 D3DDevice = SharedObj::GetD3DDevice();
    D3DXMATRIX World;          // �����̃��[���h�ϊ��s��
    D3DXMATRIX Rot_X, Rot_Y;   // �����̉�]�s��
    D3DXMATRIX Offset;         // �����̃I�t�Z�b�g�s��
    D3DXMATRIX View;   // �r���[�ϊ��s��
    D3DXMATRIX Persp;   // �ˉe�ϊ��s��

    // �r���[�ϊ�
    // ���_�͌��_�Œ�ł����A�J�����̈ʒu�͓K���ł�
    D3DXVECTOR3 a = D3DXVECTOR3(0.f, 2.f, -4.f);
    D3DXVECTOR3 b = D3DXVECTOR3(0.f, 0.f, 0.f);
    D3DXVECTOR3 c = D3DXVECTOR3(0.f, 1.f, 0.f);
    D3DXMatrixLookAtLH(
        &View,
        &a,
        &b,
        &c
    );
    float viewAngle { D3DX_PI / 4 };
    Camera::SetLookAtPos(b);

    FLOAT Ang = 0.0f;   // ��]�p�x
    do
    {
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
                // do nothing
            }
            else
            {
                Sleep(16 - dura_i);
            }
            tpStart = system_clock::now();

            if (PeekMessage(&m_msg, NULL, 0, 0, PM_REMOVE))
            {
                DispatchMessage(&m_msg);
            }
        }

        system_clock::time_point tempTime { system_clock::now() };

        m_vecTime.push_back(tempTime);
        const int removeCnt = m_vecTime.size() - 300;
        if (removeCnt >= 1)
        {
            m_vecTime.erase(m_vecTime.begin(), m_vecTime.begin() + removeCnt);
        }

        tempTime = tempTime - std::chrono::seconds { 1 };

        int fps { 0 };
        for (int i = 0; i < 300; ++i)
        {
            if (m_vecTime.size() == 300)
            {
                if (tempTime < m_vecTime.at(i))
                {
                    fps = 300 - i;
                    break;
                }
            }
        }

        KeyBoard::Update();
        Mouse::Update();
        GamePad::Update();
        Camera::Update();

        if (m_sequence == eSequence::BATTLE)
        {
            m_seqBattle->Update(&m_sequence);
            if (m_sequence == eSequence::ENDING)
            {
                SAFE_DELETE(m_seqBattle);
                m_seqEnding = new SeqEnding();
            }
        }
        else if (m_sequence == eSequence::ENDING)
        {
            m_seqEnding->Update(&m_sequence);
        }

        D3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(40, 40, 80),
            1.0f, 0);
        D3DDevice->BeginScene();


        ///////////////////////////
        // ���C�g
        ////////
            // ���C�g�̕�����]
        light.Direction.x = 20 * sinf(D3DXToRadian(Ang));
        light.Direction.z = 20 * cosf(D3DXToRadian(Ang));
        D3DDevice->SetLight(0, &light);

        ///////////////////////////
        // ������
        ////////
        D3DXMatrixRotationX(&Rot_X, D3DXToRadian(Ang * 0.15f));
        D3DXMatrixRotationY(&Rot_Y, D3DXToRadian(Ang * 0.2f) + 70.0f);
        D3DXMatrixTranslation(&Offset, 10, 10, 10);

        D3DXMatrixIdentity(&World);          // �P�ʍs��
        D3DXMatrixMultiply(&World, &World, &Offset);   // �܂��I�t�Z�b�g���āA
        D3DXMatrixMultiply(&World, &World, &Rot_X);    // X����]��
        D3DXMatrixMultiply(&World, &World, &Rot_Y);    // Y����]�����܂��B

        // �ˉe�ϊ�
        D3DXMatrixPerspectiveFovLH(&Persp, D3DXToRadian(45), 640.0f / 480.0f, 1.0f, 10000.0f);

        // �s��o�^
        D3DDevice->SetTransform(D3DTS_WORLD, &World);
        D3DDevice->SetTransform(D3DTS_VIEW, &View);
        D3DDevice->SetTransform(D3DTS_PROJECTION, &Persp);

        if (m_sequence == eSequence::BATTLE)
        {
            m_seqBattle->Render();
        }
        else if (m_sequence == eSequence::ENDING)
        {
            m_seqEnding->Render();
        }
        else if (m_sequence == eSequence::EXIT)
        {
            PostMessage(m_hWnd, WM_CLOSE, 0, 0);
        }

        if (m_sprite != nullptr)
        {
            D3DXVECTOR3 pos { 0.0f, 0.0f, 0.0f };
            m_sprite->Render(pos);
        }

        RECT rect;

        if (m_seqBattle != nullptr && m_seqBattle->GetState() == eBattleState::TITLE)
        {
            SetRect(&rect, 10, 850, 500, 890);
            m_D3DFont->DrawText(NULL,
                                m_GPUName.c_str(),
                                -1,
                                &rect,
                                DT_LEFT | DT_BOTTOM,
                                D3DCOLOR_ARGB(32, 255, 255, 255));
        }

        if (Common::DebugMode() || Common::ReleaseMode())
        {
            std::string work;

            SetRect(&rect, 0, 0, 50, 50);
            work = "fps:" + std::to_string(fps);
            m_D3DFont->DrawText(
                NULL,
                work.c_str(),
                -1,
                &rect,
                DT_LEFT | DT_NOCLIP,
                D3DCOLOR_ARGB(32, 255, 255, 255));

            {
                D3DXVECTOR3 pos {0.f, 0.f, 0.f};
                if (SharedObj::GetPlayer() != nullptr)
                {
                    pos = SharedObj::GetPlayer()->GetPos();
                }

                SetRect(&rect, 0, 25, 100, 100);
                work = "x:" + std::to_string(pos.x);

                m_D3DFont->DrawText(NULL,
                                    work.c_str(),
                                    -1,
                                    &rect,
                                    DT_LEFT | DT_NOCLIP,
                                    D3DCOLOR_ARGB(32, 255, 255, 255));

                SetRect(&rect, 0, 50, 100, 100);
                work = "y:" + std::to_string(pos.y);

                m_D3DFont->DrawText(NULL,
                                    work.c_str(),
                                    -1,
                                    &rect,
                                    DT_LEFT | DT_NOCLIP,
                                    D3DCOLOR_ARGB(32, 255, 255, 255));

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

        D3DDevice->EndScene();
        D3DDevice->Present(NULL, NULL, NULL, NULL);

        if (Common::DebugMode() || Common::ReleaseMode())
        {
            if (KeyBoard::IsDownFirstFrame(DIK_F4))
            {
                PostMessage(m_hWnd, WM_CLOSE, 0, 0);
            }
            if (KeyBoard::IsDownFirstFrame(DIK_Q))
            {
                PostMessage(m_hWnd, WM_CLOSE, 0, 0);
            }
        }

    } while (m_msg.message != WM_QUIT);
    return 0;
}

SeqBattle* MainWindow::GetBattleSequence()
{
    return m_seqBattle;
}

