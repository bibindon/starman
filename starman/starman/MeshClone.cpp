#include "MeshClone.h"
#include "Light.h"
#include "Common.h"
#include "Camera.h"
#include "SharedObj.h"

using std::string;
using std::vector;

LPD3DXEFFECT MeshClone::m_D3DEffect;
std::unordered_map<std::string, LPD3DXMESH> MeshClone::m_D3DMeshMap;
std::unordered_map<std::string, std::vector<LPDIRECT3DTEXTURE9>> MeshClone::m_vecTextureMap;
std::unordered_map<std::string, DWORD> MeshClone::m_materialCountMap;
std::unordered_map<std::string, std::vector<D3DXVECTOR4>> MeshClone::m_vecColorMap;

MeshClone::MeshClone(
    const string& xFilename,
    const D3DXVECTOR3& position,
    const D3DXVECTOR3& rotation,
    const float scale,
    const float radius)
    : m_meshName { xFilename }
    , m_loadingPos { position }
    , m_rotate { rotation }
    , m_scale { scale }
    , m_radius { radius }
{
}

MeshClone::~MeshClone()
{
    // TODO いずれ
//    for (std::size_t i = 0; i < m_vecTexture.size(); ++i)
//    {
//        SAFE_RELEASE(m_vecTexture.at(i));
//    }
//    SAFE_RELEASE(m_D3DMesh);
//    SAFE_RELEASE(m_D3DEffect);
}

void MeshClone::Init()
{
    // TODO 読み込み済みだったら読み込まない。
    HRESULT result { 0 };
    if (m_D3DEffect == nullptr)
    {
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
    }

    if (m_D3DMeshMap.find(m_meshName) == m_D3DMeshMap.end())
    {
        LPD3DXMESH tempMesh = nullptr;

        LPD3DXBUFFER adjacencyBuffer { nullptr };
        LPD3DXBUFFER materialBuffer { nullptr };
        DWORD materialCount = 0;

        result = D3DXLoadMeshFromX(
            m_meshName.c_str(),
            D3DXMESH_SYSTEMMEM,
            SharedObj::GetD3DDevice(),
            &adjacencyBuffer,
            &materialBuffer,
            nullptr,
            &materialCount,
            &tempMesh);

        if (FAILED(result))
        {
            throw std::exception("Failed to load a x-file.");
        }

        m_materialCountMap[m_meshName] = materialCount;

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

        LPD3DXMESH tempMesh2 { nullptr };
        result = tempMesh->CloneMesh(D3DXMESH_MANAGED, decl, SharedObj::GetD3DDevice(), &tempMesh2);

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
        SAFE_RELEASE(tempMesh);
        tempMesh = tempMesh2;
        DWORD* wordBuffer { static_cast<DWORD*>(adjacencyBuffer->GetBufferPointer()) };

        // ここをコメントアウトするとフラットシェーディングになる。
        result = D3DXComputeNormals(tempMesh, wordBuffer);

        if (FAILED(result))
        {
            throw std::exception("Failed 'D3DXComputeNormals' function.");
        }

        result = tempMesh->OptimizeInplace(
            D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
            wordBuffer,
            nullptr,
            nullptr,
            nullptr);

        m_D3DMeshMap[m_meshName] = tempMesh;

        SAFE_RELEASE(adjacencyBuffer);

        if (FAILED(result))
        {
            throw std::exception("Failed 'OptimizeInplace' function.");
        }

        m_vecColorMap[m_meshName].insert(begin(m_vecColorMap[m_meshName]), materialCount, D3DXVECTOR4 { });
        vector<LPDIRECT3DTEXTURE9> tempVecTexture { materialCount };
        m_vecTextureMap[m_meshName].swap(tempVecTexture);

        D3DXMATERIAL* materials { static_cast<D3DXMATERIAL*>(materialBuffer->GetBufferPointer()) };

        std::string xFileDir = m_meshName;
        std::size_t lastPos = xFileDir.find_last_of("\\");
        xFileDir = xFileDir.substr(0, lastPos + 1);

        for (DWORD i = 0; i < materialCount; ++i)
        {
            m_vecColorMap[m_meshName].at(i).x = materials[i].MatD3D.Diffuse.r;
            m_vecColorMap[m_meshName].at(i).y = materials[i].MatD3D.Diffuse.g;
            m_vecColorMap[m_meshName].at(i).z = materials[i].MatD3D.Diffuse.b;
            m_vecColorMap[m_meshName].at(i).w = materials[i].MatD3D.Diffuse.a;
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
                    SAFE_RELEASE(m_vecTextureMap[m_meshName].at(i));
                    m_vecTextureMap[m_meshName].at(i) = tempTexture;
                }
            }
        }
        SAFE_RELEASE(materialBuffer);
    }
    else
    {
    }

    m_bIsInit = true;
}

