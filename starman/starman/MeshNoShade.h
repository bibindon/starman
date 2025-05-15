﻿#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include <vector>
#include <memory>

class MeshNoShade
{
public:
    MeshNoShade(
        const std::string&,
        const D3DXVECTOR3&,
        const D3DXVECTOR3&,
        const float&);
    ~MeshNoShade();

    void Init();
    void SetPos(const D3DXVECTOR3& pos);
    D3DXVECTOR3 GetPos();
    void Render();

    LPD3DXMESH GetD3DMesh();

private:
    const std::string SHADER_FILENAME { "res\\shader\\mesh_shader_no_shade.fx" };
    LPD3DXMESH m_D3DMesh { nullptr };

    LPD3DXEFFECT m_D3DEffect { nullptr };

    D3DXVECTOR3 m_loadingPos { };
    D3DXVECTOR3 m_rotate { };

    DWORD m_materialCount { 0 };
    std::vector<D3DCOLORVALUE> m_vecColor { };
    std::vector<LPDIRECT3DTEXTURE9> m_vecTexture { };
    D3DXVECTOR3 m_centerPos { 0.0f, 0.0f, 0.0f };
    float m_radius { 0.0f };
    float m_scale { 0.0f };
    std::string m_meshName { "" };
    bool m_bIsInit { false };
};


