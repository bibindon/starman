#pragma once
#include "Mesh.h"
#include "Enemy.h"
#include "EnemyDisk.h"
class Stage3
{
public:
    Stage3();
    ~Stage3();
    void Init();
    void Update();
    void Render();
    std::vector<Enemy> GetEnemy();
    std::vector<EnemyDisk> GetEnemyDisk();
    void SetEnemy(const std::vector<Enemy>& vecEnemy);
    void SetEnemyDisk(const std::vector<EnemyDisk>& vecEnemy);
private:
    Mesh* m_mesh1 { nullptr };
    Mesh* m_meshTree { nullptr };
    Mesh* m_meshCottage { nullptr };
    Mesh* m_meshSky { nullptr };
    Mesh* m_meshSky2 { nullptr };
    std::vector<Enemy> m_vecEnemy { };
    std::vector<EnemyDisk> m_vecEnemyDisk { };
    int m_nStagenameCount { 0 };
    Sprite* m_spriteStageName { nullptr };
};

