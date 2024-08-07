#include "Mouse.h"
#include <exception>

LPDIRECTINPUTDEVICE8 Mouse::m_DIMouse;
DIMOUSESTATE Mouse::m_DIMouseState;

void Mouse::Init(LPDIRECTINPUT8 directInput, HWND hWnd)
{
    HRESULT ret = directInput->CreateDevice(GUID_SysMouse, &m_DIMouse, NULL);

    ret = m_DIMouse->SetDataFormat(&c_dfDIMouse);
    ret = m_DIMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

    // �f�o�C�X�̐ݒ�
    DIPROPDWORD diprop;
    diprop.diph.dwSize = sizeof(diprop);
    diprop.diph.dwHeaderSize = sizeof(diprop.diph);
    diprop.diph.dwObj = 0;
    diprop.diph.dwHow = DIPH_DEVICE;
    diprop.dwData = DIPROPAXISMODE_REL;	// ���Βl���[�h�Őݒ�i��Βl��DIPROPAXISMODE_ABS�j

    ret = m_DIMouse->SetProperty(DIPROP_AXISMODE, &diprop.diph);

    if (FAILED(ret))
    {
        throw std::exception("Failed initialize mouse");
    }
    ret = m_DIMouse->Acquire();
}

void Mouse::Update()
{
    HRESULT ret = m_DIMouse->Acquire();

    // �ǎ�O�̒l��ێ����܂�
    DIMOUSESTATE g_zdiMouseState_bak;	// �}�E�X���(�ω����m�p)
    memcpy(&g_zdiMouseState_bak, &m_DIMouseState, sizeof(g_zdiMouseState_bak));

    // ��������ADirectInput�ŕK�v�ȃR�[�h -->
        // �}�E�X�̏�Ԃ��擾���܂�
    HRESULT	hr = m_DIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_DIMouseState);
    if (hr == DIERR_INPUTLOST)
    {
        m_DIMouse->Acquire();
        hr = m_DIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_DIMouseState);
    }
    // --> �����܂ŁADirectInput�ŕK�v�ȃR�[�h

    if (memcmp(&g_zdiMouseState_bak, &m_DIMouseState, sizeof(g_zdiMouseState_bak)) != 0)
    {
        // �m�F�p�̏����A�������� -->
                // �l���ς������\�����܂�
        char buf[128];
        wsprintf(buf, "(%5d, %5d, %5d) %s %s %s\n",
            m_DIMouseState.lX, m_DIMouseState.lY, m_DIMouseState.lZ,
            (m_DIMouseState.rgbButtons[0] & 0x80) ? "Left" : "--",
            (m_DIMouseState.rgbButtons[1] & 0x80) ? "Right" : "--",
            (m_DIMouseState.rgbButtons[2] & 0x80) ? "Center" : "--");
        OutputDebugString(buf);
        // --> �����܂ŁA�m�F�p�̏���
    }
}

LONG Mouse::GetX()
{
    return m_DIMouseState.lX;
}

LONG Mouse::GetY()
{
    return m_DIMouseState.lY;
}

bool Mouse::IsDownLeft()
{
    if (m_DIMouseState.rgbButtons[0] & 0x80)
    {
        return true;
    }
    return false;
}

bool Mouse::IsDownRight()
{
    if (m_DIMouseState.rgbButtons[1] & 0x80)
    {
        return true;
    }
    return false;
}
