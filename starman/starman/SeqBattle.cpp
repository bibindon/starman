// F7�L�[�ŃT���v���e�L�X�g

#include "SeqBattle.h"
#include <fstream>
#include "Common.h"
#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif
#include <dinput.h>
#include "KeyBoard.h"
#include "Mouse.h"
#include "Camera.h"
#include "GamePad.h"
#include "Light.h"
#include "BGM.h"
#include "SharedObj.h"
#include "PopUp.h"
#include "SoundEffect.h"
#include "EnemySphere.h"
#include "EnemyDisk.h"
#include "PopUp2.h"
#include "SaveManager.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/MapObjManager.h"
#include "NpcManager.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/Rynen.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/Help.h"
#include "QuestManager.h"
#include "Rain.h"
#include <cassert>
#include "resource.h"

using namespace NSQuestSystem;

namespace NSStorehouseLib
{

    class Sprite : public ISprite
    {
    public:

        Sprite(LPDIRECT3DDEVICE9 dev)
            : m_pD3DDevice(dev)
        {
        }

        void DrawImage(const int x, const int y, const int transparency) override
        {
            D3DXVECTOR3 pos{ (float)x, (float)y, 0.f };
            m_D3DSprite->Begin(D3DXSPRITE_ALPHABLEND);
            RECT rect = {
                0,
                0,
                static_cast<LONG>(m_width),
                static_cast<LONG>(m_height) };
            D3DXVECTOR3 center{ 0, 0, 0 };
            m_D3DSprite->Draw(
                m_pD3DTexture,
                &rect,
                &center,
                &pos,
                D3DCOLOR_ARGB(transparency, 255, 255, 255));
            m_D3DSprite->End();

        }

        void Load(const std::string& filepath) override
        {
            LPD3DXSPRITE tempSprite{ nullptr };
            if (FAILED(D3DXCreateSprite(m_pD3DDevice, &m_D3DSprite)))
            {
                throw std::exception("Failed to create a sprite.");
            }

            if (FAILED(D3DXCreateTextureFromFile(
                m_pD3DDevice,
                filepath.c_str(),
                &m_pD3DTexture)))
            {
                throw std::exception("Failed to create a texture.");
            }

            D3DSURFACE_DESC desc{ };
            if (FAILED(m_pD3DTexture->GetLevelDesc(0, &desc)))
            {
                throw std::exception("Failed to create a texture.");
            }
            m_width = desc.Width;
            m_height = desc.Height;
        }

        ~Sprite()
        {
            SAFE_RELEASE(m_D3DSprite);
            SAFE_RELEASE(m_pD3DTexture);
        }

    private:

        LPDIRECT3DDEVICE9 m_pD3DDevice = NULL;
        LPD3DXSPRITE m_D3DSprite = NULL;
        LPDIRECT3DTEXTURE9 m_pD3DTexture = NULL;
        UINT m_width{ 0 };
        UINT m_height{ 0 };
    };

    class Font : public IFont
    {
    public:

        Font(LPDIRECT3DDEVICE9 pD3DDevice)
            : m_pD3DDevice(pD3DDevice)
        {
        }

        void Init(bool bEnglish)
        {
            HRESULT hr = E_FAIL;

            if (!bEnglish)
            {
                hr = D3DXCreateFont(m_pD3DDevice,
                                    24,
                                    0,
                                    FW_NORMAL,
                                    1,
                                    false,
                                    SHIFTJIS_CHARSET,
                                    OUT_TT_ONLY_PRECIS,
                                    ANTIALIASED_QUALITY,
                                    FF_DONTCARE,
                                    "�l�r ����",
                                    &m_pFont);
            }
            else
            {
                hr = D3DXCreateFont(m_pD3DDevice,
                                    24,
                                    0,
                                    FW_NORMAL,
                                    1,
                                    false,
                                    DEFAULT_CHARSET,
                                    OUT_TT_ONLY_PRECIS,
                                    CLEARTYPE_NATURAL_QUALITY,
                                    FF_DONTCARE,
                                    "Courier New",
                                    &m_pFont);
            }

            assert(hr == S_OK);
        }

        virtual void DrawText_(const std::string& msg, const int x, const int y, const int transparency)
        {
            RECT rect = { x, y, 0, 0 };
            m_pFont->DrawText(NULL, msg.c_str(), -1, &rect, DT_LEFT | DT_NOCLIP,
                D3DCOLOR_ARGB(transparency, 255, 255, 255));
        }

        ~Font()
        {
            m_pFont->Release();
        }

    private:

        LPDIRECT3DDEVICE9 m_pD3DDevice = NULL;
        LPD3DXFONT m_pFont = NULL;
    };


    class SoundEffect : public ISoundEffect
    {
        virtual void PlayMove() override
        {
            ::SoundEffect::get_ton()->play("res\\sound\\menu_cursor_move.wav");
        }
        virtual void PlayClick() override
        {
            ::SoundEffect::get_ton()->play("res\\sound\\menu_cursor_confirm.wav");
        }
        virtual void PlayBack() override
        {
            ::SoundEffect::get_ton()->play("res\\sound\\menu_cursor_cancel.wav");
        }
        virtual void Init() override
        {
            ::SoundEffect::get_ton()->load("res\\sound\\menu_cursor_move.wav");
            ::SoundEffect::get_ton()->load("res\\sound\\menu_cursor_confirm.wav");
            ::SoundEffect::get_ton()->load("res\\sound\\menu_cursor_cancel.wav");
        }
    };
}

namespace NSTalkLib2
{

class Sprite : public ISprite
{
public:
    Sprite(LPDIRECT3DDEVICE9 dev)
        : m_pD3DDevice(dev)
    {
    }

    void DrawImage(const int x, const int y, const int transparency) override
    {
        if (m_filepath.empty())
        {
            return;
        }

        if (m_D3DSprite == nullptr)
        {
            return;
        }

        D3DXVECTOR3 pos { (float)x, (float)y, 0.f };
        m_D3DSprite->Begin(D3DXSPRITE_ALPHABLEND);
        RECT rect = { 0,
                      0,
                      static_cast<LONG>(m_width),
                      static_cast<LONG>(m_height) };
        D3DXVECTOR3 center { 0, 0, 0 };
        m_D3DSprite->Draw(m_texMap.at(m_filepath), &rect, &center, &pos, D3DCOLOR_ARGB(transparency, 255, 255, 255));
        m_D3DSprite->End();

    }

    void Load(const std::string& filepath) override
    {
        if (filepath.empty())
        {
            return;
        }

        // �X�v���C�g�͈�̂݊m�ۂ��g���܂킷
        if (m_D3DSprite == NULL)
        {
            if (FAILED(D3DXCreateSprite(m_pD3DDevice, &m_D3DSprite)))
            {
                throw std::exception("Failed to create a sprite.");
            }
        }

        m_filepath = filepath;

        // �����摜�t�@�C���ō��ꂽ�e�N�X�`�������ɂ���Ȃ�A
        // �摜�̃T�C�Y�����m�ۂ��e�N�X�`���̍쐬���s��Ȃ�
        auto it = m_texMap.find(filepath);
        if (it != m_texMap.end())
        {
            D3DSURFACE_DESC desc{ };
            if (FAILED(m_texMap.at(m_filepath)->GetLevelDesc(0, &desc)))
            {
                throw std::exception("Failed to create a texture.");
            }
            m_width = desc.Width;
            m_height = desc.Height;
            it->second->AddRef();
            return;
        }

        // �e�N�X�`���̍쐬
        LPDIRECT3DTEXTURE9 pD3DTexture = NULL;
        HRESULT hr = D3DXCreateTextureFromFile(m_pD3DDevice, filepath.c_str(), &pD3DTexture);
        if (FAILED(hr))
        {
            std::string work;
            work = "Failed to create a texture. HRESULT: " + std::to_string(hr);
            throw std::exception(work.c_str());
        }

        m_texMap[filepath] = pD3DTexture;


        D3DSURFACE_DESC desc{ };
        if (FAILED(pD3DTexture->GetLevelDesc(0, &desc)))
        {
            throw std::exception("Failed to create a texture.");
        }
        m_width = desc.Width;
        m_height = desc.Height;
    }

    ~Sprite() override
    {
        if (m_filepath.empty())
        {
            return;
        }

        ULONG refCnt = m_texMap.at(m_filepath)->Release();
        if (refCnt == 0)
        {
            m_texMap.erase(m_filepath);
        }

        if (m_texMap.empty())
        {
            SAFE_RELEASE(m_D3DSprite);
        }
    }

    virtual ISprite* Create() override
    {
        return NEW Sprite(m_pD3DDevice);
    }
private:
    LPDIRECT3DDEVICE9 m_pD3DDevice = NULL;

    UINT m_width { 0 };
    UINT m_height { 0 };

    // �X�v���C�g�͈���g���܂킷
    static LPD3DXSPRITE m_D3DSprite;

    std::string m_filepath;

    // �������O�̉摜�t�@�C���ō��ꂽ�e�N�X�`���͎g���܂킷
    static std::unordered_map<std::string, LPDIRECT3DTEXTURE9> m_texMap;
};
 
LPD3DXSPRITE Sprite::m_D3DSprite = NULL;
std::unordered_map<std::string, LPDIRECT3DTEXTURE9> Sprite::m_texMap;

class Font : public IFont
{
public:
    Font(LPDIRECT3DDEVICE9 pD3DDevice)
        : m_pD3DDevice(pD3DDevice)
    {
    }

    void Init(const bool bEnglish)
    {
        HRESULT hr = S_FALSE;

        if (!bEnglish)
        {
            hr = D3DXCreateFont(m_pD3DDevice,
                                24,
                                0,
                                FW_NORMAL,
                                1,
                                false,
                                SHIFTJIS_CHARSET,
                                OUT_TT_ONLY_PRECIS,
                                ANTIALIASED_QUALITY,
                                FF_DONTCARE,
                                "�l�r ����",
                                &m_pFont);
        }
        else
        {
            hr = D3DXCreateFont(m_pD3DDevice,
                                24,
                                0,
                                FW_NORMAL,
                                1,
                                false,
                                DEFAULT_CHARSET,
                                OUT_TT_ONLY_PRECIS,
                                CLEARTYPE_NATURAL_QUALITY,
                                FF_DONTCARE,
                                "Courier New",
                                &m_pFont);
        }

        assert(hr == S_OK);
    }

    virtual void DrawText_(const std::string& msg, const int x, const int y)
    {
        RECT rect = { x, y, 0, 0 };
        m_pFont->DrawText(NULL, msg.c_str(), -1, &rect, DT_LEFT | DT_NOCLIP,
            D3DCOLOR_ARGB(255, 255, 255, 255));
    }

    ~Font() override
    {
        m_pFont->Release();
        m_pFont = nullptr;
    }

private:
    LPDIRECT3DDEVICE9 m_pD3DDevice = NULL;
    LPD3DXFONT m_pFont = NULL;
};


class SoundEffect : public ISoundEffect
{
    void Init() override
    {
        ::SoundEffect::get_ton()->load("res\\sound\\message1.wav");
    }

    void PlayMessage() override
    {
        ::SoundEffect::get_ton()->play("res\\sound\\message1.wav");
    }

    void Stop() override
    {
        ::SoundEffect::get_ton()->stop("res\\sound\\message1.wav");
    }
};

}

SeqBattle::SeqBattle()
{
    D3DXVECTOR3 b = D3DXVECTOR3(0, 0.f, 0);
    D3DXVECTOR3 c = D3DXVECTOR3(0, 0, 0);
    m_player = NEW Player();
    SharedObj::SetPlayer(m_player);

    D3DXVECTOR3 pos = D3DXVECTOR3(6.f, 0.f, 10.f);

    BGM::get_ton()->StopAll();
    BGM::get_ton()->load("res\\sound\\title.wav");
    BGM::get_ton()->play("res\\sound\\title.wav", 10);

    {
        BGM::get_ton()->load("res\\sound\\forest.wav");
        BGM::get_ton()->play("res\\sound\\forest.wav", 10);

        BGM::get_ton()->load("res\\sound\\sea.wav");
    }

    ::SoundEffect::get_ton()->load("res\\sound\\drink.wav");

    m_spriteGameover = NEW Sprite("res\\image\\gameover.png");
    m_spriteExamine = NEW Sprite("res\\image\\examine.png");

    NSTalkLib2::IFont* pFont = NEW NSTalkLib2::Font(SharedObj::GetD3DDevice());
    NSTalkLib2::ISoundEffect* pSE = NEW NSTalkLib2::SoundEffect();
    NSTalkLib2::ISprite* sprite = NEW NSTalkLib2::Sprite(SharedObj::GetD3DDevice());

    m_hudManager.Init();

    Common::SetCursorVisibility(false);

    m_eState = eBattleState::LOAD;
    InitLoad();

    m_sprBlack = NEW Sprite("res\\image\\black.png");

    if (Common::DebugMode())
    {
        NSTalkLib2::Talk::SetFastMode(true);
        NSStoryTelling::StoryTelling::SetFastMode(true);
    }

    m_commandManager.Init(CommandManager::eType::Main);

    m_craft.Init();
}

