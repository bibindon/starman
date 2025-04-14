#include "Mesh.h"

#include <cassert>

#include "Light.h"
#include "Common.h"
#include "Camera.h"
#include "SharedObj.h"
#include "Rain.h"

#include "../../StarmanLib/StarmanLib/StarmanLib/WeaponManager.h"

Mesh::Mesh(const std::string& xFilename,
           const D3DXVECTOR3& position,
           const D3DXVECTOR3& rotation,
           const float scale,
           const float radius)
    : m_meshName(xFilename)
    , m_pos(position)
    , m_rotate(rotation)
    , m_scale(scale)
    , m_radius(radius)
{
}

// �V�F�[�_�[�t�@�C�����w��ł���R���X�g���N�^
Mesh::Mesh(const std::string& shaderName,
           const std::string& xFilename,
           const D3DXVECTOR3& position,
           const D3DXVECTOR3& rotation,
           const float scale,
           const float radius)
    : SHADER_FILENAME(shaderName)
    , m_meshName(xFilename)
    , m_pos(position)
    , m_rotate(rotation)
    , m_scale(scale)
    , m_radius(radius)
{
}

Mesh::~Mesh()
{
    for (size_t i = 0; i < m_vecTexture.size(); ++i)
    {
        SAFE_RELEASE(m_vecTexture.at(i));
    }

    SAFE_RELEASE(m_D3DMesh);
    SAFE_RELEASE(m_D3DEffect);
}

void Mesh::Init()
{
    HRESULT hResult = E_FAIL;

    //--------------------------------------------------------
    // �G�t�F�N�g�̍쐬
    //--------------------------------------------------------
    hResult = D3DXCreateEffectFromFile(SharedObj::GetD3DDevice(),
                                      SHADER_FILENAME.c_str(),
                                      nullptr,
                                      nullptr,
                                      0,
                                      nullptr,
                                      &m_D3DEffect,
                                      nullptr);

    assert(hResult == S_OK);

    //--------------------------------------------------------
    // X�t�@�C���̓ǂݍ���
    //--------------------------------------------------------
    LPD3DXBUFFER adjacencyBuffer = nullptr;
    LPD3DXBUFFER materialBuffer = nullptr;

    hResult = D3DXLoadMeshFromX(m_meshName.c_str(),
                               D3DXMESH_SYSTEMMEM,
                               SharedObj::GetD3DDevice(),
                               &adjacencyBuffer,
                               &materialBuffer,
                               nullptr,
                               &m_materialCount,
                               &m_D3DMesh);

    assert(hResult == S_OK);

    //--------------------------------------------------------
    // �@�����������b�V���t�@�C���ɕϊ�
    //--------------------------------------------------------
    D3DVERTEXELEMENT9 decl[4];
    {
        decl[0].Stream = 0;
        decl[0].Offset = 0;
        decl[0].Type = D3DDECLTYPE_FLOAT3;
        decl[0].Method = D3DDECLMETHOD_DEFAULT;
        decl[0].Usage = D3DDECLUSAGE_POSITION;
        decl[0].UsageIndex = 0;

        decl[1].Stream = 0;
        decl[1].Offset = 12;
        decl[1].Type = D3DDECLTYPE_FLOAT3;
        decl[1].Method = D3DDECLMETHOD_DEFAULT;
        decl[1].Usage = D3DDECLUSAGE_NORMAL;
        decl[1].UsageIndex = 0;

        decl[2].Stream = 0;
        decl[2].Offset = 24;
        decl[2].Type = D3DDECLTYPE_FLOAT2;
        decl[2].Method = D3DDECLMETHOD_DEFAULT;
        decl[2].Usage = D3DDECLUSAGE_TEXCOORD;
        decl[2].UsageIndex = 0;

        decl[3] = D3DDECL_END();
    }


    LPD3DXMESH tempMesh = nullptr;
    hResult = m_D3DMesh->CloneMesh(D3DXMESH_MANAGED | D3DXMESH_32BIT,
                                  decl,
                                  SharedObj::GetD3DDevice(),
                                  &tempMesh);

    assert(hResult == S_OK);

    SAFE_RELEASE(m_D3DMesh);
    m_D3DMesh = tempMesh;

    DWORD* adjacencyList = (DWORD*)adjacencyBuffer->GetBufferPointer();

    //--------------------------------------------------------
    // �@�������Čv�Z
    //--------------------------------------------------------

    // �t���b�g�V�F�[�f�B���O���s���ꍇ�A�Čv�Z���Ȃ�
    if (!FLAT_SHADING)
    {
        hResult = D3DXComputeNormals(m_D3DMesh, adjacencyList);
        assert(hResult == S_OK);
    }

    //--------------------------------------------------------
    // �ʂƒ��_����בւ��ă��b�V���𐶐����A�`��p�t�H�[�}���X���œK��
    //--------------------------------------------------------
    hResult = m_D3DMesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
                                        adjacencyList,
                                        nullptr,
                                        nullptr,
                                        nullptr);

    SAFE_RELEASE(adjacencyBuffer);
    assert(hResult == S_OK);

    //--------------------------------------------------------
    // �}�e���A�����̓ǂݍ���
    //--------------------------------------------------------
    D3DXMATERIAL* materialList = (D3DXMATERIAL*)materialBuffer->GetBufferPointer();

    // X�t�@�C���̃f�B���N�g��
    std::string xFileDir = m_meshName;
    std::size_t lastPos = xFileDir.find_last_of("\\");
    xFileDir = xFileDir.substr(0, lastPos + 1);

    for (DWORD i = 0; i < m_materialCount; ++i)
    {
        //--------------------------------------------------------
        // �g�U���ːF�̓ǂݍ���
        //--------------------------------------------------------
        D3DXVECTOR4 diffuce { };
        diffuce.x = materialList[i].MatD3D.Diffuse.r;
        diffuce.y = materialList[i].MatD3D.Diffuse.g;
        diffuce.z = materialList[i].MatD3D.Diffuse.b;
        diffuce.w = materialList[i].MatD3D.Diffuse.a;

        m_vecDiffuse.push_back(diffuce);

        //--------------------------------------------------------
        // �e�N�X�`���̓ǂݍ���
        //--------------------------------------------------------
        if (materialList[i].pTextureFilename != nullptr)
        {
            std::string texturePath = xFileDir;
            texturePath += materialList[i].pTextureFilename;
            LPDIRECT3DTEXTURE9 tempTexture = nullptr;
            hResult = D3DXCreateTextureFromFile(SharedObj::GetD3DDevice(),
                                               texturePath.c_str(),
                                               &tempTexture);

            assert(hResult == S_OK);

            m_vecTexture.push_back(tempTexture);
        }
    }

    SAFE_RELEASE(materialBuffer);

    m_bLoaded = true;
}

