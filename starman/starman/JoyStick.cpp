#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include "JoyStick.h"
#include "Common.h"
#include <d3dx9.h>

using std::unordered_map;
using std::deque;

BOOL CALLBACK device_find_callback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);
BOOL setup_gamepad_property(LPDIRECTINPUTDEVICE8 device);

LPDIRECTINPUT8 JoyStick::direct_input8_;
LPDIRECTINPUTDEVICE8 JoyStick::input_device8_;
deque<JoyStick::JoyStickInfo> JoyStick::button_deque_;

bool JoyStick::initialize(HWND hwnd)
{
    // Init button_queue_
    JoyStickInfo temp { };
    temp.button_states_.insert({ eJoyStickButtonType::UP, eJoyStickButtonState::NONE });
    temp.button_states_.insert({ eJoyStickButtonType::DOWN, eJoyStickButtonState::NONE });
    temp.button_states_.insert({ eJoyStickButtonType::LEFT, eJoyStickButtonState::NONE });
    temp.button_states_.insert({ eJoyStickButtonType::RIGHT, eJoyStickButtonState::NONE });
    temp.button_states_.insert({ eJoyStickButtonType::X, eJoyStickButtonState::NONE });
    temp.button_states_.insert({ eJoyStickButtonType::Y, eJoyStickButtonState::NONE });
    temp.button_states_.insert({ eJoyStickButtonType::A, eJoyStickButtonState::NONE });
    temp.button_states_.insert({ eJoyStickButtonType::B, eJoyStickButtonState::NONE });
    button_deque_.push_back(temp);
    button_deque_.push_back(temp);
    button_deque_.push_back(temp);
    button_deque_.push_back(temp);
    button_deque_.push_back(temp);

    // Create input interface.
    LPDIRECTINPUT8 temp_direct_input { nullptr };
    HRESULT result { DirectInput8Create(
        GetModuleHandle(nullptr),
        DIRECTINPUT_VERSION,
        IID_IDirectInput8,
        (void**)&temp_direct_input,
        nullptr) };

    if (FAILED(result))
    {
        return false;
    }
    SAFE_RELEASE(direct_input8_);
    direct_input8_ = temp_direct_input;

    // Create device.
    if (!create_device(hwnd))
    {
        // It's not problem failing to create input device.
        finalize();
        return false;
    }
    return true;
}

void JoyStick::finalize()
{
    if (input_device8_ != nullptr)
    {
        input_device8_->Unacquire();
    }
    SAFE_RELEASE(input_device8_);
}

// A struct that receives device enumeration result.
struct device_enum_param
{
    LPDIRECTINPUTDEVICE8* arg_input_device8_;
    int find_count_;
    HWND hwnd_;
};

BOOL JoyStick::start_game_pad_control()
{
    if (input_device8_ == nullptr)
    {
        return false;
    }

    if (FAILED(input_device8_->Acquire()))
    {
        return false;
    }

    DIDEVCAPS cap = { 0 };
    input_device8_->GetCapabilities(&cap);
    // Check polling
    if (cap.dwFlags & DIDC_POLLEDDATAFORMAT)
    {
        DWORD error = GetLastError();
        if (FAILED(input_device8_->Poll()))
        {
            return false;
        }
    }
    return true;
}

BOOL CALLBACK device_find_callback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
{
    device_enum_param* parameter { (device_enum_param*)pvRef };
    LPDIRECTINPUTDEVICE8 device { nullptr };

    // Finish if already found.
    if (parameter->find_count_ >= 1)
    {
        return DIENUM_STOP;
    }

    // Create device.
    HRESULT hresult { JoyStick::direct_input8_->CreateDevice(
        lpddi->guidInstance,
        parameter->arg_input_device8_,
        nullptr) };

    if (FAILED(hresult))
    {
        return DIENUM_STOP;
    }

    // Set format.
    device = *parameter->arg_input_device8_;
    hresult = device->SetDataFormat(&c_dfDIJoystick);

    if (FAILED(hresult))
    {
        return DIENUM_STOP;
    }

    if (setup_gamepad_property(device) == false)
    {
        return DIENUM_STOP;
    }

    // Set Cooperative level
    if (FAILED(device->SetCooperativeLevel(
        parameter->hwnd_,
        DISCL_EXCLUSIVE | DISCL_FOREGROUND)))
    {
        return DIENUM_STOP;
    }

    ++parameter->find_count_;

    return DIENUM_CONTINUE;
}



