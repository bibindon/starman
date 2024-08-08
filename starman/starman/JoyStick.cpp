#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include "JoyStick.h"
#include "Common.h"
#include <d3dx9.h>

using std::unordered_map;
using std::deque;

BOOL CALLBACK DeviceFindCallback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);
BOOL SetupGamePadProperty(LPDIRECTINPUTDEVICE8 device);

LPDIRECTINPUT8 JoyStick::m_DI;
LPDIRECTINPUTDEVICE8 JoyStick::m_DIDevice;
deque<JoyStick::JoyStickInfo> JoyStick::m_deqButton;

bool JoyStick::Init(LPDIRECTINPUT8 DI, HWND hwnd)
{
    m_DI = DI;
    // Init button_queue_
    JoyStickInfo temp { };
    temp.m_buttonStatusMap.insert({ eJoyStickButtonType::UP, eJoyStickButtonState::NONE });
    temp.m_buttonStatusMap.insert({ eJoyStickButtonType::DOWN, eJoyStickButtonState::NONE });
    temp.m_buttonStatusMap.insert({ eJoyStickButtonType::LEFT, eJoyStickButtonState::NONE });
    temp.m_buttonStatusMap.insert({ eJoyStickButtonType::RIGHT, eJoyStickButtonState::NONE });
    temp.m_buttonStatusMap.insert({ eJoyStickButtonType::X, eJoyStickButtonState::NONE });
    temp.m_buttonStatusMap.insert({ eJoyStickButtonType::Y, eJoyStickButtonState::NONE });
    temp.m_buttonStatusMap.insert({ eJoyStickButtonType::A, eJoyStickButtonState::NONE });
    temp.m_buttonStatusMap.insert({ eJoyStickButtonType::B, eJoyStickButtonState::NONE });
    temp.m_buttonStatusMap.insert({ eJoyStickButtonType::Z_LEFT, eJoyStickButtonState::NONE });
    temp.m_buttonStatusMap.insert({ eJoyStickButtonType::Z_RIGHT, eJoyStickButtonState::NONE });
    m_deqButton.push_back(temp);
    m_deqButton.push_back(temp);
    m_deqButton.push_back(temp);
    m_deqButton.push_back(temp);
    m_deqButton.push_back(temp);

    // Create device.
    if (!CreateDevice(hwnd))
    {
        // It's not problem failing to create input device.
        Finalize();
        return false;
    }
    return true;
}

void JoyStick::Finalize()
{
    if (m_DIDevice != nullptr)
    {
        m_DIDevice->Unacquire();
    }
    SAFE_RELEASE(m_DIDevice);
}

// A struct that receives device enumeration result.
struct DeviceEnumParam
{
    LPDIRECTINPUTDEVICE8* m_argDIDevice;
    int m_findCount;
    HWND m_hWnd;
};

BOOL JoyStick::StartGamePadControl()
{
    if (m_DIDevice == nullptr)
    {
        return false;
    }

    if (FAILED(m_DIDevice->Acquire()))
    {
        return false;
    }

    DIDEVCAPS cap = { 0 };
    m_DIDevice->GetCapabilities(&cap);
    // Check polling
    if (cap.dwFlags & DIDC_POLLEDDATAFORMAT)
    {
        DWORD error = GetLastError();
        if (FAILED(m_DIDevice->Poll()))
        {
            return false;
        }
    }
    return true;
}

