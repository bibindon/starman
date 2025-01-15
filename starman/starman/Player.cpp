#include "Player.h"
#include "SoundEffect.h"
#include "Camera.h"
#include "SharedObj.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/StatusManager.h"
#include "KeyBoard.h"
#include "Mouse.h"
#include "GamePad.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/MapObjManager.h"

Player::Player()
{
    D3DXVECTOR3 b = D3DXVECTOR3(0, 0, 0);
    D3DXVECTOR3 rot = D3DXVECTOR3(0, D3DX_PI, 0);
    AnimSetMap animSetMap;
    {
        AnimSetting animSetting { };
        animSetting.m_startPos = 0.f;
        animSetting.m_duration = 0.5f;
        animSetting.m_loop = true;
        animSetMap["Idle"] = animSetting;
    }
    {
        AnimSetting animSetting { };
        animSetting.m_startPos = 1.f;
        animSetting.m_duration = 1.f;
        animSetting.m_loop = false;
        animSetMap["Walk"] = animSetting;
    }
    {
        AnimSetting animSetting { };
        animSetting.m_startPos = 2.f;
        animSetting.m_duration = 1.f;
        animSetting.m_loop = false;
        animSetMap["Attack"] = animSetting;
    }
    {
        AnimSetting animSetting { };
        animSetting.m_startPos = 3.f;
        animSetting.m_duration = 0.5f;
        animSetting.m_loop = false;
        animSetMap["Damaged"] = animSetting;
    }
    {
        AnimSetting animSetting { };
        animSetting.m_startPos = 3.5f;
        animSetting.m_duration = 0.45f;
        animSetting.m_loop = false;
        animSetting.m_stopEnd = true;
        animSetMap["Dead"] = animSetting;
    }
    {
        AnimSetting animSetting { };
        animSetting.m_startPos = 5.f;
        animSetting.m_duration = 2.f;
        animSetting.m_loop = false;
        animSetMap["Jump"] = animSetting;
    }
    {
        AnimSetting animSetting { };
        animSetting.m_startPos = 7.1f;
        animSetting.m_duration = 0.5f;
        animSetting.m_loop = true;
        animSetMap["Sit"] = animSetting;
    }
    {
        AnimSetting animSetting { };
        animSetting.m_startPos = 7.7f;
        animSetting.m_duration = 0.6f;
        animSetting.m_loop = true;
        animSetting.m_stopEnd = true;
        animSetMap["LieDown"] = animSetting;
    }
    {
        AnimSetting animSetting { };
        animSetting.m_startPos = 8.0f;
        animSetting.m_duration = 0.97f;
        animSetting.m_loop = true;
        animSetMap["IdleWater"] = animSetting;
    }
    {
        AnimSetting animSetting { };
        animSetting.m_startPos = 9.0f;
        animSetting.m_duration = 0.97f;
        animSetting.m_loop = true;
        animSetMap["Swim"] = animSetting;
    }
    m_AnimMesh2 = NEW AnimMesh("res\\model\\hoshiman\\hoshiman.x", b, rot, 1.f, animSetMap);
    m_AnimMesh2->SetAnim("Idle");
    SoundEffect::get_ton()->load("res\\sound\\attack01.wav");

    {
        D3DXVECTOR3 b = D3DXVECTOR3(0.f, 0.f, 0.f);
        D3DXVECTOR3 c = D3DXVECTOR3(0.f, 0.f, 0.f);
        Mesh * mesh = NEW Mesh("res\\model\\rock1\\rock1.x", b, c, 0.1f);
        mesh->Init();
        mesh->SetWeapon(true);
        m_weaponMesh["��"] = mesh;
    }
    {
        D3DXVECTOR3 b = D3DXVECTOR3(0.f, 0.f, 0.f);
        D3DXVECTOR3 c = D3DXVECTOR3(0.f, 0.f, D3DX_PI);
        Mesh * mesh = NEW Mesh("res\\model\\stick\\stick.x", b, c, 1.f);
        mesh->Init();
        mesh->SetWeapon(true);
        m_weaponMesh["�؂̖_"] = mesh;
    }
    {
        D3DXVECTOR3 b = D3DXVECTOR3(0.f, 0.f, 0.f);
        D3DXVECTOR3 c = D3DXVECTOR3(0.f, 0.f, D3DX_PI);
        Mesh * mesh = NEW Mesh("res\\model\\ax\\ax.x", b, c, 1.0f);
        mesh->Init();
        mesh->SetWeapon(true);
        m_weaponMesh["�Ε�"] = mesh;
    }
    {
        D3DXVECTOR3 b = D3DXVECTOR3(0.f, 0.f, 0.f);
        D3DXVECTOR3 c = D3DXVECTOR3(0.f, 0.f, D3DX_PI);
        Mesh * mesh = NEW Mesh("res\\model\\atlatl\\atlatl.x", b, c, 1.0f);
        mesh->Init();
        mesh->SetWeapon(true);
        m_weaponMesh["�A�g���g��"] = mesh;
    }
    {
        D3DXVECTOR3 b = D3DXVECTOR3(0.f, 0.f, 0.f);
        D3DXVECTOR3 c = D3DXVECTOR3(0.f, 0.f, D3DX_PI);
        Mesh * mesh = NEW Mesh("res\\model\\ironPipe\\ironPipe.x", b, c, 1.0f);
        mesh->Init();
        mesh->SetWeapon(true);
        m_weaponMesh["�S�p�C�v"] = mesh;
    }
    {
        D3DXVECTOR3 b = D3DXVECTOR3(0.f, 0.f, 0.f);
        D3DXVECTOR3 c = D3DXVECTOR3(0.f, 0.f, D3DX_PI);
        Mesh * mesh = NEW Mesh("res\\model\\ironPipeEx\\ironPipeEx.x", b, c, 1.0f);
        mesh->Init();
        mesh->SetWeapon(true);
        m_weaponMesh["�Εt���S�p�C�v"] = mesh;
    }
    {
        D3DXVECTOR3 b = D3DXVECTOR3(0.f, 0.f, 0.f);
        D3DXVECTOR3 c = D3DXVECTOR3(0.f, 0.f, D3DX_PI);
        Mesh * mesh = NEW Mesh("res\\model\\spear\\spear.x", b, c, 1.0f);
        mesh->Init();
        mesh->SetWeapon(true);
        m_weaponMesh["�Α�"] = mesh;
    }

    float x = 0.f;
    float y = 0.f;
    float z = 0.f;
    NSStarmanLib::StatusManager::GetObj()->GetXYZ(&x, &y, &z);
    m_loadingPos.x = x;
    m_loadingPos.y = y + 1.f;
    m_loadingPos.z = z;
}

