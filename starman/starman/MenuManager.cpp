#include "MenuManager.h"
#include "SoundEffect.h"
#include <d3d9.h>
#include <d3dx9.h>
#include "SharedObj.h"
#include "KeyBoard.h"
#include "Mouse.h"

namespace NSMenulib
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
        m_pD3DTexture->Release();
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
    }

private:

    LPDIRECT3DDEVICE9 m_pD3DDevice = NULL;
    LPD3DXFONT m_pFont = NULL;
};


class SoundEffect : public ISoundEffect
{
    virtual void PlayMove() override
    {
        ::SoundEffect::get_ton()->play("res\\sound\\cursor_move.wav");
    }
    virtual void PlayClick() override
    {
        ::SoundEffect::get_ton()->play("res\\sound\\cursor_confirm.wav");
    }
    virtual void PlayBack() override
    {
        ::SoundEffect::get_ton()->play("res\\sound\\cursor_cancel.wav");
    }
    virtual void Init() override
    {
        ::SoundEffect::get_ton()->load("res\\sound\\cursor_move.wav");
        ::SoundEffect::get_ton()->load("res\\sound\\cursor_confirm.wav");
        ::SoundEffect::get_ton()->load("res\\sound\\cursor_cancel.wav");
    }
};
}

void MenuManager::InitMenu()
{
    using namespace NSMenulib;
    NSMenulib::Sprite* sprCursor = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
    sprCursor->Load("res\\image\\menu_cursor.png");

    NSMenulib::Sprite* sprBackground = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
    sprBackground->Load("res\\image\\background.png");

    NSMenulib::Sprite* sprPanel = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
    sprPanel->Load("res\\image\\panel.png");

    NSMenulib::Sprite* sprPanelLeft = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
    sprPanelLeft->Load("res\\image\\panelLeft.png");

    NSMenulib::IFont* pFont = new NSMenulib::Font(SharedObj::GetD3DDevice());
    pFont->Init();

    NSMenulib::ISoundEffect* pSE = new NSMenulib::SoundEffect();
    pSE->Init();

    m_menu.Init("", pFont, pSE, sprCursor, sprBackground, sprPanel, sprPanelLeft);
    std::vector<ItemInfo> itemInfoList;
    {
        ItemInfo itemInfo;
        itemInfo.SetName("テストアイテム１");
        itemInfo.SetNum(10);
        NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
        sprItem->Load("res\\image\\item1.png");
        itemInfo.SetSprite(sprItem);
        itemInfo.SetDetail("テストアイテムテキスト\nテストテキストテキスト\nテストテキストテキスト\nテストテキストテストテキスト"); // TODO
        itemInfoList.push_back(itemInfo);
    }
    {
        ItemInfo itemInfo;
        itemInfo.SetName("テストアイテム２");
        itemInfo.SetNum(20);
        NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
        sprItem->Load("res\\image\\item2.png");
        itemInfo.SetSprite(sprItem);
        itemInfo.SetDetail("ＡＡＡテストアイテムテキスト\nテストテキストテストテキスト"); // TODO
        itemInfoList.push_back(itemInfo);
    }
    {
        ItemInfo itemInfo;
        itemInfo.SetName("テストアイテム３");
        itemInfo.SetNum(30);
        NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
        sprItem->Load("res\\image\\item3.png");
        itemInfo.SetSprite(sprItem);
        itemInfo.SetDetail("ＢＢＢテストアイテムテキスト\nテストテキストテストテキスト"); // TODO
        itemInfoList.push_back(itemInfo);
    }
    {
        ItemInfo itemInfo;
        itemInfo.SetName("テストアイテム１");
        itemInfo.SetNum(10);
        NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
        sprItem->Load("res\\image\\item1.png");
        itemInfo.SetSprite(sprItem);
        itemInfo.SetDetail("テストアイテムテキスト\nテストテキストテキスト\nテストテキストテキスト\nテストテキストテストテキスト"); // TODO
        itemInfoList.push_back(itemInfo);
    }
    {
        ItemInfo itemInfo;
        itemInfo.SetName("テストアイテム２");
        itemInfo.SetNum(20);
        NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
        sprItem->Load("res\\image\\item2.png");
        itemInfo.SetSprite(sprItem);
        itemInfo.SetDetail("ＡＡＡテストアイテムテキスト\nテストテキストテストテキスト"); // TODO
        itemInfoList.push_back(itemInfo);
    }
    {
        ItemInfo itemInfo;
        itemInfo.SetName("テストアイテム３");
        itemInfo.SetNum(30);
        NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
        sprItem->Load("res\\image\\item3.png");
        itemInfo.SetSprite(sprItem);
        itemInfo.SetDetail("ＢＢＢテストアイテムテキスト\nテストテキストテストテキスト"); // TODO
        itemInfoList.push_back(itemInfo);
    }
    {
        ItemInfo itemInfo;
        itemInfo.SetName("テストアイテム１");
        itemInfo.SetNum(10);
        NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
        sprItem->Load("res\\image\\item1.png");
        itemInfo.SetSprite(sprItem);
        itemInfo.SetDetail("テストアイテムテキスト\nテストテキストテキスト\nテストテキストテキスト\nテストテキストテストテキスト"); // TODO
        itemInfoList.push_back(itemInfo);
    }
    {
        ItemInfo itemInfo;
        itemInfo.SetName("テストアイテム２");
        itemInfo.SetNum(20);
        NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
        sprItem->Load("res\\image\\item2.png");
        itemInfo.SetSprite(sprItem);
        itemInfo.SetDetail("ＡＡＡテストアイテムテキスト\nテストテキストテストテキスト"); // TODO
        itemInfoList.push_back(itemInfo);
    }
    {
        ItemInfo itemInfo;
        itemInfo.SetName("テストアイテム３");
        itemInfo.SetNum(30);
        NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
        sprItem->Load("res\\image\\item3.png");
        itemInfo.SetSprite(sprItem);
        itemInfo.SetDetail("ＢＢＢテストアイテムテキスト\nテストテキストテストテキスト"); // TODO
        itemInfoList.push_back(itemInfo);
    }
    {
        ItemInfo itemInfo;
        itemInfo.SetName("テストアイテム１");
        itemInfo.SetNum(10);
        NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
        sprItem->Load("res\\image\\item1.png");
        itemInfo.SetSprite(sprItem);
        itemInfo.SetDetail("テストアイテムテキスト\nテストテキストテキスト\nテストテキストテキスト\nテストテキストテストテキスト"); // TODO
        itemInfoList.push_back(itemInfo);
    }
    {
        ItemInfo itemInfo;
        itemInfo.SetName("テストアイテム２");
        itemInfo.SetNum(20);
        NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
        sprItem->Load("res\\image\\item2.png");
        itemInfo.SetSprite(sprItem);
        itemInfo.SetDetail("ＡＡＡテストアイテムテキスト\nテストテキストテストテキスト"); // TODO
        itemInfoList.push_back(itemInfo);
    }
    {
        ItemInfo itemInfo;
        itemInfo.SetName("テストアイテム３");
        itemInfo.SetNum(30);
        NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
        sprItem->Load("res\\image\\item3.png");
        itemInfo.SetSprite(sprItem);
        itemInfo.SetDetail("ＢＢＢテストアイテムテキスト\nテストテキストテストテキスト"); // TODO
        itemInfoList.push_back(itemInfo);
    }
    {
        ItemInfo itemInfo;
        itemInfo.SetName("テストアイテム１");
        itemInfo.SetNum(10);
        NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
        sprItem->Load("res\\image\\item1.png");
        itemInfo.SetSprite(sprItem);
        itemInfo.SetDetail("テストアイテムテキスト\nテストテキストテキスト\nテストテキストテキスト\nテストテキストテストテキスト"); // TODO
        itemInfoList.push_back(itemInfo);
    }
    {
        ItemInfo itemInfo;
        itemInfo.SetName("テストアイテム２");
        itemInfo.SetNum(20);
        NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
        sprItem->Load("res\\image\\item2.png");
        itemInfo.SetSprite(sprItem);
        itemInfo.SetDetail("ＡＡＡテストアイテムテキスト\nテストテキストテストテキスト"); // TODO
        itemInfoList.push_back(itemInfo);
    }
    {
        ItemInfo itemInfo;
        itemInfo.SetName("テストアイテム３");
        itemInfo.SetNum(30);
        NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
        sprItem->Load("res\\image\\item3.png");
        itemInfo.SetSprite(sprItem);
        itemInfo.SetDetail("ＢＢＢテストアイテムテキスト\nテストテキストテストテキスト"); // TODO
        itemInfoList.push_back(itemInfo);
    }
    {
        ItemInfo itemInfo;
        itemInfo.SetName("テストアイテム１");
        itemInfo.SetNum(10);
        NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
        sprItem->Load("res\\image\\item1.png");
        itemInfo.SetSprite(sprItem);
        itemInfo.SetDetail("テストアイテムテキスト\nテストテキストテキスト\nテストテキストテキスト\nテストテキストテストテキスト"); // TODO
        itemInfoList.push_back(itemInfo);
    }
    {
        ItemInfo itemInfo;
        itemInfo.SetName("テストアイテム２");
        itemInfo.SetNum(20);
        NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
        sprItem->Load("res\\image\\item2.png");
        itemInfo.SetSprite(sprItem);
        itemInfo.SetDetail("ＡＡＡテストアイテムテキスト\nテストテキストテストテキスト"); // TODO
        itemInfoList.push_back(itemInfo);
    }
    {
        ItemInfo itemInfo;
        itemInfo.SetName("テストアイテム３");
        itemInfo.SetNum(30);
        NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
        sprItem->Load("res\\image\\item3.png");
        itemInfo.SetSprite(sprItem);
        itemInfo.SetDetail("ＢＢＢテストアイテムテキスト\nテストテキストテストテキスト"); // TODO
        itemInfoList.push_back(itemInfo);
    }
    {
        ItemInfo itemInfo;
        itemInfo.SetName("テストアイテム１");
        itemInfo.SetNum(10);
        NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
        sprItem->Load("res\\image\\item1.png");
        itemInfo.SetSprite(sprItem);
        itemInfo.SetDetail("テストアイテムテキスト\nテストテキストテキスト\nテストテキストテキスト\nテストテキストテストテキスト"); // TODO
        itemInfoList.push_back(itemInfo);
    }
    {
        ItemInfo itemInfo;
        itemInfo.SetName("テストアイテム２");
        itemInfo.SetNum(20);
        NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
        sprItem->Load("res\\image\\item2.png");
        itemInfo.SetSprite(sprItem);
        itemInfo.SetDetail("ＡＡＡテストアイテムテキスト\nテストテキストテストテキスト"); // TODO
        itemInfoList.push_back(itemInfo);
    }
    {
        ItemInfo itemInfo;
        itemInfo.SetName("テストアイテム３");
        itemInfo.SetNum(30);
        NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
        sprItem->Load("res\\image\\item3.png");
        itemInfo.SetSprite(sprItem);
        itemInfo.SetDetail("ＢＢＢテストアイテムテキスト\nテストテキストテストテキスト"); // TODO
        itemInfoList.push_back(itemInfo);
    }
    {
        ItemInfo itemInfo;
        itemInfo.SetName("テストアイテム１");
        itemInfo.SetNum(10);
        NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
        sprItem->Load("res\\image\\item1.png");
        itemInfo.SetSprite(sprItem);
        itemInfo.SetDetail("テストアイテムテキスト\nテストテキストテキスト\nテストテキストテキスト\nテストテキストテストテキスト"); // TODO
        itemInfoList.push_back(itemInfo);
    }
    {
        ItemInfo itemInfo;
        itemInfo.SetName("テストアイテム２");
        itemInfo.SetNum(20);
        NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
        sprItem->Load("res\\image\\item2.png");
        itemInfo.SetSprite(sprItem);
        itemInfo.SetDetail("ＡＡＡテストアイテムテキスト\nテストテキストテストテキスト"); // TODO
        itemInfoList.push_back(itemInfo);
    }
    {
        ItemInfo itemInfo;
        itemInfo.SetName("テストアイテム３");
        itemInfo.SetNum(30);
        NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
        sprItem->Load("res\\image\\item3.png");
        itemInfo.SetSprite(sprItem);
        itemInfo.SetDetail("ＢＢＢテストアイテムテキスト\nテストテキストテストテキスト"); // TODO
        itemInfoList.push_back(itemInfo);
    }
    m_menu.SetItem(itemInfoList);
    std::vector<HumanInfo> humanInfoList;
    {
        HumanInfo humanInfo;
        humanInfo.SetName("テスト人物１");
        NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
        sprItem->Load("res\\image\\human1.png");
        humanInfo.SetSprite(sprItem);
        humanInfo.SetDetail("テスト人物テキスト\nテスト人物テキスト\nテスト人物テキスト\nテスト人物テキスト\nテスト人物テキスト");
        humanInfoList.push_back(humanInfo);
    }
    {
        HumanInfo humanInfo;
        humanInfo.SetName("テスト人物２");
        NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
        sprItem->Load("res\\image\\human2.png");
        humanInfo.SetSprite(sprItem);
        humanInfo.SetDetail("テスト人物２\n　\nテスト人物テキスト\nテスト人物テキスト\nテスト人物テキスト\nテスト人物テキスト\nテスト人物テキスト");
        humanInfoList.push_back(humanInfo);
    }
    {
        HumanInfo humanInfo;
        humanInfo.SetName("テスト人物３");
        NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
        sprItem->Load("res\\image\\human3.png");
        humanInfo.SetSprite(sprItem);
        humanInfo.SetDetail("テスト人物３\n　\nテスト人物テキスト\nテスト人物テキスト\nテスト人物テキスト\nテスト人物テキスト\nテスト人物テキスト");
        humanInfoList.push_back(humanInfo);
    }
    m_menu.SetHuman(humanInfoList);
    {
        std::vector<WeaponInfo> infoList;
        {
            WeaponInfo info;
            info.SetName("テスト人物１");
            NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
            sprItem->Load("res\\image\\test.png");
            info.SetSprite(sprItem);
            info.SetDetail("テスト人物テキスト\nテスト人物テキスト\nテスト人物テキスト\nテスト人物テキスト\nテスト人物テキスト");
            infoList.push_back(info);
        }
        {
            WeaponInfo info;
            info.SetName("テスト人物２");
            NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
            sprItem->Load("res\\image\\test.png");
            info.SetSprite(sprItem);
            info.SetDetail("テスト人物２\n　\nテスト人物テキスト\nテスト人物テキスト\nテスト人物テキスト\nテスト人物テキスト\nテスト人物テキスト");
            infoList.push_back(info);
        }
        {
            WeaponInfo info;
            info.SetName("テスト人物３");
            NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
            sprItem->Load("res\\image\\test.png");
            info.SetSprite(sprItem);
            info.SetDetail("テスト人物３\n　\nテスト人物テキスト\nテスト人物テキスト\nテスト人物テキスト\nテスト人物テキスト\nテスト人物テキスト");
            infoList.push_back(info);
        }
        m_menu.SetWeapon(infoList);
    }
    {
        std::vector<TaskInfo> infoList;
        {
            TaskInfo info;
            info.SetName("サンプルテキスト１");
            NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
            sprItem->Load("res\\image\\test.png");
            info.SetSprite(sprItem);
            info.SetDetail("サンプルテキスト\n\nサンプルテキスト\nサンプルテキスト\nサンプルテキスト");
            infoList.push_back(info);
        }
        {
            TaskInfo info;
            info.SetName("サンプルテキスト２");
            NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
            sprItem->Load("res\\image\\test.png");
            info.SetSprite(sprItem);
            info.SetDetail("サンプルテキスト\n\nサンプルテキスト\nサンプルテキスト\nサンプルテキスト");
            infoList.push_back(info);
        }
        {
            TaskInfo info;
            info.SetName("サンプルテキスト３");
            NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
            sprItem->Load("res\\image\\test.png");
            info.SetSprite(sprItem);
            info.SetDetail("サンプルテキスト\n\nサンプルテキスト\nサンプルテキスト\nサンプルテキスト");
            infoList.push_back(info);
        }
        m_menu.SetTask(infoList);
    }
    {
        std::vector<EnemyInfo> infoList;
        {
            EnemyInfo info;
            info.SetName("サンプルテキスト１");
            NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
            sprItem->Load("res\\image\\test.png");
            info.SetSprite(sprItem);
            info.SetDetail("サンプルテキスト\n\nサンプルテキスト\nサンプルテキスト\nサンプルテキスト");
            infoList.push_back(info);
        }
        {
            EnemyInfo info;
            info.SetName("サンプルテキスト２");
            NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
            sprItem->Load("res\\image\\test.png");
            info.SetSprite(sprItem);
            info.SetDetail("サンプルテキスト\n\nサンプルテキスト\nサンプルテキスト\nサンプルテキスト");
            infoList.push_back(info);
        }
        {
            EnemyInfo info;
            info.SetName("サンプルテキスト３");
            NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
            sprItem->Load("res\\image\\test.png");
            info.SetSprite(sprItem);
            info.SetDetail("サンプルテキスト\n\nサンプルテキスト\nサンプルテキスト\nサンプルテキスト");
            infoList.push_back(info);
        }
        {
            EnemyInfo info;
            info.SetName("サンプルテキスト１");
            NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
            sprItem->Load("res\\image\\test.png");
            info.SetSprite(sprItem);
            info.SetDetail("サンプルテキスト\n\nサンプルテキスト\nサンプルテキスト\nサンプルテキスト");
            infoList.push_back(info);
        }
        {
            EnemyInfo info;
            info.SetName("サンプルテキスト２");
            NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
            sprItem->Load("res\\image\\test.png");
            info.SetSprite(sprItem);
            info.SetDetail("サンプルテキスト\n\nサンプルテキスト\nサンプルテキスト\nサンプルテキスト");
            infoList.push_back(info);
        }
        {
            EnemyInfo info;
            info.SetName("サンプルテキスト３");
            NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
            sprItem->Load("res\\image\\test.png");
            info.SetSprite(sprItem);
            info.SetDetail("サンプルテキスト\n\nサンプルテキスト\nサンプルテキスト\nサンプルテキスト");
            infoList.push_back(info);
        }
        {
            EnemyInfo info;
            info.SetName("サンプルテキスト１");
            NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
            sprItem->Load("res\\image\\test.png");
            info.SetSprite(sprItem);
            info.SetDetail("サンプルテキスト\n\nサンプルテキスト\nサンプルテキスト\nサンプルテキスト");
            infoList.push_back(info);
        }
        {
            EnemyInfo info;
            info.SetName("サンプルテキスト２");
            NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
            sprItem->Load("res\\image\\test.png");
            info.SetSprite(sprItem);
            info.SetDetail("サンプルテキスト\n\nサンプルテキスト\nサンプルテキスト\nサンプルテキスト");
            infoList.push_back(info);
        }
        {
            EnemyInfo info;
            info.SetName("サンプルテキスト３");
            NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
            sprItem->Load("res\\image\\test.png");
            info.SetSprite(sprItem);
            info.SetDetail("サンプルテキスト\n\nサンプルテキスト\nサンプルテキスト\nサンプルテキスト");
            infoList.push_back(info);
        }
        m_menu.SetEnemy(infoList);
    }
    {
        std::vector<SkillInfo> infoList;
        {
            SkillInfo info;
            info.SetName("サンプルテキスト１");
            NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
            sprItem->Load("res\\image\\test.png");
            info.SetSprite(sprItem);
            info.SetDetail("サンプルテキスト\n\nサンプルテキスト\nサンプルテキスト\nサンプルテキスト");
            infoList.push_back(info);
        }
        {
            SkillInfo info;
            info.SetName("サンプルテキスト２");
            NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
            sprItem->Load("res\\image\\test.png");
            info.SetSprite(sprItem);
            info.SetDetail("サンプルテキスト\n\nサンプルテキスト\nサンプルテキスト\nサンプルテキスト");
            infoList.push_back(info);
        }
        {
            SkillInfo info;
            info.SetName("サンプルテキスト３");
            NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
            sprItem->Load("res\\image\\test.png");
            info.SetSprite(sprItem);
            info.SetDetail("サンプルテキスト\n\nサンプルテキスト\nサンプルテキスト\nサンプルテキスト");
            infoList.push_back(info);
        }
        m_menu.SetSkill(infoList);
    }
    {
        std::vector<MapInfo> infoList;
        {
            MapInfo info;
            info.SetName("サンプルテキスト１");
            NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
            sprItem->Load("res\\image\\test.png");
            info.SetSprite(sprItem);
            info.SetDetail("サンプルテキスト\n\nサンプルテキスト\nサンプルテキスト\nサンプルテキスト");
            infoList.push_back(info);
        }
        {
            MapInfo info;
            info.SetName("サンプルテキスト２");
            NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
            sprItem->Load("res\\image\\test.png");
            info.SetSprite(sprItem);
            info.SetDetail("サンプルテキスト\n\nサンプルテキスト\nサンプルテキスト\nサンプルテキスト");
            infoList.push_back(info);
        }
        {
            MapInfo info;
            info.SetName("サンプルテキスト３");
            NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
            sprItem->Load("res\\image\\test.png");
            info.SetSprite(sprItem);
            info.SetDetail("サンプルテキスト\n\nサンプルテキスト\nサンプルテキスト\nサンプルテキスト");
            infoList.push_back(info);
        }
        {
            MapInfo info;
            info.SetName("サンプルテキスト１");
            NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
            sprItem->Load("res\\image\\test.png");
            info.SetSprite(sprItem);
            info.SetDetail("サンプルテキスト\n\nサンプルテキスト\nサンプルテキスト\nサンプルテキスト");
            infoList.push_back(info);
        }
        {
            MapInfo info;
            info.SetName("サンプルテキスト２");
            NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
            sprItem->Load("res\\image\\test.png");
            info.SetSprite(sprItem);
            info.SetDetail("サンプルテキスト\n\nサンプルテキスト\nサンプルテキスト\nサンプルテキスト");
            infoList.push_back(info);
        }
        {
            MapInfo info;
            info.SetName("サンプルテキスト３");
            NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
            sprItem->Load("res\\image\\test.png");
            info.SetSprite(sprItem);
            info.SetDetail("サンプルテキスト\n\nサンプルテキスト\nサンプルテキスト\nサンプルテキスト");
            infoList.push_back(info);
        }
        m_menu.SetMap(infoList);
    }
}

