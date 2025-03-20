#pragma once

#include "../../StarmanLib/StarmanLib/StarmanLib/Voyage.h"
#include "Common.h"
#include "AnimMesh.h"
#include "Mesh.h"

class Raft2
{
public:

    void Init(const int id);
    void Finalize();
    void Update();
    void Draw();
    void SetSail(const bool arg);

    void PullOarBoth();
    void PullOarLeft();
    void PullOarRight();

    D3DXVECTOR3 GetPos() const;
    void SetPos(const D3DXVECTOR3& pos);

    D3DXVECTOR3 GetRotate() const;
    void SetRotate(const D3DXVECTOR3& rot);

    Mesh* GetMesh();
    int GetId();

private:

    int m_id = 0;

    Mesh* m_meshRaft = nullptr;
    AnimMesh* m_meshSail = nullptr;
    AnimMesh* m_meshOarLeft = nullptr;
    AnimMesh* m_meshOarRight = nullptr;
    AnimMesh* m_meshCord = nullptr;

    // 移動量
    D3DXVECTOR3 m_move = D3DXVECTOR3(0.f, 0.f, 0.f);

    // 移動量（回転）
    D3DXVECTOR3 m_moveRot = D3DXVECTOR3(0.f, D3DX_PI, 0.f);

    D3DXVECTOR3 m_pos = D3DXVECTOR3(0.f, 0.f, 0.f);
    D3DXVECTOR3 m_rotate = D3DXVECTOR3(0.f, D3DX_PI, 0.f);
};

class VoyageManager
{
public:
    static VoyageManager* Get();
    void Init();
    void Finalize();
    void Update(eBattleState* state);
    void Draw();
    void SetSail(const bool arg);
    bool GetSail() const;

    // 現在の方角に３時間漕ぐ
    void Set3HoursAuto();
    void SetRaftMode(const bool arg);
    bool GetRaftMode() const;

    NSStarmanLib::Raft::ePosType GetPosType();
    void SetPosType(const NSStarmanLib::Raft::ePosType arg);

    // イカダが2メートル以内にあるか？
    bool CheckNearRaft(const D3DXVECTOR3& pos);
    int GetNearRaftId(const D3DXVECTOR3& pos);

    // 乗船しているイカダのID
    // -1は乗船していないを意味する？
    void Ride();

    // イカダをいくつ所有しているか
    int GetRaftCount();

    bool Intersect(const D3DXVECTOR3& pos, const D3DXVECTOR3& move);
    D3DXVECTOR3 WallSlide(const D3DXVECTOR3& pos, const D3DXVECTOR3& move);

    D3DXVECTOR3 GetRaftXYZ(const int id);
    float GetRaftRotateY(const int id);

    // 乗船中のイカダの耐久値
    int GetRaftDurability();

    // 乗船中のイカダの強化値
    int GetRaftLevel();

private:

    D3DXVECTOR3 WallSlideSub(const D3DXVECTOR3& pos,
                             Mesh* mesh,
                             const D3DXVECTOR3& move);

    static VoyageManager* m_obj;

    VoyageManager();

    // イカダは複数存在できる
    std::map<int, Raft2> m_raftMap;
};

