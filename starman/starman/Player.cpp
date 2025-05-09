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
        animSetMap["Idle"] = animSetting;
    }
    {
        AnimSetting animSetting { };
        animSetting.m_startPos = 1.f;
        animSetting.m_duration = 0.97f;
        animSetting.m_loop = false;
        animSetMap["Walk"] = animSetting;
    }
    {
        AnimSetting animSetting { };
        animSetting.m_startPos = 2.f;
        animSetting.m_duration = 0.97f;
        animSetting.m_loop = false;
        animSetMap["Attack"] = animSetting;
    }
    {
        AnimSetting animSetting { };
        animSetting.m_startPos = 3.f;
        animSetting.m_duration = 0.47f;
        animSetting.m_loop = false;
        animSetMap["Damaged"] = animSetting;
    }
    {
        AnimSetting animSetting { };
        animSetting.m_startPos = 3.5f;
        animSetting.m_duration = 0.47f;
        animSetting.m_loop = false;
        animSetting.m_stopEnd = true;
        animSetMap["Dead"] = animSetting;
    }
    {
        AnimSetting animSetting { };
        animSetting.m_startPos = 5.f;
        animSetting.m_duration = 1.9f;
        animSetting.m_loop = false;
        animSetMap["Jump"] = animSetting;
    }
    {
        AnimSetting animSetting { };
        animSetting.m_startPos = 7.0f;
        animSetting.m_duration = 0.66f;
        animSetting.m_loop = true;
        animSetMap["Sit"] = animSetting;
    }
    {
        AnimSetting animSetting { };
        animSetting.m_startPos = 7.7f;
        animSetting.m_duration = 0.27f;
        animSetting.m_loop = false;
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
    {
        AnimSetting animSetting { };
        animSetting.m_startPos = 10.0f;
        animSetting.m_duration = 0.97f;
        animSetting.m_loop = false;
        animSetMap["Magic"] = animSetting;
    }
    {
        AnimSetting animSetting { };
        animSetting.m_startPos = 11.0f;
        animSetting.m_duration = 0.97f;
        animSetting.m_loop = false;
        animSetMap["Arrow"] = animSetting;
    }
    m_AnimMesh2 = NEW AnimMesh("res\\model\\hoshiman.x", pos, rot, 1.f, animSetMap);
    m_AnimMesh2->SetAnim("Idle");
    SoundEffect::get_ton()->load("res\\sound\\attack01.wav");

    // �ǂݍ��ݏ����̑O�ɑ����Ă��܂��̂ňȉ��̂悤�ȏ�������X�t�@�C���̖��O���Q�Ƃł��Ȃ��B
    // �l�����ł���
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
        Mesh * mesh = NEW Mesh("res\\model\\rock1.x", pos, rot, 0.1f);
        mesh->Init();
        mesh->SetWeapon(true);
        m_weaponMesh["��"] = mesh;
    }
    {
        D3DXVECTOR3 pos = D3DXVECTOR3(0.f, 0.f, 0.f);
        D3DXVECTOR3 rot = D3DXVECTOR3(0.f, 0.f, D3DX_PI);
        Mesh * mesh = NEW Mesh("res\\model\\stick.x", pos, rot, 1.f);
        mesh->Init();
        mesh->SetWeapon(true);
        m_weaponMesh["�؂̖_"] = mesh;
    }
    {
        D3DXVECTOR3 pos = D3DXVECTOR3(0.f, 0.f, 0.f);
        D3DXVECTOR3 rot = D3DXVECTOR3(0.f, 0.f, D3DX_PI);
        Mesh * mesh = NEW Mesh("res\\model\\ax.x", pos, rot, 1.0f);
        mesh->Init();
        mesh->SetWeapon(true);
        m_weaponMesh["�Ε�"] = mesh;
    }
    {
        D3DXVECTOR3 pos = D3DXVECTOR3(0.f, 0.f, 0.f);
        D3DXVECTOR3 rot = D3DXVECTOR3(0.f, 0.f, D3DX_PI);
        Mesh * mesh = NEW Mesh("res\\model\\atlatl.x", pos, rot, 1.0f);
        mesh->Init();
        mesh->SetWeapon(true);
        m_weaponMesh["�A�g���g��"] = mesh;
    }
    {
        D3DXVECTOR3 pos = D3DXVECTOR3(0.f, 0.f, 0.f);
        D3DXVECTOR3 rot = D3DXVECTOR3(0.f, 0.f, D3DX_PI);
        Mesh * mesh = NEW Mesh("res\\model\\arrow.x", pos, rot, 1.0f);
        mesh->Init();
        mesh->SetWeapon(true);
        m_weaponMesh["�A�g���g���̖�"] = mesh;
    }
    {
        D3DXVECTOR3 pos = D3DXVECTOR3(0.f, 0.f, 0.f);
        D3DXVECTOR3 rot = D3DXVECTOR3(0.f, 0.f, D3DX_PI);
        Mesh * mesh = NEW Mesh("res\\model\\ironPipe.x", pos, rot, 1.0f);
        mesh->Init();
        mesh->SetWeapon(true);
        m_weaponMesh["�S�p�C�v"] = mesh;
    }
    {
        D3DXVECTOR3 pos = D3DXVECTOR3(0.f, 0.f, 0.f);
        D3DXVECTOR3 rot = D3DXVECTOR3(0.f, 0.f, D3DX_PI);
        Mesh * mesh = NEW Mesh("res\\model\\ironPipeEx.x", pos, rot, 1.0f);
        mesh->Init();
        mesh->SetWeapon(true);
        m_weaponMesh["�Εt���S�p�C�v"] = mesh;
    }
    {
        D3DXVECTOR3 pos = D3DXVECTOR3(0.f, 0.f, 0.f);
        D3DXVECTOR3 rot = D3DXVECTOR3(0.f, 0.f, D3DX_PI);
        Mesh * mesh = NEW Mesh("res\\model\\spear.x", pos, rot, 1.0f);
        mesh->Init();
        mesh->SetWeapon(true);
        m_weaponMesh["�Α�"] = mesh;
    }
    {
        D3DXVECTOR3 pos = D3DXVECTOR3(0.f, 0.f, 0.f);
        D3DXVECTOR3 rot = D3DXVECTOR3(D3DX_PI / 2, 0.f, D3DX_PI);
        Mesh * mesh = NEW Mesh("res\\model\\bow.x", pos, rot, 1.0f);
        mesh->Init();
        mesh->SetWeapon(true);
        m_weaponMesh["�|��̋|"] = mesh;
    }
    {
        D3DXVECTOR3 pos = D3DXVECTOR3(0.f, 0.f, -0.3f);
        D3DXVECTOR3 rot = D3DXVECTOR3(D3DX_PI / 2, 0.f, D3DX_PI);
        Mesh * mesh = NEW Mesh("res\\model\\arrow.x", pos, rot, 1.0f);
        mesh->Init();
        mesh->SetWeapon(true);
        m_weaponMesh["�|��̖�"] = mesh;
    }
    {
        D3DXVECTOR3 pos = D3DXVECTOR3(0.f, 0.f, 0.f);
        D3DXVECTOR3 rot = D3DXVECTOR3(0.f, 0.f, D3DX_PI);
        Mesh * mesh = NEW Mesh("res\\model\\torch.x", pos, rot, 1.f);
        mesh->Init();
        mesh->SetWeapon(true);
        m_weaponMesh["����"] = mesh;
    }
    
    // ��
    {
        D3DXVECTOR3 pos = D3DXVECTOR3(0.f, 0.f, 0.f);
        D3DXVECTOR3 rot = D3DXVECTOR3(0.f, 0.f, 0.f);
        Mesh * mesh = NEW Mesh("res\\model\\bag.x", pos, rot, 1.0f);
        mesh->Init();
        m_bagMesh[NSStarmanLib::eBagPos::Back1] = mesh;
    }
    {
        D3DXVECTOR3 pos = D3DXVECTOR3(0.f, 0.f, 0.f);
        D3DXVECTOR3 rot = D3DXVECTOR3(0.f, 0.f, 0.f);
        Mesh * mesh = NEW Mesh("res\\model\\bag.x", pos, rot, 1.0f);
        mesh->Init();
        m_bagMesh[NSStarmanLib::eBagPos::Back2] = mesh;
    }
    {
        D3DXVECTOR3 pos = D3DXVECTOR3(0.f, 0.f, 0.f);
        D3DXVECTOR3 rot = D3DXVECTOR3(0.f, 0.f, 0.f);
        Mesh * mesh = NEW Mesh("res\\model\\bag.x", pos, rot, 1.0f);
        mesh->Init();
        m_bagMesh[NSStarmanLib::eBagPos::Front] = mesh;
    }
    {
        D3DXVECTOR3 pos = D3DXVECTOR3(0.f, 0.f, 0.f);
        D3DXVECTOR3 rot = D3DXVECTOR3(0.f, 0.f, 0.f);
        Mesh * mesh = NEW Mesh("res\\model\\bag.x", pos, rot, 1.0f);
        mesh->Init();
        m_bagMesh[NSStarmanLib::eBagPos::Left] = mesh;
    }
    {
        D3DXVECTOR3 pos = D3DXVECTOR3(0.f, 0.f, 0.f);
        D3DXVECTOR3 rot = D3DXVECTOR3(0.f, 0.f, 0.f);
        Mesh * mesh = NEW Mesh("res\\model\\bag.x", pos, rot, 1.0f);
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

    SoundEffect::get_ton()->load("res\\sound\\fireSet.wav");
    SoundEffect::get_ton()->load("res\\sound\\iceSet.wav");
    SoundEffect::get_ton()->load("res\\sound\\darkSet.wav");

    D3DXCreateFont(SharedObj::GetD3DDevice(),
                   32,
                   0,
                   FW_NORMAL,
                   1,
                   false,
                   SHIFTJIS_CHARSET,
                   OUT_TT_ONLY_PRECIS,
                   ANTIALIASED_QUALITY,
                   FF_DONTCARE,
                   "�l�r ����",
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
    // �C�Ɛڂ��Ă��邩
    //-----------------------------------------------
    {
        D3DXVECTOR3 pos = m_loadingPos;
        D3DXVECTOR3 down(0.f, -0.1f, 0.f);
        
        m_bUnderwater = map->IntersectWater(pos, down);
        if (m_bUnderwater)
        {
            m_AnimMesh2->SetAnim("IdleWater");
            auto status = NSStarmanLib::StatusManager::GetObj();
            status->SetPlayerAction(NSStarmanLib::StatusManager::PlayerState::IDLE_WATER);

            // �����ŐQ���玀�S
            auto sleep = Common::Status()->GetSleep();

            if (sleep)
            {
                Common::Status()->SetDead(true);
                Common::Status()->SetDeadReason(NSStarmanLib::eDeadReason::DROWNING);
            }
        }
    }

    //-----------------------------------------------
    // ���A�ɂ��邩�B
    // 
    // ���A�̓�����̓���̃G���A�ɐG�ꂽ�瓴�A������
    // ���A�̓�����̓���̃G���A�ɐG�ꂽ�瓴�A�O����A�Ƃ���
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
            // �f�o�b�O�ړI��G�L�[�����ړ����x�A�b�v
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
            // ���A�E�A��낪���͂���Ă���Ƃ��ɃW�����v���悤�Ƃ�����X�e�b�v�ړ�
//            if (SharedObj::KeyBoard()->IsDown(DIK_A))
//            {
//                SetStep(eDir::LEFT);
//            }
//            else if (SharedObj::KeyBoard()->IsDown(DIK_S))
//            {
//                SetStep(eDir::BACK);
//            }
//            else if (SharedObj::KeyBoard()->IsDown(DIK_D))
//            {
//                SetStep(eDir::RIGHT);
//            }
//            else
            {
                SetJump();
            }
        }
    }

    if (Common::DebugMode())
    {
        // �X�[�p�[�W�����v
        if (SharedObj::KeyBoard()->IsDownFirstFrame(DIK_J))
        {
            m_bJump = true;
            m_move.y = JUMP_INITIAL_VELOCITY*5;
            m_AnimMesh2->SetAnim("Jump", 0.f);
        }

        // �؂�����
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

        // �G������
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

    // Shift + F �� ���̓���
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
            // Shift + ��Click �� ���@
            SetMagic();
        }
    }

    // ���@�؂�ւ�
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
            if (Common::FasterMode())
            {
                MAX_XZ_MOVE = 500.0f;
            }
            else
            {
                MAX_XZ_MOVE = 0.5f;
            }
        }
        else
        {
            if (Common::FasterMode())
            {
                MAX_XZ_MOVE = 1.1f;
            }
            else
            {
                MAX_XZ_MOVE = 0.1f;
            }
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

    //MAX_XZ_MOVE *= statusManager->GetWalkSpeed();

    // ����100�Z���`�ړ����悤�Ƃ��Ă�����2�Ŋ���΂悢�B
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

        if (m_attackTimeCounter >= 30)
        {
            m_attackTimeCounter = 0;
            m_bAttack = false;
        }
    }

    if (m_bThrow)
    {
        m_throwTimeCounter++;

        if (m_throwTimeCounter >= 30)
        {
            m_throwTimeCounter = 0;
            m_bThrow = false;

            // ��������A�C���x���g���ɓ���̃A�C�e��������ꍇ�A
            // �ēx���������B�Ȃ���Αf��ɂȂ�B
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

    // �d��
    m_move.y += -0.01f;

    // �ǂ���
    // �ǂ���͑��ɑ΂��Ă����s��
    bool bHit { false };
    bool bInside = false;
    m_move = map->WallSlide(m_loadingPos, m_move, &bHit, &bInside);

    if (!bInside)
    {
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

    // �C�J�_�Ƃ̏Փ˔���
    // �Ԃ����Ă�����ړ��ʂ��[���ɂ���A�̂ł͂Ȃ��A�ǂ�����s���B
    // �C�J�_�̏�������K�v�����邽�߁B
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

        std::string magicType = Common::LoadString_(IDS_STRING137);

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

    // �E��Ƀo�b�O�������Ă���Ƃ��͍U���ł��Ȃ��B
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

    SoundEffect::get_ton()->play("res\\sound\\attack01.wav", 90);
    m_AnimMesh2->SetAnim("Attack", 0.f);
    m_bAttack = true;

    D3DXVECTOR3 attackPos = GetAttackPos();
    D3DXVECTOR3 enemyPos { 0.f, 0.f, 0.f };
    std::vector<EnemyBase*> vecEnemy = SharedObj::GetMap()->GetEnemy();

    for (std::size_t i = 0; i < vecEnemy.size(); i++)
    {
        // �����̗�͈Ŗ��@�ł����_���[�W��^�����Ȃ�
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

            // �������������x�ŉ���B�΂�������
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
    // �C���x���g������|��̖������炷�B
    auto inventory = NSStarmanLib::Inventory::GetObj();
    auto itemManager = NSStarmanLib::ItemManager::GetObj();

    // �����l�̋�����̂ق�����Ȃ��Ȃ�
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

    m_AnimMesh2->SetAnim("Arrow", 0.f);

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

    // ��𓊂�����̂Ƃ��ăZ�b�g
    {
        D3DXVECTOR3 pos(m_loadingPos);
        pos.y += 0.85f;
        pos.x += std::sin(m_rotate.y + D3DX_PI) * 0.5f;
        pos.z += std::sin(m_rotate.y + (D3DX_PI * 3 / 2)) * 0.5f;

        D3DXVECTOR3 norm(0.f, 0.f, 0.f);
        norm.x = std::sin(m_rotate.y + D3DX_PI);
        norm.z = std::sin(m_rotate.y + (D3DX_PI * 3 / 2));

        norm *= 0.1f + (bowLevel * 0.1f) + (arrowLevel * 0.2f); // TODO ������
        norm.y = 0.05f;

        float power = 0.f;

        // TODO ������
        power = 0.01f;
        power += (bowLevel * 0.2f) + (arrowLevel * 10.f);

        SharedObj::GetMap()->AddThrownItem(pos, norm, Common::LoadString_(IDS_STRING143), itemInfo, 1.f, power, m_rotate.y);
    }

    // �̗͂����Ղ���
    {
        auto statusManager = NSStarmanLib::StatusManager::GetObj();
        statusManager->ConsumeAttackCost();
    }
    return true;
}

bool Player::SetAttackAtlatl()
{
    // �C���x���g������A�g���g���̖������炷�B
    auto inventory = NSStarmanLib::Inventory::GetObj();
    auto itemManager = NSStarmanLib::ItemManager::GetObj();

    // �����l�̋������̂ق�����Ȃ��Ȃ�
    // �����l��-1,1,2,3,4,5��6���
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

    // �C���x���g�����瑄������炷
    {
        itemDef = itemManager->GetItemDef(Common::LoadString_(IDS_STRING144), arrowLevel);
        auto subIdList = inventory->GetSubIdList(itemDef.GetId());
        itemInfo = inventory->GetItemInfo(itemDef.GetId(), subIdList.at(0));
        inventory->RemoveItem(itemDef.GetId(), subIdList.at(0));
    }

    m_AnimMesh2->SetAnim("Attack", 0.f);

    m_bAttack = true;

    // ��𓊂�����̂Ƃ��ăZ�b�g
    {
        // �����ʒu
        D3DXVECTOR3 pos(m_loadingPos);
        pos.y += 1.15f;
        pos.x += std::sin(m_rotate.y + D3DX_PI) * 0.5f;
        pos.z += std::sin(m_rotate.y + (D3DX_PI * 3 / 2)) * 0.5f;

        // �����Ƒ��x
        D3DXVECTOR3 norm(0.f, 0.f, 0.f);
        norm.x = std::sin(m_rotate.y + D3DX_PI);
        norm.z = std::sin(m_rotate.y + (D3DX_PI * 3 / 2));

        norm *= 0.1f + (arrowLevel * 0.2f); // TODO ������
        norm.y = 0.05f;

        float power = 0.f;

        // TODO ������
        power = 20.f;
        power += (arrowLevel * 5.f);

        SharedObj::GetMap()->AddThrownItem(pos, norm, Common::LoadString_(IDS_STRING144), itemInfo, 2.f, power, m_rotate.y);
    }

    // �̗͂����Ղ���
    {
        auto statusManager = NSStarmanLib::StatusManager::GetObj();
        statusManager->ConsumeAttackCost();
    }
    return true;
}

void Player::SetWalk()
{
    auto status = NSStarmanLib::StatusManager::GetObj();
    if (m_bJump == false &&
        status->GetDead() == false &&
        m_bAttack == false)
    {
        if (m_bUnderwater == false)
        {
            m_AnimMesh2->SetAnim("Walk");
            status->SetPlayerAction(NSStarmanLib::StatusManager::PlayerState::JOGGING);
        }
        else
        {
            m_AnimMesh2->SetAnim("Swim");
            status->SetPlayerAction(NSStarmanLib::StatusManager::PlayerState::SWIM);
        }
    }
}

void Player::SetIdle()
{
    m_AnimMesh2->SetAnim("Idle");
    Common::Status()->SetPlayerAction(NSStarmanLib::StatusManager::PlayerState::STAND);
}

void Player::SetDamaged()
{
    if (NSStarmanLib::StatusManager::GetObj()->GetDead() == false)
    {
        SoundEffect::get_ton()->play("res\\sound\\damage01.wav", 90);
        m_AnimMesh2->SetAnim("Damaged", 0.f);
        m_bDamaged = true;
    }

    // �U���H��������
    if (Common::StrongMode() && Common::DebugMode())
    {
        Common::Status()->SetMuscleCurrent(100);
        Common::Status()->SetBodyStaminaCurrent(100);
        Common::Status()->SetBrainStaminaCurrent(100);
        Common::Status()->SetWaterCurrent(100);
    }

    // �������ɍU�����ꂽ�瑦��
    if (NSStarmanLib::StatusManager::GetObj()->GetSleep())
    {
        NSStarmanLib::StatusManager::GetObj()->SetDead(true);
        NSStarmanLib::StatusManager::GetObj()->SetDeadReason(NSStarmanLib::eDeadReason::ATTACK_ON_SLEEP);
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

        // �V���ɑϋv�l��0�ɂȂ����܂���������
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

    m_AnimMesh2->SetAnim("Sit");
}

bool Player::IsInCave()
{
    return m_bInCave;
}

void Player::Throw()
{
    //-------------------------------------------------------------
    // 1. �f��ɂȂ�B
    // 2. ���郂�[�V�������Đ�
    // 3. �����A�C�e�������ォ��O���ɔ��ł����B
    // 4. �C���x���g���ɓ���̃A�C�e��������ꍇ�A�ēx���������B�Ȃ���Αf��ɂȂ�B
    //
    // �E������ꂽ���͕̂��ɏՓ˂��Ē�~����B
    // �E�G�ɂ��������ꍇ�́A�_���[�W��^���A�n�ʂɗ�����B
    // �E�n�ʂɗ��������͏E����B
    // �E�����铮���2�b�Ɉ��
    // �E�̗͂����Ղ���
    //-------------------------------------------------------------

    auto statusManager = NSStarmanLib::StatusManager::GetObj();

    // �f�肾�����牽�����Ȃ��B
    auto itemInfo = statusManager->GetEquipWeapon();
    if (itemInfo.GetId() == -1)
    {
        return;
    }

    // �����Ă���Œ��Ȃ牽�����Ȃ�
    if (m_bThrow)
    {
        return;
    }

    // �C���x���g�������A�C�e�������炷�B
    auto inventory = NSStarmanLib::Inventory::GetObj();
    inventory->RemoveItem(itemInfo.GetId(), itemInfo.GetSubId());

    // ��������̂��Z�b�g
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
        std::string xfilename = weaponManager->GetXfilename(itemDef.GetName());

        if (xfilename.find("rock") != std::string::npos)
        {
            SharedObj::GetMap()->AddThrownItem(pos, norm, itemDef.GetName(), itemInfo, 0.1f);
        }
        else
        {
            SharedObj::GetMap()->AddThrownItem(pos, norm, itemDef.GetName(), itemInfo);
        }

        m_throwItemId = itemInfo.GetId();
    }

    // �f��ɂ���
    {
        NSStarmanLib::ItemInfo itemInfo;
        itemInfo.SetId(-1);
        statusManager->SetEquipWeapon(itemInfo);

        SoundEffect::get_ton()->play("res\\sound\\attack01.wav", 90);
        m_AnimMesh2->SetAnim("Attack", 0.f);
    }

    // �̗͂����Ղ���
    statusManager->ConsumeAttackCost();
}

void Player::SetMagic()
{
    //-------------------------------------------------------------
    // 1. ���郂�[�V�������Đ�
    // 2. �΂̋ʂ����ォ��O���ɔ��ł����B
    //
    // �E�����铮���2�b�Ɉ��
    // �E�̗͂����Ղ���
    //-------------------------------------------------------------

    auto statusManager = NSStarmanLib::StatusManager::GetObj();

    auto magicType = statusManager->GetMagicType();
    if (magicType == NSStarmanLib::eMagicType::None)
    {
        return;
    }

    // �����Ă���Œ��Ȃ牽�����Ȃ�
    if (m_bMagic)
    {
        return;
    }

    // ��������̂��Z�b�g
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
        SoundEffect::get_ton()->play("res\\sound\\fireSet.wav");
    }
    else if (magicType == NSStarmanLib::eMagicType::Ice)
    {
        SoundEffect::get_ton()->play("res\\sound\\iceSet.wav");
    }
    else if (magicType == NSStarmanLib::eMagicType::Dark)
    {
        SoundEffect::get_ton()->play("res\\sound\\darkSet.wav");
    }

    m_AnimMesh2->SetAnim("Magic", 0.f);

    // �̗͂����Ղ���
    statusManager->UseMagic();
}


