﻿#include "MeshClone.h"
#include "Light.h"
#include "Common.h"
#include "Camera.h"
#include "SharedObj.h"
#include <cassert>
#include "Rain.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/WeaponManager.h"

std::unordered_map<std::wstring, LPD3DXEFFECT> MeshClone::m_D3DEffectMap;
std::unordered_map<std::wstring, LPD3DXMESH> MeshClone::m_D3DMeshMap;
std::unordered_map<std::wstring, std::vector<LPDIRECT3DTEXTURE9>> MeshClone::m_vecTextureMap;
std::unordered_map<std::wstring, DWORD> MeshClone::m_materialCountMap;
std::unordered_map<std::wstring, std::vector<D3DXVECTOR4>> MeshClone::m_vecColorMap;
std::unordered_map<std::wstring, bool> MeshClone::m_bFirstMap;

MeshClone::MeshClone(
    const std::wstring& xFilename,
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
    if (m_D3DEffectMap.empty())
    {
        return;
    }

    auto refcnt = m_D3DEffectMap.at(m_meshName)->Release();

    if (refcnt <= 0)
    {
        m_D3DEffectMap.erase(m_meshName);
    }

    ULONG ulong = m_D3DMeshMap.at(m_meshName)->Release();

    for (size_t i = 0; i < m_materialCountMap.at(m_meshName); ++i)
    {
        if (m_vecTextureMap.at(m_meshName).at(i) != nullptr)
        {
            m_vecTextureMap.at(m_meshName).at(i)->Release();
        }
    }

    if (ulong <= 0)
    {
        m_D3DMeshMap.erase(m_meshName);
    }

    if (!_T("rockWithDepressions.xの解放"))
    {
        if (m_meshName.find(_T("rockWithDepressions.x")) != std::wstring::npos)
        {
            std::wstring work;
            work = _T("m_vecTextureMap.at(") + m_meshName + _T(") ref count: ") + std::to_wstring(ulong) + _T("\n");
            OutputDebugString(work.c_str());
        }
    }
}

