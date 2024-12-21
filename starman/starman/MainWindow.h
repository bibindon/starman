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
#include <deque>

#include "Mesh.h"
#include "Sprite.h"
#include "AnimMesh.h"
#include "SeqTitle.h"
#include "SeqBattle.h"
#include "SeqOpening.h"
#include "SeqEnding.h"

class MainWindow
{
public:
    explicit MainWindow(const HINSTANCE&);
    ~MainWindow();
    int MainLoop();

    static eSequence GetSequence();

private:
    const std::string TITLE = "ホシマン";
    MSG m_msg {};
    HWND m_hWnd = NULL;
    LPDIRECT3D9 m_D3D = nullptr;
    D3DLIGHT9 light;

    LPDIRECTINPUT8 m_directInput { nullptr };
    Sprite* m_sprite { nullptr };

    // WM_CLOSEメッセージを受信したときに対処する必要があるためstatic
    static eSequence m_sequence;

    SeqTitle* m_seqTitle { nullptr };
    SeqOpening* m_seqOpening { nullptr };
    SeqBattle* m_seqBattle { nullptr };
    SeqEnding* m_seqEnding { nullptr };

    LPD3DXFONT m_D3DFont { nullptr };
    std::vector<std::chrono::system_clock::time_point> m_vecTime;

};