SeqBattle::~SeqBattle()
{
    m_loadThread->join();

    SAFE_DELETE(m_menuManager);
    m_hudManager.Finalize();
    m_commandManager.Finalize();
    SAFE_DELETE(m_loadThread);
    SAFE_DELETE(m_player);
    SAFE_DELETE(m_map);
    SAFE_DELETE(m_enemyManager);

    SAFE_DELETE(m_talk);
    SAFE_DELETE(m_storehouse);

    m_craft.Finalize();

    SAFE_DELETE(m_title);
    SAFE_DELETE(m_Opening);

    SAFE_DELETE(m_spriteGameover);
    SAFE_DELETE(m_spriteExamine);

    SAFE_DELETE(m_sprLoadClock);
    SAFE_DELETE(m_sprLoadLoading);

    SharedObj::Voyage()->Finalize();
    m_patchManager2.Finalize();
}

void SeqBattle::Update(eSequence* sequence)
{
    UpdateCommon();

    if (m_eState == eBattleState::LOAD)
    {
        UpdateLoad();
    }
    else if (m_eState == eBattleState::STORY)
    {
        OperateStory();
    }
    else if (m_eState == eBattleState::MENU)
    {
        OperateMenu(sequence);
    }
    else if (m_eState == eBattleState::PATCH_TEST)
    {
        OperatePatch();
    }
    else if (m_eState == eBattleState::OPENING)
    {
        OperateOpening();
    }
    else if (m_eState == eBattleState::TALK)
    {
        OperateTalk();
    }
    else if (m_eState == eBattleState::STOREHOUSE)
    {
        OperateStorehouse();
    }
    else if (m_eState == eBattleState::COMMAND)
    {
        OperateCommand();
    }
    else if (m_eState == eBattleState::NORMAL)
    {
        OperateNormal(sequence);
    }
    else if (m_eState == eBattleState::TITLE)
    {
        OperateTitle(sequence);
    }
    else if (m_eState == eBattleState::DEAD)
    {
        OperateGameover(sequence);
    }
    else if (m_eState == eBattleState::SLEEP)
    {
        OperateSleep();
    }
    else if (m_eState == eBattleState::CUT_TREE)
    {
        OperateCutTree();
    }
    else if (m_eState == eBattleState::PICK_PLANT)
    {
        OperatePickPlant();
    }
    else if (m_eState == eBattleState::VOYAGE)
    {
        OperateVoyage();
    }
    else if (m_eState == eBattleState::VOYAGE3HOURS)
    {
        OperateVoyage3Hours();
    }
    else if (m_eState == eBattleState::CREATE_TORCH)
    {
        OperateCreateTorch();
    }
    else if (m_eState == eBattleState::POPUP)
    {
        OperatePopUp();
    }
    else if (m_eState == eBattleState::CRAFT)
    {
        OperateCraft();
    }

    //-----------------------------------------------------
    // �J�����̍X�V����
    //
    // �J�����̍X�V������Update�֐��̍Ō�Ɏ��{����K�v������B
    // OperateVoyage�֐��̂悤�Ƀv���C���[�̈ʒu��ύX����֐�������A
    // ���̏ꍇ�́A�v���C���[�̈ʒu���ύX����Ă���J�����̍X�V���s���K�v�����邽�߁B
    //-----------------------------------------------------
    {
        D3DXVECTOR3 pos = m_player->GetPos();
        pos.y += 1.f;
        Camera::SetLookAtPos(pos);
        Camera::Update();
    }

    if (Common::DebugMode())
    {
        UpdateDebug();
    }
}


void SeqBattle::OperateStory()
{
    m_story->Update();

    if (m_story->IsFinish())
    {
        SAFE_DELETE(m_story);
        m_eState = eBattleState::NORMAL;
    }
}

void SeqBattle::OperateMenu(eSequence* sequence)
{
    std::string result = m_menuManager->OperateMenu();
    if (result == "Title")
    {
        m_eState = eBattleState::TITLE;
        Camera::SetCameraMode(eCameraMode::BATTLE_TO_TITLE);
        Common::SetCursorVisibility(true);
        m_title = NEW Title(false, false);
    }
    else if (result == "EXIT")
    {
        m_eState = eBattleState::NORMAL;
        Camera::SetCameraMode(eCameraMode::BATTLE);
        Common::SetCursorVisibility(false);
    }
}

void SeqBattle::OperateTalk()
{
    if (m_talk != nullptr)
    {
        bool talkFinish = m_talk->Update();
        if (talkFinish)
        {
            SAFE_DELETE(m_talk);
            m_eState = eBattleState::NORMAL;
            Common::SetCursorVisibility(false);
            Rain::Get()->SetShow(true);
            return;
        }
    }

    // �b��I�Ώ�
    // ���{�I�Ȍ��ׂ�����
    // �}�b�v�N���X�ɓ�̃��[�h������ׂ�
    Rain::Get()->SetShow(false);

    if (SharedObj::KeyBoard()->IsDownFirstFrame(DIK_SPACE))
    {
        m_talk->Next();
    }

    if (SharedObj::KeyBoard()->IsDownFirstFrame(DIK_RETURN))
    {
        m_talk->Next();
    }

    if (Mouse::IsDownLeft())
    {
        m_talk->Next();
    }

    if (GamePad::IsDown(eGamePadButtonType::A))
    {
        m_talk->Next();
    }
}

void SeqBattle::OperateStorehouse()
{
    std::string result;

    //---------------------------------------------------------
    // KeyBoard
    //---------------------------------------------------------

    if (Common::DebugMode() || Common::ReleaseMode())
    {
        if (SharedObj::KeyBoard()->IsDownFirstFrame(DIK_F1))
        {
            m_eState = eBattleState::STOREHOUSE;
            Camera::SetCameraMode(eCameraMode::SLEEP);
            Common::SetCursorVisibility(false);
        }
    }

    if (SharedObj::KeyBoard()->IsDownFirstFrame(DIK_UP))
    {
        m_storehouse->Up();
    }

    if (SharedObj::KeyBoard()->IsHold(DIK_UP))
    {
        m_storehouse->Up();
    }

    if (SharedObj::KeyBoard()->IsDownFirstFrame(DIK_DOWN))
    {
        m_storehouse->Down();
    }

    if (SharedObj::KeyBoard()->IsHold(DIK_DOWN))
    {
        m_storehouse->Down();
    }

    if (SharedObj::KeyBoard()->IsDownFirstFrame(DIK_LEFT))
    {
        m_storehouse->Left();
    }

    if (SharedObj::KeyBoard()->IsDownFirstFrame(DIK_RIGHT))
    {
        m_storehouse->Right();
    }

    if (SharedObj::KeyBoard()->IsDownFirstFrame(DIK_RETURN))
    {
        result = m_storehouse->Into();
        std::vector<std::string> vs = Common::split(result, ':');

        int id_ = 0;
        int subId_ = 0;
        int durability_ = 0;

        id_ = std::stoi(vs.at(2));
        subId_ = std::stoi(vs.at(3));

        NSStarmanLib::Inventory* inventory = NSStarmanLib::Inventory::GetObj();

        // �q�ɂ̕������Ή�
        float x_ = SharedObj::GetPlayer()->GetPos().x;
        float z_ = SharedObj::GetPlayer()->GetPos().z;
        auto storehouse = NSStarmanLib::StorehouseManager::Get()->GetNearStorehouse(x_, z_);

        if (Common::DebugMode() || Common::ReleaseMode())
        {
            storehouse = NSStarmanLib::StorehouseManager::Get()->GetStorehouse(1);
        }

        if (storehouse != nullptr)
        {
            if (vs.at(0) == "left")
            {
                bool equipBagExist = false;
                bool equipWeaponExist = false;

                // �������̑܂�������폜�ł��Ȃ��悤�ɂ���
                {
                    auto allBag = Common::Status()->GetAllBag();
                    for (auto it = allBag.begin(); it != allBag.end(); ++it)
                    {
                        if (it->GetId() == id_ && it->GetSubId() == subId_)
                        {
                            PopUp2::Get()->SetText(IDS_STRING102);
                            equipBagExist = true;
                            break;
                        }
                    }
                }

                // �������̕��킾������폜�ł��Ȃ��悤�ɂ���
                {
                    auto weapon = Common::Status()->GetEquipWeapon();
                    if (weapon.GetId() == id_ && weapon.GetSubId() == subId_)
                    {
                        PopUp2::Get()->SetText(IDS_STRING103);
                        equipWeaponExist = true;
                    }
                }
                
                // �������̑܂ł͂Ȃ����A�������̕���ł��Ȃ��B
                if (!equipBagExist && !equipWeaponExist)
                {
                    NSStarmanLib::ItemInfo itemInfo = inventory->GetItemInfo(id_, subId_);
                    durability_ = itemInfo.GetDurabilityCurrent();
                    inventory->RemoveItem(id_, subId_);
                    storehouse->AddItemWithSubID(id_, subId_, durability_);
                    m_storehouse->MoveFromInventoryToStorehouse(id_, subId_);
                    m_menuManager->DeleteItem(id_, subId_);
                }
            }
            else if (vs.at(0) == "right")
            {
                NSStarmanLib::ItemInfo itemInfo = storehouse->GetItemInfo(id_, subId_);
                durability_ = itemInfo.GetDurabilityCurrent();
                storehouse->RemoveItem(id_, subId_);
                inventory->AddItemWithSubID(id_, subId_, durability_);
                m_storehouse->MoveFromStorehouseToInventory(id_, subId_);
                m_menuManager->AddItem(id_, subId_, durability_);
            }
            else
            {
                throw std::exception();
            }
        }
    }

    if (SharedObj::KeyBoard()->IsDownFirstFrame(DIK_ESCAPE))
    {
        result = m_storehouse->Back();
        m_eState = eBattleState::NORMAL;
        Camera::SetCameraMode(eCameraMode::BATTLE);
        Common::SetCursorVisibility(false);
    }

    //---------------------------------------------------------
    // Mouse
    //---------------------------------------------------------

    if (Mouse::IsDownLeft())
    {
        POINT p = Common::GetScreenPos();;
        result = m_storehouse->Click(p.x, p.y);

        std::vector<std::string> vs = Common::split(result, ':');

        if (!vs.empty())
        {
            int id_ = 0;
            int subId_ = 0;
            int durability_ = 0;

            id_ = std::stoi(vs.at(2));
            subId_ = std::stoi(vs.at(3));

            NSStarmanLib::Inventory* inventory = NSStarmanLib::Inventory::GetObj();

            // �q�ɂ̕������Ή�
            float x_ = SharedObj::GetPlayer()->GetPos().x;
            float z_ = SharedObj::GetPlayer()->GetPos().z;
            auto storehouse = NSStarmanLib::StorehouseManager::Get()->GetNearStorehouse(x_, z_);

            if (Common::DebugMode() || Common::ReleaseMode())
            {
                storehouse = NSStarmanLib::StorehouseManager::Get()->GetStorehouse(1);
            }

            if (storehouse != nullptr)
            {
                if (vs.at(0) == "left")
                {
                    bool equipBagExist = false;
                    bool equipWeaponExist = false;

                    // �������̑܂�������폜�ł��Ȃ��悤�ɂ���
                    {
                        auto allBag = Common::Status()->GetAllBag();
                        for (auto it = allBag.begin(); it != allBag.end(); ++it)
                        {
                            if (it->GetId() == id_ && it->GetSubId() == subId_)
                            {
                                PopUp2::Get()->SetText(IDS_STRING102);
                                equipBagExist = true;
                                break;
                            }
                        }
                    }

                    // �������̕��킾������폜�ł��Ȃ��悤�ɂ���
                    {
                        auto weapon = Common::Status()->GetEquipWeapon();
                        if (weapon.GetId() == id_ && weapon.GetSubId() == subId_)
                        {
                            PopUp2::Get()->SetText(IDS_STRING103);
                            equipWeaponExist = true;
                        }
                    }
                    
                    // �������̑܂ł͂Ȃ����A�������̕���ł��Ȃ��B
                    if (!equipBagExist && !equipWeaponExist)
                    {
                        NSStarmanLib::ItemInfo itemInfo = inventory->GetItemInfo(id_, subId_);
                        durability_ = itemInfo.GetDurabilityCurrent();
                        inventory->RemoveItem(id_, subId_);
                        storehouse->AddItemWithSubID(id_, subId_, durability_);
                        m_storehouse->MoveFromInventoryToStorehouse(id_, subId_);
                        m_menuManager->DeleteItem(id_, subId_);
                    }
                }
                else if (vs.at(0) == "right")
                {
                    NSStarmanLib::ItemInfo itemInfo = storehouse->GetItemInfo(id_, subId_);
                    durability_ = itemInfo.GetDurabilityCurrent();
                    storehouse->RemoveItem(id_, subId_);
                    inventory->AddItemWithSubID(id_, subId_, durability_);
                    m_storehouse->MoveFromStorehouseToInventory(id_, subId_);
                    m_menuManager->AddItem(id_, subId_, durability_);
                }
                else
                {
                    throw std::exception();
                }
            }
        }
    }
    else
    {
        static POINT previousPoint = { 0, 0 };
        POINT p = Common::GetScreenPos();;

        if (p.x == previousPoint.x &&
            p.y == previousPoint.y)
        {
            // do nothing
        }
        else
        {
            m_storehouse->CursorOn(p.x, p.y);
        }

        previousPoint = p;
    }

    if (Mouse::GetZDelta() < 0)
    {
        m_storehouse->Next();
    }
    else if (Mouse::GetZDelta() > 0)
    {
        m_storehouse->Previous();
    }

    //---------------------------------------------------------
    // GamePad
    //---------------------------------------------------------

    if (GamePad::IsDown(eGamePadButtonType::UP))
    {
        m_storehouse->Up();
    }

    if (GamePad::IsDown(eGamePadButtonType::DOWN))
    {
        m_storehouse->Down();
    }

    if (GamePad::IsDown(eGamePadButtonType::LEFT))
    {
        m_storehouse->Left();
    }

    if (GamePad::IsDown(eGamePadButtonType::RIGHT))
    {
        m_storehouse->Right();
    }

    if (GamePad::IsDown(eGamePadButtonType::A))
    {
        result = m_storehouse->Into();
        std::vector<std::string> vs = Common::split(result, ':');

        int id_ = 0;
        int subId_ = 0;
        int durability_ = 0;

        id_ = std::stoi(vs.at(2));
        subId_ = std::stoi(vs.at(3));

        NSStarmanLib::Inventory* inventory = NSStarmanLib::Inventory::GetObj();

        // �q�ɂ̕������Ή�
        float x_ = SharedObj::GetPlayer()->GetPos().x;
        float z_ = SharedObj::GetPlayer()->GetPos().z;
        auto storehouse = NSStarmanLib::StorehouseManager::Get()->GetNearStorehouse(x_, z_);

        if (Common::DebugMode() || Common::ReleaseMode())
        {
            storehouse = NSStarmanLib::StorehouseManager::Get()->GetStorehouse(1);
        }

        if (storehouse != nullptr)
        {
            if (vs.at(0) == "left")
            {
                bool equipBagExist = false;
                bool equipWeaponExist = false;

                // �������̑܂�������폜�ł��Ȃ��悤�ɂ���
                {
                    auto allBag = Common::Status()->GetAllBag();
                    for (auto it = allBag.begin(); it != allBag.end(); ++it)
                    {
                        if (it->GetId() == id_ && it->GetSubId() == subId_)
                        {
                            PopUp2::Get()->SetText(IDS_STRING102);
                            equipBagExist = true;
                            break;
                        }
                    }
                }

                // �������̕��킾������폜�ł��Ȃ��悤�ɂ���
                {
                    auto weapon = Common::Status()->GetEquipWeapon();
                    if (weapon.GetId() == id_ && weapon.GetSubId() == subId_)
                    {
                        PopUp2::Get()->SetText(IDS_STRING103);
                        equipWeaponExist = true;
                    }
                }
                
                // �������̑܂ł͂Ȃ����A�������̕���ł��Ȃ��B
                if (!equipBagExist && !equipWeaponExist)
                {
                    NSStarmanLib::ItemInfo itemInfo = inventory->GetItemInfo(id_, subId_);
                    durability_ = itemInfo.GetDurabilityCurrent();
                    inventory->RemoveItem(id_, subId_);
                    storehouse->AddItemWithSubID(id_, subId_, durability_);
                    m_storehouse->MoveFromInventoryToStorehouse(id_, subId_);
                    m_menuManager->DeleteItem(id_, subId_);
                }
            }
            else if (vs.at(0) == "right")
            {
                NSStarmanLib::ItemInfo itemInfo = storehouse->GetItemInfo(id_, subId_);
                durability_ = itemInfo.GetDurabilityCurrent();
                storehouse->RemoveItem(id_, subId_);
                inventory->AddItemWithSubID(id_, subId_, durability_);
                m_storehouse->MoveFromStorehouseToInventory(id_, subId_);
                m_menuManager->AddItem(id_, subId_, durability_);
            }
            else
            {
                throw std::exception();
            }
        }
    }

    if (GamePad::IsDown(eGamePadButtonType::B))
    {
        result = m_storehouse->Back();
        m_eState = eBattleState::NORMAL;
        Camera::SetCameraMode(eCameraMode::BATTLE);
        Common::SetCursorVisibility(false);
    }

    return;
}