void MeshClone::Init()
{
    // 読み込み済みだったら読み込まない。
    HRESULT result { 0 };
    LPD3DXEFFECT _D3DEffect = NULL;

    auto it = m_D3DEffectMap.find(m_meshName);

    if (it == m_D3DEffectMap.end())
    {
        D3DXCreateEffectFromFile(SharedObj::GetD3DDevice(),
                                 SHADER_FILENAME.c_str(),
                                 nullptr,
                                 nullptr,
                                 D3DXSHADER_OPTIMIZATION_LEVEL3,
                                 nullptr,
                                 &_D3DEffect,
                                 nullptr);

        if (FAILED(result))
        {
            throw std::exception("Failed to create an effect file.");
        }

        m_D3DEffectMap[m_meshName] = _D3DEffect;;
    }
    else
    {
        m_D3DEffectMap.at(m_meshName)->AddRef();
    }

    if (m_D3DMeshMap.find(m_meshName) != m_D3DMeshMap.end())
    {
        m_D3DMeshMap.at(m_meshName)->AddRef();

        for (size_t i = 0; i < m_materialCountMap.at(m_meshName); ++i)
        {
            if (m_vecTextureMap.at(m_meshName).at(i) != nullptr)
            {
                m_vecTextureMap.at(m_meshName).at(i)->AddRef();
            }
        }
    }
    else
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
        std::vector<LPDIRECT3DTEXTURE9> tempVecTexture { materialCount };
        m_vecTextureMap[m_meshName].swap(tempVecTexture);

        D3DXMATERIAL* materials { static_cast<D3DXMATERIAL*>(materialBuffer->GetBufferPointer()) };

        std::wstring xFileDir = m_meshName;
        std::size_t lastPos = xFileDir.find_last_of(_T("\\"));
        xFileDir = xFileDir.substr(0, lastPos + 1);

        for (DWORD i = 0; i < materialCount; ++i)
        {
            m_vecColorMap[m_meshName].at(i).x = materials[i].MatD3D.Diffuse.r;
            m_vecColorMap[m_meshName].at(i).y = materials[i].MatD3D.Diffuse.g;
            m_vecColorMap[m_meshName].at(i).z = materials[i].MatD3D.Diffuse.b;
            m_vecColorMap[m_meshName].at(i).w = materials[i].MatD3D.Diffuse.a;
            if (materials[i].pTextureFilename != nullptr &&
                strlen(materials[i].pTextureFilename) != 0)
            {
                std::wstring texPath = xFileDir;
                texPath += Common::Utf8ToWstring(materials[i].pTextureFilename);
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

    m_bFirstMap[m_meshName] = false;

    if (ContainMeshName(_T("tree1.x")))
    {
        m_eMeshType = eMeshType::TREE;
    }
    else if (ContainMeshName(_T("grass.x")))
    {
        m_eMeshType = eMeshType::GRASS;
    }
    else if (ContainMeshName(_T("water.x")))
    {
        m_eMeshType = eMeshType::WATER;
    }
    else if (ContainMeshName(_T("sea.x")))
    {
        m_eMeshType = eMeshType::SEA;
    }
    else
    {
        m_eMeshType = eMeshType::OTHER;
    }

    m_bIsInit = true;
}

void MeshClone::SetPos(const D3DXVECTOR3& pos)
{
    m_loadingPos = pos;
}

D3DXVECTOR3 MeshClone::GetPos() const
{
    return m_loadingPos;
}

float MeshClone::GetScale() const
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

    m_D3DEffectMap.at(m_meshName)->SetVector("g_light_normal", &normal);

    //--------------------------------------------------------
    // ポイントライトの位置を設定
    //--------------------------------------------------------
    HRESULT hResult = E_FAIL;
    bool isLit = NSStarmanLib::WeaponManager::GetObj()->IsTorchLit();

    // 松明の点灯状態が変わったらシェーダーにポイントライトのON/OFFを設定する
    if (isLit != m_bPointLightEnablePrevious)
    {
        if (isLit)
        {
            hResult = m_D3DEffectMap.at(m_meshName)->SetBool("pointLightEnable", TRUE);
            assert(hResult == S_OK);
        }
        else
        {
            hResult = m_D3DEffectMap.at(m_meshName)->SetBool("pointLightEnable", FALSE);
            assert(hResult == S_OK);
        }
    }

    m_bPointLightEnablePrevious = isLit;

    if (isLit)
    {
        D3DXVECTOR3 ppos = SharedObj::GetPlayer()->GetPos();
        D3DXVECTOR4 ppos2;
        ppos2.x = ppos.x;
        ppos2.y = ppos.y + 2;
        ppos2.z = ppos.z;
        ppos2.w = 0;

        hResult = m_D3DEffectMap.at(m_meshName)->SetVector("g_point_light_pos", &ppos2);
        assert(hResult == S_OK);
    }

    m_D3DEffectMap.at(m_meshName)->SetFloat("g_light_brightness", Light::GetBrightness());

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
    m_D3DEffectMap.at(m_meshName)->SetMatrix("g_world", &worldViewProjMatrix);
//    m_D3DEffect->SetMatrix("g_light_pos", &worldViewProjMatrix);

    D3DXVECTOR4 vec4Color = {
        Camera::GetEyePos().x,
        Camera::GetEyePos().y,
        Camera::GetEyePos().z,
        0.f
    };

    m_D3DEffectMap.at(m_meshName)->SetVector("g_cameraPos", &vec4Color);

    worldViewProjMatrix *= Camera::GetViewMatrix();
    worldViewProjMatrix *= Camera::GetProjMatrix();

    m_D3DEffectMap.at(m_meshName)->SetMatrix("g_world_view_projection", &worldViewProjMatrix);

    //--------------------------------------------------------
    // 雨だったら霧を濃くする
    //--------------------------------------------------------
    D3DXVECTOR4 fog_color;

    if (!Rain::Get()->IsRain())
    {
        fog_color.x = 0.5f;
        fog_color.y = 0.3f;
        fog_color.z = 0.2f;
        fog_color.w = 1.0f;

        // 霧をサポートしないシェーダーがセットされている可能性があるので
        // mesh_shader.fxの時だけ適用する
        if (SHADER_FILENAME == _T("res\\shader\\mesh_shader.fx"))
        {
            hResult = m_D3DEffectMap.at(m_meshName)->SetFloat("g_fog_strength", 1.0f);
            assert(hResult == S_OK);
        }
    }
    else
    {
        fog_color.x = 0.3f;
        fog_color.y = 0.3f;
        fog_color.z = 0.5f;
        fog_color.w = 1.0f;

        // 雨だったら霧を100倍強くする。
        // 霧をサポートしないシェーダーがセットされている可能性があるので
        // mesh_shader.fxの時だけ適用する
        if (SHADER_FILENAME == _T("res\\shader\\mesh_shader.fx"))
        {
            hResult = m_D3DEffectMap.at(m_meshName)->SetFloat("g_fog_strength", 100.0f);
            assert(hResult == S_OK);
        }
    }

    hResult = m_D3DEffectMap.at(m_meshName)->SetVector("fog_color", &fog_color);
    assert(hResult == S_OK);

    m_D3DEffectMap.at(m_meshName)->Begin(nullptr, 0);

    HRESULT result { S_FALSE };
    if (FAILED(result = m_D3DEffectMap.at(m_meshName)->BeginPass(0)))
    {
        m_D3DEffectMap.at(m_meshName)->End();
        throw std::exception("Failed 'BeginPass' function.");
    }

    // マテリアルが二つ以上あることなんてあるのか？
//    for (DWORD i = 0; i < m_materialCountMap[m_meshName]; ++i)
//    {
//        // TODO この辺は毎回やる必要はない気がする
//        m_D3DEffect->SetVector(_T("g_diffuse"), &m_vecColorMap[m_meshName].at(i));
//        m_D3DEffect->SetTexture("g_mesh_texture", m_vecTextureMap[m_meshName].at(i));
//        m_D3DEffect->CommitChanges();
//        m_D3DMeshMap[m_meshName]->DrawSubset(i);
//    }

    if (!m_bFirstMap.at(m_meshName))
    {
        m_bFirstMap.at(m_meshName) = true;

        m_D3DEffectMap.at(m_meshName)->SetVector("g_diffuse", &m_vecColorMap[m_meshName].at(0));

        m_D3DEffectMap.at(m_meshName)->SetTexture("g_mesh_texture", m_vecTextureMap[m_meshName].at(0));
    }

    m_D3DEffectMap.at(m_meshName)->CommitChanges();
    m_D3DMeshMap[m_meshName]->DrawSubset(0);

    m_D3DEffectMap.at(m_meshName)->EndPass();
    m_D3DEffectMap.at(m_meshName)->End();
}

void MeshClone::Begin()
{
    m_D3DEffectMap.at(m_meshName)->Begin(nullptr, 0);

    HRESULT result { S_FALSE };
    if (FAILED(result = m_D3DEffectMap.at(m_meshName)->BeginPass(0)))
    {
        m_D3DEffectMap.at(m_meshName)->End();
        throw std::exception("Failed 'BeginPass' function.");
    }

    //--------------------------------------------------------
    // ポイントライトの位置を設定
    //--------------------------------------------------------
    HRESULT hResult = E_FAIL;
    bool isLit = NSStarmanLib::WeaponManager::GetObj()->IsTorchLit();

    // 松明の点灯状態が変わったらシェーダーにポイントライトのON/OFFを設定する
    if (isLit != m_bPointLightEnablePrevious)
    {
        if (isLit)
        {
            hResult = m_D3DEffectMap.at(m_meshName)->SetBool("pointLightEnable", TRUE);
            assert(hResult == S_OK);
        }
        else
        {
            hResult = m_D3DEffectMap.at(m_meshName)->SetBool("pointLightEnable", FALSE);
            assert(hResult == S_OK);
        }
    }

    m_bPointLightEnablePrevious = isLit;

    if (isLit)
    {
        D3DXVECTOR3 ppos = SharedObj::GetPlayer()->GetPos();
        D3DXVECTOR4 ppos2;
        ppos2.x = ppos.x;
        ppos2.y = ppos.y + 2;
        ppos2.z = ppos.z;
        ppos2.w = 0;

        hResult = m_D3DEffectMap.at(m_meshName)->SetVector("g_point_light_pos", &ppos2);
        assert(hResult == S_OK);
    }

    m_D3DEffectMap.at(m_meshName)->SetFloat("g_light_brightness", Light::GetBrightness());

    D3DXVECTOR4 vec4Color = {
        Camera::GetEyePos().x,
        Camera::GetEyePos().y,
        Camera::GetEyePos().z,
        0.f
    };

    m_D3DEffectMap.at(m_meshName)->SetVector("g_cameraPos", &vec4Color);

    //--------------------------------------------------------
    // 雨だったら霧を濃くする
    //--------------------------------------------------------
    D3DXVECTOR4 fog_color;

    if (!Rain::Get()->IsRain())
    {
        fog_color.x = 0.5f;
        fog_color.y = 0.3f;
        fog_color.z = 0.2f;
        fog_color.w = 1.0f;

        // 霧をサポートしないシェーダーがセットされている可能性があるので
        // mesh_shader.fxの時だけ適用する
        if (SHADER_FILENAME == _T("res\\shader\\mesh_shader.fx"))
        {
            hResult = m_D3DEffectMap.at(m_meshName)->SetFloat("g_fog_strength", 1.0f);
            assert(hResult == S_OK);
        }
    }
    else
    {
        fog_color.x = 0.3f;
        fog_color.y = 0.3f;
        fog_color.z = 0.5f;
        fog_color.w = 1.0f;

        // 雨だったら霧を100倍強くする。
        // 霧をサポートしないシェーダーがセットされている可能性があるので
        // mesh_shader.fxの時だけ適用する
        if (SHADER_FILENAME == _T("res\\shader\\mesh_shader.fx"))
        {
            hResult = m_D3DEffectMap.at(m_meshName)->SetFloat("g_fog_strength", 100.0f);
            assert(hResult == S_OK);
        }
    }

    hResult = m_D3DEffectMap.at(m_meshName)->SetVector("fog_color", &fog_color);
    assert(hResult == S_OK);

    if (!m_bFirstMap.at(m_meshName))
    {
        m_bFirstMap.at(m_meshName) = true;

        m_D3DEffectMap.at(m_meshName)->SetVector("g_diffuse", &m_vecColorMap[m_meshName].at(0));

        // TODO テクスチャなしにしたほうが良いかも
        m_D3DEffectMap.at(m_meshName)->SetTexture("g_mesh_texture", m_vecTextureMap[m_meshName].at(0));
    }

}

void MeshClone::Render2()
{
    HRESULT hResult = E_FAIL;
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

    m_D3DEffectMap.at(m_meshName)->SetVector("g_light_normal", &normal);

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
    m_D3DEffectMap.at(m_meshName)->SetMatrix("g_world", &worldViewProjMatrix);
//    m_D3DEffect->SetMatrix("g_light_pos", &worldViewProjMatrix);

    worldViewProjMatrix *= Camera::GetViewMatrix();
    worldViewProjMatrix *= Camera::GetProjMatrix();

    m_D3DEffectMap.at(m_meshName)->SetMatrix("g_world_view_projection", &worldViewProjMatrix);

    m_D3DEffectMap.at(m_meshName)->CommitChanges();
    m_D3DMeshMap[m_meshName]->DrawSubset(0);
}

void MeshClone::End()
{
    m_D3DEffectMap.at(m_meshName)->EndPass();
    m_D3DEffectMap.at(m_meshName)->End();
}

MeshClone::eMeshType MeshClone::GetMeshType() const
{
    return m_eMeshType;
}

LPD3DXMESH MeshClone::GetD3DMesh() const
{
    return m_D3DMeshMap.at(m_meshName);
}

void MeshClone::SetWeapon(const bool arg)
{
    m_bWeapon = arg;
}

float MeshClone::GetRadius() const
{
    return m_radius;
}

std::wstring MeshClone::GetMeshName()
{
    return m_meshName;
}

bool MeshClone::ContainMeshName(const std::wstring& arg)
{
    if (m_meshName.find(arg) != std::wstring::npos)
    {
        return true;
    }
    return false;
}

void MeshClone::ForceRelease()
{
    m_D3DEffectMap.clear();
    m_D3DMeshMap.clear();
    m_vecTextureMap.clear();
    m_materialCountMap.clear();
    m_vecColorMap.clear();
    m_bFirstMap.clear();
}