void JoyStick::update()
{
    if (input_device8_ == nullptr)
    {
        return;
    }

    DIJOYSTATE pad_data { };

    // Get device.
    HRESULT hresult { input_device8_->GetDeviceState(sizeof(DIJOYSTATE), &pad_data) };
    if (FAILED(hresult))
    {
        // Start control again.
        if (FAILED(input_device8_->Acquire()))
        {
            input_device8_->Poll();
        }
        return;
    }

    JoyStickInfo temp { };
    temp.button_states_.insert({ eJoyStickButtonType::UP, eJoyStickButtonState::NONE });
    temp.button_states_.insert({ eJoyStickButtonType::DOWN, eJoyStickButtonState::NONE });
    temp.button_states_.insert({ eJoyStickButtonType::LEFT, eJoyStickButtonState::NONE });
    temp.button_states_.insert({ eJoyStickButtonType::RIGHT, eJoyStickButtonState::NONE });
    temp.button_states_.insert({ eJoyStickButtonType::X, eJoyStickButtonState::NONE });
    temp.button_states_.insert({ eJoyStickButtonType::Y, eJoyStickButtonState::NONE });
    temp.button_states_.insert({ eJoyStickButtonType::A, eJoyStickButtonState::NONE });
    temp.button_states_.insert({ eJoyStickButtonType::B, eJoyStickButtonState::NONE });
    button_deque_.push_front(temp);

    if (button_deque_.size() > Common::KEY_DEQUE_MAX_SIZE)
    {
        button_deque_.erase(button_deque_.begin() + Common::KEY_DEQUE_MAX_SIZE,
            button_deque_.end());
    }

    unordered_map<eJoyStickButtonType, bool> is_push_map {
        { eJoyStickButtonType::UP, false },
        { eJoyStickButtonType::DOWN, false },
        { eJoyStickButtonType::LEFT, false },
        { eJoyStickButtonType::RIGHT, false },
        { eJoyStickButtonType::X, false },
        { eJoyStickButtonType::Y, false },
        { eJoyStickButtonType::A, false },
        { eJoyStickButtonType::B, false },
    };

    // Check stick.
    int unresponsive_range { 200 };
    if (pad_data.lX < -unresponsive_range)
    {
        is_push_map.at(eJoyStickButtonType::LEFT) = true;
    }
    else if (pad_data.lX > unresponsive_range)
    {
        is_push_map.at(eJoyStickButtonType::RIGHT) = true;
    }

    if (pad_data.lY < -unresponsive_range)
    {
        is_push_map.at(eJoyStickButtonType::UP) = true;
    }
    else if (pad_data.lY > unresponsive_range)
    {
        is_push_map.at(eJoyStickButtonType::DOWN) = true;
    }

    // Check D-pad
    if (pad_data.rgdwPOV[0] != 0xFFFFFFFF)
    {
        float rad { D3DXToRadian(pad_data.rgdwPOV[0] / 100.0f) };
        float x { sinf(rad) };
        float y { cosf(rad) };

        if (x < -0.01f)
        {
            is_push_map.at(eJoyStickButtonType::LEFT) = true;
        }
        else if (x > 0.01f)
        {
            is_push_map.at(eJoyStickButtonType::RIGHT) = true;
        }

        if (y > 0.01f)
        {
            is_push_map.at(eJoyStickButtonType::UP) = true;
        }
        else if (y < -0.01f)
        {
            is_push_map.at(eJoyStickButtonType::DOWN) = true;
        }
    }

    // Check button.
    for (int i = 0; i < 32; i++)
    {
        if (!(pad_data.rgbButtons[i] & 0x80))
        {
            continue;
        }

        switch (i)
        {
        case 0:
            is_push_map.at(eJoyStickButtonType::X) = true;
            break;
        case 1:
            is_push_map.at(eJoyStickButtonType::A) = true;
            break;
        case 2:
            is_push_map.at(eJoyStickButtonType::B) = true;
            break;
        case 3:
            is_push_map.at(eJoyStickButtonType::Y) = true;
            break;
        }
    }

    // Update button status.
    for (auto& _is_push : is_push_map)
    {
        if (_is_push.second == true)
        {
            if (button_deque_.at(1).button_states_.at(_is_push.first) == eJoyStickButtonState::NONE)
            {
                button_deque_.at(0).button_states_.at(_is_push.first) = eJoyStickButtonState::DOWN;
            }
            else
            {
                button_deque_.at(0).button_states_.at(_is_push.first) = eJoyStickButtonState::HOLD;
            }
        }
        else
        {
            if (button_deque_.at(1).button_states_.at(_is_push.first) == eJoyStickButtonState::HOLD)
            {
                button_deque_.at(0).button_states_.at(_is_push.first) = eJoyStickButtonState::UP;
            }
            else
            {
                button_deque_.at(0).button_states_.at(_is_push.first) = eJoyStickButtonState::NONE;
            }
        }
    }
}