void SeqBattle::ShowStorehouse()
{
    using namespace NSStarmanLib;
    // �q�ɂ͕������݂ł���B
    // �������߂��q�ɂ�ΏۂƂ���
    auto ppos = SharedObj::GetPlayer()->GetPos();
    auto storehouse = StorehouseManager::Get()->GetNearStorehouse(ppos.x, ppos.z);

    if (storehouse == nullptr)
    {
        return;
    }

    m_eState = eBattleState::STOREHOUSE;
    delete m_storehouse;

    Camera::SetCameraMode(eCameraMode::SLEEP);
    Common::SetCursorVisibility(true);

    // TODO �q�ɂ�\������x�ɑq�ɉ�ʂ����̂���߂�
    m_storehouse = NEW NSStorehouseLib::StorehouseLib();

    NSStorehouseLib::Sprite* sprCursor = NEW NSStorehouseLib::Sprite(SharedObj::GetD3DDevice());
    sprCursor->Load("res\\image\\menu_cursor.png");

    NSStorehouseLib::Sprite* sprBackground = NEW NSStorehouseLib::Sprite(SharedObj::GetD3DDevice());
    sprBackground->Load("res\\image\\background.png");

    NSStorehouseLib::IFont* pFont = NEW NSStorehouseLib::Font(SharedObj::GetD3DDevice());
    pFont->Init(SharedObj::IsEnglish());

    NSStorehouseLib::ISoundEffect* pSE = NEW NSStorehouseLib::SoundEffect();

    m_storehouse->Init(pFont, pSE, sprCursor, sprBackground, SharedObj::IsEnglish());
    {
        NSStarmanLib::Inventory* inventory = NSStarmanLib::Inventory::GetObj();
        NSStarmanLib::ItemManager* itemManager = NSStarmanLib::ItemManager::GetObj();

        std::vector<int> idList = itemManager->GetItemIdList();

        std::vector<NSStorehouseLib::StoreItem> itemInfoList;
        for (std::size_t i = 0; i < idList.size(); ++i)
        {
            NSStarmanLib::ItemDef itemDef = itemManager->GetItemDef(idList.at(i));
            std::vector<int> subIdList = inventory->GetSubIdList(idList.at(i));
            {
                for (std::size_t j = 0; j < subIdList.size(); ++j)
                {
                    std::string work_str;

                    NSStorehouseLib::StoreItem itemInfoG;

                    itemInfoG.SetName(itemDef.GetName());
                    itemInfoG.SetId(itemDef.GetId());
                    itemInfoG.SetSubId(subIdList.at(j));
                    itemInfoList.push_back(itemInfoG);
                }
            }
        }
        m_storehouse->SetInventoryList(itemInfoList);
    }
    {
        NSStarmanLib::ItemManager* itemManager = NSStarmanLib::ItemManager::GetObj();

        std::vector<int> idList = itemManager->GetItemIdList();

        std::vector<NSStorehouseLib::StoreItem> itemInfoList;
        for (std::size_t i = 0; i < idList.size(); ++i)
        {
            NSStarmanLib::ItemDef itemDef = itemManager->GetItemDef(idList.at(i));
            std::vector<int> subIdList = storehouse->GetSubIdList(idList.at(i));
            {
                for (std::size_t j = 0; j < subIdList.size(); ++j)
                {
                    std::string work_str;

                    NSStorehouseLib::StoreItem itemInfoG;

                    itemInfoG.SetName(itemDef.GetName());
                    itemInfoG.SetId(itemDef.GetId());
                    itemInfoG.SetSubId(subIdList.at(j));
                    itemInfoList.push_back(itemInfoG);
                }
            }
        }
        m_storehouse->SetStorehouseList(itemInfoList);
    }
}

void SeqBattle::OperateCraft()
{
    if (Common::DebugMode())
    {
        if (SharedObj::KeyBoard()->IsDownFirstFrame(DIK_F2))
        {
            m_eState = eBattleState::NORMAL;
            Camera::SetCameraMode(eCameraMode::BATTLE);
            Common::SetCursorVisibility(false);
        }
    }

    m_craft.Operate(&m_eState);

    return;
}

