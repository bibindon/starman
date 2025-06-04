// F7キーでサンプルテキスト

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
#include "../../StarmanLib/StarmanLib/StarmanLib/MapInfoManager.h"
#include "NpcManager.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/Rynen.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/Help.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/NpcStatusManager.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/ActivityBase.h"
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

        void Load(const std::wstring& filepath) override
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
                                    CLEARTYPE_NATURAL_QUALITY,
                                    FF_DONTCARE,
                                    _T("BIZ UDMincho"),
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
                                    _T("Courier New"),
                                    &m_pFont);
            }

            assert(hr == S_OK);
        }

        virtual void DrawText_(const std::wstring& msg, const int x, const int y, const int transparency)
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
            ::SoundEffect::get_ton()->play(_T("res\\sound\\menu_cursor_move.wav"));
        }
        virtual void PlayClick() override
        {
            ::SoundEffect::get_ton()->play(_T("res\\sound\\menu_cursor_confirm.wav"));
        }
        virtual void PlayBack() override
        {
            ::SoundEffect::get_ton()->play(_T("res\\sound\\menu_cursor_cancel.wav"));
        }
        virtual void Init() override
        {
            ::SoundEffect::get_ton()->load(_T("res\\sound\\menu_cursor_move.wav"));
            ::SoundEffect::get_ton()->load(_T("res\\sound\\menu_cursor_confirm.wav"));
            ::SoundEffect::get_ton()->load(_T("res\\sound\\menu_cursor_cancel.wav"));
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

    void Load(const std::wstring& filepath) override
    {
        if (filepath.empty())
        {
            return;
        }

        // スプライトは一つのみ確保し使いまわす
        if (m_D3DSprite == NULL)
        {
            if (FAILED(D3DXCreateSprite(m_pD3DDevice, &m_D3DSprite)))
            {
                throw std::exception("Failed to create a sprite.");
            }
        }

        m_filepath = filepath;

        // 同じ画像ファイルで作られたテクスチャが既にあるなら、
        // 画像のサイズだけ確保しテクスチャの作成を行わない
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

        // テクスチャの作成
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

    // スプライトは一つを使いまわす
    static LPD3DXSPRITE m_D3DSprite;

    std::wstring m_filepath;

    // 同じ名前の画像ファイルで作られたテクスチャは使いまわす
    static std::unordered_map<std::wstring, LPDIRECT3DTEXTURE9> m_texMap;
};
 
LPD3DXSPRITE Sprite::m_D3DSprite = NULL;
std::unordered_map<std::wstring, LPDIRECT3DTEXTURE9> Sprite::m_texMap;

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
                                CLEARTYPE_NATURAL_QUALITY,
                                FF_DONTCARE,
                                _T("BIZ UDMincho"),
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
                                _T("Courier New"),
                                &m_pFont);
        }

        assert(hr == S_OK);
    }

    virtual void DrawText_(const std::wstring& msg, const int x, const int y)
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
        ::SoundEffect::get_ton()->load(_T("res\\sound\\message1.wav"));
    }

    void PlayMessage() override
    {
        ::SoundEffect::get_ton()->play(_T("res\\sound\\message1.wav"));
    }

    void Stop() override
    {
        ::SoundEffect::get_ton()->stop(_T("res\\sound\\message1.wav"));
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

    SoundEffect::get_ton()->load(_T("res\\sound\\drink.wav"));

    m_spriteGameover = NEW Sprite(_T("res\\image\\gameover.png"));

    if (!SharedObj::IsEnglish())
    {
        m_spriteExamine = NEW Sprite(_T("res\\image\\examine.png"));
    }
    else
    {
        m_spriteExamine = NEW Sprite(_T("res\\image\\examine.en.png"));
    }

    NSTalkLib2::IFont* pFont = NEW NSTalkLib2::Font(SharedObj::GetD3DDevice());
    NSTalkLib2::ISoundEffect* pSE = NEW NSTalkLib2::SoundEffect();
    NSTalkLib2::ISprite* sprite = NEW NSTalkLib2::Sprite(SharedObj::GetD3DDevice());

    m_hudManager.Init();

    Common::SetCursorVisibility(false);

    m_eState = eBattleState::LOAD;
    InitLoad();

    m_sprBlack = NEW Sprite(_T("res\\image\\black.png"));

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
    else if (m_eState == eBattleState::REST)
    {
        OperateRest();
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
    // カメラの更新処理
    //
    // カメラの更新処理はUpdate関数の最後に実施する必要がある。
    // OperateVoyage関数のようにプレイヤーの位置を変更する関数があり、
    // その場合は、プレイヤーの位置が変更されてからカメラの更新を行う必要があるため。
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
    std::wstring result = m_menuManager->OperateMenu();
    if (result == _T("Title"))
    {
        m_eState = eBattleState::TITLE;
        Camera::SetCameraMode(eCameraMode::BATTLE_TO_TITLE);
        Common::SetCursorVisibility(true);
        m_title = NEW Title(false, false);
    }
    else if (result == _T("EXIT"))
    {
        if (VoyageManager::Get()->GetRaftMode())
        {
            m_eState = eBattleState::VOYAGE;
        }
        else
        {
            m_eState = eBattleState::NORMAL;
        }

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

    // 暫定的対処
    // 根本的な欠陥がある
    // マップクラスに二つのモードがあるべき
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
    std::wstring result;

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
        std::vector<std::wstring> vs = Common::split(result, ':');

        int id_ = 0;
        int subId_ = 0;
        int durability_ = 0;

        id_ = std::stoi(vs.at(2));
        subId_ = std::stoi(vs.at(3));

        NSStarmanLib::Inventory* inventory = NSStarmanLib::Inventory::GetObj();

        // 倉庫の複数化対応
        float x_ = SharedObj::GetPlayer()->GetPos().x;
        float z_ = SharedObj::GetPlayer()->GetPos().z;
        auto storehouse = NSStarmanLib::StorehouseManager::Get()->GetNearStorehouse(x_, z_);

        if (Common::DebugMode() || Common::ReleaseMode())
        {
            storehouse = NSStarmanLib::StorehouseManager::Get()->GetStorehouse(1);
        }

        if (storehouse != nullptr)
        {
            if (vs.at(0) == _T("left"))
            {
                bool equipBagExist = false;
                bool equipWeaponExist = false;

                // 装備中の袋だったら削除できないようにする
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

                // 装備中の武器だったら削除できないようにする
                {
                    auto weapon = Common::Status()->GetEquipWeapon();
                    if (weapon.GetId() == id_ && weapon.GetSubId() == subId_)
                    {
                        PopUp2::Get()->SetText(IDS_STRING103);
                        equipWeaponExist = true;
                    }
                }
                
                // 装備中の袋ではないし、装備中の武器でもない。
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
            else if (vs.at(0) == _T("right"))
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

        std::vector<std::wstring> vs = Common::split(result, ':');

        if (!vs.empty())
        {
            int id_ = 0;
            int subId_ = 0;
            int durability_ = 0;

            id_ = std::stoi(vs.at(2));
            subId_ = std::stoi(vs.at(3));

            NSStarmanLib::Inventory* inventory = NSStarmanLib::Inventory::GetObj();

            // 倉庫の複数化対応
            float x_ = SharedObj::GetPlayer()->GetPos().x;
            float z_ = SharedObj::GetPlayer()->GetPos().z;
            auto storehouse = NSStarmanLib::StorehouseManager::Get()->GetNearStorehouse(x_, z_);

            if (Common::DebugMode() || Common::ReleaseMode())
            {
                storehouse = NSStarmanLib::StorehouseManager::Get()->GetStorehouse(1);
            }

            if (storehouse != nullptr)
            {
                if (vs.at(0) == _T("left"))
                {
                    bool equipBagExist = false;
                    bool equipWeaponExist = false;

                    // 装備中の袋だったら削除できないようにする
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

                    // 装備中の武器だったら削除できないようにする
                    {
                        auto weapon = Common::Status()->GetEquipWeapon();
                        if (weapon.GetId() == id_ && weapon.GetSubId() == subId_)
                        {
                            PopUp2::Get()->SetText(IDS_STRING103);
                            equipWeaponExist = true;
                        }
                    }
                    
                    // 装備中の袋ではないし、装備中の武器でもない。
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
                else if (vs.at(0) == _T("right"))
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
        std::vector<std::wstring> vs = Common::split(result, ':');

        int id_ = 0;
        int subId_ = 0;
        int durability_ = 0;

        id_ = std::stoi(vs.at(2));
        subId_ = std::stoi(vs.at(3));

        NSStarmanLib::Inventory* inventory = NSStarmanLib::Inventory::GetObj();

        // 倉庫の複数化対応
        float x_ = SharedObj::GetPlayer()->GetPos().x;
        float z_ = SharedObj::GetPlayer()->GetPos().z;
        auto storehouse = NSStarmanLib::StorehouseManager::Get()->GetNearStorehouse(x_, z_);

        if (Common::DebugMode() || Common::ReleaseMode())
        {
            storehouse = NSStarmanLib::StorehouseManager::Get()->GetStorehouse(1);
        }

        if (storehouse != nullptr)
        {
            if (vs.at(0) == _T("left"))
            {
                bool equipBagExist = false;
                bool equipWeaponExist = false;

                // 装備中の袋だったら削除できないようにする
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

                // 装備中の武器だったら削除できないようにする
                {
                    auto weapon = Common::Status()->GetEquipWeapon();
                    if (weapon.GetId() == id_ && weapon.GetSubId() == subId_)
                    {
                        PopUp2::Get()->SetText(IDS_STRING103);
                        equipWeaponExist = true;
                    }
                }
                
                // 装備中の袋ではないし、装備中の武器でもない。
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
            else if (vs.at(0) == _T("right"))
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
    // 倉庫は複数存在できる。
    // 距離が近い倉庫を対象とする
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

    // TODO 倉庫を表示する度に倉庫画面を作るのをやめる
    m_storehouse = NEW NSStorehouseLib::StorehouseLib();

    NSStorehouseLib::Sprite* sprCursor = NEW NSStorehouseLib::Sprite(SharedObj::GetD3DDevice());
    sprCursor->Load(_T("res\\image\\menu_cursor.png"));

    NSStorehouseLib::Sprite* sprBackground = NEW NSStorehouseLib::Sprite(SharedObj::GetD3DDevice());
    sprBackground->Load(_T("res\\image\\background.png"));

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
                    std::wstring work_str;

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
                    std::wstring work_str;

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
    std::wstring result = m_commandManager.Operate();

    bool leave = false;

    if (result == _T("EXIT"))
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
    else if (result == Common::LoadString_(IDS_STRING181))
    {
        leave = true;
        m_player->SetSit();
        m_eState = eBattleState::NORMAL;
    }
    else if (result == Common::LoadString_(IDS_STRING180))
    {
        leave = true;
        m_player->SetLieDown();
        m_eState = eBattleState::NORMAL;
    }
    else if (result == _T("３時間休憩する"))
    {
        leave = true;
        m_player->SetSit();
        m_eState = eBattleState::REST;
        StartFadeInOut();
    }
    else if (result == Common::LoadString_(IDS_STRING182))
    {
        leave = true;
        m_eState = eBattleState::POPUP;

        {
            std::vector<std::vector<std::wstring>> vvs;
            std::vector<std::wstring> vs;
            vs.push_back(_T(""));
            vs.push_back(Common::LoadString_(IDS_STRING104));
            vvs.push_back(vs);
            vs.clear();
            auto hint = QuestManager::Get()->GetHint();
            if (hint.empty())
            {
                vs.push_back(_T(""));
                vs.push_back(Common::LoadString_(IDS_STRING105));
            }
            else
            {
                vs.push_back(_T(""));
                vs.push_back(hint);
            }
            vvs.push_back(vs);
            PopUp::Get()->SetText(vvs);
        }
    }
    else if (result == Common::LoadString_(IDS_STRING183))
    {
        leave = true;
        D3DXVECTOR3 pos(-285.f, 16.f, 539.f);
        m_player->SetPos(pos);
        m_eState = eBattleState::NORMAL;
    }
    else if (result == Common::LoadString_(IDS_STRING178))
    {
        // 石斧を装備しているか、縦長の石を所有していないと切れない
        auto status = NSStarmanLib::StatusManager::GetObj();
        auto itemInfo = status->GetEquipWeapon();

        std::wstring name;
        if (itemInfo.GetId() != -1 && itemInfo.GetId() != 0)
        {
            name = itemInfo.GetItemDef().GetName();
        }

        bool bHaveAxe = false;
        if (name == _T("石斧"))
        {
            bHaveAxe = true;
        }

        auto itemDef = NSStarmanLib::ItemManager::GetObj()->GetItemDef(_T("縦長の石"));
        auto subIdList = Common::Inventory()->GetSubIdList(itemDef.GetId());

        bool bHaveLongStone = false;

        if (!subIdList.empty())
        {
            bHaveLongStone = true;
        }

        if (!bHaveAxe && !bHaveLongStone)
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
    else if (result == Common::LoadString_(IDS_STRING179))
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
    else if (result == Common::LoadString_(IDS_STRING184))
    {
        SharedObj::Voyage()->SetSail(true);
    }
    else if (result == Common::LoadString_(IDS_STRING185))
    {
        SharedObj::Voyage()->SetSail(false);
    }
    else if (result == Common::LoadString_(IDS_STRING186))
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
    else if (result == Common::LoadString_(IDS_STRING187))
    {
        SharedObj::Voyage()->SetRaftMode(false);
        m_eState = eBattleState::NORMAL;
        SharedObj::GetPlayer()->SetIdle();
    }
    else if (result == Common::LoadString_(IDS_STRING188))
    {
        // 袋を装備していたらイカダに乗ることはできない
        auto bagState = Common::Status()->GetBagState();
        if (bagState.empty())
        {
            m_eState = eBattleState::VOYAGE;
            VoyageManager::Get()->Ride();
            leave = true;
        }
        else
        {
            PopUp2::Get()->SetText(IDS_STRING110);
        }
    }
    else if (result == Common::LoadString_(IDS_STRING189))
    {
        ShowStorehouse();
    }
    else if (result == Common::LoadString_(IDS_STRING190))
    {
        StartFadeInOut();
        m_eState = eBattleState::CREATE_TORCH;
        leave = true;
    }
    else if (result == Common::LoadString_(IDS_STRING192))
    {
        // 松明の耐久値が０の場合、エラーメッセージを表示し点火しない。
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
            }
        }
    }
    else if (result == Common::LoadString_(IDS_STRING193))
    {
        NSStarmanLib::WeaponManager::GetObj()->SetTorchLit(false);
        leave = true;
        m_eState = eBattleState::NORMAL;
    }
    else if (result == Common::LoadString_(IDS_STRING194))
    {
        m_eState = eBattleState::CRAFT;

        Common::SetCursorVisibility(true);
        m_craft.Build();
        leave = true;
    }
    else if (result == Common::LoadString_(IDS_STRING195))
    {
        m_eState = eBattleState::PATCH_TEST;

        Common::SetCursorVisibility(true);
        m_patchManager2.Finalize();
        m_patchManager2.InitPatch();
        leave = true;
    }
    else if (result == Common::LoadString_(IDS_STRING196))
    {
        // サンカクマンかシカクマンのどちらかと近くにいなくてはならない。
        // どちらも近くにいる場合、サンカクマンを優先する
        auto npcManager = NSStarmanLib::NpcStatusManager::GetObj();
        auto status = npcManager->GetNpcStatus(_T("sankakuman"));
        auto npcPos = D3DXVECTOR3(status.GetX(), status.GetY(), status.GetZ());
        auto ppos = m_player->GetPos();

        auto _near = Common::HitByBoundingBox(npcPos, ppos, 2.f);
        if (!_near)
        {
            status = npcManager->GetNpcStatus(_T("shikakuman"));
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

                    std::wstring work = Common::LoadStringWithArg(IDS_STRING114, name);
                    PopUp2::Get()->SetText(work);
                }
            }
        }
    }

    // コマンド画面を閉じる場合
    if (leave)
    {
        Camera::SetCameraMode(eCameraMode::BATTLE);
        Common::SetCursorVisibility(false);
    }
}

void SeqBattle::InitLoad()
{
    // セーブデータがあるか否か
    bool saveExist = SaveManager::Get()->SaveFolderExists();

    if (!saveExist)
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
                        std::ofstream ofs(_T("error.log"));
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
                        std::ofstream ofs(_T("error.log"));
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

    m_sprLoadBack = NEW Sprite(_T("res\\image\\black.png"));

    if (SharedObj::IsEnglish())
    {
        m_sprLoadLogo = NEW Sprite(_T("res\\image\\title01.en.png"));
    }
    else
    {
        m_sprLoadLogo = NEW Sprite(_T("res\\image\\title01.png"));
    }

    m_sprLoadClock = NEW Sprite(_T("res\\image\\load_clock.png"));
    m_sprLoadLoading = NEW Sprite(_T("res\\image\\loading.png"));

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

    // くるくるを表示
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

        int progress2 = 0;

        if (m_map != nullptr)
        {
            progress2 = m_map->GetProgress();
        }

        if (!SharedObj::IsEnglish())
        {
            if (progress < 50)
            {
                width_ = 0;
            }
            else if (progress < 100)
            {
                width_ = 90;
            }
            else
            {
                if (progress2 < 40)
                {
                    width_ = 180;
                }
                else if (progress2 < 80)
                {
                    width_ = 270;
                }
                else if (progress2 < 100)
                {
                    width_ = -1;
                }
            }
        }
        else
        {
            pos.x = 600;

            if (progress < 50)
            {
                width_ = 62 * 0;
            }
            else if (progress < 100)
            {
                width_ = 62 * 1;
            }
            else
            {
                if (progress2 < 30)
                {
                    width_ = 62 * 2;
                }
                else if (progress2 < 40)
                {
                    width_ = 62 * 3;
                }
                else if (progress2 < 50)
                {
                    width_ = 62 * 4;
                }
                else if (progress2 < 60)
                {
                    width_ = 62 * 5;
                }
                else if (progress2 < 70)
                {
                    width_ = 62 * 6;
                }
                else if (progress2 < 100)
                {
                    width_ = 62 * 6;
                }
            }
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
    // クエスト開始処理
    //------------------------------------------------
    {
        auto startQuest = QuestManager::Get()->GetStartQuest();
        if (startQuest.empty() == false)
        {
            QuestManager::Get()->SetHint(_T(""));

            auto startEvent = QuestManager::Get()->GetQuestStartEvent(startQuest.at(0));
            if (startEvent.empty() == false)
            {
                // TODO 最初のイベントだけ処理しているが必要になったら複数イベント対応
                if (startEvent.at(0).find(_T("<talk>")) != std::wstring::npos)
                {
                    std::wstring work = startEvent.at(0);
                    std::wstring::size_type it = work.find(_T("<talk>"));
                    work = work.erase(it, 6);

                    NSTalkLib2::IFont* pFont = NEW NSTalkLib2::Font(SharedObj::GetD3DDevice());
                    NSTalkLib2::ISoundEffect* pSE = NEW NSTalkLib2::SoundEffect();
                    NSTalkLib2::ISprite* sprite = NEW NSTalkLib2::Sprite(SharedObj::GetD3DDevice());

                    m_talk = NEW NSTalkLib2::Talk();
                    m_talk->Init(Common::ModExt(work), pFont, pSE, sprite,
                                 _T("res\\image\\textBack.png"), _T("res\\image\\black.png"),
                                 Common::DeployEncryptMode(),
                                 SharedObj::IsEnglish());

                    m_eState = eBattleState::TALK;
                }
                else if (startEvent.at(0).find(_T("<hint>")) != std::wstring::npos)
                {
                    std::wstring work = startEvent.at(0);
                    work = Common::RemoveSubstring(work, _T("<hint>"));
                    QuestManager::Get()->SetHint(work);
                }
            }
        }
    }

    //------------------------------------------
    // 終了クエスト
    //------------------------------------------
    {
        std::vector<std::wstring> vs = QuestManager::Get()->GetFinishQuest();
        for (std::size_t i = 0; i < vs.size(); ++i)
        {
            m_finishQuestQue.push_back(vs.at(i));
        }

        if (m_finishQuestQue.size() >= 1)
        {
            std::wstring questId = m_finishQuestQue.at(0);
            std::vector<std::wstring> vs2 = QuestManager::Get()->GetQuestFinishEvent(questId);
            m_finishQuestQue.pop_front();
            for (size_t j = 0; j < vs2.size(); ++j)
            {
                if (vs2.at(j).find(_T("<talk>")) != std::wstring::npos)
                {
                    std::wstring work = vs2.at(j);
                    std::wstring::size_type it = work.find(_T("<talk>"));
                    work = work.erase(it, 6);

                    // 花輪が飾られていたら会話を少し変える。
                    if (work == L"res\\script\\origin\\talkFinishQuest59.csv")
                    {
                        if (NSStarmanLib::ActivityBase::Get()->GetHanawa())
                        {
                            work = L"res\\script\\origin\\talkFinishQuest59-2.csv";
                        }
                    }

                    if (work == L"res\\script\\origin\\talkFinishQuest61.csv")
                    {
                        if (NSStarmanLib::ActivityBase::Get()->GetHanawa())
                        {
                            work = L"res\\script\\origin\\talkFinishQuest61-2.csv";
                        }
                    }

                    NSTalkLib2::IFont* pFont = NEW NSTalkLib2::Font(SharedObj::GetD3DDevice());
                    NSTalkLib2::ISoundEffect* pSE = NEW NSTalkLib2::SoundEffect();
                    NSTalkLib2::ISprite* sprite = NEW NSTalkLib2::Sprite(SharedObj::GetD3DDevice());

                    m_talk = NEW NSTalkLib2::Talk();
                    m_talk->Init(Common::ModExt(work), pFont, pSE, sprite,
                                 _T("res\\image\\textBack.png"), _T("res\\image\\black.png"),
                                 Common::DeployEncryptMode(),
                                 SharedObj::IsEnglish());

                    m_eState = eBattleState::TALK;
                }
                else if (vs2.at(j).find(_T("<hide>")) != std::wstring::npos)
                {
                    std::wstring work = vs2.at(j);
                    std::wstring::size_type it = work.find(_T("<hide>"));
                    work = work.erase(it, 6);

                    auto mapObjManager = NSStarmanLib::MapObjManager::GetObj();
                    std::vector<NSStarmanLib::stMapObj> mapObjs =
                        mapObjManager->GetMapObjListR(m_player->GetPos().x, m_player->GetPos().z, 5.f);

                    for (int i = 0; i < (int)mapObjs.size(); ++i)
                    {
                        std::wstring xName = mapObjManager->GetModelName(mapObjs.at(i).m_modelId);

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
                else if (vs2.at(j).find(_T("<sound>")) != std::wstring::npos)
                {
                    std::wstring work = vs2.at(j);
                    std::wstring::size_type it = work.find(_T("<sound>"));
                    work = work.erase(it, 7);

                    ::SoundEffect::get_ton()->load(work);
                    ::SoundEffect::get_ton()->play(work);
                }
                else if (vs2.at(j).find(_T("<ending>")) != std::wstring::npos)
                {
                    *sequence = eSequence::ENDING;
                }
                else if (vs2.at(j).find(_T("<finish>")) != std::wstring::npos)
                {
                    std::wstring work = vs2.at(j);
                    std::wstring::size_type it = work.find(_T("<finish>"));
                    work = work.erase(it, 8);
                    QuestManager::Get()->SetQuestFinish(work);
                }
                else if (vs2.at(j).find(_T("<story>")) != std::wstring::npos)
                {
                    std::wstring work = vs2.at(j);
                    std::wstring::size_type it = work.find(_T("<story>"));
                    work = work.erase(it, 7);
                    m_story = NEW StoryManager(work);
                    m_eState = eBattleState::STORY;
                }
                else if (vs2.at(j).find(_T("<npc>")) != std::wstring::npos)
                {
                    std::wstring work = vs2.at(j);
                    std::wstring::size_type it = work.find(_T("<npc>"));
                    work = work.erase(it, 5);
                    std::wstring npcNameKey;
                    if (work.find(_T("<daikeiman>")) != std::wstring::npos)
                    {
                        npcNameKey = _T("daikeiman");
                    }
                    else if (work.find(_T("<sankakuman>")) != std::wstring::npos)
                    {
                        npcNameKey = _T("sankakuman");
                    }
                    else if (work.find(_T("<shikakuman>")) != std::wstring::npos)
                    {
                        npcNameKey = _T("shikakuman");
                    }
                    else if (work.find(_T("vim")) != std::wstring::npos)
                    {
                        npcNameKey = _T("vim");
                    }

                    work = Common::RemoveSubstring(work, _T("<") + npcNameKey + _T(">"));

                    if (work.find(_T("<pos>")) != std::wstring::npos)
                    {
                        std::wstring work2;
                        work2 = Common::RemoveSubstring(work, _T("<pos>"));
                        std::vector<std::wstring> vs = Common::split(work2, L':');
                        float fx = std::stof(vs.at(0));
                        float fy = std::stof(vs.at(1));
                        float fz = std::stof(vs.at(2));
                        NpcManager::Get()->SetPos(npcNameKey, fx, fy, fz);
                    }
                    else if (work.find(_T("<rot>")) != std::wstring::npos)
                    {
                        std::wstring work2;
                        work2 = Common::RemoveSubstring(work, _T("<rot>"));
                        float fRot = std::stof(work2);
                        NpcManager::Get()->SetRot(npcNameKey, fRot);
                    }
                    else if (work.find(_T("<talkEnable>")) != std::wstring::npos)
                    {
                        std::wstring work2;
                        work2 = Common::RemoveSubstring(work, _T("<talkEnable>"));
                        if (work2 == _T("y"))
                        {
                            NpcManager::Get()->SetTalkEnable(npcNameKey, true);
                        }
                        else
                        {
                            NpcManager::Get()->SetTalkEnable(npcNameKey, false);
                        }
                    }
                    else if (work.find(_T("<talkScript>")) != std::wstring::npos)
                    {
                        std::wstring work2;
                        work2 = Common::RemoveSubstring(work, _T("<talkScript>"));
                        NpcManager::Get()->SetTalkScript(npcNameKey, work2);
                    }
                    else if (work.find(_T("<enableFeature>")) != std::wstring::npos)
                    {
                        std::wstring work2;
                        work2 = Common::RemoveSubstring(work, _T("<enableFeature>"));
                        if (work2 == _T("y"))
                        {
                            NpcManager::Get()->SetEnableFeature(npcNameKey, true);
                        }
                        else
                        {
                            NpcManager::Get()->SetEnableFeature(npcNameKey, false);
                        }
                    }
                    else if (work.find(_T("<showMenu>")) != std::wstring::npos)
                    {
                        std::wstring work2;
                        work2 = Common::RemoveSubstring(work, _T("<showMenu>"));
                        if (work2 == _T("y"))
                        {
                            NpcManager::Get()->SetShowMenu(npcNameKey, true);
                        }
                        else
                        {
                            NpcManager::Get()->SetShowMenu(npcNameKey, false);
                        }
                    }
                }
                else if (vs2.at(j).find(_T("<move>")) != std::wstring::npos)
                {
                    std::wstring work = vs2.at(j);
                    std::wstring work2 = Common::RemoveSubstring(work, _T("<move>"));
                    std::vector<std::wstring> vs = Common::split(work2, ':');
                    float fx = std::stof(vs.at(0));
                    float fy = std::stof(vs.at(1));
                    float fz = std::stof(vs.at(2));
                    m_player->SetPos(D3DXVECTOR3(fx, fy, fz));
                }
                else if (vs2.at(j).find(_T("<rynenContract>")) != std::wstring::npos)
                {
                    auto rynen = NSStarmanLib::Rynen::GetObj();
                    rynen->SetContracted(true);

                    auto datetime = NSStarmanLib::PowereggDateTime::GetObj();
                    rynen->SetContractDate(datetime->GetYear(),
                                           datetime->GetMonth(),
                                           datetime->GetDay());
                }
                else if (vs2.at(j).find(_T("<drinkWordBress>")) != std::wstring::npos)
                {
                    auto pos = m_player->GetPos();
                    Common::Status()->DrinkWordBress(pos.x, pos.y, pos.z);
                }
                // アイテムを取得
                else if (vs2.at(j).find(_T("<item>")) != std::wstring::npos)
                {
                    std::wstring work2 = Common::RemoveSubstring(vs2.at(j), _T("<item>"));
                    auto vs = Common::split(work2, ':');
                    auto level = std::stoi(vs.at(1));
                    auto durability = std::stoi(vs.at(2));
                    auto itemDef = Common::ItemManager()->GetItemDef(vs.at(0), level);
                    auto newSubId = Common::Inventory()->AddItem(itemDef.GetId());
                    Common::Inventory()->SetItemDurability(itemDef.GetId(), newSubId, durability);
                }
                else if (vs2.at(j).find(_T("<itemDel>")) != std::wstring::npos)
                {
                    std::wstring work2 = Common::RemoveSubstring(vs2.at(j), _T("<itemDel>"));
                    auto itemDef = Common::ItemManager()->GetItemDef(work2);

                    // subidlistが空だと落ちる。もしそうなったらスクリプトが悪い。
                    auto subidlist = Common::Inventory()->GetSubIdList(itemDef.GetId());
                    Common::Inventory()->RemoveItem(itemDef.GetId(), subidlist.at(0));
                }
                // 水を飲んだ
                else if (vs2.at(j).find(_T("<water>")) != std::wstring::npos)
                {
                    auto waterCurrent = Common::Status()->GetWaterCurrent();
                    Common::Status()->SetWaterCurrent(waterCurrent + 0.5f);
                }
                // 武器を装備
                else if (vs2.at(j).find(_T("<equip>")) != std::wstring::npos)
                {
                    std::wstring work2 = Common::RemoveSubstring(vs2.at(j), _T("<equip>"));
                    auto itemDef = Common::ItemManager()->GetItemDef(work2);
                    auto subIdList = Common::Inventory()->GetSubIdList(itemDef.GetId());
                    assert(subIdList.size() >= 1);

                    auto itemInfo = Common::Inventory()->GetItemInfo(itemDef.GetId(), subIdList.at(0));
                    Common::Status()->SetEquipWeapon(itemInfo);
                }
                else if (vs2.at(j).find(_T("<shikakumanDeath>")) != std::wstring::npos)
                {
                    auto shikakuman = NpcManager::Get()->GetNpcStatus(_T("shikakuman"));
                    shikakuman.SetDead();
                    NpcManager::Get()->SetNpcStatus(_T("shikakuman"), shikakuman);
                }
                else if (vs2.at(j).find(_T("<showMap>")) != std::wstring::npos)
                {
                    std::wstring work2 = Common::RemoveSubstring(vs2.at(j), _T("<showMap>"));
                    int id = std::stoi(work2);

                    auto mapInfoManager = NSStarmanLib::MapInfoManager::GetObj();
                    mapInfoManager->SetDiscovered(id);
                }
                else if (vs2.at(j).find(_T("<activityBase>")) != std::wstring::npos)
                {
                    std::wstring work2 = Common::RemoveSubstring(vs2.at(j), _T("<activityBase>"));
                    if (work2 == L"DirectNex")
                    {
                        auto activityBase = NSStarmanLib::ActivityBase::Get();
                        activityBase->SetBaseType(NSStarmanLib::eBaseType::DirectNex);
                    }
                }
                else if (vs2.at(j).find(_T("<hanawa>")) != std::wstring::npos)
                {
                    // 花輪があれば一つ減らす
                    auto subIdList = Common::Inventory()->GetSubIdList(12);
                    if (!subIdList.empty())
                    {
                        Common::Inventory()->RemoveItem(12, subIdList.at(0));
                        NSStarmanLib::ActivityBase::Get()->SetHanaw(true);
                        PopUp2::Get()->SetText(_T("お墓に花輪を飾った。"));
                    }
                    // インベントリになければ倉庫を見る
                    else
                    {
                        auto storeHouseManager = NSStarmanLib::StorehouseManager::Get();

                        // 海岸洞窟の倉庫を見る
                        auto storehouse = storeHouseManager->GetStorehouse(2);
                        auto subIdList2 = storehouse->GetSubIdList(12);
                        if (!subIdList2.empty())
                        {
                            storehouse->RemoveItem(12, subIdList2.at(0));
                            NSStarmanLib::ActivityBase::Get()->SetHanaw(true);
                            PopUp2::Get()->SetText(_T("お墓に花輪を飾った。"));
                        }
                    }
                }
                else if (vs2.at(j).find(_T("<sotetsu>")) != std::wstring::npos)
                {
                    auto status = Common::Status();
                    PopUp2::Get()->SetText(_T("脱水症状・頭痛・腹痛・錯乱状態になった。"));
                    PopUp2::Get()->SetText(_T("スタミナ・ミネラル・ビタミン・肉体の修復度が減少した。"));
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

    // 「調べる」アクションができることをアイコンで知らせる。
    std::wstring quest1 = QuestManager::Get()->GetQuestIdStartByExamine();
    std::wstring quest2 = QuestManager::Get()->GetQuestIdFinishByExamine();
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
    std::wstring result = m_patchManager2.Operate();

    if (result == _T("EXIT"))
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
        int pickId = 0;

        // 近くに木があったら以下を取得する
        // 木の枝 / パパイヤ / マンゴー / バナナ
        if (m_map->NearTree(SharedObj::GetPlayer()->GetPos()))
        {
            unsigned int rand = SharedObj::GetRandom();
            auto rand_ = rand % 100;

            if (rand_ < 50)
            {
                // 木の枝
                pickId = 54;
            }
            else if (rand_ < 70)
            {
                // パパイヤ
                pickId = 38;
            }
            else if (rand_ < 85)
            {
                // マンゴー
                pickId = 39;
            }
            else if (rand_ < 100)
            {
                // バナナ
                pickId = 40;
            }

            m_map->DeleteObj(m_player->GetPos(), eMapObjType::TREE);
        }
        else if (m_map->NearCoconut(SharedObj::GetPlayer()->GetPos()))
        {
            unsigned int rand = SharedObj::GetRandom();
            auto rand_ = rand % 100;

            // 10%の確率で熟したココナッツの実

            if (rand_ < 90)
            {
                pickId = 5;
            }
            else
            {
                pickId = 6;
            }

            m_map->DeleteObj(m_player->GetPos(), eMapObjType::YASHI);
        }
        // 近くに死体があったらワードブレスを取得する
        else if (m_map->NearDead(SharedObj::GetPlayer()->GetPos()))
        {
            pickId = 35;
            m_map->SetDeadItem(m_player->GetPos());
        }
        // 近くにソテツの木があったらソテツを取得する
        else if (m_map->NearSotetsu(SharedObj::GetPlayer()->GetPos()))
        {
            pickId = 3;
            m_map->DeleteObj(m_player->GetPos(), eMapObjType::SOTETSU);
        }
        // 近くにどんぐりの山があったらどんぐりなどを取得する
        else if (m_map->NearDonguri(SharedObj::GetPlayer()->GetPos()))
        {
            unsigned int rand = SharedObj::GetRandom();
            auto rand_ = rand % 100;

            if (rand_ < 25)
            {
                pickId = 7;
            }
            else if (rand_ < 50)
            {
                pickId = 8;
            }
            else if (rand_ < 75)
            {
                pickId = 9;
            }
            else if (rand_ < 99)
            {
                pickId = 54;
            }
            else if (rand_ == 99)
            {
                pickId = 35;
            }

            m_map->DeleteObj(m_player->GetPos(), eMapObjType::DONGURI);
        }
        // 砂利の近く
        else if (m_map->NearStone(SharedObj::GetPlayer()->GetPos()))
        {
            unsigned int rand = SharedObj::GetRandom();
            auto rand_ = rand % 100;

            if (rand_ < 50)
            {
                // 石
                pickId = 57;
            }
            else if (rand_ < 65)
            {
                // 縦長の石
                pickId = 41;
            }
            else if (rand_ < 80)
            {
                // いい形の石（斧）
                pickId = 33;
            }
            else if (rand_ < 100)
            {
                // いい形の石（槍）
                pickId = 34;
            }

            m_map->DeleteObj(m_player->GetPos(), eMapObjType::STONES);
        }
        else if (m_map->NearPlant(SharedObj::GetPlayer()->GetPos()))
        {
            unsigned int rand = SharedObj::GetRandom();

            // 0~99
            auto rand_ = rand % 100;

            if (rand_ < 10)
            {
                // 謎の草１
                pickId = 14;
            }
            else if (rand_ < 20)
            {
                // 謎の草２
                pickId = 15;
            }
            else if (rand_ < 30)
            {
                pickId = 16;
            }
            else if (rand_ < 40)
            {
                pickId = 17;
            }
            else if (rand_ < 45)
            {
                pickId = 18;
            }
            else if (rand_ < 50)
            {
                pickId = 19;
            }
            else if (rand_ < 55)
            {
                // ツクシ
                pickId = 13;
            }
            else if (rand_ < 60)
            {
                // ハイビスカス
                pickId = 11;
            }
            else if (rand_ < 65)
            {
                // タンポポ
                pickId = 20;
            }
            else if (rand_ < 70)
            {
                // ニラ、もしくはスイセン
                pickId = 21;
            }
            else if (rand_ < 75)
            {
                // キノコ
                pickId = 22;
            }
            else if (rand_ < 90)
            {
                // ツタ
                pickId = 56;
            }
            else if (rand_ <= 98)
            {
                // 木の枝
                pickId = 54;
            }
            else if (rand_ <= 99)
            {
                // ワードブレス
                pickId = 35;
            }

            // 草を消す処理
            m_map->DeletePlant(m_player->GetPos());
        }
        else
        {
            auto ppos = SharedObj::GetPlayer()->GetPos();

            // 海岸洞窟のあたり
            {
                D3DXVECTOR3 pos = D3DXVECTOR3(1216.f, 10.8f, 582.1f);
                if (Common::HitByBoundingBox(ppos, pos, 100.f))
                {
                    // 黒い貝
                    pickId = 42;
                }
            }

            // 港跡のあたり
            {
                D3DXVECTOR3 pos = D3DXVECTOR3(1894.3f, 13.5f, -899.2f);
                if (Common::HitByBoundingBox(ppos, pos, 100.f))
                {
                    // ブイ
                    pickId = 25;
                }
            }
        }

        auto itemDef = Common::ItemManager()->GetItemDef(pickId);
        std::wstring pick = itemDef.GetName();

        PopUp2::Get()->SetText(Common::LoadStringWithArg(IDS_STRING128, pick));

        // 30分経過させる処理
        auto dateTime = NSStarmanLib::PowereggDateTime::GetObj();
        dateTime->IncreaseDateTime(0, 0, 0, 30, 0);

        // 体力を消費
        // 装備武器で消費する時間や体力が変わる
        Common::Status()->PickPlant();

        // アイテムをインベントリに追加
        Common::Inventory()->AddItem(itemDef.GetId());

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

        // 6時間経過させる処理
        // 装備武器で消費する時間や体力が変わる
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

            // 時間を経過させ、石斧の耐久値を減らす
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

        // 体力を消費
        // 装備武器で消費する時間や体力が変わる
        Common::Status()->CutTree(name, level);

        // アイテムをインベントリに追加
        auto itemDef = Common::ItemManager()->GetItemDef(Common::LoadString_(IDS_STRING132));
        Common::Inventory()->AddItem(itemDef.GetId());

        // 木を消す処理
        m_map->DeleteThinTree(m_player->GetPos());

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
        // 3時間後の位置にワープ
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
        // 装備中の木の棒を削除し、松明を生成。
        // そして、松明を装備する
        //-----------------------------------------------

        // 装備中の木の棒を削除
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
    else if (m_eState == eBattleState::REST)
    {
        RenderRest();
    }
    else if (m_eState == eBattleState::VOYAGE)
    {
        // イカダは乗っていても乗っていなくても常に表示するべきものなので
        // ここでは何もしない。
    }
    else if (m_eState == eBattleState::POPUP)
    {
        RenderPopUp();
    }

    RenderCommon2D();
}

void SeqBattle::Confirm(eSequence* sequence)
{
    // 調べるコマンド
    // プレイヤーの現在座標で始まるクエストか終わるクエストがある。
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
            
            // どれだけ荷物が重くても落ちているものを拾うことはできる。
            // 代わりに、まともに歩いたりできなくなる。
            auto inventory = NSStarmanLib::Inventory::GetObj();
            int newSubID = inventory->AddItem(itemPos.GetItemDefId());
            m_menuManager->AddItem(itemPos.GetItemDefId(), newSubID);

            std::wstring work = itemManager->GetItemDef(itemPos.GetItemDefId()).GetName();
            SoundEffect::get_ton()->play(_T("res\\sound\\menu_cursor_confirm.wav"));
            PopUp2::Get()->SetText(Common::LoadStringWithArg(IDS_STRING128, work));
        }
    }
    else if (m_bTalkable)
    {
        // 会話を開始
        m_bTalkable = false;

        // 会話による体力の消費
        Common::Status()->Talk();

        auto npcManager = NpcManager::Get();
        std::wstring npcName;
        D3DXVECTOR3 playerPos = SharedObj::GetPlayer()->GetPos();
        npcManager->GetNpcTalkable(playerPos, &npcName);
        if (npcName.empty() == false)
        {
            auto npcStatus = npcManager->GetNpcStatus(npcName);
            std::wstring csvfile = npcStatus.GetTalkCsv();

            NSTalkLib2::IFont* pFont = NEW NSTalkLib2::Font(SharedObj::GetD3DDevice());
            NSTalkLib2::ISoundEffect* pSE = NEW NSTalkLib2::SoundEffect();
            NSTalkLib2::ISprite* sprite = NEW NSTalkLib2::Sprite(SharedObj::GetD3DDevice());

            m_talk = NEW NSTalkLib2::Talk();
            m_talk->Init(Common::ModExt(csvfile), pFont, pSE, sprite,
                         _T("res\\image\\textBack.png"), _T("res\\image\\black.png"),
                         Common::DeployEncryptMode(),
                         SharedObj::IsEnglish());

            m_eState = eBattleState::TALK;

            // 会話したので会話可能状態を解除する
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
            
            // どれだけ荷物が重くても落ちているものを拾うことはできる。
            // 代わりに、まともに歩いたりできなくなる。
            auto inventory = NSStarmanLib::Inventory::GetObj();
            int newSubID = inventory->AddItem(thrownItem.GetId());
            m_menuManager->AddItem(thrownItem.GetId(), newSubID);

            std::wstring work = itemManager->GetItemDef(thrownItem.GetId()).GetName();
            SoundEffect::get_ton()->play(_T("res\\sound\\menu_cursor_confirm.wav"));
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
        // タイトルでもポップアップを使う
        PopUp2::Get()->Update();
        return;
    }

    if (m_eState == eBattleState::TALK)
    {
        return;
    }

    m_map->Update();

    PopUp2::Get()->Update();

    // 60回に一回（＝1秒ごと）の処理
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

    // クラフトはクラフト画面が表示されていなくても更新処理が行われる必要がある。
    m_craft.Update();

    // クラフトによってイカダが生成されることがあるため、航海中でなくても更新処理を行う。
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

    // TODO 微調整
    if (m_eState != eBattleState::LOAD &&
        // m_eState != eBattleState::TITLE && // タイトル画面でもポップアップが必要
        // m_eState != eBattleState::MENU && // メニュー画面でもポップアップが必要
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
    // 復活可能か
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
            std::vector<std::vector<std::wstring>> vvs;
            std::vector<std::wstring> vs;
            vs.push_back(_T("サンプルテキスト１"));
            vs.push_back(_T("サンプルテキスト２サンプルテキスト２"));
            vs.push_back(_T("サンプルテキスト３サンプルテキスト３サンプルテキスト３"));
            vvs.push_back(vs);
            vs.clear();
            vs.push_back(_T("サンプルテキスト４"));
            vs.push_back(_T("サンプルテキスト５サンプルテキスト５"));
            vs.push_back(_T("サンプルテキスト６サンプルテキスト６サンプルテキスト６"));
            vvs.push_back(vs);
            vs.clear();
            vs.push_back(_T("サンプルテキスト７サンプルテキスト７サンプルテキスト７サンプルテキスト７"));
            vs.push_back(_T("サンプルテキスト８サンプルテキスト８サンプルテキスト８"));
            vs.push_back(_T("サンプルテキスト９サンプルテキスト９"));
            vvs.push_back(vs);
            vs.clear();
            vs.push_back(_T("サンプルテキスト１０"));
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
    // 倉庫機能
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

                // TODO 倉庫を表示する度に倉庫画面を作るのをやめる
                m_storehouse = NEW NSStorehouseLib::StorehouseLib();

                NSStorehouseLib::Sprite* sprCursor = NEW NSStorehouseLib::Sprite(SharedObj::GetD3DDevice());
                sprCursor->Load(_T("res\\image\\menu_cursor.png"));

                NSStorehouseLib::Sprite* sprBackground = NEW NSStorehouseLib::Sprite(SharedObj::GetD3DDevice());
                sprBackground->Load(_T("res\\image\\background.png"));

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
                                std::wstring work_str;

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

                    // TODO 倉庫の複数化対応
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
                                std::wstring work_str;

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
    // クラフト機能
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
    // パッチテスト機能
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
    // 時刻を進める
    //-------------------------------------
    // 2時間ゲームをしたらパワーエッグ星で24時間経過する
    NSStarmanLib::PowereggDateTime* dateTime = NSStarmanLib::PowereggDateTime::GetObj();

    if (Common::DebugMode() == false)
    {
        dateTime->IncreaseDateTime(0, 0, 0, 0, 12);
    }
    else
    {
//        dateTime->IncreaseDateTime(0, 0, 0, 30, 0); // 1秒で1時間とか経過させたい時用
        dateTime->IncreaseDateTime(0, 0, 0, 0, 12);
    }

    //-------------------------------------
    // ステータスを更新
    //-------------------------------------
    NSStarmanLib::StatusManager* statusManager = NSStarmanLib::StatusManager::GetObj();
    statusManager->Update();

    //-------------------------------------
    // NPCのステータスを更新
    //-------------------------------------
    NpcManager::Get()->Update();

    //-------------------------------------
    // 死亡チェック
    //-------------------------------------
    bool dead = statusManager->GetDead();
    if (dead)
    {
        if (m_eState != eBattleState::DEAD)
        {
            auto reason = statusManager->GetDeadReason();

            if (reason == NSStarmanLib::eDeadReason::NONE)
            {
                PopUp2::Get()->SetText(_T("死因：不明"));
            }
            else if (reason == NSStarmanLib::eDeadReason::MUSCLE_ZERO)
            {
                PopUp2::Get()->SetText(_T("死因：肉体の修復度が０になった"));
            }
            else if (reason == NSStarmanLib::eDeadReason::WATER_90)
            {
                PopUp2::Get()->SetText(_T("死因：水分量が９０％以下になった"));
            }
            else if (reason == NSStarmanLib::eDeadReason::KAROSHI)
            {
                PopUp2::Get()->SetText(_T("死因：身体のスタミナが０％になった"));
            }
            else if (reason == NSStarmanLib::eDeadReason::RYNEN_1_YEAR)
            {
                PopUp2::Get()->SetText(_T("死因：ライネンの契約をして１年が経過した"));
            }
            else if (reason == NSStarmanLib::eDeadReason::STARVATION)
            {
                PopUp2::Get()->SetText(_T("死因：餓死"));
            }
            else if (reason == NSStarmanLib::eDeadReason::DROWNING)
            {
                PopUp2::Get()->SetText(_T("死因：水中で寝た。"));
            }
            else if (reason == NSStarmanLib::eDeadReason::ATTACK_ON_SLEEP)
            {
                PopUp2::Get()->SetText(_T("死因：睡眠中に攻撃された。"));
            }

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
    // 睡眠・気絶チェック
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
    // アイテム発見
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
    // 投げて地面に落ちたアイテムを発見
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
    // 最大積載量を超えていたらメッセージを表示
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
    // 松明を使用していたら松明の耐久値を低下
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
            }

            Common::Inventory()->SetItemDurability(weapon.GetId(), weapon.GetSubId(), dura);
            weapon.SetDurabilityCurrent(dura);
            Common::Status()->SetEquipWeapon(weapon);
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
    // キーボード、マウス、ゲームパッドの処理
    //============================================

    //--------------------------------------------
    // KeyBoard
    //--------------------------------------------

    //--------------------------------------
    // メニュー機能
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
    // コマンド機能
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

    // 確定操作
    if (SharedObj::KeyBoard()->IsDownFirstFrame(DIK_E))
    {
        Confirm(sequence);
    }

    //--------------------------------------------
    // GamePad
    //--------------------------------------------

    //--------------------------------------
    // メニュー機能
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
    // コマンド機能
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

    // 確定操作
    if (GamePad::IsDown(eGamePadButtonType::A))
    {
        Confirm(sequence);
    }

    m_player->Update(m_map);

    m_hudManager.Update();

    //-------------------------------------------------
    // 0.5秒に1回くらいの処理
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
            // クエスト処理
            {
                OperateQuest(sequence);
            }

            // 近くにチェストがあったら倉庫機能が使える。
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
    // 「話しかける」ができることをアイコンで知らせる
    //---------------------------------------------------------------------
    {
        // 3メートル以内に話しかけられるNPCがいるか。
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

        SAFE_DELETE(m_map);
        m_map = NEW Map();
        SharedObj::SetMap(m_map);
        m_map->Init();

        m_Opening = NEW Opening();
    }
    else if (m_eState == eBattleState::NORMAL)
    {
        SAFE_DELETE(m_title);

        Camera::SetCameraMode(eCameraMode::BATTLE);
        Common::SetCursorVisibility(false);
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

        // 復活ナシ
        if (rynen->GetReviveEnable() == false)
        {
            // 確定操作
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

            // 確定操作
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
        // 復活あり
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

void SeqBattle::OperateRest()
{
    if (m_eFadeSeq == eFadeSeq::Finish)
    {
        // 3時間経過させる処理
        auto dateTime = NSStarmanLib::PowereggDateTime::GetObj();
        dateTime->IncreaseDateTime(0, 0, 3, 0, 0);

        auto status = NSStarmanLib::StatusManager::GetObj();

        // 体力を消費
        // 装備武器で消費する時間や体力が変わる
        Common::Status()->Rest3Hours();

        m_eState = eBattleState::NORMAL;
        Camera::SetCameraMode(eCameraMode::BATTLE);
        Common::SetCursorVisibility(false);
    }
}

void SeqBattle::RenderRest()
{
}


