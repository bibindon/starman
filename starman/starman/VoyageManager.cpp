#include "VoyageManager.h"
#include "KeyBoard.h"
#include "SharedObj.h"
#include "Mouse.h"
#include "GamePad.h"

NSStarmanLib::Voyage* Voyage()
{
    return NSStarmanLib::Voyage::Get();
}

VoyageManager* VoyageManager::m_obj = nullptr;

VoyageManager* VoyageManager::Get()
{
    if (m_obj == nullptr)
    {
        m_obj = NEW VoyageManager();
    }

    return m_obj;
}

void VoyageManager::Init()
{
    auto raftList = Voyage()->GetRaftList();

    for (auto it = raftList.begin(); it != raftList.end(); ++it)
    {
        auto id = it->GetId();
        Raft2 raft;
        raft.Init(id);
        m_raftMap[id] = raft;
    }
}

void VoyageManager::Finalize()
{
    for (auto it = m_raftMap.begin(); it != m_raftMap.end(); ++it)
    {
        it->second.Finalize();
    }
}

void VoyageManager::Update(eBattleState* state)
{
    // 乗船中ではない。
    if (!Voyage()->GetRaftMode())
    {
        return;
    }

    Voyage()->Update();

    //----------------------------------------------------
    // マウス・キーボード操作
    //----------------------------------------------------
    if (KeyBoard::IsDownFirstFrame(DIK_C))
    {
        *state = eBattleState::COMMAND;
    }

    if (KeyBoard::IsDownFirstFrame(DIK_ESCAPE))
    {
        *state = eBattleState::NORMAL;

        Voyage()->SetRaftMode(false);
    }

    //----------------------------------------------------
    // ゲームパッド操作
    //----------------------------------------------------

    if (GamePad::IsDown(eGamePadButtonType::BACK))
    {
        *state = eBattleState::COMMAND;
    }

    if (GamePad::IsDown(eGamePadButtonType::B))
    {
        *state = eBattleState::NORMAL;

        Voyage()->SetRaftMode(false);
    }

    auto id = Voyage()->GetRaftCurrentId();
    m_raftMap[id].Update();
}

void VoyageManager::Draw()
{
    for (auto it = m_raftMap.begin(); it != m_raftMap.end(); ++it)
    {
        it->second.Draw();
    }
}

void VoyageManager::SetSail(const bool arg)
{
    Voyage()->SetSailCurrentRaft(arg);
    auto id = Voyage()->GetRaftCurrentId();
    m_raftMap[id].SetSail(arg);
}

bool VoyageManager::GetSail() const
{
    return Voyage()->GetSailCurrentRaft();
}

void VoyageManager::Set3HoursAuto()
{
    Voyage()->Set3HoursAuto();
}

void VoyageManager::SetRaftMode(const bool arg)
{
    Voyage()->SetRaftMode(arg);
}

bool VoyageManager::GetRaftMode() const
{
    return Voyage()->GetRaftMode();
}

NSStarmanLib::Raft::ePosType VoyageManager::GetPosType()
{
    auto id = Voyage()->GetRaftCurrentId();
    return Voyage()->GetRaft(id).GetPosType();
}

void VoyageManager::SetPosType(const NSStarmanLib::Raft::ePosType arg)
{
    auto id = Voyage()->GetRaftCurrentId();
    Voyage()->GetRaft(id).SetPosType(arg);
}

bool VoyageManager::CheckNearRaft(const D3DXVECTOR3& pos)
{
    return Voyage()->CheckNearRaft(pos.x, pos.y, pos.z);
}

int VoyageManager::GetRaftCount()
{
    return (int)Voyage()->GetRaftList().size();
}

void Raft2::Init(const int id)
{
    m_id = id;

    auto raft = Voyage()->GetRaft(id);
    float x, y, z;
    raft.GetXYZ(&x, &y, &z);
    m_pos = D3DXVECTOR3(x, y, z);

    D3DXVECTOR3 pos = D3DXVECTOR3(0.f, 0.f, 0.f);
    D3DXVECTOR3 rot = D3DXVECTOR3(0.f, 0.f, 0.f);
    {
        AnimSetMap animSetMap;
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 0.0f;
            animSetting.m_duration = 0.97f;
            animSetting.m_loop = true;
            animSetMap["SailOn"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 1.0f;
            animSetting.m_duration = 0.97f;
            animSetting.m_loop = true;
            animSetMap["SailOff"] = animSetting;
        }
        m_meshRaft = NEW AnimMesh("res\\model\\raft\\raft.x", pos, rot, 1.f, animSetMap);
        m_meshRaft->SetAnim("SailOff");
    }
    {
        AnimSetMap animSetMap;
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 0.0f;
            animSetting.m_duration = 0.49f;
            animSetting.m_loop = false;
            animSetMap["Pull"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 0.5f;
            animSetting.m_duration = 0.49f;
            animSetting.m_loop = true;
            animSetMap["Idle"] = animSetting;
        }
        m_meshOarLeft = NEW AnimMesh("res\\model\\raft\\oarLeft.x", pos, rot, 1.f, animSetMap);
        m_meshOarLeft->SetAnim("Idle");
    }
    {
        AnimSetMap animSetMap;
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 0.0f;
            animSetting.m_duration = 0.49f;
            animSetting.m_loop = false;
            animSetMap["Pull"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 0.5f;
            animSetting.m_duration = 0.49f;
            animSetting.m_loop = true;
            animSetMap["Idle"] = animSetting;
        }
        m_meshOarRight = NEW AnimMesh("res\\model\\raft\\oarRight.x", pos, rot, 1.f, animSetMap);
        m_meshOarRight->SetAnim("Idle");
    }
    {
        AnimSetMap animSetMap;
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 0.f;
            animSetting.m_duration = 1.f;
            animSetting.m_loop = true;
            animSetMap["Idle"] = animSetting;
        }
        m_meshCord = NEW AnimMesh("res\\model\\raft\\cord.x", pos, rot, 1.f, animSetMap);
        m_meshCord->SetAnim("Idle");
    }
}