BOOL CALLBACK DeviceFindCallback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
{
    DeviceEnumParam* parameter { (DeviceEnumParam*)pvRef };
    LPDIRECTINPUTDEVICE8 DIDevice { nullptr };

    // Finish if already found.
    if (parameter->m_findCount >= 1)
    {
        return DIENUM_STOP;
    }

    // Create device.
    HRESULT hresult { JoyStick::m_DI->CreateDevice(
        lpddi->guidInstance,
        parameter->m_argDIDevice,
        nullptr) };

    if (FAILED(hresult))
    {
        return DIENUM_STOP;
    }

    // Set format.
    DIDevice = *parameter->m_argDIDevice;
    hresult = DIDevice->SetDataFormat(&c_dfDIJoystick);

    if (FAILED(hresult))
    {
        return DIENUM_STOP;
    }

    if (SetupGamePadProperty(DIDevice) == false)
    {
        return DIENUM_STOP;
    }

    // Set Cooperative level
    if (FAILED(DIDevice->SetCooperativeLevel(
        parameter->m_hWnd,
        DISCL_EXCLUSIVE | DISCL_FOREGROUND)))
    {
        return DIENUM_STOP;
    }

    ++parameter->m_findCount;

    return DIENUM_CONTINUE;
}



void JoyStick::Update()
{
    if (m_DIDevice == nullptr)
    {
        return;
    }

    DIJOYSTATE padData { };

    // Get device.
    HRESULT hresult { m_DIDevice->GetDeviceState(sizeof(DIJOYSTATE), &padData) };
    if (FAILED(hresult))
    {
        // Start control again.
        if (FAILED(m_DIDevice->Acquire()))
        {
            m_DIDevice->Poll();
        }
        return;
    }

    JoyStickInfo temp { };
    temp.m_buttonStatusMap.insert({ eJoyStickButtonType::UP, eJoyStickButtonState::NONE });
    temp.m_buttonStatusMap.insert({ eJoyStickButtonType::DOWN, eJoyStickButtonState::NONE });
    temp.m_buttonStatusMap.insert({ eJoyStickButtonType::LEFT, eJoyStickButtonState::NONE });
    temp.m_buttonStatusMap.insert({ eJoyStickButtonType::RIGHT, eJoyStickButtonState::NONE });
    temp.m_buttonStatusMap.insert({ eJoyStickButtonType::X, eJoyStickButtonState::NONE });
    temp.m_buttonStatusMap.insert({ eJoyStickButtonType::Y, eJoyStickButtonState::NONE });
    temp.m_buttonStatusMap.insert({ eJoyStickButtonType::A, eJoyStickButtonState::NONE });
    temp.m_buttonStatusMap.insert({ eJoyStickButtonType::B, eJoyStickButtonState::NONE });
    temp.m_buttonStatusMap.insert({ eJoyStickButtonType::Z_LEFT, eJoyStickButtonState::NONE });
    temp.m_buttonStatusMap.insert({ eJoyStickButtonType::Z_RIGHT, eJoyStickButtonState::NONE });
    m_deqButton.push_front(temp);

    if (m_deqButton.size() > Common::KEY_DEQUE_MAX_SIZE)
    {
        m_deqButton.erase(m_deqButton.begin() + Common::KEY_DEQUE_MAX_SIZE,
            m_deqButton.end());
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
        { eJoyStickButtonType::Z_LEFT, false },
        { eJoyStickButtonType::Z_RIGHT, false },
    };

    // Check stick.
    int unresponsiveRange { 200 };
    if (padData.lX < -unresponsiveRange)
    {
        is_push_map.at(eJoyStickButtonType::LEFT) = true;
    }
    else if (padData.lX > unresponsiveRange)
    {
        is_push_map.at(eJoyStickButtonType::RIGHT) = true;
    }

    if (padData.lY < -unresponsiveRange)
    {
        is_push_map.at(eJoyStickButtonType::UP) = true;
    }
    else if (padData.lY > unresponsiveRange)
    {
        is_push_map.at(eJoyStickButtonType::DOWN) = true;
    }

    if (padData.lZ < -unresponsiveRange)
    {
        is_push_map.at(eJoyStickButtonType::Z_LEFT) = true;
    }
    else if (padData.lZ > unresponsiveRange)
    {
        is_push_map.at(eJoyStickButtonType::Z_RIGHT) = true;
    }

    // Check D-pad
    if (padData.rgdwPOV[0] != 0xFFFFFFFF)
    {
        float rad { D3DXToRadian(padData.rgdwPOV[0] / 100.0f) };
        float x { sinf(rad) };
        float y { cosf(rad) };
        float zrad { D3DXToRadian(padData.rgdwPOV[1] / 100.0f) };
        float z { sinf(zrad) };

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
        if (z > 0.01f)
        {
            is_push_map.at(eJoyStickButtonType::Z_LEFT) = true;
        }
        else if (z < -0.01f)
        {
            is_push_map.at(eJoyStickButtonType::Z_RIGHT) = true;
        }
    }

    // Check button.
    for (int i = 0; i < 32; i++)
    {
        if (!(padData.rgbButtons[i] & 0x80))
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
            if (m_deqButton.at(1).m_buttonStatusMap.at(_is_push.first) == eJoyStickButtonState::NONE)
            {
                m_deqButton.at(0).m_buttonStatusMap.at(_is_push.first) = eJoyStickButtonState::DOWN;
            }
            else
            {
                m_deqButton.at(0).m_buttonStatusMap.at(_is_push.first) = eJoyStickButtonState::HOLD;
            }
        }
        else
        {
            if (m_deqButton.at(1).m_buttonStatusMap.at(_is_push.first) == eJoyStickButtonState::HOLD)
            {
                m_deqButton.at(0).m_buttonStatusMap.at(_is_push.first) = eJoyStickButtonState::UP;
            }
            else
            {
                m_deqButton.at(0).m_buttonStatusMap.at(_is_push.first) = eJoyStickButtonState::NONE;
            }
        }
    }
}

