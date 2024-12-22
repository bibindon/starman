#include "Player.h"
#include "SoundEffect.h"
#include "Camera.h"
#include "SharedObj.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/StatusManager.h"
#include "KeyBoard.h"
#include "Mouse.h"
#include "GamePad.h"

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
        animSetting.m_duration = 1.f;
        animSetting.m_loop = false;
        animSetMap["Dead"] = animSetting;
    }
    {
        AnimSetting animSetting { };
        animSetting.m_startPos = 5.f;
        animSetting.m_duration = 2.f;
        animSetting.m_loop = false;
        animSetMap["Jump"] = animSetting;
    }
    m_AnimMesh2 = new AnimMesh("res\\model\\hoshiman\\hoshiman.x", b, rot, 1.f, animSetMap);
    m_AnimMesh2->SetAnim("Idle");
    SoundEffect::get_ton()->load("res\\sound\\attack01.wav");

    m_spriteHP = new Sprite("res\\image\\hp_green_p.png");
    m_spriteHPBack = new Sprite("res\\image\\hp_black_p.png");

    {
        D3DXVECTOR3 b = D3DXVECTOR3(0.f, 0.f, 0.f);
        D3DXVECTOR3 c = D3DXVECTOR3(0.f, 0.f, 0.f);
        Mesh * mesh = new Mesh("res\\model\\rock1\\rock1.x", b, c, 0.1f);
        mesh->Init();
        mesh->SetWeapon(true);
        m_weaponMesh["石"] = mesh;
    }
    {
        D3DXVECTOR3 b = D3DXVECTOR3(0.f, 0.f, 0.f);
        D3DXVECTOR3 c = D3DXVECTOR3(0.f, 0.f, D3DX_PI/2);
        Mesh * mesh = new Mesh("res\\model\\stick\\stick.x", b, c, 1.f);
        mesh->Init();
        mesh->SetWeapon(true);
        m_weaponMesh["木の棒"] = mesh;
    }
    {
        D3DXVECTOR3 b = D3DXVECTOR3(0.f, 0.f, 0.f);
        D3DXVECTOR3 c = D3DXVECTOR3(D3DX_PI*3/2, 0.f, D3DX_PI/2);
        Mesh * mesh = new Mesh("res\\model\\ax\\ax.x", b, c, 1.0f);
        mesh->Init();
        mesh->SetWeapon(true);
        m_weaponMesh["石斧"] = mesh;
    }
    {
        D3DXVECTOR3 b = D3DXVECTOR3(0.f, 0.f, 0.f);
        D3DXVECTOR3 c = D3DXVECTOR3(0.f, 0.f, D3DX_PI/2);
        Mesh * mesh = new Mesh("res\\model\\atlatl\\atlatl.x", b, c, 1.0f);
        mesh->Init();
        mesh->SetWeapon(true);
        m_weaponMesh["アトラトル"] = mesh;
    }
    {
        D3DXVECTOR3 b = D3DXVECTOR3(0.f, 0.f, 0.f);
        D3DXVECTOR3 c = D3DXVECTOR3(0.f, 0.f, D3DX_PI/2);
        Mesh * mesh = new Mesh("res\\model\\ironPipe\\ironPipe.x", b, c, 1.0f);
        mesh->Init();
        mesh->SetWeapon(true);
        m_weaponMesh["鉄パイプ"] = mesh;
    }
    {
        D3DXVECTOR3 b = D3DXVECTOR3(0.f, 0.f, 0.f);
        D3DXVECTOR3 c = D3DXVECTOR3(D3DX_PI/2, 0.f, D3DX_PI/2);
        Mesh * mesh = new Mesh("res\\model\\ironPipeEx\\ironPipeEx.x", b, c, 1.0f);
        mesh->Init();
        mesh->SetWeapon(true);
        m_weaponMesh["石付き鉄パイプ"] = mesh;
    }
    {
        D3DXVECTOR3 b = D3DXVECTOR3(0.f, 0.f, 0.f);
        D3DXVECTOR3 c = D3DXVECTOR3(D3DX_PI/2, 0.f, D3DX_PI/2);
        Mesh * mesh = new Mesh("res\\model\\spear\\spear.x", b, c, 1.0f);
        mesh->Init();
        mesh->SetWeapon(true);
        m_weaponMesh["石槍"] = mesh;
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
    SAFE_DELETE(m_spriteHP);
    SAFE_DELETE(m_spriteHPBack);
    SAFE_DELETE(m_AnimMesh2);
    for (auto it = m_weaponMesh.begin(); it != m_weaponMesh.end(); ++it)
    {
        SAFE_DELETE(it->second);
    }
}

