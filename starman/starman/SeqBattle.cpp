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
            m_D3DSprite->Release();
            m_pD3DTexture->Release();
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
        return new Sprite(m_pD3DDevice);
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
    m_player = new Player();
    SharedObj::SetPlayer(m_player);

    D3DXVECTOR3 pos = D3DXVECTOR3(6.f, 0.f, 10.f);

    BGM::get_ton()->load("res\\sound\\title.wav");
    BGM::get_ton()->play(10);

    m_spriteGameover = new Sprite("res\\image\\gameover.png");
    m_spriteExamine = new Sprite("res\\image\\examine.png");

    NSTalkLib2::IFont* pFont = new NSTalkLib2::Font(SharedObj::GetD3DDevice());
    NSTalkLib2::ISoundEffect* pSE = new NSTalkLib2::SoundEffect();
    NSTalkLib2::ISprite* sprite = new NSTalkLib2::Sprite(SharedObj::GetD3DDevice());

    // ?
    m_talk = new NSTalkLib2::Talk();

    if (Common::ReleaseMode())
    {
        m_talk->Init("res\\script\\origin\\talk2Sample.csv", pFont, pSE, sprite,
                     "res\\image\\textBack.png", "res\\image\\black.png");
    }
    else
    {
        m_talk->Init("res\\script\\origin_debug\\talk2Sample.csv", pFont, pSE, sprite,
                     "res\\image\\textBack.png", "res\\image\\black.png");
    }
    m_bTalking = true;

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
}

SeqBattle::~SeqBattle()
{
    // TODO menuÇÃâï˙èàóù
}

