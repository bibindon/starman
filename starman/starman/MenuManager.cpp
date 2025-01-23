#include "MenuManager.h"
#include "SoundEffect.h"
#include <d3d9.h>
#include <d3dx9.h>
#include "SharedObj.h"
#include "KeyBoard.h"
#include "Mouse.h"
#include "SaveManager.h"
#include "..\..\StarmanLib\StarmanLib\StarmanLib\HumanInfoManager.h"
#include "..\..\StarmanLib\StarmanLib\StarmanLib\MapInfoManager.h"
#include "..\..\StarmanLib\StarmanLib\StarmanLib\ItemManager.h"
#include "..\..\StarmanLib\StarmanLib\StarmanLib\Inventory.h"
#include "..\..\StarmanLib\StarmanLib\StarmanLib\WeaponManager.h"
#include "..\..\StarmanLib\StarmanLib\StarmanLib\EnemyInfoManager.h"
#include "..\..\StarmanLib\StarmanLib\StarmanLib\SkillManager.h"
#include "..\..\StarmanLib\StarmanLib\StarmanLib\StatusManager.h"
#include "..\..\StarmanLib\StarmanLib\StarmanLib\Guide.h"
#include "GamePad.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/Rynen.h"
#include "PopUp2.h"

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
                                    20,
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

    virtual void DrawText_(const std::string& msg,
                           const int x,
                           const int y,
                           const bool hcenter,
                           const int transparency)
    {
        if (hcenter)
        {
            RECT rect = { x, y, x + 100, y + 20 };
            m_pFont->DrawText(NULL, msg.c_str(),
                              -1,
                              &rect,
                              DT_VCENTER | DT_CENTER | DT_NOCLIP,
                              D3DCOLOR_ARGB(transparency, 255, 255, 255));
        }
        else
        {
            RECT rect = { x, y, x + 200, y + 20 };
            m_pFont->DrawText(NULL, msg.c_str(),
                              -1,
                              &rect,
                              DT_VCENTER | DT_NOCLIP,
                              D3DCOLOR_ARGB(transparency, 255, 255, 255));
        }
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

MenuManager::MenuManager()
{
}

MenuManager::~MenuManager()
{
    m_menu.Finalize();
}

void MenuManager::InitMenu()
{
    using namespace NSMenulib;
    NSMenulib::Sprite* sprCursor = NEW NSMenulib::Sprite(SharedObj::GetD3DDevice());
    sprCursor->Load("res\\image\\menu_cursor.png");

    NSMenulib::Sprite* sprBackground = NEW NSMenulib::Sprite(SharedObj::GetD3DDevice());
    sprBackground->Load("res\\image\\menu_back.png");

    NSMenulib::IFont* pFont = NEW NSMenulib::Font(SharedObj::GetD3DDevice());
    pFont->Init();

    NSMenulib::ISoundEffect* pSE = NEW NSMenulib::SoundEffect();
    pSE->Init();

    m_menu.Init("", pFont, pSE, sprCursor, sprBackground);

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
                    itemInfoG.SetLevel(itemDef.GetLevel());

                    NSStarmanLib::ItemInfo itemInfo = inventory->GetItemInfo(idList.at(i), subIdList.at(j));

                    itemInfoG.SetDurability(itemInfo.GetDurabilityCurrent());

                    NSMenulib::Sprite* sprItem = NEW NSMenulib::Sprite(SharedObj::GetD3DDevice());

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

                    if (itemInfo.GetItemDef().GetName() == "��")
                    {
                        itemInfoG.SetEquipEnable(true);

                        if (IsBagEquiped(itemInfo.GetId(), itemInfo.GetSubId()))
                        {
                            itemInfoG.SetEquip(true);
                        }
                    }
                    itemInfoG.SetWeight(itemInfo.GetItemDef().GetWeight());
                    itemInfoG.SetVolume((int)itemInfo.GetItemDef().GetVolume());

                    itemInfoList.push_back(itemInfoG);
                }
            }
        }
        m_menu.SetItem(itemInfoList);

        m_menu.SetWeightAll(Common::Inventory()->GetWeight());
        m_menu.SetVolumeAll((int)Common::Inventory()->GetVolume());
        m_menu.SetVolumeMax((int)Common::Inventory()->GetVolumeMax());
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

            NSMenulib::Sprite* sprItem = NEW NSMenulib::Sprite(SharedObj::GetD3DDevice());
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

                    weaponInfoG.SetId(idList.at(i));
                    weaponInfoG.SetSubId(subIdList.at(j));

                    weaponInfoG.SetName(itemDef.GetName());
                    weaponInfoG.SetLevel(itemDef.GetLevel());

                    NSStarmanLib::ItemInfo itemInfo = inventory->GetItemInfo(idList.at(i), subIdList.at(j));

                    weaponInfoG.SetDurability(itemInfo.GetDurabilityCurrent());

                    NSMenulib::Sprite* sprItem = NEW NSMenulib::Sprite(SharedObj::GetD3DDevice());

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

    //------------------------------------------------------
    // �������
    //------------------------------------------------------
    {
        std::vector<GuideInfo> infoList;
        NSStarmanLib::Guide* guide = NSStarmanLib::Guide::GetObj();
        std::vector<std::string> vs = guide->GetCategoryList();
        for (std::size_t i = 0; i < vs.size(); ++i)
        {
            std::vector<std::string> vsSub = guide->GetSubCategoryList(vs.at(i));
            for (std::size_t j = 0; j < vsSub.size(); ++j)
            {
                if (guide->GetVisible(vs.at(i), vsSub.at(j)) == false)
                {
                    continue;
                }
                GuideInfo info;
                info.SetCategory(vs.at(i));
                info.SetSubCategory(vsSub.at(j));
                std::string detail = guide->GetText(vs.at(i), vsSub.at(j));
                info.SetDetail(detail);
                infoList.push_back(info);
            }
        }
        m_menu.SetGuide(infoList);
    }

    //------------------------------------------------------
    // �G���
    //------------------------------------------------------
    {
        std::vector<EnemyInfo> infoList;
        NSStarmanLib::EnemyInfoManager* enemyInfoManager = NSStarmanLib::EnemyInfoManager::GetObj();
        std::vector<std::string> nameList = enemyInfoManager->GetEnemyNameList();
        for (std::size_t i = 0; i < nameList.size(); ++i)
        {
            NSStarmanLib::EnemyDef enemyDef = enemyInfoManager->GetEnemyDef(nameList.at(i));
            if (enemyDef.GetVisible() == false)
            {
                continue;
            }
            NSMenulib::EnemyInfo enemyInfo;
            enemyInfo.SetName(enemyDef.GetName());
            enemyInfo.SetDetail(enemyDef.GetDetail());

            NSMenulib::Sprite* sprItem = NEW NSMenulib::Sprite(SharedObj::GetD3DDevice());
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

            NSMenulib::Sprite* sprItem = NEW NSMenulib::Sprite(SharedObj::GetD3DDevice());
            sprItem->Load("res\\image\\test.png");
            info.SetSprite(sprItem);
            infoList.push_back(info);
        }
        m_menu.SetSkill(infoList);
    }

    //------------------------------------------------------
    // �X�e�[�^�X
    //------------------------------------------------------
    {
        std::vector<StatusInfo> infoList;

        NSStarmanLib::StatusManager* statusManager = NSStarmanLib::StatusManager::GetObj();
        StatusInfo info;
        info.SetName("�z�V�}��");
        NSMenulib::Sprite* sprItem = NEW NSMenulib::Sprite(SharedObj::GetD3DDevice());
        sprItem->Load("res\\image\\test.png");
        info.SetSprite(sprItem);
        std::string work;
        work = "�p�����[�^�@�@�@���ݒl/�񕜉\�l/�ő�l\n";
        /*
        work += "�̂̃X�^�~�i:\t";
        work += Common::ToStringWithPrecision(statusManager->GetBodyStaminaCurrent(), 2) + "/";
        work += Common::ToStringWithPrecision(statusManager->GetBodyStaminaMaxSub(), 2) + "/";
        work += Common::ToStringWithPrecision(statusManager->GetBodyStaminaMax(), 2) + "\n";
        work += "�]�̃X�^�~�i:\t";
        work += Common::ToStringWithPrecision(statusManager->GetBrainStaminaCurrent(), 2) + "/";
        work += Common::ToStringWithPrecision(statusManager->GetBrainStaminaMaxSub(), 2) + "/";
        work += Common::ToStringWithPrecision(statusManager->GetBrainStaminaMax(), 2) + "\n";
        work += "�u����:\t";
        work += Common::ToStringWithPrecision(statusManager->GetExplosivePower(), 2) + "/-/-\n";
        work += "���̂̏C���x:\t";
        work += Common::ToStringWithPrecision(statusManager->GetMuscleCurrent(), 2) + "/-/";
        work += Common::ToStringWithPrecision(statusManager->GetMuscleMax(), 2) + "\n";
        work += "����:\t";
        work += Common::ToStringWithPrecision(statusManager->GetCarboCurrent(), 2) + "/-/";
        work += Common::ToStringWithPrecision(statusManager->GetCarboMax(), 2) + "\n";
        work += "�^���p�N��:";
        work += Common::ToStringWithPrecision(statusManager->GetProteinCurrent(), 2) + "/-/";
        work += Common::ToStringWithPrecision(statusManager->GetProteinMax(), 2) + "\n";
        work += "����:";
        work += Common::ToStringWithPrecision(statusManager->GetLipidCurrent(), 2) + "/-/";
        work += Common::ToStringWithPrecision(statusManager->GetLipidMax(), 2) + "\n";
        work += "�r�^�~��:";
        work += Common::ToStringWithPrecision(statusManager->GetVitaminCurrent(), 2) + "/-/";
        work += Common::ToStringWithPrecision(statusManager->GetVitaminMax(), 2) + "\n";
        work += "�~�l����:";
        work += Common::ToStringWithPrecision(statusManager->GetMineralCurrent(), 2) + "/-/";
        work += Common::ToStringWithPrecision(statusManager->GetMineralMax(), 2) + "\n";
        work += "����:";
        work += Common::ToStringWithPrecision(statusManager->GetWaterCurrent(), 2) + "/-/";
        work += Common::ToStringWithPrecision(statusManager->GetWaterMax(), 2) + "\n";
        work += "\n";
        work += "��Ԉُ�F";
        work += "����/����/����/�r����/������/�E���Ǐ�/�Q�s��";
        work += "\n";
        work += "��������F\taaa";
        work += "\n";
        work += "�U���́F\t";
        work += Common::ToStringWithPrecision(statusManager->GetAttackPower()) + "\n";
        work += "�h��́F\t";
        work += Common::ToStringWithPrecision(statusManager->GetDefensePower()) + "\n";
        */
        info.SetDetail(work);
        infoList.push_back(info);
        m_menu.SetStatus(infoList);
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

                NSMenulib::Sprite* sprItem = NEW NSMenulib::Sprite(SharedObj::GetD3DDevice());
                sprItem->Load(mapInfoManager->GetImagePath(mapName));
                mapInfo.SetSprite(sprItem);

                mapInfoList.push_back(mapInfo);
            }
        }
        m_menu.SetMap(mapInfoList);
    }
}

