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
                "ＭＳ 明朝",
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
            "ＭＳ 明朝",
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

SeqBattle::SeqBattle(const bool isContinue)
{
    D3DXVECTOR3 b = D3DXVECTOR3(0, 0.f, 0);
    D3DXVECTOR3 c = D3DXVECTOR3(0, 0, 0);
    m_player = new Player();
    SharedObj::SetPlayer(m_player);

    D3DXVECTOR3 pos = D3DXVECTOR3(6.f, 0.f, 10.f);

    BGM::get_ton()->load("res\\sound\\novel.wav");
    BGM::get_ton()->play(10);

    m_spriteGameover = new Sprite("res\\image\\gameover.png");
    m_spriteExamine = new Sprite("res\\image\\examine.png");

    if (isContinue)
    {
        std::ifstream ifs(".savedata");
        std::string line;
        std::getline(ifs, line);
        if (line.empty() == false)
        {
            m_nCurrentStage = atoi(line.c_str());
            if (m_nCurrentStage == 1)
            {
                m_map = new Map();
                m_map->Init();
            }
        }
        else
        {
            m_map = new Map();
            m_map->Init();
        }
    }
    else
    {
        m_map = new Map();
        m_map->Init();

        NSTalkLib2::IFont* pFont = new NSTalkLib2::Font(SharedObj::GetD3DDevice());
        NSTalkLib2::ISoundEffect* pSE = new NSTalkLib2::SoundEffect();
        NSTalkLib2::ISprite* sprite = new NSTalkLib2::Sprite(SharedObj::GetD3DDevice());

        m_talk = new NSTalkLib2::Talk();
        m_talk->Init("res\\script\\talk2Sample.csv", pFont, pSE, sprite,
                     "res\\image\\textBack.png", "res\\image\\black.png");
        m_bTalking = true;
    }

    m_menuManager.InitMenu();
}

SeqBattle::~SeqBattle()
{
    // TODO menuの解放処理
}

