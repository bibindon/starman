#include "KeyBoard.h"

LPDIRECTINPUTDEVICE8 KeyBoard::m_keyboard;
BYTE KeyBoard::m_key[256];
BYTE KeyBoard::m_keyPrev[256];

void KeyBoard::Init(LPDIRECTINPUT8 directInput, HWND hWnd)
{
    HRESULT ret = directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);

    ret = m_keyboard->SetDataFormat(&c_dfDIKeyboard);

    // �r������̃Z�b�g
    ret = m_keyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);

    // ����J�n
    ret = m_keyboard->Acquire();
}

void KeyBoard::Update()
{
    // �L�[�̓���
    memcpy(m_keyPrev, m_key, 256);
    ZeroMemory(m_key, sizeof(m_key));
    HRESULT ret = m_keyboard->GetDeviceState(sizeof(m_key), m_key);
    if (FAILED(ret))
    {
        // ���s�Ȃ�ĊJ�����Ă�����x�擾
        m_keyboard->Acquire();
        m_keyboard->GetDeviceState(sizeof(m_key), m_key);
    }
}

bool KeyBoard::IsDown(int keyCode)
{
    if (m_key[keyCode] & 0x80)
    {
        if ((m_keyPrev[keyCode] & 0x80) == 0)
        {
            return true;
        }
    }
    return false;
}

bool KeyBoard::IsHold(int keyCode)
{
    if (m_key[keyCode] & 0x80)
    {
        return true;
    }
    return false;
}