bool JoyStick::IsHold(eJoyStickButtonType button)
{
    if (m_deqButton.at(0).m_buttonStatusMap.at(button) == eJoyStickButtonState::HOLD)
    {
        return true;
    }
    return false;
}

bool JoyStick::IsUp(eJoyStickButtonType button)
{
    if (m_deqButton.at(0).m_buttonStatusMap.at(button) == eJoyStickButtonState::UP)
    {
        return true;
    }
    return false;
}

bool JoyStick::IsDown(eJoyStickButtonType button)
{
    if (m_deqButton.at(0).m_buttonStatusMap.at(button) == eJoyStickButtonState::DOWN)
    {
        return true;
    }
    return false;
}

bool JoyStick::CheckSimultaneous(eJoyStickButtonType button)
{
    for (std::size_t i = 0; i < Common::SIMULTANEOUS_ALLOW_FRAME; ++i)
    {
        if (i >= m_deqButton.size())
        {
            break;
        }
        if (m_deqButton.at(i).m_buttonStatusMap.at(button) == eJoyStickButtonState::DOWN)
        {
            return true;
        }
    }
    return false;
}

bool JoyStick::CreateDevice(HWND hwnd)
{
    DeviceEnumParam parameter = { 0 };

    LPDIRECTINPUTDEVICE8 tempDIDevice { nullptr };
    parameter.m_argDIDevice = &tempDIDevice;
    parameter.m_hWnd = hwnd;

    // Search GAMEPAD
    m_DI->EnumDevices(
        DI8DEVTYPE_GAMEPAD,
        DeviceFindCallback,
        &parameter,
        DIEDFL_ATTACHEDONLY
    );

    // Search JOYSTICK
    m_DI->EnumDevices(
        DI8DEVTYPE_JOYSTICK,
        DeviceFindCallback,
        &parameter,
        DIEDFL_ATTACHEDONLY
    );

    if (parameter.m_findCount == 0)
    {
        return false;
    }

    SAFE_RELEASE(m_DIDevice);
    m_DIDevice = tempDIDevice;

    int count { 0 };
    while (StartGamePadControl() == false)
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

BOOL SetupGamePadProperty(LPDIRECTINPUTDEVICE8 device)
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

    // Set Z ranges.
    diprg.diph.dwObj = DIJOFS_Z;
    if (FAILED(device->SetProperty(DIPROP_RANGE, &diprg.diph)))
    {
        return false;
    }
    return true;
}

