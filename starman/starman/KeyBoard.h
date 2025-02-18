#pragma once

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif

#include <dinput.h>
#include <deque>
#include <vector>

class KeyBoard
{
public:
    static void Init(LPDIRECTINPUT8 directInput, HWND hWnd);
    static void Update();
    static void Finalize();

    // 押されている
    static bool IsDown(int keyCode);

    // 押されていて、その直前まで押されていなかった
    static bool IsDownFirstFrame(int keyCode);

    // 0.5秒以上長押ししていた
    static bool IsHold(int keyCode);
private:
    static LPDIRECTINPUTDEVICE8 m_keyboard;
    static BYTE m_key[256];
    static BYTE m_keyPrev[256];
    static std::deque<std::vector<BYTE>> m_keyDeque;
};

