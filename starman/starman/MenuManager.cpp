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
#include <cassert>
#include "resource.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/NpcStatusManager.h"

namespace NSMenulib
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
        if (m_D3DSprite == nullptr)
        {
            return;
        }

        D3DXVECTOR3 pos { (float)x, (float)y, 0.f };
        m_D3DSprite->Begin(D3DXSPRITE_ALPHABLEND);
        RECT rect = {
            0,
            0,
            static_cast<LONG>(m_width),
            static_cast<LONG>(m_height) };
        D3DXVECTOR3 center { 0, 0, 0 };
		m_D3DSprite->Draw(m_texMap.at(m_filepath),
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

    void Init(const bool bEnglish)
    {
        HRESULT hr = S_OK;

        if (!bEnglish)
        {
            hr = D3DXCreateFont(m_pD3DDevice,
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
        else
        {
            hr = D3DXCreateFont(m_pD3DDevice,
                                20,
                                0,
                                FW_NORMAL,
                                1,
                                false,
                                DEFAULT_CHARSET,
                                OUT_TT_ONLY_PRECIS,
                                CLEARTYPE_NATURAL_QUALITY,
                                FF_DONTCARE,
                                "Courier New",
                                &m_pFont);
        }

        assert(hr == S_OK);
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
    NSMenulib::ISprite* sprCursor = NEW NSMenulib::Sprite(SharedObj::GetD3DDevice());
    sprCursor->Load("res\\image\\menu_cursor.png");

    NSMenulib::ISprite* sprBackground = NEW NSMenulib::Sprite(SharedObj::GetD3DDevice());
    sprBackground->Load("res\\image\\menu_back.png");

    NSMenulib::IFont* pFont = NEW NSMenulib::Font(SharedObj::GetD3DDevice());

    NSMenulib::ISoundEffect* pSE = NEW NSMenulib::SoundEffect();

    m_menu.Init("", pFont, pSE, sprCursor, sprBackground, SharedObj::IsEnglish());

    NSStarmanLib::ItemManager* itemManager = NSStarmanLib::ItemManager::GetObj();
    NSStarmanLib::WeaponManager* weaponManager = NSStarmanLib::WeaponManager::GetObj();
    //------------------------------------------------------
    // アイテム情報（＝インベントリ）
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

                    // 画像ファイル名を取得して設定
                    // アイテム種別が武器の時は武器クラスから取得する必要がある
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

                    // ID : 27 ~ 32 == 袋
                    if (itemInfo.GetItemDef().GetId() == 27 ||
                        itemInfo.GetItemDef().GetId() == 28 ||
                        itemInfo.GetItemDef().GetId() == 29 ||
                        itemInfo.GetItemDef().GetId() == 30 ||
                        itemInfo.GetItemDef().GetId() == 31 ||
                        itemInfo.GetItemDef().GetId() == 32)
                    {
                        itemInfoG.SetEquipEnable(true);

                        if (IsBagEquiped(itemInfo.GetId(), itemInfo.GetSubId()))
                        {
                            itemInfoG.SetEquip(true);
                        }
                    }
                    else if (itemDef.GetType() == NSStarmanLib::ItemDef::ItemType::WEAPON)
                    {
                        itemInfoG.SetEquipEnable(true);

                        auto weapon = Common::Status()->GetEquipWeapon();
                        if (weapon.GetId() == itemInfo.GetId() &&
                            weapon.GetSubId() == itemInfo.GetSubId())
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
    // 人物情報
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
    // 武器情報
    //------------------------------------------------------
    {
        NSStarmanLib::Inventory* inventory = NSStarmanLib::Inventory::GetObj();

        std::vector<int> idList = itemManager->GetItemIdList();

        std::vector<WeaponInfo> weaponInfoList;
        for (std::size_t i = 0; i < idList.size(); ++i)
        {
            NSStarmanLib::ItemDef itemDef = itemManager->GetItemDef(idList.at(i));
            NSStarmanLib::ItemDef::ItemType itemType = itemDef.GetType();

            // 武器じゃなかったら無視
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
    // 操作説明
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
    // 敵情報
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
    // 技・魔法情報
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
    // ステータス
    //------------------------------------------------------
    {
        // 毎秒更新する。ここではやらない。
    }

    //------------------------------------------------------
    // 地図情報
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

    if (SharedObj::KeyBoard()->IsDownFirstFrame(DIK_UP))
    {
        m_menu.Up();
    }

    if (SharedObj::KeyBoard()->IsHold(DIK_UP))
    {
        m_menu.Up();
    }

    if (SharedObj::KeyBoard()->IsDownFirstFrame(DIK_DOWN))
    {
        m_menu.Down();
    }

    if (SharedObj::KeyBoard()->IsHold(DIK_DOWN))
    {
        m_menu.Down();
    }

    if (SharedObj::KeyBoard()->IsDownFirstFrame(DIK_LEFT))
    {
        m_menu.Left();
    }

    if (SharedObj::KeyBoard()->IsDownFirstFrame(DIK_RIGHT))
    {
        m_menu.Right();
    }

    if (SharedObj::KeyBoard()->IsDownFirstFrame(DIK_RETURN))
    {
        result = m_menu.Into();

        std::vector<std::string> vs = Common::split(result, ':');
        if (vs.size() == 5 && vs.at(0) == "Item")
        {
            // アイテムを使う
            if (vs.at(4) == "Use")
            {
                int id = std::stoi(vs.at(2));
                int subId = std::stoi(vs.at(3));

                if (UseItem(id, subId))
                {
                    DeleteItem(id, subId);
                }
            }
            // アイテムを捨てる
            else if (vs.at(4) == "Discard")
            {
                int id = std::stoi(vs.at(2));
                int subId = std::stoi(vs.at(3));
                DeleteItem(id, subId);
            }
            else if (vs.at(4) == "Equip")
            {
                // 火のついた松明を装備していたら武器を装備できない。袋も装備できない。
                bool lit = NSStarmanLib::WeaponManager::GetObj()->IsTorchLit();

                if (lit)
                {
                    PopUp2::Get()->SetText(IDS_STRING197);
                }
                else
                {
                    int id = std::stoi(vs.at(2));
                    int subId = std::stoi(vs.at(3));
                    Equip(id, subId);
                }
            }
            else if (vs.at(4) == "Unequip")
            {
                // 点灯中の松明の装備を外すことはできない。
                bool lit = NSStarmanLib::WeaponManager::GetObj()->IsTorchLit();
                if (lit)
                {
                    PopUp2::Get()->SetText(IDS_STRING198);
                }
                else
                {
                    int id = std::stoi(vs.at(2));
                    int subId = std::stoi(vs.at(3));
                    Unequip(id, subId);
                }
            }
        }
        else if (vs.size() == 5 && vs.at(0) == "Weapon")
        {
            if (vs.at(4) == "Equip")
            {
                int id = std::stoi(vs.at(2));
                int subId = std::stoi(vs.at(3));

                Equip(id, subId);
            }
        }
        else if (vs.size() >= 1 && vs.at(0) == "Save and Exit")
        {
            auto saveManager = SaveManager::Get();
            saveManager->Save();
            PostMessage(SharedObj::GetWindowHandle(), WM_CLOSE, 0, 0);
        }
    }

    if (SharedObj::KeyBoard()->IsDownFirstFrame(DIK_ESCAPE))
    {
        // EXIT以外なら無視する
        result = m_menu.Back();
        if (result != "EXIT")
        {
            result = "";
        }
    }

    if (SharedObj::KeyBoard()->IsDownFirstFrame(DIK_BACK))
    {
        // EXIT以外なら無視する
        result = m_menu.Back();
        if (result != "EXIT")
        {
            result = "";
        }
    }

    if (Mouse::IsDownLeft())
    {
        POINT p = Common::GetScreenPos();;
        result = m_menu.Click(p.x, p.y);

        std::vector<std::string> vs = Common::split(result, ':');
        if (vs.size() == 5 && vs.at(0) == "Item")
        {
            // アイテムを使う
            if (vs.at(4) == "Use")
            {
                int id = std::stoi(vs.at(2));
                int subId = std::stoi(vs.at(3));

                if (UseItem(id, subId))
                {
                    DeleteItem(id, subId);
                }
            }
            // アイテムを捨てる
            else if (vs.at(4) == "Discard")
            {
                int id = std::stoi(vs.at(2));
                int subId = std::stoi(vs.at(3));
                DeleteItem(id, subId);
            }
            else if (vs.at(4) == "Equip")
            {
                int id = std::stoi(vs.at(2));
                int subId = std::stoi(vs.at(3));
                Equip(id, subId);
            }
            else if (vs.at(4) == "Unequip")
            {
                int id = std::stoi(vs.at(2));
                int subId = std::stoi(vs.at(3));
                Unequip(id, subId);
            }
        }
        else if (vs.size() == 5 && vs.at(0) == "Weapon")
        {
            if (vs.at(4) == "Equip")
            {
                int id = std::stoi(vs.at(2));
                int subId = std::stoi(vs.at(3));

                NSStarmanLib::Inventory* inventory = NSStarmanLib::Inventory::GetObj();
                NSStarmanLib::ItemInfo itemInfo = inventory->GetItemInfo(id, subId);

                NSStarmanLib::StatusManager* statusManager = NSStarmanLib::StatusManager::GetObj();
                statusManager->SetEquipWeapon(itemInfo);
            }
        }
        else if (vs.size() >= 1 && vs.at(0) == "Save and Exit")
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
        if (vs.size() == 5 && vs.at(0) == "Item")
        {
            // アイテムを使う
            if (vs.at(4) == "Use")
            {
                int id = std::stoi(vs.at(2));
                int subId = std::stoi(vs.at(3));

                if (UseItem(id, subId))
                {
                    DeleteItem(id, subId);
                }
            }
            // アイテムを捨てる
            else if (vs.at(4) == "Discard")
            {
                int id = std::stoi(vs.at(2));
                int subId = std::stoi(vs.at(3));
                DeleteItem(id, subId);
            }
            else if (vs.at(4) == "Equip")
            {
                int id = std::stoi(vs.at(2));
                int subId = std::stoi(vs.at(3));
                Equip(id, subId);
            }
            else if (vs.at(4) == "Unequip")
            {
                int id = std::stoi(vs.at(2));
                int subId = std::stoi(vs.at(3));
                Unequip(id, subId);
            }
        }
        else if (vs.size() == 5 && vs.at(0) == "Weapon")
        {
            if (vs.at(4) == "Equip")
            {
                int id = std::stoi(vs.at(2));
                int subId = std::stoi(vs.at(3));

                NSStarmanLib::Inventory* inventory = NSStarmanLib::Inventory::GetObj();
                NSStarmanLib::ItemInfo itemInfo = inventory->GetItemInfo(id, subId);

                NSStarmanLib::StatusManager* statusManager = NSStarmanLib::StatusManager::GetObj();
                statusManager->SetEquipWeapon(itemInfo);
            }
        }
        else if (vs.size() >= 1 && vs.at(0) == "Save and Exit")
        {
            auto saveManager = SaveManager::Get();
            saveManager->Save();
            PostMessage(SharedObj::GetWindowHandle(), WM_CLOSE, 0, 0);
        }
    }

    if (GamePad::IsDown(eGamePadButtonType::B))
    {
        // EXIT以外なら無視する
        result = m_menu.Back();
        if (result != "EXIT")
        {
            result = "";
        }
    }

    // ステータス画面の表示内容を更新
    {
		std::vector<NSMenulib::StatusInfo> infoList;

        // ホシマン
        {

			NSStarmanLib::StatusManager* statusManager = NSStarmanLib::StatusManager::GetObj();

			NSStarmanLib::ItemInfo itemInfo = statusManager->GetEquipWeapon();
			std::string weaponName;

			if (itemInfo.GetId() != -1)
			{
				NSStarmanLib::ItemManager* itemManager = NSStarmanLib::ItemManager::GetObj();
				NSStarmanLib::ItemDef itemDef = itemManager->GetItemDef(itemInfo.GetId());
				weaponName = itemDef.GetName();
			}

			// 状態異常リスト
			std::string condition;
			{
				if (statusManager->GetFractureArm())
				{
					condition += Common::LoadString_(IDS_STRING155)+ "/";
				}

				if (statusManager->GetFractureLeg())
				{
					condition += Common::LoadString_(IDS_STRING156)+ "/";
				}

				if (statusManager->GetHeadache())
				{
					condition += Common::LoadString_(IDS_STRING157)+ "/";
				}

				if (statusManager->GetCold())
				{
					condition += Common::LoadString_(IDS_STRING158)+ "/";
				}

				if (statusManager->GetStomachache())
				{
					condition += Common::LoadString_(IDS_STRING159)+ "/";
				}

				if (statusManager->GetSleep())
				{
					condition += Common::LoadString_(IDS_STRING160)+ "/";
				}

				if (statusManager->GetDehydration())
				{
					condition += Common::LoadString_(IDS_STRING161)+ "/";
				}

				if (statusManager->GetLackOfSleep())
				{
					condition += Common::LoadString_(IDS_STRING162)+ "/";
				}

				if (statusManager->GetDead())
				{
					condition += Common::LoadString_(IDS_STRING163)+ "/";
				}

				if (condition.empty() == false)
				{
					condition.pop_back();
				}
			}

			NSMenulib::StatusInfo info;
			info.SetName(Common::LoadString_(IDS_STRING211));

			NSMenulib::Sprite* sprItem = NEW NSMenulib::Sprite(SharedObj::GetD3DDevice());
			sprItem->Load("res\\image\\hoshiman00.png");
			info.SetSprite(sprItem);

			char buf[1024];
			std::string work;
			work =  Common::LoadString_(IDS_STRING210) + "\n";

			snprintf(buf, sizeof(buf), "%-20s", Common::LoadString_(IDS_STRING146).c_str());
			work += buf;
			work += Common::ToStringWithPrecision(statusManager->GetBodyStaminaCurrent(), 2) + "/";
			work += Common::ToStringWithPrecision(statusManager->GetBodyStaminaMaxSub(), 2) + "/";
			work += Common::ToStringWithPrecision(statusManager->GetBodyStaminaMax(), 2) + "\n";

			snprintf(buf, sizeof(buf), "%-20s", Common::LoadString_(IDS_STRING147).c_str());
			work += buf;
			work += Common::ToStringWithPrecision(statusManager->GetBrainStaminaCurrent(), 2) + "/";
			work += Common::ToStringWithPrecision(statusManager->GetBrainStaminaMaxSub(), 2) + "/";
			work += Common::ToStringWithPrecision(statusManager->GetBrainStaminaMax(), 2) + "\n";

			snprintf(buf, sizeof(buf), "%-20s", Common::LoadString_(IDS_STRING209).c_str());
			work += buf;
			work += Common::ToStringWithPrecision(statusManager->GetExplosivePower(), 2) + "/-/-\n";

			snprintf(buf, sizeof(buf), "%-20s", Common::LoadString_(IDS_STRING149).c_str());
			work += buf;
			work += Common::ToStringWithPrecision(statusManager->GetMuscleCurrent(), 2) + "/-/";
			work += Common::ToStringWithPrecision(statusManager->GetMuscleMax(), 2) + "\n";

			snprintf(buf, sizeof(buf), "%-20s", Common::LoadString_(IDS_STRING150).c_str());
			work += buf;
			work += Common::ToStringWithPrecision(statusManager->GetCarboCurrent(), 2) + "/-/";
			work += Common::ToStringWithPrecision(statusManager->GetCarboMax(), 2) + "\n";

			snprintf(buf, sizeof(buf), "%-20s", Common::LoadString_(IDS_STRING151).c_str());
			work += buf;
			work += Common::ToStringWithPrecision(statusManager->GetProteinCurrent(), 2) + "/-/";
			work += Common::ToStringWithPrecision(statusManager->GetProteinMax(), 2) + "\n";

			snprintf(buf, sizeof(buf), "%-20s", Common::LoadString_(IDS_STRING152).c_str());
			work += buf;
			work += Common::ToStringWithPrecision(statusManager->GetLipidCurrent(), 2) + "/-/";
			work += Common::ToStringWithPrecision(statusManager->GetLipidMax(), 2) + "\n";

			snprintf(buf, sizeof(buf), "%-20s", Common::LoadString_(IDS_STRING153).c_str());
			work += buf;
			work += Common::ToStringWithPrecision(statusManager->GetVitaminCurrent(), 2) + "/-/";
			work += Common::ToStringWithPrecision(statusManager->GetVitaminMax(), 2) + "\n";

			snprintf(buf, sizeof(buf), "%-20s", Common::LoadString_(IDS_STRING154).c_str());
			work += buf;
			work += Common::ToStringWithPrecision(statusManager->GetMineralCurrent(), 2) + "/-/";
			work += Common::ToStringWithPrecision(statusManager->GetMineralMax(), 2) + "\n";

			snprintf(buf, sizeof(buf), "%-20s", Common::LoadString_(IDS_STRING148).c_str());
			work += buf;
			work += Common::ToStringWithPrecision(statusManager->GetWaterCurrent(), 2) + "/-/";
			work += Common::ToStringWithPrecision(statusManager->GetWaterMax(), 2) + "\n";
	//        work += "\n";

			snprintf(buf, sizeof(buf), "%-20s", Common::LoadString_(IDS_STRING213).c_str());
			work += buf;
			work += condition;
			work += "\n";

			snprintf(buf, sizeof(buf), "%-20s", Common::LoadString_(IDS_STRING214).c_str());
			work += buf;
			work += weaponName;
			work += "\n";

			snprintf(buf, sizeof(buf), "%-20s", Common::LoadString_(IDS_STRING215).c_str());
			work += buf;
			work += Common::ToStringWithPrecision(statusManager->GetAttackPower(), 2) + "\n";

			auto rynen = NSStarmanLib::Rynen::GetObj();
			if (rynen->GetContracted())
			{
				work += "ワードブレス   ";

				if (rynen->GetReviveEnable())
				{
					work += "復活可能\n";
				}
				else
				{
					work += "復活済み\n";
				}
			}

			info.SetDetail(work);
			infoList.push_back(info);
        }

		//----------------------------------------------------
		// イカダのステータス
		//----------------------------------------------------
		{
			if (SharedObj::Voyage()->GetRaftMode())
			{
				// イカダは複数個持つことができるが、ステータスに表示するのは乗船中のイカダだけ
				// ・耐久度
				// ・強化値
				NSMenulib::StatusInfo info;
				info.SetName("イカダ");
				NSMenulib::Sprite* sprItem = NEW NSMenulib::Sprite(SharedObj::GetD3DDevice());
				sprItem->Load("res\\image\\raft.png");
				info.SetSprite(sprItem);
				std::string work;
				work += "現在の耐久値\n";
				work += std::to_string(SharedObj::Voyage()->GetRaftDurability()) + "\n";
				work += "現在の強化値\n";
				work += std::to_string(SharedObj::Voyage()->GetRaftLevel()) + "\n";

				info.SetDetail(work);
				infoList.push_back(info);
			}
		}

        // ダイケイマン
        {
			auto npcStatusMgr = NSStarmanLib::NpcStatusManager::GetObj();

            auto status = npcStatusMgr->GetNpcStatus("daikeiman");
            if (status.GetMenuShow())
            {
				NSMenulib::StatusInfo info;
				info.SetName(Common::LoadString_(IDS_STRING121));

				NSMenulib::Sprite* sprItem = NEW NSMenulib::Sprite(SharedObj::GetD3DDevice());
				sprItem->Load("res\\image\\daikeiman00.png");
				info.SetSprite(sprItem);

				char buf[1024];
				std::string work;
				work =  Common::LoadString_(IDS_STRING210) + "\n";

				snprintf(buf, sizeof(buf), "%-20s", Common::LoadString_(IDS_STRING150).c_str());
				work += buf;
				work += Common::ToStringWithPrecision(status.GetCarbo(), 2) + "/-/";
				work += Common::ToStringWithPrecision(100, 2) + "\n";

				snprintf(buf, sizeof(buf), "%-20s", Common::LoadString_(IDS_STRING151).c_str());
				work += buf;
				work += Common::ToStringWithPrecision(status.GetProtein(), 2) + "/-/";
				work += Common::ToStringWithPrecision(100, 2) + "\n";

				snprintf(buf, sizeof(buf), "%-20s", Common::LoadString_(IDS_STRING152).c_str());
				work += buf;
				work += Common::ToStringWithPrecision(status.GetLipid(), 2) + "/-/";
				work += Common::ToStringWithPrecision(100, 2) + "\n";

				snprintf(buf, sizeof(buf), "%-20s", Common::LoadString_(IDS_STRING153).c_str());
				work += buf;
				work += Common::ToStringWithPrecision(status.GetVitamin(), 2) + "/-/";
				work += Common::ToStringWithPrecision(100, 2) + "\n";

				snprintf(buf, sizeof(buf), "%-20s", Common::LoadString_(IDS_STRING154).c_str());
				work += buf;
				work += Common::ToStringWithPrecision(status.GetMineral(), 2) + "/-/";
				work += Common::ToStringWithPrecision(100, 2) + "\n";

				snprintf(buf, sizeof(buf), "%-20s", Common::LoadString_(IDS_STRING148).c_str());
				work += buf;
				work += Common::ToStringWithPrecision(status.GetWater(), 2) + "/-/";
				work += Common::ToStringWithPrecision(100, 2) + "\n";
		        work += "\n";

				if (status.GetRynenContract())
				{
					work += "ワードブレス   ";

					if (status.GetDrinkWordbress())
					{
						work += "使用済み\n";
					}
					else
					{
						work += "未使用\n";
					}
				}

                if (status.GetDead())
                {
                    work += "死亡\n";
                }
				else
                {
					work += "生存\n";
				}

				info.SetDetail(work);
				infoList.push_back(info);
            }
        }

        // サンカクマン
        {
			auto npcStatusMgr = NSStarmanLib::NpcStatusManager::GetObj();

            auto status = npcStatusMgr->GetNpcStatus("sankakuman");
            if (status.GetMenuShow())
            {
				NSMenulib::StatusInfo info;
				info.SetName(Common::LoadString_(IDS_STRING112));

				NSMenulib::Sprite* sprItem = NEW NSMenulib::Sprite(SharedObj::GetD3DDevice());
				sprItem->Load("res\\image\\sankakuman00.png");
				info.SetSprite(sprItem);

				char buf[1024];
				std::string work;
				work =  Common::LoadString_(IDS_STRING210) + "\n";

				snprintf(buf, sizeof(buf), "%-20s", Common::LoadString_(IDS_STRING150).c_str());
				work += buf;
				work += Common::ToStringWithPrecision(status.GetCarbo(), 2) + "/-/";
				work += Common::ToStringWithPrecision(100, 2) + "\n";

				snprintf(buf, sizeof(buf), "%-20s", Common::LoadString_(IDS_STRING151).c_str());
				work += buf;
				work += Common::ToStringWithPrecision(status.GetProtein(), 2) + "/-/";
				work += Common::ToStringWithPrecision(100, 2) + "\n";

				snprintf(buf, sizeof(buf), "%-20s", Common::LoadString_(IDS_STRING152).c_str());
				work += buf;
				work += Common::ToStringWithPrecision(status.GetLipid(), 2) + "/-/";
				work += Common::ToStringWithPrecision(100, 2) + "\n";

				snprintf(buf, sizeof(buf), "%-20s", Common::LoadString_(IDS_STRING153).c_str());
				work += buf;
				work += Common::ToStringWithPrecision(status.GetVitamin(), 2) + "/-/";
				work += Common::ToStringWithPrecision(100, 2) + "\n";

				snprintf(buf, sizeof(buf), "%-20s", Common::LoadString_(IDS_STRING154).c_str());
				work += buf;
				work += Common::ToStringWithPrecision(status.GetMineral(), 2) + "/-/";
				work += Common::ToStringWithPrecision(100, 2) + "\n";

				snprintf(buf, sizeof(buf), "%-20s", Common::LoadString_(IDS_STRING148).c_str());
				work += buf;
				work += Common::ToStringWithPrecision(status.GetWater(), 2) + "/-/";
				work += Common::ToStringWithPrecision(100, 2) + "\n";
		        work += "\n";

				if (status.GetRynenContract())
				{
					work += "ワードブレス   ";

					if (status.GetDrinkWordbress())
					{
						work += "使用済み\n";
					}
					else
					{
						work += "未使用\n";
					}
				}

                if (status.GetDead())
                {
                    work += "死亡\n";
                }
				else
                {
					work += "生存\n";
				}


				info.SetDetail(work);
				infoList.push_back(info);
            }
        }

        // シカクマン
        {
			auto npcStatusMgr = NSStarmanLib::NpcStatusManager::GetObj();

            auto status = npcStatusMgr->GetNpcStatus("shikakuman");
            if (status.GetMenuShow())
            {
				NSMenulib::StatusInfo info;
				info.SetName(Common::LoadString_(IDS_STRING112));

				NSMenulib::Sprite* sprItem = NEW NSMenulib::Sprite(SharedObj::GetD3DDevice());
				sprItem->Load("res\\image\\shikakuman00.png");
				info.SetSprite(sprItem);

				char buf[1024];
				std::string work;
				work =  Common::LoadString_(IDS_STRING210) + "\n";

				snprintf(buf, sizeof(buf), "%-20s", Common::LoadString_(IDS_STRING150).c_str());
				work += buf;
				work += Common::ToStringWithPrecision(status.GetCarbo(), 2) + "/-/";
				work += Common::ToStringWithPrecision(100, 2) + "\n";

				snprintf(buf, sizeof(buf), "%-20s", Common::LoadString_(IDS_STRING151).c_str());
				work += buf;
				work += Common::ToStringWithPrecision(status.GetProtein(), 2) + "/-/";
				work += Common::ToStringWithPrecision(100, 2) + "\n";

				snprintf(buf, sizeof(buf), "%-20s", Common::LoadString_(IDS_STRING152).c_str());
				work += buf;
				work += Common::ToStringWithPrecision(status.GetLipid(), 2) + "/-/";
				work += Common::ToStringWithPrecision(100, 2) + "\n";

				snprintf(buf, sizeof(buf), "%-20s", Common::LoadString_(IDS_STRING153).c_str());
				work += buf;
				work += Common::ToStringWithPrecision(status.GetVitamin(), 2) + "/-/";
				work += Common::ToStringWithPrecision(100, 2) + "\n";

				snprintf(buf, sizeof(buf), "%-20s", Common::LoadString_(IDS_STRING154).c_str());
				work += buf;
				work += Common::ToStringWithPrecision(status.GetMineral(), 2) + "/-/";
				work += Common::ToStringWithPrecision(100, 2) + "\n";

				snprintf(buf, sizeof(buf), "%-20s", Common::LoadString_(IDS_STRING148).c_str());
				work += buf;
				work += Common::ToStringWithPrecision(status.GetWater(), 2) + "/-/";
				work += Common::ToStringWithPrecision(100, 2) + "\n";
		        work += "\n";

				if (status.GetRynenContract())
				{
					work += "ワードブレス   ";

					if (status.GetDrinkWordbress())
					{
						work += "使用済み\n";
					}
					else
					{
						work += "未使用\n";
					}
				}

                if (status.GetDead())
                {
                    work += "死亡\n";
                }
				else
                {
					work += "生存\n";
				}


				info.SetDetail(work);
				infoList.push_back(info);
            }
        }

		m_menu.SetStatus(infoList);
    }
    return result;
}

bool MenuManager::UseItem(const int id, const int subId)
{
    bool result = false;

    // 食材だったらステータスを更新
    // TODO 食材以外はあとで考える
    auto itemManager = NSStarmanLib::ItemManager::GetObj();
    auto itemDef = itemManager->GetItemDef(id);
    auto statusManager = NSStarmanLib::StatusManager::GetObj();

    if (itemDef.GetType() == NSStarmanLib::ItemDef::ItemType::FOOD)
    {
        // 満腹だったらfalseが返ってくる
        result = statusManager->Eat(itemDef);
        if (!result)
        {
            PopUp2::Get()->SetText("満腹なのでこれ以上食べられない。");
        }
    }
    else if (itemDef.GetType() == NSStarmanLib::ItemDef::ItemType::MATERIAL)
    {
        PopUp2::Get()->SetText("ホシマン（何かの素材として使えそうだ）");
        result = false;
    }
    else if (itemDef.GetType() == NSStarmanLib::ItemDef::ItemType::VALUABLES)
    {
        // スマホ
        if (itemDef.GetId() == 1)
        {
            PopUp2::Get()->SetText("ホシマン（反応しない。海水で基盤がダメになったのだろう）");
            result = false;
        }
        // 家の鍵
        else if (itemDef.GetId() == 2)
        {
            PopUp2::Get()->SetText("ホシマン（まだ捨てないで取っておこう）");
            result = false;
        }
    }
    else if (itemDef.GetType() == NSStarmanLib::ItemDef::ItemType::OTHERS)
    {
        // ワードブレス
        if (itemDef.GetId() == 35)
        {
            auto pos = SharedObj::GetPlayer();
            statusManager->DrinkWordBress(pos->GetPos().x, pos->GetPos().y, pos->GetPos().z);
            result = true;
        }
        else
        {
            PopUp2::Get()->SetText("ホシマン（使い道が思いつかない）");
            result = false;
        }
    }

    return result;
}

void MenuManager::DeleteItem(const int id, const int subId)
{
    // 装備中の袋だったら削除できないようにする
    {
        auto allBag = Common::Status()->GetAllBag();
        for (auto it = allBag.begin(); it != allBag.end(); ++it)
        {
            if (it->GetId() == id && it->GetSubId() == subId)
            {
                PopUp2::Get()->SetText(IDS_STRING199);
                return;
            }
        }
    }

    // 装備中の武器だったら削除できないようにする
    {
        auto weapon = Common::Status()->GetEquipWeapon();
        if (weapon.GetId() == id && weapon.GetSubId() == subId)
        {
            PopUp2::Get()->SetText(IDS_STRING200);
            return;
        }
    }

    Common::Inventory()->RemoveItem(id, subId);

    int brokenBagNum1 = 0;
    {
        auto allBag = Common::Status()->GetAllBag();
        brokenBagNum1 = (int)std::count_if(allBag.begin(), allBag.end(),
                                           [&](auto x)
                                           {
                                               return x.GetDurabilityCurrent() == 0;
                                           });
    }

    Common::ReduceBrainStaminaCurrent(0.1f);
    Common::Inventory()->ReduceEquipBagDurability();

    m_menu.DeleteItem(id, subId);

    // 耐久度が下がったので更新
    {
        auto allBag = Common::Status()->GetAllBag();
        for (auto it = allBag.begin(); it != allBag.end(); ++it)
        {
            if (it->GetId() == -1)
            {
                continue;
            }

            // NSMenulib::ItemInfoとNSStarmanLib::ItemInfoの変換
            // レベルと装備状態と耐久度だけでいい
            NSMenulib::ItemInfo itemInfo;
            itemInfo.SetId(it->GetId());
            itemInfo.SetSubId(it->GetSubId());
            itemInfo.SetLevel(it->GetItemDef().GetLevel());
            itemInfo.SetEquip(true);
            itemInfo.SetDurability(it->GetDurabilityCurrent());
            m_menu.UpdateItem(itemInfo);
        }
    }

    // 新たに耐久値が0になった袋があったか
    int brokenBagNum2 = 0;
    {
        auto allBag = Common::Status()->GetAllBag();
        brokenBagNum2 = (int)std::count_if(allBag.begin(), allBag.end(),
                                           [&](auto x)
                                           {
                                               return x.GetDurabilityCurrent() == 0;
                                           });
    }

    if (brokenBagNum1 != brokenBagNum2)
    {
        PopUp2::Get()->SetText(IDS_STRING145);
    }

    m_menu.SetWeightAll(Common::Inventory()->GetWeight());
    m_menu.SetVolumeAll((int)Common::Inventory()->GetVolume());
    m_menu.SetVolumeMax((int)Common::Inventory()->GetVolumeMax());
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

    // 画像ファイル名を取得して設定
    // アイテム種別が武器の時は武器クラスから取得する必要がある
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

    // ID : 27 ~ 32 == 袋
    if (itemDef.GetId() == 27 ||
        itemDef.GetId() == 28 ||
        itemDef.GetId() == 29 ||
        itemDef.GetId() == 30 ||
        itemDef.GetId() == 31 ||
        itemDef.GetId() == 32)
    {
        itemInfoG.SetEquipEnable(true);

        if (IsBagEquiped(id, subId))
        {
            itemInfoG.SetEquip(true);
        }
    }
    else if (itemDef.GetType() == NSStarmanLib::ItemDef::ItemType::WEAPON)
    {
        itemInfoG.SetEquipEnable(true);

        auto weapon = Common::Status()->GetEquipWeapon();
        if (weapon.GetId() == id &&
            weapon.GetSubId() == subId)
        {
            itemInfoG.SetEquip(true);
        }
    }

    itemInfoG.SetDetail(work_str);

    itemInfoG.SetWeight(itemDef.GetWeight());
    itemInfoG.SetVolume((int)itemDef.GetVolume());

    Common::ReduceBrainStaminaCurrent(0.1f);

    int brokenBagNum1 = 0;
    {
        auto allBag = Common::Status()->GetAllBag();
        brokenBagNum1 = (int)std::count_if(allBag.begin(), allBag.end(),
                                           [&](auto x)
                                           {
                                               return x.GetDurabilityCurrent() == 0;
                                           });
    }

    Common::Inventory()->ReduceEquipBagDurability();

    m_menu.AddItem(itemInfoG);

    // 耐久値が下がったので更新
    {
        auto allBag = Common::Status()->GetAllBag();
        for (auto it = allBag.begin(); it != allBag.end(); ++it)
        {
            if (it->GetId() == -1)
            {
                continue;
            }

            // NSMenulib::ItemInfoとNSStarmanLib::ItemInfoの変換
            // レベルと装備状態と耐久度だけでいい
            NSMenulib::ItemInfo itemInfo;
            itemInfo.SetId(it->GetId());
            itemInfo.SetSubId(it->GetSubId());
            itemInfo.SetLevel(it->GetItemDef().GetLevel());
            itemInfo.SetEquip(true);
            itemInfo.SetDurability(it->GetDurabilityCurrent());
            m_menu.UpdateItem(itemInfo);
        }
    }

    // 新たに耐久値が0になった袋があったか
    int brokenBagNum2 = 0;
    {
        auto allBag = Common::Status()->GetAllBag();
        brokenBagNum2 = (int)std::count_if(allBag.begin(), allBag.end(),
                                           [&](auto x)
                                           {
                                               return x.GetDurabilityCurrent() == 0;
                                           });
    }

    if (brokenBagNum1 != brokenBagNum2)
    {
        PopUp2::Get()->SetText(IDS_STRING145);
    }

    m_menu.SetWeightAll(Common::Inventory()->GetWeight());
    m_menu.SetVolumeAll((int)Common::Inventory()->GetVolume());
    m_menu.SetVolumeMax((int)Common::Inventory()->GetVolumeMax());
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

void MenuManager::Equip(const int id, const int subId)
{
    //--------------------------------------
    // 袋か武器か
    //--------------------------------------

    // 武器だったら
    if (Common::ItemManager()->GetItemDef(id).GetType() == NSStarmanLib::ItemDef::ItemType::WEAPON)
    {
        //--------------------------------------
        // すでに装備していたものがあったら、その装備状態は解除する
        // ただし、装備していたものが袋や松明の場合、やや複雑になる。
        // 袋を装備していた場合、武器を装備しても袋は装備したままでよい。
        // しかし、右手に袋を装備していた場合、武器を装備ができないようにする。
        //--------------------------------------
        {
            bool lit = NSStarmanLib::WeaponManager::GetObj()->IsTorchLit();
            if (lit)
            {
                PopUp2::Get()->SetText(IDS_STRING198);
                return;
            }

            if (Common::Status()->GetBagState().size() >= 5)
            {
                PopUp2::Get()->SetText("右手がふさがっている。");
                return;
            }

            auto weapon = Common::Status()->GetEquipWeapon();
            if (weapon.GetId() != -1)
            {
                Unequip(weapon.GetId(), weapon.GetSubId());
            }
        }

        auto itemInfo = Common::Inventory()->GetItemInfo(id, subId);
        Common::Status()->SetEquipWeapon(itemInfo);

        NSMenulib::ItemInfo itemInfoG;
        itemInfoG.SetId(id);
        itemInfoG.SetSubId(subId);
        itemInfoG.SetDurability(itemInfo.GetDurabilityCurrent());
        itemInfoG.SetLevel(itemInfo.GetItemDef().GetLevel());
        itemInfoG.SetEquip(true);
        m_menu.UpdateItem(itemInfoG);

    }
    // 袋だったら
    else
    {
        // 袋を5個装備していたら装備しない
        if (Common::Status()->GetBagState().size() >= 5)
        {
            PopUp2::Get()->SetText(IDS_STRING202);
            return;
        }

        auto itemInfo = Common::Inventory()->GetItemInfo(id, subId);
        Common::Status()->EquipBag(itemInfo);

        NSMenulib::ItemInfo itemInfoG;
        itemInfoG.SetId(id);
        itemInfoG.SetSubId(subId);
        itemInfoG.SetDurability(itemInfo.GetDurabilityCurrent());
        itemInfoG.SetLevel(itemInfo.GetItemDef().GetLevel());
        itemInfoG.SetEquip(true);
        m_menu.UpdateItem(itemInfoG);

        m_menu.SetWeightAll(Common::Inventory()->GetWeight());
        m_menu.SetVolumeAll((int)Common::Inventory()->GetVolume());
        m_menu.SetVolumeMax((int)Common::Inventory()->GetVolumeMax());
    }
}

void MenuManager::Unequip(const int id, const int subId)
{
    auto itemInfo = Common::Inventory()->GetItemInfo(id, subId);
    NSMenulib::ItemInfo itemInfoG;
    itemInfoG.SetId(id);
    itemInfoG.SetSubId(subId);
    itemInfoG.SetDurability(itemInfo.GetDurabilityCurrent());
    itemInfoG.SetLevel(itemInfo.GetItemDef().GetLevel());
    itemInfoG.SetEquip(false);
    m_menu.UpdateItem(itemInfoG);

    if (Common::ItemManager()->GetItemDef(id).GetType() == NSStarmanLib::ItemDef::ItemType::WEAPON)
    {
        NSStarmanLib::ItemInfo itemInfo;
        itemInfo.SetId(-1);
        Common::Status()->SetEquipWeapon(itemInfo);
    }
    else
    {
        Common::Status()->UnequipBag(id, subId);

        m_menu.SetWeightAll(Common::Inventory()->GetWeight());
        m_menu.SetVolumeAll((int)Common::Inventory()->GetVolume());
        m_menu.SetVolumeMax((int)Common::Inventory()->GetVolumeMax());
    }
}