Player::~Player()
{
    SAFE_DELETE(m_AnimMesh2);
    for (auto it = m_weaponMesh.begin(); it != m_weaponMesh.end(); ++it)
    {
        SAFE_DELETE(it->second);
    }
}

void Player::Update(Map* map)
{
    // �C�Ɛڂ��Ă��邩
    {
        D3DXVECTOR3 down(0.f, -1.f, 0.f);
        m_bUnderwater = map->IntersectWater(m_loadingPos, down);
        if (m_bUnderwater)
        {
            m_AnimMesh2->SetAnim("IdleWater");
        }
    }

    //============================================================
    // Key Mouse Pad
    //============================================================

    //------------------------------------------------------------
    // Keyboard
    //------------------------------------------------------------

    D3DXVECTOR3 move { 0.f, 0.f, 0.f };
    D3DXVECTOR3 rotate { 0.f, 0.f, 0.f };
    float radian = Camera::GetRadian();
    float yaw = -1.f * (radian - (D3DX_PI / 2));

    if (KeyBoard::IsDown(DIK_W))
    {
        move.x += -std::sin(radian + D3DX_PI / 2);
        move.z += std::sin(radian + D3DX_PI);

        D3DXVECTOR3 rotate { 0.f, yaw, 0.f };
        SetRotate(rotate);
        SetWalk();
    }

    if (Common::DebugMode())
    {
        if (KeyBoard::IsDown(DIK_G))
        {
            // �f�o�b�O�ړI��G�L�[�����ړ����x�A�b�v
            move.x += -std::sin(radian + (D3DX_PI / 2)) * 50;
            move.z += std::sin(radian + D3DX_PI) * 50;

            D3DXVECTOR3 rotate{ 0.f, yaw, 0.f };
            SetRotate(rotate);
            SetWalk();

        }
    }

    if (KeyBoard::IsDown(DIK_A))
    {
        move.x += -std::sin(radian + D3DX_PI);
        move.z += std::sin(radian + D3DX_PI * 3 / 2);

        D3DXVECTOR3 rotate { 0.f, yaw + D3DX_PI * 3 / 2, 0.f };
        SetRotate(rotate);
        SetWalk();
    }

    if (KeyBoard::IsDown(DIK_S))
    {
        move.x += -std::sin(radian + D3DX_PI * 3 / 2);
        move.z += std::sin(radian);

        D3DXVECTOR3 rotate { 0.f, yaw + D3DX_PI, 0.f };
        SetRotate(rotate);
        SetWalk();
    }

    if (KeyBoard::IsDown(DIK_D))
    {
        move.x += -std::sin(radian);
        move.z += std::sin(radian + D3DX_PI / 2);

        D3DXVECTOR3 rotate { 0.f, yaw + D3DX_PI / 2, 0.f };
        SetRotate(rotate);
        SetWalk();
    }

    if (KeyBoard::IsDownFirstFrame(DIK_SPACE))
    {
        if (m_bUnderwater == false)
        {
            // ���A�E�A��낪���͂���Ă���Ƃ��ɃW�����v���悤�Ƃ�����X�e�b�v�ړ�
            if (KeyBoard::IsDown(DIK_A))
            {
                SetStep(eDir::LEFT);
            }
            else if (KeyBoard::IsDown(DIK_S))
            {
                SetStep(eDir::BACK);
            }
            else if (KeyBoard::IsDown(DIK_D))
            {
                SetStep(eDir::RIGHT);
            }
            else
            {
                SetJump();
            }
        }
    }

    if (Common::DebugMode())
    {
        // �X�[�p�[�W�����v
        if (KeyBoard::IsDownFirstFrame(DIK_J))
        {
            m_bJump = true;
            m_move.y = JUMP_INITIAL_VELOCITY*5;
            m_AnimMesh2->SetAnim("Jump", 0.f);
        }

        // �؂�����
        if (KeyBoard::IsDownFirstFrame(DIK_Z))
        {
            auto mapObjManager = NSStarmanLib::MapObjManager::GetObj();
            std::vector<NSStarmanLib::stMapObj> mapObjs =
                mapObjManager->GetMapObjListR(m_loadingPos.x, m_loadingPos.z, 20);

            for (size_t i = 0; i < mapObjs.size(); ++i)
            {
                mapObjManager->SetVisible(mapObjs.at(i).m_frameX,
                                          mapObjs.at(i).m_frameZ,
                                          mapObjs.at(i).m_id,
                                          false);
            }
        }

        // �G������
        if (KeyBoard::IsDownFirstFrame(DIK_X))
        {
            std::vector<EnemyBase*> vecEnemy = SharedObj::GetMap()->GetEnemy();

            for (std::size_t i = 0; i < vecEnemy.size(); i++)
            {
                D3DXVECTOR3 enemyPos(0.f, 0.f, 0.f);
                enemyPos = vecEnemy.at(i)->GetPos();

                D3DXVECTOR3 subPos = m_loadingPos - enemyPos;
                FLOAT distance = D3DXVec3Length(&subPos);

                if (distance <= 20.f)
                {
                    vecEnemy.at(i)->SetHP(0);
                }
            }

            SharedObj::GetMap()->SetEnemy(vecEnemy);
        }
    }

    //------------------------------------------------------------
    // Mouse
    //------------------------------------------------------------

    if (Mouse::IsDownLeft())
    {
        bool ret = SetAttack();
    }

    //------------------------------------------------------------
    // GamePad
    //------------------------------------------------------------

    if (GamePad::IsLeftStickUsed())
    {
        float joyRadian = GamePad::GetLeftRadian();
        float cameRadian = Camera::GetRadian();
        float radian = joyRadian + (cameRadian - D3DX_PI * 3 / 2);
        move.x += std::cos(radian);
        move.z += std::sin(radian);

        yaw = -1.f * (radian + D3DX_PI / 2);
        D3DXVECTOR3 rotate { 0.f, yaw, 0.f };
        SetRotate(rotate);
        SetWalk();
    }

    if (GamePad::IsDown(eGamePadButtonType::R1))
    {
        bool ret = SetAttack();
    }

    if (GamePad::IsDown(eGamePadButtonType::B))
    {
        SetJump();
    }

    //----------------------------------------------------------
    // Finalize
    //----------------------------------------------------------

    // �W�����v���͈ړ�������ς��������������ł��Ȃ�
    auto statusManager = NSStarmanLib::StatusManager::GetObj();
    if (m_bJump || statusManager->GetDead())
    {
//        if (Common::DebugMode() == false)
        {
            move = D3DXVECTOR3(0.f, 0.f, 0.f);
        }
    }

    m_move += move;

    float MAX_XZ_MOVE = 0.f;

    // 1�t���[����50�Z���`�ȏ�ړ����悤�Ƃ�����50�Z���`�ɂ���
    // �������AXZ���ʂ̂݁BY�������͉������Ȃ��B
    if (Common::DebugMode())
    {
        if (m_bUnderwater == false)
        {
            MAX_XZ_MOVE = 5.0f;
        }
        else
        {
            MAX_XZ_MOVE = 1.0f;
        }
    }
    else
    {
        if (m_bUnderwater == false)
        {
            MAX_XZ_MOVE = 0.5f;
        }
        else
        {
            MAX_XZ_MOVE = 0.1f;
        }
    }

    D3DXVECTOR2 move_XZ(m_move.x, m_move.z);
    FLOAT speed = D3DXVec2Length(&move_XZ);

    MAX_XZ_MOVE *= statusManager->GetWalkSpeed();

    // ����100�Z���`�ړ����悤�Ƃ��Ă�����2�Ŋ���΂悢�B
    if (speed >= MAX_XZ_MOVE)
    {
        float work = MAX_XZ_MOVE / speed;
        move_XZ *= work;
        m_move.x = move_XZ.x;
        m_move.z = move_XZ.y;
    }

    // XZ���ʏ�̈ړ��ʂ͖��t���[�������ɂ���B�W�����v���Ă���Ƃ��͔����ɂ��Ȃ��B
    if (m_bJump == false)
    {
        m_move.x *= 0.5f;
        m_move.z *= 0.5f;
    }

    if (map == nullptr)
    {
        return;
    }
    if (m_bAttack)
    {
        m_attackTimeCounter++;
    }
    if (m_attackTimeCounter >= 30)
    {
        m_attackTimeCounter = 0;
        m_bAttack = false;
    }
    if (m_bDamaged)
    {
        m_damagedTimeCounter++;
    }
    if (m_damagedTimeCounter >= 30)
    {
        m_damagedTimeCounter = 0;
        m_bDamaged = false;
    }

    // �d��
    m_move.y += -0.01f;

    // �ǂ���
    // �ǂ���͑��ɑ΂��Ă����s��
    bool bHit { false };
    m_move = map->WallSlide(m_loadingPos, m_move, &bHit);

    // ���ȊO�̏Փ˔���
    // ���Ɠ�
    D3DXVECTOR3 tmp = m_loadingPos;
    tmp.y += 0.8f;
    bool bHit2 = map->Intersect(tmp, m_move);
    if (bHit2)
    {
        m_move = D3DXVECTOR3(0.f, 0.f, 0.f);
    }

    tmp.y += 0.8f;
    bool bHit3 = map->Intersect(tmp, m_move);
    if (bHit3)
    {
        m_move = D3DXVECTOR3(0.f, 0.f, 0.f);
    }

    // ���������ɐG�ꂽ��W�����v����������B
    // �ǃW�����v�ł��Ă��܂����A�ЂƂ܂��悵�Ƃ���B
    // �����ɐG��Ă����Ԃ�1�b��������ēx�W�����v�ł���A�ł��悢��������Ȃ��B
    if (bHit)
    {
        m_attachCount++;

        // 0.1�b?����t����Ԃ��ێ�������ēx�W�����v�\�ɂȂ�
        if (m_attachCount >= 6)
        {
            m_bJumpEnable = true;
            m_bJump = false;
        }
    }
    else
    {
        m_attachCount = 0;
    }

    m_loadingPos += m_move;

    // �ً}��i
    // ������n�ʂ܂ł̋�����1.5���[�g���ȉ���������n�ʂɂ߂荞��ł���H
    D3DXVECTOR3 tmp2 = m_loadingPos;
    tmp2.y += 1.6f;
    bool bHit4 = map->Intersect(tmp2, D3DXVECTOR3(0.f, -1.5f, 0.f));
    if (bHit4)
    {
        m_loadingPos.y = tmp2.y;
    }

    if (statusManager->GetDead())
    {
        SetDead();
    }
}

