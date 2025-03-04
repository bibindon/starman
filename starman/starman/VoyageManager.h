#pragma once

#include "../../StarmanLib/StarmanLib/StarmanLib/Voyage.h"
#include "Common.h"
#include "AnimMesh.h"

class Raft2
{
public:

    void Init(const int id);
    void Finalize();
    void Update();
    void Draw();
    void SetSail(const bool arg);
    bool GetSail() const;

    void PullOarBoth();
    void PullOarLeft();
    void PullOarRight();

    auto GetPos() const;
    void SetPos(const D3DXVECTOR3& pos);

    auto GetRotate() const;
    void SetRotate(const D3DXVECTOR3& rot);

private:

    int m_id = 0;

    AnimMesh* m_meshRaft = nullptr;
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
    void Init();
    void Finalize();
    void Update(eBattleState* state);
    void Draw();
    void SetSail(const bool arg);
    bool GetSail() const;

    // 現在の方角に３時間漕ぐ
    void Set3HoursAuto();
    void SetRaftMode(const bool arg);

private:

    // イカダは複数存在できる
    std::map<int, Raft2> m_raftMap;
};

