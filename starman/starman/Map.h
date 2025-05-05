#pragma once
#include <unordered_map>
#include <thread>
#include "Mesh.h"
#include "EnemyBase.h"
#include "MeshNoShade.h"
#include "MeshClone.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/ItemManager.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/StatusManager.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/MapObjManager.h"

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

    // 高速化。未完成。posがダメ。
    bool IntersectSub2(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, Mesh* mesh);
    bool IntersectSub(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, MeshClone* mesh);

    // 海と接しているか
    bool IntersectWater(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);

    //--------------------------------------------------
    // 設置判定
    //--------------------------------------------------

    // 地面のみ
    bool CollisionGround(const D3DXVECTOR3& pos, const D3DXVECTOR3& move);
    bool CollisionGroundSub(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, Mesh* mesh);
    bool CollisionGroundSub(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, MeshClone* mesh);

    // 壁ずり処理
    D3DXVECTOR3 WallSlide(const D3DXVECTOR3& pos, const D3DXVECTOR3& move, bool* bHit);

    void SetNpcPos(const std::string& name, const D3DXVECTOR3& pos);
    void SetNpcRot(const std::string& name, const float yRot);

    //------------------------------------
    // もの投げ
    //------------------------------------

    void AddThrownItem(const D3DXVECTOR3& pos,
                       const D3DXVECTOR3& move,
                       const std::string& weaponName,
                       const NSStarmanLib::ItemInfo& itemInfo,
                       const float scale = 1.f,
                       const float power = 10.f,
                       const float rotY = 0.f);

    // 投げて地面に落ちた武器のうち近くのものを1つ取得（半径2メートル以内）
    NSStarmanLib::ItemInfo GetThrownItem(const D3DXVECTOR3& pos);

    // 投げて地面に落ちた武器を削除
    void DeleteThrownItem(const NSStarmanLib::ItemInfo& thrownItem);

    //------------------------------------
    // 魔法攻撃
    //------------------------------------

    // 魔法攻撃は武器を投げるのと同じような処理を行う
    // 武器を投げた時と違い、地面に落ちたら消える。
    void SetThrownMagic(const D3DXVECTOR3& pos,
                        const D3DXVECTOR3& move,
                        const NSStarmanLib::eMagicType& magicType,
                        const float power);

    // 草の近くにいるか
    bool NearPlant(const D3DXVECTOR3& pos);

    // 木の近くにいるか
    bool NearTree(const D3DXVECTOR3& pos);

    // ２メートル以内の木を１本消す
    void DeleteTree(const D3DXVECTOR3& pos);

    // ２メートル以内の草を１本消す
    void DeletePlant(const D3DXVECTOR3& pos);

private:
    NSStarmanLib::MapObjManager* MapLib();

    D3DXVECTOR3 WallSlideSub(
        const D3DXVECTOR3& pos, Mesh* mesh, const D3DXVECTOR3& move, bool* bHit);

    D3DXVECTOR3 WallSlideSub(
        const D3DXVECTOR3& pos, MeshClone* mesh, const D3DXVECTOR3& move, bool* bHit);

    std::unordered_map<std::string, Mesh*> m_meshMap;

    std::vector<EnemyBase*> m_vecEnemy { };

    // 太陽
    MeshNoShade* m_pSun = nullptr;

    // NPC
    std::unordered_map<std::string, AnimMesh*> m_NPC;

    std::unordered_map<int, MeshClone*> m_meshCloneMap;

    //-------------------------------------------------------
    // 投げられたもの（武器）
    //-------------------------------------------------------

    // 投げるもの
    struct ThrownItem
    {
        NSStarmanLib::ItemInfo m_itemInfo;
        D3DXVECTOR3 m_move;
        MeshClone* m_mesh;

        // 一度でも敵にヒットした
        bool m_bHit = false;

        // 地面に落ちた
        bool m_bStop = false;

        // 0.5秒待ってから発射
        int m_counter = 0;

        // 威力
        float m_power = 0.f;
    };

    std::vector<ThrownItem> m_thrownList;

    //-------------------------------------------------------
    // 投げられたもの（魔法）
    //-------------------------------------------------------

    // 投げるもの（魔法）
    struct ThrownMagic
    {
        NSStarmanLib::eMagicType m_eMagicType;
        D3DXVECTOR3 m_move;

        MeshNoShade* m_mesh;

        // 一度でも敵にヒットした
        bool m_bHit = false;

        // 1秒待ってから発射
        int m_counter = 0;

        // 威力
        float m_magicPower = 0.f;
    };

    std::vector<ThrownMagic> m_thrownMagicList;

};

