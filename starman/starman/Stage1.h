#pragma once
#include "Mesh.h"
#include "Enemy.h"
class Stage1
{
public:
    Stage1();
    ~Stage1();
    void Init();
    void Update();
    void Render();
    std::vector<Enemy> GetEnemy();
    void SetEnemy(const std::vector<Enemy>& vecEnemy);
    bool Intersect(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);
    bool CollisionGround(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);
private:
    Mesh* m_mesh1 { nullptr };
    Mesh* m_mesh2 { nullptr };
    Mesh* m_mesh3 { nullptr };
    Mesh* m_mesh4 { nullptr };
    Mesh* m_meshTree { nullptr };
    Mesh* m_meshCottage { nullptr };
    Mesh* m_meshSky { nullptr };
    Mesh* m_meshSky2 { nullptr };
    std::vector<Enemy> m_vecEnemy { };
    int m_nStagenameCount { 0 };
    Sprite* m_spriteStageName { nullptr };
};

