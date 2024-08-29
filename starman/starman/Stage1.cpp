#include "Stage1.h"
#include "Light.h"
#include "SharedObj.h"

Stage1::Stage1()
{
}

Stage1::~Stage1()
{
    SAFE_DELETE(m_mesh1);
    SAFE_DELETE(m_mesh2);
    SAFE_DELETE(m_mesh3);
    SAFE_DELETE(m_mesh4);
    SAFE_DELETE(m_meshColli);
    SAFE_DELETE(m_meshTree);
    SAFE_DELETE(m_meshCottage);
    SAFE_DELETE(m_meshSky);
    SAFE_DELETE(m_meshSky2);
    SAFE_DELETE(m_spriteStageName);
}

void Stage1::Init()
{
    {
        // 拡大縮小すると衝突判定が効かなくなる！！！
        m_mesh1 = new Mesh(
            "res\\model\\stage1\\stage1.x",
            D3DXVECTOR3(0.f, 0.f, 0.f),
            D3DXVECTOR3(0.f, 0.f, 0.f),
            1.0f);
    }
    {
        m_mesh2 = new Mesh(
            "res\\model\\cube6\\cube6.x",
            D3DXVECTOR3(-10.f, 0.f, 0.f),
            D3DXVECTOR3(0.f, 0.f, 0.f),
            1.0f);
        m_mesh3 = new Mesh(
            "res\\model\\cube6\\cube6.x",
            D3DXVECTOR3(-13.f, 1.f, 0.f),
            D3DXVECTOR3(0.f, 0.f, 0.f),
            1.0f);
        m_mesh4 = new Mesh(
            "res\\model\\cube6\\cube6.x",
            D3DXVECTOR3(-16.f, 2.f, 0.f),
            D3DXVECTOR3(0.f, 0.f, 0.f),
            1.0f);
        m_meshColli = new Mesh(
            "res\\model\\collisionTest\\colli.x",
            D3DXVECTOR3(0.f, 0.f, -20.f),
            D3DXVECTOR3(0.f, 0.f, 0.f),
            1.0f);
    }
    {
        Enemy enemy;
        enemy.Init();
        D3DXVECTOR3 pos = D3DXVECTOR3(0.f, 0.f, 25.f);
        enemy.SetPos(pos);
        m_vecEnemy.push_back(enemy);
    }
    {
        Enemy enemy;
        enemy.Init();
        D3DXVECTOR3 pos = D3DXVECTOR3(50.f, 0.f, 5.f);
        enemy.SetPos(pos);
        m_vecEnemy.push_back(enemy);
    }
    {
        Enemy enemy;
        enemy.Init();
        D3DXVECTOR3 pos = D3DXVECTOR3(30.f, 0.f, 13.f);
        enemy.SetPos(pos);
        m_vecEnemy.push_back(enemy);
    }
    m_spriteStageName = new Sprite("res\\image\\stage1.png");
    m_nStagenameCount = 0;

    {
        D3DXVECTOR3 b = D3DXVECTOR3(0, 0.f, 0);
        D3DXVECTOR3 c = D3DXVECTOR3(0, 0, 0);
        b.x = 15.f;
        b.y = -80.f;
        m_meshSky = new Mesh("res\\model\\hemisphere\\hemisphere.x", b, c, 1000.0f);
        c.y = D3DX_PI;
        m_meshSky2 = new Mesh("res\\model\\hemisphere\\hemisphere.x", b, c, 1000.0f);
    }
    {
        D3DXVECTOR3 b = D3DXVECTOR3(22.f, 0.f, 0.f);
        D3DXVECTOR3 c = D3DXVECTOR3(0.f, 0.f, 0.f);
        m_meshTree = new Mesh("res\\model\\tree1\\tree1.x", b, c, 0.5f);
    }
    {
        D3DXVECTOR3 b = D3DXVECTOR3(10.f, 0.f, 20.f);
        D3DXVECTOR3 c = D3DXVECTOR3(0.f, 0.f, 0.f);
        m_meshCottage = new Mesh("res\\model\\cottage\\cottage.x", b, c, 1.f);
    }
}

