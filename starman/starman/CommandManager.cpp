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
#include "../../StarmanLib/StarmanLib/StarmanLib/NpcStatusManager.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/Help.h"
#include <cassert>
#include "resource.h"
#include "SaveManager.h"

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

    void Load(const std::wstring& filepath) override
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

    void Init(const bool bEnglish)
    {
        HRESULT hr = S_OK;

        if (!bEnglish)
        {
            hr = D3DXCreateFont(m_pD3DDevice,
                                18,
                                0,
                                FW_NORMAL,
                                1,
                                false,
                                SHIFTJIS_CHARSET,
                                OUT_TT_ONLY_PRECIS,
                                CLEARTYPE_NATURAL_QUALITY,
                                FF_DONTCARE,
                                _T("BIZ UDMincho"),
                                &m_pFont);
        }
        else
        {
            hr = D3DXCreateFont(m_pD3DDevice,
                                20,
                                0,
                                FW_THIN,
                                1,
                                false,
                                DEFAULT_CHARSET,
                                OUT_TT_ONLY_PRECIS,
                                CLEARTYPE_NATURAL_QUALITY,
                                FF_DONTCARE,
                                _T("Courier New"),
                                &m_pFont);
        }

        assert(hr == S_OK);
    }

    virtual void DrawText_(const std::wstring& msg,
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
        ::SoundEffect::get_ton()->play(_T("res\\sound\\menu_cursor_move.wav"));
    }
    virtual void PlayClick() override
    {
        ::SoundEffect::get_ton()->play(_T("res\\sound\\menu_cursor_confirm.wav"));
    }
    virtual void PlayBack() override
    {
        ::SoundEffect::get_ton()->play(_T("res\\sound\\menu_cursor_cancel.wav"));
    }
    virtual void Init() override
    {
        ::SoundEffect::get_ton()->load(_T("res\\sound\\menu_cursor_move.wav"));
        ::SoundEffect::get_ton()->load(_T("res\\sound\\menu_cursor_confirm.wav"));
        ::SoundEffect::get_ton()->load(_T("res\\sound\\menu_cursor_cancel.wav"));
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
    sprCursor->Load(_T("res\\image\\command_cursor.png"));

    NSCommand::IFont* pFont = NEW NSCommand::Font(SharedObj::GetD3DDevice());

    NSCommand::ISoundEffect* pSE = NEW NSCommand::SoundEffect();

    m_commandLib->Init(pFont, pSE, sprCursor, SharedObj::IsEnglish());

    m_eType = type;
    if (type == eType::Title)
    {
        BuildOpeningCommand();
    }
    else if (type == eType::Title_Language)
    {
        BuildLangCommand();
    }
    else if (type == eType::Main)
    {
        BuildCommand();
    }
}