void SeqBattle::OperateCommand()
{
    std::string result = m_commandManager.Operate();

    bool leave = false;

    if (result == "EXIT")
    {
        leave = true;
        if (m_commandManager.GetPreviousState() == eBattleState::NORMAL)
        {
            m_eState = eBattleState::NORMAL;
        }
        else if (m_commandManager.GetPreviousState() == eBattleState::VOYAGE)
        {
            m_eState = eBattleState::VOYAGE;
        }
    }
    else if (result == "����")
    {
        leave = true;
        m_player->SetSit();
        m_eState = eBattleState::NORMAL;
    }
    else if (result == "���ɂȂ�")
    {
        leave = true;
        m_player->SetLieDown();
        m_eState = eBattleState::NORMAL;
    }
    else if (result == "�ґz")
    {
        leave = true;
        m_eState = eBattleState::POPUP;

        {
            std::vector<std::vector<std::string>> vvs;
            std::vector<std::string> vs;
            vs.push_back("");
            vs.push_back(Common::LoadString_(IDS_STRING104));
            vvs.push_back(vs);
            vs.clear();
            auto hint = QuestManager::Get()->GetHint();
            if (hint.empty())
            {
                vs.push_back("");
                vs.push_back(Common::LoadString_(IDS_STRING105));
            }
            else
            {
                vs.push_back("");
                vs.push_back(hint);
            }
            vvs.push_back(vs);
            PopUp::Get()->SetText(vvs);
        }
    }
    else if (result == "�E�o")
    {
        leave = true;
        D3DXVECTOR3 pos(-285.f, 16.f, 539.f);
        m_player->SetPos(pos);
        m_eState = eBattleState::NORMAL;
    }
    else if (result == "����")
    {
        // �΂��Ε��𑕔����Ă��Ȃ��Ɛ؂�Ȃ�
        auto status = NSStarmanLib::StatusManager::GetObj();
        auto itemInfo = status->GetEquipWeapon();

        std::string name;
        if (itemInfo.GetId() != -1 && itemInfo.GetId() != 0)
        {
            name = itemInfo.GetItemDef().GetName();
        }

        if (name != "�Ε�" && name != "�c���̐�")
        {
            PopUp2::Get()->SetText(IDS_STRING106);
        }
        else
        {
            if (status->GetBodyStaminaCurrent() < 30.f)
            {
                PopUp2::Get()->SetText(IDS_STRING106);
            }
            else if (status->GetWaterCurrent() < 0.95f)
            {
                PopUp2::Get()->SetText(IDS_STRING106);
            }
            else
            {
                leave = true;

                m_eState = eBattleState::CUT_TREE;
                StartFadeInOut();
            }
        }
    }
    else if (result == "�̎�")
    {
        auto status = NSStarmanLib::StatusManager::GetObj();
        auto stamina = status->GetBodyStaminaCurrent();
        if (stamina <= 16.f)
        {
            PopUp2::Get()->SetText(IDS_STRING107);
        }
        else
        {
            leave = true;

            m_eState = eBattleState::PICK_PLANT;
            StartFadeInOut();
        }
    }
    else if (result == "���𒣂�")
    {
        SharedObj::Voyage()->SetSail(true);
    }
    else if (result == "�������")
    {
        SharedObj::Voyage()->SetSail(false);
    }
    else if (result == "�R���ԑ���")
    {
        if (!SharedObj::Voyage()->Can3HoursAuto())
        {
            PopUp2::Get()->SetText(IDS_STRING108);
        }
        else
        {
            leave = true;
            StartFadeInOut();
            m_eState = eBattleState::VOYAGE3HOURS;
        }
    }
    else if (result == "�����オ��")
    {
        SharedObj::Voyage()->SetRaftMode(false);
        m_eState = eBattleState::NORMAL;
        SharedObj::GetPlayer()->SetIdle();

        BGM::get_ton()->StopAll();

        BGM::get_ton()->load("res\\sound\\field.wav");
        BGM::get_ton()->play("res\\sound\\field.wav", 10, true);
    }
    else if (result == "�C�J�_�ɏ��")
    {
        // �܂𑕔����Ă�����C�J�_�ɏ�邱�Ƃ͂ł��Ȃ�
        auto bagState = Common::Status()->GetBagState();
        if (bagState.empty())
        {
            m_eState = eBattleState::VOYAGE;
            VoyageManager::Get()->Ride();
            leave = true;

            BGM::get_ton()->StopAll();

            BGM::get_ton()->load("res\\sound\\voyage.wav");
            BGM::get_ton()->play("res\\sound\\voyage.wav", 10, true);

            BGM::get_ton()->load("res\\sound\\sea.wav");
            BGM::get_ton()->play("res\\sound\\sea.wav", 5);
        }
        else
        {
            PopUp2::Get()->SetText(IDS_STRING110);
        }
    }
    else if (result == "�C�J�_�̑܂�����")
    {
        ShowStorehouse();
    }
    else if (result == "���������")
    {
        StartFadeInOut();
        m_eState = eBattleState::CREATE_TORCH;
        leave = true;
    }
    else if (result == "�����ɉ΂�����")
    {
        // �����̑ϋv�l���O�̏ꍇ�A�G���[���b�Z�[�W��\�����_�΂��Ȃ��B
        auto weapon = Common::Status()->GetEquipWeapon();
        if (weapon.GetDurabilityCurrent() <= 0)
        {
            PopUp2::Get()->SetText(IDS_STRING110);
        }
        else
        {
            auto rain = Rain::Get()->IsRain();

            if (rain)
            {
                PopUp2::Get()->SetText(IDS_STRING111);
            }
            else
            {
                NSStarmanLib::WeaponManager::GetObj()->SetTorchLit(true);
                leave = true;
                m_eState = eBattleState::NORMAL;
                BGM::get_ton()->load("res\\sound\\torch.wav");
                BGM::get_ton()->play("res\\sound\\torch.wav", 10, true);
            }
        }
    }
    else if (result == "�����̉΂�����")
    {
        NSStarmanLib::WeaponManager::GetObj()->SetTorchLit(false);
        leave = true;
        m_eState = eBattleState::NORMAL;
        BGM::get_ton()->stop("res\\sound\\torch.wav");
    }
    else if (result == "�N���t�g")
    {
        m_eState = eBattleState::CRAFT;

        Common::SetCursorVisibility(true);
        m_craft.Build();
        leave = true;
    }
    else if (result == "�p�b�`�e�X�g")
    {
        m_eState = eBattleState::PATCH_TEST;

        Common::SetCursorVisibility(true);
        m_patchManager2.Finalize();
        m_patchManager2.InitPatch();
        leave = true;
    }
    else if (result == "����`��")
    {
        // �T���J�N�}�����V�J�N�}���̂ǂ��炩�Ƌ߂��ɂ��Ȃ��Ă͂Ȃ�Ȃ��B
        // �ǂ�����߂��ɂ���ꍇ�A�T���J�N�}����D�悷��
        auto npcManager = NSStarmanLib::NpcStatusManager::GetObj();
        auto status = npcManager->GetNpcStatus(Common::LoadString_(IDS_STRING112));
        auto npcPos = D3DXVECTOR3(status.GetX(), status.GetY(), status.GetZ());
        auto ppos = m_player->GetPos();

        auto _near = Common::HitByBoundingBox(npcPos, ppos, 2.f);
        if (!_near)
        {
            status = npcManager->GetNpcStatus(Common::LoadString_(IDS_STRING113));
            npcPos = D3DXVECTOR3(status.GetX(), status.GetY(), status.GetZ());
            ppos = m_player->GetPos();
            _near = Common::HitByBoundingBox(npcPos, ppos, 2.f);
        }

        if (_near)
        {
            auto npcName = status.GetName();
            auto help = NSStarmanLib::Help::Get();
            auto enable = help->CanReceive(npcName);

            if (enable)
            {
                m_eState = eBattleState::NORMAL;

                Common::SetCursorVisibility(true);
                leave = true;

                auto items = help->ReceiveItems(npcName);

                for (auto& item : items)
                {
                    Common::Inventory()->AddItem(item.GetId());
                    auto name = item.GetName();

                    std::string work = Common::LoadStringWithArg(IDS_STRING114, name);
                    PopUp2::Get()->SetText(work);
                }
            }
        }
    }

    // �R�}���h��ʂ����ꍇ
    if (leave)
    {
        Camera::SetCameraMode(eCameraMode::BATTLE);
        Common::SetCursorVisibility(false);
    }
}

void SeqBattle::InitLoad()
{
    // �Z�[�u�f�[�^�����邩�ۂ�
    int saveExist = PathFileExists("res\\script\\save");

    if (saveExist == FALSE)
    {
        m_bSavedataExists = false;
        SAFE_DELETE(m_loadThread);
        m_loadThread = NEW std::thread(
            [&]
            {
                if (Common::DeployMode() || Common::DeployEncryptMode())
                {
                    try
                    {
                        SaveManager::Get()->LoadOrigin();

						SAFE_DELETE(m_map);
						m_map = NEW Map();
						SharedObj::SetMap(m_map);
						m_map->Init();

                        m_loadLoaded.store(true);
                    }
                    catch (const std::exception& e)
                    {
                        std::ofstream ofs("error.log");
                        ofs << e.what();
                        std::terminate();
                    }
                }
                else
                {
                    SaveManager::Get()->LoadOrigin();

					SAFE_DELETE(m_map);
					m_map = NEW Map();
					SharedObj::SetMap(m_map);
					m_map->Init();

                    m_loadLoaded.store(true);
                }
            });
    }
    else
    {
        m_bSavedataExists = true;
        SAFE_DELETE(m_loadThread);
        m_loadThread = NEW std::thread(
            [&]
            {
                if (Common::DeployMode() || Common::DeployEncryptMode())
                {
                    try
                    {
                        SaveManager::Get()->Load();

						SAFE_DELETE(m_map);
						m_map = NEW Map();
						SharedObj::SetMap(m_map);
						m_map->Init();

                        m_loadLoaded.store(true);
                    }
                    catch (const std::exception& e)
                    {
                        std::ofstream ofs("error.log");
                        ofs << e.what();
                        std::terminate();
                    }
                }
                else
                {
                    SaveManager::Get()->Load();

					SAFE_DELETE(m_map);
					m_map = NEW Map();
					SharedObj::SetMap(m_map);
					m_map->Init();

                    m_loadLoaded.store(true);
                }
            });
    }

    m_sprLoadBack = NEW Sprite("res\\image\\black.png");
    m_sprLoadLogo = NEW Sprite("res\\image\\title01.png");
    m_sprLoadClock = NEW Sprite("res\\image\\load_clock.png");
    m_sprLoadLoading = NEW Sprite("res\\image\\loading.png");

    Common::SetCursorVisibility(false);

    Camera::SetCameraMode(eCameraMode::TITLE);
}

void SeqBattle::UpdateLoad()
{
    if (m_loadLoaded.load())
    {
        InitializeAfterLoad();
        FinalizeLoad();
        m_eState = eBattleState::TITLE;
        m_title = NEW Title(true, true);
    }
}

void SeqBattle::InitializeAfterLoad()
{
    delete m_menuManager;
    m_menuManager = NEW MenuManager();
    m_menuManager->InitMenu();

    auto status = NSStarmanLib::StatusManager::GetObj();
    D3DXVECTOR3 pos;
    status->GetXYZ(&pos.x, &pos.y, &pos.z);
    m_player->SetPos(pos);

    pos.y += 1.f;
    Camera::SetLookAtPos(pos);

    Camera::SetCameraMode(eCameraMode::TITLE);

    SharedObj::Voyage()->Init();
}

void SeqBattle::RenderLoad()
{
    D3DXVECTOR3 pos(0.f, 0.f, 0.f);
    m_sprLoadBack->Render(pos);

    // ���邭���\��
    if (m_loadThread != nullptr && m_loadLoaded.load() == false)
    {

        static float counter = 0;
        counter += 0.02f;
        int temp1 = 0;

        D3DXVECTOR3 pos { 0.0f, 0.0f, 0.0f };
        pos.x = std::sin(counter) * 100 + 800;
        pos.y = std::cos(counter) * 60 + 450;
        temp1 = (int)((std::sin(counter * 0.2) + 1) * 64);
        m_sprLoadClock->Render(pos, temp1);

        static float counter2 = 0;
        counter2 += 0.03f;

        pos.x = std::sin(counter2) * 60 + 800;
        pos.y = std::cos(counter2) * 90 + 450;
        temp1 = (int)((std::sin(counter2 * 0.2) + 1) * 128);
        m_sprLoadClock->Render(pos, temp1);

        static int counter3 = 0;

        counter3 += 2;
        if (counter3 >= 256)
        {
            counter3 = 0;
        }

        int temp = 0;
        if (counter3 <= 127)
        {
            temp = counter3;
        }
        else
        {
            temp = 256 - counter3;
        }

        pos = D3DXVECTOR3(800 - 40, 450 -10, 0.0f);
        m_sprLoadLoading->Render(pos, temp);

        static int width_ = 0;
        pos.x = 630;
        pos.y = 200;

        static int counter4 = 0;
        ++counter4;

        int progress = SaveManager::Get()->GetProgress();

//        if (counter4 <= 60*1)
//        {
//            width_ = 0;
//        }
//        else if (counter4 <= 60*4)
//        {
//            width_ = 90;
//        }
//        else if (counter4 <= 60*9)
//        {
//            width_ = 180;
//        }
//        else if (counter4 <= 60*16)
//        {
//            width_ = 270;
//        }
//        else if (counter4 <= 60*25)
//        {
//            width_ = -1;
//        }

        if (progress <= 20)
        {
            width_ = 0;
        }
        else if (progress <= 40)
        {
            width_ = 90;
        }
        else if (progress <= 60)
        {
            width_ = 180;
        }
        else if (progress <= 80)
        {
            width_ = 270;
        }
        else if (progress <= 99)
        {
            width_ = -1;
        }

        m_sprLoadLogo->Render(pos, 255, width_);
    }
}

void SeqBattle::FinalizeLoad()
{
    SAFE_DELETE(m_sprLoadBack);
    SAFE_DELETE(m_sprLoadLogo);
    SAFE_DELETE(m_sprLoadLoading);
    SAFE_DELETE(m_sprLoadClock);
}

void SeqBattle::OperateOpening()
{
    m_Opening->Update(&m_eState);

    if (m_eState == eBattleState::NORMAL)
    {
        SAFE_DELETE(m_Opening);
        Camera::SetCameraMode(eCameraMode::BATTLE);
        Common::SetCursorVisibility(false);

        BGM::get_ton()->StopAll();
        BGM::get_ton()->load("res\\sound\\novel.wav");
        BGM::get_ton()->play("res\\sound\\novel.wav", 10);

        {
            BGM::get_ton()->load("res\\sound\\forest.wav");
            BGM::get_ton()->play("res\\sound\\forest.wav", 10);

            BGM::get_ton()->load("res\\sound\\sea.wav");
        }
    }
}

void SeqBattle::OperateSleep()
{
    if (m_fadeBlackCount == 1)
    {
        auto status = NSStarmanLib::StatusManager::GetObj();
        status->Sleep();
        if (status->GetLevelUpFire())
        {
            PopUp2::Get()->SetText(IDS_STRING115);
        }

        if (status->GetLevelUpIce())
        {
            PopUp2::Get()->SetText(IDS_STRING116);
        }

        if (status->GetLevelUpDark())
        {
            PopUp2::Get()->SetText(IDS_STRING117);
        }

        if (status->GetLevelDownFire())
        {
            PopUp2::Get()->SetText(IDS_STRING118);
        }

        if (status->GetLevelDownIce())
        {
            PopUp2::Get()->SetText(IDS_STRING119);
        }

        if (status->GetLevelDownDark())
        {
            PopUp2::Get()->SetText(IDS_STRING120);
        }
    }

    if (m_eFadeSeq == eFadeSeq::Finish)
    {
        m_eState = eBattleState::NORMAL;
        m_player->SetSleep(false);
    }
}

void SeqBattle::RenderSleep()
{
    // do nothing
}

void SeqBattle::StartFadeInOut()
{
    m_eFadeSeq = eFadeSeq::FadeOut;
    m_fadeOutCount = 0;
    m_fadeBlackCount = 0;
    m_fadeInCount = 0;
}

