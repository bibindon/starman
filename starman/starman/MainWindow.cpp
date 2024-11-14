#include "MainWindow.h"

#include <exception>
#include <chrono>

#include "KeyBoard.h"
#include "Mouse.h"
#include "GamePad.h"
#include "BGM.h"
#include "SoundEffect.h"
#include "Common.h"
#include "Camera.h"
#include "SharedObj.h"
#include <random>
#include "PopUp.h"
using std::chrono::system_clock;

LRESULT CALLBACK WndProc(HWND hWnd, UINT mes, WPARAM wParam, LPARAM lParam)
{
    if (mes == WM_DESTROY)
    {
        PostQuitMessage(0);
        return 0;
    }
    else if (mes == WM_SETCURSOR)
    {
        ShowCursor(false);
        return 0;
    }

    return DefWindowProc(hWnd, mes, wParam, lParam);
}

MainWindow::MainWindow(const HINSTANCE& hInstance)
{
    SharedObj::Init();

    WNDCLASSEX wcex = {
        sizeof(WNDCLASSEX),
        CS_HREDRAW | CS_VREDRAW,
        WndProc,
        0,
        0,
        hInstance,
        NULL,
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

    // Direct3Dの初期化
    if (!(m_D3D = Direct3DCreate9(D3D_SDK_VERSION)))
    {
        throw std::exception("");
    }

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

    LPDIRECT3DDEVICE9 D3DDevice;
    if (FAILED(m_D3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,
        D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &D3DDevice)))
    {
        if (FAILED(m_D3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,
            D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &D3DDevice)))
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
    D3DDevice->SetRenderState(D3DRS_AMBIENT, 0x00808080);   // アンビエントライト

    // directinput
    HRESULT ret = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
        (LPVOID*)&m_directInput, NULL);

    KeyBoard::Init(m_directInput, m_hWnd);
    Mouse::Init(m_directInput, m_hWnd);
    GamePad::Init(m_directInput, m_hWnd);
    BGM::initialize(m_hWnd);
    SoundEffect::initialize(m_hWnd);
    PopUp::Init();

    m_seqTitle = new SeqTitle();

    ret = D3DXCreateFont(
        D3DDevice,
        30,
        0,
        FW_HEAVY,
        1,
        false,
        SHIFTJIS_CHARSET,
        OUT_TT_ONLY_PRECIS,
        ANTIALIASED_QUALITY,
        FF_DONTCARE,
        "ＭＳ ゴシック",
        &m_D3DFont);

    timeBeginPeriod(1);

    // ウィンドウ表示
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
    LPDIRECT3DDEVICE9 D3DDevice = SharedObj::GetD3DDevice();
    D3DXMATRIX World;          // 立方体ワールド変換行列
    D3DXMATRIX Rot_X, Rot_Y;   // 立方体回転行列
    D3DXMATRIX Offset;         // 立方体オフセット行列
    D3DXMATRIX View;   // ビュー変換行列
    D3DXMATRIX Persp;   // 射影変換行列

    // ビュー変換
    // 視点は原点固定ですが、カメラの位置は適当です
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
    Camera::SetPos(b);

    FLOAT Ang = 0.0f;   // 回転角度
