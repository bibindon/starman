
#include <d3d9.h>
#include <d3dx9.h>

#include "../../StarmanLib/StarmanLib/StarmanLib/PowereggDateTime.h"
#include "../../StarmanLib\StarmanLib\StarmanLib\ItemManager.h"
#include "../../StarmanLib\StarmanLib\StarmanLib\Inventory.h"

#include "Common.h"
#include "SharedObj.h"
#include "SoundEffect.h"
#include "KeyBoard.h"
#include "Mouse.h"
#include "SaveManager.h"
#include "GamePad.h"

#include "PatchTestManager2.h"
#include "PopUp2.h"

namespace NSPatchTestLib
{
// 画像のファイル名が同じなら読み込まずに共有するようにする。
// さもないと簡単にメモリー不足で落ちる
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
    UINT m_width { 0 };
    UINT m_height { 0 };

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

    void Init()
    {
        HRESULT hr = D3DXCreateFont(m_pD3DDevice,
                                    20,
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

    virtual void DrawText_(const std::string& msg,
                           const int x,
                           const int y,
                           const int transparency)
    {
        RECT rect = { x, y, x + 200, y + 20 };
        m_pFont->DrawText(NULL, msg.c_str(),
                          -1,
                          &rect,
                          DT_VCENTER | DT_NOCLIP,
                          D3DCOLOR_ARGB(transparency, 255, 255, 255));
    }

    ~Font()
    {
        SAFE_RELEASE(m_pFont);
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


void PatchTestManager2::Finalize()
{
}

void PatchTestManager2::InitPatch()
{
    using namespace NSPatchTestLib;
    NSPatchTestLib::Sprite* sprCursor = NEW NSPatchTestLib::Sprite(SharedObj::GetD3DDevice());
    sprCursor->Load("res\\image\\menu_cursor.png");

    NSPatchTestLib::Sprite* sprBackground = NEW NSPatchTestLib::Sprite(SharedObj::GetD3DDevice());
    sprBackground->Load("res\\image\\menu_back.png");

    NSPatchTestLib::Sprite* sprVBar = NEW NSPatchTestLib::Sprite(SharedObj::GetD3DDevice());
    sprVBar->Load("res\\image\\vbar.png");

    NSPatchTestLib::IFont* pFont = NEW NSPatchTestLib::Font(SharedObj::GetD3DDevice());
    pFont->Init();

    NSPatchTestLib::ISoundEffect* pSE = NEW NSPatchTestLib::SoundEffect();
    pSE->Init();

    m_guiLib.Init(pFont, pSE, sprCursor, sprBackground, sprVBar);

    CreateList();
}

std::string PatchTestManager2::Operate()
{
    // 一秒に一回くらいの処理
    {
        static int work = 0;
        ++work;

        if (work >= 60)
        {
            work = 0;
        }

        if (work == 0)
        {
            auto datetime = NSStarmanLib::PowereggDateTime::GetObj();
            m_guiLib.UpdateDateTime(datetime->GetYear(),
                                          datetime->GetMonth(),
                                          datetime->GetDay(),
                                          datetime->GetHour(),
                                          datetime->GetMinute(),
                                          datetime->GetSecond());

            GetPatchLib()->Update();

            // リストを作り直す
            CreateList();
        }
    }

    //-------------------------------------------------
    // Mouse, Keyboard, GamePad
    //-------------------------------------------------

    std::string result;
    std::string work_str;

    if (KeyBoard::IsDownFirstFrame(DIK_UP))
    {
        m_guiLib.Up();
    }

    if (KeyBoard::IsHold(DIK_UP))
    {
        m_guiLib.Up();
    }

    if (KeyBoard::IsDownFirstFrame(DIK_DOWN))
    {
        m_guiLib.Down();
    }

    if (KeyBoard::IsHold(DIK_DOWN))
    {
        m_guiLib.Down();
    }

    if (KeyBoard::IsDownFirstFrame(DIK_LEFT))
    {
        m_guiLib.Left();
    }

    if (KeyBoard::IsDownFirstFrame(DIK_RIGHT))
    {
        m_guiLib.Right();
    }

    if (KeyBoard::IsDownFirstFrame(DIK_RETURN))
    {
        result = m_guiLib.Into();

        QueueTest(result);
    }

    if (KeyBoard::IsDownFirstFrame(DIK_ESCAPE))
    {
        result = m_guiLib.Back();
    }

    if (KeyBoard::IsDownFirstFrame(DIK_BACK))
    {
        result = m_guiLib.Back();
    }

    if (Mouse::IsDownLeft())
    {
        POINT p = Common::GetScreenPos();;
        result = m_guiLib.Click(p.x, p.y);

        QueueTest(result);
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
            m_guiLib.CursorOn(p.x, p.y);
        }
        
        previousPoint = p;
    }

    if (Mouse::GetZDelta() < 0)
    {
        m_guiLib.Next();
    }
    else if (Mouse::GetZDelta() > 0)
    {
        m_guiLib.Previous();
    }

    if (GamePad::IsDown(eGamePadButtonType::UP))
    {
        m_guiLib.Up();
    }

    if (GamePad::IsHold(eGamePadButtonType::UP))
    {
        m_guiLib.Up();
    }

    if (GamePad::IsDown(eGamePadButtonType::DOWN))
    {
        m_guiLib.Down();
    }

    if (GamePad::IsHold(eGamePadButtonType::DOWN))
    {
        m_guiLib.Down();
    }

    if (GamePad::IsDown(eGamePadButtonType::LEFT))
    {
        m_guiLib.Left();
    }

    if (GamePad::IsDown(eGamePadButtonType::RIGHT))
    {
        m_guiLib.Right();
    }

    if (GamePad::IsDown(eGamePadButtonType::A))
    {
        result = m_guiLib.Into();
        QueueTest(result);
    }

    if (GamePad::IsDown(eGamePadButtonType::B))
    {
        result = m_guiLib.Back();
    }

    return result;
}

void PatchTestManager2::Draw()
{
    m_guiLib.Draw();
}

void PatchTestManager2::CreateList()
{
    m_guiLib.ClearAll();

    // 左側のリストを作る
    // 現在所持している物で、パッチテスト対象のものを表示する
    // 同じアイテムを複数所持していたら複数表示する。
    {
        auto keyList = GetPatchLib()->GetKeyList();

        for (size_t i = 0; i < keyList.size(); ++i)
        {
            auto itemDef = Common::ItemManager()->GetItemDef(keyList.at(i));
            auto subIdList = Common::Inventory()->GetSubIdList(itemDef.GetId());

            for (size_t j = 0; j < subIdList.size(); ++j)
            {
                NSPatchTestLib::TestItem testItem;

                testItem.SetName(keyList.at(i));
                testItem.SetId(itemDef.GetId());
                testItem.SetSubId(subIdList.at(j));

                m_guiLib.AddTestItem(testItem);
            }
        }
    }

    // 右側のリストを作る
    {
        auto queList = GetPatchLib()->GetQueue();

        for (size_t i = 0; i < queList.size(); ++i)
        {
            NSPatchTestLib::QueuedTestItem itemInfo;

            itemInfo.SetName(queList.at(i).GetItemName());

            int y, M, d, h, m, s;
            queList.at(i).GetDateTimeReq(&y, &M, &d, &h, &m, &s);
            itemInfo.SetDateReq(y, M, d, h, m, s);

            queList.at(i).GetDateTimeStart(&y, &M, &d, &h, &m, &s);
            itemInfo.SetDateStart(y, M, d, h, m, s);

            if (queList.at(i).GetState() == NSStarmanLib::PatchTest::eState::FINISHED)
            {
                queList.at(i).GetDateTimeEnd(&y, &M, &d, &h, &m, &s);
                itemInfo.SetDateEnd(y, M, d, h, m, s);
            }

            if (queList.at(i).GetState() == NSStarmanLib::PatchTest::eState::NOT_START)
            {
                itemInfo.SetResult("未実施");
            }
            else if (queList.at(i).GetState() == NSStarmanLib::PatchTest::eState::STARTED)
            {
                itemInfo.SetResult("テスト中");
            }
            else if (queList.at(i).GetState() == NSStarmanLib::PatchTest::eState::FINISHED)
            {
                if (queList.at(i).GetResult() == NSStarmanLib::PatchTest::eResult::POISON)
                {
                    itemInfo.SetResult("毒");
                }
                else if (queList.at(i).GetResult() == NSStarmanLib::PatchTest::eResult::NOT_POISON)
                {
                    itemInfo.SetResult("毒ではない");
                }
            }

            m_guiLib.AddQueueItem(itemInfo);
        }
    }

    m_guiLib.UpdateCursorPos();
}

void PatchTestManager2::QueueTest(const std::string& result)
{
    if (result.empty())
    {
        return;
    }

    std::vector<std::string> vs = Common::split(result, ':');

    std::string name = vs.at(1);

    // パッチテストをキューイングする
    bool result2 = GetPatchLib()->QueuePatchTest(name);

    if (result2)
    {
        int id = std::stoi(vs.at(2));
        int subId = std::stoi(vs.at(3));

        Common::Inventory()->RemoveItem(id, subId);

        // リストを作り直す
        CreateList();
    }
    else
    {
        PopUp2::Get()->SetText("パッチテストの予約は３件まで");
    }
}

NSStarmanLib::PatchTestManager* PatchTestManager2::GetPatchLib()
{
    return NSStarmanLib::PatchTestManager::Get();
}
