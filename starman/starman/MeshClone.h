#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

// ����3D���f�����R�\���������Ƃ��Ɏg���N���X
// ����3D���f�����R�\���������Ƃ��A3D���f���̓ǂݍ��݂�1�x�ł悭
// ���W�A��]�A�g�嗦���Đݒ肵�čĕ\�����邱�ƂŌy�ʉ�����B
class MeshClone
{
public:
    enum class eMeshType
    {
        TREE,
        GRASS,
        OTHER,
    };

    MeshClone(
        const std::string&,
        const D3DXVECTOR3&,
        const D3DXVECTOR3&,
        const float scale,
        const float radius = 20.f);
    ~MeshClone();

    void Init();
    void SetPos(const D3DXVECTOR3& pos);
    D3DXVECTOR3 GetPos() const;
    float GetScale() const;
    void Render();
    void Begin();
    void Render2();
    void End();

    eMeshType GetMeshType() const;

    LPD3DXMESH GetD3DMesh() const;

    void SetWeapon(const bool arg);

    float GetRadius() const;

    std::string GetMeshName();
    bool ContainMeshName(const std::string& arg);

private:
    const std::string SHADER_FILENAME { "res\\shader\\mesh_shader.fx" };

    // TODO �G�t�F�N�g�t�@�C���̓��b�V���̐������p�ӂ���ׂ�
    static std::unordered_map<std::string, LPD3DXEFFECT> m_D3DEffectMap;
    static std::unordered_map<std::string, LPD3DXMESH> m_D3DMeshMap;
    static std::unordered_map<std::string, std::vector<LPDIRECT3DTEXTURE9>> m_vecTextureMap;
    static std::unordered_map<std::string, DWORD> m_materialCountMap;
    static std::unordered_map<std::string, std::vector<D3DXVECTOR4>> m_vecColorMap;

    // �ŏ��Ɉ�񂾂����΂�������
    static std::unordered_map<std::string, bool> m_bFirstMap;

    D3DXVECTOR3 m_loadingPos { };
    D3DXVECTOR3 m_rotate { };

    D3DXVECTOR3 m_centerPos { 0.0f, 0.0f, 0.0f };
    float m_scale { 0.0f };

    float m_radius = 0.f;

    std::string m_meshName { "" };
    bool m_bIsInit { false };

    bool m_bWeapon = false;

    bool m_bPointLightEnablePrevious = false;

    eMeshType m_eMeshType = eMeshType::OTHER;
};

