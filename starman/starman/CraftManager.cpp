#include "CraftManager.h"
#include "KeyBoard.h"
#include "Camera.h"
#include "Mouse.h"
#include "GamePad.h"
#include "SoundEffect.h"
#include "SharedObj.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/CraftSystem.h"
#include "PopUp2.h"

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
            m_D3DSprite->Draw(m_pD3DTexture,
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

            if (FAILED(D3DXCreateTextureFromFile(m_pD3DDevice,
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
    pFont->Init();

    NSCraftLib::ISoundEffect* pSE = NEW NSCraftLib::SoundEffect();

    m_gui.Init(pFont, pSE, sprCursor, sprBackground, sprPanelLeft, sprPanelTop);
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

    if (KeyBoard::IsDownFirstFrame(DIK_DOWN))
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
        POINT p = Common::GetScreenPos();;
        m_gui.CursorOn(p.x, p.y);
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

    if (GamePad::IsDown(eGamePadButtonType::DOWN))
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
        else
        {
            // クラフト開始
            bool started = craftSys->QueueCraftRequest(result);
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
                name += "+" + level;
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

            for (auto& req : reqList)
            {
                auto output = req.GetCraftInfo().GetOutput();
                work += "成果物の名前：" + output.GetName() + "\n";
                work += "成果物の数：" + std::to_string(output.GetNumber()) + "\n";
                work += "成果物の強化度：" + std::to_string(output.GetLevel()) + "\n";
                work += "\n";

                auto materials = req.GetCraftInfo().GetCraftMaterial();

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
                m_gui.SetOutputInfo(output.GetName(), work);

                NSCraftLib::ISprite* sprite1 = NEW NSCraftLib::Sprite(SharedObj::GetD3DDevice());
                auto itemDef = Common::ItemManager()->GetItemDef(output.GetName(), output.GetLevel());
                auto imagePath = itemDef.GetImagePath();

                m_gui.SetOutputImage(output.GetName(), imagePath, sprite1);
            }

        }
    }
}

