#include "MenuManager.h"
#include "SoundEffect.h"
#include <d3d9.h>
#include <d3dx9.h>
#include "SharedObj.h"
#include "KeyBoard.h"
#include "Mouse.h"
#include "..\..\StarmanLib\StarmanLib\StarmanLib\HumanInfoManager.h"
#include "..\..\StarmanLib\StarmanLib\StarmanLib\MapInfoManager.h"
#include "..\..\StarmanLib\StarmanLib\StarmanLib\ItemManager.h"
#include "..\..\StarmanLib\StarmanLib\StarmanLib\Inventory.h"
#include "..\..\StarmanLib\StarmanLib\StarmanLib\WeaponManager.h"
#include "..\..\StarmanLib\StarmanLib\StarmanLib\EnemyManager.h"
#include "..\..\StarmanLib\StarmanLib\StarmanLib\SkillManager.h"
#include "..\..\StarmanLib\StarmanLib\StarmanLib\StatusManager.h"

namespace NSMenulib
{
// �摜�̃t�@�C�����������Ȃ�ǂݍ��܂��ɋ��L����悤�ɂ���B
// �����Ȃ��ƊȒP�Ƀ������[�s���ŗ�����
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
        // �X�v���C�g�͈�̂݊m�ۂ��g���܂킷
        if (m_D3DSprite == NULL)
        {
            if (FAILED(D3DXCreateSprite(m_pD3DDevice, &m_D3DSprite)))
            {
                throw std::exception("Failed to create a sprite.");
            }
        }

        m_filepath = filepath;

        // �����摜�t�@�C���ō��ꂽ�e�N�X�`�������ɂ���Ȃ�A
        // �摜�̃T�C�Y�����m�ۂ��e�N�X�`���̍쐬���s��Ȃ�
        if (m_texMap.find(filepath) != m_texMap.end())
        {
            D3DSURFACE_DESC desc { };
            if (FAILED(m_texMap.at(m_filepath)->GetLevelDesc(0, &desc)))
            {
                throw std::exception("Failed to create a texture.");
            }
            m_width = desc.Width;
            m_height = desc.Height;
            return;
        }

        // �e�N�X�`���̍쐬
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
        // TODO �X�v���C�g���e�N�X�`�����g���܂킵�Ă���̂Ńf�X�g���N�^�ŉ���������ł��Ȃ��B
        // ������l���邱�ƂƂ���
//        if (m_D3DSprite != NULL)
//        {
//            m_D3DSprite->Release();
//            m_D3DSprite = NULL;
//        }
    }

