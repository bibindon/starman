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

    //---------------------------------------------------
    // テスト用関数
    // キー入力を設定できるようにする
    //---------------------------------------------------
    static void ForTest_SetTestMode();

    static void ForTest_SetIsDown(int keyCode);
    static void ForTest_SetIsDownFirstFrame(int keyCode);
    static void ForTest_SetIsHold(int keyCode);
    static void ForTest_Clear();

private:
    static LPDIRECTINPUTDEVICE8 m_keyboard;
    static BYTE m_key[256];
    static BYTE m_keyPrev[256];
    static std::deque<std::vector<BYTE>> m_keyDeque;

    //---------------------------------------------------
    // テスト用関数
    //---------------------------------------------------
    static bool m_testMode;
    static bool m_keyIsDown[256];
    static bool m_keyIsDownFirstFrame[256];
    static bool m_keyIsHold[256];
};