void SeqBattle::UpdateFadeInOut()
{
    {
        if (m_eFadeSeq == eFadeSeq::FadeOut)
        {
            ++m_fadeOutCount;
            if (m_fadeOutCount >= 30)
            {
                m_eFadeSeq = eFadeSeq::Sleep;
            }
        }
        else if (m_eFadeSeq == eFadeSeq::Sleep)
        {
            ++m_fadeBlackCount;
            if (m_fadeBlackCount >= 60)
            {
                m_eFadeSeq = eFadeSeq::FadeIn;
            }
        }
        else if (m_eFadeSeq == eFadeSeq::FadeIn)
        {
            ++m_fadeInCount;
            if (m_fadeInCount >= 30)
            {
                m_eFadeSeq = eFadeSeq::Finish;
            }
        }
        else if (m_eFadeSeq == eFadeSeq::Finish)
        {
            m_eFadeSeq = eFadeSeq::NotStart;
            m_fadeOutCount = 0;
            m_fadeBlackCount = 0;
            m_fadeInCount = 0;
        }
    }
}

void SeqBattle::DrawFadeInOut()
{
    D3DXVECTOR3 pos(0.f, 0.f, 0.f);
    int transparency = 0;
    if (m_eFadeSeq == eFadeSeq::FadeOut)
    {
        transparency = m_fadeOutCount * 255 / 30;
        m_sprBlack->Render(pos, transparency);
    }
    else if (m_eFadeSeq == eFadeSeq::Sleep)
    {
        m_sprBlack->Render(pos);
    }
    else if (m_eFadeSeq == eFadeSeq::FadeIn)
    {
        transparency = 255 - (m_fadeInCount * 255 / 30);
        m_sprBlack->Render(pos, transparency);
    }
}

void SeqBattle::OperateQuest(eSequence* sequence)
{
    QuestManager::Get()->Update();

    //------------------------------------------------
    // �N�G�X�g�J�n����
    //------------------------------------------------
    {
        auto startQuest = QuestManager::Get()->GetStartQuest();
        if (startQuest.empty() == false)
        {
            QuestManager::Get()->SetHint("");

            {
                // TODO ���o�����鏈��
                auto it = std::find(startQuest.begin(), startQuest.end(), "Q10");
                if (it != startQuest.end())
                {
                    int i = 0;
                    ++i;
                }
            }

            auto startEvent = QuestManager::Get()->GetQuestStartEvent(startQuest.at(0));
            if (startEvent.empty() == false)
            {
                // TODO �ŏ��̃C�x���g�����������Ă��邪�K�v�ɂȂ����畡���C�x���g�Ή�
                if (startEvent.at(0).find("<talk>") != std::string::npos)
                {
                    std::string work = startEvent.at(0);
                    std::string::size_type it = work.find("<talk>");
                    work = work.erase(it, 6);

                    NSTalkLib2::IFont* pFont = NEW NSTalkLib2::Font(SharedObj::GetD3DDevice());
                    NSTalkLib2::ISoundEffect* pSE = NEW NSTalkLib2::SoundEffect();
                    NSTalkLib2::ISprite* sprite = NEW NSTalkLib2::Sprite(SharedObj::GetD3DDevice());

                    m_talk = NEW NSTalkLib2::Talk();
                    m_talk->Init(Common::ModExt(work), pFont, pSE, sprite,
                                 "res\\image\\textBack.png", "res\\image\\black.png",
                                 Common::DeployEncryptMode(),
                                 SharedObj::IsEnglish());

                    m_eState = eBattleState::TALK;
                }
                else if (startEvent.at(0).find("<hint>") != std::string::npos)
                {
                    std::string work = startEvent.at(0);
                    work = Common::RemoveSubstring(work, "<hint>");
                    QuestManager::Get()->SetHint(work);
                }
            }
        }
    }

    //------------------------------------------
    // �I���N�G�X�g
    //------------------------------------------
    {
        std::vector<std::string> vs = QuestManager::Get()->GetFinishQuest();
        for (std::size_t i = 0; i < vs.size(); ++i)
        {
            m_finishQuestQue.push_back(vs.at(i));
        }

        if (m_finishQuestQue.size() >= 1)
        {
            std::string questId = m_finishQuestQue.at(0);
            std::vector<std::string> vs2 = QuestManager::Get()->GetQuestFinishEvent(questId);
            m_finishQuestQue.pop_front();
            for (size_t j = 0; j < vs2.size(); ++j)
            {
                if (vs2.at(j).find("<talk>") != std::string::npos)
                {
                    std::string work = vs2.at(j);
                    std::string::size_type it = work.find("<talk>");
                    work = work.erase(it, 6);

                    NSTalkLib2::IFont* pFont = NEW NSTalkLib2::Font(SharedObj::GetD3DDevice());
                    NSTalkLib2::ISoundEffect* pSE = NEW NSTalkLib2::SoundEffect();
                    NSTalkLib2::ISprite* sprite = NEW NSTalkLib2::Sprite(SharedObj::GetD3DDevice());

                    m_talk = NEW NSTalkLib2::Talk();
                    m_talk->Init(Common::ModExt(work), pFont, pSE, sprite,
                                 "res\\image\\textBack.png", "res\\image\\black.png",
                                 Common::DeployEncryptMode(),
                                 SharedObj::IsEnglish());

                    m_eState = eBattleState::TALK;
                }
                else if (vs2.at(j).find("<hide>") != std::string::npos)
                {
                    std::string work = vs2.at(j);
                    std::string::size_type it = work.find("<hide>");
                    work = work.erase(it, 6);

                    auto mapObjManager = NSStarmanLib::MapObjManager::GetObj();
                    std::vector<NSStarmanLib::stMapObj> mapObjs =
                        mapObjManager->GetMapObjListR(m_player->GetPos().x, m_player->GetPos().z, 5.f);

                    for (int i = 0; i < (int)mapObjs.size(); ++i)
                    {
                        std::string xName = mapObjManager->GetModelName(mapObjs.at(i).m_modelId);

                        if (xName == work)
                        {
                            mapObjManager->SetVisible(mapObjs.at(i).m_frameX,
                                                      mapObjs.at(i).m_frameZ,
                                                      mapObjs.at(i).m_id,
                                                      false);
                            break;
                        }
                    }
                }
                else if (vs2.at(j).find("<sound>") != std::string::npos)
                {
                    std::string work = vs2.at(j);
                    std::string::size_type it = work.find("<sound>");
                    work = work.erase(it, 7);

                    ::SoundEffect::get_ton()->load(work);
                    ::SoundEffect::get_ton()->play(work);
                }
                else if (vs2.at(j).find("<ending>") != std::string::npos)
                {
                    *sequence = eSequence::ENDING;
                }
                else if (vs2.at(j).find("<finish>") != std::string::npos)
                {
                    std::string work = vs2.at(j);
                    std::string::size_type it = work.find("<finish>");
                    work = work.erase(it, 8);
                    QuestManager::Get()->SetQuestFinish(work);
                }
                else if (vs2.at(j).find("<story>") != std::string::npos)
                {
                    std::string work = vs2.at(j);
                    std::string::size_type it = work.find("<story>");
                    work = work.erase(it, 7);
                    m_story = NEW StoryManager(work);
                    m_eState = eBattleState::STORY;
                }
                else if (vs2.at(j).find("<npc>") != std::string::npos)
                {
                    std::string work = vs2.at(j);
                    std::string::size_type it = work.find("<npc>");
                    work = work.erase(it, 5);
                    std::string npcName;
                    if (work.find("<" + Common::LoadString_(IDS_STRING121) + ">") != std::string::npos)
                    {
                        npcName = Common::LoadString_(IDS_STRING121);
                    }
                    else if (work.find("<" + Common::LoadString_(IDS_STRING112) + ">") != std::string::npos)
                    {
                        npcName = Common::LoadString_(IDS_STRING112);
                    }
                    else if (work.find("<" + Common::LoadString_(IDS_STRING113) + ">") != std::string::npos)
                    {
                        npcName = Common::LoadString_(IDS_STRING113);
                    }
                    else if (work.find("<" + Common::LoadString_(IDS_STRING122) + ">") != std::string::npos)
                    {
                        npcName = Common::LoadString_(IDS_STRING122);
                    }

                    work = Common::RemoveSubstring(work, "<" + npcName + ">");

                    if (work.find("<pos>") != std::string::npos)
                    {
                        std::string work2;
                        work2 = Common::RemoveSubstring(work, "<pos>");
                        std::vector<std::string> vs = Common::split(work2, ':');
                        float fx = std::stof(vs.at(0));
                        float fy = std::stof(vs.at(1));
                        float fz = std::stof(vs.at(2));
                        NpcManager::Get()->SetPos(npcName, fx, fy, fz);
                    }
                    else if (work.find("<rot>") != std::string::npos)
                    {
                        std::string work2;
                        work2 = Common::RemoveSubstring(work, "<rot>");
                        float fRot = std::stof(work2);
                        NpcManager::Get()->SetRot(npcName, fRot);
                    }
                    else if (work.find("<talkEnable>") != std::string::npos)
                    {
                        std::string work2;
                        work2 = Common::RemoveSubstring(work, "<talkEnable>");
                        if (work2 == "y")
                        {
                            NpcManager::Get()->SetTalkEnable(npcName, true);
                        }
                        else
                        {
                            NpcManager::Get()->SetTalkEnable(npcName, false);
                        }
                    }
                    else if (work.find("<talkScript>") != std::string::npos)
                    {
                        std::string work2;
                        work2 = Common::RemoveSubstring(work, "<talkScript>");
                        NpcManager::Get()->SetTalkScript(npcName, work2);
                    }
                    else if (work.find("<enableFeature>") != std::string::npos)
                    {
                        std::string work2;
                        work2 = Common::RemoveSubstring(work, "<enableFeature>");
                        if (work2 == "y")
                        {
                            NpcManager::Get()->SetEnableFeature(npcName, true);
                        }
                        else
                        {
                            NpcManager::Get()->SetEnableFeature(npcName, false);
                        }
                    }
                    else if (work.find("<showMenu>") != std::string::npos)
                    {
                        std::string work2;
                        work2 = Common::RemoveSubstring(work, "<showMenu>");
                        if (work2 == "y")
                        {
                            NpcManager::Get()->SetEnableFeature(npcName, true);
                        }
                        else
                        {
                            NpcManager::Get()->SetEnableFeature(npcName, false);
                        }
                    }
                }
                else if (vs2.at(j).find("<move>") != std::string::npos)
                {
                    std::string work = vs2.at(j);
                    std::string work2 = Common::RemoveSubstring(work, "<move>");
                    std::vector<std::string> vs = Common::split(work2, ':');
                    float fx = std::stof(vs.at(0));
                    float fy = std::stof(vs.at(1));
                    float fz = std::stof(vs.at(2));
                    m_player->SetPos(D3DXVECTOR3(fx, fy, fz));
                }
                else if (vs2.at(j).find("<rynenContract>") != std::string::npos)
                {
                    auto rynen = NSStarmanLib::Rynen::GetObj();
                    rynen->SetContracted(true);

                    auto datetime = NSStarmanLib::PowereggDateTime::GetObj();
                    rynen->SetContractDate(datetime->GetYear(),
                                           datetime->GetMonth(),
                                           datetime->GetDay());
                }
                else if (vs2.at(j).find("<drinkWordBress>") != std::string::npos)
                {
                    auto pos = m_player->GetPos();
                    Common::Status()->DrinkWordBress(pos.x, pos.y, pos.z);
                }
                // �A�C�e�����擾
                else if (vs2.at(j).find("<item>") != std::string::npos)
                {
                    std::string work2 = Common::RemoveSubstring(vs2.at(j), "<item>");
                    auto itemDef = Common::ItemManager()->GetItemDef(work2);
                    auto newSubId = Common::Inventory()->AddItem(itemDef.GetId());
                }
                // ����𑕔�
                else if (vs2.at(j).find("<equip>") != std::string::npos)
                {
                    std::string work2 = Common::RemoveSubstring(vs2.at(j), "<equip>");
                    auto itemDef = Common::ItemManager()->GetItemDef(work2);
                    auto subIdList = Common::Inventory()->GetSubIdList(itemDef.GetId());
                    assert(subIdList.size() >= 1);

                    auto itemInfo = Common::Inventory()->GetItemInfo(itemDef.GetId(), subIdList.at(0));
                    Common::Status()->SetEquipWeapon(itemInfo);
                }
                else if (vs2.at(j).find("<sotetsu>") != std::string::npos)
                {
                    auto status = Common::Status();
                    status->SetDehydration(true);
                    status->SetHeadache(true);
                    status->SetStomachache(true);
                    status->SetSuperStress();
                    status->SetWaterCurrent(92.f);

                    if (status->GetBodyStaminaCurrent() > 20.f)
                    {
                        status->SetBodyStaminaCurrent(20.f);
                    }

                    if (status->GetBrainStaminaCurrent() > 20.f)
                    {
                        status->SetBrainStaminaCurrent(20.f);
                    }

                    if (status->GetMineralCurrent() > 20.f)
                    {
                        status->SetMineralCurrent(20.f);
                    }

                    if (status->GetVitaminCurrent() > 20.f)
                    {
                        status->SetVitaminCurrent(20.f);
                    }

                    if (status->GetMuscleCurrent() > 20.f)
                    {
                        status->SetMuscleCurrent(20.f);
                    }
                }
            }
        }
    }

    // �u���ׂ�v�A�N�V�������ł��邱�Ƃ��A�C�R���Œm�点��B
    std::string quest1 = QuestManager::Get()->GetQuestIdStartByExamine();
    std::string quest2 = QuestManager::Get()->GetQuestIdFinishByExamine();
    if (quest1.empty() == false || quest2.empty() == false)
    {
        m_bShowExamine = true;
    }
    else
    {
        m_bShowExamine = false;
    }
}