private:

    LPDIRECT3DDEVICE9 m_pD3DDevice = NULL;

    // �X�v���C�g�͈���g���܂킷
    static LPD3DXSPRITE m_D3DSprite;

    std::string m_filepath;
    UINT m_width { 0 };
    UINT m_height { 0 };

    // �������O�̉摜�t�@�C���ō��ꂽ�e�N�X�`���͎g���܂킷
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
                                    24,
                                    0,
                                    FW_NORMAL,
                                    1,
                                    false,
                                    SHIFTJIS_CHARSET,
                                    OUT_TT_ONLY_PRECIS,
                                    ANTIALIASED_QUALITY,
                                    FF_DONTCARE,
                                    "�l�r ����",
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

    NSStarmanLib::ItemManager* itemManager = NSStarmanLib::ItemManager::GetObj();
    NSStarmanLib::WeaponManager* weaponManager = NSStarmanLib::WeaponManager::GetObj();
    //------------------------------------------------------
    // �A�C�e�����i���C���x���g���j
    //------------------------------------------------------
    {
        NSStarmanLib::Inventory* inventory = NSStarmanLib::Inventory::GetObj();

        std::vector<int> idList = itemManager->GetItemIdList();

        std::vector<ItemInfo> itemInfoList;
        for (std::size_t i = 0; i < idList.size(); ++i)
        {
            NSStarmanLib::ItemDef itemDef = itemManager->GetItemDef(idList.at(i));
            std::vector<int> subIdList = inventory->GetSubIdList(idList.at(i));
            {
                for (std::size_t j = 0; j < subIdList.size(); ++j)
                {
                    std::string work_str;

                    NSMenulib::ItemInfo itemInfoG;

                    itemInfoG.SetName(itemDef.GetName());

                    NSStarmanLib::ItemInfo itemInfo = inventory->GetItemInfo(idList.at(i), subIdList.at(j));

                    itemInfoG.SetDurability(itemInfo.GetDurabilityCurrent());

                    NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());

                    // �摜�t�@�C�������擾���Đݒ�
                    // �A�C�e����ʂ�����̎��͕���N���X����擾����K�v������
                    if (itemDef.GetType() != NSStarmanLib::ItemDef::ItemType::WEAPON)
                    {
                        work_str = itemDef.GetImagePath();
                    }
                    else
                    {
                        work_str = weaponManager->GetImageName(itemDef.GetName());
                    }

                    sprItem->Load(work_str);
                    itemInfoG.SetSprite(sprItem);

                    if (itemDef.GetType() != NSStarmanLib::ItemDef::ItemType::WEAPON)
                    {
                        work_str = itemDef.GetDetail();
                    }
                    else
                    {
                        work_str = weaponManager->GetDetail(itemDef.GetName());
                    }
                    work_str = itemDef.GetDetail();
                    work_str.erase(std::remove(work_str.begin(), work_str.end(), '"'), work_str.end());

                    itemInfoG.SetId(itemInfo.GetId());
                    itemInfoG.SetSubId(itemInfo.GetSubId());

                    itemInfoG.SetDetail(work_str);
                    itemInfoList.push_back(itemInfoG);
                }
            }
        }
        m_menu.SetItem(itemInfoList);
    }

    //------------------------------------------------------
    // �l�����
    //------------------------------------------------------
    std::vector<HumanInfo> humanInfoList;
    {
        NSStarmanLib::HumanInfoManager* humanInfoManager = NSStarmanLib::HumanInfoManager::GetObj();
        std::vector<std::string> humanNameList = humanInfoManager->GetHumanNameList();
        for (std::size_t i = 0; i < humanNameList.size(); ++i)
        {
            HumanInfo humanInfo;

            NSStarmanLib::HumanInfo libHumanInfo = humanInfoManager->GetHumanInfo(humanNameList.at(i));

            if (libHumanInfo.GetVisible() == false)
            {
                continue;
            }

            humanInfo.SetName(libHumanInfo.GetName());

            NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
            sprItem->Load(libHumanInfo.GetImagePath());
            humanInfo.SetSprite(sprItem);

            humanInfo.SetDetail(libHumanInfo.GetDetail());

            humanInfoList.push_back(humanInfo);
        }
    }
    m_menu.SetHuman(humanInfoList);

    //------------------------------------------------------
    // ������
    //------------------------------------------------------
    {
        NSStarmanLib::Inventory* inventory = NSStarmanLib::Inventory::GetObj();

        std::vector<int> idList = itemManager->GetItemIdList();

        std::vector<WeaponInfo> weaponInfoList;
        for (std::size_t i = 0; i < idList.size(); ++i)
        {
            NSStarmanLib::ItemDef itemDef = itemManager->GetItemDef(idList.at(i));
            NSStarmanLib::ItemDef::ItemType itemType = itemDef.GetType();

            // ���킶��Ȃ������疳��
            if (itemType != NSStarmanLib::ItemDef::ItemType::WEAPON)
            {
                continue;
            }

            std::vector<int> subIdList = inventory->GetSubIdList(idList.at(i));
            {
                for (std::size_t j = 0; j < subIdList.size(); ++j)
                {
                    std::string work_str;

                    NSMenulib::WeaponInfo weaponInfoG;

                    weaponInfoG.SetName(itemDef.GetName());

                    NSStarmanLib::ItemInfo itemInfo = inventory->GetItemInfo(idList.at(i), subIdList.at(j));

                    weaponInfoG.SetDurability(itemInfo.GetDurabilityCurrent());

                    NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());

                    work_str = weaponManager->GetImageName(itemDef.GetName());

                    sprItem->Load(work_str);
                    weaponInfoG.SetSprite(sprItem);

                    work_str = weaponManager->GetDetail(itemDef.GetName());
                    work_str.erase(std::remove(work_str.begin(), work_str.end(), '"'), work_str.end());

                    weaponInfoG.SetDetail(work_str);
                    weaponInfoList.push_back(weaponInfoG);
                }
            }
        }
        m_menu.SetWeapon(weaponInfoList);
    }

    // TODO �p�~
    // ����ɃX�e�[�^�X���������
    {
        std::vector<TaskInfo> infoList;
        {
            TaskInfo info;
            info.SetName("�T���v���e�L�X�g�P");
            NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
            sprItem->Load("res\\image\\test2.png");
            info.SetSprite(sprItem);
            info.SetDetail("�T���v���e�L�X�g\n\n�T���v���e�L�X�g\n�T���v���e�L�X�g\n�T���v���e�L�X�g");
            infoList.push_back(info);
        }
        {
            TaskInfo info;
            info.SetName("�T���v���e�L�X�g�Q");
            NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
            sprItem->Load("res\\image\\test2.png");
            info.SetSprite(sprItem);
            info.SetDetail("�T���v���e�L�X�g\n\n�T���v���e�L�X�g\n�T���v���e�L�X�g\n�T���v���e�L�X�g");
            infoList.push_back(info);
        }
        {
            TaskInfo info;
            info.SetName("�T���v���e�L�X�g�R");
            NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
            sprItem->Load("res\\image\\test2.png");
            info.SetSprite(sprItem);
            info.SetDetail("�T���v���e�L�X�g\n\n�T���v���e�L�X�g\n�T���v���e�L�X�g\n�T���v���e�L�X�g");
            infoList.push_back(info);
        }
        m_menu.SetTask(infoList);
    }

    //------------------------------------------------------
    // �G���
    //------------------------------------------------------
    {
        std::vector<EnemyInfo> infoList;
        NSStarmanLib::EnemyManager* enemyManager = NSStarmanLib::EnemyManager::GetObj();
        std::vector<std::string> nameList = enemyManager->GetEnemyNameList();
        for (std::size_t i = 0; i < nameList.size(); ++i)
        {
            NSStarmanLib::EnemyDef enemyDef = enemyManager->GetEnemyDef(nameList.at(i));
            if (enemyDef.GetVisible() == false)
            {
                continue;
            }
            NSMenulib::EnemyInfo enemyInfo;
            enemyInfo.SetName(enemyDef.GetName());
            enemyInfo.SetDetail(enemyDef.GetDetail());

            NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
            sprItem->Load(enemyDef.GetImagePath());
            enemyInfo.SetSprite(sprItem);
            infoList.push_back(enemyInfo);
        }
        m_menu.SetEnemy(infoList);
    }

    //------------------------------------------------------
    // �Z�E���@���
    //------------------------------------------------------
    {
        std::vector<SkillInfo> infoList;

        NSStarmanLib::SkillManager* skillManager = NSStarmanLib::SkillManager::GetObj();
        std::vector<std::string> nameList = skillManager->GetNameList();
        for (std::size_t i = 0; i < nameList.size(); ++i)
        {
            SkillInfo info;
            info.SetName(nameList.at(i));

            std::string work_str;
            work_str = skillManager->GetDetail(nameList.at(i));
            info.SetDetail(work_str);

            NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
            sprItem->Load("res\\image\\test.png");
            info.SetSprite(sprItem);
            infoList.push_back(info);
        }
        m_menu.SetSkill(infoList);
    }

    //------------------------------------------------------
    // �n�}���
    //------------------------------------------------------
    {
        std::vector<MapInfo> mapInfoList;
        {
            NSStarmanLib::MapInfoManager* mapInfoManager = NSStarmanLib::MapInfoManager::GetObj();

            std::vector<std::string> mapNameList = mapInfoManager->GetNameList();
            for (std::size_t i = 0; i < mapNameList.size(); ++i)
            {
                std::string mapName = mapNameList.at(i);

                bool visible = mapInfoManager->IsDiscovered(mapName);

                if (visible == false)
                {
                    continue;
                }

                MapInfo mapInfo;
                mapInfo.SetName(mapName);
                mapInfo.SetDetail(mapInfoManager->GetDetail(mapName));

                NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
                sprItem->Load(mapInfoManager->GetImagePath(mapName));
                mapInfo.SetSprite(sprItem);

                mapInfoList.push_back(mapInfo);
            }
        }
        m_menu.SetMap(mapInfoList);
    }
}

