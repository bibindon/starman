#pragma once

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

class Mouse
{
public:
    static void Init(LPDIRECTINPUT8 directInput, HWND hWnd);
    static void Update();
    static bool IsDownLeft();
    static bool IsDownRight();

private:
    static LPDIRECTINPUTDEVICE8 m_DIMouse;
    static DIMOUSESTATE m_DIMouseState;
};