bool JoyStick::is_hold(eJoyStickButtonType button)
{
    if (button_deque_.at(0).button_states_.at(button) == eJoyStickButtonState::HOLD)
    {
        return true;
    }
    return false;
}

bool JoyStick::is_up(eJoyStickButtonType button)
{
    if (button_deque_.at(0).button_states_.at(button) == eJoyStickButtonState::UP)
    {
        return true;
    }
    return false;
}

bool JoyStick::is_down(eJoyStickButtonType button)
{
    if (button_deque_.at(0).button_states_.at(button) == eJoyStickButtonState::DOWN)
    {
        return true;
    }
    return false;
}

bool JoyStick::check_simultaneous(eJoyStickButtonType button)
{
    for (std::size_t i = 0; i < Common::SIMULTANEOUS_ALLOW_FRAME; ++i)
    {
        if (i >= button_deque_.size())
        {
            break;
        }
        if (button_deque_.at(i).button_states_.at(button) == eJoyStickButtonState::DOWN)
        {
            return true;
        }
    }
    return false;
}

bool JoyStick::create_device(HWND hwnd)
{
    device_enum_param parameter = { 0 };

    LPDIRECTINPUTDEVICE8 temp_device { nullptr };
    parameter.arg_input_device8_ = &temp_device;
    parameter.hwnd_ = hwnd;

    // Search GAMEPAD
    direct_input8_->EnumDevices(
        DI8DEVTYPE_GAMEPAD,
        device_find_callback,
        &parameter,
        DIEDFL_ATTACHEDONLY
    );

    // Search JOYSTICK
    direct_input8_->EnumDevices(
        DI8DEVTYPE_JOYSTICK,
        device_find_callback,
        &parameter,
        DIEDFL_ATTACHEDONLY
    );

    if (parameter.find_count_ == 0)
    {
        return false;
    }

    SAFE_RELEASE(input_device8_);
    input_device8_ = temp_device;

    int count { 0 };
    while (start_game_pad_control() == false)
    {
        Sleep(100);
        count++;
        if (count >= 5)
        {
            break;
        }
    }
    return true;
}

BOOL setup_gamepad_property(LPDIRECTINPUTDEVICE8 device)
{
    // Set coordinate type to abusolute value mode.
    DIPROPDWORD diprop;
    ZeroMemory(&diprop, sizeof(diprop));
    diprop.diph.dwSize = sizeof(diprop);
    diprop.diph.dwHeaderSize = sizeof(diprop.diph);
    diprop.diph.dwHow = DIPH_DEVICE;
    diprop.diph.dwObj = 0;
    diprop.dwData = DIPROPAXISMODE_ABS;
    if (FAILED(device->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
    {
        return false;
    }

    // Set X ranges
    DIPROPRANGE diprg;
    ZeroMemory(&diprg, sizeof(diprg));
    diprg.diph.dwSize = sizeof(diprg);
    diprg.diph.dwHeaderSize = sizeof(diprg.diph);
    diprg.diph.dwHow = DIPH_BYOFFSET;
    diprg.diph.dwObj = DIJOFS_X;
    diprg.lMin = -1000;
    diprg.lMax = 1000;
    if (FAILED(device->SetProperty(DIPROP_RANGE, &diprg.diph)))
    {
        return false;
    }

    // Set Y ranges.
    diprg.diph.dwObj = DIJOFS_Y;
    if (FAILED(device->SetProperty(DIPROP_RANGE, &diprg.diph)))
    {
        return false;
    }
    return true;
}

