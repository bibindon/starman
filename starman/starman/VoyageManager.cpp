#include "VoyageManager.h"
#include "KeyBoard.h"
#include "SharedObj.h"
#include "Mouse.h"
#include "GamePad.h"
#include "Camera.h"
#include "BGM.h"
#include "SoundEffect.h"

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
        SoundEffect::get_ton()->load("res\\sound\\pullOar.wav");
        SoundEffect::get_ton()->load("res\\sound\\pullOar2.wav");
        SoundEffect::get_ton()->load("res\\sound\\collideRaft.wav");
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

// この関数は航海中でなくても常に呼ばれる。
// イカダを表示するため
void VoyageManager::Update()
{
    // イカダは増えることがあるため、イカダに乗っていなくても
    // m_raftMapは更新されなければいけない。
    // 具体的にはイカダをクラフトしたときに増える
    static int counter = 0;
    counter++;
    if (counter > 60)
    {
        counter = 0;
        auto raftList = Voyage()->GetRaftList();

        for (auto it = raftList.begin(); it != raftList.end(); ++it)
        {
            auto id = it->GetId();
            bool find_ = false;
            for (auto it2 = m_raftMap.begin(); it2 != m_raftMap.end(); ++it2)
            {
                if (it2->second.GetId() == id)
                {
                    find_ = true;
                    break;
                }
            }

            if (!find_)
            {
                Raft2 raft2;
                raft2.Init(id);
                m_raftMap[id] = raft2;
            }
        }
    }
}

