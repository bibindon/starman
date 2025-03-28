#include "Util.h"
#include "../starman/SharedObj.h"
#include "../starman/GamePad.h"
#include "../starman/SoundEffect.h"
#include "../starman/BGM.h"
#include "../starman/Mouse.h"
#include "../starman/KeyBoard.h"

LPDIRECT3D9 Util::m_D3D = nullptr;
LPDIRECTINPUT8 Util::m_directInput = nullptr;

void Util::InitWin_DX9_DI8(const bool bShow)
{
    WNDCLASS wc = {};
    wc.lpfnWndProc = DefWindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = "MyWindowClass";

    RegisterClass(&wc);

    auto hInstance = (HINSTANCE)GetModuleHandle(0);
    HWND hWnd = CreateWindow("MyWindowClass",
                             "dummy",
                             WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT,
                             CW_USEDEFAULT,
                             CW_USEDEFAULT,
                             CW_USEDEFAULT,
                             NULL,
                             NULL,
                             hInstance,
                             NULL);

    SharedObj::SetWindowHandle(hWnd);

    if (bShow)
    {
		ShowWindow(SharedObj::GetWindowHandle(), SW_SHOW);
    }

    // Direct3D‚Ì‰Šú‰»
    m_D3D = nullptr;
    if (!(m_D3D = Direct3DCreate9(D3D_SDK_VERSION)))
    {
        throw std::exception("");
    }

    LPDIRECT3DDEVICE9 D3DDevice;

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

    auto result = m_D3D->CreateDevice(0,
                                    D3DDEVTYPE_HAL,
                                    hWnd,
                                    D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
                                    &d3dpp,
                                    &D3DDevice);

    if (FAILED(result))
    {
        throw std::exception("");
    }

    SharedObj::SetD3DDevice(D3DDevice);

    m_directInput = nullptr;

    HRESULT ret = DirectInput8Create(hInstance,
                                     DIRECTINPUT_VERSION,
                                     IID_IDirectInput8,
                                     (LPVOID*)&m_directInput,
                                     NULL);

    KeyBoard::Init(m_directInput, hWnd);
    Mouse::Init(m_directInput, hWnd);
    GamePad::Init(m_directInput, hWnd);
    BGM::initialize(hWnd);
    SoundEffect::initialize(hWnd);
}

void Util::ReleaseWin_DX9_DI8()
{
    SharedObj::Finalize();
    KeyBoard::Finalize();
    Mouse::Finalize();
    GamePad::Finalize();
    BGM::finalize();
    SoundEffect::finalize();

    SAFE_RELEASE(m_D3D);
    SAFE_RELEASE(m_directInput);
}