void MeshClone::SetPos(const D3DXVECTOR3& pos)
{
    m_loadingPos = pos;
}

D3DXVECTOR3 MeshClone::GetPos()
{
    return m_loadingPos;
}

float MeshClone::GetScale()
{
    return m_scale;
}

void MeshClone::Render()
{
    if (m_bIsInit == false)
    {
        return;
    }
    // Y軸回転だけしているうちは正しく影が表示される。
    D3DXVECTOR4 normal = Light::GetLightNormal();
    float work = m_rotate.y * -1.f;
    normal.x = std::sin(work);
    normal.z = std::cos(work);
    D3DXVec4Normalize(&normal, &normal);

    m_D3DEffect->SetVector("g_light_normal", &normal);

    SharedObj::GetPlayer();
    D3DXVECTOR3 ppos = SharedObj::GetPlayer()->GetPos();
    D3DXVECTOR4 ppos2;
    ppos2.x = ppos.x;
    ppos2.y = ppos.y+2;
    ppos2.z = ppos.z;
    ppos2.w = 0;
    m_D3DEffect->SetVector("g_point_light_pos", &ppos2);

    m_D3DEffect->SetFloat("g_light_brightness", Light::GetBrightness());

    D3DXMATRIX worldViewProjMatrix { };
    D3DXMatrixIdentity(&worldViewProjMatrix);
    {
        D3DXMATRIX mat { };

        if (m_bWeapon == false)
        {
            D3DXMatrixTranslation(&mat, -m_centerPos.x, -m_centerPos.y, -m_centerPos.z);
            worldViewProjMatrix *= mat;

            D3DXMatrixScaling(&mat, m_scale, m_scale, m_scale);
            worldViewProjMatrix *= mat;

            // D3DXMatrixRotationYawPitchRollを使うと、Z軸回転が最初に行われる。
            // Y軸回転を最初に行いたいのでD3DXMatrixRotationYawPitchRollは使わない
// D3DXMatrixRotationYawPitchRoll(&mat, m_rotate.y, m_rotate.x, m_rotate.z);
// worldViewProjMatrix *= mat;

            D3DXMatrixRotationY(&mat, m_rotate.y);
            worldViewProjMatrix *= mat;

            D3DXMatrixRotationX(&mat, m_rotate.x);
            worldViewProjMatrix *= mat;

            D3DXMatrixRotationZ(&mat, m_rotate.z);
            worldViewProjMatrix *= mat;

            D3DXMatrixTranslation(&mat, m_loadingPos.x, m_loadingPos.y, m_loadingPos.z);
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
//    m_D3DEffect->SetMatrix("g_light_pos", &worldViewProjMatrix);

    D3DXVECTOR4 vec4Color = {
        Camera::GetEyePos().x,
        Camera::GetEyePos().y,
        Camera::GetEyePos().z,
        0.f
    };

    m_D3DEffect->SetVector("g_cameraPos", &vec4Color);

    worldViewProjMatrix *= Camera::GetViewMatrix();
    worldViewProjMatrix *= Camera::GetProjMatrix();

    m_D3DEffect->SetMatrix("g_world_view_projection", &worldViewProjMatrix);

    m_D3DEffect->Begin(nullptr, 0);

    HRESULT result { S_FALSE };
    if (FAILED(result = m_D3DEffect->BeginPass(0)))
    {
        m_D3DEffect->End();
        throw std::exception("Failed 'BeginPass' function.");
    }

    // マテリアルが二つ以上あることなんてあるのか？
//    for (DWORD i = 0; i < m_materialCountMap[m_meshName]; ++i)
//    {
//        // TODO この辺は毎回やる必要はない気がする
//        m_D3DEffect->SetVector("g_diffuse", &m_vecColorMap[m_meshName].at(i));
//        m_D3DEffect->SetTexture("g_mesh_texture", m_vecTextureMap[m_meshName].at(i));
//        m_D3DEffect->CommitChanges();
//        m_D3DMeshMap[m_meshName]->DrawSubset(i);
//    }

    // TODO この辺は毎回やる必要はない気がする
    m_D3DEffect->SetVector("g_diffuse", &m_vecColorMap[m_meshName].at(0));

    // TODO テクスチャなしにしたほうが良いかも
    m_D3DEffect->SetTexture("g_mesh_texture", m_vecTextureMap[m_meshName].at(0));
    m_D3DEffect->CommitChanges();
    m_D3DMeshMap[m_meshName]->DrawSubset(0);

    m_D3DEffect->EndPass();
    m_D3DEffect->End();
}

LPD3DXMESH MeshClone::GetD3DMesh()
{
    return m_D3DMeshMap[m_meshName];
}

void MeshClone::SetWeapon(const bool arg)
{
    m_bWeapon = arg;
}

float MeshClone::GetRadius() const
{
    return m_radius;
}


