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
        // TODO スプライトもテクスチャも使いまわしているのでデストラクタで解放処理ができない。
        // いずれ考えることとする
//        if (m_D3DSprite != NULL)
//        {
//            m_D3DSprite->Release();
//            m_D3DSprite = NULL;
//        }
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
    sprBackground->Load("res\\image\\menu_back.png");

    NSMenulib::IFont* pFont = new NSMenulib::Font(SharedObj::GetD3DDevice());
    pFont->Init();

    NSMenulib::ISoundEffect* pSE = new NSMenulib::SoundEffect();
    pSE->Init();

    m_menu.Init("", pFont, pSE, sprCursor, sprBackground);

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

                    NSStarmanLib::ItemInfo itemInfo = inventory->GetItemInfo(idList.at(i), subIdList.at(j));

                    itemInfoG.SetDurability(itemInfo.GetDurabilityCurrent());

                    NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());

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
                    itemInfoList.push_back(itemInfoG);
                }
            }
        }
        m_menu.SetItem(itemInfoList);
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

            NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
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

            NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
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

            NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
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
        std::vector<StatusInfo> infoList;

        NSStarmanLib::StatusManager* statusManager = NSStarmanLib::StatusManager::GetObj();
        StatusInfo info;
        info.SetName("ホシマン");
        NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
        sprItem->Load("res\\image\\test.png");
        info.SetSprite(sprItem);
        std::string work;
        work = "パラメータ　　　現在値/回復可能値/最大値\n";
        work += "体のスタミナ:";
        work += Common::ToStringWithPrecision(statusManager->GetBodyStaminaCurrent()) + "/";
        work += Common::ToStringWithPrecision(statusManager->GetBodyStaminaMaxSub()) + "/";
        work += Common::ToStringWithPrecision(statusManager->GetBodyStaminaMax()) + "\n";
        work += "脳のスタミナ:";
        work += Common::ToStringWithPrecision(statusManager->GetBrainStaminaCurrent()) + "/";
        work += Common::ToStringWithPrecision(statusManager->GetBrainStaminaMaxSub()) + "/";
        work += Common::ToStringWithPrecision(statusManager->GetBrainStaminaMax()) + "\n";
        work += "瞬発力:";
        work += Common::ToStringWithPrecision(statusManager->GetExplosivePower()) + "/-/-\n";
        work += "肉体の修復度:";
        work += Common::ToStringWithPrecision(statusManager->GetMuscleCurrent()) + "/-/";
        work += Common::ToStringWithPrecision(statusManager->GetMuscleMax()) + "\n";
        work += "糖質:";
        work += Common::ToStringWithPrecision(statusManager->GetCarboCurrent()) + "/-/";
        work += Common::ToStringWithPrecision(statusManager->GetCarboMax()) + "\n";
        work += "タンパク質:";
        work += Common::ToStringWithPrecision(statusManager->GetProteinCurrent()) + "/-/";
        work += Common::ToStringWithPrecision(statusManager->GetProteinMax()) + "\n";
        work += "脂質:";
        work += Common::ToStringWithPrecision(statusManager->GetLipidCurrent()) + "/-/";
        work += Common::ToStringWithPrecision(statusManager->GetLipidMax()) + "\n";
        work += "ビタミン:";
        work += Common::ToStringWithPrecision(statusManager->GetVitaminCurrent()) + "/-/";
        work += Common::ToStringWithPrecision(statusManager->GetVitaminMax()) + "\n";
        work += "ミネラル:";
        work += Common::ToStringWithPrecision(statusManager->GetMineralCurrent()) + "/-/";
        work += Common::ToStringWithPrecision(statusManager->GetMineralMax()) + "\n";
        work += "水分:";
        work += Common::ToStringWithPrecision(statusManager->GetWaterCurrent()) + "/-/";
        work += Common::ToStringWithPrecision(statusManager->GetWaterMax()) + "\n";
        work += "\n";
        work += "状態異常：";
        work += "頭痛/腹痛/風邪/腕骨折/足骨折/脱水症状/寝不足";
        work += "\n";
        work += "装備武器：";
        work += "\n";
        work += "攻撃力：";
        work += Common::ToStringWithPrecision(statusManager->GetAttackPower()) + "\n";
        work += "防御力：";
        work += Common::ToStringWithPrecision(statusManager->GetDefensePower()) + "\n";
        info.SetDetail(work);
        infoList.push_back(info);
        m_menu.SetStatus(infoList);
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
        if (vs.size() == 5 && vs.at(0) == "アイテム")
        {
            // アイテムを使う
            if (vs.at(4) == "使う")
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

            // アイテムを捨てる
            if (vs.at(4) == "捨てる")
            {
                int id = std::stoi(vs.at(2));
                int subId = std::stoi(vs.at(3));

                NSStarmanLib::Inventory* inventory = NSStarmanLib::Inventory::GetObj();
                inventory->RemoveItem(id, subId);
                m_menu.DeleteItem(id, subId);
            }
        }
        else if (vs.size() == 5 && vs.at(0) == "武器")
        {
            if (vs.at(4) == "装備")
            {
                int id = std::stoi(vs.at(2));
                int subId = std::stoi(vs.at(3));

                NSStarmanLib::Inventory* inventory = NSStarmanLib::Inventory::GetObj();
                NSStarmanLib::ItemInfo itemInfo = inventory->GetItemInfo(id, subId);

                NSStarmanLib::StatusManager* statusManager = NSStarmanLib::StatusManager::GetObj();
                statusManager->SetEquipWeapon(itemInfo);
            }
        }
        else if (vs.size() >= 1 && vs.at(0) == "セーブして終了")
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

    if (Mouse::IsDownLeft())
    {
        POINT p = Common::GetScreenPos();;
        result = m_menu.Click(p.x, p.y);

        std::vector<std::string> vs = Common::split(result, ':');
        if (vs.size() == 5 && vs.at(0) == "アイテム")
        {
            // アイテムを使う
            if (vs.at(4) == "使う")
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

            // アイテムを捨てる
            if (vs.at(4) == "捨てる")
            {
                int id = std::stoi(vs.at(2));
                int subId = std::stoi(vs.at(3));

                NSStarmanLib::Inventory* inventory = NSStarmanLib::Inventory::GetObj();
                inventory->RemoveItem(id, subId);
                m_menu.DeleteItem(id, subId);
            }
        }
        else if (vs.size() == 5 && vs.at(0) == "武器")
        {
            if (vs.at(4) == "装備")
            {
                int id = std::stoi(vs.at(2));
                int subId = std::stoi(vs.at(3));

                NSStarmanLib::Inventory* inventory = NSStarmanLib::Inventory::GetObj();
                NSStarmanLib::ItemInfo itemInfo = inventory->GetItemInfo(id, subId);

                NSStarmanLib::StatusManager* statusManager = NSStarmanLib::StatusManager::GetObj();
                statusManager->SetEquipWeapon(itemInfo);
            }
        }
        else if (vs.size() >= 1 && vs.at(0) == "セーブして終了")
        {
            auto saveManager = SaveManager::Get();
            saveManager->Save();
            PostMessage(SharedObj::GetWindowHandle(), WM_CLOSE, 0, 0);
        }
    }
    else
    {
        POINT p = Common::GetScreenPos();;
        m_menu.CursorOn(p.x, p.y);
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
        if (vs.size() == 5 && vs.at(0) == "アイテム")
        {
            // アイテムを使う
            if (vs.at(4) == "使う")
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

            // アイテムを捨てる
            if (vs.at(4) == "捨てる")
            {
                int id = std::stoi(vs.at(2));
                int subId = std::stoi(vs.at(3));

                NSStarmanLib::Inventory* inventory = NSStarmanLib::Inventory::GetObj();
                inventory->RemoveItem(id, subId);
                m_menu.DeleteItem(id, subId);
            }
        }
        else if (vs.size() == 5 && vs.at(0) == "武器")
        {
            if (vs.at(4) == "装備")
            {
                int id = std::stoi(vs.at(2));
                int subId = std::stoi(vs.at(3));

                NSStarmanLib::Inventory* inventory = NSStarmanLib::Inventory::GetObj();
                NSStarmanLib::ItemInfo itemInfo = inventory->GetItemInfo(id, subId);

                NSStarmanLib::StatusManager* statusManager = NSStarmanLib::StatusManager::GetObj();
                statusManager->SetEquipWeapon(itemInfo);
            }
        }
        else if (vs.size() >= 1 && vs.at(0) == "セーブして終了")
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

    // 表示内容を更新
    {
        using namespace NSMenulib;
        std::vector<StatusInfo> infoList;

        NSStarmanLib::StatusManager* statusManager = NSStarmanLib::StatusManager::GetObj();
        StatusInfo info;
        info.SetName("ホシマン");
        NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());
        sprItem->Load("res\\image\\test.png");
        info.SetSprite(sprItem);
        std::string work;
        work = "パラメータ　　　現在値/回復可能値/最大値\n";
        work += "体のスタミナ:";
        work += Common::ToStringWithPrecision(statusManager->GetBodyStaminaCurrent()) + "/";
        work += Common::ToStringWithPrecision(statusManager->GetBodyStaminaMaxSub()) + "/";
        work += Common::ToStringWithPrecision(statusManager->GetBodyStaminaMax()) + "\n";
        work += "脳のスタミナ:";
        work += Common::ToStringWithPrecision(statusManager->GetBrainStaminaCurrent()) + "/";
        work += Common::ToStringWithPrecision(statusManager->GetBrainStaminaMaxSub()) + "/";
        work += Common::ToStringWithPrecision(statusManager->GetBrainStaminaMax()) + "\n";
        work += "瞬発力:";
        work += Common::ToStringWithPrecision(statusManager->GetExplosivePower()) + "/-/-\n";
        work += "肉体の修復度:";
        work += Common::ToStringWithPrecision(statusManager->GetMuscleCurrent()) + "/-/";
        work += Common::ToStringWithPrecision(statusManager->GetMuscleMax()) + "\n";
        work += "糖質:";
        work += Common::ToStringWithPrecision(statusManager->GetCarboCurrent()) + "/-/";
        work += Common::ToStringWithPrecision(statusManager->GetCarboMax()) + "\n";
        work += "タンパク質:";
        work += Common::ToStringWithPrecision(statusManager->GetProteinCurrent()) + "/-/";
        work += Common::ToStringWithPrecision(statusManager->GetProteinMax()) + "\n";
        work += "脂質:";
        work += Common::ToStringWithPrecision(statusManager->GetLipidCurrent()) + "/-/";
        work += Common::ToStringWithPrecision(statusManager->GetLipidMax()) + "\n";
        work += "ビタミン:";
        work += Common::ToStringWithPrecision(statusManager->GetVitaminCurrent()) + "/-/";
        work += Common::ToStringWithPrecision(statusManager->GetVitaminMax()) + "\n";
        work += "ミネラル:";
        work += Common::ToStringWithPrecision(statusManager->GetMineralCurrent()) + "/-/";
        work += Common::ToStringWithPrecision(statusManager->GetMineralMax()) + "\n";
        work += "水分:";
        work += Common::ToStringWithPrecision(statusManager->GetWaterCurrent()) + "/-/";
        work += Common::ToStringWithPrecision(statusManager->GetWaterMax()) + "\n";
        work += "\n";
        work += "状態異常：";
        work += "頭痛/腹痛/風邪/腕骨折/足骨折/脱水症状/寝不足";
        work += "\n";
        work += "装備武器：";
        work += "\n";
        work += "攻撃力：";
        work += Common::ToStringWithPrecision(statusManager->GetAttackPower()) + "\n";
        work += "防御力：";
        work += Common::ToStringWithPrecision(statusManager->GetDefensePower()) + "\n";
        info.SetDetail(work);
        infoList.push_back(info);
        m_menu.SetStatus(infoList);
    }
    return result;
}

bool MenuManager::UseItem(const int id, const int subId)
{
    // 食材だったらステータスを更新
    // TODO 食材以外はあとで考える
    NSStarmanLib::ItemManager* itemManager = NSStarmanLib::ItemManager::GetObj();
    NSStarmanLib::ItemDef itemDef = itemManager->GetItemDef(id);

    if (itemDef.GetType() != NSStarmanLib::ItemDef::ItemType::FOOD)
    {
        return false;
    }

    NSStarmanLib::StatusManager* statusManager = NSStarmanLib::StatusManager::GetObj();

    // 満腹だったらfalseが返ってくる
    return statusManager->Eat(itemDef);
}

void MenuManager::DeleteItem(const int id, const int subId)
{
    m_menu.DeleteItem(id, subId);
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

    NSMenulib::Sprite* sprItem = new NSMenulib::Sprite(SharedObj::GetD3DDevice());

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

    itemInfoG.SetDetail(work_str);
    m_menu.AddItem(itemInfoG);
}
