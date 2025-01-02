#pragma once
#include <unordered_map>
#include <thread>
#include "Mesh.h"
#include "EnemyBase.h"
#include "MeshNoShade.h"
#include "MeshClone.h"

class Map
{
public:
    Map();
    ~Map();
    void Init();
    void Update();
    void Render();
    std::vector<EnemyBase*> GetEnemy();
    void SetEnemy(const std::vector<EnemyBase*>& vecEnemy);

    // è’ìÀîªíË
    bool Intersect(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);
    bool IntersectSub(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, Mesh* mesh);
    bool IntersectSub(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, MeshClone* mesh);

    // ê›íuîªíË
    bool CollisionGround(const D3DXVECTOR3& pos, const D3DXVECTOR3& move);
    bool CollisionGroundSub(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, Mesh* mesh);
    bool CollisionGroundSub(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, MeshClone* mesh);

    // ï«Ç∏ÇËèàóù
    D3DXVECTOR3 WallSlide(const D3DXVECTOR3& pos, const D3DXVECTOR3& move, bool* bHit);

    void SetNpcPos(const std::string& name, const D3DXVECTOR3& pos);
    void SetNpcRot(const std::string& name, const float yRot);
private:
    D3DXVECTOR3 WallSlideSub(
        const D3DXVECTOR3& pos, Mesh* mesh, const D3DXVECTOR3& move, bool* bHit);

    D3DXVECTOR3 WallSlideSub(
        const D3DXVECTOR3& pos, MeshClone* mesh, const D3DXVECTOR3& move, bool* bHit);

    std::unordered_map<std::string, Mesh*> m_meshMap;

    std::vector<EnemyBase*> m_vecEnemy { };

    // ëæóz
    MeshNoShade* m_pSun = nullptr;

    // NPC
    std::unordered_map<std::string, AnimMesh*> m_NPC;

    std::unordered_map<int, MeshClone*> m_meshCloneMap;
};

