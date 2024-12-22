#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

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
        const float scale,
        const float radius = 20.f);
    ~MeshClone();

    void Init();
    void SetPos(const D3DXVECTOR3& pos);
    D3DXVECTOR3 GetPos();
    float GetScale();
    void Render();

    LPD3DXMESH GetD3DMesh();

    void SetWeapon(const bool arg);

    float GetRadius() const;

private:
    const std::string SHADER_FILENAME { "res\\shader\\mesh_shader.fx" };

    static LPD3DXEFFECT m_D3DEffect;
    static std::unordered_map<std::string, LPD3DXMESH> m_D3DMeshMap;
    static std::unordered_map<std::string, std::vector<LPDIRECT3DTEXTURE9>> m_vecTextureMap;
    static std::unordered_map<std::string, DWORD> m_materialCountMap;
    static std::unordered_map<std::string, std::vector<D3DXVECTOR4>> m_vecColorMap;

    D3DXVECTOR3 m_loadingPos { };
    D3DXVECTOR3 m_rotate { };

    D3DXVECTOR3 m_centerPos { 0.0f, 0.0f, 0.0f };
    float m_scale { 0.0f };

    float m_radius = 0.f;

    std::string m_meshName { "" };
    bool m_bIsInit { false };

    bool m_bWeapon = false;
};

