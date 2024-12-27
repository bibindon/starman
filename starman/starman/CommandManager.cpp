#include "CommandManager.h"
#include <d3d9.h>
#include <d3dx9math.h>
#include "SoundEffect.h"
#include "SharedObj.h"
#include "KeyBoard.h"
#include "Mouse.h"
#include "GamePad.h"

namespace NSCommand
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

    virtual void DrawText_(const std::string& msg,
                           const int x,
                           const int y,
                           const int transparent)
    {
        RECT rect = { x, y, x + 100, y + 100 };
        m_pFont->DrawText(NULL, msg.c_str(), -1, &rect, DT_CENTER | DT_VCENTER | DT_NOCLIP,
                          D3DCOLOR_ARGB(transparent, 255, 255, 255));
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

void CommandManager::Init(std::vector<std::string> commands,
                          std::vector<bool> visibles)
{
    if (m_commandLib != nullptr)
    {
        delete m_commandLib;
    }
    m_commandLib = NEW NSCommand::CommandLib();

    NSCommand::Sprite* sprCursor = NEW NSCommand::Sprite(SharedObj::GetD3DDevice());
    sprCursor->Load("res\\image\\command_cursor.png");

    NSCommand::IFont* pFont = NEW NSCommand::Font(SharedObj::GetD3DDevice());
    pFont->Init();

    NSCommand::ISoundEffect* pSE = NEW NSCommand::SoundEffect();
    pSE->Init();

    m_commandLib->Init(pFont, pSE, sprCursor);

    for (int i = 0; i < (int)commands.size(); ++i)
    {
        m_commandLib->UpsertCommand(commands.at(i), visibles.at(i));
    }
}

std::string CommandManager::Operate()
{
    std::string result;
    std::string work_str;

    if (KeyBoard::IsDownFirstFrame(DIK_LEFT))
    {
        m_commandLib->Previous();
    }

    if (KeyBoard::IsDownFirstFrame(DIK_RIGHT))
    {
        m_commandLib->Next();
    }

    if (KeyBoard::IsDownFirstFrame(DIK_RETURN))
    {
        result = m_commandLib->Into();
    }

    if (KeyBoard::IsDownFirstFrame(DIK_ESCAPE))
    {
        result = "EXIT";
    }

    if (Mouse::IsDownLeft())
    {
        POINT p;
        GetCursorPos(&p);
        ScreenToClient(FindWindowA("ホシマン", nullptr), &p);
        result = m_commandLib->Click(p.x, p.y);
    }
    else
    {
        POINT p;
        GetCursorPos(&p);
        ScreenToClient(FindWindow("ホシマン", nullptr), &p);
        m_commandLib->MouseMove(p.x, p.y);
    }

    if (GamePad::IsDown(eGamePadButtonType::LEFT))
    {
        m_commandLib->Previous();
    }

    if (GamePad::IsDown(eGamePadButtonType::RIGHT))
    {
        m_commandLib->Next();
    }

    if (GamePad::IsDown(eGamePadButtonType::A))
    {
        result = m_commandLib->Into();
    }

    if (GamePad::IsDown(eGamePadButtonType::B))
    {
        result = "EXIT";
    }

    return result;
}

void CommandManager::Draw()
{
    m_commandLib->Draw();
}

void CommandManager::Finalize()
{
    m_commandLib->Finalize();
    SAFE_DELETE(m_commandLib);
}
