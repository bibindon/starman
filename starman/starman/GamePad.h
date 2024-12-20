#pragma once

#ifndef DIRECTINPUT_VERSION
// constexpr�ɕϊ��ł���A�Ƃ����x�����\������邪�ϊ����Ă͂����Ȃ��B
#define DIRECTINPUT_VERSION 0x0800
#endif

#include <dinput.h>
#include <unordered_map>
#include <deque>

// Monostate pattern.
enum class eGamePadButtonType
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    X,
    Y,
    A, // Confirm
    B, // Cancel
    Z_LEFT,
    Z_RIGHT,
    Z_UP,
    Z_DOWN,
    L1,
    R1,
    BACK,
    START,
    UNKNOWN,
};
static constexpr int BUTTON_KIND_MAX { 8 };

enum class eGamePadButtonState
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
    static bool IsHold(eGamePadButtonType button);
    static bool IsUp(eGamePadButtonType button);
    static bool IsDown(eGamePadButtonType button);
    static float GetLeftRadian();
    static bool IsLeftStickUsed();
    static bool CheckSimultaneous(eGamePadButtonType button);

    static LPDIRECTINPUT8 m_DI;
    static LPDIRECTINPUTDEVICE8 m_DIDevice;
    struct GamePadInfo
    {
        std::unordered_map<eGamePadButtonType, eGamePadButtonState> m_buttonStatusMap;
    };
    static std::deque<GamePadInfo> m_deqButton;

private:
    static bool CreateDevice(HWND hwnd);
    static BOOL StartGamePadControl();
    static float m_leftRadian;
    static bool m_bLeftStickUsed;

};


