#include "Mesh.h"
#include "Light.h"

using std::string;
using std::vector;

Mesh::Mesh(
    const LPDIRECT3DDEVICE9 D3DDevice,
    const string& xFilename,
    const D3DXVECTOR3& position,
    const D3DXVECTOR3& rotation,
    const float& scale)
{
    m_D3DDevice = D3DDevice;

    HRESULT result { 0 };
    D3DXCreateEffectFromFile(
        D3DDevice,
        SHADER_FILENAME.c_str(),
        nullptr,
        nullptr,
        0,
        nullptr,
        &m_Effect,
        nullptr);
    if (FAILED(result))
    {
        throw std::exception("Failed to create an effect file.");
    }

    m_WorldViewProj = m_Effect->GetParameterByName(nullptr, "g_world_view_projection");
    m_LightNormal = m_Effect->GetParameterByName(nullptr, "g_light_normal");
    m_Brightness = m_Effect->GetParameterByName(nullptr, "g_light_brightness");
    m_MeshTexture = m_Effect->GetParameterByName(nullptr, "g_mesh_texture");
    m_Diffuse = m_Effect->GetParameterByName(nullptr, "g_diffuse");

    LPD3DXBUFFER adjacencyBuffer { nullptr };
    LPD3DXBUFFER materialBuffer { nullptr };

    result = D3DXLoadMeshFromX(
        xFilename.c_str(),
        D3DXMESH_SYSTEMMEM,
        m_D3DDevice,
        &adjacencyBuffer,
        &materialBuffer,
        nullptr,
        &m_MaterialCount,
        &m_D3DMesh);

    if (FAILED(result))
    {
        throw std::exception("Failed to load a x-file.");
    }

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

    LPD3DXMESH tempMesh { nullptr };
    result = m_D3DMesh->CloneMesh(D3DXMESH_MANAGED, decl, m_D3DDevice, &tempMesh);

    if (FAILED(result))
    {
        throw std::exception("Failed 'CloneMesh' function.");
    }
    SAFE_RELEASE(m_D3DMesh);
    m_D3DMesh = tempMesh;
    DWORD* wordBuffer { static_cast<DWORD*>(adjacencyBuffer->GetBufferPointer()) };

    result = D3DXComputeNormals(m_D3DMesh, wordBuffer);

    if (FAILED(result))
    {
        throw std::exception("Failed 'D3DXComputeNormals' function.");
    }

    result = m_D3DMesh->OptimizeInplace(
        D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
        wordBuffer,
        nullptr,
        nullptr,
        nullptr);

    SAFE_RELEASE(adjacencyBuffer);

    if (FAILED(result))
    {
        throw std::exception("Failed 'OptimizeInplace' function.");
    }

    m_vecColor.insert(begin(m_vecColor), m_MaterialCount, D3DCOLORVALUE { });
    vector<LPDIRECT3DTEXTURE9> tempVecTexture { m_MaterialCount };
    m_vecTexture.swap(tempVecTexture);

    D3DXMATERIAL* materials { static_cast<D3DXMATERIAL*>(materialBuffer->GetBufferPointer()) };

    for (DWORD i = 0; i < m_MaterialCount; ++i)
    {
        m_vecColor.at(i) = materials[i].MatD3D.Diffuse;
        if (materials[i].pTextureFilename != nullptr)
        {
            LPDIRECT3DTEXTURE9 tempTexture { };
            if (FAILED(D3DXCreateTextureFromFile(
                m_D3DDevice,
                "res\\model\\tiger\\tiger.bmp", /* TODO */
                &tempTexture)))
            {
                throw std::exception("texture file is not found.");
            }
            else
            {
                SAFE_RELEASE(m_vecTexture.at(i));
                m_vecTexture.at(i) = tempTexture;
            }
        }
    }
    SAFE_RELEASE(materialBuffer);

    m_Scale = scale;
}

Mesh::~Mesh()
{
}

void Mesh::Render(const D3DXMATRIX& viewMatrix, const D3DXMATRIX& projMatrix)
{
    D3DXVECTOR4 normal = Light::GetLightNormal();
    m_Effect->SetVector(m_LightNormal, &normal);
    m_Effect->SetFloat(m_Brightness, Light::GetBrightness());

    D3DXMATRIX worldViewProjMatrix { };
    D3DXMatrixIdentity(&worldViewProjMatrix);
    {
        D3DXMATRIX mat { };

        D3DXMatrixTranslation(&mat, -m_CenterCoord.x, -m_CenterCoord.y, -m_CenterCoord.z);
        worldViewProjMatrix *= mat;

        D3DXMatrixScaling(&mat, m_Scale, m_Scale, m_Scale);
        worldViewProjMatrix *= mat;

        D3DXMatrixRotationYawPitchRoll(&mat, m_Rotate.x, m_Rotate.y, m_Rotate.z);
        worldViewProjMatrix *= mat;

        D3DXMatrixTranslation(&mat, m_Pos.x, m_Pos.y, m_Pos.z);
        worldViewProjMatrix *= mat;
    }
    worldViewProjMatrix *= viewMatrix;
    worldViewProjMatrix *= projMatrix;

    m_Effect->SetMatrix(m_WorldViewProj, &worldViewProjMatrix);

    m_Effect->Begin(nullptr, 0);

    HRESULT result { S_FALSE };
    if (FAILED(result = m_Effect->BeginPass(0)))
    {
        m_Effect->End();
        throw std::exception("Failed 'BeginPass' function.");
    }

    for (DWORD i = 0; i < m_MaterialCount; ++i)
    {
        // TODO : remove redundant set****.
        D3DXVECTOR4 vec4Color {
            m_vecColor.at(i).r,
            m_vecColor.at(i).g,
            m_vecColor.at(i).b,
            m_vecColor.at(i).a
        };
        m_Effect->SetVector(m_Diffuse, &vec4Color);
        m_Effect->SetTexture(m_MeshTexture, m_vecTexture.at(i));
        m_Effect->CommitChanges();
        m_D3DMesh->DrawSubset(i);
    }
    m_Effect->EndPass();
    m_Effect->End();
}

