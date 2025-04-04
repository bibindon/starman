#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include <vector>
#include <memory>

class Mesh
{
public:
    Mesh(const std::string&,
         const D3DXVECTOR3&,
         const D3DXVECTOR3&,
         const float,
         const float = -1.f);

    // �V�F�[�_�[�t�@�C�����w��ł���R���X�g���N�^
    Mesh(const std::string&,
         const std::string&,
         const D3DXVECTOR3&,
         const D3DXVECTOR3&,
         const float,
         const float = -1.f);

    ~Mesh();

    void Init();
    void SetPos(const D3DXVECTOR3& pos);
    D3DXVECTOR3 GetPos();
    void SetRotY(const float rotY);
    float GetScale();
    void Render();
    LPD3DXMESH GetD3DMesh() const;
    void SetWeapon(const bool arg);
    float GetRadius() const;

private:
    const bool FLAT_SHADING = false;
    const std::string SHADER_FILENAME = "res\\shader\\mesh_shader.fx";
    std::string m_meshName;

    LPD3DXEFFECT m_D3DEffect = nullptr;
    LPD3DXMESH m_D3DMesh = nullptr;

    DWORD m_materialCount = 0;
    std::vector<D3DXVECTOR4> m_vecDiffuse;
    std::vector<LPDIRECT3DTEXTURE9> m_vecTexture;

    D3DXVECTOR3 m_pos { };
    D3DXVECTOR3 m_rotate { };

    //-------------------------------------------------
    // ���̕��̂̔��a
    // �v���C���[�����̔��a�ȓ��ɋ߂Â����炱�̕��̂͏Փ˔���̑ΏۂƂȂ�
    // -1��������K���Փ˔���̑Ώۂɂ���
    //-------------------------------------------------
    float m_radius = 0.0f;

    float m_scale = 0.0f;
    bool m_bLoaded = false;

    bool m_bWeapon = false;
};


