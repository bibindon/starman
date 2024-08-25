#pragma once
#include "Mesh.h"
#include "Enemy.h"
#include "EnemySphere.h"
class Stage2
{
public:
    Stage2();
    ~Stage2();
    void Init();
    void Update();
    void Render();
    std::vector<Enemy> GetEnemy();
    std::vector<EnemySphere> GetEnemySphere();
    void SetEnemy(const std::vector<Enemy>& vecEnemy);
    void SetEnemySphere(const std::vector<EnemySphere>& vecEnemySphere);
private:
    Mesh* m_mesh1 { nullptr };
    Mesh* m_meshTree { nullptr };
    Mesh* m_meshCottage { nullptr };
    Mesh* m_meshSky { nullptr };
    Mesh* m_meshSky2 { nullptr };
    std::vector<Enemy> m_vecEnemy { };
    std::vector<EnemySphere> m_vecEnemySphere { };
    int m_nStagenameCount { 0 };
    Sprite* m_spriteStageName { nullptr };
};