void Player::Render()
{
    m_AnimMesh2->SetPos(m_loadingPos);
    m_AnimMesh2->SetRotate(m_rotate);
    m_AnimMesh2->Render();

    NSStarmanLib::StatusManager* statusManager = NSStarmanLib::StatusManager::GetObj();
    NSStarmanLib::ItemInfo itemInfo = statusManager->GetEquipWeapon();

    if (itemInfo.GetId() != -1)
    {
        NSStarmanLib::ItemManager* itemManager = NSStarmanLib::ItemManager::GetObj();
        NSStarmanLib::ItemDef itemDef = itemManager->GetItemDef(itemInfo.GetId());
        m_weaponMesh.at(itemDef.GetName())->Render();
    }
}

void Player::SetPos(const D3DXVECTOR3& pos)
{
    m_loadingPos = pos;
}

D3DXVECTOR3 Player::GetPos() const
{
    return m_loadingPos;
}

void Player::SetMove(const D3DXVECTOR3& move)
{
    if (m_bJumpEnable && NSStarmanLib::StatusManager::GetObj()->GetDead() == false)
    {
        m_move = move;
    }
}

D3DXVECTOR3 Player::GetMove() const
{
    return m_move;
}

void Player::SetRotate(const D3DXVECTOR3& rotate)
{
    if (m_bJump == false && NSStarmanLib::StatusManager::GetObj()->GetDead() == false)
    {
        m_rotate = rotate;
    }
}

