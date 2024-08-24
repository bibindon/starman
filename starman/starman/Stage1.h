#pragma once
#include "Mesh.h"
#include "Enemy.h"
class Stage1
{
public:
    Stage1();
    void Init();
    void Update();
    void Render();
    std::vector<Enemy> GetEnemy();
private:
    Mesh* m_mesh1 { nullptr };
    std::vector<Enemy> m_vecEnemy { };
};

