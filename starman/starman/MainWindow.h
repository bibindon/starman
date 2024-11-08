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

private:
    const std::string TITLE = "�z�V�}��";
    MSG m_msg;
    HWND m_hWnd;
    LPDIRECT3D9 m_D3D;
    D3DLIGHT9 light;
    ID3DXBuffer* pMaterials;
    DWORD NumMaterials;
    ID3DXMesh* pMesh;
    ID3DXBuffer* pAX_Materials;
    DWORD AX_NumMaterials;
    ID3DXMesh* pAX_Mesh;

    LPDIRECTINPUT8 m_directInput { nullptr };
    Sprite* m_sprite { nullptr };

    eSequence m_sequence { eSequence::TITLE };
    SeqTitle* m_seqTitle { nullptr };
    SeqOpening* m_seqOpening { nullptr };
    SeqBattle* m_seqBattle { nullptr };
    SeqEnding* m_seqEnding { nullptr };

    LPD3DXFONT m_D3DFont { nullptr };
    std::vector<std::chrono::system_clock::time_point> m_vecTime;
};