void MenuManager::Draw()
{
    m_menu.Draw();
}

std::string MenuManager::OperateMenu()
{
    std::string result;
    if (KeyBoard::IsDown(DIK_UP))
    {
        m_menu.Up();
    }

    if (KeyBoard::IsDown(DIK_DOWN))
    {
        m_menu.Down();
    }

    if (KeyBoard::IsDown(DIK_LEFT))
    {
        m_menu.Left();
    }

    if (KeyBoard::IsDown(DIK_RIGHT))
    {
        m_menu.Right();
    }

    if (KeyBoard::IsDown(DIK_RETURN))
    {
        m_menu.Into();

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
        result = m_menu.Back();
    }

    if (Mouse::IsDownLeft())
    {
        POINT p;
        GetCursorPos(&p);
        ScreenToClient(FindWindowA("ホシマン", nullptr), &p);
        m_menu.Click(p.x, p.y);
    }

    if (Mouse::IsDownRight())
    {
        POINT p;
        GetCursorPos(&p);
        ScreenToClient(FindWindowA("ホシマン", nullptr), &p);
        m_menu.RightClick(p.x, p.y);
    }

    if (Mouse::GetZDelta() < 0)
    {
        m_menu.Next();
    }
    else if (Mouse::GetZDelta() > 0)
    {
        m_menu.Previous();
    }
    return result;
}

