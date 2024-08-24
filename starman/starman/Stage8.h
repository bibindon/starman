#pragma once
#include "Mesh.h"
#include "Enemy.h"
class Stage8
{
public:
    Stage8();
    ~Stage8();
    void Init();
    void Update();
    void Render();
    std::vector<Enemy> GetEnemy();
    void SetEnemy(const std::vector<Enemy>& vecEnemy);
private:
    Mesh* m_mesh1 { nullptr };
    Mesh* m_meshTree { nullptr };
    Mesh* m_meshCottage { nullptr };
    Mesh* m_meshSky { nullptr };
    Mesh* m_meshSky2 { nullptr };
    std::vector<Enemy> m_vecEnemy { };
    int m_nStagenameCount { 0 };
    Sprite* m_spriteStageName { nullptr };
};

