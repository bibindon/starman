#pragma once

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif

#include <dinput.h>
#include <deque>
#include <vector>

class IKeyBoard
{
public:
    virtual void Init(LPDIRECTINPUT8 directInput, HWND hWnd) = 0;
    virtual void Update() = 0;
    virtual void Finalize() = 0;

    // ������Ă���
    virtual bool IsDown(int keyCode) = 0;

    // ������Ă��āA���̒��O�܂ŉ�����Ă��Ȃ�����
    virtual bool IsDownFirstFrame(int keyCode) = 0;

    // 0.5�b�ȏ㒷�������Ă���
    virtual bool IsHold(int keyCode) = 0;

};

class KeyBoard : public IKeyBoard
{
public:
    void Init(LPDIRECTINPUT8 directInput, HWND hWnd);
    void Update();
    void Finalize();

    // ������Ă���
    bool IsDown(int keyCode);

    // ������Ă��āA���̒��O�܂ŉ�����Ă��Ȃ�����
    bool IsDownFirstFrame(int keyCode);

    // 0.5�b�ȏ㒷�������Ă���
    bool IsHold(int keyCode);

private:
    LPDIRECTINPUTDEVICE8 m_keyboard;
    BYTE m_key[256];
    BYTE m_keyPrev[256];
    std::deque<std::vector<BYTE>> m_keyDeque;
};

