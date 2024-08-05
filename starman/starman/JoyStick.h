#pragma once

// constexprÇ…ïœä∑Ç≈Ç´ÇÈÅAÇ∆Ç¢Ç§åxçêÇ™ï\é¶Ç≥ÇÍÇÈÇ™ïœä∑ÇµÇƒÇÕÇ¢ÇØÇ»Ç¢ÅB
#define DIRECTINPUT_VERSION 0x0800

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
class JoyStick
{
public:

    static bool initialize(HWND hwnd);
    static void finalize();
    static void update();
    static bool is_hold(eJoyStickButtonType button);
    static bool is_up(eJoyStickButtonType button);
    static bool is_down(eJoyStickButtonType button);
    static bool check_simultaneous(eJoyStickButtonType button);

    static LPDIRECTINPUT8 direct_input8_;
    static LPDIRECTINPUTDEVICE8 input_device8_;
    struct JoyStickInfo
    {
        std::unordered_map<eJoyStickButtonType, eJoyStickButtonState> button_states_;
    };
    static std::deque<JoyStickInfo> button_deque_;

private:
    static bool create_device(HWND hwnd);
    static BOOL start_game_pad_control();

};