void SeqBattle::Update(eSequence* sequence)
{
    if (m_nCurrentStage == 1)
    {
        m_map->Update();
    }

    // TODO Gamepad support
    // メニュー画面が表示されているときの処理
    if (m_bShowMenu)
    {
        OperateMenu(sequence);
        return;
    }
    // 会話画面が表示されているときの処理
    else if (m_bTalking)
    {
        OperateTalk();
        return;
    }
    else if (m_bShowStorehouse)
    {
        OperateStorehouse();
        return;
    }

    if (KeyBoard::IsDown(DIK_ESCAPE))
    {
        m_bShowMenu = true;
        Camera::SleepModeON();
        ShowCursor(true);
        ClipCursor(NULL);

        return;
    }

    D3DXVECTOR3 move { 0.f, 0.f, 0.f };
    D3DXVECTOR3 rotate { 0.f, 0.f, 0.f };
    float radian = Camera::GetRadian();
    float yaw = -1.f * (radian - (D3DX_PI / 2));
    bool isHit { false };
    if (KeyBoard::IsHold(DIK_W))
    {
        // デバッグ目的でWキーだけ移動速度10倍
        //move.x += -std::sin(radian + D3DX_PI / 2) / 5;
        //move.z += std::sin(radian + D3DX_PI) / 5;
        move.x += -std::sin(radian + D3DX_PI / 2) / 5 * 10;
        move.z += std::sin(radian + D3DX_PI) / 5 * 10;

        D3DXVECTOR3 rotate { 0.f, yaw, 0.f };
        m_player->SetRotate(rotate);
        m_player->SetWalk();

    }
    if (KeyBoard::IsHold(DIK_A))
    {
        move.x += -std::sin(radian + D3DX_PI) / 5;
        move.z += std::sin(radian + D3DX_PI * 3 / 2) / 5;

        D3DXVECTOR3 rotate { 0.f, yaw + D3DX_PI * 3 / 2, 0.f };
        m_player->SetRotate(rotate);
        m_player->SetWalk();
    }
    if (KeyBoard::IsHold(DIK_S))
    {
        move.x += -std::sin(radian + D3DX_PI * 3 / 2) / 5;
        move.z += std::sin(radian) / 5;

        D3DXVECTOR3 rotate { 0.f, yaw + D3DX_PI, 0.f };
        m_player->SetRotate(rotate);
        m_player->SetWalk();
    }
    if (KeyBoard::IsHold(DIK_D))
    {
        move.x += -std::sin(radian) / 5;
        move.z += std::sin(radian + D3DX_PI / 2) / 5;

        D3DXVECTOR3 rotate { 0.f, yaw + D3DX_PI / 2, 0.f };
        m_player->SetRotate(rotate);
        m_player->SetWalk();
    }

    if (KeyBoard::IsDown(DIK_E))
    {
        InputB(sequence);
    }

    if (KeyBoard::IsDown(DIK_SPACE))
    {
        InputA();
    }

    if (KeyBoard::IsDown(DIK_F7))
    {
        std::vector<std::vector<std::string>> vss;
        std::vector<std::string> vs;
        vs.push_back("サンプルテキスト１");
        vs.push_back("サンプルテキスト２サンプルテキスト２");
        vs.push_back("サンプルテキスト３サンプルテキスト３サンプルテキスト３");
        vss.push_back(vs);
        vs.clear();
        vs.push_back("サンプルテキスト４");
        vs.push_back("サンプルテキスト５サンプルテキスト５");
        vs.push_back("サンプルテキスト６サンプルテキスト６サンプルテキスト６");
        vss.push_back(vs);
        vs.clear();
        vs.push_back("サンプルテキスト７サンプルテキスト７サンプルテキスト７サンプルテキスト７");
        vs.push_back("サンプルテキスト８サンプルテキスト８サンプルテキスト８");
        vs.push_back("サンプルテキスト９サンプルテキスト９");
        vss.push_back(vs);
        vs.clear();
        vs.push_back("サンプルテキスト１０");
        vss.push_back(vs);
        PopUp::Get()->SetText(vss);
    }

    if (KeyBoard::IsDown(DIK_F8))
    {
        PopUp::Get()->Next();
    }

    if (KeyBoard::IsDown(DIK_F9))
    {
        PopUp::Get()->Cancel();
    }

    PopUp::Get()->Update();

    if (KeyBoard::IsDown(DIK_F1))
    {
        if (m_bShowStorehouse == false)
        {
            m_bShowStorehouse = true;
            delete m_storehouse;
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
                std::vector<std::string> vs;

                vs.push_back("アイテムＡＡＡ");
                vs.push_back("武器ＢＢＢ");
                vs.push_back("アイテムＣ");
                vs.push_back("アイテムＤ");
                vs.push_back("アイテムＥ");
                vs.push_back("アイテムＦ");
                vs.push_back("アイテムＧ");
                vs.push_back("アイテムＨ");
                vs.push_back("アイテムＩ");
                vs.push_back("アイテムＪ");
                vs.push_back("アイテムＫ");
                vs.push_back("アイテムＬ");
                vs.push_back("アイテムＭ");
                vs.push_back("アイテムＮ");
                vs.push_back("アイテムＯ");
                vs.push_back("アイテムＰ");
                m_storehouse->SetInventoryList(vs);

                vs.clear();
                vs.push_back("アイテム１");
                vs.push_back("アイテム２");
                vs.push_back("アイテム３");
                vs.push_back("アイテム４");
                vs.push_back("アイテム５");
                vs.push_back("アイテム６");
                vs.push_back("アイテム７");
                vs.push_back("アイテム８");
                vs.push_back("アイテム９");
                vs.push_back("アイテム１０");
                vs.push_back("アイテム１１");
                vs.push_back("アイテム１２");
                vs.push_back("アイテム１３");
                vs.push_back("アイテム１４");
                m_storehouse->SetStorehouseList(vs);
            }
        }
    }

    if (Mouse::IsDownLeft())
    {
        InputR1();
    }

    if (GamePad::IsLeftStickUsed())
    {
        float joyRadian = GamePad::GetLeftRadian();
        float cameRadian = Camera::GetRadian();
        float radian = joyRadian + (cameRadian - D3DX_PI * 3 / 2);
        move.x += std::cos(radian) / 5;
        move.z += std::sin(radian) / 5;

        yaw = -1.f * (radian + D3DX_PI / 2);
        D3DXVECTOR3 rotate { 0.f, yaw, 0.f };
        m_player->SetRotate(rotate);
        m_player->SetWalk();
    }
    if (GamePad::IsDown(eJoyStickButtonType::R1))
    {
        InputR1();
    }
    if (GamePad::IsDown(eJoyStickButtonType::B))
    {
        InputB(sequence);
    }
    if (GamePad::IsDown(eJoyStickButtonType::A))
    {
        InputA();
    }

    if (isHit == false)
    {
        m_player->SetMove(m_player->GetMove() + move);
        D3DXVECTOR3 pos = m_player->GetPos();
        pos.y += 1.f;
        Camera::SetLookAtPos(pos);
    }
    m_player->Update(m_map);
    if (m_player->GetHP() <= 0)
    {
        m_player->SetDead();
        m_eState = eBattleState::GAMEOVER;
    }
    if (m_eState == eBattleState::GAMEOVER)
    {
        ++m_nGameoverCounter;
        if (m_nGameoverCounter >= 120)
        {
            *sequence = eSequence::TITLE;
        }
    }
    if (m_nCurrentStage == 1)
    {
        if (m_map->GetEnemy().size() == 0)
        {
//            SAFE_DELETE(m_map);
//            m_stage2 = new Stage2();
//            m_stage2->Init();
//            m_player->SetHP(100);
//            m_player->SetPos(D3DXVECTOR3 { 0.f, 0.f, 0.f });
//            Camera::SetRadian(D3DX_PI);
//            m_nCurrentStage = 2;
//
//            SaveLastStage(2);
        }
    }
    else if (m_nCurrentStage == 2)
    {
        {
            *sequence = eSequence::ENDING;
        }
    }

    // 60回に一回くらいクエスト管理クラスにプレイヤーの現在地を知らせる。
    {
        static int counter = 0;
        counter++;
        if (counter >= 60)
        {
            counter = 0;
        }
        if (counter == 0)
        {
            D3DXVECTOR3 playerPos = SharedObj::GetPlayer()->GetPos();
            SharedObj::GetQuestSystem()->SetPos(playerPos.x, playerPos.y, playerPos.z);

            // プレイヤーの現在座標で開始or完了できるクエストがあるなら
            // 「調べる」アクションができることをアイコンで知らせる。
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
}

void SeqBattle::OperateMenu(eSequence* sequence)
{
    std::string result = m_menuManager.OperateMenu();
    if (result == "最初から")
    {
        m_bShowMenu = false;
        Camera::SleepModeOFF();
        ShowCursor(false);
        {
            RECT rect;
            rect.left = 100;
            rect.top = 100;
            rect.right = 1600 + 100;
            rect.bottom = 900 + 100;
            ClipCursor(&rect);
        }
        // TODO fix
        *sequence = eSequence::TITLE;
    }
    else if (result == "タイトル")
    {
        m_bShowMenu = false;
        Camera::SleepModeOFF();
        ShowCursor(false);
        {
            RECT rect;
            rect.left = 100;
            rect.top = 100;
            rect.right = 1600 + 100;
            rect.bottom = 900 + 100;
            ClipCursor(&rect);
        }
        *sequence = eSequence::TITLE;
    }
    else if (result == "EXIT")
    {
        m_bShowMenu = false;
        Camera::SleepModeOFF();
        ShowCursor(false);
        {
            RECT rect;
            rect.left = 100;
            rect.top = 100;
            rect.right = 1600 + 100;
            rect.bottom = 900 + 100;
            ClipCursor(&rect);
        }
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

    if (KeyBoard::IsDown(DIK_SPACE))
    {
        m_talk->Next();
    }
}

void SeqBattle::OperateStorehouse()
{
    std::string result;

    if (KeyBoard::IsDown(DIK_F1))
    {
        m_bShowStorehouse = false;
    }

    if (KeyBoard::IsDown(DIK_UP))
    {
        m_storehouse->Up();
    }

    if (KeyBoard::IsDown(DIK_DOWN))
    {
        m_storehouse->Down();
    }

    if (KeyBoard::IsDown(DIK_LEFT))
    {
        m_storehouse->Left();
    }

    if (KeyBoard::IsDown(DIK_RIGHT))
    {
        m_storehouse->Right();
    }

    if (KeyBoard::IsDown(DIK_RETURN))
    {
        m_storehouse->Into();

        if (result == "タイトル")
        {
            //m_bShowMenu = false;
        }
        else if (result == "最初から")
        {
            //m_bShowMenu = false;
        }
    }

    if (KeyBoard::IsDown(DIK_ESCAPE))
    {
        result = m_storehouse->Back();
    }

    if (Mouse::IsDownLeft())
    {
        POINT p;
        GetCursorPos(&p);
        ScreenToClient(FindWindowA("ホシマン", nullptr), &p);
        m_storehouse->Click(p.x, p.y);
    }

    if (Mouse::GetZDelta() < 0)
    {
        m_storehouse->Next();
    }
    else if (Mouse::GetZDelta() > 0)
    {
        m_storehouse->Previous();
    }
    return;
}

void SeqBattle::Render()
{
    m_player->Render();
    D3DXVECTOR3 pos { 0.f, 0.f, 0.f };
    if (m_player->GetDead())
    {
        m_spriteGameover->Render(pos);
    }
    if (m_nCurrentStage == 1)
    {
        m_map->Render();
    }

    PopUp::Get()->Render();

    if (m_bTalking)
    {
        m_talk->Render();
    }

    if (m_bShowExamine)
    {
        D3DXVECTOR3 pos { 200.f, 600.f, 0.f };
        m_spriteExamine->Render(pos);
    }
    
    if (m_bShowMenu)
    {
        m_menuManager.Draw();
    }

    if (m_bShowStorehouse)
    {
        m_storehouse->Draw();
    }
}

void SeqBattle::InputR1()
{
    bool ret { m_player->SetAttack() };
    if (ret == false)
    {
        return;
    }
    D3DXVECTOR3 attackPos { m_player->GetAttackPos() };
    D3DXVECTOR3 enemyPos { 0.f, 0.f, 0.f };
    std::vector<Enemy> vecEnemy;
    std::vector<EnemySphere> vecEnemySphere;
    std::vector<EnemyDisk> vecEnemyDisk;
    if (m_nCurrentStage == 1)
    {
        vecEnemy = m_map->GetEnemy();
    }
    else if (m_nCurrentStage == 2)
    {
        //vecEnemy = m_stage2->GetEnemy();
        //vecEnemySphere = m_stage2->GetEnemySphere();
    }
    for (std::size_t i = 0; i < vecEnemy.size(); i++)
    {
        D3DXVECTOR3 enemyPos { 0.f, 0.f, 0.f };
        enemyPos = vecEnemy.at(i).GetPos();

        D3DXVECTOR3 subPos { attackPos - enemyPos };
        FLOAT distance = D3DXVec3Length(&subPos);

        if (distance <= 1.5f)
        {
            vecEnemy.at(i).SetState(eState::DAMAGED);
            int hp = vecEnemy.at(i).GetHP();
            vecEnemy.at(i).SetHP(hp - 50);
        }
    }
    for (std::size_t i = 0; i < vecEnemySphere.size(); i++)
    {
        D3DXVECTOR3 enemyPos { 0.f, 0.f, 0.f };
        enemyPos = vecEnemySphere.at(i).GetPos();

        D3DXVECTOR3 subPos { attackPos - enemyPos };
        FLOAT distance = D3DXVec3Length(&subPos);

        if (distance <= 1.5f)
        {
            vecEnemySphere.at(i).SetState(eSphereState::DAMAGED);
            int hp = vecEnemySphere.at(i).GetHP();
            vecEnemySphere.at(i).SetHP(hp - 40);
        }
    }
    for (std::size_t i = 0; i < vecEnemyDisk.size(); i++)
    {
        D3DXVECTOR3 enemyPos { 0.f, 0.f, 0.f };
        enemyPos = vecEnemyDisk.at(i).GetPos();

        D3DXVECTOR3 subPos { attackPos - enemyPos };
        FLOAT distance = D3DXVec3Length(&subPos);

        if (distance <= 1.5f)
        {
            vecEnemyDisk.at(i).SetState(eDiskState::DAMAGED);
            int hp = vecEnemyDisk.at(i).GetHP();
            vecEnemyDisk.at(i).SetHP(hp - 40);
        }
    }
    if (m_nCurrentStage == 1)
    {
        m_map->SetEnemy(vecEnemy);
    }
    else if (m_nCurrentStage == 2)
    {
        //m_stage2->SetEnemy(vecEnemy);
        //m_stage2->SetEnemySphere(vecEnemySphere);
    }
}

void SeqBattle::InputA()
{
    m_player->SetJump();
}

void SeqBattle::InputB(eSequence* sequence)
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
        // 調べるコマンド
        // プレイヤーの現在座標で始まるクエストか終わるクエストがある。
        D3DXVECTOR3 playerPos = SharedObj::GetPlayer()->GetPos();
        if (m_bShowExamine)
        {
            SharedObj::GetQuestSystem()->SetExamine(playerPos.x, playerPos.y, playerPos.z);
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