D3DXVECTOR3 Player::GetRotate() const
{
    return m_rotate;
}

bool Player::SetAttack()
{
    if (m_bAttack || m_bDamaged || NSStarmanLib::StatusManager::GetObj()->GetDead())
    {
        return false;
    }

    auto statusManager = NSStarmanLib::StatusManager::GetObj();
    statusManager->ConsumeAttackCost();

    SoundEffect::get_ton()->play("res\\sound\\attack01.wav", 90);
    m_AnimMesh2->SetAnim("Attack", 0.f);
    m_bAttack = true;

    D3DXVECTOR3 attackPos = GetAttackPos();
    D3DXVECTOR3 enemyPos { 0.f, 0.f, 0.f };
    std::vector<EnemyBase*> vecEnemy = SharedObj::GetMap()->GetEnemy();

    for (std::size_t i = 0; i < vecEnemy.size(); i++)
    {
        D3DXVECTOR3 enemyPos { 0.f, 0.f, 0.f };
        enemyPos = vecEnemy.at(i)->GetPos();

        D3DXVECTOR3 subPos { attackPos - enemyPos };
        FLOAT distance = D3DXVec3Length(&subPos);

        if (distance <= 1.5f)
        {
            vecEnemy.at(i)->SetState(eEnemyState::DAMAGED);
            int hp = vecEnemy.at(i)->GetHP();

            float attackPower = statusManager->GetAttackPower();
            vecEnemy.at(i)->SetHP(hp - (int)attackPower);
        }
    }

    SharedObj::GetMap()->SetEnemy(vecEnemy);

    return true;
}

