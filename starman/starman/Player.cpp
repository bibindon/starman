#include "Player.h"
#include "SoundEffect.h"
#include "Camera.h"
#include "SharedObj.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/StatusManager.h"
#include "KeyBoard.h"
#include "Mouse.h"
#include "GamePad.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/MapObjManager.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/Inventory.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/WeaponManager.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/Rynen.h"
#include "PopUp2.h"
#include "resource.h"

Player::Player()
{
    D3DXVECTOR3 pos = D3DXVECTOR3(0, 0, 0);
    D3DXVECTOR3 rot = D3DXVECTOR3(0, D3DX_PI, 0);
    AnimSetMap animSetMap;
    {
        AnimSetting animSetting { };
        animSetting.m_startPos = 0.f;
        animSetting.m_duration = 0.47f;
        animSetting.m_loop = true;
        animSetMap[_T("Idle")] = animSetting;
    }
    {
        AnimSetting animSetting { };
        animSetting.m_startPos = 1.f;
        animSetting.m_duration = 0.97f;
        animSetting.m_loop = false;
        animSetMap[_T("Walk")] = animSetting;
    }
    {
        AnimSetting animSetting { };
        animSetting.m_startPos = 2.f;
        animSetting.m_duration = 0.97f;
        animSetting.m_loop = false;
        animSetMap[_T("Attack")] = animSetting;
    }
    {
        AnimSetting animSetting { };
        animSetting.m_startPos = 3.f;
        animSetting.m_duration = 0.47f;
        animSetting.m_loop = false;
        animSetMap[_T("Damaged")] = animSetting;
    }
    {
        AnimSetting animSetting { };
        animSetting.m_startPos = 3.5f;
        animSetting.m_duration = 0.47f;
        animSetting.m_loop = false;
        animSetting.m_stopEnd = true;
        animSetMap[_T("Dead")] = animSetting;
    }
    {
        AnimSetting animSetting { };
        animSetting.m_startPos = 5.f;
        animSetting.m_duration = 1.9f;
        animSetting.m_loop = false;
        animSetMap[_T("Jump")] = animSetting;
    }
    {
        AnimSetting animSetting { };
        animSetting.m_startPos = 7.0f;
        animSetting.m_duration = 0.66f;
        animSetting.m_loop = true;
        animSetMap[_T("Sit")] = animSetting;
    }
    {
        AnimSetting animSetting { };
        animSetting.m_startPos = 7.7f;
        animSetting.m_duration = 0.27f;
        animSetting.m_loop = false;
        animSetting.m_stopEnd = true;
        animSetMap[_T("LieDown")] = animSetting;
    }
    {
        AnimSetting animSetting { };
        animSetting.m_startPos = 8.0f;
        animSetting.m_duration = 0.97f;
        animSetting.m_loop = true;
        animSetMap[_T("IdleWater")] = animSetting;
    }
    {
        AnimSetting animSetting { };
        animSetting.m_startPos = 9.0f;
        animSetting.m_duration = 0.97f;
        animSetting.m_loop = true;
        animSetMap[_T("Swim")] = animSetting;
    }
    {
        AnimSetting animSetting { };
        animSetting.m_startPos = 10.0f;
        animSetting.m_duration = 0.97f;
        animSetting.m_loop = false;
        animSetMap[_T("Magic")] = animSetting;
    }
    {
        AnimSetting animSetting { };
        animSetting.m_startPos = 11.0f;
        animSetting.m_duration = 0.97f;
        animSetting.m_loop = false;
        animSetMap[_T("Arrow")] = animSetting;
    }
    m_AnimMesh2 = NEW AnimMesh(_T("res\\model\\hoshiman.x"), pos, rot, 1.f, animSetMap);
    m_AnimMesh2->SetAnim(_T("Idle"));
    SoundEffect::get_ton()->load(_T("res\\sound\\attack01.wav"));

    // 読み込み処理の前に走ってしまうので以下のような書き方でXファイルの名前を参照できない。
    // 考え物である
//    auto weaponManager = NSStarmanLib::WeaponManager::GetObj();
//    auto weaponNameList = weaponManager->GetWeaponNameList();
//
//    for (auto it = weaponNameList.begin(); it != weaponNameList.end(); ++it)
//    {
//        auto weaponName = weaponManager->GetXfilename(*it);
//
//        D3DXVECTOR3 m_pos = D3DXVECTOR3(0.f, 0.f, 0.f);
//        D3DXVECTOR3 rot = D3DXVECTOR3(0.f, 0.f, 0.f);
//        Mesh* mesh = NEW Mesh(weaponName, m_pos, rot, 0.1f);
//        mesh->Init();
//        mesh->SetWeapon(true);
//        m_weaponMesh[*it] = mesh;
//    }

    {
        D3DXVECTOR3 pos = D3DXVECTOR3(0.f, 0.f, 0.f);
        D3DXVECTOR3 rot = D3DXVECTOR3(0.f, 0.f, 0.f);
        Mesh * mesh = NEW Mesh(_T("res\\model\\rock1.x"), pos, rot, 0.1f);
        mesh->Init();
        mesh->SetWeapon(true);
        m_weaponMesh[_T("石")] = mesh;
    }
    {
        D3DXVECTOR3 pos = D3DXVECTOR3(0.f, 0.f, 0.f);
        D3DXVECTOR3 rot = D3DXVECTOR3(0.f, 0.f, D3DX_PI);
        Mesh * mesh = NEW Mesh(_T("res\\model\\stick.x"), pos, rot, 1.f);
        mesh->Init();
        mesh->SetWeapon(true);
        m_weaponMesh[_T("木の棒")] = mesh;
    }
    {
        D3DXVECTOR3 pos = D3DXVECTOR3(0.f, 0.f, 0.f);
        D3DXVECTOR3 rot = D3DXVECTOR3(0.f, 0.f, D3DX_PI);
        Mesh * mesh = NEW Mesh(_T("res\\model\\ax.x"), pos, rot, 1.0f);
        mesh->Init();
        mesh->SetWeapon(true);
        m_weaponMesh[_T("石斧")] = mesh;
    }
    {
        D3DXVECTOR3 pos = D3DXVECTOR3(0.f, 0.f, 0.f);
        D3DXVECTOR3 rot = D3DXVECTOR3(0.f, 0.f, D3DX_PI);
        Mesh * mesh = NEW Mesh(_T("res\\model\\atlatl.x"), pos, rot, 1.0f);
        mesh->Init();
        mesh->SetWeapon(true);
        m_weaponMesh[_T("アトラトル")] = mesh;
    }
    {
        D3DXVECTOR3 pos = D3DXVECTOR3(0.f, 0.f, 0.f);
        D3DXVECTOR3 rot = D3DXVECTOR3(0.f, 0.f, D3DX_PI);
        Mesh * mesh = NEW Mesh(_T("res\\model\\arrow.x"), pos, rot, 1.0f);
        mesh->Init();
        mesh->SetWeapon(true);
        m_weaponMesh[_T("アトラトルの矢")] = mesh;
    }
    {
        D3DXVECTOR3 pos = D3DXVECTOR3(0.f, 0.f, 0.f);
        D3DXVECTOR3 rot = D3DXVECTOR3(0.f, 0.f, D3DX_PI);
        Mesh * mesh = NEW Mesh(_T("res\\model\\ironPipe.x"), pos, rot, 1.0f);
        mesh->Init();
        mesh->SetWeapon(true);
        m_weaponMesh[_T("鉄パイプ")] = mesh;
    }
    {
        D3DXVECTOR3 pos = D3DXVECTOR3(0.f, 0.f, 0.f);
        D3DXVECTOR3 rot = D3DXVECTOR3(0.f, 0.f, D3DX_PI);
        Mesh * mesh = NEW Mesh(_T("res\\model\\ironPipeEx.x"), pos, rot, 1.0f);
        mesh->Init();
        mesh->SetWeapon(true);
        m_weaponMesh[_T("石付き鉄パイプ")] = mesh;
    }
    {
        D3DXVECTOR3 pos = D3DXVECTOR3(0.f, 0.f, 0.f);
        D3DXVECTOR3 rot = D3DXVECTOR3(0.f, 0.f, D3DX_PI);
        Mesh * mesh = NEW Mesh(_T("res\\model\\spear.x"), pos, rot, 1.0f);
        mesh->Init();
        mesh->SetWeapon(true);
        m_weaponMesh[_T("石槍")] = mesh;
    }
    {
        D3DXVECTOR3 pos = D3DXVECTOR3(0.f, 0.f, 0.f);
        D3DXVECTOR3 rot = D3DXVECTOR3(D3DX_PI / 2, 0.f, D3DX_PI);
        Mesh * mesh = NEW Mesh(_T("res\\model\\bow.x"), pos, rot, 1.0f);
        mesh->Init();
        mesh->SetWeapon(true);
        m_weaponMesh[_T("弓矢の弓")] = mesh;
    }
    {
        D3DXVECTOR3 pos = D3DXVECTOR3(0.f, 0.f, -0.3f);
        D3DXVECTOR3 rot = D3DXVECTOR3(D3DX_PI / 2, 0.f, D3DX_PI);
        Mesh * mesh = NEW Mesh(_T("res\\model\\arrow.x"), pos, rot, 1.0f);
        mesh->Init();
        mesh->SetWeapon(true);
        m_weaponMesh[_T("弓矢の矢")] = mesh;
    }
    {
        D3DXVECTOR3 pos = D3DXVECTOR3(0.f, 0.f, 0.f);
        D3DXVECTOR3 rot = D3DXVECTOR3(0.f, 0.f, D3DX_PI);
        Mesh * mesh = NEW Mesh(_T("res\\model\\torch.x"), pos, rot, 1.f);
        mesh->Init();
        mesh->SetWeapon(true);
        m_weaponMesh[_T("松明")] = mesh;
    }
    
    // 袋
    {
        D3DXVECTOR3 pos = D3DXVECTOR3(0.f, 0.f, 0.f);
        D3DXVECTOR3 rot = D3DXVECTOR3(0.f, 0.f, 0.f);
        Mesh * mesh = NEW Mesh(_T("res\\model\\bag.x"), pos, rot, 1.0f);
        mesh->Init();
        m_bagMesh[NSStarmanLib::eBagPos::Back1] = mesh;
    }
    {
        D3DXVECTOR3 pos = D3DXVECTOR3(0.f, 0.f, 0.f);
        D3DXVECTOR3 rot = D3DXVECTOR3(0.f, 0.f, 0.f);
        Mesh * mesh = NEW Mesh(_T("res\\model\\bag.x"), pos, rot, 1.0f);
        mesh->Init();
        m_bagMesh[NSStarmanLib::eBagPos::Back2] = mesh;
    }
    {
        D3DXVECTOR3 pos = D3DXVECTOR3(0.f, 0.f, 0.f);
        D3DXVECTOR3 rot = D3DXVECTOR3(0.f, 0.f, 0.f);
        Mesh * mesh = NEW Mesh(_T("res\\model\\bag.x"), pos, rot, 1.0f);
        mesh->Init();
        m_bagMesh[NSStarmanLib::eBagPos::Front] = mesh;
    }
    {
        D3DXVECTOR3 pos = D3DXVECTOR3(0.f, 0.f, 0.f);
        D3DXVECTOR3 rot = D3DXVECTOR3(0.f, 0.f, 0.f);
        Mesh * mesh = NEW Mesh(_T("res\\model\\bag.x"), pos, rot, 1.0f);
        mesh->Init();
        m_bagMesh[NSStarmanLib::eBagPos::Left] = mesh;
    }
    {
        D3DXVECTOR3 pos = D3DXVECTOR3(0.f, 0.f, 0.f);
        D3DXVECTOR3 rot = D3DXVECTOR3(0.f, 0.f, 0.f);
        Mesh * mesh = NEW Mesh(_T("res\\model\\bag.x"), pos, rot, 1.0f);
        mesh->Init();
        m_bagMesh[NSStarmanLib::eBagPos::Right] = mesh;
    }

    float x = 0.f;
    float y = 0.f;
    float z = 0.f;
    NSStarmanLib::StatusManager::GetObj()->GetXYZ(&x, &y, &z);
    m_loadingPos.x = x;
    m_loadingPos.y = y + 1.f;
    m_loadingPos.z = z;

    SoundEffect::get_ton()->load(_T("res\\sound\\fireSet.wav"));
    SoundEffect::get_ton()->load(_T("res\\sound\\iceSet.wav"));
    SoundEffect::get_ton()->load(_T("res\\sound\\darkSet.wav"));

    D3DXCreateFont(SharedObj::GetD3DDevice(),
                   48,
                   0,
                   FW_NORMAL,
                   1,
                   false,
                   SHIFTJIS_CHARSET,
                   OUT_TT_ONLY_PRECIS,
                   CLEARTYPE_NATURAL_QUALITY,
                   FF_DONTCARE,
                   _T("Shippori Mincho"),
                   &m_D3DFont);
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
    //-----------------------------------------------
    // 海と接しているか
    //-----------------------------------------------
    {
        D3DXVECTOR3 pos = m_loadingPos;
        D3DXVECTOR3 down(0.f, -0.1f, 0.f);
        
        m_bUnderwater = map->IntersectWater(pos, down);
        if (m_bUnderwater)
        {
            m_AnimMesh2->SetAnim(_T("IdleWater"));
            auto status = NSStarmanLib::StatusManager::GetObj();
            status->SetPlayerAction(NSStarmanLib::StatusManager::PlayerState::IDLE_WATER);

            // 水中で寝たら死亡
            auto sleep = Common::Status()->GetSleep();

            if (sleep)
            {
                Common::Status()->SetDead(true);
                Common::Status()->SetDeadReason(NSStarmanLib::eDeadReason::DROWNING);
            }
        }
    }

    //-----------------------------------------------
    // 洞窟にいるか。
    // 
    // 洞窟の入り口の特定のエリアに触れたら洞窟内判定
    // 洞窟の入り口の特定のエリアに触れたら洞窟外判定、とする
    //-----------------------------------------------
    {
        D3DXVECTOR3 pos = m_loadingPos;

        // TODO
        D3DXVECTOR3 inCave(100.f, 0.f, 0.f);

        // TODO
        D3DXVECTOR3 outCave(200.f, 0.f, 0.f);

        if (!m_bInCave)
        {
            if (Common::HitByBoundingBox(pos, inCave, 20.f))
            {
                m_bInCave = true;
            }
        }
        else
        {
            if (Common::HitByBoundingBox(pos, outCave, 20.f))
            {
                m_bInCave = false;
            }
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

    if (SharedObj::KeyBoard()->IsDown(DIK_W))
    {
        move.x += -std::sin(radian + D3DX_PI / 2);
        move.z += std::sin(radian + D3DX_PI);

        D3DXVECTOR3 rotate { 0.f, yaw, 0.f };
        SetRotate(rotate);
        SetWalk();
    }

    if (Common::DebugMode())
    {
        if (SharedObj::KeyBoard()->IsDown(DIK_G))
        {
            // デバッグ目的でGキーだけ移動速度アップ
            move.x += -std::sin(radian + (D3DX_PI / 2)) * 10;
            move.z += std::sin(radian + D3DX_PI) * 10;

            D3DXVECTOR3 rotate{ 0.f, yaw, 0.f };
            SetRotate(rotate);
            SetWalk();

        }
    }

    if (SharedObj::KeyBoard()->IsDown(DIK_A))
    {
        move.x += -std::sin(radian + D3DX_PI);
        move.z += std::sin(radian + D3DX_PI * 3 / 2);

        D3DXVECTOR3 rotate { 0.f, yaw + D3DX_PI * 3 / 2, 0.f };
        SetRotate(rotate);
        SetWalk();
    }

    if (SharedObj::KeyBoard()->IsDown(DIK_S))
    {
        move.x += -std::sin(radian + D3DX_PI * 3 / 2);
        move.z += std::sin(radian);

        D3DXVECTOR3 rotate { 0.f, yaw + D3DX_PI, 0.f };
        SetRotate(rotate);
        SetWalk();
    }

    if (SharedObj::KeyBoard()->IsDown(DIK_D))
    {
        move.x += -std::sin(radian);
        move.z += std::sin(radian + D3DX_PI / 2);

        D3DXVECTOR3 rotate { 0.f, yaw + D3DX_PI / 2, 0.f };
        SetRotate(rotate);
        SetWalk();
    }

    if (SharedObj::KeyBoard()->IsDownFirstFrame(DIK_SPACE))
    {
        if (m_bUnderwater == false)
        {
            // 左、右、後ろが入力されているときにジャンプしようとしたらステップ移動
            if (SharedObj::KeyBoard()->IsDown(DIK_A))
            {
                SetStep(eDir::LEFT);
            }
            else if (SharedObj::KeyBoard()->IsDown(DIK_S))
            {
                SetStep(eDir::BACK);
            }
            else if (SharedObj::KeyBoard()->IsDown(DIK_D))
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
        // スーパージャンプ
        if (SharedObj::KeyBoard()->IsDownFirstFrame(DIK_J))
        {
            m_bJump = true;
            m_move.y = JUMP_INITIAL_VELOCITY*5;
            m_AnimMesh2->SetAnim(_T("Jump"), 0.f);
        }

        // 木を消す
        if (SharedObj::KeyBoard()->IsDownFirstFrame(DIK_Z))
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

        // 敵を消す
        if (SharedObj::KeyBoard()->IsDownFirstFrame(DIK_X))
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

    // Shift + F → もの投げ
    if (SharedObj::KeyBoard()->IsDownFirstFrame(DIK_F))
    {
        if (SharedObj::KeyBoard()->IsDown(DIK_LSHIFT))
        {
            Throw();
        }
    }

    //------------------------------------------------------------
    // Mouse
    //------------------------------------------------------------

    if (Mouse::IsDownLeft())
    {
        if (SharedObj::KeyBoard()->IsDown(DIK_LSHIFT) == false)
        {
            bool ret = SetAttack();
        }
        else
        {
            // Shift + 左Click → 魔法
            SetMagic();
        }
    }

    // 魔法切り替え
    if (Mouse::IsWheelUp())
    {
        if (NSStarmanLib::Rynen::GetObj()->GetContracted())
        {
            auto status = NSStarmanLib::StatusManager::GetObj();
            auto magicType = status->GetMagicType();

            if (magicType == NSStarmanLib::eMagicType::None)
            {
                magicType = NSStarmanLib::eMagicType::Dark;
            }
            else if (magicType == NSStarmanLib::eMagicType::Fire)
            {
                magicType = NSStarmanLib::eMagicType::None;
            }
            else if (magicType == NSStarmanLib::eMagicType::Ice)
            {
                magicType = NSStarmanLib::eMagicType::Fire;
            }
            else if (magicType == NSStarmanLib::eMagicType::Dark)
            {
                magicType = NSStarmanLib::eMagicType::Ice;
            }

            status->SetMagicType(magicType);
            m_switchMagicCounter = 0;
            m_bSwitchMagic = true;
        }
    }
    else if (Mouse::IsWheelDown())
    {
        if (NSStarmanLib::Rynen::GetObj()->GetContracted())
        {
            auto status = NSStarmanLib::StatusManager::GetObj();
            auto magicType = status->GetMagicType();

            if (magicType == NSStarmanLib::eMagicType::None)
            {
                magicType = NSStarmanLib::eMagicType::Fire;
            }
            else if (magicType == NSStarmanLib::eMagicType::Fire)
            {
                magicType = NSStarmanLib::eMagicType::Ice;
            }
            else if (magicType == NSStarmanLib::eMagicType::Ice)
            {
                magicType = NSStarmanLib::eMagicType::Dark;
            }
            else if (magicType == NSStarmanLib::eMagicType::Dark)
            {
                magicType = NSStarmanLib::eMagicType::None;
            }

            status->SetMagicType(magicType);
            m_switchMagicCounter = 0;
            m_bSwitchMagic = true;
        }
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

    if (GamePad::IsDown(eGamePadButtonType::L1))
    {
        Throw();
    }

    if (GamePad::IsDown(eGamePadButtonType::B))
    {
        SetJump();
    }

    if (GamePad::IsDown(eGamePadButtonType::UP))
    {
        if (NSStarmanLib::Rynen::GetObj()->GetContracted())
        {
            auto status = NSStarmanLib::StatusManager::GetObj();
            auto magicType = status->GetMagicType();

            if (magicType == NSStarmanLib::eMagicType::None)
            {
                magicType = NSStarmanLib::eMagicType::Dark;
            }
            else if (magicType == NSStarmanLib::eMagicType::Fire)
            {
                magicType = NSStarmanLib::eMagicType::None;
            }
            else if (magicType == NSStarmanLib::eMagicType::Ice)
            {
                magicType = NSStarmanLib::eMagicType::Fire;
            }
            else if (magicType == NSStarmanLib::eMagicType::Dark)
            {
                magicType = NSStarmanLib::eMagicType::Ice;
            }

            status->SetMagicType(magicType);
            m_switchMagicCounter = 0;
            m_bSwitchMagic = true;
        }
    }

    if (GamePad::IsDown(eGamePadButtonType::DOWN))
    {
        if (NSStarmanLib::Rynen::GetObj()->GetContracted())
        {
            auto status = NSStarmanLib::StatusManager::GetObj();
            auto magicType = status->GetMagicType();

            if (magicType == NSStarmanLib::eMagicType::None)
            {
                magicType = NSStarmanLib::eMagicType::Fire;
            }
            else if (magicType == NSStarmanLib::eMagicType::Fire)
            {
                magicType = NSStarmanLib::eMagicType::Ice;
            }
            else if (magicType == NSStarmanLib::eMagicType::Ice)
            {
                magicType = NSStarmanLib::eMagicType::Dark;
            }
            else if (magicType == NSStarmanLib::eMagicType::Dark)
            {
                magicType = NSStarmanLib::eMagicType::None;
            }

            status->SetMagicType(magicType);
            m_switchMagicCounter = 0;
            m_bSwitchMagic = true;
        }
    }

    //----------------------------------------------------------
    // Finalize
    //----------------------------------------------------------

    // ジャンプ中は移動方向を変えたり加速したりできない
    auto statusManager = NSStarmanLib::StatusManager::GetObj();
    if (m_bJump || m_bStep || statusManager->GetDead())
    {
        {
            move = D3DXVECTOR3(0.f, 0.f, 0.f);
        }
    }

    // 高度が1500を超えたら上昇をやめる
    if (m_loadingPos.y >= 1500.f)
    {
        move.y = -10.f;
    }

    m_move += move;

    float MAX_XZ_MOVE = 0.f;

    // 1フレームで50センチ以上移動しようとしたら50センチにする
    // ただし、XZ平面のみ。Y軸方向は加味しない。
    if (Common::DebugMode())
    {
        if (!m_bUnderwater)
        {
            if (Common::FasterMode())
            {
                MAX_XZ_MOVE = 50.0f;
            }
            else
            {
				if (!m_bStep)
				{
					MAX_XZ_MOVE = 0.5f;
				}
                else
                {
					MAX_XZ_MOVE = 1.f;
                }
            }
        }
        else
        {
            if (Common::FasterMode())
            {
                MAX_XZ_MOVE = 500.f;
            }
            else
            {
                MAX_XZ_MOVE = 0.1f;
            }
        }
    }
    else
    {
        if (!m_bUnderwater)
        {
			if (!m_bStep)
			{
				MAX_XZ_MOVE = 0.5f;
			}
			else
			{
				MAX_XZ_MOVE = 1.f;
			}
        }
        else
        {
            MAX_XZ_MOVE = 0.1f;
        }
    }

    D3DXVECTOR2 move_XZ(m_move.x, m_move.z);
    FLOAT speed = D3DXVec2Length(&move_XZ);

    //MAX_XZ_MOVE *= statusManager->GetWalkSpeed();

    // もし100センチ移動しようとしていたら2で割ればよい。
    if (speed >= MAX_XZ_MOVE)
    {
        float work = 0.f;
        if (speed != 0.f)
        {
            work = MAX_XZ_MOVE / speed;
        }
        move_XZ *= work;
        m_move.x = move_XZ.x;
        m_move.z = move_XZ.y;
    }

    // XZ平面上の移動量は毎フレーム半分にする。ジャンプしているときは半分にしない。
    if (!m_bJump && !m_bStep)
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

        if (m_attackTimeCounter >= 30)
        {
            m_attackTimeCounter = 0;
            m_bAttack = false;
        }
    }

    if (m_bStep)
    {
        m_stepCounter++;
        if (m_stepCounter >= 15)
        {
            m_bStep = false;
            m_stepCounter = 0;
        }
    }

    if (m_bThrow)
    {
        m_throwTimeCounter++;

        if (m_throwTimeCounter >= 30)
        {
            m_throwTimeCounter = 0;
            m_bThrow = false;

            // 投げた後、インベントリに同一のアイテムがある場合、
            // 再度装備される。なければ素手になる。
            NSStarmanLib::Inventory* inventory = NSStarmanLib::Inventory::GetObj();
            std::vector<int> subIdList = inventory->GetSubIdList(m_throwItemId);
            if (subIdList.empty() == false)
            {
                NSStarmanLib::ItemInfo itemInfo =
                    inventory->GetItemInfo(m_throwItemId, subIdList.at(0));

                NSStarmanLib::StatusManager* statusManager = NSStarmanLib::StatusManager::GetObj();
                statusManager->SetEquipWeapon(itemInfo);
            }
        }
    }

    if (m_bMagic)
    {
        m_magicTimeCounter++;

        if (m_magicTimeCounter >= 90)
        {
            m_magicTimeCounter = 0;
            m_bMagic = false;
        }
    }

    if (m_bDamaged)
    {
        m_damagedTimeCounter++;

        if (m_damagedTimeCounter >= 30)
        {
            m_damagedTimeCounter = 0;
            m_bDamaged = false;
        }
    }

    // 重力
    m_move.y += -0.01f;

    // 壁ずり
    // 壁ずりは足に対してだけ行う
    bool bHit = false;
    bool bInside = false;
    m_move = map->WallSlide(m_loadingPos, m_move, &bHit, &bInside);

    if (!bInside)
    {
        // 足以外の衝突判定
        // 胴と頭
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
    }

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

    // イカダとの衝突判定
    // ぶつかっていたら移動量をゼロにする、のではなく、壁ずりを行う。
    // イカダの上を歩ける必要があるため。
    bool bHitRaft = false;
    m_move = SharedObj::Voyage()->WallSlide(m_loadingPos, m_move, &bHitRaft);

    if (bHitRaft)
    {
        if (m_bJump)
        {
            m_bJump = false;
            m_bJumpEnable = true;
        }
    }

    m_loadingPos += m_move;

    // 緊急手段
    // 頭から地面までの距離が1.5メートル以下だったら地面にめり込んでいる？
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

    auto bagState = Common::Status()->GetBagState();
    for (auto it = bagState.begin(); it != bagState.end(); ++it)
    {
        if ((*it) == NSStarmanLib::eBagPos::Back1)
        {
            auto pos = m_loadingPos;
            pos.y += 0.6f;

            pos.x += std::sin(m_rotate.y) * 0.1f;
            pos.z += std::cos(m_rotate.y) * 0.1f;

            m_bagMesh[(*it)]->SetPos(pos);
            m_bagMesh[(*it)]->SetRotY(m_rotate.y);
        }
        else if ((*it) == NSStarmanLib::eBagPos::Back2)
        {
            auto pos = m_loadingPos;
            pos.y += 0.7f;
            pos.x += std::sin(m_rotate.y) * 0.5f;
            pos.z += std::cos(m_rotate.y) * 0.5f;

            m_bagMesh[(*it)]->SetPos(pos);
            m_bagMesh[(*it)]->SetRotY(m_rotate.y);

        }
        else if ((*it) == NSStarmanLib::eBagPos::Front)
        {
            auto pos = m_loadingPos;
            pos.y += 0.5f;
            pos.x += std::sin(m_rotate.y + D3DX_PI) * 0.1f;
            pos.z += std::cos(m_rotate.y + D3DX_PI) * 0.1f;

            m_bagMesh[(*it)]->SetPos(pos);
            m_bagMesh[(*it)]->SetRotY(m_rotate.y + D3DX_PI);

        }
        else if ((*it) == NSStarmanLib::eBagPos::Left)
        {
            auto pos = m_loadingPos;
            pos.y += 0.1f;
            pos.x += std::sin(m_rotate.y + (D3DX_PI/2.f)) * 0.5f;
            pos.z += std::cos(m_rotate.y + (D3DX_PI/2.f)) * 0.5f;

            m_bagMesh[(*it)]->SetPos(pos);
            m_bagMesh[(*it)]->SetRotY(m_rotate.y + D3DX_PI + (D3DX_PI/2.f));

        }
        else if ((*it) == NSStarmanLib::eBagPos::Right)
        {
            auto pos = m_loadingPos;
            pos.y += 0.1f;
            pos.x += std::sin(m_rotate.y - (D3DX_PI/2.f)) * 0.5f;
            pos.z += std::cos(m_rotate.y - (D3DX_PI/2.f)) * 0.5f;

            m_bagMesh[(*it)]->SetPos(pos);
            m_bagMesh[(*it)]->SetRotY(m_rotate.y + D3DX_PI - (D3DX_PI/2.f));

        }
        m_bagMesh[(*it)]->Render();

    }
}

void Player::Render2D()
{
    if (m_bSwitchMagic)
    {
        ++m_switchMagicCounter;
        if (m_switchMagicCounter >= 60)
        {
            m_bSwitchMagic = false;
        }
        int transparency = 255 - (m_switchMagicCounter * 255 / 60);

        std::wstring magicType = Common::LoadString_(IDS_STRING137);

        auto statusManager = NSStarmanLib::StatusManager::GetObj();
        auto equipMagic = statusManager->GetMagicType();
        if (equipMagic == NSStarmanLib::eMagicType::Fire)
        {
            magicType = Common::LoadString_(IDS_STRING138);
        }
        else if (equipMagic == NSStarmanLib::eMagicType::Ice)
        {
            magicType = Common::LoadString_(IDS_STRING139);
        }
        else if (equipMagic == NSStarmanLib::eMagicType::Dark)
        {
            magicType = Common::LoadString_(IDS_STRING140);
        }

        RECT rect;
        SetRect(&rect, 750, 300, 850, 360);
        m_D3DFont->DrawText(NULL,
                            magicType.c_str(),
                            -1,
                            &rect,
                            DT_CENTER,
                            D3DCOLOR_ARGB(transparency, 255, 255, 255));
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
    if (!m_bJump && !m_bStep && NSStarmanLib::StatusManager::GetObj()->GetDead() == false)
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

    // 右手にバッグを持っているときは攻撃できない。
    if (statusManager->GetBag(NSStarmanLib::eBagPos::Right).GetId() != -1)
    {
        return false;
    }

    statusManager->ConsumeAttackCost();

    auto itemManager = NSStarmanLib::ItemManager::GetObj();
    auto weaponId = statusManager->GetEquipWeapon().GetId();
    if (statusManager->GetEquipWeapon().GetId() != -1)
    {
        auto itemDef = itemManager->GetItemDef(weaponId);
        auto weaponName = itemDef.GetName();
        if (weaponName == Common::LoadString_(IDS_STRING141))
        {
            SetAttackArrow();
            return true;
        }
        else if (weaponName == Common::LoadString_(IDS_STRING142))
        {
            SetAttackAtlatl();
            return true;
        }
    }

    SoundEffect::get_ton()->play(_T("res\\sound\\attack01.wav"), 90);
    m_AnimMesh2->SetAnim(_T("Attack"), 0.f);
    m_bAttack = true;

    D3DXVECTOR3 attackPos = GetAttackPos();
    D3DXVECTOR3 enemyPos { 0.f, 0.f, 0.f };
    std::vector<EnemyBase*> vecEnemy = SharedObj::GetMap()->GetEnemy();

    for (std::size_t i = 0; i < vecEnemy.size(); i++)
    {
        // 島民の霊は闇魔法でしかダメージを与えられない
        if (vecEnemy.at(i)->GetEnemyType() == eEnemyType::Ghost)
        {
            continue;
        }

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

            // 松明だったら一度で壊れる。火も消える
            auto itemInfo = Common::Status()->GetEquipWeapon();
            if (itemInfo.GetId() != -1)
            {
                if (itemInfo.GetItemDef().GetName() == Common::LoadString_(IDS_STRING133))
                {
                    NSStarmanLib::WeaponManager::GetObj()->SetTorchLit(false);
                    Common::Inventory()->SetItemDurability(itemInfo.GetId(), itemInfo.GetSubId(), 0);
                }
            }
        }
    }

    SharedObj::GetMap()->SetEnemy(vecEnemy);

    return true;
}

bool Player::SetAttackArrow()
{
    // インベントリから弓矢の矢を一つ減らす。
    auto inventory = NSStarmanLib::Inventory::GetObj();
    auto itemManager = NSStarmanLib::ItemManager::GetObj();

    // 強化値の強い矢のほうからなくなる
    int arrowCnt = 0;
    arrowCnt = inventory->CountItem(Common::LoadString_(IDS_STRING143), 1);

    int arrowLevel = 0;

    NSStarmanLib::ItemDef itemDef;
    NSStarmanLib::ItemInfo itemInfo;

    if (arrowCnt > 0)
    {
        itemDef = itemManager->GetItemDef(Common::LoadString_(IDS_STRING143), 1);

        auto subIdList = inventory->GetSubIdList(itemDef.GetId());

        itemInfo = inventory->GetItemInfo(itemDef.GetId(), subIdList.at(0));

        inventory->RemoveItem(itemDef.GetId(), subIdList.at(0));
        arrowLevel = 1;
    }
    else
    {
        arrowCnt = inventory->CountItem(Common::LoadString_(IDS_STRING143), -1);
        if (arrowCnt > 0)
        {
            itemDef = itemManager->GetItemDef(Common::LoadString_(IDS_STRING143), -1);

            auto subIdList = inventory->GetSubIdList(itemDef.GetId());

            itemInfo = inventory->GetItemInfo(itemDef.GetId(), subIdList.at(0));

            inventory->RemoveItem(itemDef.GetId(), subIdList.at(0));
            arrowLevel = 0;
        }
        else
        {
            return false;
        }
    }

    m_AnimMesh2->SetAnim(_T("Arrow"), 0.f);

    int bowLevel = 0;
    {
        auto statusManager = NSStarmanLib::StatusManager::GetObj();
        NSStarmanLib::ItemInfo equip = statusManager->GetEquipWeapon();
        NSStarmanLib::ItemDef equip2 = itemManager->GetItemDef(equip.GetId());
        bowLevel = equip2.GetLevel();
        if (bowLevel == -1)
        {
            bowLevel = 0;
        }
    }

    m_bAttack = true;

    // 矢を投げるものとしてセット
    {
        D3DXVECTOR3 pos(m_loadingPos);
        pos.y += 0.85f;
        pos.x += std::sin(m_rotate.y + D3DX_PI) * 0.5f;
        pos.z += std::sin(m_rotate.y + (D3DX_PI * 3 / 2)) * 0.5f;

        D3DXVECTOR3 norm(0.f, 0.f, 0.f);
        norm.x = std::sin(m_rotate.y + D3DX_PI);
        norm.z = std::sin(m_rotate.y + (D3DX_PI * 3 / 2));

        norm *= 0.1f + (bowLevel * 0.1f) + (arrowLevel * 0.2f); // TODO 微調整
        norm.y = 0.05f;

        float power = 0.f;

        // TODO 微調整
        power = 0.01f;
        power += (bowLevel * 0.2f) + (arrowLevel * 10.f);

        SharedObj::GetMap()->AddThrownItem(pos, norm, Common::LoadString_(IDS_STRING143), itemInfo, 1.f, power, m_rotate.y);
    }

    // 体力を消耗する
    {
        auto statusManager = NSStarmanLib::StatusManager::GetObj();
        statusManager->ConsumeAttackCost();
    }
    return true;
}

bool Player::SetAttackAtlatl()
{
    // インベントリからアトラトルの矢を一つ減らす。
    auto inventory = NSStarmanLib::Inventory::GetObj();
    auto itemManager = NSStarmanLib::ItemManager::GetObj();

    // 強化値の強い槍のほうからなくなる
    // 強化値は-1,1,2,3,4,5の6種類
    int arrowLevel = 0;
    for (int i = 5; ;)
    {
        int arrowCnt = inventory->CountItem(Common::LoadString_(IDS_STRING144), i);
        if (arrowCnt >= 1)
        {
            arrowLevel = i;
            break;
        }

        --i;
        if (i == 0)
        {
            i = -1;
        }
        else if (i == -2)
        {
            break;
        }
    }

    if (arrowLevel == -2)
    {
        return false;
    }

    NSStarmanLib::ItemDef itemDef;
    NSStarmanLib::ItemInfo itemInfo;

    // インベントリから槍を一つ減らす
    {
        itemDef = itemManager->GetItemDef(Common::LoadString_(IDS_STRING144), arrowLevel);
        auto subIdList = inventory->GetSubIdList(itemDef.GetId());
        itemInfo = inventory->GetItemInfo(itemDef.GetId(), subIdList.at(0));
        inventory->RemoveItem(itemDef.GetId(), subIdList.at(0));
    }

    m_AnimMesh2->SetAnim(_T("Attack"), 0.f);

    m_bAttack = true;

    // 矢を投げるものとしてセット
    {
        // 初期位置
        D3DXVECTOR3 pos(m_loadingPos);
        pos.y += 1.15f;
        pos.x += std::sin(m_rotate.y + D3DX_PI) * 0.5f;
        pos.z += std::sin(m_rotate.y + (D3DX_PI * 3 / 2)) * 0.5f;

        // 方向と速度
        D3DXVECTOR3 norm(0.f, 0.f, 0.f);
        norm.x = std::sin(m_rotate.y + D3DX_PI);
        norm.z = std::sin(m_rotate.y + (D3DX_PI * 3 / 2));

        norm *= 0.1f + (arrowLevel * 0.2f); // TODO 微調整
        norm.y = 0.05f;

        float power = 0.f;

        // TODO 微調整
        power = 20.f;
        power += (arrowLevel * 5.f);

        SharedObj::GetMap()->AddThrownItem(pos, norm, Common::LoadString_(IDS_STRING144), itemInfo, 2.f, power, m_rotate.y);
    }

    // 体力を消耗する
    {
        auto statusManager = NSStarmanLib::StatusManager::GetObj();
        statusManager->ConsumeAttackCost();
    }
    return true;
}

void Player::SetWalk()
{
    auto status = NSStarmanLib::StatusManager::GetObj();
    if (!m_bJump &&
        !m_bStep &&
        !status->GetDead() &&
        !m_bAttack)
    {
        if (m_bUnderwater == false)
        {
            m_AnimMesh2->SetAnim(_T("Walk"));
            status->SetPlayerAction(NSStarmanLib::StatusManager::PlayerState::JOGGING);
        }
        else
        {
            m_AnimMesh2->SetAnim(_T("Swim"));
            status->SetPlayerAction(NSStarmanLib::StatusManager::PlayerState::SWIM);
        }
    }
}

void Player::SetIdle()
{
    m_AnimMesh2->SetAnim(_T("Idle"));
    Common::Status()->SetPlayerAction(NSStarmanLib::StatusManager::PlayerState::STAND);
}

void Player::SetDamaged()
{
    if (NSStarmanLib::StatusManager::GetObj()->GetDead() == false)
    {
        SoundEffect::get_ton()->play(_T("res\\sound\\damage01.wav"), 90);
        m_AnimMesh2->SetAnim(_T("Damaged"), 0.f);
        m_bDamaged = true;
    }

    // 攻撃食らったら回復
    if (Common::StrongMode() && Common::DebugMode())
    {
        Common::Status()->SetMuscleCurrent(100);
        Common::Status()->SetBodyStaminaCurrent(100);
        Common::Status()->SetBrainStaminaCurrent(100);
        Common::Status()->SetWaterCurrent(100);
    }

    // 睡眠中に攻撃されたら即死
    if (NSStarmanLib::StatusManager::GetObj()->GetSleep())
    {
        NSStarmanLib::StatusManager::GetObj()->SetDead(true);
        NSStarmanLib::StatusManager::GetObj()->SetDeadReason(NSStarmanLib::eDeadReason::ATTACK_ON_SLEEP);
    }
}

void Player::SetDead()
{
    m_AnimMesh2->SetAnim(_T("Dead"), 0.f);
}

bool Player::GetDead() const
{
    return NSStarmanLib::StatusManager::GetObj()->GetDead();
}

void Player::SetSleep(const bool arg)
{
    if (arg)
    {
        m_AnimMesh2->SetAnim(_T("LieDown"), 0.f);
        auto status = NSStarmanLib::StatusManager::GetObj();
        status->SetPlayerAction(NSStarmanLib::StatusManager::PlayerState::LYING_DOWN);
        status->SetSleep(true);
    }
    else
    {
        m_AnimMesh2->SetAnim(_T("Idle"), 0.f);
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
        m_AnimMesh2->SetAnim(_T("Jump"), 0.f);

        int brokenBagNum1 = 0;
        int brokenBagNum2 = 0;
        {
            auto allBag = Common::Status()->GetAllBag();
            brokenBagNum1 = (int)std::count_if(allBag.begin(), allBag.end(),
                                               [&](auto x)
                                               {
                                                   return x.GetDurabilityCurrent() == 0;
                                               });
        }

        Common::Inventory()->ReduceEquipBagDurability();

        // 新たに耐久値が0になった袋があったか
        {
            auto allBag = Common::Status()->GetAllBag();
            brokenBagNum2 = (int)std::count_if(allBag.begin(), allBag.end(),
                                               [&](auto x)
                                               {
                                                   return x.GetDurabilityCurrent() == 0;
                                               });
        }

        if (brokenBagNum1 != brokenBagNum2)
        {
            PopUp2::Get()->SetText(Common::LoadString_(IDS_STRING145));
        }

        SoundEffect::get_ton()->load(_T("res\\sound\\jump.wav"));
        SoundEffect::get_ton()->play(_T("res\\sound\\jump.wav"), 90);
    }
}

void Player::SetSit()
{
    m_AnimMesh2->SetAnim(_T("Sit"), 0.f);

    auto status = NSStarmanLib::StatusManager::GetObj();
    status->SetPlayerAction(NSStarmanLib::StatusManager::PlayerState::SIT);
}

void Player::SetLieDown()
{
    m_AnimMesh2->SetAnim(_T("LieDown"), 0.f);

    auto status = NSStarmanLib::StatusManager::GetObj();
    status->SetPlayerAction(NSStarmanLib::StatusManager::PlayerState::LYING_DOWN);
}

void Player::SetStep(const eDir dir)
{
    const float STEP_VELOCITY = 1.f;
    float radian = Camera::GetRadian();
    float yaw = -1.f * (radian - (D3DX_PI / 2));
    D3DXVECTOR3 move(0.f, 0.f, 0.f);
	D3DXVECTOR3 rotate { 0.f, yaw, 0.f };

    if (dir == eDir::LEFT)
    {
        move.x += -std::sin(radian + D3DX_PI);
        move.z += std::sin(radian + D3DX_PI * 3 / 2);
        move *= STEP_VELOCITY;
    }
    else if (dir == eDir::BACK)
    {
        move.x += -std::sin(radian + D3DX_PI * 3 / 2);
        move.z += std::sin(radian);
        move *= STEP_VELOCITY;
    }
    else if (dir == eDir::RIGHT)
    {
        move.x += -std::sin(radian);
        move.z += std::sin(radian + D3DX_PI / 2);
        move *= STEP_VELOCITY;
    }
    SetMove(move);
    SetRotate(rotate);

    m_bStep = true;
	SoundEffect::get_ton()->load(_T("res\\sound\\jump.wav"));
	SoundEffect::get_ton()->play(_T("res\\sound\\jump.wav"), 90);

    Common::Status()->ConsumeJumpCost();
}

void Player::SetExamine()
{
    // 
}

void Player::RideRaft()
{
    auto id = SharedObj::Voyage()->GetNearRaftId(m_loadingPos);
    if (id == 0)
    {
        return;
    }

    auto pos = SharedObj::Voyage()->GetRaftXYZ(id);
    m_loadingPos = pos;
    m_loadingPos.y += 0.2f;

    auto rotateY = SharedObj::Voyage()->GetRaftRotateY(id);
    m_rotate.y = rotateY;

    m_AnimMesh2->SetAnim(_T("Sit"));
}

bool Player::IsInCave()
{
    return m_bInCave;
}

bool Player::IsUnderWater()
{
    return m_bUnderwater;
}

void Player::Throw()
{
    //-------------------------------------------------------------
    // 1. 素手になる。
    // 2. 殴るモーションを再生
    // 3. 装備アイテムが頭上から前方に飛んでいく。
    // 4. インベントリに同一のアイテムがある場合、再度装備される。なければ素手になる。
    //
    // ・投げられたものは物に衝突して停止する。
    // ・敵にあたった場合は、ダメージを与え、地面に落ちる。
    // ・地面に落ちた物は拾える。
    // ・投げる動作は2秒に一回
    // ・体力を消耗する
    //-------------------------------------------------------------

    auto statusManager = NSStarmanLib::StatusManager::GetObj();

    // 素手だったら何もしない。
    auto itemInfo = statusManager->GetEquipWeapon();
    if (itemInfo.GetId() == -1)
    {
        return;
    }

    // 投げている最中なら何もしない
    if (m_bThrow)
    {
        return;
    }

    // インベントリから一つアイテムを減らす。
    auto inventory = NSStarmanLib::Inventory::GetObj();
    inventory->RemoveItem(itemInfo.GetId(), itemInfo.GetSubId());

    // 投げるものをセット
    {
        m_bThrow = true;

        D3DXVECTOR3 pos(m_loadingPos);
        pos.y += 2.f;

        D3DXVECTOR3 norm(0.f, 0.f, 0.f);
        norm.x = std::sin(m_rotate.y + D3DX_PI);
        norm.z = std::sin(m_rotate.y + (D3DX_PI * 3 / 2));

        norm *= 0.4f;
        norm.y = 0.1f;

        auto itemManager = NSStarmanLib::ItemManager::GetObj();
        auto itemDef = itemManager->GetItemDef(itemInfo.GetId());
        auto weaponManager = NSStarmanLib::WeaponManager::GetObj();
        std::wstring xfilename = weaponManager->GetXfilename(itemDef.GetName());

        if (xfilename.find(_T("rock")) != std::wstring::npos)
        {
            SharedObj::GetMap()->AddThrownItem(pos, norm, itemDef.GetName(), itemInfo, 0.1f);
        }
        else
        {
            SharedObj::GetMap()->AddThrownItem(pos, norm, itemDef.GetName(), itemInfo);
        }

        m_throwItemId = itemInfo.GetId();
    }

    // 素手にする
    {
        NSStarmanLib::ItemInfo itemInfo;
        itemInfo.SetId(-1);
        statusManager->SetEquipWeapon(itemInfo);

        SoundEffect::get_ton()->play(_T("res\\sound\\attack01.wav"), 90);
        m_AnimMesh2->SetAnim(_T("Attack"), 0.f);
    }

    // 体力を消耗する
    statusManager->ConsumeAttackCost();
}

void Player::SetMagic()
{
    //-------------------------------------------------------------
    // 1. 殴るモーションを再生
    // 2. 火の玉が頭上から前方に飛んでいく。
    //
    // ・投げる動作は2秒に一回
    // ・体力を消耗する
    //-------------------------------------------------------------

    auto statusManager = NSStarmanLib::StatusManager::GetObj();

    auto magicType = statusManager->GetMagicType();
    if (magicType == NSStarmanLib::eMagicType::None)
    {
        return;
    }

    // 投げている最中なら何もしない
    if (m_bMagic)
    {
        return;
    }

    // 投げるものをセット
    {
        m_bMagic = true;

        D3DXVECTOR3 pos(m_loadingPos);
        pos.y += 1.f;
        pos.x += std::sin(m_rotate.y + D3DX_PI);
        pos.z += std::sin(m_rotate.y + (D3DX_PI * 3 / 2));

        D3DXVECTOR3 norm(0.f, 0.f, 0.f);
        norm.x = std::sin(m_rotate.y + D3DX_PI);
        norm.z = std::sin(m_rotate.y + (D3DX_PI * 3 / 2));

        norm *= 0.4f;
        norm.y = 0.1f;

        float power = 0.f;

        if (magicType == NSStarmanLib::eMagicType::Fire)
        {
            power = ((float)statusManager->GetLevelFire()+1) * 2;
        }
        else if (magicType == NSStarmanLib::eMagicType::Ice)
        {
            power = ((float)statusManager->GetLevelIce()+1) * 2;
        }
        else if (magicType == NSStarmanLib::eMagicType::Dark)
        {
            power = ((float)statusManager->GetLevelDark()+1) * 2;
        }

        SharedObj::GetMap()->SetThrownMagic(pos, norm, magicType, power);
    }

    if (magicType == NSStarmanLib::eMagicType::Fire)
    {
        SoundEffect::get_ton()->play(_T("res\\sound\\fireSet.wav"));
    }
    else if (magicType == NSStarmanLib::eMagicType::Ice)
    {
        SoundEffect::get_ton()->play(_T("res\\sound\\iceSet.wav"));
    }
    else if (magicType == NSStarmanLib::eMagicType::Dark)
    {
        SoundEffect::get_ton()->play(_T("res\\sound\\darkSet.wav"));
    }

    m_AnimMesh2->SetAnim(_T("Magic"), 0.f);

    // 体力を消耗する
    statusManager->UseMagic();
}