void SeqBattle::OperatePatch()
{
    std::string result = m_patchManager2.Operate();

    if (result == "EXIT")
    {
        m_eState = eBattleState::NORMAL;
        Camera::SetCameraMode(eCameraMode::BATTLE);
        Common::SetCursorVisibility(false);
    }
}

void SeqBattle::OperatePickPlant()
{
    if (m_eFadeSeq == eFadeSeq::Finish)
    {
        int rand = SharedObj::GetRandom();

        std::string pick;

        // TODO ���S�����_���ł͂Ȃ��A�ꏊ�ƌ����ڂɂ���đ����̂΂���������Ăق���
        // ���\�[�X�t�@�C������擾����̂ł͂Ȃ��ACSV�t�@�C������擾���ׂ�
        if (rand % 100 < 10)
        {
            pick = Common::LoadString_(IDS_STRING123);
        }
        else if (rand % 100 < 20)
        {
            pick = Common::LoadString_(IDS_STRING124);
        }
        else if (rand % 100 < 30)
        {
            pick = Common::LoadString_(IDS_STRING125);
        }
        else if (rand % 100 < 40)
        {
            pick = Common::LoadString_(IDS_STRING126);
        }
        else if (rand % 100 < 45)
        {
            pick = Common::LoadString_(IDS_STRING127);
        }
        else if (rand % 100 < 50)
        {
            pick = "�Ԃ���";
        }
        else if (rand % 100 < 55)
        {
            pick = "�c�N�V";
        }
        else if (rand % 100 < 60)
        {
            pick = "�n�C�r�X�J�X";
        }
        else if (rand % 100 < 65)
        {
            pick = "�^���|�|";
        }
        else if (rand % 100 < 70)
        {
            pick = "�j���A�������̓X�C�Z��";
        }
        else if (rand % 100 < 75)
        {
            pick = "�L�m�R";
        }
        else if (rand % 100 < 80)
        {
            pick = "�p�p�C��";
        }
        else if (rand % 100 < 85)
        {
            pick = "�}���S�[";
        }
        else if (rand % 100 < 90)
        {
            pick = "�o�i�i";
        }
        else if (rand % 100 < 95)
        {
            pick = "�c�^";
        }
        else if (rand % 100 < 100)
        {
            pick = "�؂̎}";
        }

        PopUp2::Get()->SetText(Common::LoadStringWithArg(IDS_STRING128, pick));

        // 30���o�߂����鏈��
        auto dateTime = NSStarmanLib::PowereggDateTime::GetObj();
        dateTime->IncreaseDateTime(0, 0, 0, 30, 0);

        // �̗͂�����
        // ��������ŏ���鎞�Ԃ�̗͂��ς��
        Common::Status()->PickPlant();

        // �A�C�e�����C���x���g���ɒǉ�
        auto itemDef = Common::ItemManager()->GetItemDef(pick);
        Common::Inventory()->AddItem(itemDef.GetId());

        // ������������
        m_map->DeletePlant(m_player->GetPos());

        m_eState = eBattleState::NORMAL;
        Camera::SetCameraMode(eCameraMode::BATTLE);
        Common::SetCursorVisibility(false);
    }
}

void SeqBattle::RenderPickPlant()
{
}

void SeqBattle::OperateCutTree()
{
    if (m_eFadeSeq == eFadeSeq::Finish)
    {
        PopUp2::Get()->SetText(IDS_STRING129);

        // 6���Ԍo�߂����鏈��
        // ��������ŏ���鎞�Ԃ�̗͂��ς��
        // 

        auto status = NSStarmanLib::StatusManager::GetObj();
        auto itemInfo = status->GetEquipWeapon();
        auto name = itemInfo.GetItemDef().GetName();
        auto level = itemInfo.GetItemDef().GetLevel();

        auto dateTime = NSStarmanLib::PowereggDateTime::GetObj();

        if (name == Common::LoadString_(IDS_STRING130))
        {
            dateTime->IncreaseDateTime(0, 0, 6, 0, 0);
        }
        else if (name == Common::LoadString_(IDS_STRING131))
        {
            auto durability = itemInfo.GetDurabilityCurrent();

            // ���Ԃ��o�߂����A�Ε��̑ϋv�l�����炷
            if (level == 0 || level == -1)
            {
                dateTime->IncreaseDateTime(0, 0, 8, 0, 0);
                durability -= 1000;
            }
            else if (level == 1)
            {
                dateTime->IncreaseDateTime(0, 0, 5, 30, 0);
                durability -= 900;
            }
            else if (level == 2)
            {
                dateTime->IncreaseDateTime(0, 0, 5, 0, 0);
                durability -= 800;
            }
            else if (level == 3)
            {
                dateTime->IncreaseDateTime(0, 0, 4, 30, 0);
                durability -= 700;
            }
            else if (level == 4)
            {
                dateTime->IncreaseDateTime(0, 0, 4, 0, 0);
                durability -= 600;
            }
            else if (level == 5)
            {
                dateTime->IncreaseDateTime(0, 0, 3, 30, 0);
                durability -= 500;
            }

            if (durability <= -1)
            {
                durability = 0;
            }

            Common::Inventory()->SetItemDurability(itemInfo.GetId(),
                                                   itemInfo.GetSubId(),
                                                   durability);
        }

        // �̗͂�����
        // ��������ŏ���鎞�Ԃ�̗͂��ς��
        Common::Status()->CutTree(name, level);

        // �A�C�e�����C���x���g���ɒǉ�
        auto itemDef = Common::ItemManager()->GetItemDef(Common::LoadString_(IDS_STRING132));
        Common::Inventory()->AddItem(itemDef.GetId());

        // �؂���������
        m_map->DeleteTree(m_player->GetPos());

        m_eState = eBattleState::NORMAL;
        Camera::SetCameraMode(eCameraMode::BATTLE);
        Common::SetCursorVisibility(false);
    }
}

void SeqBattle::RenderCutTree()
{
}

void SeqBattle::OperateVoyage()
{
    SharedObj::Voyage()->Operate(&m_eState);
    if (m_eState == eBattleState::COMMAND)
    {
        m_commandManager.SetPreviousState(eBattleState::VOYAGE);
    }
}

void SeqBattle::OperateVoyage3Hours()
{
    if (m_eFadeSeq == eFadeSeq::Sleep)
    {
        // 3���Ԍ�̈ʒu�Ƀ��[�v
        SharedObj::Voyage()->Set3HoursAuto();

        m_eState = eBattleState::VOYAGE;
        Camera::SetCameraMode(eCameraMode::BATTLE);
        Common::SetCursorVisibility(false);
    }
}

void SeqBattle::OperateCreateTorch()
{
    if (m_eFadeSeq == eFadeSeq::Sleep)
    {
        //-----------------------------------------------
        // �������̖؂̖_���폜���A�����𐶐��B
        // �����āA�����𑕔�����
        //-----------------------------------------------

        // �������̖؂̖_���폜
        auto weapon = Common::Status()->GetEquipWeapon();
        Common::Inventory()->RemoveItem(weapon.GetId(), weapon.GetSubId());

        auto itemDef = Common::ItemManager()->GetItemDef(Common::LoadString_(IDS_STRING133));
        auto newSubId = Common::Inventory()->AddItem(itemDef.GetId());

        auto itemInfo = Common::Inventory()->GetItemInfo(itemDef.GetId(), newSubId);
        Common::Status()->SetEquipWeapon(itemInfo);

        m_eState = eBattleState::NORMAL;
        Camera::SetCameraMode(eCameraMode::BATTLE);
        Common::SetCursorVisibility(false);
    }
}

void SeqBattle::Render()
{
    RenderCommon();

    if (m_eState == eBattleState::LOAD)
    {
        RenderLoad();
    }
    else if (m_eState == eBattleState::TITLE)
    {
        m_title->Render();
    }
    else if (m_eState == eBattleState::STORY)
    {
        m_story->Render();
    }
    else if (m_eState == eBattleState::OPENING)
    {
        m_Opening->Render();
    }
    else if (m_eState == eBattleState::TALK)
    {
        m_talk->Render();
    }
    else if (m_eState == eBattleState::MENU)
    {
        m_menuManager->Draw();
    }
    else if (m_eState == eBattleState::PATCH_TEST)
    {
        m_patchManager2.Draw();
    }
    else if (m_eState == eBattleState::STOREHOUSE)
    {
        m_storehouse->Draw();
    }
    else if (m_eState == eBattleState::CRAFT)
    {
        m_craft.Draw();
    }
    else if (m_eState == eBattleState::COMMAND)
    {
        m_commandManager.Draw();
    }
    else if (m_eState == eBattleState::NORMAL)
    {
        RenderNormal();
    }
    else if (m_eState == eBattleState::DEAD)
    {
        RenderGameover();
    }
    else if (m_eState == eBattleState::SLEEP)
    {
        RenderSleep();
    }
    else if (m_eState == eBattleState::PICK_PLANT)
    {
        RenderPickPlant();
    }
    else if (m_eState == eBattleState::CUT_TREE)
    {
        RenderCutTree();
    }
    else if (m_eState == eBattleState::VOYAGE)
    {
        // �C�J�_�͏���Ă��Ă�����Ă��Ȃ��Ă���ɕ\������ׂ����̂Ȃ̂�
        // �����ł͉������Ȃ��B
    }
    else if (m_eState == eBattleState::POPUP)
    {
        RenderPopUp();
    }

    RenderCommon2D();
}

void SeqBattle::Confirm(eSequence* sequence)
{
    // ���ׂ�R�}���h
    // �v���C���[�̌��ݍ��W�Ŏn�܂�N�G�X�g���I���N�G�X�g������B
    if (m_bShowExamine)
    {
        m_bShowExamine = false;
        QuestManager::Get()->SetExamine();
    }
    else if (m_bObtainable)
    {
        m_bObtainable = false;

        NSStarmanLib::ItemManager* itemManager = NSStarmanLib::ItemManager::GetObj();

        D3DXVECTOR3 playerPos = SharedObj::GetPlayer()->GetPos();
        NSStarmanLib::ItemPos itemPos = itemManager->GetItemPosByPos(playerPos.x,
                                                                     playerPos.y,
                                                                     playerPos.z);
        if (itemPos.GetItemPosId() != -1)
        {
            int itemPosId = itemPos.GetItemPosId();
            itemManager->SetItemPosObtained(itemPosId);
            
            // �ǂꂾ���ו����d���Ă������Ă�����̂��E�����Ƃ͂ł���B
            // ����ɁA�܂Ƃ��ɕ�������ł��Ȃ��Ȃ�B
            auto inventory = NSStarmanLib::Inventory::GetObj();
            int newSubID = inventory->AddItem(itemPos.GetItemDefId());
            m_menuManager->AddItem(itemPos.GetItemDefId(), newSubID);

            std::string work = itemManager->GetItemDef(itemPos.GetItemDefId()).GetName();
            SoundEffect::get_ton()->play("res\\sound\\menu_cursor_confirm.wav");
            PopUp2::Get()->SetText(Common::LoadStringWithArg(IDS_STRING128, work));
        }
    }
    else if (m_bTalkable)
    {
        // ��b���J�n
        m_bTalkable = false;

        // ��b�ɂ��̗͂̏���
        Common::Status()->Talk();

        auto npcManager = NpcManager::Get();
        std::string npcName;
        D3DXVECTOR3 playerPos = SharedObj::GetPlayer()->GetPos();
        npcManager->GetNpcTalkable(playerPos, &npcName);
        if (npcName.empty() == false)
        {
            auto npcStatus = npcManager->GetNpcStatus(npcName);
            std::string csvfile = npcStatus.GetTalkCsv();

            NSTalkLib2::IFont* pFont = NEW NSTalkLib2::Font(SharedObj::GetD3DDevice());
            NSTalkLib2::ISoundEffect* pSE = NEW NSTalkLib2::SoundEffect();
            NSTalkLib2::ISprite* sprite = NEW NSTalkLib2::Sprite(SharedObj::GetD3DDevice());

            m_talk = NEW NSTalkLib2::Talk();
            m_talk->Init(Common::ModExt(csvfile), pFont, pSE, sprite,
                         "res\\image\\textBack.png", "res\\image\\black.png",
                         Common::DeployEncryptMode(),
                         SharedObj::IsEnglish());

            m_eState = eBattleState::TALK;

            // ��b�����̂ŉ�b�\��Ԃ���������
            npcManager->SetTalkEnable(npcName, false);

            QuestManager::Get()->SetTalk(npcName);
        }
    }
    else if (m_bObtainWeapon)
    {
        m_bObtainWeapon = false;

        D3DXVECTOR3 playerPos = SharedObj::GetPlayer()->GetPos();
        auto thrownItem = SharedObj::GetMap()->GetThrownItem(playerPos);

        NSStarmanLib::ItemManager* itemManager = NSStarmanLib::ItemManager::GetObj();

        if (thrownItem.GetId() != -1)
        {
            SharedObj::GetMap()->DeleteThrownItem(thrownItem);
            
            // �ǂꂾ���ו����d���Ă������Ă�����̂��E�����Ƃ͂ł���B
            // ����ɁA�܂Ƃ��ɕ�������ł��Ȃ��Ȃ�B
            auto inventory = NSStarmanLib::Inventory::GetObj();
            int newSubID = inventory->AddItem(thrownItem.GetId());
            m_menuManager->AddItem(thrownItem.GetId(), newSubID);

            std::string work = itemManager->GetItemDef(thrownItem.GetId()).GetName();
            SoundEffect::get_ton()->play("res\\sound\\menu_cursor_confirm.wav");
            PopUp2::Get()->SetText(Common::LoadStringWithArg(IDS_STRING128, work));
        }

    }
    else if (m_bShowStorehouse)
    {
        ShowStorehouse();
    }
}

