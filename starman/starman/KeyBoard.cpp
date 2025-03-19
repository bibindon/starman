#include "KeyBoard.h"

LPDIRECTINPUTDEVICE8 KeyBoard::m_keyboard;
BYTE KeyBoard::m_key[256];
BYTE KeyBoard::m_keyPrev[256];
std::deque<std::vector<BYTE>> KeyBoard::m_keyDeque;
bool KeyBoard::m_testMode = false;
bool KeyBoard::m_keyIsDown[256] = {0};
bool KeyBoard::m_keyIsDownFirstFrame[256] = {0};
bool KeyBoard::m_keyIsHold[256] = {0};

void KeyBoard::Init(LPDIRECTINPUT8 directInput, HWND hWnd)
{
    HRESULT ret = directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);

    ret = m_keyboard->SetDataFormat(&c_dfDIKeyboard);

    // 排他制御のセット
    ret = m_keyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);

    // 動作開始
    ret = m_keyboard->Acquire();
}

void KeyBoard::Update()
{
    // キーの入力
    memcpy(m_keyPrev, m_key, 256);
    ZeroMemory(m_key, sizeof(m_key));
    HRESULT ret = m_keyboard->GetDeviceState(sizeof(m_key), m_key);
    if (FAILED(ret))
    {
        // 失敗なら再開させてもう一度取得
        m_keyboard->Acquire();
        m_keyboard->GetDeviceState(sizeof(m_key), m_key);
    }

    std::vector<BYTE> temp(256);
    std::copy(&m_key[0], &m_key[256], temp.begin());
    m_keyDeque.push_front(temp);

    // 5秒分のキー情報、以上のキー情報が保存されているなら消す
    if (m_keyDeque.size() >= 60 * 5)
    {
        m_keyDeque.erase(m_keyDeque.begin() + 60 * 5, m_keyDeque.end());
    }
}

void KeyBoard::Finalize()
{
    m_keyboard->Release();
}

bool KeyBoard::IsDown(int keyCode)
{
    if (m_testMode)
    {
        return m_keyIsDown[keyCode];
    }

    if (m_key[keyCode] & 0x80)
    {
        return true;
    }
    return false;
}

bool KeyBoard::IsDownFirstFrame(int keyCode)
{
    if (m_testMode)
    {
        return m_keyIsDownFirstFrame[keyCode];
    }

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
    if (m_testMode)
    {
        return m_keyIsHold[keyCode];
    }
    
    // 500ミリ秒以上押されていたら長押しと判断する
    if (m_keyDeque.size() <= 30)
    {
        return false;
    }

    bool isHold = true;
    for (std::size_t i = 0; i < 30; ++i)
    {
        if (m_keyDeque.at(i).at((std::size_t)keyCode) & 0x80)
        {
            continue;
        }
        else
        {
            isHold = false;
            break;
        }
    }

    if (isHold)
    {
        return true;
    }

    return isHold;
}

void KeyBoard::ForTest_SetTestMode()
{
    m_testMode = true;
}

void KeyBoard::ForTest_SetIsDown(int keyCode)
{
    m_keyIsDown[keyCode] = true;
}

void KeyBoard::ForTest_SetIsDownFirstFrame(int keyCode)
{
    m_keyIsDownFirstFrame[keyCode] = true;
}

void KeyBoard::ForTest_SetIsHold(int keyCode)
{
    m_keyIsHold[keyCode] = true;
}

void KeyBoard::ForTest_Clear()
{
    std::fill(&m_keyIsDown[0], &m_keyIsDown[256], false);
    std::fill(&m_keyIsDownFirstFrame[0], &m_keyIsDownFirstFrame[256], false);
    std::fill(&m_keyIsHold[0], &m_keyIsHold[256], false);
}

