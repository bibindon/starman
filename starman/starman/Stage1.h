#pragma once
#include <unordered_map>
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

    // 衝突判定
    bool Intersect(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);
    bool IntersectSub(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, Mesh* mesh);

    // 設置判定
    bool CollisionGround(const D3DXVECTOR3& pos, const D3DXVECTOR3& move);
    bool CollisionGroundSub(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, Mesh* mesh);

    // 壁ずり処理
    // 現在、斜面を登るために、地面に対しては壁ずり処理をするが
    // 壁に対して壁ずりをする予定はない。
    D3DXVECTOR3 WallSlide(const D3DXVECTOR3& pos, const D3DXVECTOR3& move);
private:
    void WallSlideSub(const D3DXVECTOR3& pos, Mesh* mesh, const D3DXVECTOR3& move, D3DXVECTOR3& result);
    std::unordered_map<std::string, Mesh*> m_meshMap;
    std::vector<Enemy> m_vecEnemy { };
    int m_nStagenameCount { 0 };
    Sprite* m_spriteStageName { nullptr };
};