void MenuManager::Finalize()
{
    m_menu.Finalize();
}

void MenuManager::Draw()
{
    m_menu.Draw();
}

std::string MenuManager::OperateMenu()
{
    std::string result;
    std::string work_str;

    if (KeyBoard::IsDownFirstFrame(DIK_UP))
    {
        m_menu.Up();
    }

    if (KeyBoard::IsHold(DIK_UP))
    {
        m_menu.Up();
    }

    if (KeyBoard::IsDownFirstFrame(DIK_DOWN))
    {
        m_menu.Down();
    }

    if (KeyBoard::IsHold(DIK_DOWN))
    {
        m_menu.Down();
    }

    if (KeyBoard::IsDownFirstFrame(DIK_LEFT))
    {
        m_menu.Left();
    }

    if (KeyBoard::IsDownFirstFrame(DIK_RIGHT))
    {
        m_menu.Right();
    }

    if (KeyBoard::IsDownFirstFrame(DIK_RETURN))
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
                    DeleteItem(id, subId);
                }
            }
            // �A�C�e�����̂Ă�
            else if (vs.at(4) == "�̂Ă�")
            {
                int id = std::stoi(vs.at(2));
                int subId = std::stoi(vs.at(3));

                NSStarmanLib::Inventory* inventory = NSStarmanLib::Inventory::GetObj();
                inventory->RemoveItem(id, subId);
                DeleteItem(id, subId);
            }
            else if (vs.at(4) == "��������")
            {
                int id = std::stoi(vs.at(2));
                int subId = std::stoi(vs.at(3));

                NSStarmanLib::Inventory* inventory = NSStarmanLib::Inventory::GetObj();
                auto itemInfo = inventory->GetItemInfo(id, subId);

                auto statusManager = NSStarmanLib::StatusManager::GetObj();
                statusManager->EquipBag(itemInfo);

                NSMenulib::ItemInfo itemInfoG;
                itemInfoG.SetId(id);
                itemInfoG.SetSubId(subId);
                itemInfoG.SetDurability(itemInfo.GetDurabilityCurrent());
                itemInfoG.SetLevel(itemInfo.GetItemDef().GetLevel());
                itemInfoG.SetEquip(true);
                m_menu.UpdateItem(itemInfoG);
            }
            else if (vs.at(4) == "�������O��")
            {
                int id = std::stoi(vs.at(2));
                int subId = std::stoi(vs.at(3));

                auto statusManager = NSStarmanLib::StatusManager::GetObj();
                statusManager->UnequipBag(id, subId);

                NSStarmanLib::Inventory* inventory = NSStarmanLib::Inventory::GetObj();
                auto itemInfo = inventory->GetItemInfo(id, subId);

                NSMenulib::ItemInfo itemInfoG;
                itemInfoG.SetId(id);
                itemInfoG.SetSubId(subId);
                itemInfoG.SetDurability(itemInfo.GetDurabilityCurrent());
                itemInfoG.SetLevel(itemInfo.GetItemDef().GetLevel());
                itemInfoG.SetEquip(false);
                m_menu.UpdateItem(itemInfoG);
            }
        }
        else if (vs.size() == 5 && vs.at(0) == "����")
        {
            if (vs.at(4) == "����")
            {
                int id = std::stoi(vs.at(2));
                int subId = std::stoi(vs.at(3));

                NSStarmanLib::Inventory* inventory = NSStarmanLib::Inventory::GetObj();
                NSStarmanLib::ItemInfo itemInfo = inventory->GetItemInfo(id, subId);

                NSStarmanLib::StatusManager* statusManager = NSStarmanLib::StatusManager::GetObj();
                statusManager->SetEquipWeapon(itemInfo);
            }
        }
        else if (vs.size() >= 1 && vs.at(0) == "�Z�[�u���ďI��")
        {
            auto saveManager = SaveManager::Get();
            saveManager->Save();
            PostMessage(SharedObj::GetWindowHandle(), WM_CLOSE, 0, 0);
        }
    }

    if (KeyBoard::IsDownFirstFrame(DIK_ESCAPE))
    {
        result = m_menu.Back();
    }

    if (KeyBoard::IsDownFirstFrame(DIK_BACK))
    {
        result = m_menu.Back();
    }

    if (Mouse::IsDownLeft())
    {
        POINT p = Common::GetScreenPos();;
        result = m_menu.Click(p.x, p.y);

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
                    DeleteItem(id, subId);
                }
            }
            // �A�C�e�����̂Ă�
            else if (vs.at(4) == "�̂Ă�")
            {
                int id = std::stoi(vs.at(2));
                int subId = std::stoi(vs.at(3));

                NSStarmanLib::Inventory* inventory = NSStarmanLib::Inventory::GetObj();
                inventory->RemoveItem(id, subId);
                DeleteItem(id, subId);
            }
            else if (vs.at(4) == "��������")
            {
                int id = std::stoi(vs.at(2));
                int subId = std::stoi(vs.at(3));

                NSStarmanLib::Inventory* inventory = NSStarmanLib::Inventory::GetObj();
                auto itemInfo = inventory->GetItemInfo(id, subId);

                auto statusManager = NSStarmanLib::StatusManager::GetObj();
                statusManager->EquipBag(itemInfo);

                NSMenulib::ItemInfo itemInfoG;
                itemInfoG.SetId(id);
                itemInfoG.SetSubId(subId);
                itemInfoG.SetDurability(itemInfo.GetDurabilityCurrent());
                itemInfoG.SetLevel(itemInfo.GetItemDef().GetLevel());
                itemInfoG.SetEquip(true);
                m_menu.UpdateItem(itemInfoG);
            }
            else if (vs.at(4) == "�������O��")
            {
                int id = std::stoi(vs.at(2));
                int subId = std::stoi(vs.at(3));

                auto statusManager = NSStarmanLib::StatusManager::GetObj();
                statusManager->UnequipBag(id, subId);

                NSStarmanLib::Inventory* inventory = NSStarmanLib::Inventory::GetObj();
                auto itemInfo = inventory->GetItemInfo(id, subId);

                NSMenulib::ItemInfo itemInfoG;
                itemInfoG.SetId(id);
                itemInfoG.SetSubId(subId);
                itemInfoG.SetDurability(itemInfo.GetDurabilityCurrent());
                itemInfoG.SetLevel(itemInfo.GetItemDef().GetLevel());
                itemInfoG.SetEquip(false);
                m_menu.UpdateItem(itemInfoG);
            }
        }
        else if (vs.size() == 5 && vs.at(0) == "����")
        {
            if (vs.at(4) == "����")
            {
                int id = std::stoi(vs.at(2));
                int subId = std::stoi(vs.at(3));

                NSStarmanLib::Inventory* inventory = NSStarmanLib::Inventory::GetObj();
                NSStarmanLib::ItemInfo itemInfo = inventory->GetItemInfo(id, subId);

                NSStarmanLib::StatusManager* statusManager = NSStarmanLib::StatusManager::GetObj();
                statusManager->SetEquipWeapon(itemInfo);
            }
        }
        else if (vs.size() >= 1 && vs.at(0) == "�Z�[�u���ďI��")
        {
            auto saveManager = SaveManager::Get();
            saveManager->Save();
            PostMessage(SharedObj::GetWindowHandle(), WM_CLOSE, 0, 0);
        }
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
            m_menu.CursorOn(p.x, p.y);
        }
        
        previousPoint = p;
    }

    if (Mouse::IsDownRight())
    {
        m_menu.RightClick();
    }

    if (Mouse::GetZDelta() < 0)
    {
        m_menu.Next();
    }
    else if (Mouse::GetZDelta() > 0)
    {
        m_menu.Previous();
    }

    if (GamePad::IsDown(eGamePadButtonType::UP))
    {
        m_menu.Up();
    }

    if (GamePad::IsHold(eGamePadButtonType::UP))
    {
        m_menu.Up();
    }

    if (GamePad::IsDown(eGamePadButtonType::DOWN))
    {
        m_menu.Down();
    }

    if (GamePad::IsHold(eGamePadButtonType::DOWN))
    {
        m_menu.Down();
    }

    if (GamePad::IsDown(eGamePadButtonType::LEFT))
    {
        m_menu.Left();
    }

    if (GamePad::IsDown(eGamePadButtonType::RIGHT))
    {
        m_menu.Right();
    }

    if (GamePad::IsDown(eGamePadButtonType::A))
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
                    DeleteItem(id, subId);
                }
            }
            // �A�C�e�����̂Ă�
            else if (vs.at(4) == "�̂Ă�")
            {
                int id = std::stoi(vs.at(2));
                int subId = std::stoi(vs.at(3));

                NSStarmanLib::Inventory* inventory = NSStarmanLib::Inventory::GetObj();
                inventory->RemoveItem(id, subId);
                DeleteItem(id, subId);
            }
            else if (vs.at(4) == "��������")
            {
                int id = std::stoi(vs.at(2));
                int subId = std::stoi(vs.at(3));

                NSStarmanLib::Inventory* inventory = NSStarmanLib::Inventory::GetObj();
                auto itemInfo = inventory->GetItemInfo(id, subId);

                auto statusManager = NSStarmanLib::StatusManager::GetObj();
                statusManager->EquipBag(itemInfo);

                NSMenulib::ItemInfo itemInfoG;
                itemInfoG.SetId(id);
                itemInfoG.SetSubId(subId);
                itemInfoG.SetDurability(itemInfo.GetDurabilityCurrent());
                itemInfoG.SetLevel(itemInfo.GetItemDef().GetLevel());
                itemInfoG.SetEquip(true);
                m_menu.UpdateItem(itemInfoG);
            }
            else if (vs.at(4) == "�������O��")
            {
                int id = std::stoi(vs.at(2));
                int subId = std::stoi(vs.at(3));

                auto statusManager = NSStarmanLib::StatusManager::GetObj();
                statusManager->UnequipBag(id, subId);

                NSStarmanLib::Inventory* inventory = NSStarmanLib::Inventory::GetObj();
                auto itemInfo = inventory->GetItemInfo(id, subId);

                NSMenulib::ItemInfo itemInfoG;
                itemInfoG.SetId(id);
                itemInfoG.SetSubId(subId);
                itemInfoG.SetDurability(itemInfo.GetDurabilityCurrent());
                itemInfoG.SetLevel(itemInfo.GetItemDef().GetLevel());
                itemInfoG.SetEquip(false);
                m_menu.UpdateItem(itemInfoG);
            }
        }
        else if (vs.size() == 5 && vs.at(0) == "����")
        {
            if (vs.at(4) == "����")
            {
                int id = std::stoi(vs.at(2));
                int subId = std::stoi(vs.at(3));

                NSStarmanLib::Inventory* inventory = NSStarmanLib::Inventory::GetObj();
                NSStarmanLib::ItemInfo itemInfo = inventory->GetItemInfo(id, subId);

                NSStarmanLib::StatusManager* statusManager = NSStarmanLib::StatusManager::GetObj();
                statusManager->SetEquipWeapon(itemInfo);
            }
        }
        else if (vs.size() >= 1 && vs.at(0) == "�Z�[�u���ďI��")
        {
            auto saveManager = SaveManager::Get();
            saveManager->Save();
            PostMessage(SharedObj::GetWindowHandle(), WM_CLOSE, 0, 0);
        }
    }

    if (GamePad::IsDown(eGamePadButtonType::B))
    {
        result = m_menu.Back();
    }

    // �\�����e���X�V
    {
        using namespace NSMenulib;
        std::vector<StatusInfo> infoList;

        NSStarmanLib::StatusManager* statusManager = NSStarmanLib::StatusManager::GetObj();

        NSStarmanLib::ItemInfo itemInfo = statusManager->GetEquipWeapon();
        std::string weaponName;

        if (itemInfo.GetId() != -1)
        {
            NSStarmanLib::ItemManager* itemManager = NSStarmanLib::ItemManager::GetObj();
            NSStarmanLib::ItemDef itemDef = itemManager->GetItemDef(itemInfo.GetId());
            weaponName = itemDef.GetName();
        }

        // ��Ԉُ탊�X�g
        std::string condition;
        {
            if (statusManager->GetFractureArm())
            {
                condition += "�r����/";
            }

            if (statusManager->GetFractureLeg())
            {
                condition += "������/";
            }

            if (statusManager->GetHeadache())
            {
                condition += "����/";
            }

            if (statusManager->GetCold())
            {
                condition += "����/";
            }

            if (statusManager->GetStomachache())
            {
                condition += "����/";
            }

            if (statusManager->GetSleep())
            {
                condition += "�����E�C��/";
            }

            if (statusManager->GetDehydration())
            {
                condition += "�E���Ǐ�/";
            }

            if (statusManager->GetLackOfSleep())
            {
                condition += "�����s��/";
            }

            if (statusManager->GetDead())
            {
                condition += "���S/";
            }

            if (condition.empty() == false)
            {
                condition.pop_back();
            }
        }

        StatusInfo info;
        info.SetName("�z�V�}��");
        NSMenulib::Sprite* sprItem = NEW NSMenulib::Sprite(SharedObj::GetD3DDevice());
        sprItem->Load("res\\image\\hoshiman00.png");
        info.SetSprite(sprItem);
        std::string work;
        work =  "�p�����[�^      ���ݒl/�񕜉\�l/�ő�l\n";
        work += "�̂̃X�^�~�i    ";
        work += Common::ToStringWithPrecision(statusManager->GetBodyStaminaCurrent(), 2) + "/";
        work += Common::ToStringWithPrecision(statusManager->GetBodyStaminaMaxSub(), 2) + "/";
        work += Common::ToStringWithPrecision(statusManager->GetBodyStaminaMax(), 2) + "\n";
        work += "�]�̃X�^�~�i    ";
        work += Common::ToStringWithPrecision(statusManager->GetBrainStaminaCurrent(), 2) + "/";
        work += Common::ToStringWithPrecision(statusManager->GetBrainStaminaMaxSub(), 2) + "/";
        work += Common::ToStringWithPrecision(statusManager->GetBrainStaminaMax(), 2) + "\n";
        work += "�u����          ";
        work += Common::ToStringWithPrecision(statusManager->GetExplosivePower(), 2) + "/-/-\n";
        work += "���̂̏C���x    ";
        work += Common::ToStringWithPrecision(statusManager->GetMuscleCurrent(), 2) + "/-/";
        work += Common::ToStringWithPrecision(statusManager->GetMuscleMax(), 2) + "\n";
        work += "����            ";
        work += Common::ToStringWithPrecision(statusManager->GetCarboCurrent(), 2) + "/-/";
        work += Common::ToStringWithPrecision(statusManager->GetCarboMax(), 2) + "\n";
        work += "�^���p�N��      ";
        work += Common::ToStringWithPrecision(statusManager->GetProteinCurrent(), 2) + "/-/";
        work += Common::ToStringWithPrecision(statusManager->GetProteinMax(), 2) + "\n";
        work += "����            ";
        work += Common::ToStringWithPrecision(statusManager->GetLipidCurrent(), 2) + "/-/";
        work += Common::ToStringWithPrecision(statusManager->GetLipidMax(), 2) + "\n";
        work += "�r�^�~��        ";
        work += Common::ToStringWithPrecision(statusManager->GetVitaminCurrent(), 2) + "/-/";
        work += Common::ToStringWithPrecision(statusManager->GetVitaminMax(), 2) + "\n";
        work += "�~�l����        ";
        work += Common::ToStringWithPrecision(statusManager->GetMineralCurrent(), 2) + "/-/";
        work += Common::ToStringWithPrecision(statusManager->GetMineralMax(), 2) + "\n";
        work += "����            ";
        work += Common::ToStringWithPrecision(statusManager->GetWaterCurrent(), 2) + "/-/";
        work += Common::ToStringWithPrecision(statusManager->GetWaterMax(), 2) + "\n";
//        work += "\n";
        work += "��Ԉُ�       ";
        work += condition;
        work += "\n";
        work += "��������       ";
        work += weaponName;
        work += "\n";
        work += "�U����         ";
        work += Common::ToStringWithPrecision(statusManager->GetAttackPower(), 2) + "\n";

        auto rynen = NSStarmanLib::Rynen::GetObj();
        if (rynen->GetContracted())
        {
            work += "���[�h�u���X   ";

            if (rynen->GetReviveEnable())
            {
                work += "�����\\n";
            }
            else
            {
                work += "�����ς�\n";
            }
        }

        {
            work += "�ύڗ�         ";

            auto vol = NSStarmanLib::Inventory::GetObj()->GetVolume();
            work += Common::ToStringWithPrecision(vol, 2) + "/";

            auto volMax = NSStarmanLib::Inventory::GetObj()->GetVolumeMax();
            work += Common::ToStringWithPrecision(volMax, 2) + "\n";
        }

        info.SetDetail(work);
        infoList.push_back(info);
        m_menu.SetStatus(infoList);
    }
    return result;
}

