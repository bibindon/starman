// F7ÉLÅ[Ç≈ÉTÉìÉvÉãÉeÉLÉXÉg

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

        void Init()
        {
            HRESULT hr = D3DXCreateFont(
                m_pD3DDevice,
                24,
                0,
                FW_NORMAL,
                1,
                false,
                SHIFTJIS_CHARSET,
                OUT_TT_ONLY_PRECIS,
                ANTIALIASED_QUALITY,
                FF_DONTCARE,
                "ÇlÇr ñæí©",
                &m_pFont);
        }

        virtual void DrawText_(const std::string& msg, const int x, const int y)
        {
            RECT rect = { x, y, 0, 0 };
            m_pFont->DrawText(NULL, msg.c_str(), -1, &rect, DT_LEFT | DT_NOCLIP,
                D3DCOLOR_ARGB(255, 255, 255, 255));
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

namespace NSCraftLib
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
            m_D3DSprite->Release();
            m_pD3DTexture->Release();
        }

    private:

        LPDIRECT3DDEVICE9 m_pD3DDevice = NULL;
        LPD3DXSPRITE m_D3DSprite = NULL;
        LPDIRECT3DTEXTURE9 m_pD3DTexture = NULL;
        UINT m_width = 0;
        UINT m_height = 0;
    };

    class Font : public IFont
    {
    public:

        Font(LPDIRECT3DDEVICE9 pD3DDevice)
            : m_pD3DDevice(pD3DDevice)
        {
        }

        void Init()
        {
            HRESULT hr = D3DXCreateFont(
                m_pD3DDevice,
                24,
                0,
                FW_NORMAL,
                1,
                false,
                SHIFTJIS_CHARSET,
                OUT_TT_ONLY_PRECIS,
                ANTIALIASED_QUALITY,
                FF_DONTCARE,
                "ÇlÇr ñæí©",
                &m_pFont);
        }

        virtual void DrawText_(const std::string& msg, const int x, const int y)
        {
            RECT rect = { x, y, 0, 0 };
            m_pFont->DrawText(NULL, msg.c_str(), -1, &rect, DT_LEFT | DT_NOCLIP,
                D3DCOLOR_ARGB(255, 255, 255, 255));
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
        m_D3DSprite->Draw(m_pD3DTexture, &rect, &center, &pos, D3DCOLOR_ARGB(transparency, 255, 255, 255));
        m_D3DSprite->End();

    }

    void Load(const std::string& filepath) override
    {
        if (FAILED(D3DXCreateSprite(m_pD3DDevice, &m_D3DSprite)))
        {
            throw std::exception("Failed to create a sprite.");
        }

        if (FAILED(D3DXCreateTextureFromFile(m_pD3DDevice, filepath.c_str(), &m_pD3DTexture)))
        {
            throw std::exception("Failed to create a texture.");
        }

        D3DSURFACE_DESC desc { };
        if (FAILED(m_pD3DTexture->GetLevelDesc(0, &desc)))
        {
            throw std::exception("Failed to create a texture.");
        }
        m_width = desc.Width;
        m_height = desc.Height;
    }

    ~Sprite() override
    {
        if (m_D3DSprite != nullptr)
        {
            m_D3DSprite->Release();
            m_D3DSprite = nullptr;
        }

        if (m_pD3DTexture != nullptr)
        {
            m_pD3DTexture->Release();
            m_pD3DTexture = nullptr;
        }
    }

    virtual ISprite* Create() override
    {
        return NEW Sprite(m_pD3DDevice);
    }
private:
    LPDIRECT3DDEVICE9 m_pD3DDevice = NULL;
    LPD3DXSPRITE m_D3DSprite = NULL;
    LPDIRECT3DTEXTURE9 m_pD3DTexture = NULL;
    UINT m_width { 0 };
    UINT m_height { 0 };

};

class Font : public IFont
{
public:
    Font(LPDIRECT3DDEVICE9 pD3DDevice)
        : m_pD3DDevice(pD3DDevice)
    {
    }