void VoyageManager::Operate(eBattleState* state)
{
    // 乗船中ではない。
    if (!Voyage()->GetRaftMode())
    {
        return;
    }

    // 主人公をイカダの中央に座らせる

    // イカダで移動しているとき、移動しているのはイカダであり、
    // 主人公はイカダについてきている、と考える。

    Voyage()->Update(SharedObj::GetPlayer()->GetPos().x,
                     SharedObj::GetPlayer()->GetPos().y,
                     SharedObj::GetPlayer()->GetPos().z);

    //----------------------------------------------------
    // マウス・キーボード操作
    //----------------------------------------------------
    if (KeyBoard::IsDownFirstFrame(DIK_C))
    {
        *state = eBattleState::COMMAND;
    }

    if (KeyBoard::IsDownFirstFrame(DIK_ESCAPE))
    {
        *state = eBattleState::MENU;
        Camera::SetCameraMode(eCameraMode::SLEEP);
        Common::SetCursorVisibility(true);
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
        *state = eBattleState::MENU;
        Camera::SetCameraMode(eCameraMode::SLEEP);
        Common::SetCursorVisibility(true);
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
    
    auto id = Voyage()->GetRaftCurrentId();
    m_raftMap[id].Pull3Hours();
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

int VoyageManager::GetNearRaftId(const D3DXVECTOR3& pos)
{
    int id = 0;
    Voyage()->CheckNearRaft(pos.x, pos.y, pos.z, &id);
    return id;
}

void VoyageManager::Ride()
{
    auto pos = SharedObj::GetPlayer()->GetPos();
    auto id = GetNearRaftId(pos);
    Voyage()->SetRaftCurrentId(id);
    Voyage()->SetRaftMode(true);

    SharedObj::GetPlayer()->RideRaft();
}

int VoyageManager::GetRaftCount()
{
    return (int)Voyage()->GetRaftList().size();
}

bool VoyageManager::Intersect(const D3DXVECTOR3& pos, const D3DXVECTOR3& move)
{
    // 新しい主人公の位置
    auto newPpos = pos + move;
    bool bHit = false;

    for (const auto& it : m_raftMap)
    {
        auto posRaft = it.second.GetPos();
        bool bHitWork = Common::HitByBoundingBox(newPpos, posRaft, 2.f);
        if (bHitWork)
        {
            bHit = true;
            break;
        }
    }

    return bHit;
}

D3DXVECTOR3 VoyageManager::WallSlide(const D3DXVECTOR3& pos, const D3DXVECTOR3& move, bool* bHit)
{
    D3DXVECTOR3 result = move;
    for (auto& pair : m_raftMap)
    {
        bool bHit2 = false;
        result = WallSlideSub(pos, pair.second.GetCollisionMesh(), result, &bHit2);
        if (bHit2)
        {
            *bHit = true;
        }
    }
    return result;
}

D3DXVECTOR3 VoyageManager::GetRaftXYZ(const int id)
{
    return m_raftMap.at(id).GetPos();
}

float VoyageManager::GetRaftRotateY(const int id)
{
    return m_raftMap.at(id).GetRotate().y;
}

int VoyageManager::GetRaftDurability()
{
    auto raft = Voyage()->GetRaftCurrent();
    return raft.GetDurability();
}

int VoyageManager::GetRaftLevel()
{
    auto raft = Voyage()->GetRaftCurrent();
    return raft.GetLevel();
}

bool VoyageManager::Can3HoursAuto()
{
    auto id = Voyage()->GetRaftCurrentId();
    D3DXVECTOR3 rotate = m_raftMap[id].GetRotate();
    D3DXVECTOR3 move(0.f, 0.f, 0.f);

    // TODO いい感じに
    move.x = std::sin(rotate.y) * -10000.f;
    move.z = std::cos(rotate.y) * -10000.f;
    bool bHit = SharedObj::GetMap()->Intersect(SharedObj::GetPlayer()->GetPos(), move);
    if (bHit)
    {
        return false;
    }
    else
    {
        return true;
    }
}

D3DXVECTOR3 VoyageManager::WallSlideSub(const D3DXVECTOR3& pos,
                                        Mesh* mesh,
                                        const D3DXVECTOR3& move,
                                        bool* bHit)
{
    D3DXVECTOR3 result {move};
    D3DXVECTOR3 targetPos = pos - mesh->GetPos();
    targetPos /= mesh->GetScale();
    LPD3DXMESH d3dmesh = mesh->GetD3DMesh();
    float fLandDistance;
    DWORD dwHitIndex = -1;
    BOOL  bIsHit = false;
    float fHitU;
    float fHitV;
    D3DXVECTOR3 rot2 { 0.f, 0.2f, 0.f };

    D3DXIntersect(d3dmesh,
                  &targetPos,
                  &move,
                  &bIsHit,
                  &dwHitIndex,
                  &fHitU,
                  &fHitV,
                  &fLandDistance,
                  NULL,
                  NULL);

    float judgeDistance = 2.f / mesh->GetScale();
    if (bIsHit && fLandDistance <= judgeDistance)
    {
        *bHit = true;

        // ----- キャラY座標補正 -----
        // 当たったインデックスバッファ取得
        WORD dwHitVertexNo[3] = {};
        WORD* pIndex = nullptr;
        HRESULT hr = d3dmesh->LockIndexBuffer(0, (void**)&pIndex);

        for (int nIdxIdx = 0; nIdxIdx < 3; nIdxIdx++)
        {
            dwHitVertexNo[nIdxIdx] = pIndex[dwHitIndex * 3 + nIdxIdx];
        }

        d3dmesh->UnlockIndexBuffer();

        // 当たったポリゴン取得
        struct VERTEX
        {
            FLOAT x, y, z; // 頂点の座標
            FLOAT normX, normY, normZ; // 法線の座標
            FLOAT u, v;   // 頂点の色
        };
        VERTEX* pVertex = nullptr;
        hr = d3dmesh->LockVertexBuffer(0, (void**)&pVertex);

        // 地面の高さに合わせる
        D3DXVECTOR3 p1 { pVertex[dwHitVertexNo[0]].x, pVertex[dwHitVertexNo[0]].y, pVertex[dwHitVertexNo[0]].z };
        D3DXVECTOR3 p2 { pVertex[dwHitVertexNo[1]].x, pVertex[dwHitVertexNo[1]].y, pVertex[dwHitVertexNo[1]].z };
        D3DXVECTOR3 p3 { pVertex[dwHitVertexNo[2]].x, pVertex[dwHitVertexNo[2]].y, pVertex[dwHitVertexNo[2]].z };

        D3DXVECTOR3 v1 = p2 - p1;
        D3DXVECTOR3 v2 = p3 - p1;

        D3DXVECTOR3 normal;

        D3DXVec3Cross(&normal, &v1, &v2);
        D3DXVECTOR3 normal_n;
        D3DXVec3Normalize(&normal_n, &normal);

        D3DXVECTOR3 front;
        front = move;

        result = (front - D3DXVec3Dot(&front, &normal_n) * normal_n);

        d3dmesh->UnlockVertexBuffer();
    }
    else
    {
        result = move;
    }
    return result;
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
        m_meshRaft = NEW Mesh("res\\model\\raft\\raft.x", pos, rot, 1.f);
        m_meshRaft->Init();
    }
    {
        m_meshRaftCollision = NEW Mesh("res\\model\\raft\\raftCollision.x", pos, rot, 1.f);
        m_meshRaftCollision->Init();
    }
    {
        AnimSetMap animSetMap;
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 0.0f;
            animSetting.m_duration = 0.47f;
            animSetting.m_loop = true;
            animSetting.m_stopEnd = true;
            animSetMap["SailOn"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 0.5f;
            animSetting.m_duration = 0.47f;
            animSetting.m_loop = true;
            animSetting.m_stopEnd = true;
            animSetMap["SailOff"] = animSetting;
        }
        m_meshSail = NEW AnimMesh("res\\model\\raft\\sail.x", pos, rot, 1.f, animSetMap);
        if (raft.GetSail())
        {
            m_meshSail->SetAnim("SailOn");
        }
        else
        {
            m_meshSail->SetAnim("SailOff");
        }
    }
    {
        AnimSetMap animSetMap;
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 0.0f;
            animSetting.m_duration = 0.49f;
            animSetting.m_loop = true;
            animSetMap["Idle"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 0.5f;
            animSetting.m_duration = 0.49f;
            animSetting.m_loop = false;
            animSetMap["Pull"] = animSetting;
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
            animSetting.m_loop = true;
            animSetMap["Idle"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 0.5f;
            animSetting.m_duration = 0.49f;
            animSetting.m_loop = false;
            animSetMap["Pull"] = animSetting;
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
        D3DXVECTOR3 centerPos(0.584f, 1.554f, 1.140f);
        m_meshCord->SetCenterPos(centerPos);
    }
}

void Raft2::Finalize()
{
    SAFE_DELETE(m_meshCord);
    SAFE_DELETE(m_meshOarRight);
    SAFE_DELETE(m_meshOarLeft);
    SAFE_DELETE(m_meshSail);
    SAFE_DELETE(m_meshRaftCollision);
    SAFE_DELETE(m_meshRaft);
}

void Raft2::Update()
{
    D3DXVECTOR3 _move = m_move;

    _move *= 0.97f;
    m_moveRot *= 0.97f;

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

    static bool bothClick = false;

    m_coolTime++;
    if (m_coolTime > 60)
    {
        //----------------------------------------------------
        // マウス・キーボード操作
        //----------------------------------------------------
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
    }

    auto id = Voyage()->GetRaftCurrentId();

    if (pendingLeft)
    {
        ++counterLeft;
        if (counterLeft >= 10)
        {
            if (bothClick)
            {
                Voyage()->PullBothOar();
                PullOarBoth(&_move);
            }
            else
            {
                Voyage()->PullLeftOar();
                PullOarLeft();
            }

            bothClick = false;

            pendingLeft = false;
            counterLeft = 0;

            pendingRight = false;
            counterRight = 0;

            m_coolTime = 0;
        }
    }

    if (pendingRight)
    {
        ++counterRight;
        if (counterRight >= 10)
        {
            if (bothClick)
            {
                Voyage()->PullBothOar();
                PullOarBoth(&_move);
            }
            else
            {
                Voyage()->PullRightOar();
                PullOarRight();
            }

            bothClick = false;

            pendingLeft = false;
            counterLeft = 0;

            pendingRight = false;
            counterRight = 0;

            m_coolTime = 0;
        }
    }

    // 風の強さと方向により流される
    {
        float x, z;
        Voyage()->GetWindXZ(&x, &z);
        x /= 10;
        z /= 10;

        if (Voyage()->GetSailCurrentRaft())
        {
            _move.x += (_move.x - x) / (60 * 20);
            _move.z += (_move.z - z) / (60 * 20);
        }
        else
        {
            _move.x += (_move.x - x) / (60 * 60);
            _move.z += (_move.z - z) / (60 * 60);
        }
    }

    // 潮の強さと方向により流される
    {
        float x, z;
        Voyage()->GetTideXZ(&x, &z);
        x /= 10;
        z /= 10;

        _move.x += (_move.x - x) / (60 * 20);
        _move.z += (_move.z - z) / (60 * 20);
    }
    
    // 重力
    _move.y += -0.01f;

    // 衝突判定

    // 海と接触していたらそれ以上、落下させない
    D3DXVECTOR3 _pos = m_pos;

    // 少し浮いていてほしいので20センチ下の部分を接地判定に使用する
    _pos.y += -0.2f;
    bool isHit1 = SharedObj::GetMap()->IntersectWater(_pos, _move);

    if (isHit1)
    {
        _move.y = 0.f;
    }

    // 陸地と接触していたら停止
    bool isHit2 = SharedObj::GetMap()->CollisionGround(m_pos, _move);

    if (!isHit2)
    {
        m_move = _move;
        m_pos += m_move;
        FLOAT speed = D3DXVec3Length(&_move);
        speed += 1.f;
    }
    // 激突
    else
    {
        // スピードが出ていたか
        FLOAT speed = D3DXVec3Length(&m_move);
        if (speed >= 0.1f)
        {
            Voyage()->CollideGround();
            SoundEffect::get_ton()->play("res\\sound\\collideRaft.wav");
        }
        m_move = D3DXVECTOR3(0.f, 0.f, 0.f);
    }

    m_rotate += m_moveRot;

    // 主人公はイカダに追従させる
    auto ppos = m_pos;
    ppos.y += 0.2f;

    auto workRotate = m_rotate;
    workRotate.y += D3DX_PI;

    SharedObj::GetPlayer()->SetPos(ppos);
    SharedObj::GetPlayer()->SetRotate(workRotate);

    // イカダで川を進むことも出来ることに注意
}

void Raft2::Draw()
{
    m_meshRaft->SetPos(m_pos);
    m_meshRaft->SetRotY(m_rotate.y);
    m_meshRaft->Render();

    m_meshRaftCollision->SetPos(m_pos);
    m_meshRaftCollision->SetRotY(m_rotate.y);
    // 描画する必要がある。
    // 描画しないと衝突判定が働かない。
    m_meshRaftCollision->Render();

    m_meshSail->SetPos(m_pos);
    m_meshSail->SetRotate(m_rotate);
    m_meshSail->Render();

    m_meshOarLeft->SetPos(m_pos);
    m_meshOarLeft->SetRotate(m_rotate);
    m_meshOarLeft->Render();

    m_meshOarRight->SetPos(m_pos);
    m_meshOarRight->SetRotate(m_rotate);
    m_meshOarRight->Render();

    m_meshCord->SetPos(m_pos);
    m_meshCord->SetRotate(m_rotate);

    // 風でハンカチがたなびく
    {
        float x, z;
        Voyage()->GetWindXZ(&x, &z);
        D3DXVECTOR3 tempRot(0.f, 0.f, 0.f);
        tempRot.y = std::atan2(x, z);
        tempRot.y -= m_rotate.y;
        tempRot.y += D3DX_PI;
        m_meshCord->SetRotateLocal(tempRot);

        // 風速によりアニメーション速度を変える
        // 風速4m/sで1倍速、8m/sで2倍速、2m/sで0.5倍速
        float windSpeed = std::sqrt(x*x + z*z);
        float animSpeed = windSpeed/4;
        animSpeed /= 60.f; // 60fps
        m_meshCord->SetAnimSpeed(animSpeed);
    }

    m_meshCord->Render();
}

void Raft2::SetSail(const bool arg)
{
    if (arg)
    {
        m_meshSail->SetAnim("SailOn");
    }
    else
    {
        m_meshSail->SetAnim("SailOff");
    }
}

void Raft2::PullOarBoth(D3DXVECTOR3* _move)
{
    m_meshOarLeft->SetAnim("Pull");
    m_meshOarRight->SetAnim("Pull");

    _move->x += std::sin(m_rotate.y) * -0.3f;
    _move->z += std::cos(m_rotate.y) * -0.3f;

    SoundEffect::get_ton()->play("res\\sound\\pullOar.wav", 80);
    SoundEffect::get_ton()->play("res\\sound\\pullOar2.wav", 80);
}

void Raft2::PullOarLeft()
{
    m_meshOarLeft->SetAnim("Pull");
    m_moveRot.y += -0.01f;

    SoundEffect::get_ton()->play("res\\sound\\pullOar.wav", 80);
    SoundEffect::get_ton()->play("res\\sound\\pullOar2.wav", 80);
}

void Raft2::PullOarRight()
{
    m_meshOarRight->SetAnim("Pull");
    m_moveRot.y += 0.01f;

    SoundEffect::get_ton()->play("res\\sound\\pullOar.wav", 80);
    SoundEffect::get_ton()->play("res\\sound\\pullOar2.wav", 80);
}

void Raft2::Pull3Hours()
{
    // TODO 適切な値に
    m_pos.x += std::sin(m_rotate.y) * -10000.f;
    m_pos.z += std::cos(m_rotate.y) * -10000.f;
}

D3DXVECTOR3 Raft2::GetPos() const
{
    return m_pos;
}

void Raft2::SetPos(const D3DXVECTOR3& pos)
{
    m_pos = pos;
}

D3DXVECTOR3 Raft2::GetRotate() const
{
    return m_rotate;
}

void Raft2::SetRotate(const D3DXVECTOR3& rot)
{
    m_rotate = rot;
}

Mesh* Raft2::GetCollisionMesh()
{
    return m_meshRaftCollision;
}

int Raft2::GetId()
{
    return m_id;
}

VoyageManager::VoyageManager()
{

}

