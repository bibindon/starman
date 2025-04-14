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
            "������",
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
        // 20�b�o�߂�����E�o�R�}���h��ǉ�
        ++m_counter;

        // 0.3�b�����ɍX�V
        // 20�Ŋ������]�肪1�ł���Ƃ��A�Ƃ��邱�ƂŁA
        // 0.33�b�����ɍX�V�����ƂƂ��ɁA���߂Ă��̊֐����Ă΂ꂽ�Ƃ��ɂ��X�V�Ă΂��B
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
        ScreenToClient(FindWindow("�z�V�}��", nullptr), &p);
        result = m_commandLib->Click(p.x, p.y);
    }
    else
    {
        POINT p;
        GetCursorPos(&p);
        ScreenToClient(FindWindow("�z�V�}��", nullptr), &p);
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

    // result����ł͂Ȃ��Ȃ�R�}���h���j���[����x����A�Ƃ�������
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
    // ���́E�E�E�߂��ɖ؂�����Ƃ��ɕ\�������B�C�J�_���[�h�̎��͕\������Ȃ��B
    // �̎�E�E�E�߂��ɑ�������Ƃ��ɕ\�������B�C�J�_���[�h�̎��͕\������Ȃ��B
    // ���ɂȂ�E�E�E��ɕ\�������
    // ����E�E�E��ɕ\�������
    // �E�o�E�E�E20�b�R�}���h���\�����ꂽ��
    // ���𒣂�E�E�E�C�J�_���[�h�̎�
    // ������ށE�E�E�C�J�_���[�h�̎��B�u���𒣂�v�Ɓu������ށv�͂ǂ��炩���\�������B
    // �R���ԑ����E�E�E�C�J�_���[�h�̎��A��ł͂Ȃ��C�ɂ���Ƃ�
    // �����オ��E�E�E�C�J�_���[�h�̎��B�C�J�_���[�h�����������B
    // �C�J�_�ɏ��E�E�E�C�J�_���߂��ɂ��鎞
    // �C�J�_�̑܂�����E�E�E�C�J�_���߂��ɂ��鎞
    //---------------------------------------------------

    m_commandLib->RemoveAll();
    auto voyage = VoyageManager::Get();
    auto raftMode = voyage->GetRaftMode();
    auto ppos = SharedObj::GetPlayer()->GetPos();

    // ���́E�E�E�߂��ɖ؂�����Ƃ��ɕ\�������B�C�J�_���[�h�̎��͕\������Ȃ��B
    if (!raftMode)
    {
        if (SharedObj::GetMap()->NearTree(ppos))
        {
            m_commandLib->UpsertCommand("����", true);
        }
        else
        {
            m_commandLib->UpsertCommand("����", false);
        }
    }

    // �̎�E�E�E�߂��ɑ�������Ƃ��ɕ\�������B�C�J�_���[�h�̎��͕\������Ȃ��B
    if (!raftMode)
    {
        if (SharedObj::GetMap()->NearPlant(ppos))
        {
            m_commandLib->UpsertCommand("�̎�", true);
        }
        else
        {
            m_commandLib->UpsertCommand("�̎�", false);
        }
    }

    // ���ɂȂ�E�E�E��ɕ\�������
    m_commandLib->UpsertCommand("���ɂȂ�", true);

    // ����E�E�E��ɕ\�������
    m_commandLib->UpsertCommand("����", true);

    // �E�o�E�E�E20�b�R�}���h���\�����ꂽ��
    if (m_counter > (60 * 20))
    {
        m_commandLib->UpsertCommand("�E�o", true);
    }

    // ���𒣂�E�E�E�C�J�_���[�h�̎�
    if (raftMode)
    {
        if (!voyage->GetSail())
        {
            m_commandLib->UpsertCommand("���𒣂�", true);
        }
    }

    // ������ށE�E�E�C�J�_���[�h�̎��B�u���𒣂�v�Ɓu������ށv�͂ǂ��炩���\�������B
    if (raftMode)
    {
        if (voyage->GetSail())
        {
            m_commandLib->UpsertCommand("�������", true);
        }
    }

    // �R���ԑ����E�E�E�C�J�_���[�h�̎��A��ł͂Ȃ��C�ɂ���Ƃ�
    if (raftMode)
    {
        if (voyage->GetPosType() == NSStarmanLib::Raft::ePosType::Sea)
        {
            m_commandLib->UpsertCommand("�R���ԑ���", true);
        }
        else
        {
            m_commandLib->UpsertCommand("�R���ԑ���", false);
        }
    }

    // �����オ��E�E�E�C�J�_���[�h�̎��B�C�J�_���[�h�����������B
    if (raftMode)
    {
        m_commandLib->UpsertCommand("�����オ��", true);
    }

    // �C�J�_�ɏ��E�E�E�C�J�_���߂��ɂ��鎞
    //
    // �C�J�_�����L���Ă��Ȃ��Ƃ��A��\��
    // �C�J�_�����L���Ă��āA�C�J�_���߂��ɂ���Ƃ��A�����ŕ\������
    // �C�J�_�����L���Ă��āA�C�J�_���߂��ɂȂ��Ƃ��A�񊈐��ŕ\������
    if (!raftMode)
    {
        if (voyage->GetRaftCount() == 0)
        {
            m_commandLib->RemoveCommand("�C�J�_�ɏ��");
        }
        else
        {
            if (voyage->CheckNearRaft(ppos))
            {
                m_commandLib->UpsertCommand("�C�J�_�ɏ��", true);
            }
            else
            {
                m_commandLib->UpsertCommand("�C�J�_�ɏ��", false);
            }
        }
    }

    // �C�J�_�̑܂�����E�E�E�C�J�_���߂��ɂ��鎞
    //
    // �C�J�_�����L���Ă��Ȃ��Ƃ��A��\��
    // �C�J�_�����L���Ă��āA�C�J�_���߂��ɂ���Ƃ��A�����ŕ\������
    // �C�J�_�����L���Ă��āA�C�J�_���߂��ɂȂ��Ƃ��A�񊈐��ŕ\������
    if (!raftMode)
    {
        if (voyage->GetRaftCount() == 0)
        {
            m_commandLib->RemoveCommand("�C�J�_�̑܂�����");
        }
        else
        {
            if (voyage->CheckNearRaft(ppos))
            {
                m_commandLib->UpsertCommand("�C�J�_�̑܂�����", true);
            }
            else
            {
                m_commandLib->UpsertCommand("�C�J�_�̑܂�����", false);
            }
        }
    }

    //----------------------------------------------
    // ���������
    //
    // ���@���g����悤�ɂȂ��Ă���Ƃ��\�������
    // �؂̖_�𑕔����Ă���Ɗ�����ԂɂȂ�B
    //----------------------------------------------
    {
        auto enableMagic = NSStarmanLib::Rynen::GetObj()->GetContracted();
        if (enableMagic)
        {
            if (Common::Status()->GetEquipWeapon().GetItemDef().GetName() == "�؂̖_")
            {
                m_commandLib->UpsertCommand("���������", true);
            }
            else
            {
                m_commandLib->UpsertCommand("���������", false);
            }
        }
    }

    //----------------------------------------------
    // �����ɉ΂�����
    // �����𑕔����Ă��āA�����ɉ΂����Ă��Ȃ��Ƃ��ɕ\�������
    //
    // �����̉΂�����
    // �����𑕔����Ă��āA�����ɉ΂����Ă���Ƃ��ɕ\�������
    //----------------------------------------------
    {
        auto weapon = Common::Status()->GetEquipWeapon();

        // ��������͑������Ă���B
        if (weapon.GetId() != 0 && weapon.GetId() != -1)
        {
            if (Common::Status()->GetEquipWeapon().GetItemDef().GetName() == "����")
            {
                auto lit = NSStarmanLib::WeaponManager::GetObj()->IsTorchLit();
                if (!lit)
                {
                    m_commandLib->UpsertCommand("�����ɉ΂�����", true);
                }
                else
                {
                    m_commandLib->UpsertCommand("�����̉΂�����", true);
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