    void Init()
    {
        HRESULT hr = D3DXCreateFont(m_pD3DDevice,
            24,
            0,
            FW_NORMAL,
            1,
            false,
            SHIFTJIS_CHARSET,
            OUT_TT_ONLY_PRECIS,
            ANTIALIASED_QUALITY,
            FF_DONTCARE,
            "ÇlÇr ñæí©",
            &m_pFont);
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

    BGM::get_ton()->load("res\\sound\\title.wav");
    BGM::get_ton()->play(10);

    ::SoundEffect::get_ton()->load("res\\sound\\drink.wav");

    m_spriteGameover = NEW Sprite("res\\image\\gameover.png");
    m_spriteExamine = NEW Sprite("res\\image\\examine.png");

    NSTalkLib2::IFont* pFont = NEW NSTalkLib2::Font(SharedObj::GetD3DDevice());
    NSTalkLib2::ISoundEffect* pSE = NEW NSTalkLib2::SoundEffect();
    NSTalkLib2::ISprite* sprite = NEW NSTalkLib2::Sprite(SharedObj::GetD3DDevice());

    m_hudManager.Init();

    {
        std::vector<std::string> vs;
        std::vector<bool> vb;

        vs.push_back("î∞çÃ");
        vb.push_back(true);

        vs.push_back("â°Ç…Ç»ÇÈ");
        vb.push_back(true);

        vs.push_back("ç¿ÇÈ");
        vb.push_back(true);

        vs.push_back("çÃèW");
        vb.push_back(true);

        vs.push_back("â¡çH");
        vb.push_back(false);

        vs.push_back("í≤óù");
        vb.push_back(false);

        vs.push_back("íﬁÇË");
        vb.push_back(true);

        m_commandManager.Init(vs, vb);
    }

    Common::SetCursorVisibility(false);

    m_eState = eBattleState::LOAD;
    InitLoad();

    m_sprSleepBlack = NEW Sprite("res\\image\\black.png");
}

SeqBattle::~SeqBattle()
{
    m_loadThread->join();

    m_hudManager.Finalize();
    m_commandManager.Finalize();
    SAFE_DELETE(m_loadThread);
    SAFE_DELETE(m_player);
    SAFE_DELETE(m_map);
    SAFE_DELETE(m_enemyManager);

    SAFE_DELETE(m_talk);
    SAFE_DELETE(m_storehouse);
    SAFE_DELETE(m_craft);
    SAFE_DELETE(m_title);
    SAFE_DELETE(m_Opening);

    SAFE_DELETE(m_spriteGameover);
    SAFE_DELETE(m_spriteExamine);

    SAFE_DELETE(m_sprLoadClock);
    SAFE_DELETE(m_sprLoadLoading);
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
    else if (m_eState == eBattleState::CRAFT)
    {
        OperateCraft();
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
    else if (m_eState == eBattleState::GAMEOVER)
    {
        OperateGameover(sequence);
    }
    else if (m_eState == eBattleState::SLEEP)
    {
        OperateSleep();
    }

    if (Common::DebugMode())
    {
        UpdateDebug();
    }
}


void SeqBattle::OperateStory()
{
    m_story->Update();
}

void SeqBattle::OperateMenu(eSequence* sequence)
{
    std::string result = m_menuManager.OperateMenu();
    if (result == "É^ÉCÉgÉãÇ…ñﬂÇÈ")
    {
        m_eState = eBattleState::TITLE;
        Camera::SetCameraMode(eCameraMode::BATTLE_TO_TITLE);
        Common::SetCursorVisibility(true);
        m_title = NEW Title(false);
    }
    else if (result == "ÉZÅ[ÉuÇµÇƒèIóπ")
    {
//        m_eState = eBattleState::TITLE;
//        Camera::SetCameraMode(eCameraMode::BATTLE_TO_TITLE);
//        Common::SetCursorVisibility(true);
//        InitTitle();
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
        }
    }

    if (KeyBoard::IsDownFirstFrame(DIK_SPACE))
    {
        m_talk->Next();
    }

    if (KeyBoard::IsDownFirstFrame(DIK_RETURN))
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

    if (Common::DeployMode() == false)
    {
        if (KeyBoard::IsDownFirstFrame(DIK_F1))
        {
            m_eState = eBattleState::STOREHOUSE;
            Camera::SetCameraMode(eCameraMode::SLEEP);
            Common::SetCursorVisibility(false);
        }
    }

    if (KeyBoard::IsDownFirstFrame(DIK_UP))
    {
        m_storehouse->Up();
    }

    if (KeyBoard::IsDownFirstFrame(DIK_DOWN))
    {
        m_storehouse->Down();
    }

    if (KeyBoard::IsDownFirstFrame(DIK_LEFT))
    {
        m_storehouse->Left();
    }

    if (KeyBoard::IsDownFirstFrame(DIK_RIGHT))
    {
        m_storehouse->Right();
    }

    if (KeyBoard::IsDownFirstFrame(DIK_RETURN))
    {
        result = m_storehouse->Into();
        std::vector<std::string> vs = Common::split(result, ':');

        int id_ = 0;
        int subId_ = 0;
        int durability_ = 0;

        id_ = std::stoi(vs.at(2));
        subId_ = std::stoi(vs.at(3));

        NSStarmanLib::Inventory* inventory = NSStarmanLib::Inventory::GetObj();
        NSStarmanLib::Storehouse* storehouse = NSStarmanLib::Storehouse::GetObj();
        if (vs.at(0) == "left")
        {
            NSStarmanLib::ItemInfo itemInfo = inventory->GetItemInfo(id_, subId_);
            durability_ = itemInfo.GetDurabilityCurrent();
            inventory->RemoveItem(id_, subId_);
            storehouse->AddItemWithSubID(id_, subId_, durability_);
            m_storehouse->MoveFromInventoryToStorehouse(id_, subId_);
            m_menuManager.DeleteItem(id_, subId_);
        }
        else if (vs.at(0) == "right")
        {
            NSStarmanLib::ItemInfo itemInfo = storehouse->GetItemInfo(id_, subId_);
            durability_ = itemInfo.GetDurabilityCurrent();
            storehouse->RemoveItem(id_, subId_);
            inventory->AddItemWithSubID(id_, subId_, durability_);
            m_storehouse->MoveFromStorehouseToInventory(id_, subId_);
            m_menuManager.AddItem(id_, subId_, durability_);
        }
        else
        {
            throw std::exception();
        }
    }

    if (KeyBoard::IsDownFirstFrame(DIK_ESCAPE))
    {
        result = m_storehouse->Back();
        m_eState = eBattleState::NORMAL;
    }

    //---------------------------------------------------------
    // Mouse
    //---------------------------------------------------------

    if (Mouse::IsDownLeft())
    {
        POINT p = Common::GetScreenPos();;
        m_storehouse->Click(p.x, p.y);
    }
    else
    {
        POINT p = Common::GetScreenPos();;
        m_storehouse->CursorOn(p.x, p.y);
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
        NSStarmanLib::Storehouse* storehouse = NSStarmanLib::Storehouse::GetObj();
        if (vs.at(0) == "left")
        {
            NSStarmanLib::ItemInfo itemInfo = inventory->GetItemInfo(id_, subId_);
            durability_ = itemInfo.GetDurabilityCurrent();
            inventory->RemoveItem(id_, subId_);
            storehouse->AddItemWithSubID(id_, subId_, durability_);
            m_storehouse->MoveFromInventoryToStorehouse(id_, subId_);
            m_menuManager.DeleteItem(id_, subId_);
        }
        else if (vs.at(0) == "right")
        {
            NSStarmanLib::ItemInfo itemInfo = storehouse->GetItemInfo(id_, subId_);
            durability_ = itemInfo.GetDurabilityCurrent();
            storehouse->RemoveItem(id_, subId_);
            inventory->AddItemWithSubID(id_, subId_, durability_);
            m_storehouse->MoveFromStorehouseToInventory(id_, subId_);
            m_menuManager.AddItem(id_, subId_, durability_);
        }
        else
        {
            throw std::exception();
        }
    }

    if (GamePad::IsDown(eGamePadButtonType::B))
    {
        result = m_storehouse->Back();
        m_eState = eBattleState::NORMAL;
    }

    return;
}

void SeqBattle::OperateCraft()
{
    std::string result;

    if (KeyBoard::IsDownFirstFrame(DIK_F2))
    {
        m_eState = eBattleState::NORMAL;
        Camera::SetCameraMode(eCameraMode::SLEEP);
        Common::SetCursorVisibility(false);
    }

    if (KeyBoard::IsDownFirstFrame(DIK_UP))
    {
        m_craft->Up();
    }

    if (KeyBoard::IsDownFirstFrame(DIK_DOWN))
    {
        m_craft->Down();
    }

    if (KeyBoard::IsDownFirstFrame(DIK_LEFT))
    {
        m_craft->Left();
    }

    if (KeyBoard::IsDownFirstFrame(DIK_RIGHT))
    {
        m_craft->Right();
    }

    if (KeyBoard::IsDownFirstFrame(DIK_RETURN))
    {
        m_craft->Into();
    }

    if (KeyBoard::IsDownFirstFrame(DIK_ESCAPE))
    {
        result = m_craft->Back();
    }

    if (KeyBoard::IsDownFirstFrame(DIK_BACK))
    {
        result = m_craft->Back();
    }

    if (Mouse::IsDownLeft())
    {
        POINT p = Common::GetScreenPos();
        m_craft->Click(p.x, p.y);
    }
    else
    {
        POINT p = Common::GetScreenPos();;
        m_craft->CursorOn(p.x, p.y);
    }

    if (Mouse::GetZDelta() < 0)
    {
        m_craft->Next();
    }
    else if (Mouse::GetZDelta() > 0)
    {
        m_craft->Previous();
    }

    if (GamePad::IsDown(eGamePadButtonType::UP))
    {
        m_craft->Up();
    }

    if (GamePad::IsDown(eGamePadButtonType::DOWN))
    {
        m_craft->Down();
    }

    if (GamePad::IsDown(eGamePadButtonType::LEFT))
    {
        m_craft->Left();
    }

    if (GamePad::IsDown(eGamePadButtonType::RIGHT))
    {
        m_craft->Right();
    }

    if (GamePad::IsDown(eGamePadButtonType::A))
    {
        m_craft->Into();
    }

    if (GamePad::IsDown(eGamePadButtonType::B))
    {
        result = m_craft->Back();
    }

    if (result == "EXIT")
    {
        m_eState = eBattleState::NORMAL;
        Camera::SetCameraMode(eCameraMode::BATTLE);
        Common::SetCursorVisibility(false);
    }

    return;
}

void SeqBattle::OperateCommand()
{
    ++m_commandCounter;

    std::string result = m_commandManager.Operate();

    bool leave = false;

    if (result == "EXIT")
    {
        leave = true;
    }
    else if (result == "ç¿ÇÈ")
    {
        leave = true;
        m_player->SetSit();
    }
    else if (result == "â°Ç…Ç»ÇÈ")
    {
        leave = true;
        m_player->SetLieDown();
    }
    else if (result == "íEèo")
    {
        leave = true;
        D3DXVECTOR3 pos(-285.f, 16.f, 539.f);
        m_player->SetPos(pos);
    }

    if (leave)
    {
        m_eState = eBattleState::NORMAL;
        Camera::SetCameraMode(eCameraMode::BATTLE);
        Common::SetCursorVisibility(false);

        m_commandCounter = 0;

        if (m_commandShowEscape)
        {
            m_commandShowEscape = false;
            std::vector<std::string> vs;
            std::vector<bool> vb;

            vs.push_back("î∞çÃ");
            vb.push_back(true);

            vs.push_back("â°Ç…Ç»ÇÈ");
            vb.push_back(true);

            vs.push_back("ç¿ÇÈ");
            vb.push_back(true);

            vs.push_back("çÃèW");
            vb.push_back(true);

            vs.push_back("â¡çH");
            vb.push_back(false);

            vs.push_back("í≤óù");
            vb.push_back(false);

            vs.push_back("íﬁÇË");
            vb.push_back(true);

            m_commandManager.Init(vs, vb);
        }
    }

    // ÉRÉ}ÉìÉhâÊñ Ç™1ï™ï\é¶Ç≥ÇÍÇƒÇ¢ÇΩÇÁíEèoÉRÉ}ÉìÉhÇï\é¶Ç∑ÇÈ
    if (m_commandCounter >= 60 * 20)
    {
        if (m_commandShowEscape == false)
        {
            m_commandShowEscape = true;
            std::vector<std::string> vs;
            std::vector<bool> vb;

            vs.push_back("î∞çÃ");
            vb.push_back(true);

            vs.push_back("â°Ç…Ç»ÇÈ");
            vb.push_back(true);

            vs.push_back("ç¿ÇÈ");
            vb.push_back(true);

            vs.push_back("çÃèW");
            vb.push_back(true);

            vs.push_back("â¡çH");
            vb.push_back(false);

            vs.push_back("í≤óù");
            vb.push_back(false);

            vs.push_back("íﬁÇË");
            vb.push_back(true);

            vs.push_back("íEèo");
            vb.push_back(true);

            m_commandManager.Init(vs, vb);
        }
    }
}

void SeqBattle::InitLoad()
{
    // ÉZÅ[ÉuÉfÅ[É^Ç™Ç†ÇÈÇ©î€Ç©
    int saveExist = 0;
    if (Common::DeployMode())
    {
        saveExist = PathFileExists("res\\script\\save");
    }
    else
    {
        // îpé~
        // saveExist = PathFileExists("res\\script\\save_debug");
        saveExist = PathFileExists("res\\script\\save");
    }

    if (saveExist == FALSE)
    {
        m_bSavedataExists = false;
        SAFE_DELETE(m_loadThread);
        m_loadThread = NEW std::thread(
            [&]
            {
                SaveManager::Get()->LoadOrigin();
                m_loadLoaded.store(true);
            });
    }
    else
    {
        m_bSavedataExists = true;
        SAFE_DELETE(m_loadThread);
        m_loadThread = NEW std::thread(
            [&]
            {
                SaveManager::Get()->Load();
                m_loadLoaded.store(true);
            });
    }

    m_sprLoadBack = NEW Sprite("res\\image\\black.png");
    m_sprLoadLogo = NEW Sprite("res\\image\\title01.png");
    m_sprLoadClock = NEW Sprite("res\\image\\load_clock.png");
    m_sprLoadLoading = NEW Sprite("res\\image\\loading.png");

    Common::SetCursorVisibility(false);
}

void SeqBattle::UpdateLoad()
{
    if (m_loadLoaded.load())
    {
        InitializeAfterLoad();
        FinalizeLoad();
        m_eState = eBattleState::TITLE;
        m_title = NEW Title(true);
    }
}

void SeqBattle::InitializeAfterLoad()
{
    m_menuManager.InitMenu();

    m_map = NEW Map();
    SharedObj::SetMap(m_map);
    m_map->Init();

    auto status = NSStarmanLib::StatusManager::GetObj();
    D3DXVECTOR3 pos;
    status->GetXYZ(&pos.x, &pos.y, &pos.z);
    m_player->SetPos(pos);

    pos.y += 1.f;
    Camera::SetLookAtPos(pos);

    Camera::SetCameraMode(eCameraMode::TITLE);
}

void SeqBattle::RenderLoad()
{
    D3DXVECTOR3 pos;
    pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_sprLoadBack->Render(pos);

    // Ç≠ÇÈÇ≠ÇÈÇï\é¶
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

        BGM::get_ton()->load("res\\sound\\novel.wav");
        BGM::get_ton()->play(10);
    }
}

