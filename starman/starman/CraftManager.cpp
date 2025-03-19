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

namespace NSCraftLib
{
class Sprite : public ISprite
{
public:

    Sprite(LPDIRECT3DDEVICE9 dev, bool testMode)
        : m_pD3DDevice(dev)
        , m_testMode(testMode)
    {
    }

    void DrawImage(const int x, const int y, const int transparency) override
    {
        if (m_testMode)
        {
            return;
        }

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
        if (m_testMode)
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
        if (m_testMode)
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

private:

    LPDIRECT3DDEVICE9 m_pD3DDevice = NULL;

    // スプライトは一つを使いまわす
    static LPD3DXSPRITE m_D3DSprite;
    std::string m_filepath;

    UINT m_width = 0;
    UINT m_height = 0;

    // 同じ名前の画像ファイルで作られたテクスチャは使いまわす
    static std::unordered_map<std::string, LPDIRECT3DTEXTURE9> m_texMap;

    bool m_testMode = false;
};

LPD3DXSPRITE Sprite::m_D3DSprite = NULL;
std::unordered_map<std::string, LPDIRECT3DTEXTURE9> Sprite::m_texMap;

class Font : public IFont
{
public:

    Font(LPDIRECT3DDEVICE9 pD3DDevice, bool testMode)
        : m_pD3DDevice(pD3DDevice)
        , m_testMode(testMode)
    {
    }

    void Init()
    {
        if (m_testMode)
        {
            return;
        }

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
        if (m_testMode)
        {
            return;
        }

        RECT rect = { x, y, 0, 0 };
        m_pFont->DrawText(NULL, msg.c_str(), -1, &rect, DT_LEFT | DT_NOCLIP,
            D3DCOLOR_ARGB(255, 255, 255, 255));
    }

    ~Font()
    {
        if (m_testMode)
        {
            return;
        }

        m_pFont->Release();
    }

private:

    LPDIRECT3DDEVICE9 m_pD3DDevice = NULL;
    LPD3DXFONT m_pFont = NULL;

    bool m_testMode = false;
};


class SoundEffect : public ISoundEffect
{
public:
    SoundEffect(bool testMode)
        : m_testMode(testMode)
    {
    }

    virtual void PlayMove() override
    {
        if (m_testMode)
        {
            return;
        }

        ::SoundEffect::get_ton()->play("res\\sound\\menu_cursor_move.wav");
    }

    virtual void PlayClick() override
    {
        if (m_testMode)
        {
            return;
        }

        ::SoundEffect::get_ton()->play("res\\sound\\menu_cursor_confirm.wav");
    }

    virtual void PlayBack() override
    {
        if (m_testMode)
        {
            return;
        }

        ::SoundEffect::get_ton()->play("res\\sound\\menu_cursor_cancel.wav");
    }

    virtual void Init() override
    {
        if (m_testMode)
        {
            return;
        }

        ::SoundEffect::get_ton()->load("res\\sound\\menu_cursor_move.wav");
        ::SoundEffect::get_ton()->load("res\\sound\\menu_cursor_confirm.wav");
        ::SoundEffect::get_ton()->load("res\\sound\\menu_cursor_cancel.wav");
    }
private:
    bool m_testMode = false;
};
}

void CraftManager::Init()
{
    NSCraftLib::Sprite* sprCursor = NEW NSCraftLib::Sprite(SharedObj::GetD3DDevice(), m_testMode);
    sprCursor->Load("res\\image\\menu_cursor.png");

    NSCraftLib::Sprite* sprBackground = NEW NSCraftLib::Sprite(SharedObj::GetD3DDevice(), m_testMode);
    sprBackground->Load("res\\image\\background.png");

    NSCraftLib::Sprite* sprPanelLeft = NEW NSCraftLib::Sprite(SharedObj::GetD3DDevice(), m_testMode);
    sprPanelLeft->Load("res\\image\\panelLeft.png");

    NSCraftLib::Sprite* sprPanelTop = NEW NSCraftLib::Sprite(SharedObj::GetD3DDevice(), m_testMode);
    sprPanelTop->Load("res\\image\\craftPanel.png");

    NSCraftLib::IFont* pFont = NEW NSCraftLib::Font(SharedObj::GetD3DDevice(), m_testMode);
    pFont->Init();

    NSCraftLib::ISoundEffect* pSE = NEW NSCraftLib::SoundEffect(m_testMode);

    m_gui.Init(pFont, pSE, sprCursor, sprBackground, sprPanelLeft, sprPanelTop);

    Build();
}

void CraftManager::Finalize()
{
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
        craftSys->UpdateCraftStatus();

        // 画面更新
        Build();
    }