void Stage1::Update()
{
    for (auto it = m_vecEnemy.begin(); it != m_vecEnemy.end();)
    {
        it->Update();
        if (it->GetState() == eState::DISABLE)
        {
            it->Finalize();
            it = m_vecEnemy.erase(it);
        }
        else
        {
            it++;
        }
    }
    m_nStagenameCount++;
}

void Stage1::Render()
{
    {
        D3DXVECTOR4 norm = Light::GetLightNormal();
        D3DXVECTOR4 norm2 { norm };
        // 天球用に下から光を当てていることにする
        // 正しくないやり方。
        norm2.x = 0.f;
        norm2.y = -1.f;
        Light::SetLightNormal(norm2);
        m_meshSky->Render();
        m_meshSky2->Render();
        Light::SetLightNormal(norm);
    }
    m_mesh1->Render();
    m_mesh2->Render();
    m_mesh3->Render();
    m_mesh4->Render();
    m_meshColli->Render();
    m_meshCottage->Render();
    m_meshTree->Render();
    for (std::size_t i = 0; i < m_vecEnemy.size(); i++)
    {
        m_vecEnemy.at(i).Render();
    }
    if (m_nStagenameCount <= 255)
    {
        m_spriteStageName->Render(D3DXVECTOR3 {0.f, 0.f, 0.f}, 255 - m_nStagenameCount);
    }
}

std::vector<Enemy> Stage1::GetEnemy()
{
    return m_vecEnemy;
}

void Stage1::SetEnemy(const std::vector<Enemy>& vecEnemy)
{
    m_vecEnemy = vecEnemy;
}

bool Stage1::Intersect(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
    // ステージ上のオブジェクトを原点としたときのposの位置
    BOOL  bIsHit = false;

    {
        D3DXVECTOR3 targetPos = pos - m_mesh2->GetPos();
        LPD3DXMESH mesh = m_mesh2->GetD3DMesh();
        float fLandDistance;
        DWORD dwHitIndex = -1;
        float fHitU;
        float fHitV;
        D3DXIntersect(mesh, &targetPos, &rot, &bIsHit, &dwHitIndex,
            &fHitU, &fHitV, &fLandDistance, NULL, NULL);
        if (bIsHit && fLandDistance <= 1.f)
        {
            return true;
        }
        else
        {
            bIsHit = false;
        }
    }
    {
        D3DXVECTOR3 targetPos = pos - m_mesh3->GetPos();
        LPD3DXMESH mesh = m_mesh3->GetD3DMesh();
        float fLandDistance;
        DWORD dwHitIndex = -1;
        float fHitU;
        float fHitV;
        D3DXIntersect(mesh, &targetPos, &rot, &bIsHit, &dwHitIndex,
            &fHitU, &fHitV, &fLandDistance, NULL, NULL);
        if (bIsHit && fLandDistance <= 1.f)
        {
            return true;
        }
        else
        {
            bIsHit = false;
        }
    }
    {
        D3DXVECTOR3 targetPos = pos - m_mesh4->GetPos();
        LPD3DXMESH mesh = m_mesh4->GetD3DMesh();
        float fLandDistance;
        DWORD dwHitIndex = -1;
        float fHitU;
        float fHitV;
        D3DXIntersect(mesh, &targetPos, &rot, &bIsHit, &dwHitIndex,
            &fHitU, &fHitV, &fLandDistance, NULL, NULL);
        if (bIsHit && fLandDistance <= 1.f)
        {
            return true;
        }
        else
        {
            bIsHit = false;
        }
    }
    {
        D3DXVECTOR3 targetPos = pos - m_meshColli->GetPos();
        LPD3DXMESH mesh = m_meshColli->GetD3DMesh();
        float fLandDistance;
        DWORD dwHitIndex = -1;
        float fHitU;
        float fHitV;
        D3DXIntersect(mesh, &targetPos, &rot, &bIsHit, &dwHitIndex,
            &fHitU, &fHitV, &fLandDistance, NULL, NULL);
        if (bIsHit && fLandDistance <= 1.f)
        {
            //return true;
        }
        else
        {
            bIsHit = false;
        }
    }
    {
        D3DXVECTOR3 targetPos = pos - m_meshTree->GetPos();
        LPD3DXMESH mesh = m_meshTree->GetD3DMesh();
        float fLandDistance;
        DWORD dwHitIndex = -1;
        float fHitU;
        float fHitV;
        D3DXIntersect(mesh, &targetPos, &rot, &bIsHit, &dwHitIndex,
            &fHitU, &fHitV, &fLandDistance, NULL, NULL);
        if (bIsHit && fLandDistance <= 1.f)
        {
            return true;
        }
        else
        {
            bIsHit = false;
        }
    }
    {
        D3DXVECTOR3 targetPos = pos - m_meshCottage->GetPos();
        LPD3DXMESH mesh = m_meshCottage->GetD3DMesh();
        float fLandDistance;
        DWORD dwHitIndex = -1;
        float fHitU;
        float fHitV;
        D3DXIntersect(mesh, &targetPos, &rot, &bIsHit, &dwHitIndex,
            &fHitU, &fHitV, &fLandDistance, NULL, NULL);
        if (bIsHit && fLandDistance <= 1.0f)
        {
            return true;
        }
        else
        {
            bIsHit = false;
        }
    }

    return bIsHit;
}