void SeqBattle::OperateSleep()
{
    auto statusManager = NSStarmanLib::StatusManager::GetObj();
    {
        if (m_eSleepSeq == eSleepSeq::FadeOut)
        {
            ++m_sleepFadeOut;
            if (m_sleepFadeOut >= 300)
            {
                m_eSleepSeq = eSleepSeq::Sleep;
            }
        }
        else if (m_eSleepSeq == eSleepSeq::Sleep)
        {
            ++m_sleepBlack;
            if (m_sleepBlack >= 60)
            {
                m_eSleepSeq = eSleepSeq::FadeIn;
            }
            else if (m_sleepBlack == 1)
            {
                auto status = NSStarmanLib::StatusManager::GetObj();
                status->Sleep();
            }
        }
        else if (m_eSleepSeq == eSleepSeq::FadeIn)
        {
            ++m_sleepFadeIn;
            if (m_sleepFadeIn >= 60)
            {
                m_eSleepSeq = eSleepSeq::Finish;
            }
        }
        else if (m_eSleepSeq == eSleepSeq::Finish)
        {
            m_eState = eBattleState::NORMAL;
            m_player->SetSleep(false);
            m_eSleepSeq = eSleepSeq::NotStart;
            m_sleepFadeOut = 0;
            m_sleepBlack = 0;
            m_sleepFadeIn = 0;
        }
    }
}

void SeqBattle::RenderSleep()
{
    D3DXVECTOR3 pos(0.f, 0.f, 0.f);
    int transparency = 0;
    if (m_eSleepSeq == eSleepSeq::FadeOut)
    {
        transparency = m_sleepFadeOut * 255 / 300;
        m_sprSleepBlack->Render(pos, transparency);
    }
    else if (m_eSleepSeq == eSleepSeq::Sleep)
    {
        m_sprSleepBlack->Render(pos);
    }
    else if (m_eSleepSeq == eSleepSeq::FadeIn)
    {
        transparency = 255 - (m_sleepFadeIn * 255 / 60);
        m_sprSleepBlack->Render(pos, transparency);
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
        m_menuManager.Draw();
    }
    else if (m_eState == eBattleState::STOREHOUSE)
    {
        m_storehouse->Draw();
    }
    else if (m_eState == eBattleState::CRAFT)
    {
        m_craft->Draw();
    }
    else if (m_eState == eBattleState::COMMAND)
    {
        m_commandManager.Draw();
    }
    else if (m_eState == eBattleState::NORMAL)
    {
        RenderNormal();
    }
    else if (m_eState == eBattleState::GAMEOVER)
    {
        RenderGameover();
    }
    else if (m_eState == eBattleState::SLEEP)
    {
        RenderSleep();
    }
}

void SeqBattle::InputR1()
{
    bool ret = m_player->SetAttack();
}

void SeqBattle::Confirm(eSequence* sequence)
{
    // í≤Ç◊ÇÈÉRÉ}ÉìÉh
    // ÉvÉåÉCÉÑÅ[ÇÃåªç›ç¿ïWÇ≈énÇ‹ÇÈÉNÉGÉXÉgÇ©èIÇÌÇÈÉNÉGÉXÉgÇ™Ç†ÇÈÅB
    if (m_bShowExamine)
    {
        m_bShowExamine = false;
        D3DXVECTOR3 playerPos = SharedObj::GetPlayer()->GetPos();
        SharedObj::GetQuestSystem()->SetExamine(playerPos.x, playerPos.y, playerPos.z);
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
            
            // Ç«ÇÍÇæÇØâ◊ï®Ç™èdÇ≠ÇƒÇ‡óéÇøÇƒÇ¢ÇÈÇ‡ÇÃÇèEÇ§Ç±Ç∆ÇÕÇ≈Ç´ÇÈÅB
            // ë„ÇÌÇËÇ…ÅAÇ‹Ç∆Ç‡Ç…ï‡Ç¢ÇΩÇËÇ≈Ç´Ç»Ç≠Ç»ÇÈÅB
            auto inventory = NSStarmanLib::Inventory::GetObj();
            int newSubID = inventory->AddItem(itemPos.GetItemDefId());
            m_menuManager.AddItem(itemPos.GetItemDefId(), newSubID);

            std::string work = itemManager->GetItemDef(itemPos.GetItemDefId()).GetName();
            SoundEffect::get_ton()->play("res\\sound\\menu_cursor_confirm.wav");
            PopUp2::Get()->SetText(work + " ÇéËÇ…ì¸ÇÍÇΩÅB");
        }
    }
    else if (m_bTalkable)
    {
        // âÔòbÇäJén
        m_bTalkable = false;
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
            m_talk->Init(csvfile, pFont, pSE, sprite,
                         "res\\image\\textBack.png", "res\\image\\black.png");

            m_eState = eBattleState::TALK;
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
            
            // Ç«ÇÍÇæÇØâ◊ï®Ç™èdÇ≠ÇƒÇ‡óéÇøÇƒÇ¢ÇÈÇ‡ÇÃÇèEÇ§Ç±Ç∆ÇÕÇ≈Ç´ÇÈÅB
            // ë„ÇÌÇËÇ…ÅAÇ‹Ç∆Ç‡Ç…ï‡Ç¢ÇΩÇËÇ≈Ç´Ç»Ç≠Ç»ÇÈÅB
            auto inventory = NSStarmanLib::Inventory::GetObj();
            int newSubID = inventory->AddItem(thrownItem.GetId());
            m_menuManager.AddItem(thrownItem.GetId(), newSubID);

            std::string work = itemManager->GetItemDef(thrownItem.GetId()).GetName();
            SoundEffect::get_ton()->play("res\\sound\\menu_cursor_confirm.wav");
            PopUp2::Get()->SetText(work + " ÇéËÇ…ì¸ÇÍÇΩÅB");
        }

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

    // 60âÒÇ…àÍâÒÅiÅÅ1ïbÇ≤Ç∆ÅjÇÃèàóù
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
}

