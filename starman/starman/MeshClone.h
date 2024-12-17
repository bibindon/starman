#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <map>
#include <string>
#include <vector>
#include <memory>

// 同じ3Dモデルを沢山表示したいときに使うクラス
// 同じ3Dモデルを沢山表示したいとき、3Dモデルの読み込みは1度でよく
// 座標、回転、拡大率を再設定して再表示することで軽量化する。
class MeshClone
{
public:
    MeshClone(
        const std::string&,
        const D3DXVECTOR3&,
        const D3DXVECTOR3&,
        const float&);
    ~MeshClone();

    void Init();
    void SetPos(const D3DXVECTOR3& pos);
    D3DXVECTOR3 GetPos();
    void Render();

    LPD3DXMESH GetD3DMesh();

    void SetWeapon(const bool arg);

private:
    const std::string SHADER_FILENAME { "res\\shader\\mesh_shader.fx" };

    static LPD3DXEFFECT m_D3DEffect;
    static std::map<std::string, LPD3DXMESH> m_D3DMeshMap;
    static std::map<std::string, std::vector<LPDIRECT3DTEXTURE9>> m_vecTextureMap;
    static std::map<std::string, DWORD> m_materialCountMap;
    static std::map<std::string, std::vector<D3DCOLORVALUE>> m_vecColorMap;

    D3DXVECTOR3 m_loadingPos { };
    D3DXVECTOR3 m_rotate { };

    D3DXVECTOR3 m_centerPos { 0.0f, 0.0f, 0.0f };
    float m_scale { 0.0f };
    std::string m_meshName { "" };
    bool m_bIsInit { false };

    bool m_bWeapon = false;
};