void SeqBattle::Update(eSequence* sequence)
{
    if (m_eState == eBattleState::LOAD)
    {
        UpdateLoad();
        return;
    }

    UpdateCommon();

    if (m_eState == eBattleState::MENU)
    {
        OperateMenu(sequence);
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

    if (Common::DebugMode())
    {
        UpdateDebug();
    }
}


void SeqBattle::OperateMenu(eSequence* sequence)
{
    std::string result = m_menuManager.OperateMenu();
    if (result == "É^ÉCÉgÉãÇ…ñﬂÇÈ")
    {
        m_eState = eBattleState::TITLE;
        Camera::SetCameraMode(eCameraMode::BATTLE_TO_TITLE);
        Common::SetCursorVisibility(true);
        m_title = new Title(false);
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
            m_bTalking = false;
            m_talk->Finalize();
            delete m_talk;
            m_talk = nullptr;
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

    if (Common::DebugMode())
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
    std::string result = m_commandManager.Operate();

    if (result == "EXIT")
    {
        m_eState = eBattleState::NORMAL;
        Camera::SetCameraMode(eCameraMode::BATTLE);
        Common::SetCursorVisibility(false);
    }
}

void SeqBattle::InitLoad()
{
    // ÉZÅ[ÉuÉfÅ[É^Ç™Ç†ÇÈÇ©î€Ç©
    int saveExist = 0;
    if (Common::DebugMode())
    {
        saveExist = PathFileExists("res\\script\\save_debug");
    }
    else
    {
        saveExist = PathFileExists("res\\script\\save");
    }

    if (saveExist == FALSE)
    {
        m_bSavedataExists = false;
        m_loadThread = new std::thread(
            [&]
            {
                SaveManager::Get()->LoadOrigin();
                m_loadLoaded.store(true);
            });
    }
    else
    {
        m_bSavedataExists = true;
        m_loadThread = new std::thread(
            [&]
            {
                SaveManager::Get()->Load();
                m_loadLoaded.store(true);
            });
    }

    m_sprLoadBack = new Sprite("res\\image\\black.png");
    m_sprLoadLogo = new Sprite("res\\image\\title01.png");
    m_sprLoadClock = new Sprite("res\\image\\load_clock.png");
    m_sprLoadLoading = new Sprite("res\\image\\loading.png");

    Common::SetCursorVisibility(false);
}

void SeqBattle::UpdateLoad()
{
    if (m_loadLoaded.load())
    {
        InitializeAfterLoad();
        FinalizeLoad();
        m_eState = eBattleState::TITLE;
        m_title = new Title(true);
    }
}

void SeqBattle::InitializeAfterLoad()
{
    m_menuManager.InitMenu();

    m_map = new Map();
    SharedObj::SetMap(m_map);
    m_map->Init();

    auto status = NSStarmanLib::StatusManager::GetObj();
    D3DXVECTOR3 pos;
    status->GetXYZ(&pos.x, &pos.y, &pos.z);
    m_player->SetPos(pos);

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
        counter += 0.05f;
        int temp1 = 0;

        D3DXVECTOR3 pos { 0.0f, 0.0f, 0.0f };
        pos.x = std::sin(counter) * 100 + 800;
        pos.y = std::cos(counter) * 60 + 450;
        temp1 = (int)std::sin(counter * 0.2) * 255;
        m_sprLoadClock->Render(pos, temp1);

        static float counter2 = 0;
        counter2 += 0.04f;

        pos.x = std::sin(counter2) * 60 + 800;
        pos.y = std::cos(counter2) * 90 + 450;
        temp1 = (int)std::sin(counter2 * 0.2) * 192;
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

        // TODO ì«Ç›çûÇ›ÇÃêiíªìxÇ…çáÇÌÇπÇƒÅuÉzÉVÉ}ÉìÅvÇ1ï∂éöÇ∏Ç¬ï\é¶Ç∑ÇÈ
        static int width_ = 0;
        pos.x = 630;
        pos.y = 200;

        static int counter4 = 0;
        ++counter4;

        if (counter4 <= 60*1)
        {
            width_ = 0;
        }
        else if (counter4 <= 60*4)
        {
            width_ = 90;
        }
        else if (counter4 <= 60*9)
        {
            width_ = 180;
        }
        else if (counter4 <= 60*16)
        {
            width_ = 270;
        }
        else if (counter4 <= 60*25)
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
}

void SeqBattle::InputR1()
{
    bool ret = m_player->SetAttack();
}

void SeqBattle::Confirm(eSequence* sequence)
{
    if (m_eState == eBattleState::GAMEOVER)
    {
        if (m_nGameoverCounter >= 60)
        {
            *sequence = eSequence::TITLE;
        }
    }
    else
    {
        // í≤Ç◊ÇÈÉRÉ}ÉìÉh
        // ÉvÉåÉCÉÑÅ[ÇÃåªç›ç¿ïWÇ≈énÇ‹ÇÈÉNÉGÉXÉgÇ©èIÇÌÇÈÉNÉGÉXÉgÇ™Ç†ÇÈÅB
        D3DXVECTOR3 playerPos = SharedObj::GetPlayer()->GetPos();
        if (m_bShowExamine)
        {
            SharedObj::GetQuestSystem()->SetExamine(playerPos.x, playerPos.y, playerPos.z);
        }

        if (m_bObtainable)
        {
            NSStarmanLib::ItemManager* itemManager = NSStarmanLib::ItemManager::GetObj();

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
    }
}

eBattleState SeqBattle::GetState() const
{
    return m_eState;
}

void SeqBattle::UpdateCommon()
{
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

    if (m_player->GetDead())
    {
        if (m_eState == eBattleState::GAMEOVER)
        {
            SaveManager::Get()->DeleteSavedata();
            ++m_nGameoverCounter;
            if (m_nGameoverCounter >= 120)
            {
                m_eState = eBattleState::TITLE;
            }
        }
        return;
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
    if (m_player->GetDead())
    {
        m_spriteGameover->Render(pos);
    }
    if (m_bShowExamine || m_bObtainable)
    {
        D3DXVECTOR3 pos { 200.f, 600.f, 0.f };
        m_spriteExamine->Render(pos);
    }
    m_hudManager.Draw();
}

void SeqBattle::UpdateDebug()
{
    // PopUp Sample
    {
        if (KeyBoard::IsDownFirstFrame(DIK_F7))
        {
            std::vector<std::vector<std::string>> vss;
            std::vector<std::string> vs;
            vs.push_back("ÉTÉìÉvÉãÉeÉLÉXÉgÇP");
            vs.push_back("ÉTÉìÉvÉãÉeÉLÉXÉgÇQÉTÉìÉvÉãÉeÉLÉXÉgÇQ");
            vs.push_back("ÉTÉìÉvÉãÉeÉLÉXÉgÇRÉTÉìÉvÉãÉeÉLÉXÉgÇRÉTÉìÉvÉãÉeÉLÉXÉgÇR");
            vss.push_back(vs);
            vs.clear();
            vs.push_back("ÉTÉìÉvÉãÉeÉLÉXÉgÇS");
            vs.push_back("ÉTÉìÉvÉãÉeÉLÉXÉgÇTÉTÉìÉvÉãÉeÉLÉXÉgÇT");
            vs.push_back("ÉTÉìÉvÉãÉeÉLÉXÉgÇUÉTÉìÉvÉãÉeÉLÉXÉgÇUÉTÉìÉvÉãÉeÉLÉXÉgÇU");
            vss.push_back(vs);
            vs.clear();
            vs.push_back("ÉTÉìÉvÉãÉeÉLÉXÉgÇVÉTÉìÉvÉãÉeÉLÉXÉgÇVÉTÉìÉvÉãÉeÉLÉXÉgÇVÉTÉìÉvÉãÉeÉLÉXÉgÇV");
            vs.push_back("ÉTÉìÉvÉãÉeÉLÉXÉgÇWÉTÉìÉvÉãÉeÉLÉXÉgÇWÉTÉìÉvÉãÉeÉLÉXÉgÇW");
            vs.push_back("ÉTÉìÉvÉãÉeÉLÉXÉgÇXÉTÉìÉvÉãÉeÉLÉXÉgÇX");
            vss.push_back(vs);
            vs.clear();
            vs.push_back("ÉTÉìÉvÉãÉeÉLÉXÉgÇPÇO");
            vss.push_back(vs);
            PopUp::Get()->SetText(vss);
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
                m_storehouse = new NSStorehouseLib::StorehouseLib();

                NSStorehouseLib::Sprite* sprCursor = new NSStorehouseLib::Sprite(SharedObj::GetD3DDevice());
                sprCursor->Load("res\\image\\menu_cursor.png");

                NSStorehouseLib::Sprite* sprBackground = new NSStorehouseLib::Sprite(SharedObj::GetD3DDevice());
                sprBackground->Load("res\\image\\background.png");

                NSStorehouseLib::Sprite* sprPanelLeft = new NSStorehouseLib::Sprite(SharedObj::GetD3DDevice());
                sprPanelLeft->Load("res\\image\\panelLeft.png");

                NSStorehouseLib::Sprite* sprPanelTop = new NSStorehouseLib::Sprite(SharedObj::GetD3DDevice());
                sprPanelTop->Load("res\\image\\craftPanel.png");

                NSStorehouseLib::IFont* pFont = new NSStorehouseLib::Font(SharedObj::GetD3DDevice());
                pFont->Init();

                NSStorehouseLib::ISoundEffect* pSE = new NSStorehouseLib::SoundEffect();

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

                m_craft = new NSCraftLib::CraftLib();

                NSCraftLib::Sprite* sprCursor = new NSCraftLib::Sprite(SharedObj::GetD3DDevice());
                sprCursor->Load("res\\image\\menu_cursor.png");

                NSCraftLib::Sprite* sprBackground = new NSCraftLib::Sprite(SharedObj::GetD3DDevice());
                sprBackground->Load("res\\image\\background.png");

                NSCraftLib::Sprite* sprPanelLeft = new NSCraftLib::Sprite(SharedObj::GetD3DDevice());
                sprPanelLeft->Load("res\\image\\panelLeft.png");

                NSCraftLib::Sprite* sprPanelTop = new NSCraftLib::Sprite(SharedObj::GetD3DDevice());
                sprPanelTop->Load("res\\image\\craftPanel.png");

                NSCraftLib::IFont* pFont = new NSCraftLib::Font(SharedObj::GetD3DDevice());
                pFont->Init();

                NSCraftLib::ISoundEffect* pSE = new NSCraftLib::SoundEffect();

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

                        NSCraftLib::ISprite* sprite1 = new NSCraftLib::Sprite(SharedObj::GetD3DDevice());
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

                        NSCraftLib::ISprite* sprite2 = new NSCraftLib::Sprite(SharedObj::GetD3DDevice());
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

                        NSCraftLib::ISprite* sprite2 = new NSCraftLib::Sprite(SharedObj::GetD3DDevice());
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

                        NSCraftLib::ISprite* sprite2 = new NSCraftLib::Sprite(SharedObj::GetD3DDevice());
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

                        NSCraftLib::ISprite* sprite2 = new NSCraftLib::Sprite(SharedObj::GetD3DDevice());
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

                        NSCraftLib::ISprite* sprite2 = new NSCraftLib::Sprite(SharedObj::GetD3DDevice());
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

                        NSCraftLib::ISprite* sprite2 = new NSCraftLib::Sprite(SharedObj::GetD3DDevice());
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

                        NSCraftLib::ISprite* sprite2 = new NSCraftLib::Sprite(SharedObj::GetD3DDevice());
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

                        NSCraftLib::ISprite* sprite2 = new NSCraftLib::Sprite(SharedObj::GetD3DDevice());
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

                        NSCraftLib::ISprite* sprite2 = new NSCraftLib::Sprite(SharedObj::GetD3DDevice());
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

                        NSCraftLib::ISprite* sprite2 = new NSCraftLib::Sprite(SharedObj::GetD3DDevice());
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

                        NSCraftLib::ISprite* sprite2 = new NSCraftLib::Sprite(SharedObj::GetD3DDevice());
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

                        NSCraftLib::ISprite* sprite2 = new NSCraftLib::Sprite(SharedObj::GetD3DDevice());
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

                        NSCraftLib::ISprite* sprite2 = new NSCraftLib::Sprite(SharedObj::GetD3DDevice());
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

                        NSCraftLib::ISprite* sprite2 = new NSCraftLib::Sprite(SharedObj::GetD3DDevice());
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

                        NSCraftLib::ISprite* sprite2 = new NSCraftLib::Sprite(SharedObj::GetD3DDevice());
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

    if (Common::ReleaseMode())
    {
        dateTime->IncreaseDateTime(0, 0, 0, 0, 12);
    }
    else
    {
        dateTime->IncreaseDateTime(0, 0, 0, 10, 0); // 1ïbÇ≈1éûä‘Ç∆Ç©åoâﬂÇ≥ÇπÇΩÇ¢éûóp
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
        m_player->SetDead();
        m_eState = eBattleState::GAMEOVER;
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
    // ÉNÉGÉXÉgä«óùÉNÉâÉXÇ…ÉvÉåÉCÉÑÅ[ÇÃåªç›ínÇímÇÁÇπÇÈÅB
    //-------------------------------------
    {
        D3DXVECTOR3 playerPos = SharedObj::GetPlayer()->GetPos();
        SharedObj::GetQuestSystem()->SetPos(playerPos.x, playerPos.y, playerPos.z);

        // ÉvÉåÉCÉÑÅ[ÇÃåªç›ç¿ïWÇ≈äJénoräÆóπÇ≈Ç´ÇÈÉNÉGÉXÉgÇ™Ç†ÇÈÇ»ÇÁ
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
    }
}

void SeqBattle::OperateNormal(eSequence* sequence)
{
    //--------------------------------------------
    // ÉLÅ[É{Å[ÉhÅAÉ}ÉEÉXÅAÉQÅ[ÉÄÉpÉbÉhÇÃèàóù
    //--------------------------------------------

    //--------------------------------------------
    // KeyBoard
    //--------------------------------------------

    // ÉÅÉjÉÖÅ[ã@î\
    if (KeyBoard::IsDownFirstFrame(DIK_ESCAPE))
    {
        m_eState = eBattleState::MENU;
        Camera::SetCameraMode(eCameraMode::SLEEP);
        Common::SetCursorVisibility(true);

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

    if (m_player->GetHP() <= 0)
    {
        m_player->SetDead();
        m_eState = eBattleState::GAMEOVER;
    }

    // ÉNÉGÉXÉgèàóù
    {
        QuestSystem* qs = SharedObj::GetQuestSystem();
        std::vector<std::string> vs = qs->GetFinishQuest();
        for (std::size_t i = 0; i < vs.size(); ++i)
        {
            m_finishQuestQue.push_back(vs.at(i));
        }

        if (m_finishQuestQue.size() >= 1 && m_bTalking == false)
        {
            std::string questId = m_finishQuestQue.at(0);
            std::vector<std::string> vs2 = qs->GetQuestFinishEvent(questId);
            m_finishQuestQue.pop_front();
            if (vs2.at(0).find("<talk>") != std::string::npos)
            {
                std::string work = vs2.at(0);
                std::string::size_type it = work.find("<talk>");
                work = work.erase(it, 6);

                NSTalkLib2::IFont* pFont = new NSTalkLib2::Font(SharedObj::GetD3DDevice());
                NSTalkLib2::ISoundEffect* pSE = new NSTalkLib2::SoundEffect();
                NSTalkLib2::ISprite* sprite = new NSTalkLib2::Sprite(SharedObj::GetD3DDevice());

                m_talk = new NSTalkLib2::Talk();
                m_talk->Init(work, pFont, pSE, sprite,
                             "res\\image\\textBack.png", "res\\image\\black.png");
                m_bTalking = true;
            }
            else if (vs2.at(0).find("<ending>") != std::string::npos)
            {
                *sequence = eSequence::ENDING;
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
        m_Opening = new SeqOpening();
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

void SeqBattle::SaveLastStage(const int stageNum)
{
    std::ofstream outputfile(".savedata");
    outputfile << stageNum;
    outputfile.close();
    SetFileAttributes(".savedata", FILE_ATTRIBUTE_HIDDEN);
}