void Player::SetWalk()
{
    if (m_bJump == false &&
        NSStarmanLib::StatusManager::GetObj()->GetDead() == false &&
        m_bAttack == false)
    {
        if (m_bUnderwater == false)
        {
            m_AnimMesh2->SetAnim("Walk");
        }
        else
        {
            m_AnimMesh2->SetAnim("Swim");
        }
    }
}

void Player::SetDamaged()
{
    if (NSStarmanLib::StatusManager::GetObj()->GetDead() == false)
    {
        SoundEffect::get_ton()->play("res\\sound\\damage01.wav", 90);
        m_AnimMesh2->SetAnim("Damaged", 0.f);
        m_bDamaged = true;
    }

    // �������ɍU�����ꂽ�瑦��
    if (NSStarmanLib::StatusManager::GetObj()->GetSleep())
    {
        NSStarmanLib::StatusManager::GetObj()->SetDead(true);
    }
}

void Player::SetDead()
{
    m_AnimMesh2->SetAnim("Dead", 0.f);
}

bool Player::GetDead() const
{
    return NSStarmanLib::StatusManager::GetObj()->GetDead();
}

void Player::SetSleep(const bool arg)
{
    if (arg)
    {
        m_AnimMesh2->SetAnim("LieDown", 0.f);
        auto status = NSStarmanLib::StatusManager::GetObj();
        status->SetPlayerAction(NSStarmanLib::StatusManager::PlayerState::LYING_DOWN);
        status->SetSleep(true);
    }
    else
    {
        m_AnimMesh2->SetAnim("Idle", 0.f);
        auto status = NSStarmanLib::StatusManager::GetObj();
        status->SetPlayerAction(NSStarmanLib::StatusManager::PlayerState::STAND);
        status->SetSleep(false);
    }
}