void Player::Update(Map* map)
{
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
            // デバッグ目的でGキーだけ移動速度アップ
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
        SetJump();
    }

    if (Common::DebugMode())
    {
        if (KeyBoard::IsDownFirstFrame(DIK_J))
        {
            m_bJump = true;
            m_move.y = JUMP_INITIAL_VELOCITY*5;
            m_AnimMesh2->SetAnim("Jump", 0.f);
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
    move /= 10.f;
    m_move += move;


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

    // 重力
    m_move.y += -0.01f;

    // 壁ずり
    // 高さを変えて3回チェック
    bool bHit { false };
    D3DXVECTOR3 tmp = m_loadingPos;
    m_move = map->WallSlide(m_loadingPos, m_move, &bHit);

    // 足が何かに触れたらジャンプを解除する。
    // 壁ジャンプできてしまうが、ひとまずよしとする。
    // 何かに触れている状態が1秒続いたら再度ジャンプできる、でもよいかもしれない。
    if (bHit)
    {
        m_attachCount++;

        // 0.1秒?張り付き状態を維持したら再度ジャンプ可能になる
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

//    if (bHit == false)
//    {
//        D3DXVECTOR3 tempPos { m_loadingPos };
//        tempPos.y += 1.f;
//        m_move = map->WallSlide(tempPos, m_move, &bHit);
//        if (bHit == false)
//        {
//            D3DXVECTOR3 tempPos { m_loadingPos };
//            tempPos.y += 2.f;
//            m_move = map->WallSlide(tempPos, m_move, &bHit);
//        }
//    }
//
//    // 接地判定
//    {
//        D3DXVECTOR3 temp { m_move };
//
//        temp.y += -0.1f;
//        bool isHit = map->CollisionGround(m_loadingPos, temp);
//        if (isHit == false)
//        {
//           // m_move.y += -0.01f;
//        }
//    }

    m_loadingPos += m_move;

    m_move.x = 0.f;
    // m_move.y = 0.f;
    m_move.z = 0.f;
}

void Player::Render()
{
    m_AnimMesh2->SetPos(m_loadingPos);
    m_AnimMesh2->SetRotate(m_rotate);
    m_AnimMesh2->Render();

    POINT screenPos = Camera::GetScreenPos(m_loadingPos);
    m_spriteHPBack->Render(
        D3DXVECTOR3 { (FLOAT)screenPos.x - 128, (FLOAT)screenPos.y, 0.f });
    if (m_HP >= 0)
    {
        m_spriteHP->Render(
            D3DXVECTOR3 { (FLOAT)screenPos.x - 128, (FLOAT)screenPos.y, 0.f },
            255,
            (m_HP*256/100));
    }

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
    m_move = move;
}

D3DXVECTOR3 Player::GetMove() const
{
    return m_move;
}

void Player::SetRotate(const D3DXVECTOR3& rotate)
{
    m_rotate = rotate;
}

D3DXVECTOR3 Player::GetRotate() const
{
    return m_rotate;
}

bool Player::SetAttack()
{
    if (m_bAttack || m_bDamaged)
    {
        return false;
    }

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

            auto statusManager = NSStarmanLib::StatusManager::GetObj();
            float attackPower = statusManager->GetAttackPower();
            vecEnemy.at(i)->SetHP(hp - (int)attackPower);
            statusManager->ConsumeAttackCost();
        }
    }

    SharedObj::GetMap()->SetEnemy(vecEnemy);
    return true;
}

void Player::SetWalk()
{
    if (m_bJump == false)
    {
        m_AnimMesh2->SetAnim("Walk");
    }
}

void Player::SetDamaged()
{
    //if (!m_bDamaged)
    {
        SoundEffect::get_ton()->play("res\\sound\\damage01.wav", 90);
        m_AnimMesh2->SetAnim("Damaged", 0.f);
        m_bDamaged = true;
    }
}

void Player::SetDead()
{
    if (m_bDead == false)
    {
        m_bDead = true;
        m_AnimMesh2->SetAnim("Dead", 0.f);
    }
}

bool Player::GetDead() const
{
    return m_bDead;
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
    if (m_bJumpEnable)
    {
        m_bJump = true;
        m_bJumpEnable = false;

        m_move.y = JUMP_INITIAL_VELOCITY;
        m_AnimMesh2->SetAnim("Jump", 0.f);
    }
}

void Player::SetExamine()
{
    // 
}

void Player::SetHP(const int hp)
{
    m_HP = hp;
}

int Player::GetHP() const
{
    return m_HP;
}