void SeqBattle::RenderCommon()
{
    if (m_eState == eBattleState::LOAD)
    {
        // do nothing
    }
    else if (m_eState == eBattleState::TITLE && m_title->GetLoading())
    {
        // do nothing
    }
    else
    {
        m_player->Render();
        m_map->Render();
    }
}

void SeqBattle::RenderNormal()
{
    PopUp::Get()->Render();
    PopUp2::Get()->Render();
    D3DXVECTOR3 pos { 0.f, 0.f, 0.f };

    if (m_bShowExamine || m_bObtainable || m_bTalkable || m_bObtainWeapon)
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

    if (m_nGameoverCounter < 120)
    {
        transparency = m_nGameoverCounter * 255 / 120;
        m_spriteGameover->Render(pos, transparency);
    }
    else
    {
        m_spriteGameover->Render(pos);
        pos = D3DXVECTOR3(720.f, 700.f, 0.f);
        m_spriteExamine->Render(pos);
    }
}

void SeqBattle::UpdateDebug()
{
    // PopUp Sample
    {
        if (KeyBoard::IsDownFirstFrame(DIK_F7))
        {
            std::vector<std::vector<std::string>> vvs;
            std::vector<std::string> vs;
            vs.push_back("ÉTÉìÉvÉãÉeÉLÉXÉgÇP");
            vs.push_back("ÉTÉìÉvÉãÉeÉLÉXÉgÇQÉTÉìÉvÉãÉeÉLÉXÉgÇQ");
            vs.push_back("ÉTÉìÉvÉãÉeÉLÉXÉgÇRÉTÉìÉvÉãÉeÉLÉXÉgÇRÉTÉìÉvÉãÉeÉLÉXÉgÇR");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("ÉTÉìÉvÉãÉeÉLÉXÉgÇS");
            vs.push_back("ÉTÉìÉvÉãÉeÉLÉXÉgÇTÉTÉìÉvÉãÉeÉLÉXÉgÇT");
            vs.push_back("ÉTÉìÉvÉãÉeÉLÉXÉgÇUÉTÉìÉvÉãÉeÉLÉXÉgÇUÉTÉìÉvÉãÉeÉLÉXÉgÇU");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("ÉTÉìÉvÉãÉeÉLÉXÉgÇVÉTÉìÉvÉãÉeÉLÉXÉgÇVÉTÉìÉvÉãÉeÉLÉXÉgÇVÉTÉìÉvÉãÉeÉLÉXÉgÇV");
            vs.push_back("ÉTÉìÉvÉãÉeÉLÉXÉgÇWÉTÉìÉvÉãÉeÉLÉXÉgÇWÉTÉìÉvÉãÉeÉLÉXÉgÇW");
            vs.push_back("ÉTÉìÉvÉãÉeÉLÉXÉgÇXÉTÉìÉvÉãÉeÉLÉXÉgÇX");
            vvs.push_back(vs);
            vs.clear();
            vs.push_back("ÉTÉìÉvÉãÉeÉLÉXÉgÇPÇO");
            vvs.push_back(vs);
            PopUp::Get()->SetText(vvs);
        }

        if (KeyBoard::IsDownFirstFrame(DIK_F8))
        {
            PopUp::Get()->Next();
        }

        if (KeyBoard::IsDownFirstFrame(DIK_F9))
        {
            PopUp::Get()->Cancel();
        }

        PopUp::Get()->Update();
    }

    // ëqå…ã@î\
    {
        if (KeyBoard::IsDownFirstFrame(DIK_F1))
        {
            if (m_eState == eBattleState::NORMAL)
            {
                m_eState = eBattleState::STOREHOUSE;
                delete m_storehouse;

                Camera::SetCameraMode(eCameraMode::SLEEP);
                Common::SetCursorVisibility(true);

                // TODO ëqå…Çï\é¶Ç∑ÇÈìxÇ…ëqå…âÊñ ÇçÏÇÈÇÃÇÇ‚ÇﬂÇÈ
                m_storehouse = NEW NSStorehouseLib::StorehouseLib();

                NSStorehouseLib::Sprite* sprCursor = NEW NSStorehouseLib::Sprite(SharedObj::GetD3DDevice());
                sprCursor->Load("res\\image\\menu_cursor.png");

                NSStorehouseLib::Sprite* sprBackground = NEW NSStorehouseLib::Sprite(SharedObj::GetD3DDevice());
                sprBackground->Load("res\\image\\background.png");

                NSStorehouseLib::Sprite* sprPanelLeft = NEW NSStorehouseLib::Sprite(SharedObj::GetD3DDevice());
                sprPanelLeft->Load("res\\image\\panelLeft.png");

                NSStorehouseLib::Sprite* sprPanelTop = NEW NSStorehouseLib::Sprite(SharedObj::GetD3DDevice());
                sprPanelTop->Load("res\\image\\craftPanel.png");

                NSStorehouseLib::IFont* pFont = NEW NSStorehouseLib::Font(SharedObj::GetD3DDevice());
                pFont->Init();

                NSStorehouseLib::ISoundEffect* pSE = NEW NSStorehouseLib::SoundEffect();

                m_storehouse->Init(pFont, pSE, sprCursor, sprBackground, sprPanelLeft, sprPanelTop);
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
                                itemInfoList.push_back(itemInfoG);
                            }
                        }
                    }
                    m_storehouse->SetInventoryList(itemInfoList);
                }
                {
                    using namespace NSStarmanLib;
                    NSStarmanLib::Storehouse* storehouse = NSStarmanLib::Storehouse::GetObj();
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
        }
    }

    // ÉNÉâÉtÉgã@î\
    {
        if (KeyBoard::IsDownFirstFrame(DIK_F2))
        {
            if (m_eState == eBattleState::NORMAL)
            {
                m_eState = eBattleState::CRAFT;

                delete m_craft;

                Camera::SetCameraMode(eCameraMode::SLEEP);
                Common::SetCursorVisibility(true);

                m_craft = NEW NSCraftLib::CraftLib();

                NSCraftLib::Sprite* sprCursor = NEW NSCraftLib::Sprite(SharedObj::GetD3DDevice());
                sprCursor->Load("res\\image\\menu_cursor.png");

                NSCraftLib::Sprite* sprBackground = NEW NSCraftLib::Sprite(SharedObj::GetD3DDevice());
                sprBackground->Load("res\\image\\background.png");

                NSCraftLib::Sprite* sprPanelLeft = NEW NSCraftLib::Sprite(SharedObj::GetD3DDevice());
                sprPanelLeft->Load("res\\image\\panelLeft.png");

                NSCraftLib::Sprite* sprPanelTop = NEW NSCraftLib::Sprite(SharedObj::GetD3DDevice());
                sprPanelTop->Load("res\\image\\craftPanel.png");

                NSCraftLib::IFont* pFont = NEW NSCraftLib::Font(SharedObj::GetD3DDevice());
                pFont->Init();

                NSCraftLib::ISoundEffect* pSE = NEW NSCraftLib::SoundEffect();

                m_craft->Init(pFont, pSE, sprCursor, sprBackground, sprPanelLeft, sprPanelTop);

                {
                    std::vector<std::string> vs;

                    vs.push_back("ÉAÉCÉeÉÄÇ`Ç`Ç`");
                    vs.push_back("ïêäÌÇaÇaÇa");
                    vs.push_back("ÉAÉCÉeÉÄÇb");
                    vs.push_back("ÉAÉCÉeÉÄÇc");
                    vs.push_back("ÉAÉCÉeÉÄÇd");
                    vs.push_back("ÉAÉCÉeÉÄÇe");
                    vs.push_back("ÉAÉCÉeÉÄÇf");
                    vs.push_back("ÉAÉCÉeÉÄÇg");
                    vs.push_back("ÉAÉCÉeÉÄÇh");
                    vs.push_back("ÉAÉCÉeÉÄÇi");
                    vs.push_back("ÉAÉCÉeÉÄÇj");
                    vs.push_back("ÉAÉCÉeÉÄÇk");
                    vs.push_back("ÉAÉCÉeÉÄÇl");
                    vs.push_back("ÉAÉCÉeÉÄÇm");
                    vs.push_back("ÉAÉCÉeÉÄÇn");
                    vs.push_back("ÉAÉCÉeÉÄÇo");
                    m_craft->SetOutputList(vs);

                    m_craft->SetCraftingItem("ÉAÉCÉeÉÄÇyÇyÇyÇyÇy", 24);

                    vs.clear();
                    vs.push_back("ÉAÉCÉeÉÄÇP");
                    vs.push_back("ÉAÉCÉeÉÄÇQ");
                    vs.push_back("ÉAÉCÉeÉÄÇR");
                    vs.push_back("ÉAÉCÉeÉÄÇS");
                    m_craft->SetCraftQue(vs);

                    std::string work;

                    {
                        work = "ê¨â ï®ÇÃñºëOÅFÉAÉCÉeÉÄÇ`Ç`Ç`\n";
                        work += "ê¨â ï®ÇÃêîÅFÇP\n";
                        work += "ê¨â ï®ÇÃã≠âªìx\n";
                        work += "\n";
                        work += "ëfçﬁÇPÇÃñºëOÅFëfçﬁñºÇ†Ç†Ç†\n";
                        work += "ëfçﬁÇPÇÃêîÅFÇPÇO\n";
                        work += "ëfçﬁÇPÇÃã≠âªìxÅFÇP\n";
                        work += "\n";
                        work += "ëfçﬁÇQÇÃñºëOÅFëfçﬁñºÇ¢Ç¢Ç¢\n";
                        work += "ëfçﬁÇQÇÃêîÅFÇQÇO\n";
                        work += "ëfçﬁÇQÇÃã≠âªìxÅFÇQ\n";

                        m_craft->SetOutputInfo("ÉAÉCÉeÉÄÇ`Ç`Ç`", work);

                        NSCraftLib::ISprite* sprite1 = NEW NSCraftLib::Sprite(SharedObj::GetD3DDevice());
                        m_craft->SetOutputImage("ÉAÉCÉeÉÄÇ`Ç`Ç`", "res\\image\\item1.png", sprite1);
                    }
                    {
                        work = "ê¨â ï®ÇÃñºëOÅFïêäÌÇaÇaÇa\n";
                        work += "ê¨â ï®ÇÃêîÅFÇQ\n";
                        work += "ê¨â ï®ÇÃã≠âªìxÅFÇQ\n";
                        work += "\n";
                        work += "ëfçﬁÇPÇÃñºëOÅFëfçﬁñºÇ©Ç©Ç©Ç©Ç©\n";
                        work += "ëfçﬁÇPÇÃêîÅFÇPÇPÇP\n";
                        work += "ëfçﬁÇPÇÃã≠âªìxÅFÇP\n";
                        work += "\n";
                        work += "ëfçﬁÇQÇÃñºëOÅFëfçﬁñºÇ´Ç´Ç´Ç´Ç´\n";
                        work += "ëfçﬁÇQÇÃêîÅFÇQÇQÇQ\n";
                        work += "ëfçﬁÇQÇÃã≠âªìxÅFÇQ\n";

                        m_craft->SetOutputInfo("ïêäÌÇaÇaÇa", work);

                        NSCraftLib::ISprite* sprite2 = NEW NSCraftLib::Sprite(SharedObj::GetD3DDevice());
                        m_craft->SetOutputImage("ïêäÌÇaÇaÇa", "res\\image\\item2.png", sprite2);
                    }
                    {
                        work = "ê¨â ï®ÇÃñºëOÅFÉAÉCÉeÉÄÇb\n";
                        work += "ê¨â ï®ÇÃêîÅFÇQ\n";
                        work += "ê¨â ï®ÇÃã≠âªìxÅFÇQ\n";
                        work += "\n";
                        work += "ëfçﬁÇPÇÃñºëOÅFëfçﬁñºÇ©Ç©Ç©Ç©Ç©\n";
                        work += "ëfçﬁÇPÇÃêîÅFÇPÇPÇP\n";
                        work += "ëfçﬁÇPÇÃã≠âªìxÅFÇP\n";
                        work += "\n";
                        work += "ëfçﬁÇQÇÃñºëOÅFëfçﬁñºÇ´Ç´Ç´Ç´Ç´\n";
                        work += "ëfçﬁÇQÇÃêîÅFÇQÇQÇQ\n";
                        work += "ëfçﬁÇQÇÃã≠âªìxÅFÇQ\n";

                        m_craft->SetOutputInfo("ÉAÉCÉeÉÄÇb", work);

                        NSCraftLib::ISprite* sprite2 = NEW NSCraftLib::Sprite(SharedObj::GetD3DDevice());
                        m_craft->SetOutputImage("ÉAÉCÉeÉÄÇb", "res\\image\\item3.png", sprite2);
                    }
                    {
                        work = "ê¨â ï®ÇÃñºëOÅFÉAÉCÉeÉÄÇc\n";
                        work += "ê¨â ï®ÇÃêîÅFÇQ\n";
                        work += "ê¨â ï®ÇÃã≠âªìxÅFÇQ\n";
                        work += "\n";
                        work += "ëfçﬁÇPÇÃñºëOÅFëfçﬁñºÇ©Ç©Ç©Ç©Ç©\n";
                        work += "ëfçﬁÇPÇÃêîÅFÇPÇPÇP\n";
                        work += "ëfçﬁÇPÇÃã≠âªìxÅFÇP\n";
                        work += "\n";
                        work += "ëfçﬁÇQÇÃñºëOÅFëfçﬁñºÇ´Ç´Ç´Ç´Ç´\n";
                        work += "ëfçﬁÇQÇÃêîÅFÇQÇQÇQ\n";
                        work += "ëfçﬁÇQÇÃã≠âªìxÅFÇQ\n";

                        m_craft->SetOutputInfo("ÉAÉCÉeÉÄÇc", work);

                        NSCraftLib::ISprite* sprite2 = NEW NSCraftLib::Sprite(SharedObj::GetD3DDevice());
                        m_craft->SetOutputImage("ÉAÉCÉeÉÄÇc", "res\\image\\item1.png", sprite2);
                    }
                    {
                        work = "ê¨â ï®ÇÃñºëOÅFÉAÉCÉeÉÄÇd\n";
                        work += "ê¨â ï®ÇÃêîÅFÇQ\n";
                        work += "ê¨â ï®ÇÃã≠âªìxÅFÇQ\n";
                        work += "\n";
                        work += "ëfçﬁÇPÇÃñºëOÅFëfçﬁñºÇ©Ç©Ç©Ç©Ç©\n";
                        work += "ëfçﬁÇPÇÃêîÅFÇPÇPÇP\n";
                        work += "ëfçﬁÇPÇÃã≠âªìxÅFÇP\n";
                        work += "\n";
                        work += "ëfçﬁÇQÇÃñºëOÅFëfçﬁñºÇ´Ç´Ç´Ç´Ç´\n";
                        work += "ëfçﬁÇQÇÃêîÅFÇQÇQÇQ\n";
                        work += "ëfçﬁÇQÇÃã≠âªìxÅFÇQ\n";

                        m_craft->SetOutputInfo("ÉAÉCÉeÉÄÇd", work);

                        NSCraftLib::ISprite* sprite2 = NEW NSCraftLib::Sprite(SharedObj::GetD3DDevice());
                        m_craft->SetOutputImage("ÉAÉCÉeÉÄÇd", "res\\image\\item1.png", sprite2);
                    }
                    {
                        work = "ê¨â ï®ÇÃñºëOÅFÉAÉCÉeÉÄÇe\n";
                        work += "ê¨â ï®ÇÃêîÅFÇQ\n";
                        work += "ê¨â ï®ÇÃã≠âªìxÅFÇQ\n";
                        work += "\n";
                        work += "ëfçﬁÇPÇÃñºëOÅFëfçﬁñºÇ©Ç©Ç©Ç©Ç©\n";
                        work += "ëfçﬁÇPÇÃêîÅFÇPÇPÇP\n";
                        work += "ëfçﬁÇPÇÃã≠âªìxÅFÇP\n";
                        work += "\n";
                        work += "ëfçﬁÇQÇÃñºëOÅFëfçﬁñºÇ´Ç´Ç´Ç´Ç´\n";
                        work += "ëfçﬁÇQÇÃêîÅFÇQÇQÇQ\n";
                        work += "ëfçﬁÇQÇÃã≠âªìxÅFÇQ\n";

                        m_craft->SetOutputInfo("ÉAÉCÉeÉÄÇe", work);

                        NSCraftLib::ISprite* sprite2 = NEW NSCraftLib::Sprite(SharedObj::GetD3DDevice());
                        m_craft->SetOutputImage("ÉAÉCÉeÉÄÇe", "res\\image\\item1.png", sprite2);
                    }
                    {
                        work = "ê¨â ï®ÇÃñºëOÅFÉAÉCÉeÉÄÇf\n";
                        work += "ê¨â ï®ÇÃêîÅFÇQ\n";
                        work += "ê¨â ï®ÇÃã≠âªìxÅFÇQ\n";
                        work += "\n";
                        work += "ëfçﬁÇPÇÃñºëOÅFëfçﬁñºÇ©Ç©Ç©Ç©Ç©\n";
                        work += "ëfçﬁÇPÇÃêîÅFÇPÇPÇP\n";
                        work += "ëfçﬁÇPÇÃã≠âªìxÅFÇP\n";
                        work += "\n";
                        work += "ëfçﬁÇQÇÃñºëOÅFëfçﬁñºÇ´Ç´Ç´Ç´Ç´\n";
                        work += "ëfçﬁÇQÇÃêîÅFÇQÇQÇQ\n";
                        work += "ëfçﬁÇQÇÃã≠âªìxÅFÇQ\n";

                        m_craft->SetOutputInfo("ÉAÉCÉeÉÄÇf", work);

                        NSCraftLib::ISprite* sprite2 = NEW NSCraftLib::Sprite(SharedObj::GetD3DDevice());
                        m_craft->SetOutputImage("ÉAÉCÉeÉÄÇf", "res\\image\\item1.png", sprite2);
                    }
                    {
                        work = "ê¨â ï®ÇÃñºëOÅFÉAÉCÉeÉÄÇg\n";
                        work += "ê¨â ï®ÇÃêîÅFÇQ\n";
                        work += "ê¨â ï®ÇÃã≠âªìxÅFÇQ\n";
                        work += "\n";
                        work += "ëfçﬁÇPÇÃñºëOÅFëfçﬁñºÇ©Ç©Ç©Ç©Ç©\n";
                        work += "ëfçﬁÇPÇÃêîÅFÇPÇPÇP\n";
                        work += "ëfçﬁÇPÇÃã≠âªìxÅFÇP\n";
                        work += "\n";
                        work += "ëfçﬁÇQÇÃñºëOÅFëfçﬁñºÇ´Ç´Ç´Ç´Ç´\n";
                        work += "ëfçﬁÇQÇÃêîÅFÇQÇQÇQ\n";
                        work += "ëfçﬁÇQÇÃã≠âªìxÅFÇQ\n";

                        m_craft->SetOutputInfo("ÉAÉCÉeÉÄÇg", work);

                        NSCraftLib::ISprite* sprite2 = NEW NSCraftLib::Sprite(SharedObj::GetD3DDevice());
                        m_craft->SetOutputImage("ÉAÉCÉeÉÄÇg", "res\\image\\item1.png", sprite2);
                    }
                    {
                        work = "ê¨â ï®ÇÃñºëOÅFÉAÉCÉeÉÄÇh\n";
                        work += "ê¨â ï®ÇÃêîÅFÇQ\n";
                        work += "ê¨â ï®ÇÃã≠âªìxÅFÇQ\n";
                        work += "\n";
                        work += "ëfçﬁÇPÇÃñºëOÅFëfçﬁñºÇ©Ç©Ç©Ç©Ç©\n";
                        work += "ëfçﬁÇPÇÃêîÅFÇPÇPÇP\n";
                        work += "ëfçﬁÇPÇÃã≠âªìxÅFÇP\n";
                        work += "\n";
                        work += "ëfçﬁÇQÇÃñºëOÅFëfçﬁñºÇ´Ç´Ç´Ç´Ç´\n";
                        work += "ëfçﬁÇQÇÃêîÅFÇQÇQÇQ\n";
                        work += "ëfçﬁÇQÇÃã≠âªìxÅFÇQ\n";

                        m_craft->SetOutputInfo("ÉAÉCÉeÉÄÇh", work);

                        NSCraftLib::ISprite* sprite2 = NEW NSCraftLib::Sprite(SharedObj::GetD3DDevice());
                        m_craft->SetOutputImage("ÉAÉCÉeÉÄÇh", "res\\image\\item1.png", sprite2);
                    }
                    {
                        work = "ê¨â ï®ÇÃñºëOÅFÉAÉCÉeÉÄÇi\n";
                        work += "ê¨â ï®ÇÃêîÅFÇQ\n";
                        work += "ê¨â ï®ÇÃã≠âªìxÅFÇQ\n";
                        work += "\n";
                        work += "ëfçﬁÇPÇÃñºëOÅFëfçﬁñºÇ©Ç©Ç©Ç©Ç©\n";
                        work += "ëfçﬁÇPÇÃêîÅFÇPÇPÇP\n";
                        work += "ëfçﬁÇPÇÃã≠âªìxÅFÇP\n";
                        work += "\n";
                        work += "ëfçﬁÇQÇÃñºëOÅFëfçﬁñºÇ´Ç´Ç´Ç´Ç´\n";
                        work += "ëfçﬁÇQÇÃêîÅFÇQÇQÇQ\n";
                        work += "ëfçﬁÇQÇÃã≠âªìxÅFÇQ\n";

                        m_craft->SetOutputInfo("ÉAÉCÉeÉÄÇi", work);

                        NSCraftLib::ISprite* sprite2 = NEW NSCraftLib::Sprite(SharedObj::GetD3DDevice());
                        m_craft->SetOutputImage("ÉAÉCÉeÉÄÇi", "res\\image\\item1.png", sprite2);
                    }
                    {
                        work = "ê¨â ï®ÇÃñºëOÅFÉAÉCÉeÉÄÇj\n";
                        work += "ê¨â ï®ÇÃêîÅFÇQ\n";
                        work += "ê¨â ï®ÇÃã≠âªìxÅFÇQ\n";
                        work += "\n";
                        work += "ëfçﬁÇPÇÃñºëOÅFëfçﬁñºÇ©Ç©Ç©Ç©Ç©\n";
                        work += "ëfçﬁÇPÇÃêîÅFÇPÇPÇP\n";
                        work += "ëfçﬁÇPÇÃã≠âªìxÅFÇP\n";
                        work += "\n";
                        work += "ëfçﬁÇQÇÃñºëOÅFëfçﬁñºÇ´Ç´Ç´Ç´Ç´\n";
                        work += "ëfçﬁÇQÇÃêîÅFÇQÇQÇQ\n";
                        work += "ëfçﬁÇQÇÃã≠âªìxÅFÇQ\n";

                        m_craft->SetOutputInfo("ÉAÉCÉeÉÄÇj", work);

                        NSCraftLib::ISprite* sprite2 = NEW NSCraftLib::Sprite(SharedObj::GetD3DDevice());
                        m_craft->SetOutputImage("ÉAÉCÉeÉÄÇj", "res\\image\\item1.png", sprite2);
                    }
                    {
                        work = "ê¨â ï®ÇÃñºëOÅFÉAÉCÉeÉÄÇk\n";
                        work += "ê¨â ï®ÇÃêîÅFÇQ\n";
                        work += "ê¨â ï®ÇÃã≠âªìxÅFÇQ\n";
                        work += "\n";
                        work += "ëfçﬁÇPÇÃñºëOÅFëfçﬁñºÇ©Ç©Ç©Ç©Ç©\n";
                        work += "ëfçﬁÇPÇÃêîÅFÇPÇPÇP\n";
                        work += "ëfçﬁÇPÇÃã≠âªìxÅFÇP\n";
                        work += "\n";
                        work += "ëfçﬁÇQÇÃñºëOÅFëfçﬁñºÇ´Ç´Ç´Ç´Ç´\n";
                        work += "ëfçﬁÇQÇÃêîÅFÇQÇQÇQ\n";
                        work += "ëfçﬁÇQÇÃã≠âªìxÅFÇQ\n";

                        m_craft->SetOutputInfo("ÉAÉCÉeÉÄÇk", work);

                        NSCraftLib::ISprite* sprite2 = NEW NSCraftLib::Sprite(SharedObj::GetD3DDevice());
                        m_craft->SetOutputImage("ÉAÉCÉeÉÄÇk", "res\\image\\item1.png", sprite2);
                    }
                    {
                        work = "ê¨â ï®ÇÃñºëOÅFÉAÉCÉeÉÄÇl\n";
                        work += "ê¨â ï®ÇÃêîÅFÇQ\n";
                        work += "ê¨â ï®ÇÃã≠âªìxÅFÇQ\n";
                        work += "\n";
                        work += "ëfçﬁÇPÇÃñºëOÅFëfçﬁñºÇ©Ç©Ç©Ç©Ç©\n";
                        work += "ëfçﬁÇPÇÃêîÅFÇPÇPÇP\n";
                        work += "ëfçﬁÇPÇÃã≠âªìxÅFÇP\n";
                        work += "\n";
                        work += "ëfçﬁÇQÇÃñºëOÅFëfçﬁñºÇ´Ç´Ç´Ç´Ç´\n";
                        work += "ëfçﬁÇQÇÃêîÅFÇQÇQÇQ\n";
                        work += "ëfçﬁÇQÇÃã≠âªìxÅFÇQ\n";

                        m_craft->SetOutputInfo("ÉAÉCÉeÉÄÇl", work);

                        NSCraftLib::ISprite* sprite2 = NEW NSCraftLib::Sprite(SharedObj::GetD3DDevice());
                        m_craft->SetOutputImage("ÉAÉCÉeÉÄÇl", "res\\image\\item1.png", sprite2);
                    }
                    {
                        work = "ê¨â ï®ÇÃñºëOÅFÉAÉCÉeÉÄÇm\n";
                        work += "ê¨â ï®ÇÃêîÅFÇQ\n";
                        work += "ê¨â ï®ÇÃã≠âªìxÅFÇQ\n";
                        work += "\n";
                        work += "ëfçﬁÇPÇÃñºëOÅFëfçﬁñºÇ©Ç©Ç©Ç©Ç©\n";
                        work += "ëfçﬁÇPÇÃêîÅFÇPÇPÇP\n";
                        work += "ëfçﬁÇPÇÃã≠âªìxÅFÇP\n";
                        work += "\n";
                        work += "ëfçﬁÇQÇÃñºëOÅFëfçﬁñºÇ´Ç´Ç´Ç´Ç´\n";
                        work += "ëfçﬁÇQÇÃêîÅFÇQÇQÇQ\n";
                        work += "ëfçﬁÇQÇÃã≠âªìxÅFÇQ\n";

                        m_craft->SetOutputInfo("ÉAÉCÉeÉÄÇm", work);

                        NSCraftLib::ISprite* sprite2 = NEW NSCraftLib::Sprite(SharedObj::GetD3DDevice());
                        m_craft->SetOutputImage("ÉAÉCÉeÉÄÇm", "res\\image\\item1.png", sprite2);
                    }
                    {
                        work = "ê¨â ï®ÇÃñºëOÅFÉAÉCÉeÉÄÇn\n";
                        work += "ê¨â ï®ÇÃêîÅFÇQ\n";
                        work += "ê¨â ï®ÇÃã≠âªìxÅFÇQ\n";
                        work += "\n";
                        work += "ëfçﬁÇPÇÃñºëOÅFëfçﬁñºÇ©Ç©Ç©Ç©Ç©\n";
                        work += "ëfçﬁÇPÇÃêîÅFÇPÇPÇP\n";
                        work += "ëfçﬁÇPÇÃã≠âªìxÅFÇP\n";
                        work += "\n";
                        work += "ëfçﬁÇQÇÃñºëOÅFëfçﬁñºÇ´Ç´Ç´Ç´Ç´\n";
                        work += "ëfçﬁÇQÇÃêîÅFÇQÇQÇQ\n";
                        work += "ëfçﬁÇQÇÃã≠âªìxÅFÇQ\n";

                        m_craft->SetOutputInfo("ÉAÉCÉeÉÄÇn", work);

                        NSCraftLib::ISprite* sprite2 = NEW NSCraftLib::Sprite(SharedObj::GetD3DDevice());
                        m_craft->SetOutputImage("ÉAÉCÉeÉÄÇn", "res\\image\\item1.png", sprite2);
                    }
                    {
                        work = "ê¨â ï®ÇÃñºëOÅFÉAÉCÉeÉÄÇo\n";
                        work += "ê¨â ï®ÇÃêîÅFÇQ\n";
                        work += "ê¨â ï®ÇÃã≠âªìxÅFÇQ\n";
                        work += "\n";
                        work += "ëfçﬁÇPÇÃñºëOÅFëfçﬁñºÇ©Ç©Ç©Ç©Ç©\n";
                        work += "ëfçﬁÇPÇÃêîÅFÇPÇPÇP\n";
                        work += "ëfçﬁÇPÇÃã≠âªìxÅFÇP\n";
                        work += "\n";
                        work += "ëfçﬁÇQÇÃñºëOÅFëfçﬁñºÇ´Ç´Ç´Ç´Ç´\n";
                        work += "ëfçﬁÇQÇÃêîÅFÇQÇQÇQ\n";
                        work += "ëfçﬁÇQÇÃã≠âªìxÅFÇQ\n";

                        m_craft->SetOutputInfo("ÉAÉCÉeÉÄÇo", work);

                        NSCraftLib::ISprite* sprite2 = NEW NSCraftLib::Sprite(SharedObj::GetD3DDevice());
                        m_craft->SetOutputImage("ÉAÉCÉeÉÄÇo", "res\\image\\item1.png", sprite2);
                    }
                }
            }
        }
    }
}