bool Stage1::CollisionGround(const D3DXVECTOR3& pos, const D3DXVECTOR3& move)
{
    const D3DXVECTOR3 rot {move};
    BOOL  bIsHit = false;
    {
        D3DXVECTOR3 targetPos = pos - m_mesh2->GetPos();
        LPD3DXMESH mesh = m_mesh2->GetD3DMesh();
        float fLandDistance;
        DWORD dwHitIndex = -1;
        float fHitU;
        float fHitV;
        D3DXIntersect(mesh, &targetPos, &rot, &bIsHit, &dwHitIndex,
            &fHitU, &fHitV, &fLandDistance, NULL, NULL);
        if (bIsHit && fLandDistance <= 2.f)
        {
            return true;
        }
        else
        {
            bIsHit = false;
        }
    }
    {
        D3DXVECTOR3 targetPos = pos - m_mesh3->GetPos();
        LPD3DXMESH mesh = m_mesh3->GetD3DMesh();
        float fLandDistance;
        DWORD dwHitIndex = -1;
        float fHitU;
        float fHitV;
        D3DXIntersect(mesh, &targetPos, &rot, &bIsHit, &dwHitIndex,
            &fHitU, &fHitV, &fLandDistance, NULL, NULL);
        if (bIsHit && fLandDistance <= 2.f)
        {
            return true;
        }
        else
        {
            bIsHit = false;
        }
    }
    {
        D3DXVECTOR3 targetPos = pos - m_mesh4->GetPos();
        LPD3DXMESH mesh = m_mesh4->GetD3DMesh();
        float fLandDistance;
        DWORD dwHitIndex = -1;
        float fHitU;
        float fHitV;
        D3DXIntersect(mesh, &targetPos, &rot, &bIsHit, &dwHitIndex,
            &fHitU, &fHitV, &fLandDistance, NULL, NULL);
        if (bIsHit && fLandDistance <= 2.f)
        {
            return true;
        }
        else
        {
            bIsHit = false;
        }
    }
    {
        D3DXVECTOR3 targetPos = pos - m_meshColli->GetPos();
        LPD3DXMESH mesh = m_meshColli->GetD3DMesh();
        float fLandDistance;
        DWORD dwHitIndex = -1;
        float fHitU;
        float fHitV;
        D3DXIntersect(mesh, &targetPos, &rot, &bIsHit, &dwHitIndex,
            &fHitU, &fHitV, &fLandDistance, NULL, NULL);
        if (bIsHit && fLandDistance <= 1.f)
        {
            return true;
        }
        else
        {
            bIsHit = false;
        }
    }
    {
        D3DXVECTOR3 targetPos = pos - m_meshTree->GetPos();
        LPD3DXMESH mesh = m_meshTree->GetD3DMesh();
        float fLandDistance;
        DWORD dwHitIndex = -1;
        float fHitU;
        float fHitV;
        D3DXIntersect(mesh, &targetPos, &rot, &bIsHit, &dwHitIndex,
            &fHitU, &fHitV, &fLandDistance, NULL, NULL);
        if (bIsHit && fLandDistance <= 1.f)
        {
            return true;
        }
        else
        {
            bIsHit = false;
        }
    }
    {
        D3DXVECTOR3 targetPos = pos - m_meshCottage->GetPos();
        LPD3DXMESH mesh = m_meshCottage->GetD3DMesh();
        float fLandDistance;
        DWORD dwHitIndex = -1;
        float fHitU;
        float fHitV;
        D3DXIntersect(mesh, &targetPos, &rot, &bIsHit, &dwHitIndex,
            &fHitU, &fHitV, &fLandDistance, NULL, NULL);
        if (bIsHit && fLandDistance <= 2.f)
        {
            return true;
        }
        else
        {
            bIsHit = false;
        }
    }
    {
        D3DXVECTOR3 targetPos = pos - m_mesh1->GetPos();
//        targetPos.y += 0.1f;
        LPD3DXMESH mesh = m_mesh1->GetD3DMesh();
        float fLandDistance;
        DWORD dwHitIndex = -1;
        float fHitU;
        float fHitV;
        D3DXIntersect(mesh, &targetPos, &rot, &bIsHit, &dwHitIndex,
            &fHitU, &fHitV, &fLandDistance, NULL, NULL);
        if (bIsHit && fLandDistance <= 2.f)
        {
            return true;
        }
        else
        {
            bIsHit = false;
        }
    }

    return bIsHit;
}

