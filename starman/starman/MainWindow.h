#pragma once

#include <windows.h>
#include <tchar.h>
#include <d3d9.h>
#include <d3dx9.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

class MainWindow
{
public:
    explicit MainWindow(const HINSTANCE&);
    ~MainWindow();
    int MainLoop();

private:
    MSG msg;
    HWND hWnd;
    LPDIRECT3D9 g_pD3D;
    LPDIRECT3DDEVICE9 g_pD3DDev;
    D3DLIGHT9 light;
    ID3DXBuffer* pMaterials;
    DWORD NumMaterials;
    ID3DXMesh* pMesh;
    ID3DXBuffer* pAX_Materials;
    DWORD AX_NumMaterials;
    ID3DXMesh* pAX_Mesh;

    LPDIRECTINPUT8 m_directInput = NULL;
};

