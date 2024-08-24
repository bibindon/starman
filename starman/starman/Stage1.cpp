#include "Stage1.h"

Stage1::Stage1()
{
}

void Stage1::Init()
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
}

void Stage1::Render()
{
    m_mesh1->Render();
    for (std::size_t i = 0; i < m_vecEnemy.size(); i++)
    {
        m_vecEnemy.at(i).Render();
    }
}

std::vector<Enemy> Stage1::GetEnemy()
{
    return m_vecEnemy;
}
