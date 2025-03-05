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

    // �ړ���
    D3DXVECTOR3 m_move = D3DXVECTOR3(0.f, 0.f, 0.f);

    // �ړ��ʁi��]�j
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

    // ���݂̕��p�ɂR���ԑ���
    void Set3HoursAuto();
    void SetRaftMode(const bool arg);
    bool GetRaftMode() const;

    NSStarmanLib::Raft::ePosType GetPosType();
    void SetPosType(const NSStarmanLib::Raft::ePosType arg);

    // �C�J�_��2���[�g���ȓ��ɂ��邩�H
    bool CheckNearRaft(const D3DXVECTOR3& pos);

    // �C�J�_���������L���Ă��邩
    int GetRaftCount();

private:

    static VoyageManager* m_obj;

    VoyageManager();

    // �C�J�_�͕������݂ł���
    std::map<int, Raft2> m_raftMap;
};

