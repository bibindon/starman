#pragma once

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

class KeyBoard
{
public:
    static void Init(LPDIRECTINPUT8 directInput, HWND hWnd);
    static void Update();
    static bool IsDown(int keyCode);
private:
    static LPDIRECTINPUTDEVICE8 m_keyboard;
    static BYTE m_key[256];
};

