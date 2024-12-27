#include "HudManager.h"
#include <d3d9.h>
#include <d3dx9math.h>
#include "SharedObj.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/StatusManager.h"

namespace NSHud
{
class Sprite : public ISprite
{
public:

    Sprite(LPDIRECT3DDEVICE9 dev)
        : m_pD3DDevice(dev)
    {
    }

    void DrawImage(const int percent, const int x, const int y, const int transparency) override
    {
        D3DXVECTOR3 pos { (float)x, (float)y, 0.f };
        m_D3DSprite->Begin(D3DXSPRITE_ALPHABLEND);
        RECT rect = {
            0,
            0,
            static_cast<LONG>(m_width * percent / 100),
            static_cast<LONG>(m_height / 2) };
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
            20,
            0,
            FW_THIN,
            1,
            false,
            SHIFTJIS_CHARSET,
            OUT_TT_ONLY_PRECIS,
            ANTIALIASED_QUALITY,
            FF_DONTCARE,
            "游明朝",
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

}

void HudManager::Init()
{
    // HUD
    {
        if (m_hud != nullptr)
        {
            delete m_hud;
        }
        m_hud = NEW NSHud::hud();
        NSHud::IFont* pFont = NEW NSHud::Font(SharedObj::GetD3DDevice());
        pFont->Init();

        NSHud::ISprite* sprBack = NEW NSHud::Sprite(SharedObj::GetD3DDevice());
        sprBack->Load("res\\image\\status_back.png");

        NSHud::ISprite* sprMiddle = NEW NSHud::Sprite(SharedObj::GetD3DDevice());
        sprMiddle->Load("res\\image\\status_middle.png");

        NSHud::ISprite* sprFront = NEW NSHud::Sprite(SharedObj::GetD3DDevice());
        sprFront->Load("res\\image\\status_front.png");

        m_hud->Init(pFont, sprBack, sprMiddle, sprFront);

        m_hud->UpsertStatus("身体のスタミナ", 100, 100, true);
        m_hud->UpsertStatus("脳のスタミナ", 100, 100, true);
    }
}

void HudManager::Finalize()
{
    m_hud->Finalize();
    SAFE_DELETE(m_hud);
}

void HudManager::Update()
{
    NSStarmanLib::StatusManager* statusManager = NSStarmanLib::StatusManager::GetObj();
    float work1 = 0.f;
    float work2 = 0.f;
    float work3 = 0.f;

    work1 = statusManager->GetBodyStaminaMax();
    work2 = statusManager->GetBodyStaminaMaxSub();
    work3 = statusManager->GetBodyStaminaCurrent();

    m_hud->UpsertStatus("身体のスタミナ",
                        (int)(work3 * 100 / work1),
                        (int)(work2 * 100 / work1),
                        true);

    work1 = statusManager->GetBrainStaminaMax();
    work2 = statusManager->GetBrainStaminaMaxSub();
    work3 = statusManager->GetBrainStaminaCurrent();

    m_hud->UpsertStatus("脳のスタミナ",
                        (int)(work3 * 100 / work1),
                        (int)(work2 * 100 / work1),
                        true);

    work1 = statusManager->GetWaterMax();
    work2 = statusManager->GetWaterCurrent();
    work3 = work2 * 100 / work1;

    // 水分
    // 98%以下なら表示する
    if ((int)work3 <= 98)
    {
        m_hud->UpsertStatus("水分", (int)work3, (int)work3, true);
    }
    else
    {
        m_hud->RemoveStatus("水分");
    }

    // 肉体の修復度
    // 90%以下なら表示する
    work1 = statusManager->GetMuscleMax();
    work2 = statusManager->GetMuscleCurrent();
    work3 = work2 * 100 / work1;

    if ((int)work3 <= 90)
    {
        m_hud->UpsertStatus("肉体の修復度",
                            (int)(work3 * 100 / work1),
                            (int)(work2 * 100 / work1),
                            true);
    }
    else
    {
        m_hud->RemoveStatus("肉体の修復度");
    }

    // 10％以下なら表示する
    work1 = statusManager->GetCarboCurrent() * 100 / statusManager->GetCarboMax();
    if (work1 <= 10.f)
    {
        m_hud->UpsertStatus("糖質", (int)work1, (int)work1, true);
    }
    else
    {
        m_hud->RemoveStatus("糖質");
    }

    work1 = statusManager->GetProteinCurrent() * 100 / statusManager->GetProteinMax();
    if (work1 <= 10.f)
    {
        m_hud->UpsertStatus("タンパク質", (int)work1, (int)work1, true);
    }
    else
    {
        m_hud->RemoveStatus("タンパク質");
    }

    work1 = statusManager->GetLipidCurrent() * 100 / statusManager->GetLipidMax();
    if (work1 <= 10.f)
    {
        m_hud->UpsertStatus("脂質", (int)work1, (int)work1, true);
    }
    else
    {
        m_hud->RemoveStatus("脂質");
    }

    work1 = statusManager->GetVitaminCurrent() * 100 / statusManager->GetVitaminMax();
    if (work1 <= 10.f)
    {
        m_hud->UpsertStatus("ビタミン", (int)work1, (int)work1, true);
    }
    else
    {
        m_hud->RemoveStatus("ビタミン");
    }

    work1 = statusManager->GetMineralCurrent() * 100 / statusManager->GetMineralMax();
    if (work1 <= 10.f)
    {
        m_hud->UpsertStatus("ミネラル", (int)work1, (int)work1, true);
    }
    else
    {
        m_hud->RemoveStatus("ミネラル");
    }

    if (statusManager->GetFractureArm())
    {
        m_hud->UpsertStatus("腕骨折", 0, 0, false);
    }
    else
    {
        m_hud->RemoveStatus("腕骨折");
    }

    if (statusManager->GetFractureLeg())
    {
        m_hud->UpsertStatus("足骨折", 0, 0, false);
    }
    else
    {
        m_hud->RemoveStatus("足骨折");
    }

    if (statusManager->GetHeadache())
    {
        m_hud->UpsertStatus("頭痛", 0, 0, false);
    }
    else
    {
        m_hud->RemoveStatus("頭痛");
    }

    if (statusManager->GetCold())
    {
        m_hud->UpsertStatus("風邪", 0, 0, false);
    }
    else
    {
        m_hud->RemoveStatus("風邪");
    }

    if (statusManager->GetStomachache())
    {
        m_hud->UpsertStatus("腹痛", 0, 0, false);
    }
    else
    {
        m_hud->RemoveStatus("腹痛");
    }

    if (statusManager->GetSleep())
    {
        m_hud->UpsertStatus("睡眠", 0, 0, false);
    }
    else
    {
        m_hud->RemoveStatus("睡眠");
    }

    if (statusManager->GetDehydration())
    {
        m_hud->UpsertStatus("脱水症状", 0, 0, false);
    }
    else
    {
        m_hud->RemoveStatus("脱水症状");
    }

    if (statusManager->GetLackOfSleep())
    {
        m_hud->UpsertStatus("睡眠不足", 0, 0, false);
    }
    else
    {
        m_hud->RemoveStatus("睡眠不足");
    }

    if (statusManager->GetDead())
    {
        m_hud->UpsertStatus("死亡", 0, 0, false);
    }
    else
    {
        m_hud->RemoveStatus("死亡");
    }
}

void HudManager::Draw()
{
    m_hud->Draw();
}
