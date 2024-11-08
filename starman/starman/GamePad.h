#pragma once

#ifndef DIRECTINPUT_VERSION
// constexprに変換できる、という警告が表示されるが変換してはいけない。
#define DIRECTINPUT_VERSION 0x0800
#endif

#include <dinput.h>
#include <unordered_map>
#include <deque>

// Monostate pattern.
enum class eJoyStickButtonType
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    X,
    Y,
    A,
    B,
    Z_LEFT,
    Z_RIGHT,
    Z_UP,
    Z_DOWN,
    R1,
    UNKNOWN,
};
static constexpr int BUTTON_KIND_MAX { 8 };

enum class eJoyStickButtonState
{
    NONE,
    DOWN,
    HOLD,
    UP,
    MAX,
};
class GamePad
{
public:

    static bool Init(LPDIRECTINPUT8 DI, HWND hwnd);
    static void Finalize();
    static void Update();
    static bool IsHold(eJoyStickButtonType button);
    static bool IsUp(eJoyStickButtonType button);
    static bool IsDown(eJoyStickButtonType button);
    static float GetLeftRadian();
    static bool IsLeftStickUsed();
    static bool CheckSimultaneous(eJoyStickButtonType button);

    static LPDIRECTINPUT8 m_DI;
    static LPDIRECTINPUTDEVICE8 m_DIDevice;
    struct JoyStickInfo
    {
        std::unordered_map<eJoyStickButtonType, eJoyStickButtonState> m_buttonStatusMap;
    };
    static std::deque<JoyStickInfo> m_deqButton;

private:
    static bool CreateDevice(HWND hwnd);
    static BOOL StartGamePadControl();
    static float m_leftRadian;
    static bool m_bLeftStickUsed;

};


