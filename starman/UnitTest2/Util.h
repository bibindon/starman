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

class Util
{
public:

    // ウィンドウ作成とDirctX9の初期化、DirectInputの初期化を行う
    static void InitWin_DX9_DI8(const bool bShow = false);
    static void ReleaseWin_DX9_DI8();

private:

    static LPDIRECT3D9 m_D3D;
    static LPDIRECTINPUT8 m_directInput;

};

