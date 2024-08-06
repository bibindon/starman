#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include <vector>
#include <memory>
#include "AnimationStrategy.h"

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

    void render(const D3DXMATRIX&, const D3DXMATRIX&);

private:
    const static std::string SHADER_FILENAME;
    LPD3DXMESH d3dx_mesh_ { };
    DWORD materials_count_ { 0 };
    D3DXHANDLE world_view_proj_handle_ { nullptr };
    std::vector<D3DCOLORVALUE> colors_ { };
    std::vector<LPDIRECT3DTEXTURE9> textures_ { };
    D3DXVECTOR3 center_coodinate_ { 0.0f, 0.0f, 0.0f };
    float radius_ { 0.0f };
    float scale_ { 0.0f };
    std::string mesh_name_ { };

    LPDIRECT3DDEVICE9 d3d_device_;
    LPD3DXEFFECT effect_;

    D3DXVECTOR3 position_;
    D3DXVECTOR3 rotation_;

    D3DXHANDLE mesh_texture_handle_;
    D3DXHANDLE diffuse_handle_;

    D3DXHANDLE light_normal_handle_;
    D3DXHANDLE brightness_handle_;

};


