#include "Util.h"
#include "../starman/SharedObj.h"
#include "../starman/GamePad.h"
#include "../starman/SoundEffect.h"
#include "../starman/BGM.h"
#include "../starman/Mouse.h"
#include "../starman/KeyBoard.h"

void Util::InitWin_DX9_DI8()
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

    // Direct3D‚Ì‰Šú‰»
    LPDIRECT3D9 D3D = nullptr;
    if (!(D3D = Direct3DCreate9(D3D_SDK_VERSION)))
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

    auto result = D3D->CreateDevice(0,
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

    LPDIRECTINPUT8 directInput = nullptr;

    HRESULT ret = DirectInput8Create(hInstance,
                                     DIRECTINPUT_VERSION,
                                     IID_IDirectInput8,
                                     (LPVOID*)&directInput,
                                     NULL);

    KeyBoard::Init(directInput, hWnd);
    Mouse::Init(directInput, hWnd);
    GamePad::Init(directInput, hWnd);
    BGM::initialize(hWnd);
    SoundEffect::initialize(hWnd);
}



