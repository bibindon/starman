#include "Stage1.h"
#include "Light.h"
#include "SharedObj.h"

Stage1::Stage1()
{
}

Stage1::~Stage1()
{
    for (auto pair : m_meshMap)
    {
        SAFE_DELETE(pair.second);
    }
    SAFE_DELETE(m_spriteStageName);
}

void Stage1::Init()
{
    // 拡大縮小すると衝突判定が効かなくなる！！！
    {
        Mesh* mesh = new Mesh(
            //"res\\model\\stage1\\stage1.x",
            "res\\model\\prolitan\\prolitan.x",
            D3DXVECTOR3(0.f, 0.f, 0.f),
            D3DXVECTOR3(0.f, 0.f, 0.f),
            1.0f);
        mesh->Init();
        m_meshMap["stage1"] = mesh;
    }
    {
        Mesh* mesh = new Mesh(
            "res\\model\\prolitan\\sea.x",
            D3DXVECTOR3(10.f, 10.f, 10.f),
            D3DXVECTOR3(0.f, 0.f, 0.f),
            1.0f);
        mesh->Init();
        m_meshMap["sea"] = mesh;
    }
    {
        Mesh* mesh = new Mesh(
            "res\\model\\cube6\\cube6.x",
            D3DXVECTOR3(-10.f, 0.f, 0.f),
            D3DXVECTOR3(0.f, 0.f, 0.f),
            1.0f);
        mesh->Init();
        m_meshMap["cube61"] = mesh;

        mesh = new Mesh(
            "res\\model\\cube6\\cube6.x",
            D3DXVECTOR3(-13.f, 1.f, 0.f),
            D3DXVECTOR3(0.f, 0.f, 0.f),
            1.0f);
        mesh->Init();
        m_meshMap["cube62"] = mesh;

        mesh = new Mesh(
            "res\\model\\cube6\\cube6.x",
            D3DXVECTOR3(-16.f, 2.f, 0.f),
            D3DXVECTOR3(0.f, 0.f, 0.f),
            1.0f);
        mesh->Init();
        m_meshMap["cube63"] = mesh;

        mesh = new Mesh(
            "res\\model\\collisionTest\\colli.x",
            D3DXVECTOR3(0.f, 0.f, -20.f),
            D3DXVECTOR3(0.f, 0.f, 0.f),
            1.0f);
        mesh->Init();
        m_meshMap["colli"] = mesh;
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
        Mesh* mesh { nullptr };
        D3DXVECTOR3 b = D3DXVECTOR3(0.f, 0.f, 0.f);
        D3DXVECTOR3 c = D3DXVECTOR3(0.f, 0.f, 0.f);
        b.x = 15.f;
        b.y = -80.f;
        mesh = new Mesh("res\\model\\hemisphere\\hemisphere.x", b, c, 3000.0f);
        mesh->Init();
        m_meshMap["sky"] = mesh;

        c.y = D3DX_PI;
        mesh = new Mesh("res\\model\\hemisphere\\hemisphere.x", b, c, 3000.0f);
        mesh->Init();
        m_meshMap["sky2"] = mesh;
    }
    {
        D3DXVECTOR3 b = D3DXVECTOR3(22.f, 0.f, 0.f);
        D3DXVECTOR3 c = D3DXVECTOR3(0.f, 0.f, 0.f);
        Mesh* mesh = new Mesh("res\\model\\tree1\\tree1.x", b, c, 1.f);
        mesh->Init();
        m_meshMap["tree"] = mesh;
    }
    {
        D3DXVECTOR3 b = D3DXVECTOR3(10.f, 0.f, 20.f);
        D3DXVECTOR3 c = D3DXVECTOR3(0.f, 0.f, 0.f);
        Mesh* mesh = new Mesh("res\\model\\cottage\\cottage.x", b, c, 1.f);
        mesh->Init();
        m_meshMap["cottage"] = mesh;
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
        m_meshMap["sky"]->Render();
        m_meshMap["sky2"]->Render();
        Light::SetLightNormal(norm);
    }
    for (auto pair : m_meshMap)
    {
        if (pair.first == "sky" || pair.first == "sky2")
        {
            continue;
        }
        pair.second->Render();
    }
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

bool Stage1::IntersectSub(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, Mesh* mesh)
{
    BOOL  bIsHit = false;
    D3DXVECTOR3 targetPos = pos - mesh->GetPos();
    LPD3DXMESH d3dmesh = mesh->GetD3DMesh();
    float fLandDistance;
    DWORD dwHitIndex = -1;
    float fHitU;
    float fHitV;
    D3DXIntersect(d3dmesh, &targetPos, &rot, &bIsHit, &dwHitIndex,
        &fHitU, &fHitV, &fLandDistance, NULL, NULL);
    if (bIsHit && fLandDistance <= 1.f)
    {
        bIsHit = true;
    }
    else
    {
        bIsHit = false;
    }
    return bIsHit;
}

bool Stage1::Intersect(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
    // ステージ上のオブジェクトを原点としたときのposの位置
    BOOL  bIsHit = false;
    for (auto pair : m_meshMap)
    {
        bIsHit = IntersectSub(pos, rot, pair.second);
        if (bIsHit)
        {
            break;
        }
    }
    return bIsHit;
}

bool Stage1::CollisionGround(const D3DXVECTOR3& pos, const D3DXVECTOR3& move)
{
    // ステージ上のオブジェクトを原点としたときのposの位置
    BOOL  bIsHit = false;
    for (auto pair : m_meshMap)
    {
        bIsHit = IntersectSub(pos, move, pair.second);
        if (bIsHit)
        {
            break;
        }
    }
    return bIsHit;
}

bool Stage1::CollisionGroundSub(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, Mesh* mesh)
{
    BOOL  bIsHit = false;
    D3DXVECTOR3 targetPos = pos - mesh->GetPos();
    LPD3DXMESH d3dmesh = mesh->GetD3DMesh();
    float fLandDistance;
    DWORD dwHitIndex = -1;
    float fHitU;
    float fHitV;
    D3DXIntersect(d3dmesh, &targetPos, &rot, &bIsHit, &dwHitIndex,
        &fHitU, &fHitV, &fLandDistance, NULL, NULL);
    if (bIsHit && fLandDistance <= 2.f)
    {
        bIsHit = true;
    }
    else
    {
        bIsHit = false;
    }
    return bIsHit;
}

D3DXVECTOR3 Stage1::WallSlide(const D3DXVECTOR3& pos, const D3DXVECTOR3& move, bool* bHit)
{
    D3DXVECTOR3 result { move };
    for (auto pair : m_meshMap)
    {
        bool bIsHit = false;
        result = WallSlideSub(pos, pair.second, result, &bIsHit);
        if (bIsHit)
        {
            *bHit = true;
        }
    }
    return result;
}

D3DXVECTOR3 Stage1::WallSlideSub(
    const D3DXVECTOR3& pos, Mesh* mesh, const D3DXVECTOR3& move, bool* bHit)
{
    D3DXVECTOR3 result {move};
    D3DXVECTOR3 targetPos = pos - mesh->GetPos();
    LPD3DXMESH d3dmesh = mesh->GetD3DMesh();
    float fLandDistance;
    DWORD dwHitIndex = -1;
    BOOL  bIsHit = false;
    float fHitU;
    float fHitV;
    D3DXVECTOR3 rot2 { 0.f, 0.2f, 0.f };
    D3DXIntersect(d3dmesh, &targetPos, &move, &bIsHit, &dwHitIndex,
        &fHitU, &fHitV, &fLandDistance, NULL, NULL);
    if (bIsHit && fLandDistance <= 2.f)
    {
        OutputDebugString("IsHit\n");
        *bHit = true;

        // ----- キャラY座標補正 -----
        // 当たったインデックスバッファ取得
        WORD dwHitVertexNo[3];
        WORD* pIndex;
        HRESULT hr = d3dmesh->LockIndexBuffer(0, (void**)&pIndex);

        for (int nIdxIdx = 0; nIdxIdx < 3; nIdxIdx++)
        {
            dwHitVertexNo[nIdxIdx] = pIndex[dwHitIndex * 3 + nIdxIdx];
        }

        d3dmesh->UnlockIndexBuffer();

        // 当たったポリゴン取得
        struct VERTEX
        {
            FLOAT x, y, z; // 頂点の座標
            FLOAT normX, normY, normZ; // 法線の座標
            FLOAT u, v;   // 頂点の色
        };
        VERTEX* pVertex;
        hr = d3dmesh->LockVertexBuffer(0, (void**)&pVertex);

        // 地面の高さに合わせる
        D3DXVECTOR3 p1 { pVertex[dwHitVertexNo[0]].x, pVertex[dwHitVertexNo[0]].y, pVertex[dwHitVertexNo[0]].z };
        D3DXVECTOR3 p2 { pVertex[dwHitVertexNo[1]].x, pVertex[dwHitVertexNo[1]].y, pVertex[dwHitVertexNo[1]].z };
        D3DXVECTOR3 p3 { pVertex[dwHitVertexNo[2]].x, pVertex[dwHitVertexNo[2]].y, pVertex[dwHitVertexNo[2]].z };

        D3DXVECTOR3 v1 = p2 - p1;
        D3DXVECTOR3 v2 = p3 - p1;

        D3DXVECTOR3 normal;

        D3DXVec3Cross(&normal, &v1, &v2);
        D3DXVECTOR3 normal_n;
        D3DXVec3Normalize(&normal_n, &normal);

        D3DXVECTOR3 front;
        front = move;

        result = (front - D3DXVec3Dot(&front, &normal_n) * normal_n);

        d3dmesh->UnlockVertexBuffer();
    }
    else
    {
        result = move;
    }
    return result;
}

