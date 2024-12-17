#pragma once
#include <unordered_map>
#include <thread>
#include "Mesh.h"
#include "EnemyBase.h"
#include "MeshNoShade.h"
#include <map>
#include "MeshClone.h"

// 主人公が近づいたらメッシュを読み込み表示するためのメッシュクラス
// 別スレッドで読む。DirectX9はマルチスレッドに対応していないらしいが
// なぜか問題なく動く。
// TODO 別のファイルに移動すべき？
// TODO LazyMeshは削除するべき。Load
class LazyMesh
{
public:
    void Init(const std::string& xFilename,
              const D3DXVECTOR3& position,
              const D3DXVECTOR3& rotation);

    void Load();

    void Unload();

    // 読み込みが開始される座標と半径
    void SetLoadPos(const D3DXVECTOR3& pos, const float r);

    // 読み込みが開始される座標か確認
    bool IsLoadPos(const D3DXVECTOR3& pos);

    // 読み込み済みか
    bool IsLoaded();

    void Render();

private:
    Mesh* m_Mesh { nullptr };
    bool m_bLoaded { false};
    std::thread* m_thread { nullptr };
    D3DXVECTOR3 m_loadingPos { 0.f, 0.f, 0.f };
    float m_radius { 0.f };

    std::string m_xFilename;
    D3DXVECTOR3 m_drawPos { 0.f, 0.f, 0.f };
    D3DXVECTOR3 m_rotation { 0.f, 0.f, 0.f };
};

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

    // 衝突判定
    bool Intersect(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);
    bool IntersectSub(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, Mesh* mesh);
    bool IntersectSub(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, MeshClone* mesh);

    // 設置判定
    bool CollisionGround(const D3DXVECTOR3& pos, const D3DXVECTOR3& move);
    bool CollisionGroundSub(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, Mesh* mesh);
    bool CollisionGroundSub(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, MeshClone* mesh);

    // 壁ずり処理
    D3DXVECTOR3 WallSlide(const D3DXVECTOR3& pos, const D3DXVECTOR3& move, bool* bHit);
private:
    D3DXVECTOR3 WallSlideSub(
        const D3DXVECTOR3& pos, Mesh* mesh, const D3DXVECTOR3& move, bool* bHit);

    D3DXVECTOR3 WallSlideSub(
        const D3DXVECTOR3& pos, MeshClone* mesh, const D3DXVECTOR3& move, bool* bHit);

    std::unordered_map<std::string, Mesh*> m_meshMap;

    std::vector<EnemyBase*> m_vecEnemy { };

    int m_nStagenameCount { 0 };
    Sprite* m_spriteStageName { nullptr };

    LazyMesh m_lazyMesh;

    // 太陽
    MeshNoShade* m_pSun = nullptr;

    // NPC
    std::map<std::string, AnimMesh*> m_NPC;

    std::unordered_map<int, MeshClone*> m_meshCloneMap;
};

