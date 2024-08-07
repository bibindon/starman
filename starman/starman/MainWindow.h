#pragma once

#include <windows.h>
#include <tchar.h>
#include <d3d9.h>
#include <d3dx9.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include  <string>
#include "Mesh.h"
#include "Sprite.h"

class MainWindow
{
public:
    explicit MainWindow(const HINSTANCE&);
    ~MainWindow();
    int MainLoop();

private:
    const std::string TITLE = "ƒzƒVƒ}ƒ“";
    MSG m_msg;
    HWND m_hWnd;
    LPDIRECT3D9 m_D3D;
    LPDIRECT3DDEVICE9 m_D3DDevice;
    D3DLIGHT9 light;
    ID3DXBuffer* pMaterials;
    DWORD NumMaterials;
    ID3DXMesh* pMesh;
    ID3DXBuffer* pAX_Materials;
    DWORD AX_NumMaterials;
    ID3DXMesh* pAX_Mesh;

    Mesh* m_Mesh1 { nullptr };
    LPDIRECTINPUT8 m_directInput { nullptr };
    Sprite* m_sprite { nullptr };

};