eBattleState SeqBattle::GetState() const
{
    return m_eState;
}

void SeqBattle::UpdateCommon()
{
    if (m_eState == eBattleState::LOAD)
    {
        return;
    }

    if (m_eState == eBattleState::TITLE)
    {
        return;
    }

    if (m_eState == eBattleState::TALK)
    {
        return;
    }

    m_map->Update();

    PopUp2::Get()->Update();

    // 60��Ɉ��i��1�b���Ɓj�̏���
    {
        static int counter = 0;
        counter++;
        if (counter >= 60)
        {
            counter = 0;
        }
        if (counter == 0)
        {
            UpdatePerSecond();
        }
    }

    UpdateFadeInOut();

    // �N���t�g�̓N���t�g��ʂ��\������Ă��Ȃ��Ă��X�V�������s����K�v������B
    m_craft.Update();

    // �N���t�g�ɂ���ăC�J�_����������邱�Ƃ����邽�߁A�q�C���łȂ��Ă��X�V�������s���B
    SharedObj::Voyage()->Update();
}

void SeqBattle::RenderCommon()
{
    if (m_eState == eBattleState::LOAD)
    {
        // do nothing
    }
    else if (m_eState == eBattleState::TITLE && m_title->GetLoading())
    {
        m_map->Render();
    }
    else
    {
        m_player->Render();
        m_map->Render();
        SharedObj::Voyage()->Draw();

        DrawFadeInOut();
    }
}

void SeqBattle::RenderCommon2D()
{
    m_player->Render2D();

    // TODO ������
    if (m_eState != eBattleState::LOAD &&
        m_eState != eBattleState::TITLE &&
        // m_eState != eBattleState::MENU && // ���j���[��ʂł��|�b�v�A�b�v���K�v
        m_eState != eBattleState::OPENING &&
        m_eState != eBattleState::TALK)
    {
        PopUp2::Get()->Render();
    }
}

void SeqBattle::RenderNormal()
{
    PopUp::Get()->Render();
    D3DXVECTOR3 pos { 0.f, 0.f, 0.f };

    if (m_bShowExamine || m_bObtainable || m_bTalkable || m_bObtainWeapon || m_bShowStorehouse)
    {
        D3DXVECTOR3 pos { 720.f, 700.f, 0.f };
        m_spriteExamine->Render(pos);
    }

    m_hudManager.Draw();
}

void SeqBattle::RenderGameover()
{
    D3DXVECTOR3 pos(0.f, 0.f, 0.f);
    int transparency = 0;

    auto rynen = NSStarmanLib::Rynen::GetObj();
    // �����\��
    if (rynen->GetReviveEnable() == false)
    {
        if (m_nDeadCounter < 120)
        {
            transparency = m_nDeadCounter * 255 / 120;
            m_spriteGameover->Render(pos, transparency);
        }
        else
        {
            m_spriteGameover->Render(pos);
            pos = D3DXVECTOR3(720.f, 700.f, 0.f);
            m_spriteExamine->Render(pos);
        }
    }
    else
    {
        if (m_nDeadCounter < 120)
        {
            transparency = m_nDeadCounter * 255 / 120;
            m_sprBlack->Render(pos, transparency);
        }
        else
        {
            m_sprBlack->Render(pos);
        }
    }
}

void SeqBattle::UpdateDebug()
{
    // PopUp Sample
    {
        if (SharedObj::KeyBoard()->IsDownFirstFrame(DIK_F7))
        {
            std::vector<std::vector<std::string>> vvs;
            std::vector<std::string> vs;
            vs.push_back("�T���v���e�L�X�g�P");
            vs.push_back("�T���v���e�L�X�g�Q�T���v���e�L�X�g�Q");
            vs.push_back("�T���v���e�L�X�g�R�T���v���e�L�X�g�R�T���v���e�L�X�g�R");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("�T���v���e�L�X�g�S");
            vs.push_back("�T���v���e�L�X�g�T�T���v���e�L�X�g�T");
            vs.push_back("�T���v���e�L�X�g�U�T���v���e�L�X�g�U�T���v���e�L�X�g�U");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("�T���v���e�L�X�g�V�T���v���e�L�X�g�V�T���v���e�L�X�g�V�T���v���e�L�X�g�V");
            vs.push_back("�T���v���e�L�X�g�W�T���v���e�L�X�g�W�T���v���e�L�X�g�W");
            vs.push_back("�T���v���e�L�X�g�X�T���v���e�L�X�g�X");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("�T���v���e�L�X�g�P�O");
            vvs.push_back(vs);
            PopUp::Get()->SetText(vvs);
        }

        if (SharedObj::KeyBoard()->IsDownFirstFrame(DIK_F8))
        {
            PopUp::Get()->Next();
        }

        if (SharedObj::KeyBoard()->IsDownFirstFrame(DIK_F9))
        {
            PopUp::Get()->Cancel();
        }

        // PopUp::Get()->Update();
    }

    //--------------------------------------
    // �q�ɋ@�\
    //--------------------------------------
    {
        if (SharedObj::KeyBoard()->IsDownFirstFrame(DIK_F1))
        {
            if (m_eState == eBattleState::NORMAL)
            {
                m_eState = eBattleState::STOREHOUSE;
                delete m_storehouse;

                Camera::SetCameraMode(eCameraMode::SLEEP);
                Common::SetCursorVisibility(true);

                // TODO �q�ɂ�\������x�ɑq�ɉ�ʂ����̂���߂�
                m_storehouse = NEW NSStorehouseLib::StorehouseLib();

                NSStorehouseLib::Sprite* sprCursor = NEW NSStorehouseLib::Sprite(SharedObj::GetD3DDevice());
                sprCursor->Load("res\\image\\menu_cursor.png");

                NSStorehouseLib::Sprite* sprBackground = NEW NSStorehouseLib::Sprite(SharedObj::GetD3DDevice());
                sprBackground->Load("res\\image\\background.png");

                NSStorehouseLib::IFont* pFont = NEW NSStorehouseLib::Font(SharedObj::GetD3DDevice());
                pFont->Init(SharedObj::IsEnglish());

                NSStorehouseLib::ISoundEffect* pSE = NEW NSStorehouseLib::SoundEffect();

                m_storehouse->Init(pFont, pSE, sprCursor, sprBackground, SharedObj::IsEnglish());
                {
                    using namespace NSStarmanLib;
                    NSStarmanLib::Inventory* inventory = NSStarmanLib::Inventory::GetObj();
                    NSStarmanLib::ItemManager* itemManager = NSStarmanLib::ItemManager::GetObj();

                    std::vector<int> idList = itemManager->GetItemIdList();

                    std::vector<NSStorehouseLib::StoreItem> itemInfoList;
                    for (std::size_t i = 0; i < idList.size(); ++i)
                    {
                        NSStarmanLib::ItemDef itemDef = itemManager->GetItemDef(idList.at(i));
                        std::vector<int> subIdList = inventory->GetSubIdList(idList.at(i));
                        {
                            for (std::size_t j = 0; j < subIdList.size(); ++j)
                            {
                                std::string work_str;

                                NSStorehouseLib::StoreItem itemInfoG;

                                itemInfoG.SetName(itemDef.GetName());
                                itemInfoG.SetId(itemDef.GetId());
                                itemInfoG.SetSubId(subIdList.at(j));
                                if (itemDef.GetLevel() != -1)
                                {
                                    itemInfoG.SetLevel(subIdList.at(j));
                                }

                                auto info = inventory->GetItemInfo(itemDef.GetId(), subIdList.at(j));
                                if (info.GetDurabilityCurrent() != -1)
                                {
                                    itemInfoG.SetDurability(info.GetDurabilityCurrent());
                                }
                                itemInfoList.push_back(itemInfoG);
                            }
                        }
                    }
                    m_storehouse->SetInventoryList(itemInfoList);
                }
                {
                    using namespace NSStarmanLib;

                    // TODO �q�ɂ̕������Ή�
                    auto storehouse = NSStarmanLib::StorehouseManager::Get()->GetStorehouse(1);

                    NSStarmanLib::ItemManager* itemManager = NSStarmanLib::ItemManager::GetObj();

                    std::vector<int> idList = itemManager->GetItemIdList();

                    std::vector<NSStorehouseLib::StoreItem> itemInfoList;
                    for (std::size_t i = 0; i < idList.size(); ++i)
                    {
                        NSStarmanLib::ItemDef itemDef = itemManager->GetItemDef(idList.at(i));
                        std::vector<int> subIdList = storehouse->GetSubIdList(idList.at(i));
                        {
                            for (std::size_t j = 0; j < subIdList.size(); ++j)
                            {
                                std::string work_str;

                                NSStorehouseLib::StoreItem itemInfoG;

                                itemInfoG.SetName(itemDef.GetName());
                                itemInfoG.SetId(itemDef.GetId());
                                itemInfoG.SetSubId(subIdList.at(j));
                                if (itemDef.GetLevel() != -1)
                                {
                                    itemInfoG.SetLevel(subIdList.at(j));
                                }

                                auto info = storehouse->GetItemInfo(itemDef.GetId(), subIdList.at(j));
                                if (info.GetDurabilityCurrent() != -1)
                                {
                                    itemInfoG.SetDurability(info.GetDurabilityCurrent());
                                }
                                itemInfoList.push_back(itemInfoG);
                            }
                        }
                    }
                    m_storehouse->SetStorehouseList(itemInfoList);
                }
            }
        }
    }

    //--------------------------------------
    // �N���t�g�@�\
    //--------------------------------------
    {
        if (SharedObj::KeyBoard()->IsDownFirstFrame(DIK_F2))
        {
            if (m_eState == eBattleState::NORMAL)
            {
                m_eState = eBattleState::CRAFT;

                Camera::SetCameraMode(eCameraMode::SLEEP);
                Common::SetCursorVisibility(true);
                m_craft.Build();
            }
        }
    }

    //--------------------------------------
    // �p�b�`�e�X�g�@�\
    //--------------------------------------
    {
        if (SharedObj::KeyBoard()->IsDownFirstFrame(DIK_F3))
        {
            if (m_eState == eBattleState::NORMAL)
            {
                m_eState = eBattleState::PATCH_TEST;
                Camera::SetCameraMode(eCameraMode::SLEEP);
                Common::SetCursorVisibility(true);
                m_patchManager2.Finalize();
                m_patchManager2.InitPatch();

            }
        }
    }
}