D3DXVECTOR3 Player::GetAttackPos() const
{
    D3DXVECTOR3 pos { m_loadingPos };
    D3DXVECTOR3 norm { 0.f, 0.f, 0.f };
    norm.x = std::sin(m_rotate.y + D3DX_PI);
    norm.z = std::sin(m_rotate.y + (D3DX_PI * 3 / 2));
    pos += norm * 2;
    return pos;
}

void Player::SetJump()
{
    if (m_bJumpEnable && NSStarmanLib::StatusManager::GetObj()->GetDead() == false)
    {
        m_bJump = true;
        m_bJumpEnable = false;

        m_move.y = JUMP_INITIAL_VELOCITY;
        m_AnimMesh2->SetAnim("Jump", 0.f);
    }
}

void Player::SetSit()
{
    m_AnimMesh2->SetAnim("Sit", 0.f);

    auto status = NSStarmanLib::StatusManager::GetObj();
    status->SetPlayerAction(NSStarmanLib::StatusManager::PlayerState::SIT);
}

void Player::SetLieDown()
{
    m_AnimMesh2->SetAnim("LieDown", 0.f);

    auto status = NSStarmanLib::StatusManager::GetObj();
    status->SetPlayerAction(NSStarmanLib::StatusManager::PlayerState::LYING_DOWN);
}

void Player::SetStep(const eDir dir)
{
    const float STEP_VELOCITY = 10.f;
    float radian = Camera::GetRadian();
    float yaw = -1.f * (radian - (D3DX_PI / 2));
    D3DXVECTOR3 move(0.f, 0.f, 0.f);
    D3DXVECTOR3 rotate(0.f, 0.f, 0.f);

    if (dir == eDir::LEFT)
    {
        move.x += -std::sin(radian + D3DX_PI);
        move.z += std::sin(radian + D3DX_PI * 3 / 2);
        move *= STEP_VELOCITY;
        rotate = D3DXVECTOR3 { 0.f, yaw + D3DX_PI * 3 / 2, 0.f };
    }
    else if (dir == eDir::BACK)
    {
        move.x += -std::sin(radian + D3DX_PI * 3 / 2);
        move.z += std::sin(radian);
        move *= STEP_VELOCITY;
        rotate = D3DXVECTOR3 { 0.f, yaw + D3DX_PI, 0.f };
    }
    else if (dir == eDir::RIGHT)
    {
        move.x += -std::sin(radian);
        move.z += std::sin(radian + D3DX_PI / 2);
        move *= STEP_VELOCITY;
        rotate = D3DXVECTOR3 { 0.f, yaw + D3DX_PI / 2, 0.f };
    }
    SetMove(move);

    // TODO �J���������������Ȃ�
    SetRotate(-rotate);
}

void Player::SetExamine()
{
    // 
}