void Stage1::WallSlide(const D3DXVECTOR3& pos, const D3DXVECTOR3& move)
{
    const D3DXVECTOR3 rot {move};
    BOOL  bIsHit = false;
    {
        D3DXVECTOR3 targetPos = pos - m_meshColli->GetPos();
        LPD3DXMESH mesh = m_meshColli->GetD3DMesh();
        float fLandDistance;
        DWORD dwHitIndex = -1;
        float fHitU;
        float fHitV;
        D3DXVECTOR3 rot2 { 0.f, 0.2f, 0.f };
        D3DXIntersect(mesh, &targetPos, &rot, &bIsHit, &dwHitIndex,
            &fHitU, &fHitV, &fLandDistance, NULL, NULL);
        if (bIsHit && fLandDistance <= 2.f)
        {
            OutputDebugString("IsHit\n");

            // ----- キャラY座標補正 -----
            // 当たったインデックスバッファ取得
            WORD dwHitVertexNo[3];
            WORD* pIndex;
            HRESULT hr = mesh->LockIndexBuffer(0, (void**)&pIndex);

            for (int nIdxIdx = 0; nIdxIdx < 3; nIdxIdx++)
            {
                dwHitVertexNo[nIdxIdx] = pIndex[dwHitIndex * 3 + nIdxIdx];
            }

            mesh->UnlockIndexBuffer();

            // 当たったポリゴン取得
            struct VERTEX
            {
                FLOAT x, y, z; // 頂点の座標
                FLOAT normX, normY, normZ; // 法線の座標
                FLOAT u, v;   // 頂点の色
            };
            VERTEX* pVertex;
            hr = mesh->LockVertexBuffer(0, (void**)&pVertex);

            // 地面の高さに合わせる
            D3DXVECTOR3 move = SharedObj::GetPlayer()->GetMove();
            D3DXVECTOR3 p1 {pVertex[dwHitVertexNo[0]].x, pVertex[dwHitVertexNo[0]].y, pVertex[dwHitVertexNo[0]].z};
            D3DXVECTOR3 p2 {pVertex[dwHitVertexNo[1]].x, pVertex[dwHitVertexNo[1]].y, pVertex[dwHitVertexNo[1]].z};
            D3DXVECTOR3 p3 {pVertex[dwHitVertexNo[2]].x, pVertex[dwHitVertexNo[2]].y, pVertex[dwHitVertexNo[2]].z};

            D3DXVECTOR3 v1 = p2 - p1;
            D3DXVECTOR3 v2 = p3 - p1;

            D3DXVECTOR3 normal;

            D3DXVec3Cross(&normal, &v1, &v2);
            D3DXVECTOR3 normal_n;
            D3DXVec3Normalize(&normal_n, &normal);

            D3DXVECTOR3 front;
            front = move;

            D3DXVECTOR3 work;
            work = (front - D3DXVec3Dot(&front, &normal_n) * normal_n);

            mesh->UnlockVertexBuffer();
            SharedObj::GetPlayer()->SetMove(work);
            return ;
        }
        else
        {
            bIsHit = false;
        }
    }
}

