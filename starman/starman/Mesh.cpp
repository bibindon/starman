﻿#include "Mesh.h"

#include <cassert>

#include "Light.h"
#include "Common.h"
#include "Camera.h"
#include "SharedObj.h"
#include "Rain.h"

#include "../../StarmanLib/StarmanLib/StarmanLib/WeaponManager.h"

Mesh::Mesh(const std::wstring& xFilename,
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

// シェーダーファイルを指定できるコンストラクタ
Mesh::Mesh(const std::wstring& shaderName,
           const std::wstring& xFilename,
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
    // エフェクトの作成
    //--------------------------------------------------------
    hResult = D3DXCreateEffectFromFile(SharedObj::GetD3DDevice(),
                                      SHADER_FILENAME.c_str(),
                                      nullptr,
                                      nullptr,
                                      D3DXSHADER_OPTIMIZATION_LEVEL3,
                                      nullptr,
                                      &m_D3DEffect,
                                      nullptr);

    assert(hResult == S_OK);

    //--------------------------------------------------------
    // Xファイルの読み込み
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
    // 法線情報をもつメッシュファイルに変換
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
    // 法線情報を再計算
    //--------------------------------------------------------

    // フラットシェーディングを行う場合、再計算しない
    if (!FLAT_SHADING)
    {
        hResult = D3DXComputeNormals(m_D3DMesh, adjacencyList);
        assert(hResult == S_OK);
    }

    //--------------------------------------------------------
    // 面と頂点を並べ替えてメッシュを生成し、描画パフォーマンスを最適化
    //--------------------------------------------------------
    hResult = m_D3DMesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
                                        adjacencyList,
                                        nullptr,
                                        nullptr,
                                        nullptr);

    SAFE_RELEASE(adjacencyBuffer);
    assert(hResult == S_OK);

    //--------------------------------------------------------
    // マテリアル情報の読み込み
    //--------------------------------------------------------
    D3DXMATERIAL* materialList = (D3DXMATERIAL*)materialBuffer->GetBufferPointer();

    // Xファイルのディレクトリ
    std::wstring xFileDir = m_meshName;
    std::size_t lastPos = xFileDir.find_last_of(_T("\\"));
    xFileDir = xFileDir.substr(0, lastPos + 1);

    for (DWORD i = 0; i < m_materialCount; ++i)
    {
        //--------------------------------------------------------
        // 拡散反射色の読み込み
        //--------------------------------------------------------
        D3DXVECTOR4 diffuce { };
        diffuce.x = materialList[i].MatD3D.Diffuse.r;
        diffuce.y = materialList[i].MatD3D.Diffuse.g;
        diffuce.z = materialList[i].MatD3D.Diffuse.b;
        diffuce.w = materialList[i].MatD3D.Diffuse.a;

        m_vecDiffuse.push_back(diffuce);

        //--------------------------------------------------------
        // テクスチャの読み込み
        //--------------------------------------------------------
        if (materialList[i].pTextureFilename != nullptr &&
            strlen(materialList[i].pTextureFilename) != 0)
        {
            std::wstring texturePath = xFileDir;
            texturePath += Common::Utf8ToWstring(materialList[i].pTextureFilename);
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
    // 初期化が終わっていないなら描画しない
    // （別スレッドで初期化を行う場合を考慮）
    //--------------------------------------------------------
    if (m_bLoaded == false)
    {
        return;
    }

    //--------------------------------------------------------
    // 光源の方向を設定
    //--------------------------------------------------------
    D3DXVECTOR4 normal = Light::GetLightNormal();

    float work = m_rotate.y * -1.f;
    normal.x = std::sin(work + D3DX_PI);
    normal.z = std::cos(work + D3DX_PI);
    D3DXVec4Normalize(&normal, &normal);

    hResult = m_D3DEffect->SetVector("g_light_normal", &normal);
    assert(hResult == S_OK);

    //--------------------------------------------------------
    // ポイントライトの位置を設定
    //--------------------------------------------------------

    bool isLit = NSStarmanLib::WeaponManager::GetObj()->IsTorchLit();

    // 松明の点灯状態が変わったらシェーダーにポイントライトのON/OFFを設定する
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
        ppos2.y = ppos.y;
        ppos2.z = ppos.z;
        ppos2.w = 0;

        hResult = m_D3DEffect->SetVector("g_point_light_pos", &ppos2);
        assert(hResult == S_OK);
    }

    //--------------------------------------------------------
    // 光源の明るさを設定
    //--------------------------------------------------------
    hResult = m_D3DEffect->SetFloat("g_light_brightness", Light::GetBrightness());
    assert(hResult == S_OK);

    //--------------------------------------------------------
    // 洞窟
    //--------------------------------------------------------
    if (SHADER_FILENAME == _T("res\\shader\\mesh_shader.fx"))
    {
        if (SharedObj::GetMap()->IsFinishCaveInFade())
        {
            hResult = m_D3DEffect->SetBool("g_inCaveFadeFinish", SharedObj::GetPlayer()->IsInCave());
            assert(hResult == S_OK);
        }
    }

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
        if (SHADER_FILENAME == _T("res\\shader\\mesh_shader.fx")||
            SHADER_FILENAME == _T("res\\shader\\mesh_shader_2Texture.fx"))
        {
            // 超巨大なオブジェクトに霧をかけると霧の色しか見えなくなってしまうので切りかけない。
            if (m_meshName.find(_T("continent.x")) != std::wstring::npos)
            {
                hResult = m_D3DEffect->SetFloat("g_fog_strength", 1.0f);
                assert(hResult == S_OK);
            }
            else
            {
                hResult = m_D3DEffect->SetFloat("g_fog_strength", 1.0f);
                assert(hResult == S_OK);
            }
        }
    }
    else
    {
        fog_color.x = 0.3f;
        fog_color.y = 0.3f;
        fog_color.z = 0.5f;
        fog_color.w = 1.0f;

        // 雨だったら霧を3倍強くする。
        // 霧をサポートしないシェーダーがセットされている可能性があるので
        // mesh_shader.fxの時だけ適用する
        if (SHADER_FILENAME == _T("res\\shader\\mesh_shader.fx")||
            SHADER_FILENAME == _T("res\\shader\\mesh_shader_2Texture.fx"))
        {
            // 超巨大なオブジェクトに霧をかけると霧の色しか見えなくなってしまうので切りかけない。
            if (m_meshName.find(_T("continent.x")) != std::wstring::npos)
            {
                hResult = m_D3DEffect->SetFloat("g_fog_strength", 100.0f);
                assert(hResult == S_OK);
            }
            else
            {
                hResult = m_D3DEffect->SetFloat("g_fog_strength", 100.0f);
                assert(hResult == S_OK);
            }
        }
    }

    hResult = m_D3DEffect->SetVector("fog_color", &fog_color);
    assert(hResult == S_OK);

    //--------------------------------------------------------
    // ワールド変換行列を設定
    //--------------------------------------------------------
    D3DXMATRIX worldViewProjMatrix { };
    D3DXMatrixIdentity(&worldViewProjMatrix);
    {
        D3DXMATRIX mat { };

        // 武器か否か
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
    // カメラの位置を設定
    //--------------------------------------------------------
    D3DXVECTOR4 cameraPos { };
    cameraPos.x = Camera::GetEyePos().x; 
    cameraPos.y = Camera::GetEyePos().y;
    cameraPos.z = Camera::GetEyePos().z;
    cameraPos.w = 0.f;

    hResult = m_D3DEffect->SetVector("g_cameraPos", &cameraPos);
    assert(hResult == S_OK);

    //--------------------------------------------------------
    // ワールドビュー射影変換行列を設定
    //--------------------------------------------------------
    worldViewProjMatrix *= Camera::GetViewMatrix();
    worldViewProjMatrix *= Camera::GetProjMatrix();

    hResult = m_D3DEffect->SetMatrix("g_world_view_projection", &worldViewProjMatrix);
    assert(hResult == S_OK);

    //--------------------------------------------------------
    // 描画開始
    //--------------------------------------------------------
    hResult = m_D3DEffect->Begin(nullptr, 0);
    assert(hResult == S_OK);

    hResult = m_D3DEffect->BeginPass(0);
    assert(hResult == S_OK);

    //--------------------------------------------------------
    // マテリアルの数だけ色とテクスチャを設定して描画
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

        // prolitan.xの場合に限り、もう一枚テクスチャを使う
        if (m_meshName == L"res\\model\\prolitan.x")
        {
            hResult = m_D3DEffect->SetTexture("g_mesh_texture2", m_vecTexture.at(1));
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

std::wstring Mesh::GetMeshName()
{
    return m_meshName;
}

