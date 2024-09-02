#include "Mesh.h"
#include "Light.h"
#include "Common.h"
#include "Camera.h"
#include "SharedObj.h"

using std::string;
using std::vector;

Mesh::Mesh(
    const string& xFilename,
    const D3DXVECTOR3& position,
    const D3DXVECTOR3& rotation,
    const float& scale)
    : m_meshName { xFilename }
    , m_pos { position }
    , m_rotate { rotation }
    , m_scale { scale }
{
}

Mesh::~Mesh()
{
    for (std::size_t i = 0; i < m_vecTexture.size(); ++i)
    {
        SAFE_RELEASE(m_vecTexture.at(i));
    }
    SAFE_RELEASE(m_D3DMesh);
    SAFE_RELEASE(m_D3DEffect);
}

void Mesh::Init()
{
    HRESULT result { 0 };
    D3DXCreateEffectFromFile(
        SharedObj::GetD3DDevice(),
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

    m_worldViewProjHandle = m_D3DEffect->GetParameterByName(nullptr, "g_world_view_projection");
    m_lightNormalHandle = m_D3DEffect->GetParameterByName(nullptr, "g_light_normal");
    m_brightnessHandle = m_D3DEffect->GetParameterByName(nullptr, "g_light_brightness");
    m_meshTextureHandle = m_D3DEffect->GetParameterByName(nullptr, "g_mesh_texture");
    m_diffuseHandle = m_D3DEffect->GetParameterByName(nullptr, "g_diffuse");

    LPD3DXBUFFER adjacencyBuffer { nullptr };
    LPD3DXBUFFER materialBuffer { nullptr };

    result = D3DXLoadMeshFromX(
        m_meshName.c_str(),
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
    result = m_D3DMesh->CloneMesh(D3DXMESH_MANAGED, decl, SharedObj::GetD3DDevice(), &tempMesh);

    // 頂点バッファの取得
    {
//        LPDIRECT3DVERTEXBUFFER9 pVertexBuffer;
//        tempMesh->GetVertexBuffer(&pVertexBuffer);
//
//        struct CUSTOMVERTEX
//        {
//            FLOAT x, y, z; // 頂点の座標
//            FLOAT normX, normY, normZ; // 法線の座標
//            FLOAT u, v;   // 頂点の色
//        };
//        CUSTOMVERTEX* pVertices = nullptr;
//        CUSTOMVERTEX temp[24]; // 立方体なら24個
//        pVertexBuffer->Lock(0, 0, (void**)&pVertices, D3DLOCK_READONLY);
//        memcpy(temp, pVertices, sizeof(temp));
//        pVertexBuffer->Unlock();
    }

    if (FAILED(result))
    {
        throw std::exception("Failed 'CloneMesh' function.");
    }
    SAFE_RELEASE(m_D3DMesh);
    m_D3DMesh = tempMesh;
    DWORD* wordBuffer { static_cast<DWORD*>(adjacencyBuffer->GetBufferPointer()) };

     // ここをコメントアウトするとフラットシェーディングになる。
//    result = D3DXComputeNormals(m_D3DMesh, wordBuffer);

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

    m_vecColor.insert(begin(m_vecColor), m_materialCount, D3DCOLORVALUE { });
    vector<LPDIRECT3DTEXTURE9> tempVecTexture { m_materialCount };
    m_vecTexture.swap(tempVecTexture);

    D3DXMATERIAL* materials { static_cast<D3DXMATERIAL*>(materialBuffer->GetBufferPointer()) };

    std::string xFileDir = m_meshName;
    std::size_t lastPos = xFileDir.find_last_of("\\");
    xFileDir = xFileDir.substr(0, lastPos + 1);

    for (DWORD i = 0; i < m_materialCount; ++i)
    {
        m_vecColor.at(i) = materials[i].MatD3D.Diffuse;
        if (materials[i].pTextureFilename != nullptr)
        {
            std::string texPath = xFileDir;
            texPath += materials[i].pTextureFilename;
            LPDIRECT3DTEXTURE9 tempTexture { nullptr };
            if (FAILED(D3DXCreateTextureFromFile(
                SharedObj::GetD3DDevice(),
                texPath.c_str(),
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

    m_bIsInit = true;
}

void Mesh::SetPos(const D3DXVECTOR3& pos)
{
    m_pos = pos;
}

D3DXVECTOR3 Mesh::GetPos()
{
    return m_pos;
}

void Mesh::Render()
{
    if (m_bIsInit == false)
    {
        return;
    }
    D3DXVECTOR4 normal = Light::GetLightNormal();
    m_D3DEffect->SetVector(m_lightNormalHandle, &normal);
    m_D3DEffect->SetFloat(m_brightnessHandle, Light::GetBrightness());

    D3DXMATRIX worldViewProjMatrix { };
    D3DXMatrixIdentity(&worldViewProjMatrix);
    {
        D3DXMATRIX mat { };

        D3DXMatrixTranslation(&mat, -m_centerPos.x, -m_centerPos.y, -m_centerPos.z);
        worldViewProjMatrix *= mat;

        D3DXMatrixScaling(&mat, m_scale, m_scale, m_scale);
        worldViewProjMatrix *= mat;

        D3DXMatrixRotationYawPitchRoll(&mat, m_rotate.y, m_rotate.x, m_rotate.z);
        worldViewProjMatrix *= mat;

        D3DXMatrixTranslation(&mat, m_pos.x, m_pos.y, m_pos.z);
        worldViewProjMatrix *= mat;
    }
    worldViewProjMatrix *= Camera::GetViewMatrix();
    worldViewProjMatrix *= Camera::GetProjMatrix();

    m_D3DEffect->SetMatrix(m_worldViewProjHandle, &worldViewProjMatrix);

    m_D3DEffect->Begin(nullptr, 0);

    HRESULT result { S_FALSE };
    if (FAILED(result = m_D3DEffect->BeginPass(0)))
    {
        m_D3DEffect->End();
        throw std::exception("Failed 'BeginPass' function.");
    }

    for (DWORD i = 0; i < m_materialCount; ++i)
    {
        D3DXVECTOR4 vec4Color {
            m_vecColor.at(i).r, m_vecColor.at(i).g, m_vecColor.at(i).b, m_vecColor.at(i).a};
        m_D3DEffect->SetVector(m_diffuseHandle, &vec4Color);
        m_D3DEffect->SetTexture(m_meshTextureHandle, m_vecTexture.at(i));
        m_D3DEffect->CommitChanges();
        m_D3DMesh->DrawSubset(i);
    }
    m_D3DEffect->EndPass();
    m_D3DEffect->End();
}

LPD3DXMESH Mesh::GetD3DMesh()
{
    return m_D3DMesh;
}

