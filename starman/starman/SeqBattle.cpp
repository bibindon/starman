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

namespace NSStoryTelling
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
        D3DXVECTOR3 pos { (float)x, (float)y, 0.f };
        m_D3DSprite->Begin(D3DXSPRITE_ALPHABLEND);
        RECT rect = {
            0,
            0,
            static_cast<LONG>(m_width),
            static_cast<LONG>(m_height) };
        D3DXVECTOR3 center { 0, 0, 0 };
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
        LPD3DXSPRITE tempSprite { nullptr };
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

        D3DSURFACE_DESC desc { };
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
        m_D3DSprite = nullptr;
        m_pD3DTexture->Release();
        m_pD3DTexture = nullptr;
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
        m_pFont = nullptr;
    }

private:

    LPDIRECT3DDEVICE9 m_pD3DDevice = NULL;
    LPD3DXFONT m_pFont = NULL;
};

class SoundEffect : public ISoundEffect
{
    virtual void PlayMove() override
    {
        PlaySound("cursor_move.wav", NULL, SND_FILENAME | SND_ASYNC);
    }
    virtual void Init() override
    {

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

    BGM::get_ton()->load("res\\sound\\letsgo.wav");
    BGM::get_ton()->play(10);

    m_spriteGameover = new Sprite("res\\image\\gameover.png");

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
                m_stage1 = new Stage1();
                m_stage1->Init();
            }
            else if (m_nCurrentStage == 2)
            {
                m_stage2 = new Stage2();
                m_stage2->Init();
            }
            else if (m_nCurrentStage == 3)
            {
                m_stage3 = new Stage3();
                m_stage3->Init();
            }
            else if (m_nCurrentStage == 4)
            {
                m_stage4 = new Stage4();
                m_stage4->Init();
            }
            else if (m_nCurrentStage == 5)
            {
                m_stage5 = new Stage5();
                m_stage5->Init();
            }
            else if (m_nCurrentStage == 6)
            {
                m_stage6 = new Stage6();
                m_stage6->Init();
            }
            else if (m_nCurrentStage == 7)
            {
                m_stage7 = new Stage7();
                m_stage7->Init();
            }
            else if (m_nCurrentStage == 8)
            {
                m_stage8 = new Stage8();
                m_stage8->Init();
            }
            else if (m_nCurrentStage == 9)
            {
                m_stage9 = new Stage9();
                m_stage9->Init();
            }
            else if (m_nCurrentStage == 10)
            {
                m_stage10 = new Stage10();
                m_stage10->Init();
            }
        }
        else
        {
            m_stage1 = new Stage1();
            m_stage1->Init();
        }
    }
    else
    {
        m_stage1 = new Stage1();
        m_stage1->Init();
    }
    {
        ISoundEffect* pSE = new NSStoryTelling::SoundEffect();

        NSStoryTelling::Sprite* sprTextBack = new NSStoryTelling::Sprite(SharedObj::GetD3DDevice());
        sprTextBack->Load("res\\image\\textBack.png");

        NSStoryTelling::Sprite* sprFade = new NSStoryTelling::Sprite(SharedObj::GetD3DDevice());
        sprFade->Load("res\\image\\black.png");

        IFont* pFont = new NSStoryTelling::Font(SharedObj::GetD3DDevice());
        pFont->Init();

        std::vector<Page> pageList;
        {
            Page page;
            NSStoryTelling::Sprite* sprite = new NSStoryTelling::Sprite(SharedObj::GetD3DDevice());
            sprite->Load("res\\image\\opening01.png");
            page.SetSprite(sprite);
            std::vector<std::vector<std::string> > vss;
            std::vector<std::string> vs;
            vs.push_back("サンプルテキスト１");
            vs.push_back("サンプルテキスト２");
            vs.push_back("サンプルテキスト３");
            vss.push_back(vs);
            vs.clear();
            vs.push_back("サンプルテキスト４サンプルテキスト４サンプルテキスト４");
            vs.push_back("サンプルテキスト５サンプルテキスト５サンプルテキスト５");
            vs.push_back("サンプルテキスト６サンプルテキスト６サンプルテキスト６");
            vss.push_back(vs);
            vs.clear();
            vs.push_back("サンプルテキスト７サンプルテキスト７サンプルテキスト７サンプルテキスト７サンプルテキスト７");
            vs.push_back("サンプルテキスト８サンプルテキスト８サンプルテキスト８サンプルテキスト８サンプルテキスト８");
            vs.push_back("サンプルテキスト９サンプルテキスト９サンプルテキスト９サンプルテキスト９サンプルテキスト９");
            vss.push_back(vs);
            page.SetTextList(vss);
            pageList.push_back(page);
        }
        {
            Page page;
            NSStoryTelling::Sprite* sprite = new NSStoryTelling::Sprite(SharedObj::GetD3DDevice());
            sprite->Load("res\\image\\opening02.png");
            page.SetSprite(sprite);
            std::vector<std::vector<std::string> > vss;
            std::vector<std::string> vs;
            vs.push_back("サンプルテキストＡ");
            vs.push_back("サンプルテキストＢ");
            vs.push_back("サンプルテキストＣ");
            vss.push_back(vs);
            vs.clear();
            vs.push_back("サンプルテキストＤサンプルテキストＤサンプルテキストＤ");
            vs.push_back("サンプルテキストＥサンプルテキストＥ");
            vs.push_back("サンプルテキストＦ");
            vss.push_back(vs);
            page.SetTextList(vss);
            pageList.push_back(page);
        }
        {
            Page page;
            NSStoryTelling::Sprite* sprite = new NSStoryTelling::Sprite(SharedObj::GetD3DDevice());
            sprite->Load("res\\image\\opening03.png");
            page.SetSprite(sprite);
            std::vector<std::vector<std::string> > vss;
            std::vector<std::string> vs;
            vs.push_back("１１１１１１１１１１１");
            vs.push_back("２２２２２２２２２２２２２");
            vs.push_back("３３３３３３３３３３３３３３３３３");
            vss.push_back(vs);
            vs.clear();
            vs.push_back("４４４４４４４４４４４４４４４４４４４４４４４４４４４４４４４４４");
            vs.push_back("");
            vss.push_back(vs);
            vs.clear();
            vs.push_back("５５５５５５５５５５５５５５５５５");
            vss.push_back(vs);
            page.SetTextList(vss);
            pageList.push_back(page);
        }

        m_storyTelling = new StoryTelling();
        m_storyTelling->Init(pFont, pSE, sprTextBack, sprFade, pageList);
    }
}