void SeqBattle::UpdatePerSecond()
{
    //-------------------------------------
    // ������i�߂�
    //-------------------------------------
    // 2���ԃQ�[����������p���[�G�b�O����24���Ԍo�߂���
    NSStarmanLib::PowereggDateTime* dateTime = NSStarmanLib::PowereggDateTime::GetObj();

    if (Common::DebugMode() == false)
    {
        dateTime->IncreaseDateTime(0, 0, 0, 0, 12);
    }
    else
    {
//        dateTime->IncreaseDateTime(0, 0, 0, 30, 0); // 1�b��1���ԂƂ��o�߂����������p
        dateTime->IncreaseDateTime(0, 0, 0, 0, 12);
    }

    //-------------------------------------
    // �X�e�[�^�X���X�V
    //-------------------------------------
    NSStarmanLib::StatusManager* statusManager = NSStarmanLib::StatusManager::GetObj();
    statusManager->Update();

    //-------------------------------------
    // ���S�`�F�b�N
    //-------------------------------------
    bool dead = statusManager->GetDead();
    if (dead)
    {
        if (m_eState != eBattleState::DEAD)
        {
            m_eState = eBattleState::DEAD;
            m_nDeadCounter = 0;

            auto rynen = NSStarmanLib::Rynen::GetObj();
            if (rynen->GetReviveEnable())
            {
                PopUp2::Get()->SetText(IDS_STRING134);
            }
        }
        return;
    }

    //-------------------------------------
    // �����E�C��`�F�b�N
    //-------------------------------------
    if (m_eFadeSeq == eFadeSeq::NotStart)
    {
        bool sleep = statusManager->GetSleep();
        if (sleep)
        {
            m_eState = eBattleState::SLEEP;
            m_player->SetSleep(true);
            PopUp2::Get()->SetText(IDS_STRING135);

            StartFadeInOut();
        }
    }

    //-------------------------------------
    // �A�C�e������
    //-------------------------------------
    D3DXVECTOR3 playerPos = SharedObj::GetPlayer()->GetPos();
    NSStarmanLib::ItemManager* itemManager = NSStarmanLib::ItemManager::GetObj();

    NSStarmanLib::ItemPos itemPos = itemManager->GetItemPosByPos(playerPos.x,
                                                                 playerPos.y,
                                                                 playerPos.z);

    if (itemPos.GetItemPosId() != -1)
    {
        m_bObtainable = true;
    }
    else
    {
        m_bObtainable = false;
    }

    //-------------------------------------
    // �����Ēn�ʂɗ������A�C�e���𔭌�
    //-------------------------------------
    auto thrownItem = SharedObj::GetMap()->GetThrownItem(playerPos);

    if (thrownItem.GetId() != -1)
    {
        m_bObtainWeapon = true;
    }
    else
    {
        m_bObtainWeapon = false;
    }

    //-------------------------------------
    // �ő�ύڗʂ𒴂��Ă����烁�b�Z�[�W��\��
    //-------------------------------------
    {
        static float previousVol = 0.f;
        static float previousVolMax = 0.f;

        auto inventory = NSStarmanLib::Inventory::GetObj();

        auto vol = inventory->GetVolume();
        auto volMax = inventory->GetVolumeMax();

        if (previousVol != vol ||
            previousVolMax != volMax)
        {
            if (vol > volMax)
            {
                PopUp2::Get()->SetText(IDS_STRING136);
            }
        }
        previousVol = vol;
        previousVolMax = volMax;
    }
    //-------------------------------------
    // �������g�p���Ă����珼���̑ϋv�l��ቺ
    //-------------------------------------
    {
        auto lit = NSStarmanLib::WeaponManager::GetObj()->IsTorchLit();
        if (lit)
        {
            auto weapon = Common::Status()->GetEquipWeapon();
            auto dura = weapon.GetDurabilityCurrent() - 1;
            if (dura < 0)
            {
                dura = 0;
                NSStarmanLib::WeaponManager::GetObj()->SetTorchLit(false);
                BGM::get_ton()->stop("res\\sound\\torch.wav");
            }

            Common::Inventory()->SetItemDurability(weapon.GetId(), weapon.GetSubId(), dura);
            weapon.SetDurabilityCurrent(dura);
            Common::Status()->SetEquipWeapon(weapon);
        }
        else
        {
            auto isPlay = BGM::get_ton()->IsPlay("res\\sound\\torch.wav");
            if (isPlay)
            {
                BGM::get_ton()->stop("res\\sound\\torch.wav");
            }
        }
    }
    {
        auto help = NSStarmanLib::Help::Get();
        help->Update();
    }
}

void SeqBattle::OperateNormal(eSequence* sequence)
{
    //============================================
    // �L�[�{�[�h�A�}�E�X�A�Q�[���p�b�h�̏���
    //============================================

    //--------------------------------------------
    // KeyBoard
    //--------------------------------------------

    //--------------------------------------
    // ���j���[�@�\
    //--------------------------------------
    if (SharedObj::KeyBoard()->IsDownFirstFrame(DIK_ESCAPE))
    {
        m_eState = eBattleState::MENU;
        Camera::SetCameraMode(eCameraMode::SLEEP);
        Common::SetCursorVisibility(true);

        delete m_menuManager;
        m_menuManager = NEW MenuManager();
        m_menuManager->InitMenu();

        return;
    }

    //--------------------------------------
    // �R�}���h�@�\
    //--------------------------------------
    if (SharedObj::KeyBoard()->IsDownFirstFrame(DIK_C))
    {
        if (m_eState == eBattleState::NORMAL)
        {
            m_eState = eBattleState::COMMAND;

            Camera::SetCameraMode(eCameraMode::SLEEP);
            Common::SetCursorVisibility(true);

            m_commandManager.SetPreviousState(eBattleState::NORMAL);
        }
    }

    // �m�葀��
    if (SharedObj::KeyBoard()->IsDownFirstFrame(DIK_E))
    {
        Confirm(sequence);
    }

    //--------------------------------------------
    // GamePad
    //--------------------------------------------

    //--------------------------------------
    // ���j���[�@�\
    //--------------------------------------
    if (GamePad::IsDown(eGamePadButtonType::START))
    {
        if (m_eState == eBattleState::NORMAL)
        {
            m_eState = eBattleState::MENU;
            Camera::SetCameraMode(eCameraMode::SLEEP);
            Common::SetCursorVisibility(true);

            delete m_menuManager;
            m_menuManager = NEW MenuManager();
            m_menuManager->InitMenu();
        }
    }

    //--------------------------------------
    // �R�}���h�@�\
    //--------------------------------------
    if (GamePad::IsDown(eGamePadButtonType::BACK))
    {
        if (m_eState == eBattleState::NORMAL)
        {
            m_eState = eBattleState::COMMAND;

            Camera::SetCameraMode(eCameraMode::SLEEP);
            Common::SetCursorVisibility(true);

            m_commandManager.SetPreviousState(eBattleState::NORMAL);
        }
    }

    // �m�葀��
    if (GamePad::IsDown(eGamePadButtonType::A))
    {
        Confirm(sequence);
    }

    m_player->Update(m_map);

    m_hudManager.Update();

    //-------------------------------------------------
    // 0.5�b��1�񂭂炢�̏���
    //-------------------------------------------------
    {
        static int work = 0;
        ++work;
        if (work >= 30)
        {
            work = 0;
        }

        if (work == 0)
        {
            // �N�G�X�g����
            {
                OperateQuest(sequence);
            }

            // �߂��Ƀ`�F�X�g����������q�ɋ@�\���g����B
            {
                auto ppos = m_player->GetPos();
                auto storehouse = 
                    NSStarmanLib::StorehouseManager::Get()-> GetNearStorehouse(ppos.x, ppos.z);
                if (storehouse != nullptr)
                {
                    m_bShowStorehouse = true;
                }
                else
                {
                    m_bShowStorehouse = false;
                }
            }
        }
    }

    //---------------------------------------------------------------------
    // �u�b��������v���ł��邱�Ƃ��A�C�R���Œm�点��
    //---------------------------------------------------------------------
    {
        // 3���[�g���ȓ��ɘb����������NPC�����邩�B
        auto npcManager = NpcManager::Get();
        auto playerPos = SharedObj::GetPlayer()->GetPos();
        bool exist = npcManager->GetNpcTalkable(playerPos);
        if (exist)
        {
            m_bTalkable = true;
        }
        else
        {
            m_bTalkable = false;
        }
    }
}

void SeqBattle::OperateTitle(eSequence* sequence)
{
    m_title->Update(sequence, &m_eState);

    if (*sequence == eSequence::EXIT)
    {
        SAFE_DELETE(m_title);
    }

    if (m_eState == eBattleState::OPENING)
    {
        SAFE_DELETE(m_title);
        InitializeAfterLoad();

        FinalizeLoad();
        m_Opening = NEW Opening();
    }
    else if (m_eState == eBattleState::NORMAL)
    {
        SAFE_DELETE(m_title);

        Camera::SetCameraMode(eCameraMode::BATTLE);
        Common::SetCursorVisibility(false);

        BGM::get_ton()->StopAll();
        BGM::get_ton()->load("res\\sound\\novel.wav");
        BGM::get_ton()->play("res\\sound\\novel.wav", 10);

        {
            BGM::get_ton()->load("res\\sound\\forest.wav");
            BGM::get_ton()->play("res\\sound\\forest.wav", 10);

            BGM::get_ton()->load("res\\sound\\sea.wav");
        }
    }
}

void SeqBattle::OperateGameover(eSequence* sequence)
{
    ++m_nDeadCounter;

    if (m_nDeadCounter == 1)
    {
        if (Common::DeployMode() || Common::DeployEncryptMode())
        {
            SaveManager::Get()->DeleteSavedata();
        }
    }

    if (m_nDeadCounter >= 120)
    {
        auto rynen = NSStarmanLib::Rynen::GetObj();

        // �����i�V
        if (rynen->GetReviveEnable() == false)
        {
            // �m�葀��
            if (SharedObj::KeyBoard()->IsDownFirstFrame(DIK_E))
            {
                if (m_nDeadCounter >= 120)
                {
                    m_eState = eBattleState::TITLE;
                    Camera::SetCameraMode(eCameraMode::TITLE);
                    Common::SetCursorVisibility(true);
                    m_title = NEW Title(true, false);
                }
            }

            // �m�葀��
            if (GamePad::IsDown(eGamePadButtonType::A))
            {
                if (m_nDeadCounter >= 120)
                {
                    m_eState = eBattleState::TITLE;
                    Camera::SetCameraMode(eCameraMode::TITLE);
                    Common::SetCursorVisibility(true);
                    m_title = NEW Title(true, false);
                }
            }
        }
        // ��������
        else
        {
            rynen->SetReviveEnable(false);
            float x = 0.f;
            float y = 0.f;
            float z = 0.f;
            rynen->GetRevivePos(&x, &y, &z);

            m_player->SetPos(D3DXVECTOR3(x, y, z));

            m_eState = eBattleState::NORMAL;

            auto status = NSStarmanLib::StatusManager::GetObj();
            float work1 = 0.f;
            work1 = status->GetBodyStaminaMax();
            status->SetBodyStaminaCurrent(work1);
            status->SetBodyStaminaMaxSub(work1);

            work1 = status->GetBrainStaminaMax();
            status->SetBrainStaminaCurrent(work1);
            status->SetBrainStaminaMaxSub(work1);

            work1 = status->GetMuscleMax();
            status->SetMuscleCurrent(work1);

            work1 = status->GetCarboMax();
            status->SetCarboCurrent(work1);

            work1 = status->GetProteinMax();
            status->SetProteinCurrent(work1);

            work1 = status->GetLipidMax();
            status->SetLipidCurrent(work1);

            work1 = status->GetVitaminMax();
            status->SetVitaminCurrent(work1);

            work1 = status->GetMineralMax();
            status->SetMineralCurrent(work1);

            work1 = status->GetWaterMax();
            status->SetWaterCurrent(work1);

            status->SetFractureArm(false);
            status->SetFractureLeg(false);
            status->SetHeadache(false);
            status->SetHeadache(false);
            status->SetCold(false);
            status->SetStomachache(false);
            status->SetDehydration(false);
            status->SetLackOfSleep(false);

            status->SetDead(false);
        }
    }
}

void SeqBattle::OperatePopUp()
{
    PopUp::Get()->Update();

    if (SharedObj::KeyBoard()->IsDownFirstFrame(DIK_SPACE))
    {
        PopUp::Get()->Next();
    }

    if (SharedObj::KeyBoard()->IsDownFirstFrame(DIK_RETURN))
    {
        PopUp::Get()->Next();
    }

    if (Mouse::IsDownLeft())
    {
        PopUp::Get()->Next();
    }

    if (GamePad::IsDown(eGamePadButtonType::A))
    {
        PopUp::Get()->Next();
    }

    bool isShow = PopUp::Get()->IsShow();
    if (!isShow)
    {
        m_eState = eBattleState::NORMAL;
    }
}

void SeqBattle::RenderPopUp()
{
    PopUp::Get()->Render();
}