void Raft2::Finalize()
{
    SAFE_DELETE(m_meshCord);
    SAFE_DELETE(m_meshOarRight);
    SAFE_DELETE(m_meshOarLeft);
    SAFE_DELETE(m_meshRaft);
}

void Raft2::Update()
{
    m_move /= 2.f;
    m_moveRot /= 2.f;

    auto ppos = SharedObj::GetPlayer()->GetPos();

    Voyage()->SetCurrentRaftCoord(ppos.x, ppos.y, ppos.z);

    //----------------------------------------------------
    // マウス・キーボード操作
    //----------------------------------------------------

    //--------------------------------------------
    // 右クリックで右のオールを漕ぐ
    // 左クリックで左のオールを漕ぐ
    // 左右同時押しで両方のオールを漕ぐ
    // 同時押しの猶予を0.33秒持たせる。
    // 長押しは扱わない。一回漕ぐたびに一回クリックする必要がある。
    //--------------------------------------------

    static int counterLeft = 0;
    static int counterRight = 0;

    static bool pendingLeft = false;
    static bool pendingRight = false;

    bool bothClick = false;

    if (Mouse::IsDownLeft())
    {
        counterLeft = 0;
        pendingLeft = true;

        if (pendingRight)
        {
            bothClick = true;
        }
    }

    if (Mouse::IsDownRight())
    {
        counterRight = 0;
        pendingRight = true;

        if (pendingLeft)
        {
            bothClick = true;
        }
    }

    //----------------------------------------------------
    // ゲームパッド操作
    //----------------------------------------------------

    if (GamePad::IsDown(eGamePadButtonType::L1))
    {
        counterLeft = 0;
        pendingLeft = true;

        if (pendingRight)
        {
            bothClick = true;
        }
    }

    if (GamePad::IsDown(eGamePadButtonType::R1))
    {
        counterRight = 0;
        pendingRight = true;

        if (pendingLeft)
        {
            bothClick = true;
        }
    }

    auto id = Voyage()->GetRaftCurrentId();

    if (pendingLeft)
    {
        ++counterLeft;
        if (counterLeft >= 20)
        {
            if (bothClick)
            {
                Voyage()->PullBothOar();
                PullOarBoth();
            }
            else
            {
                Voyage()->PullLeftOar();
                PullOarLeft();
            }

            pendingLeft = false;
            counterLeft = 0;

            pendingRight = false;
            counterRight = 0;
        }
    }

    if (pendingRight)
    {
        ++counterRight;
        if (counterRight >= 20)
        {
            if (bothClick)
            {
                Voyage()->PullBothOar();
                PullOarBoth();
            }
            else
            {
                Voyage()->PullRightOar();
                PullOarRight();
            }

            pendingLeft = false;
            counterLeft = 0;

            pendingRight = false;
            counterRight = 0;
        }
    }

    // 風の強さと方向により流される
    {
        float x, z;
        Voyage()->GetWindXZ(&x, &z);

        if (Voyage()->GetSailCurrentRaft())
        {
            m_move.x += x/10;
            m_move.z += z/10;
        }
        else
        {
            m_move.x += x/100;
            m_move.z += z/100;
        }
    }

    // 潮の強さと方向により流される
    {
        float x, z;
        Voyage()->GetTideXZ(&x, &z);
        m_move.x += x;
        m_move.z += z;
    }

    m_pos += m_move;
    m_rotate += m_moveRot;

    // 衝突判定
    // 島と設置していたら停止

    // イカダで川を進むことも出来ることに注意
}

void Raft2::Draw()
{
    m_meshRaft->SetPos(m_pos);
    m_meshRaft->SetRotate(m_rotate);
    m_meshRaft->Render();

    m_meshOarLeft->SetPos(m_pos);
    m_meshOarLeft->SetRotate(m_rotate);
    m_meshOarLeft->Render();

    m_meshOarRight->SetPos(m_pos);
    m_meshOarRight->SetRotate(m_rotate);
    m_meshOarRight->Render();

    m_meshCord->SetPos(m_pos);

    // TODO 風向き
    m_meshCord->SetRotate(m_rotate);

    m_meshCord->Render();
}

void Raft2::SetSail(const bool arg)
{
    if (arg)
    {
        m_meshRaft->SetAnim("SailOn");
    }
    else
    {
        m_meshRaft->SetAnim("SailOff");
    }
}

bool Raft2::GetSail() const
{
    return false;
}

void Raft2::PullOarBoth()
{
    m_meshOarLeft->SetAnim("Pull");
    m_meshOarRight->SetAnim("Pull");

    m_move.x += std::sin(m_rotate.y);
    m_move.z += std::cos(m_rotate.y);
}

void Raft2::PullOarLeft()
{
    m_meshOarLeft->SetAnim("Pull");
    m_moveRot.y += 0.1f;
}

void Raft2::PullOarRight()
{
    m_meshOarRight->SetAnim("Pull");
    m_moveRot.y += -0.1f;
}

auto Raft2::GetPos() const
{
    return m_pos;
}

void Raft2::SetPos(const D3DXVECTOR3& pos)
{
    m_pos = pos;
}

auto Raft2::GetRotate() const
{
    return m_rotate;
}

void Raft2::SetRotate(const D3DXVECTOR3& rot)
{
    m_rotate = rot;
}

VoyageManager::VoyageManager()
{

}