bool MenuManager::UseItem(const int id, const int subId)
{
    bool result = false;

    // �H�ނ�������X�e�[�^�X���X�V
    // TODO �H�ވȊO�͂��Ƃōl����
    auto itemManager = NSStarmanLib::ItemManager::GetObj();
    auto itemDef = itemManager->GetItemDef(id);
    auto statusManager = NSStarmanLib::StatusManager::GetObj();

    if (itemDef.GetType() == NSStarmanLib::ItemDef::ItemType::FOOD)
    {
        // ������������false���Ԃ��Ă���
        result = statusManager->Eat(itemDef);
    }
    else if (itemDef.GetType() == NSStarmanLib::ItemDef::ItemType::OTHERS)
    {
        // ���[�h�u���X
        if (itemDef.GetName() == "���[�h�u���X")
        {
            auto pos = SharedObj::GetPlayer();
            statusManager->DrinkWordBress(pos->GetPos().x, pos->GetPos().y, pos->GetPos().z);
            result = true;
        }
    }

    return result;
}

void MenuManager::DeleteItem(const int id, const int subId)
{
    Common::ReduceBrainStaminaCurrent(0.1f);
    Common::Inventory()->ReduceEquipBagDurability();

    m_menu.DeleteItem(id, subId);

    // �ϋv�x�����������̂ōX�V
    auto allBag = Common::Status()->GetAllBag();
    for (auto it = allBag.begin(); it != allBag.end(); ++it)
    {
        if (it->GetId() == -1)
        {
            continue;
        }

        // NSMenulib::ItemInfo��NSStarmanLib::ItemInfo�̕ϊ�
        // ���x���Ƒ�����ԂƑϋv�x�����ł���
        NSMenulib::ItemInfo itemInfo;
        itemInfo.SetId(it->GetId());
        itemInfo.SetSubId(it->GetSubId());
        itemInfo.SetLevel(it->GetItemDef().GetLevel());
        itemInfo.SetEquip(true);
        itemInfo.SetDurability(it->GetDurabilityCurrent());
        m_menu.UpdateItem(itemInfo);
    }
}