void MenuManager::Draw()
{
    m_menu.Draw();
}

std::string MenuManager::OperateMenu()
{
    std::string result;
    std::string work_str;

    if (KeyBoard::IsDown(DIK_UP))
    {
        m_menu.Up();
    }

    if (KeyBoard::IsHold(DIK_UP))
    {
        m_menu.Up();
    }

    if (KeyBoard::IsDown(DIK_DOWN))
    {
        m_menu.Down();
    }

    if (KeyBoard::IsHold(DIK_DOWN))
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
        result = m_menu.Into();

        std::vector<std::string> vs = Common::split(result, ':');
        if (vs.size() == 5 && vs.at(0) == "�A�C�e��")
        {
            // �A�C�e�����g��
            if (vs.at(4) == "�g��")
            {
                int id = std::stoi(vs.at(2));
                int subId = std::stoi(vs.at(3));

                if (UseItem(id, subId))
                {
                    NSStarmanLib::Inventory* inventory = NSStarmanLib::Inventory::GetObj();
                    inventory->RemoveItem(id, subId);
                    m_menu.DeleteItem(id, subId);
                }
            }
            // �A�C�e�����̂Ă�
            if (vs.at(4) == "�̂Ă�")
            {
                int id = std::stoi(vs.at(2));
                int subId = std::stoi(vs.at(3));

                NSStarmanLib::Inventory* inventory = NSStarmanLib::Inventory::GetObj();
                inventory->RemoveItem(id, subId);
                m_menu.DeleteItem(id, subId);
            }
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
        ScreenToClient(FindWindowA("�z�V�}��", nullptr), &p);
        m_menu.Click(p.x, p.y);
    }

    if (Mouse::IsDownRight())
    {
        POINT p;
        GetCursorPos(&p);
        ScreenToClient(FindWindowA("�z�V�}��", nullptr), &p);
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

bool MenuManager::UseItem(const int id, const int subId)
{
    // TODO StatusManager�Ɉړ����ׂ�
    // 
    // �H�ނ�������X�e�[�^�X���X�V
    // TODO �H�ވȊO�͂��Ƃōl����
    NSStarmanLib::ItemManager* itemManager = NSStarmanLib::ItemManager::GetObj();
    NSStarmanLib::ItemDef itemDef = itemManager->GetItemDef(id);

    if (itemDef.GetType() != NSStarmanLib::ItemDef::ItemType::FOOD)
    {
        return false;
    }

    NSStarmanLib::StatusManager* statusManager = NSStarmanLib::StatusManager::GetObj();

    float work_f = 0.f;
    work_f = statusManager->GetCarboCurrent();
    work_f += itemDef.GetCarbo();
    statusManager->SetCarboCurrent(work_f);

    work_f = statusManager->GetProteinCurrent();
    work_f += itemDef.GetProtein();
    statusManager->SetProteinCurrent(work_f);

    work_f = statusManager->GetLipidCurrent();
    work_f += itemDef.GetLipid();
    statusManager->SetLipidCurrent(work_f);

    work_f = statusManager->GetVitaminCurrent();
    work_f += itemDef.GetVitamin();
    statusManager->SetVitaminCurrent(work_f);

    work_f = statusManager->GetMineralCurrent();
    work_f += itemDef.GetMineral();
    statusManager->SetMineralCurrent(work_f);

    work_f = statusManager->GetMineralCurrent();
    work_f += itemDef.GetMineral();
    statusManager->SetMineralCurrent(work_f);

    work_f = statusManager->GetWaterCurrent();
    work_f += itemDef.GetWater();
    statusManager->SetWaterCurrent(work_f);
    
    return true;
}