void Mesh::SetPos(const D3DXVECTOR3& pos)
{
    m_pos = pos;
}

void Mesh::SetRotY(const float rotY)
{
    m_rotate.y = rotY;
}

D3DXVECTOR3 Mesh::GetPos()
{
    return m_pos;
}

float Mesh::GetScale()
{
    return m_scale;
}

void Mesh::Render()
{
    HRESULT hResult = E_FAIL;

    //--------------------------------------------------------
    // ���������I����Ă��Ȃ��Ȃ�`�悵�Ȃ�
    // �i�ʃX���b�h�ŏ��������s���ꍇ���l���j
    //--------------------------------------------------------
    if (m_bLoaded == false)
    {
        return;
    }

    //--------------------------------------------------------
    // �����̕�����ݒ�
    //--------------------------------------------------------
    D3DXVECTOR4 normal = Light::GetLightNormal();

    float work = m_rotate.y * -1.f;
    normal.x = std::sin(work + D3DX_PI);
    normal.z = std::cos(work + D3DX_PI);
    D3DXVec4Normalize(&normal, &normal);

    hResult = m_D3DEffect->SetVector("g_light_normal", &normal);
    assert(hResult == S_OK);

    //--------------------------------------------------------
    // �|�C���g���C�g�̈ʒu��ݒ�
    //--------------------------------------------------------

    bool isLit = NSStarmanLib::WeaponManager::GetObj()->IsTorchLit();

    // �����̓_����Ԃ��ς������V�F�[�_�[�Ƀ|�C���g���C�g��ON/OFF��ݒ肷��
    if (isLit != m_bPointLightEnablePrevious)
    {
        if (isLit)
        {
            hResult = m_D3DEffect->SetBool("pointLightEnable", TRUE);
            assert(hResult == S_OK);
        }
        else
        {
            hResult = m_D3DEffect->SetBool("pointLightEnable", FALSE);
            assert(hResult == S_OK);
        }
    }

    m_bPointLightEnablePrevious = isLit;

    if (isLit)
    {
        D3DXVECTOR3 ppos = SharedObj::GetPlayer()->GetPos();
        D3DXVECTOR4 ppos2;
        ppos2.x = ppos.x;
        ppos2.y = ppos.y+2;
        ppos2.z = ppos.z;
        ppos2.w = 0;

        hResult = m_D3DEffect->SetVector("g_point_light_pos", &ppos2);
        assert(hResult == S_OK);
    }

    //--------------------------------------------------------
    // �����̖��邳��ݒ�
    //--------------------------------------------------------
    hResult = m_D3DEffect->SetFloat("g_light_brightness", Light::GetBrightness());
    assert(hResult == S_OK);

    //--------------------------------------------------------
    // �J�������疶��Z������
    //--------------------------------------------------------
    D3DXVECTOR4 fog_color;

    if (!Rain::Get()->IsRain())
    {
        fog_color.x = 0.5f;
        fog_color.y = 0.3f;
        fog_color.z = 0.2f;
        fog_color.w = 1.0f;

        // �����T�|�[�g���Ȃ��V�F�[�_�[���Z�b�g����Ă���\��������̂�
        // mesh_shader.fx�̎������K�p����
        if (SHADER_FILENAME == "res\\shader\\mesh_shader.fx")
        {
            hResult = m_D3DEffect->SetFloat("g_fog_strength", 1.0f);
            assert(hResult == S_OK);
        }
    }
    else
    {
        fog_color.x = 0.3f;
        fog_color.y = 0.3f;
        fog_color.z = 0.5f;
        fog_color.w = 1.0f;

        // �J�������疶��3�{��������B
        // �����T�|�[�g���Ȃ��V�F�[�_�[���Z�b�g����Ă���\��������̂�
        // mesh_shader.fx�̎������K�p����
        if (SHADER_FILENAME == "res\\shader\\mesh_shader.fx")
        {
            hResult = m_D3DEffect->SetFloat("g_fog_strength", 100.0f);
            assert(hResult == S_OK);
        }
    }

    hResult = m_D3DEffect->SetVector("fog_color", &fog_color);
    assert(hResult == S_OK);

    //--------------------------------------------------------
    // ���[���h�ϊ��s���ݒ�
    //--------------------------------------------------------
    D3DXMATRIX worldViewProjMatrix { };
    D3DXMatrixIdentity(&worldViewProjMatrix);
    {
        D3DXMATRIX mat { };

        // ���킩�ۂ�
        if (m_bWeapon == false)
        {
            D3DXMatrixScaling(&mat, m_scale, m_scale, m_scale);
            worldViewProjMatrix *= mat;

            D3DXMatrixRotationYawPitchRoll(&mat, m_rotate.y, m_rotate.x, m_rotate.z);
            worldViewProjMatrix *= mat;

            D3DXMatrixTranslation(&mat, m_pos.x, m_pos.y, m_pos.z);
            worldViewProjMatrix *= mat;
        }
        else
        {
            D3DXMatrixScaling(&mat, m_scale, m_scale, m_scale);
            worldViewProjMatrix *= mat;

            D3DXMatrixRotationYawPitchRoll(&mat, m_rotate.y, m_rotate.x, m_rotate.z);
            worldViewProjMatrix *= mat;

            worldViewProjMatrix *= SharedObj::GetRightHandMat();
        }
    }

    hResult = m_D3DEffect->SetMatrix("g_world", &worldViewProjMatrix);
    assert(hResult == S_OK);

    //--------------------------------------------------------
    // �J�����̈ʒu��ݒ�
    //--------------------------------------------------------
    D3DXVECTOR4 cameraPos { };
    cameraPos.x = Camera::GetEyePos().x; 
    cameraPos.y = Camera::GetEyePos().y;
    cameraPos.z = Camera::GetEyePos().z;
    cameraPos.w = 0.f;

    hResult = m_D3DEffect->SetVector("g_cameraPos", &cameraPos);
    assert(hResult == S_OK);

    //--------------------------------------------------------
    // ���[���h�r���[�ˉe�ϊ��s���ݒ�
    //--------------------------------------------------------
    worldViewProjMatrix *= Camera::GetViewMatrix();
    worldViewProjMatrix *= Camera::GetProjMatrix();

    hResult = m_D3DEffect->SetMatrix("g_world_view_projection", &worldViewProjMatrix);
    assert(hResult == S_OK);

    //--------------------------------------------------------
    // �`��J�n
    //--------------------------------------------------------
    hResult = m_D3DEffect->Begin(nullptr, 0);
    assert(hResult == S_OK);

    hResult = m_D3DEffect->BeginPass(0);
    assert(hResult == S_OK);

    //--------------------------------------------------------
    // �}�e���A���̐������F�ƃe�N�X�`����ݒ肵�ĕ`��
    //--------------------------------------------------------
    for (DWORD i = 0; i < m_materialCount; ++i)
    {
        hResult = m_D3DEffect->SetVector("g_diffuse", &m_vecDiffuse.at(i));
        assert(hResult == S_OK);

        if (i < m_vecTexture.size())
        {
            hResult = m_D3DEffect->SetTexture("g_mesh_texture", m_vecTexture.at(i));
            assert(hResult == S_OK);
        }

        hResult = m_D3DEffect->CommitChanges();
        assert(hResult == S_OK);

        hResult = m_D3DMesh->DrawSubset(i);
        assert(hResult == S_OK);
    }

    hResult = m_D3DEffect->EndPass();
    assert(hResult == S_OK);

    hResult = m_D3DEffect->End();
    assert(hResult == S_OK);
}

LPD3DXMESH Mesh::GetD3DMesh() const
{
    return m_D3DMesh;
}

void Mesh::SetWeapon(const bool arg)
{
    m_bWeapon = arg;
}

float Mesh::GetRadius() const
{
    return m_radius;
}

