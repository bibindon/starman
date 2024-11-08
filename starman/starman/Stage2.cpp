#include "Stage2.h"
#include "Light.h"
#include "SharedObj.h"

Stage2::Stage2()
{
}

Stage2::~Stage2()
{
    SAFE_DELETE(m_mesh1);
    SAFE_DELETE(m_meshTree);
    SAFE_DELETE(m_meshCottage);
    SAFE_DELETE(m_meshSky);
    SAFE_DELETE(m_meshSky2);
    SAFE_DELETE(m_spriteStageName);
}

void Stage2::Init()
{
    {
        m_mesh1 = new Mesh(
            "res\\model\\stage1\\stage1.x",
            D3DXVECTOR3(0.f, 0.f, 0.f),
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
    {
        EnemySphere enemy;
        enemy.Init();
        D3DXVECTOR3 pos = D3DXVECTOR3(10.f, 0.f, 25.f);
        enemy.SetPos(pos);
        m_vecEnemySphere.push_back(enemy);
    }
    {
        EnemySphere enemy;
        enemy.Init();
        D3DXVECTOR3 pos = D3DXVECTOR3(60.f, 0.f, 5.f);
        enemy.SetPos(pos);
        m_vecEnemySphere.push_back(enemy);
    }
    {
        EnemySphere enemy;
        enemy.Init();
        D3DXVECTOR3 pos = D3DXVECTOR3(30.f, 0.f, 23.f);
        enemy.SetPos(pos);
        m_vecEnemySphere.push_back(enemy);
    }
    m_spriteStageName = new Sprite("res\\image\\stage2.png");
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
        D3DXVECTOR3 b = D3DXVECTOR3(20.f, 0.f, 20.f);
        D3DXVECTOR3 c = D3DXVECTOR3(0.f, 0.f, 0.f);
        m_meshTree = new Mesh("res\\model\\tree1\\tree1.x", b, c, 0.5f);
    }
    {
        D3DXVECTOR3 b = D3DXVECTOR3(10.f, 0.f, 20.f);
        D3DXVECTOR3 c = D3DXVECTOR3(0.f, 0.f, 0.f);
        m_meshCottage = new Mesh("res\\model\\cottage\\cottage.x", b, c, 0.5f);
    }
}

void Stage2::Update()
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
    for (auto it = m_vecEnemySphere.begin(); it != m_vecEnemySphere.end();)
    {
        it->Update();
        if (it->GetState() == eSphereState::DISABLE)
        {
            it->Finalize();
            it = m_vecEnemySphere.erase(it);
        }
        else
        {
            it++;
        }
    }
    m_nStagenameCount++;
}

void Stage2::Render()
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
    m_meshCottage->Render();
    m_meshTree->Render();
    for (std::size_t i = 0; i < m_vecEnemy.size(); i++)
    {
        m_vecEnemy.at(i).Render();
    }
    for (std::size_t i = 0; i < m_vecEnemySphere.size(); i++)
    {
        m_vecEnemySphere.at(i).Render();
    }
    if (m_nStagenameCount <= 255)
    {
        m_spriteStageName->Render(D3DXVECTOR3 {0.f, 0.f, 0.f}, 255 - m_nStagenameCount);
    }
}

std::vector<Enemy> Stage2::GetEnemy()
{
    return m_vecEnemy;
}

std::vector<EnemySphere> Stage2::GetEnemySphere()
{
    return m_vecEnemySphere;
}

void Stage2::SetEnemySphere(const std::vector<EnemySphere>& vecEnemySphere)
{
    m_vecEnemySphere = vecEnemySphere;
}

void Stage2::SetEnemy(const std::vector<Enemy>& vecEnemy)
{
    m_vecEnemy = vecEnemy;
}
