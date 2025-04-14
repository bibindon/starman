#include "CommandManager.h"
#include <d3d9.h>
#include <d3dx9math.h>
#include "SoundEffect.h"
#include "SharedObj.h"
#include "KeyBoard.h"
#include "Mouse.h"
#include "GamePad.h"
#include "VoyageManager.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/Rynen.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/WeaponManager.h"

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

void CommandManager::Init(const eType type)
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

    m_eType = type;
    if (type == eType::Title)
    {
        BuildOpeningCommand();
    }
    else if (type == eType::Main)
    {
        BuildCommand();
    }
}

std::string CommandManager::Operate()
{
    if (m_eType == eType::Main)
    {
        // 20秒経過したら脱出コマンドを追加
        ++m_counter;

        // 0.3秒おきに更新
        // 20で割った余りが1であるとき、とすることで、
        // 0.33秒おきに更新されるとともに、初めてこの関数が呼ばれたときにも更新呼ばれる。
        if (m_counter % 20 == 1)
        {
            BuildCommand();
        }
    }

    std::string result;
    std::string work_str;

    if (SharedObj::KeyBoard()->IsDownFirstFrame(DIK_LEFT))
    {
        m_commandLib->Previous();
    }

    if (SharedObj::KeyBoard()->IsDownFirstFrame(DIK_RIGHT))
    {
        m_commandLib->Next();
    }

    if (SharedObj::KeyBoard()->IsDownFirstFrame(DIK_RETURN))
    {
        result = m_commandLib->Into();
    }

    if (SharedObj::KeyBoard()->IsDownFirstFrame(DIK_ESCAPE))
    {
        result = "EXIT";
    }

    if (Mouse::IsDownLeft())
    {
        POINT p;
        GetCursorPos(&p);
        ScreenToClient(FindWindow("ホシマン", nullptr), &p);
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

    // resultが空ではないならコマンドメニューを一度閉じる、ということ
    if (!result.empty())
    {
        m_counter = 0;
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

void CommandManager::Upsert(const std::string name, const bool visible)
{
    m_commandLib->UpsertCommand(name, visible);
}

void CommandManager::SetPreviousState(const eBattleState previousState)
{
    m_previousState = previousState;
}

eBattleState CommandManager::GetPreviousState() const
{
    return m_previousState;
}

void CommandManager::BuildCommand()
{
    //---------------------------------------------------
    // 伐採・・・近くに木があるときに表示される。イカダモードの時は表示されない。
    // 採取・・・近くに草があるときに表示される。イカダモードの時は表示されない。
    // 横になる・・・常に表示される
    // 座る・・・常に表示される
    // 脱出・・・20秒コマンドが表示されたら
    // 帆を張る・・・イカダモードの時
    // 帆を畳む・・・イカダモードの時。「帆を張る」と「帆を畳む」はどちらかが表示される。
    // ３時間漕ぐ・・・イカダモードの時、川ではなく海にいるとき
    // 立ち上がる・・・イカダモードの時。イカダモードが解除される。
    // イカダに乗る・・・イカダが近くにある時
    // イカダの袋を見る・・・イカダが近くにある時
    //---------------------------------------------------

    m_commandLib->RemoveAll();
    auto voyage = VoyageManager::Get();
    auto raftMode = voyage->GetRaftMode();
    auto ppos = SharedObj::GetPlayer()->GetPos();

    // 伐採・・・近くに木があるときに表示される。イカダモードの時は表示されない。
    if (!raftMode)
    {
        if (SharedObj::GetMap()->NearTree(ppos))
        {
            m_commandLib->UpsertCommand("伐採", true);
        }
        else
        {
            m_commandLib->UpsertCommand("伐採", false);
        }
    }

    // 採取・・・近くに草があるときに表示される。イカダモードの時は表示されない。
    if (!raftMode)
    {
        if (SharedObj::GetMap()->NearPlant(ppos))
        {
            m_commandLib->UpsertCommand("採取", true);
        }
        else
        {
            m_commandLib->UpsertCommand("採取", false);
        }
    }

    // 横になる・・・常に表示される
    m_commandLib->UpsertCommand("横になる", true);

    // 座る・・・常に表示される
    m_commandLib->UpsertCommand("座る", true);

    // 脱出・・・20秒コマンドが表示されたら
    if (m_counter > (60 * 20))
    {
        m_commandLib->UpsertCommand("脱出", true);
    }

    // 帆を張る・・・イカダモードの時
    if (raftMode)
    {
        if (!voyage->GetSail())
        {
            m_commandLib->UpsertCommand("帆を張る", true);
        }
    }

    // 帆を畳む・・・イカダモードの時。「帆を張る」と「帆を畳む」はどちらかが表示される。
    if (raftMode)
    {
        if (voyage->GetSail())
        {
            m_commandLib->UpsertCommand("帆を畳む", true);
        }
    }

    // ３時間漕ぐ・・・イカダモードの時、川ではなく海にいるとき
    if (raftMode)
    {
        if (voyage->GetPosType() == NSStarmanLib::Raft::ePosType::Sea)
        {
            m_commandLib->UpsertCommand("３時間漕ぐ", true);
        }
        else
        {
            m_commandLib->UpsertCommand("３時間漕ぐ", false);
        }
    }

    // 立ち上がる・・・イカダモードの時。イカダモードが解除される。
    if (raftMode)
    {
        m_commandLib->UpsertCommand("立ち上がる", true);
    }

    // イカダに乗る・・・イカダが近くにある時
    //
    // イカダを所有していないとき、非表示
    // イカダを所有していて、イカダが近くにあるとき、活性で表示する
    // イカダを所有していて、イカダが近くにないとき、非活性で表示する
    if (!raftMode)
    {
        if (voyage->GetRaftCount() == 0)
        {
            m_commandLib->RemoveCommand("イカダに乗る");
        }
        else
        {
            if (voyage->CheckNearRaft(ppos))
            {
                m_commandLib->UpsertCommand("イカダに乗る", true);
            }
            else
            {
                m_commandLib->UpsertCommand("イカダに乗る", false);
            }
        }
    }

    // イカダの袋を見る・・・イカダが近くにある時
    //
    // イカダを所有していないとき、非表示
    // イカダを所有していて、イカダが近くにあるとき、活性で表示する
    // イカダを所有していて、イカダが近くにないとき、非活性で表示する
    if (!raftMode)
    {
        if (voyage->GetRaftCount() == 0)
        {
            m_commandLib->RemoveCommand("イカダの袋を見る");
        }
        else
        {
            if (voyage->CheckNearRaft(ppos))
            {
                m_commandLib->UpsertCommand("イカダの袋を見る", true);
            }
            else
            {
                m_commandLib->UpsertCommand("イカダの袋を見る", false);
            }
        }
    }

    //----------------------------------------------
    // 松明を作る
    //
    // 魔法を使えるようになっているとき表示される
    // 木の棒を装備していると活性状態になる。
    //----------------------------------------------
    {
        auto enableMagic = NSStarmanLib::Rynen::GetObj()->GetContracted();
        if (enableMagic)
        {
            if (Common::Status()->GetEquipWeapon().GetItemDef().GetName() == "木の棒")
            {
                m_commandLib->UpsertCommand("松明を作る", true);
            }
            else
            {
                m_commandLib->UpsertCommand("松明を作る", false);
            }
        }
    }

    //----------------------------------------------
    // 松明に火をつける
    // 松明を装備していて、松明に火がついていないときに表示される
    //
    // 松明の火を消す
    // 松明を装備していて、松明に火がついているときに表示される
    //----------------------------------------------
    {
        auto weapon = Common::Status()->GetEquipWeapon();

        // 何かしらは装備している。
        if (weapon.GetId() != 0 && weapon.GetId() != -1)
        {
            if (Common::Status()->GetEquipWeapon().GetItemDef().GetName() == "松明")
            {
                auto lit = NSStarmanLib::WeaponManager::GetObj()->IsTorchLit();
                if (!lit)
                {
                    m_commandLib->UpsertCommand("松明に火をつける", true);
                }
                else
                {
                    m_commandLib->UpsertCommand("松明の火を消す", true);
                }
            }
        }
    }
}

void CommandManager::BuildOpeningCommand()
{
    //---------------------------------------------------
    // Start
    // Continue
    // Exit
    //---------------------------------------------------

    BOOL saveExist = PathFileExists("res\\script\\save");
    bool enable = false;
    if (saveExist == TRUE)
    {
        enable = true;
    }
    else
    {
        enable = false;
    }

    m_commandLib->RemoveAll();
    m_commandLib->UpsertCommand("Start", true);
    m_commandLib->UpsertCommand("Continue", enable);
    m_commandLib->UpsertCommand("Exit", true);
}

