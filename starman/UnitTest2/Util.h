#pragma once

#include <windows.h>
#include <tchar.h>
#include <d3d9.h>
#include <d3dx9.h>

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif

#include <dinput.h>
#include <string>
#include <chrono>

#include "..\\starman\PopUp2.h"
#include "..\\starman\KeyBoard.h"

class Util
{
public:

    // �E�B���h�E�쐬��DirctX9�̏������ADirectInput�̏��������s��
    static void InitWin_DX9_DI8(const bool bShow = false);
    static void ReleaseWin_DX9_DI8();

    static void DestroyLibData();
    static BOOL DeleteDirectory(LPCTSTR lpPathName);
private:

    static LPDIRECT3D9 m_D3D;
    static LPDIRECTINPUT8 m_directInput;

};

class MockPopUpFont : public IPopUpFont
{
public:

    void Draw(const std::string& text, const int transparent);
    std::string GetShowText();

private:

    std::string m_text;
};

class MockKeyBoard : public IKeyBoard
{
public:
    void Init(LPDIRECTINPUT8 directInput, HWND hWnd) override;
    void Update() override;
    void Finalize() override;
    bool IsDown(int keyCode) override;
    bool IsDownFirstFrame(int keyCode) override;
    bool IsHold(int keyCode) override;

    void SetKeyDown(int keyCode);
    void SetKeyDownFirst(int keyCode);
    void SetKeyHold(int keyCode);
    void ClearAll();
    void SetAltF4();

private:

    bool m_bDown[256] = { };
    bool m_bDownFirst[256] = { };
    bool m_bHold[256] = { };
    bool m_bAltF4 = false;

};

