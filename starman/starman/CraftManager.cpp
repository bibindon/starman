#include "CraftManager.h"
#include "KeyBoard.h"
#include "Camera.h"
#include "Mouse.h"
#include "GamePad.h"
#include "SoundEffect.h"
#include "SharedObj.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/CraftSystem.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/CraftInfoManager.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/ActivityBase.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/WeaponManager.h"
#include "PopUp2.h"
#include <cassert>
#include "Common.h"
#include "resource.h"

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
        D3DXVECTOR3 pos { (float)x, (float)y, 0.f };
        m_D3DSprite->Begin(D3DXSPRITE_ALPHABLEND);
        RECT rect = {
            0,
            0,
            static_cast<LONG>(m_width),
            static_cast<LONG>(m_height) };
        D3DXVECTOR3 center { 0, 0, 0 };
        m_D3DSprite->Draw(
            m_texMap.at(m_filepath),
            &rect,
            &center,
            &pos,
            D3DCOLOR_ARGB(transparency, 255, 255, 255));
        m_D3DSprite->End();

    }

    void Load(const std::string& filepath) override
    {
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
            D3DSURFACE_DESC desc { };
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

        D3DSURFACE_DESC desc { };
        if (FAILED(pD3DTexture->GetLevelDesc(0, &desc)))
        {
            throw std::exception("Failed to create a texture.");
        }
        m_width = desc.Width;
        m_height = desc.Height;
    }

    ~Sprite()
    {
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

private:

    LPDIRECT3DDEVICE9 m_pD3DDevice = NULL;

    // スプライトは一つを使いまわす
    static LPD3DXSPRITE m_D3DSprite;
    std::string m_filepath;

    UINT m_width = 0;
    UINT m_height = 0;

    // 同じ名前の画像ファイルで作られたテクスチャは使いまわす
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
        HRESULT hr = S_OK;

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
                                "ＭＳ 明朝",
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
                                "Calibri",
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
public:
    SoundEffect()
    {
    }

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

void CraftManager::Init()
{
    NSCraftLib::Sprite* sprCursor = NEW NSCraftLib::Sprite(SharedObj::GetD3DDevice());
    sprCursor->Load("res\\image\\menu_cursor.png");

    NSCraftLib::Sprite* sprBackground = NEW NSCraftLib::Sprite(SharedObj::GetD3DDevice());
    sprBackground->Load("res\\image\\background.png");

    NSCraftLib::Sprite* sprPanelLeft = NEW NSCraftLib::Sprite(SharedObj::GetD3DDevice());
    sprPanelLeft->Load("res\\image\\panelLeft.png");

    NSCraftLib::Sprite* sprPanelTop = NEW NSCraftLib::Sprite(SharedObj::GetD3DDevice());
    sprPanelTop->Load("res\\image\\craftPanel.png");

    NSCraftLib::IFont* pFont = NEW NSCraftLib::Font(SharedObj::GetD3DDevice());

    NSCraftLib::ISoundEffect* pSE = NEW NSCraftLib::SoundEffect();

    m_gui.Init(pFont, pSE, sprCursor, sprBackground,
               sprPanelLeft, sprPanelTop, SharedObj::IsEnglish());

    Build();
}

void CraftManager::Finalize()
{
    m_gui.Finalize();
}

// この関数はクラフト画面が表示されていないときも呼ばれることに気を付ける
void CraftManager::Update()
{
    auto craftSys = NSStarmanLib::CraftSystem::GetObj();

    static int counter = 0;
    ++counter;

    // 1秒に一回
    if (counter % 60 == 1)
    {
        craftSys->UpdateCraftStatus();
    }
}

void CraftManager::Operate(eBattleState* state)
{
    auto craftSys = NSStarmanLib::CraftSystem::GetObj();

    // 定期的に画面を作り直す。
    static int counter = 0;
    ++counter;

    // 1秒に一回
    if (counter % 60 == 1)
    {
        if (*state == eBattleState::CRAFT)
        {
            // 画面更新
            Build();
        }
    }

    std::string result;

    if (SharedObj::KeyBoard()->IsDownFirstFrame(DIK_UP))
    {
        m_gui.Up();
    }

    if (SharedObj::KeyBoard()->IsHold(DIK_UP))
    {
        m_gui.Up();
    }

    if (SharedObj::KeyBoard()->IsDownFirstFrame(DIK_DOWN))
    {
        m_gui.Down();
    }

    if (SharedObj::KeyBoard()->IsHold(DIK_DOWN))
    {
        m_gui.Down();
    }

    if (SharedObj::KeyBoard()->IsDownFirstFrame(DIK_LEFT))
    {
        m_gui.Left();
    }

    if (SharedObj::KeyBoard()->IsDownFirstFrame(DIK_RIGHT))
    {
        m_gui.Right();
    }

    if (SharedObj::KeyBoard()->IsDownFirstFrame(DIK_RETURN))
    {
        result = m_gui.Into();
    }

    if (SharedObj::KeyBoard()->IsDownFirstFrame(DIK_ESCAPE))
    {
        result = m_gui.Back();
    }

    if (SharedObj::KeyBoard()->IsDownFirstFrame(DIK_BACK))
    {
        result = m_gui.Back();
    }

    if (Mouse::IsDownLeft())
    {
        POINT p = Common::GetScreenPos();
        result = m_gui.Click(p.x, p.y);
    }
    else
    {
        static POINT previousPoint = { 0, 0 };
        POINT p = Common::GetScreenPos();

        if (p.x == previousPoint.x &&
            p.y == previousPoint.y)
        {
            // do nothing
        }
        else
        {
            m_gui.CursorOn(p.x, p.y);
        }

        previousPoint = p;
    }

    if (Mouse::GetZDelta() < 0)
    {
        m_gui.Next();
    }
    else if (Mouse::GetZDelta() > 0)
    {
        m_gui.Previous();
    }

    if (GamePad::IsDown(eGamePadButtonType::UP))
    {
        m_gui.Up();
    }

    if (GamePad::IsHold(eGamePadButtonType::UP))
    {
        m_gui.Up();
    }

    if (GamePad::IsDown(eGamePadButtonType::DOWN))
    {
        m_gui.Down();
    }

    if (GamePad::IsHold(eGamePadButtonType::DOWN))
    {
        m_gui.Down();
    }

    if (GamePad::IsDown(eGamePadButtonType::LEFT))
    {
        m_gui.Left();
    }

    if (GamePad::IsDown(eGamePadButtonType::RIGHT))
    {
        m_gui.Right();
    }

    if (GamePad::IsDown(eGamePadButtonType::A))
    {
        result = m_gui.Into();
    }

    if (GamePad::IsDown(eGamePadButtonType::B))
    {
        result = m_gui.Back();
    }

    if (!result.empty())
    {
        if (result == "EXIT")
        {
            *state = eBattleState::NORMAL;
            Camera::SetCameraMode(eCameraMode::BATTLE);
            Common::SetCursorVisibility(false);
        }
        // イカダの場合は活動拠点の船着き場にイカダがないかチェック
        else if (result == Common::LoadString_(IDS_STRING169))
        {
            bool raftExist = NSStarmanLib::ActivityBase::Get()->CheckRaftExist();

            if (raftExist)
            {
                PopUp2::Get()->SetText(IDS_STRING170);
                PopUp2::Get()->SetText(IDS_STRING171);
            }
            else
            {
                // クラフト開始
                std::string work = result;
                auto index = work.find("+");
                if (index != std::string::npos)
                {
                    work.erase(index);
                }

                std::string errMsg;
                bool started = craftSys->QueueCraftRequest(work, &errMsg);
                if (!started)
                {
                    PopUp2::Get()->SetText(errMsg);
                }
                else
                {
                    Build();
                }
            }
        }
        else
        {
            // クラフト開始
            std::string work = result;
            auto index = work.find("+");
            if (index != std::string::npos)
            {
                work.erase(index);
            }

            std::string errMsg;
            bool started = craftSys->QueueCraftRequest(work, &errMsg);
            if (!started)
            {
                PopUp2::Get()->SetText(errMsg);
            }
            else
            {
                Build();
            }
        }
    }
}

void CraftManager::Draw()
{
    m_gui.Draw();
}

void CraftManager::Build()
{
    auto craftSys = NSStarmanLib::CraftSystem::GetObj();
    auto craftInfo = NSStarmanLib::CraftInfoManager::GetObj();

    {
        std::vector<std::string> vs;

        auto infoList = craftInfo->GetCraftItemList();

        // クラフト可能リストにはクラフト可能なアイテムを一覧で表示する。
        // イカダは強化値10まであり、イカダ、イカダ+1、イカダ+2・・・イカダ+10をクラフトできるが
        // リストに表示されるのは「イカダ」だけ
        // 絵の上手い人が下手な絵を描くことが困難なように、自動でクラフト可能な強化値が選ばれるため
        // 強化値を選択できる必要はない
        for (auto& info : infoList)
        {
            auto name = info.GetName();
            auto level = info.GetLevel();
            if (level >= 1)
            {
                continue;
            }
            vs.push_back(name);
        }

        m_gui.SetOutputList(vs);

        auto reqList = craftSys->GetCraftRequestList();

        if (reqList.size() >= 1)
        {
            auto outputName = reqList.front().GetCraftInfo().GetOutput().GetName();
            auto progress = craftSys->GetProgress();

            m_gui.SetCraftingItem(outputName, progress);
        }
        else
        {
            m_gui.SetCraftingItem("", 0);
        }

        vs.clear();

        // 2番目以降を予約リストに表示
        if (reqList.size() >= 2)
        {
            std::string name;
            auto it = reqList.begin();
            ++it;

            for (; it != reqList.end(); ++it)
            {
                name = it->GetCraftInfo().GetOutput().GetName();
                vs.push_back(name);
            }

            m_gui.SetCraftQue(vs);
        }
        else
        {
            m_gui.SetCraftQue(vs);
        }

        // 画像、説明文を登録
        {
            std::string work;

            auto allCraftList = craftInfo->GetCraftItemList();

            // クラフトマンの熟練度に応じて成果物の強化値を変更
            auto craftSys = NSStarmanLib::CraftSystem::GetObj();

            for (auto& info : allCraftList)
            {
                auto skill = craftSys->GetCraftsmanSkill(info.GetName());

                if (skill != info.GetLevel())
                {
                    continue;
                }

                work += Common::LoadString_(IDS_STRING172) + info.GetName() + "\n";
                work += Common::LoadString_(IDS_STRING173) + std::to_string(info.GetNumber()) + "\n";
                if (info.GetLevel() != -1)
                {
                    work += Common::LoadString_(IDS_STRING174) + std::to_string(info.GetLevel()) + "\n";
                }
                else
                {
                    work += Common::LoadString_(IDS_STRING174) + "---\n";
                }
                work += "\n";

                auto materials = craftInfo->GetCraftInfo(info).GetCraftMaterialDef();

                int i = 1;
                for (auto& material : materials)
                {
                    work += Common::LoadStringWithArg(IDS_STRING175, std::to_string(i)) + material.GetName() + "\n";
                    work += Common::LoadStringWithArg(IDS_STRING176, std::to_string(i)) + std::to_string(material.GetNumber()) + "\n";
                    if (material.GetLevel() >= 1)
                    {
                        work += Common::LoadStringWithArg(IDS_STRING177, std::to_string(i)) + std::to_string(material.GetLevel()) + "\n";
                    }
                    work += "\n";

                    ++i;
                }

                m_gui.SetOutputInfo(info.GetName(), work);
                work.clear();

                NSCraftLib::ISprite* sprite1 = NEW NSCraftLib::Sprite(SharedObj::GetD3DDevice());
                auto itemDef = Common::ItemManager()->GetItemDef(info.GetName(), info.GetLevel());
                auto imagePath = itemDef.GetImagePath();

                m_gui.SetOutputImage(info.GetName(), imagePath, sprite1);
            }

        }
    }
}