SeqBattle::~SeqBattle()
{
}

void SeqBattle::Update(eSequence* sequence)
{
    if (m_nCurrentStage == 1)
    {
        m_stage1->Update();
    }
    else if (m_nCurrentStage == 2)
    {
        m_stage2->Update();
    }
    else if (m_nCurrentStage == 3)
    {
        m_stage3->Update();
    }
    else if (m_nCurrentStage == 4)
    {
        m_stage4->Update();
    }
    else if (m_nCurrentStage == 5)
    {
        m_stage5->Update();
    }
    else if (m_nCurrentStage == 6)
    {
        m_stage6->Update();
    }
    else if (m_nCurrentStage == 7)
    {
        m_stage7->Update();
    }
    else if (m_nCurrentStage == 8)
    {
        m_stage8->Update();
    }
    else if (m_nCurrentStage == 9)
    {
        m_stage9->Update();
    }
    else if (m_nCurrentStage == 10)
    {
        m_stage10->Update();
    }
    D3DXVECTOR3 move { 0.f, 0.f, 0.f };
    D3DXVECTOR3 rotate { 0.f, 0.f, 0.f };
    float radian = Camera::GetRadian();
    float yaw = -1.f * (radian - (D3DX_PI / 2));
    bool isHit { false };
    if (KeyBoard::IsHold(DIK_W))
    {
        move.x += -std::sin(radian + D3DX_PI / 2) / 5;
        move.z += std::sin(radian + D3DX_PI) / 5;

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
    if (KeyBoard::IsDown(DIK_F))
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
        Camera::SetPos(m_player->GetPos());
    }
    m_player->Update(m_stage1);
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
        if (m_stage1->GetEnemy().size() == 0)
        {
            SAFE_DELETE(m_stage1);
            m_stage2 = new Stage2();
            m_stage2->Init();
            m_player->SetHP(100);
            m_player->SetPos(D3DXVECTOR3 { 0.f, 0.f, 0.f });
            Camera::SetRadian(D3DX_PI);
            m_nCurrentStage = 2;

            SaveLastStage(2);

        }
    }
    else if (m_nCurrentStage == 2)
    {
        if (m_stage2->GetEnemy().size() == 0 &&
            m_stage2->GetEnemySphere().size() == 0)
        {
            SAFE_DELETE(m_stage2);
            m_stage3 = new Stage3();
            m_stage3->Init();
            m_player->SetHP(100);
            m_player->SetPos(D3DXVECTOR3 { 0.f, 0.f, 0.f });
            Camera::SetRadian(D3DX_PI);
            m_nCurrentStage = 3;
            SaveLastStage(3);
        }
    }
    else if (m_nCurrentStage == 3)
    {
        if (m_stage3->GetEnemy().size() == 0)
        {
            SAFE_DELETE(m_stage3);
            m_stage4 = new Stage4();
            m_stage4->Init();
            m_player->SetHP(100);
            m_player->SetPos(D3DXVECTOR3 { 0.f, 0.f, 0.f });
            Camera::SetRadian(D3DX_PI);
            m_nCurrentStage = 4;
            SaveLastStage(4);
        }
    }
    else if (m_nCurrentStage == 4)
    {
        if (m_stage4->GetEnemy().size() == 0)
        {
            SAFE_DELETE(m_stage4);
            m_stage5 = new Stage5();
            m_stage5->Init();
            m_player->SetHP(100);
            m_player->SetPos(D3DXVECTOR3 { 0.f, 0.f, 0.f });
            Camera::SetRadian(D3DX_PI);
            m_nCurrentStage = 5;
            SaveLastStage(5);
        }
    }
    else if (m_nCurrentStage == 5)
    {
        if (m_stage5->GetEnemy().size() == 0)
        {
            SAFE_DELETE(m_stage5);
            m_stage6 = new Stage6();
            m_stage6->Init();
            m_player->SetHP(100);
            m_player->SetPos(D3DXVECTOR3 { 0.f, 0.f, 0.f });
            Camera::SetRadian(D3DX_PI);
            m_nCurrentStage = 6;
            SaveLastStage(6);
        }
    }
    else if (m_nCurrentStage == 6)
    {
        if (m_stage6->GetEnemy().size() == 0)
        {
            SAFE_DELETE(m_stage6);
            m_stage7 = new Stage7();
            m_stage7->Init();
            m_player->SetHP(100);
            m_player->SetPos(D3DXVECTOR3 { 0.f, 0.f, 0.f });
            Camera::SetRadian(D3DX_PI);
            m_nCurrentStage = 7;
            SaveLastStage(7);
        }
    }
    else if (m_nCurrentStage == 7)
    {
        if (m_stage7->GetEnemy().size() == 0)
        {
            SAFE_DELETE(m_stage7);
            m_stage8 = new Stage8();
            m_stage8->Init();
            m_player->SetHP(100);
            m_player->SetPos(D3DXVECTOR3 { 0.f, 0.f, 0.f });
            Camera::SetRadian(D3DX_PI);
            m_nCurrentStage = 8;
            SaveLastStage(8);
        }
    }
    else if (m_nCurrentStage == 8)
    {
        if (m_stage8->GetEnemy().size() == 0)
        {
            SAFE_DELETE(m_stage8);
            m_stage9 = new Stage9();
            m_stage9->Init();
            m_player->SetHP(100);
            m_player->SetPos(D3DXVECTOR3 { 0.f, 0.f, 0.f });
            Camera::SetRadian(D3DX_PI);
            m_nCurrentStage = 9;
            SaveLastStage(9);
        }
    }
    else if (m_nCurrentStage == 9)
    {
        if (m_stage9->GetEnemy().size() == 0)
        {
            SAFE_DELETE(m_stage9);
            m_stage10 = new Stage10();
            m_stage10->Init();
            m_player->SetHP(100);
            m_player->SetPos(D3DXVECTOR3 { 0.f, 0.f, 0.f });
            Camera::SetRadian(D3DX_PI);
            m_nCurrentStage = 10;
            SaveLastStage(10);
        }
    }
    else if (m_nCurrentStage == 10)
    {
        if (m_stage10->GetEnemy().size() == 0)
        {
            SAFE_DELETE(m_stage10);
            *sequence = eSequence::ENDING;
        }
    }

    if (m_storyTelling != nullptr)
    {
        bFinish = m_storyTelling->Update();
        if (bFinish)
        {
            m_storyTelling->Finalize();
            delete m_storyTelling;
            m_storyTelling = nullptr;
        }
    }

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
        m_stage1->Render();
    }
    else if (m_nCurrentStage == 2)
    {
        m_stage2->Render();
    }
    else if (m_nCurrentStage == 3)
    {
        m_stage3->Render();
    }
    else if (m_nCurrentStage == 4)
    {
        m_stage4->Render();
    }
    else if (m_nCurrentStage == 5)
    {
        m_stage5->Render();
    }
    else if (m_nCurrentStage == 6)
    {
        m_stage6->Render();
    }
    else if (m_nCurrentStage == 7)
    {
        m_stage7->Render();
    }
    else if (m_nCurrentStage == 8)
    {
        m_stage8->Render();
    }
    else if (m_nCurrentStage == 9)
    {
        m_stage9->Render();
    }
    else if (m_nCurrentStage == 10)
    {
        m_stage10->Render();
    }
    PopUp::Get()->Render();

    if (m_storyTelling != nullptr)
    {
        m_storyTelling->Render();
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
        vecEnemy = m_stage1->GetEnemy();
    }
    else if (m_nCurrentStage == 2)
    {
        vecEnemy = m_stage2->GetEnemy();
        vecEnemySphere = m_stage2->GetEnemySphere();
    }
    else if (m_nCurrentStage == 3)
    {
        vecEnemy = m_stage3->GetEnemy();
        vecEnemyDisk = m_stage3->GetEnemyDisk();
    }
    else if (m_nCurrentStage == 4)
    {
        vecEnemy = m_stage4->GetEnemy();
    }
    else if (m_nCurrentStage == 5)
    {
        vecEnemy = m_stage5->GetEnemy();
    }
    else if (m_nCurrentStage == 6)
    {
        vecEnemy = m_stage6->GetEnemy();
    }
    else if (m_nCurrentStage == 7)
    {
        vecEnemy = m_stage7->GetEnemy();
    }
    else if (m_nCurrentStage == 8)
    {
        vecEnemy = m_stage8->GetEnemy();
    }
    else if (m_nCurrentStage == 9)
    {
        vecEnemy = m_stage9->GetEnemy();
    }
    else if (m_nCurrentStage == 10)
    {
        vecEnemy = m_stage10->GetEnemy();
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
        m_stage1->SetEnemy(vecEnemy);
    }
    else if (m_nCurrentStage == 2)
    {
        m_stage2->SetEnemy(vecEnemy);
        m_stage2->SetEnemySphere(vecEnemySphere);
    }
    else if (m_nCurrentStage == 3)
    {
        m_stage3->SetEnemy(vecEnemy);
        m_stage3->SetEnemyDisk(vecEnemyDisk);
    }
    else if (m_nCurrentStage == 4)
    {
        m_stage4->SetEnemy(vecEnemy);
    }
    else if (m_nCurrentStage == 5)
    {
        m_stage5->SetEnemy(vecEnemy);
    }
    else if (m_nCurrentStage == 6)
    {
        m_stage6->SetEnemy(vecEnemy);
    }
    else if (m_nCurrentStage == 7)
    {
        m_stage7->SetEnemy(vecEnemy);
    }
    else if (m_nCurrentStage == 8)
    {
        m_stage8->SetEnemy(vecEnemy);
    }
    else if (m_nCurrentStage == 9)
    {
        m_stage9->SetEnemy(vecEnemy);
    }
    else if (m_nCurrentStage == 10)
    {
        m_stage10->SetEnemy(vecEnemy);
    }
}

void SeqBattle::InputA()
{
    m_player->SetJump();

    if (m_storyTelling != nullptr)
    {
        m_storyTelling->Next();
    }
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
}

void SeqBattle::SaveLastStage(const int stageNum)
{
    std::ofstream outputfile(".savedata");
    outputfile << stageNum;
    outputfile.close();
    SetFileAttributes(".savedata", FILE_ATTRIBUTE_HIDDEN);
}