void MenuManager::AddItem(const int id, const int subId, const int durability)
{
    using namespace NSMenulib;
    NSStarmanLib::ItemManager* itemManager = NSStarmanLib::ItemManager::GetObj();
    NSStarmanLib::Inventory* inventory = NSStarmanLib::Inventory::GetObj();
    NSStarmanLib::WeaponManager* weaponManager = NSStarmanLib::WeaponManager::GetObj();

    NSStarmanLib::ItemDef itemDef = itemManager->GetItemDef(id);
    std::string work_str;

    NSMenulib::ItemInfo itemInfoG;

    itemInfoG.SetName(itemDef.GetName());
    itemInfoG.SetDurability(durability);

    NSMenulib::Sprite* sprItem = NEW NSMenulib::Sprite(SharedObj::GetD3DDevice());

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

    itemInfoG.SetId(id);
    itemInfoG.SetSubId(subId);

    if (itemDef.GetName() == "��")
    {
        itemInfoG.SetEquipEnable(true);

        if (IsBagEquiped(id, subId))
        {
            itemInfoG.SetEquip(true);
        }
    }

    itemInfoG.SetDetail(work_str);

    itemInfoG.SetWeight(itemDef.GetWeight());
    itemInfoG.SetVolume((int)itemDef.GetVolume());

    Common::ReduceBrainStaminaCurrent(0.1f);
    Common::Inventory()->ReduceEquipBagDurability();

    m_menu.AddItem(itemInfoG);

    // �ϋv�x�����������̂ōX�V
    auto allBag = Common::Status()->GetAllBag();
    for (auto it = allBag.begin(); it != allBag.end(); ++it)
    {
        if (it->GetId() == -1)
        {
            continue;
        }

        // NSMenulib::ItemInfo��NSStarmanLib::ItemInfo�̕ϊ�
        // ���x���Ƒ�����ԂƑϋv�x�����ł���
        NSMenulib::ItemInfo itemInfo;
        itemInfo.SetId(it->GetId());
        itemInfo.SetSubId(it->GetSubId());
        itemInfo.SetLevel(it->GetItemDef().GetLevel());
        itemInfo.SetEquip(true);
        itemInfo.SetDurability(it->GetDurabilityCurrent());
        m_menu.UpdateItem(itemInfo);
    }
}

bool MenuManager::IsBagEquiped(const int id, const int subId)
{
    auto statusManager = NSStarmanLib::StatusManager::GetObj();
    auto allBag = statusManager->GetAllBag();
    auto it = std::find_if(allBag.begin(), allBag.end(),
                           [&](const NSStarmanLib::ItemInfo& x)
                           {
                               return x.GetId() == id && x.GetSubId() == subId;
                           });

    if (it != allBag.end())
    {
        return true;
    }
    else
    {
        return false;
    }
}