    std::string result;

    if (KeyBoard::IsDownFirstFrame(DIK_UP))
    {
        m_gui.Up();
    }

    if (KeyBoard::IsHold(DIK_UP))
    {
        m_gui.Up();
    }

    if (KeyBoard::IsDownFirstFrame(DIK_DOWN))
    {
        m_gui.Down();
    }

    if (KeyBoard::IsHold(DIK_DOWN))
    {
        m_gui.Down();
    }

    if (KeyBoard::IsDownFirstFrame(DIK_LEFT))
    {
        m_gui.Left();
    }

    if (KeyBoard::IsDownFirstFrame(DIK_RIGHT))
    {
        m_gui.Right();
    }

    if (KeyBoard::IsDownFirstFrame(DIK_RETURN))
    {
        result = m_gui.Into();
    }

    if (KeyBoard::IsDownFirstFrame(DIK_ESCAPE))
    {
        result = m_gui.Back();
    }

    if (KeyBoard::IsDownFirstFrame(DIK_BACK))
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
        else if (result == "イカダ")
        {
            bool raftExist = NSStarmanLib::ActivityBase::Get()->CheckRaftExist();

            if (raftExist)
            {
                PopUp2::Get()->SetText("船着き場にイカダがある");
                PopUp2::Get()->SetText("イカダを移動してから依頼しよう");
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

                bool started = craftSys->QueueCraftRequest(work);
                if (!started)
                {
                    PopUp2::Get()->SetText("クラフト用の素材が足りない");
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

            bool started = craftSys->QueueCraftRequest(work);
            if (!started)
            {
                PopUp2::Get()->SetText("クラフト用の素材が足りない");
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

        for (auto& info : infoList)
        {
            auto name = info.GetName();
            auto level = info.GetLevel();
            if (level >= 1)
            {
                name += "+" + std::to_string(level);
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

        // 画像、説明文を登録
        {
            std::string work;

            auto allCraftList = craftInfo->GetCraftItemList();

            for (auto& info : allCraftList)
            {
                work += "成果物の名前：" + info.GetName() + "\n";
                work += "成果物の数：" + std::to_string(info.GetNumber()) + "\n";
                work += "成果物の強化度：" + std::to_string(info.GetLevel()) + "\n";
                work += "\n";

                auto materials = craftInfo->GetCraftInfo(info).GetCraftMaterialDef();

                int i = 1;
                for (auto& material : materials)
                {
                    work += "素材" + std::to_string(i) + "の名前：" + material.GetName() + "\n";
                    work += "素材" + std::to_string(i) + "の必要数：" + std::to_string(material.GetNumber()) + "\n";
                    if (material.GetLevel() >= 1)
                    {
                        work += "素材" + std::to_string(i) + "の必要強化値：" + std::to_string(material.GetLevel()) + "\n";
                    }
                    work += "\n";

                    ++i;
                }

                std::string work2 = info.GetName();

                if (info.GetLevel() >= 1)
                {
                    work2 += "+" + std::to_string(info.GetLevel());
                }
                m_gui.SetOutputInfo(work2, work);

                NSCraftLib::ISprite* sprite1 = NEW NSCraftLib::Sprite(SharedObj::GetD3DDevice(), m_testMode);
                auto itemDef = Common::ItemManager()->GetItemDef(info.GetName(), info.GetLevel());
                auto imagePath = itemDef.GetImagePath();

                m_gui.SetOutputImage(work2, imagePath, sprite1);
            }

        }
    }
}

void CraftManager::SetTestMode()
{
    m_testMode = true;
}