//    unsigned int i;
    do
    {
        Sleep(15);
        if (PeekMessage(&m_msg, NULL, 0, 0, PM_REMOVE))
        {
            DispatchMessage(&m_msg);
        }

        system_clock::time_point tempTime { system_clock::now() };

        m_vecTime.push_back(tempTime);
        const int removeCnt = m_vecTime.size() - 100;
        if (removeCnt >= 1)
        {
            m_vecTime.erase(m_vecTime.begin(), m_vecTime.begin() + removeCnt);
        }

        tempTime = tempTime - std::chrono::seconds { 1 };

        int fps { 0 };
        for (int i = 0; i < 100; ++i)
        {
            if (m_vecTime.size() == 100)
            {
                if (tempTime < m_vecTime.at(i))
                {
                    fps = 100 - i;
                    break;
                }
            }
        }

        RECT rect;
        SetRect(&rect, 0, 0, 50, 50);

        KeyBoard::Update();
        Mouse::Update();
        GamePad::Update();
        Camera::Update();

        if (KeyBoard::IsDown(DIK_ESCAPE))
        {
            PostMessage(m_hWnd, WM_CLOSE, 0, 0);
        }
        if (KeyBoard::IsDown(DIK_Q))
        {
            //BGM::get_ton()->load("res\\sound\\title.wav");
//            SoundEffect::get_ton()->load("res\\sound\\damage01.wav");
//            MessageBox(NULL, TEXT("aaa"), TEXT("bbb"), 0);
            PostMessage(m_hWnd, WM_CLOSE, 0, 0);
        }
        if (KeyBoard::IsDown(DIK_E))
        {
            //BGM::get_ton()->play();
            SoundEffect::get_ton()->play("res\\sound\\damage01.wav");
            MessageBox(NULL, TEXT("aaa"), TEXT("bbb"), 0);
        }
        if (Mouse::IsDownLeft())
        {
//            MessageBox(NULL, TEXT("aaaaaaa"), TEXT("aaaaaaa"), 0);
        //    m_sprite = new Sprite("res\\image\\board.png");
        }
        if (GamePad::IsDown(eJoyStickButtonType::A))
        {
//            MessageBox(NULL, TEXT("aaaaaaa"), TEXT("aaaaaaa"), 0);
         //   m_sprite = new Sprite("res\\image\\board.png");
        }

        if (m_sequence == eSequence::TITLE)
        {
            m_seqTitle->Update(&m_sequence);
            if (m_sequence == eSequence::OPENING)
            {
                SAFE_DELETE(m_seqTitle);
                m_seqOpening = new SeqOpening();
            }
            else if (m_sequence == eSequence::BATTLE)
            {
                SAFE_DELETE(m_seqTitle);
                m_seqBattle = new SeqBattle(true);
            }
        }
        else if (m_sequence == eSequence::OPENING)
        {
            m_seqOpening->Update(&m_sequence);
            if (m_sequence == eSequence::BATTLE)
            {
                SAFE_DELETE(m_seqOpening);
                m_seqBattle = new SeqBattle(false);
            }
        }
        else if (m_sequence == eSequence::BATTLE)
        {
            m_seqBattle->Update(&m_sequence);
            if (m_sequence == eSequence::ENDING)
            {
                SAFE_DELETE(m_seqBattle);
                m_seqEnding = new SeqEnding();
            }
            else if (m_sequence == eSequence::TITLE)
            {
                SAFE_DELETE(m_seqBattle);
                m_seqTitle = new SeqTitle();
            }
        }
        else if (m_sequence == eSequence::ENDING)
        {
            m_seqEnding->Update(&m_sequence);
            if (m_sequence == eSequence::TITLE)
            {
                SAFE_DELETE(m_seqEnding);
                m_seqTitle = new SeqTitle();
            }
        }

        D3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(40, 40, 80),
            1.0f, 0);
        D3DDevice->BeginScene();


        ///////////////////////////
        // ライト
        ////////
            // ライトの方向回転
        light.Direction.x = 20 * sinf(D3DXToRadian(Ang));
        light.Direction.z = 20 * cosf(D3DXToRadian(Ang));
        D3DDevice->SetLight(0, &light);

        ///////////////////////////
        // 立方体
        ////////
        D3DXMatrixRotationX(&Rot_X, D3DXToRadian(Ang * 0.15f));
        D3DXMatrixRotationY(&Rot_Y, D3DXToRadian(Ang * 0.2f) + 70.0f);
        D3DXMatrixTranslation(&Offset, 10, 10, 10);

        D3DXMatrixIdentity(&World);          // 単位行列化
        D3DXMatrixMultiply(&World, &World, &Offset);   // まずオフセットして、
        D3DXMatrixMultiply(&World, &World, &Rot_X);    // X軸回転後
        D3DXMatrixMultiply(&World, &World, &Rot_Y);    // Y軸回転させます。

        // 射影変換
        D3DXMatrixPerspectiveFovLH(&Persp, D3DXToRadian(45), 640.0f / 480.0f, 1.0f, 10000.0f);

        // 行列登録
        D3DDevice->SetTransform(D3DTS_WORLD, &World);
        D3DDevice->SetTransform(D3DTS_VIEW, &View);
        D3DDevice->SetTransform(D3DTS_PROJECTION, &Persp);


        if (m_sequence == eSequence::TITLE)
        {
            m_seqTitle->Render();
        }
        else if (m_sequence == eSequence::OPENING)
        {
            m_seqOpening->Render();
        }
        else if (m_sequence == eSequence::BATTLE)
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

        m_D3DFont->DrawText(
            NULL,
            std::to_string(fps).c_str(),
            -1,
            &rect,
            DT_LEFT | DT_NOCLIP,
            D3DCOLOR_ARGB(255, 0, 127, 0));

        {
            std::string work;
            D3DXVECTOR3 pos {0.f, 0.f, 0.f};
            if (SharedObj::GetPlayer() != nullptr)
            {
                pos = SharedObj::GetPlayer()->GetPos();
            }

            SetRect(&rect, 0, 40, 100, 100);
            work = "x:" + std::to_string(pos.x);

            m_D3DFont->DrawText(
                NULL,
                work.c_str(),
                -1,
                &rect,
                DT_LEFT | DT_NOCLIP,
                D3DCOLOR_ARGB(255, 0, 127, 0));

            SetRect(&rect, 0, 80, 100, 100);
            work = "y:" + std::to_string(pos.y);

            m_D3DFont->DrawText(
                NULL,
                work.c_str(),
                -1,
                &rect,
                DT_LEFT | DT_NOCLIP,
                D3DCOLOR_ARGB(255, 0, 127, 0));

            SetRect(&rect, 0, 120, 100, 100);
            work = "z:" + std::to_string(pos.z);

            m_D3DFont->DrawText(
                NULL,
                work.c_str(),
                -1,
                &rect,
                DT_LEFT | DT_NOCLIP,
                D3DCOLOR_ARGB(255, 0, 127, 0));
        }

        D3DDevice->EndScene();
        D3DDevice->Present(NULL, NULL, NULL, NULL);


    } while (m_msg.message != WM_QUIT);
    return 0;
}
