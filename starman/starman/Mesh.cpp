#include <cassert>
#include "Mesh.h"
#include "Light.h"
#include "Common.h"
#include "Camera.h"
#include "SharedObj.h"

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
    //--------------------------------------------------------
    // �G�t�F�N�g�̍쐬
    //--------------------------------------------------------
    HRESULT result = E_FAIL;
    result = D3DXCreateEffectFromFile(SharedObj::GetD3DDevice(),
                                      SHADER_FILENAME.c_str(),
                                      nullptr,
                                      nullptr,
                                      0,
                                      nullptr,
                                      &m_D3DEffect,
                                      nullptr);

    if (FAILED(result))
    {
        throw std::exception("Failed to create an effect file.");
    }

    //--------------------------------------------------------
    // X�t�@�C���̓ǂݍ���
    //--------------------------------------------------------
    LPD3DXBUFFER adjacencyBuffer = nullptr;
    LPD3DXBUFFER materialBuffer = nullptr;

    result = D3DXLoadMeshFromX(m_meshName.c_str(),
                               D3DXMESH_SYSTEMMEM,
                               SharedObj::GetD3DDevice(),
                               &adjacencyBuffer,
                               &materialBuffer,
                               nullptr,
                               &m_materialCount,
                               &m_D3DMesh);

    if (FAILED(result))
    {
        throw std::exception("Failed to load a x-file.");
    }

    //--------------------------------------------------------
    // �@�����������b�V���t�@�C���ɕϊ�
    //--------------------------------------------------------
    D3DVERTEXELEMENT9 decl[] = {
        {
            0,
            0,
            D3DDECLTYPE_FLOAT3,
            D3DDECLMETHOD_DEFAULT,
            D3DDECLUSAGE_POSITION,
            0},
        {
            0,
            12,
            D3DDECLTYPE_FLOAT3,
            D3DDECLMETHOD_DEFAULT,
            D3DDECLUSAGE_NORMAL,
            0
        },
        {
            0,
            24,
            D3DDECLTYPE_FLOAT2,
            D3DDECLMETHOD_DEFAULT,
            D3DDECLUSAGE_TEXCOORD,
            0
        },
        D3DDECL_END(),
    };

    LPD3DXMESH tempMesh = nullptr;
    result = m_D3DMesh->CloneMesh(D3DXMESH_MANAGED,
                                  decl,
                                  SharedObj::GetD3DDevice(),
                                  &tempMesh);

    if (FAILED(result))
    {
        throw std::exception("Failed 'CloneMesh' function.");
    }

    SAFE_RELEASE(m_D3DMesh);
    m_D3DMesh = tempMesh;

    DWORD* adjacencyList = (DWORD*)adjacencyBuffer->GetBufferPointer();

    //--------------------------------------------------------
    // �@�������Čv�Z
    //--------------------------------------------------------

    // �t���b�g�V�F�[�f�B���O���s���ꍇ�A�Čv�Z���Ȃ�
    if (!FLAT_SHADING)
    {
        result = D3DXComputeNormals(m_D3DMesh, adjacencyList);

        if (FAILED(result))
        {
            throw std::exception("Failed 'D3DXComputeNormals' function.");
        }
    }

    //--------------------------------------------------------
    // �ʂƒ��_����בւ��ă��b�V���𐶐����A�`��p�t�H�[�}���X���œK��
    //--------------------------------------------------------
    result = m_D3DMesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
                                        adjacencyList,
                                        nullptr,
                                        nullptr,
                                        nullptr);

    SAFE_RELEASE(adjacencyBuffer);

    if (FAILED(result))
    {
        throw std::exception("Failed 'OptimizeInplace' function.");
    }

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
            result = D3DXCreateTextureFromFile(SharedObj::GetD3DDevice(),
                                               texturePath.c_str(),
                                               &tempTexture);

            if (FAILED(result))
            {
                throw std::exception("texture file is not found.");
            }

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

    m_D3DEffect->SetVector("g_light_normal", &normal);

    //--------------------------------------------------------
    // �|�C���g���C�g�̈ʒu��ݒ�i���g�p�j
    //--------------------------------------------------------
    D3DXVECTOR3 ppos = SharedObj::GetPlayer()->GetPos();
    D3DXVECTOR4 ppos2;
    ppos2.x = ppos.x;
    ppos2.y = ppos.y+2;
    ppos2.z = ppos.z;
    ppos2.w = 0;

    m_D3DEffect->SetVector("g_point_light_pos", &ppos2);

    //--------------------------------------------------------
    // �����̖��邳��ݒ�
    //--------------------------------------------------------
    m_D3DEffect->SetFloat("g_light_brightness", Light::GetBrightness());

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

    m_D3DEffect->SetMatrix("g_world", &worldViewProjMatrix);

    //--------------------------------------------------------
    // �J�����̈ʒu��ݒ�
    //--------------------------------------------------------
    D3DXVECTOR4 cameraPos { };
    cameraPos.x = Camera::GetEyePos().x; 
    cameraPos.y = Camera::GetEyePos().y;
    cameraPos.z = Camera::GetEyePos().z;
    cameraPos.w = 0.f;

    m_D3DEffect->SetVector("g_cameraPos", &cameraPos);

    //--------------------------------------------------------
    // ���[���h�r���[�ˉe�ϊ��s���ݒ�
    //--------------------------------------------------------
    worldViewProjMatrix *= Camera::GetViewMatrix();
    worldViewProjMatrix *= Camera::GetProjMatrix();

    m_D3DEffect->SetMatrix("g_world_view_projection", &worldViewProjMatrix);

    //--------------------------------------------------------
    // �`��J�n
    //--------------------------------------------------------
    m_D3DEffect->Begin(nullptr, 0);

    HRESULT result = E_FAIL;

    result = m_D3DEffect->BeginPass(0);

    if (FAILED(result))
    {
        result = m_D3DEffect->End();
        throw std::exception("Failed 'BeginPass' function.");
    }

    //--------------------------------------------------------
    // �}�e���A���̐������F�ƃe�N�X�`����ݒ肵�ĕ`��
    //--------------------------------------------------------
    for (DWORD i = 0; i < m_materialCount; ++i)
    {
        result = m_D3DEffect->SetVector("g_diffuse", &m_vecDiffuse.at(i));
        assert(result == S_OK);

        if (i < m_vecTexture.size())
        {
            result = m_D3DEffect->SetTexture("g_mesh_texture", m_vecTexture.at(i));
            assert(result == S_OK);
        }

        result = m_D3DEffect->CommitChanges();
        assert(result == S_OK);

        result = m_D3DMesh->DrawSubset(i);
        assert(result == S_OK);
    }

    result = m_D3DEffect->EndPass();
    assert(result == S_OK);

    result = m_D3DEffect->End();
    assert(result == S_OK);
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