std::wstring CommandManager::Operate()
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

    std::wstring result;
    std::wstring work_str;

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
        result = _T("EXIT");
    }

    if (Mouse::IsDownLeft())
    {
        POINT p;
        GetCursorPos(&p);
        ScreenToClient(FindWindow(Common::LoadString_(IDS_STRING101).c_str(), nullptr), &p);
        result = m_commandLib->Click(p.x, p.y);
    }
    else
    {
        POINT p;
        GetCursorPos(&p);
        ScreenToClient(FindWindow(Common::LoadString_(IDS_STRING101).c_str(), nullptr), &p);
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
        result = _T("EXIT");
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

void CommandManager::Upsert(const std::wstring name, const bool visible)
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

    // 伐採・・・近くに細い木があるときに表示される。イカダモードの時は表示されない。
    if (!raftMode)
    {
        if (SharedObj::GetMap()->NearThinTree(ppos))
        {
            m_commandLib->UpsertCommand(Common::LoadString_(IDS_STRING178), true);
        }
        else
        {
            m_commandLib->UpsertCommand(Common::LoadString_(IDS_STRING178), false);
        }
    }

    // 採取・・・近くに草があるときに表示される。イカダモードの時は表示されない。
    // ココナツの木があるときも表示される。
    // ソテツの木があるときも表示される
    if (!raftMode)
    {
        if (SharedObj::GetMap()->NearPlant(ppos) ||
            SharedObj::GetMap()->NearCoconut(ppos) ||
            SharedObj::GetMap()->NearTree(ppos) ||
            SharedObj::GetMap()->NearSotetsu(ppos) ||
            SharedObj::GetMap()->NearDonguri(ppos) ||
            SharedObj::GetMap()->NearStone(ppos)
            )
        {
            m_commandLib->UpsertCommand(Common::LoadString_(IDS_STRING179), true);
        }
        else
        {
            m_commandLib->UpsertCommand(Common::LoadString_(IDS_STRING179), false);
        }
    }

    // 水中か否か
    bool bUnderwater = SharedObj::GetPlayer()->IsUnderWater();


    // 横になる・・・常に表示される
    m_commandLib->UpsertCommand(Common::LoadString_(IDS_STRING180), !bUnderwater);

    // 座る・・・常に表示される
    m_commandLib->UpsertCommand(Common::LoadString_(IDS_STRING181), !bUnderwater);

    // ３時間休憩する・・・常に表示される
    m_commandLib->UpsertCommand(_T("３時間休憩する"), !bUnderwater);

    // 瞑想・・・常に表示される
    m_commandLib->UpsertCommand(Common::LoadString_(IDS_STRING182), true);

    // 脱出・・・20秒コマンドが表示されたら
    if (m_counter > (60 * 20))
    {
        m_commandLib->UpsertCommand(Common::LoadString_(IDS_STRING183), true);
    }

    // 帆を張る・・・イカダモードの時
    if (raftMode)
    {
        if (!voyage->GetSail())
        {
            m_commandLib->UpsertCommand(Common::LoadString_(IDS_STRING184), true);
        }
    }

    // 帆を畳む・・・イカダモードの時。「帆を張る」と「帆を畳む」はどちらかが表示される。
    if (raftMode)
    {
        if (voyage->GetSail())
        {
            m_commandLib->UpsertCommand(Common::LoadString_(IDS_STRING185), true);
        }
    }

    // ３時間漕ぐ・・・イカダモードの時、川ではなく海にいるとき
    if (raftMode)
    {
        if (voyage->GetPosType() == NSStarmanLib::Raft::ePosType::Sea)
        {
            m_commandLib->UpsertCommand(Common::LoadString_(IDS_STRING186), true);
        }
        else
        {
            m_commandLib->UpsertCommand(Common::LoadString_(IDS_STRING186), false);
        }
    }

    // 立ち上がる・・・イカダモードの時。イカダモードが解除される。
    if (raftMode)
    {
        m_commandLib->UpsertCommand(Common::LoadString_(IDS_STRING187), true);
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
            m_commandLib->RemoveCommand(Common::LoadString_(IDS_STRING188));
        }
        else
        {
            if (voyage->CheckNearRaft(ppos))
            {
                m_commandLib->UpsertCommand(Common::LoadString_(IDS_STRING188), true);
            }
            else
            {
                m_commandLib->UpsertCommand(Common::LoadString_(IDS_STRING188), false);
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
            m_commandLib->RemoveCommand(Common::LoadString_(IDS_STRING189));
        }
        else
        {
            if (voyage->CheckNearRaft(ppos))
            {
                m_commandLib->UpsertCommand(Common::LoadString_(IDS_STRING189), true);
            }
            else
            {
                m_commandLib->UpsertCommand(Common::LoadString_(IDS_STRING189), false);
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
            auto weapon = Common::Status()->GetEquipWeapon();

            if (weapon.GetId() == -1 || weapon.GetId() == 0)
            {
                m_commandLib->UpsertCommand(Common::LoadString_(IDS_STRING190), false);
            }
            else
            {
                if (weapon.GetItemDef().GetName() == Common::LoadString_(IDS_STRING191))
                {
                    m_commandLib->UpsertCommand(Common::LoadString_(IDS_STRING190), true);
                }
                else
                {
                    m_commandLib->UpsertCommand(Common::LoadString_(IDS_STRING190), false);
                }
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
            if (Common::Status()->GetEquipWeapon().GetItemDef().GetName() == Common::LoadString_(IDS_STRING133))
            {
                auto lit = NSStarmanLib::WeaponManager::GetObj()->IsTorchLit();
                if (!lit)
                {
                    m_commandLib->UpsertCommand(Common::LoadString_(IDS_STRING192), true);
                }
                else
                {
                    m_commandLib->UpsertCommand(Common::LoadString_(IDS_STRING193), true);
                }
            }
        }
    }
    // NPCの機能を表示
    //
    // NPCとの距離が近ければ表示する。
    // ダイケイマンの場合、パッチテストとクラフト
    // サンカクマンとシカクマンはお手伝い
    // もし、万が一、サンカクマンとシカクマンの両方の近くにいた場合、
    // サンカクマンを優先して表示する
    {
        // ダイケイマン
        {
            auto npcMgr = NSStarmanLib::NpcStatusManager::GetObj();
            auto status = npcMgr->GetNpcStatus(_T("daikeiman"));
            auto enable = status.GetFeatureEnable();
            if (enable)
            {
                auto npcPos = D3DXVECTOR3(status.GetX(), status.GetY(), status.GetZ());
                auto ppos = SharedObj::GetPlayer()->GetPos();
                auto _near = Common::HitByBoundingBox(npcPos, ppos, 2.f);

                if (_near)
                {
                    m_commandLib->UpsertCommand(Common::LoadString_(IDS_STRING194), true);
                    m_commandLib->UpsertCommand(Common::LoadString_(IDS_STRING195), true);
                }
            }
        }

        // サンカクマン
        bool bShowSankakuHelp = false;
        {
            auto npcMgr = NSStarmanLib::NpcStatusManager::GetObj();
            auto status = npcMgr->GetNpcStatus(_T("sankakuman"));
            auto enable = status.GetFeatureEnable();
            if (enable)
            {
                auto npcPos = D3DXVECTOR3(status.GetX(), status.GetY(), status.GetZ());
                auto ppos = SharedObj::GetPlayer()->GetPos();
                auto _near = Common::HitByBoundingBox(npcPos, ppos, 2.f);

                if (_near)
                {
                    bool canReceive = NSStarmanLib::Help::Get()->CanReceive(_T("sankakuman"));
                    if (canReceive)
                    {
                        m_commandLib->UpsertCommand(Common::LoadString_(IDS_STRING196), true);
                        bShowSankakuHelp = true;
                    }
                }
            }
        }

        // シカクマン
        {
            if (!bShowSankakuHelp)
            {
                auto npcMgr = NSStarmanLib::NpcStatusManager::GetObj();
                auto status = npcMgr->GetNpcStatus(_T("shikakuman"));
                auto enable = status.GetFeatureEnable();
                if (enable)
                {
                    auto npcPos = D3DXVECTOR3(status.GetX(), status.GetY(), status.GetZ());
                    auto ppos = SharedObj::GetPlayer()->GetPos();
                    auto _near = Common::HitByBoundingBox(npcPos, ppos, 2.f);

                    if (_near)
                    {
                        bool canReceive = NSStarmanLib::Help::Get()->CanReceive(_T("shikakuman"));
                        if (canReceive)
                        {
                            m_commandLib->UpsertCommand(Common::LoadString_(IDS_STRING196), true);
                            bShowSankakuHelp = true;
                        }
                    }
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

    bool saveExist = SaveManager::Get()->SaveFolderExists();
    bool enable = false;
    if (saveExist)
    {
        enable = true;
    }
    else
    {
        enable = false;
    }

    m_commandLib->RemoveAll();
    m_commandLib->UpsertCommand(_T("Start"), true);
    m_commandLib->UpsertCommand(_T("Continue"), enable);
    m_commandLib->UpsertCommand(_T("Language"), true);
    m_commandLib->UpsertCommand(_T("Exit"), true);
}

void CommandManager::BuildLangCommand()
{
    //---------------------------------------------------
    // Japanese
    // English
    // Back
    //---------------------------------------------------

    m_commandLib->RemoveAll();
    m_commandLib->UpsertCommand(_T("Japanese"), true);
    m_commandLib->UpsertCommand(_T("English"), true);
    m_commandLib->UpsertCommand(_T("Back"), true);
}