void SeqBattle::UpdatePerSecond()
{
    //-------------------------------------
    // éûçèÇêiÇﬂÇÈ
    //-------------------------------------
    // 2éûä‘ÉQÅ[ÉÄÇÇµÇΩÇÁÉpÉèÅ[ÉGÉbÉOêØÇ≈24éûä‘åoâﬂÇ∑ÇÈ
    NSStarmanLib::PowereggDateTime* dateTime = NSStarmanLib::PowereggDateTime::GetObj();

    if (Common::DebugMode() == false)
    {
        dateTime->IncreaseDateTime(0, 0, 0, 0, 12);
    }
    else
    {
        dateTime->IncreaseDateTime(0, 0, 0, 30, 0); // 1ïbÇ≈1éûä‘Ç∆Ç©åoâﬂÇ≥ÇπÇΩÇ¢éûóp
    }

    //-------------------------------------
    // ÉXÉeÅ[É^ÉXÇçXêV
    //-------------------------------------
    NSStarmanLib::StatusManager* statusManager = NSStarmanLib::StatusManager::GetObj();
    statusManager->Update();

    //-------------------------------------
    // éÄñSÉ`ÉFÉbÉN
    //-------------------------------------
    bool dead = statusManager->GetDead();
    if (dead)
    {
        if (m_eState != eBattleState::GAMEOVER)
        {
            m_eState = eBattleState::GAMEOVER;
            m_nGameoverCounter = 0;
        }
    }

    //-------------------------------------
    // êáñ∞ÅEãCê‚É`ÉFÉbÉN
    //-------------------------------------
    if (m_eSleepSeq == eSleepSeq::NotStart)
    {
        bool sleep = statusManager->GetSleep();
        if (sleep)
        {
            m_eState = eBattleState::SLEEP;
            m_eSleepSeq = eSleepSeq::FadeOut;
            m_sleepFadeOut = 0;
            m_sleepBlack = 0;
            m_sleepFadeIn = 0;
            m_player->SetSleep(true);
        }
    }

    //-------------------------------------
    // ÉAÉCÉeÉÄî≠å©
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
    // ìäÇ∞Çƒínñ Ç…óéÇøÇΩÉAÉCÉeÉÄÇî≠å©
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
    // ÉNÉGÉXÉgä«óùÉNÉâÉXÇ…ÉvÉåÉCÉÑÅ[ÇÃåªç›ínÇímÇÁÇπÇÈÅB
    //-------------------------------------
    {
        D3DXVECTOR3 playerPos = SharedObj::GetPlayer()->GetPos();
        SharedObj::GetQuestSystem()->SetPos(playerPos.x, playerPos.y, playerPos.z);

        // ÉvÉåÉCÉÑÅ[ÇÃåªç›ç¿ïWÇ≈äJénoräÆóπÇ≈Ç´ÇÈÉNÉGÉXÉgÇ™Ç†ÇÈÇ»ÇÁ
        auto startQuest = SharedObj::GetQuestSystem()->GetStartQuest();
        if (startQuest.empty() == false)
        {
            {
                auto it = std::find(startQuest.begin(), startQuest.end(), "Q10");
                if (it != startQuest.end())
                {
                    int i = 0;
                    ++i;
                }
            }

            auto startEvent = SharedObj::GetQuestSystem()->GetQuestStartEvent(startQuest.at(0));
            if (startEvent.empty() == false)
            {
                // TODO ç≈èâÇÃÉCÉxÉìÉgÇæÇØèàóùÇµÇƒÇ¢ÇÈÇ™ïKóvÇ…Ç»Ç¡ÇΩÇÁï°êîÉCÉxÉìÉgëŒâû
                if (startEvent.at(0).find("<talk>") != std::string::npos)
                {
                    std::string work = startEvent.at(0);
                    std::string::size_type it = work.find("<talk>");
                    work = work.erase(it, 6);

                    NSTalkLib2::IFont* pFont = NEW NSTalkLib2::Font(SharedObj::GetD3DDevice());
                    NSTalkLib2::ISoundEffect* pSE = NEW NSTalkLib2::SoundEffect();
                    NSTalkLib2::ISprite* sprite = NEW NSTalkLib2::Sprite(SharedObj::GetD3DDevice());

                    m_talk = NEW NSTalkLib2::Talk();
                    m_talk->Init(work, pFont, pSE, sprite,
                                 "res\\image\\textBack.png", "res\\image\\black.png");

                    m_eState = eBattleState::TALK;
                }
            }

        }


        // Åuí≤Ç◊ÇÈÅvÉAÉNÉVÉáÉìÇ™Ç≈Ç´ÇÈÇ±Ç∆ÇÉAÉCÉRÉìÇ≈ímÇÁÇπÇÈÅB
        std::string quest1 = SharedObj::GetQuestSystem()->GetQuestIdStartByExamine(playerPos.x, playerPos.y, playerPos.z);
        std::string quest2 = SharedObj::GetQuestSystem()->GetQuestIdFinishByExamine(playerPos.x, playerPos.y, playerPos.z);
        if (quest1.empty() == false || quest2.empty() == false)
        {
            m_bShowExamine = true;
        }
        else
        {
            m_bShowExamine = false;
        }

        //---------------------------------------------------------------------
        // ÅuòbÇµÇ©ÇØÇÈÅvÇ™Ç≈Ç´ÇÈÇ±Ç∆ÇÉAÉCÉRÉìÇ≈ímÇÁÇπÇÈ
        //---------------------------------------------------------------------

        // 3ÉÅÅ[ÉgÉãà»ì‡Ç…òbÇµÇ©ÇØÇÁÇÍÇÈNPCÇ™Ç¢ÇÈÇ©ÅB
        auto npcManager = NpcManager::Get();
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

void SeqBattle::OperateNormal(eSequence* sequence)
{
    //============================================
    // ÉLÅ[É{Å[ÉhÅAÉ}ÉEÉXÅAÉQÅ[ÉÄÉpÉbÉhÇÃèàóù
    //============================================

    //--------------------------------------------
    // KeyBoard
    //--------------------------------------------

    // ÉÅÉjÉÖÅ[ã@î\
    if (KeyBoard::IsDownFirstFrame(DIK_ESCAPE))
    {
        m_eState = eBattleState::MENU;
        Camera::SetCameraMode(eCameraMode::SLEEP);
        Common::SetCursorVisibility(true);
        m_menuManager.Finalize();
        m_menuManager.InitMenu();

        return;
    }

    // ÉRÉ}ÉìÉhã@î\
    if (KeyBoard::IsDownFirstFrame(DIK_C))
    {
        if (m_eState == eBattleState::NORMAL)
        {
            m_eState = eBattleState::COMMAND;

            Camera::SetCameraMode(eCameraMode::SLEEP);
            Common::SetCursorVisibility(true);
            m_commandCounter = 0;
        }
    }

    // ämíËëÄçÏ
    if (KeyBoard::IsDownFirstFrame(DIK_E))
    {
        Confirm(sequence);
    }

    //--------------------------------------------
    // GamePad
    //--------------------------------------------

    // ÉÅÉjÉÖÅ[ã@î\
    if (GamePad::IsDown(eGamePadButtonType::START))
    {
        if (m_eState == eBattleState::NORMAL)
        {
            m_eState = eBattleState::MENU;
            Camera::SetCameraMode(eCameraMode::SLEEP);
            Common::SetCursorVisibility(true);
            m_menuManager.Finalize();
            m_menuManager.InitMenu();
        }
    }

    // ÉRÉ}ÉìÉhã@î\
    if (GamePad::IsDown(eGamePadButtonType::BACK))
    {
        if (m_eState == eBattleState::NORMAL)
        {
            m_eState = eBattleState::COMMAND;

            Camera::SetCameraMode(eCameraMode::SLEEP);
            Common::SetCursorVisibility(true);
            m_commandCounter = 0;
        }
    }

    // ämíËëÄçÏ
    if (GamePad::IsDown(eGamePadButtonType::A))
    {
        Confirm(sequence);
    }

    m_player->Update(m_map);

    // Camera
    {
        D3DXVECTOR3 pos = m_player->GetPos();
        pos.y += 1.f;
        Camera::SetLookAtPos(pos);
    }

    // ÉNÉGÉXÉgèàóù
    // TODO QuestManagerÇ≈Ç‚ÇÈÇ◊Çµ
    {
        QuestSystem* qs = SharedObj::GetQuestSystem();
        std::vector<std::string> vs = qs->GetFinishQuest();
        for (std::size_t i = 0; i < vs.size(); ++i)
        {
            m_finishQuestQue.push_back(vs.at(i));
        }

        if (m_finishQuestQue.size() >= 1)
        {
            std::string questId = m_finishQuestQue.at(0);
            std::vector<std::string> vs2 = qs->GetQuestFinishEvent(questId);
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
                    m_talk->Init(work, pFont, pSE, sprite,
                                 "res\\image\\textBack.png", "res\\image\\black.png");

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
                    qs->SetQuestFinish(work);
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
                    if (work.find("<É_ÉCÉPÉCÉ}Éì>") != std::string::npos)
                    {
                        npcName = "É_ÉCÉPÉCÉ}Éì";
                    }
                    else if (work.find("<ÉTÉìÉJÉNÉ}Éì>") != std::string::npos)
                    {
                        npcName = "ÉTÉìÉJÉNÉ}Éì";
                    }
                    else if (work.find("<ÉVÉJÉNÉ}Éì>") != std::string::npos)
                    {
                        npcName = "ÉVÉJÉNÉ}Éì";
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
            }
        }
    }

    m_hudManager.Update();
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
        m_Opening = NEW Opening();
    }
    else if (m_eState == eBattleState::NORMAL)
    {
        SAFE_DELETE(m_title);

        Camera::SetCameraMode(eCameraMode::BATTLE);
        Common::SetCursorVisibility(false);

        BGM::get_ton()->load("res\\sound\\novel.wav");
        BGM::get_ton()->play(10);
    }
}

void SeqBattle::OperateGameover(eSequence* sequence)
{
    ++m_nGameoverCounter;

    if (m_nGameoverCounter == 1)
    {
        if (Common::DeployMode())
        {
            SaveManager::Get()->DeleteSavedata();
        }
    }

    if (m_nGameoverCounter >= 120)
    {
        // ämíËëÄçÏ
        if (KeyBoard::IsDownFirstFrame(DIK_E))
        {
            if (m_nGameoverCounter >= 120)
            {
                m_eState = eBattleState::TITLE;
                Camera::SetCameraMode(eCameraMode::TITLE);
                Common::SetCursorVisibility(true);
                m_title = NEW Title(true);
            }
        }

        // ämíËëÄçÏ
        if (GamePad::IsDown(eGamePadButtonType::A))
        {
            if (m_nGameoverCounter >= 120)
            {
                m_eState = eBattleState::TITLE;
                Camera::SetCameraMode(eCameraMode::TITLE);
                Common::SetCursorVisibility(true);
                m_title = NEW Title(true);
            }
        }
    }
}

void SeqBattle::SaveLastStage(const int stageNum)
{
    std::ofstream outputfile(".savedata");
    outputfile << stageNum;
    outputfile.close();
    SetFileAttributes(".savedata", FILE_ATTRIBUTE_HIDDEN);
}

