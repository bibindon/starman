#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include <vector>
#include <memory>

class Mesh
{
public:
    Mesh(
        const LPDIRECT3DDEVICE9,
        const std::string&,
        const D3DXVECTOR3&,
        const D3DXVECTOR3&,
        const float&);
    ~Mesh();

    void Render(const D3DXMATRIX&, const D3DXMATRIX&);

private:
    const std::string SHADER_FILENAME { "res\\shader\\mesh_shader.fx" };
    LPD3DXMESH m_D3DMesh { nullptr };

    LPDIRECT3DDEVICE9 m_D3DDevice { nullptr };
    LPD3DXEFFECT m_Effect { nullptr };

    D3DXVECTOR3 m_Pos { };
    D3DXVECTOR3 m_Rotate { };

    D3DXHANDLE m_MeshTexture { nullptr };
    D3DXHANDLE m_Diffuse { nullptr };
    D3DXHANDLE m_LightNormal { nullptr };
    D3DXHANDLE m_Brightness { nullptr };

    DWORD m_MaterialCount { 0 };
    D3DXHANDLE m_WorldViewProj { nullptr };
    std::vector<D3DCOLORVALUE> m_vecColor { };
    std::vector<LPDIRECT3DTEXTURE9> m_vecTexture { };
    D3DXVECTOR3 m_CenterCoord { 0.0f, 0.0f, 0.0f };
    float m_Radius { 0.0f };
    float m_Scale { 0.0f };
    std::string m_MeshName { "" };
};


