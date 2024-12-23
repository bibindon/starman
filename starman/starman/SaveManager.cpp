#include "SaveManager.h"
#include "SharedObj.h"

#include "../../StarmanLib/StarmanLib/StarmanLib/HumanInfoManager.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/MapInfoManager.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/ItemManager.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/Inventory.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/Storehouse.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/WeaponManager.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/EnemyInfoManager.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/SkillManager.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/StatusManager.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/Guide.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/PowereggDateTime.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/MapObjManager.h"

SaveManager* SaveManager::m_obj = nullptr;

SaveManager* SaveManager::Get()
{
    if (m_obj == nullptr)
    {
        m_obj = new SaveManager();
    }

    return m_obj;
}

void SaveManager::Destroy()
{
    delete m_obj;
    m_obj = nullptr;
}

std::string SaveManager::CreateOriginFilePath(const std::string& filename)
{
    std::string originDataPath;
    if (Common::DebugMode())
    {
        originDataPath = ORIGIN_DATA_PATH_DEBUG;
        originDataPath += filename;
    }
    else
    {
        originDataPath = ORIGIN_DATA_PATH;
        originDataPath += filename;
        originDataPath.replace(originDataPath.size() - 3, 3, "enc");
    }
    return originDataPath;
}

std::string SaveManager::CreateSaveFilePath(const std::string& filename)
{
    std::string saveDataPath;
    if (Common::DebugMode())
    {
        saveDataPath = SAVEDATA_PATH_DEBUG;
        saveDataPath += filename;
    }
    else
    {
        saveDataPath = SAVEDATA_PATH;
        saveDataPath += filename;
        saveDataPath.replace(saveDataPath.size() - 3, 3, "enc");
    }
    return saveDataPath;
}

void SaveManager::Save()
{
    // ƒtƒHƒ‹ƒ_‚ª‚È‚¯‚ê‚Îì‚é
    std::string savedir;
    if (Common::ReleaseMode())
    {
        savedir = "res\\script\\save";
    }
    else
    {
        savedir = "res\\script\\save_debug";
    }

    if (PathFileExists(savedir.c_str()) == FALSE)
    {
        BOOL result = CreateDirectory(savedir.c_str(), NULL);
        if (result == FALSE)
        {
            throw std::exception();
        }
    }

    NSStarmanLib::HumanInfoManager* him = NSStarmanLib::HumanInfoManager::GetObj();
    him->Save(CreateSaveFilePath("humanInfoSub.csv"), !Common::DebugMode());

    NSStarmanLib::MapInfoManager* mapManager = NSStarmanLib::MapInfoManager::GetObj();
    mapManager->Save(CreateSaveFilePath("mapInfo.csv"), !Common::DebugMode());

    NSStarmanLib::ItemManager* itemManager = NSStarmanLib::ItemManager::GetObj();
    itemManager->Save(CreateSaveFilePath("item_pos.csv"), !Common::DebugMode());

    NSStarmanLib::Inventory* inventory = NSStarmanLib::Inventory::GetObj();
    inventory->Save(CreateSaveFilePath("inventory.csv"), !Common::DebugMode());

    NSStarmanLib::Storehouse* storehouse = NSStarmanLib::Storehouse::GetObj();
    storehouse->Save(CreateSaveFilePath("storehouse.csv"), !Common::DebugMode());

    NSStarmanLib::WeaponManager* weaponManager = NSStarmanLib::WeaponManager::GetObj();
    weaponManager->Save(CreateSaveFilePath("weaponSave.csv"), !Common::DebugMode());

    NSStarmanLib::EnemyInfoManager* enemyInfoManager = NSStarmanLib::EnemyInfoManager::GetObj();
    enemyInfoManager->Save(CreateSaveFilePath("enemy.csv"),
                           CreateSaveFilePath("enemyVisible.csv"),
                           !Common::DebugMode());

    NSStarmanLib::SkillManager* skillManager = NSStarmanLib::SkillManager::GetObj();
    skillManager->Save(CreateSaveFilePath("skillSub.csv"), !Common::DebugMode());

    NSStarmanLib::StatusManager* statusManager = NSStarmanLib::StatusManager::GetObj();
    statusManager->Save(CreateSaveFilePath("status.csv"),
                        SharedObj::GetPlayer()->GetPos().x,
                        SharedObj::GetPlayer()->GetPos().y,
                        SharedObj::GetPlayer()->GetPos().z,
                        !Common::DebugMode());

    NSStarmanLib::Guide* guide = NSStarmanLib::Guide::GetObj();
    guide->Save(CreateSaveFilePath("guide.csv"), !Common::DebugMode());

    NSStarmanLib::PowereggDateTime* datetime = NSStarmanLib::PowereggDateTime::GetObj();
    datetime->Save(CreateSaveFilePath("datetime.csv"), !Common::DebugMode());

    NSStarmanLib::MapObjManager* mapObjManager = NSStarmanLib::MapObjManager::GetObj();
    mapObjManager->Save(CreateSaveFilePath("map_obj.csv"), !Common::DebugMode());
}

void SaveManager::LoadOrigin()
{
    if (m_savedataLoaded)
    {
        NSStarmanLib::HumanInfoManager::Destroy();
        NSStarmanLib::MapInfoManager::Destroy();
        NSStarmanLib::ItemManager::Destroy();
        NSStarmanLib::Inventory::Destroy();
        NSStarmanLib::Storehouse::Destroy();
        NSStarmanLib::WeaponManager::Destroy();
        NSStarmanLib::EnemyInfoManager::Destroy();
        NSStarmanLib::SkillManager::Destroy();
        NSStarmanLib::StatusManager::Destroy();
        NSStarmanLib::Guide::Destroy();
        NSStarmanLib::PowereggDateTime::Destroy();
        NSStarmanLib::MapObjManager::Destroy();
    }

    NSStarmanLib::HumanInfoManager* him = NSStarmanLib::HumanInfoManager::GetObj();
    him->Init(CreateOriginFilePath("humanInfo.csv"),
              CreateOriginFilePath("humanInfoSub.csv"),
              !Common::DebugMode());

    NSStarmanLib::MapInfoManager* mapManager = NSStarmanLib::MapInfoManager::GetObj();
    mapManager->Init(CreateOriginFilePath("mapInfo.csv"), !Common::DebugMode());

    NSStarmanLib::ItemManager* itemManager = NSStarmanLib::ItemManager::GetObj();
    itemManager->Init(CreateOriginFilePath("item.csv"),
                      CreateOriginFilePath("item_pos.csv"),
                      !Common::DebugMode());

    NSStarmanLib::Inventory* inventory = NSStarmanLib::Inventory::GetObj();
    inventory->Init(CreateOriginFilePath("inventory.csv"), !Common::DebugMode());

    NSStarmanLib::Storehouse* storehouse = NSStarmanLib::Storehouse::GetObj();
    storehouse->Init(CreateOriginFilePath("storehouse.csv"), !Common::DebugMode());

    NSStarmanLib::WeaponManager* weaponManager = NSStarmanLib::WeaponManager::GetObj();
    weaponManager->Init(CreateOriginFilePath("weapon.csv"),
                        CreateOriginFilePath("weaponSave.csv"),
                        !Common::DebugMode());

    NSStarmanLib::EnemyInfoManager* enemyInfoManager = NSStarmanLib::EnemyInfoManager::GetObj();
    enemyInfoManager->Init(CreateOriginFilePath("enemyDef.csv"),
                           CreateOriginFilePath("enemy.csv"),
                           CreateOriginFilePath("enemyVisible.csv"),
                           !Common::DebugMode());
            
        
    NSStarmanLib::SkillManager* skillManager = NSStarmanLib::SkillManager::GetObj();
    skillManager->Init(CreateOriginFilePath("skill.csv"),
                       CreateOriginFilePath("skillSub.csv"),
                        !Common::DebugMode());
            
    NSStarmanLib::StatusManager* statusManager = NSStarmanLib::StatusManager::GetObj();
    statusManager->Init(CreateOriginFilePath("status.csv"), !Common::DebugMode());

    NSStarmanLib::Guide* guide = NSStarmanLib::Guide::GetObj();
    guide->Init(CreateOriginFilePath("guide.csv"), !Common::DebugMode());

    NSStarmanLib::PowereggDateTime* datetime = NSStarmanLib::PowereggDateTime::GetObj();
    datetime->Init(CreateOriginFilePath("datetime.csv"), !Common::DebugMode());

    NSStarmanLib::MapObjManager* mapObjManager = NSStarmanLib::MapObjManager::GetObj();
    mapObjManager->Init(CreateOriginFilePath("map_obj.csv"), !Common::DebugMode());
}

void SaveManager::Load()
{
    NSStarmanLib::HumanInfoManager* him = NSStarmanLib::HumanInfoManager::GetObj();
    him->Init(CreateOriginFilePath("humanInfo.csv"),
              CreateSaveFilePath("humanInfoSub.csv"),
              !Common::DebugMode());

    NSStarmanLib::MapInfoManager* mapManager = NSStarmanLib::MapInfoManager::GetObj();
    mapManager->Init(CreateSaveFilePath("mapInfo.csv"), !Common::DebugMode());

    NSStarmanLib::ItemManager* itemManager = NSStarmanLib::ItemManager::GetObj();
    itemManager->Init(CreateOriginFilePath("item.csv"),
                      CreateSaveFilePath("item_pos.csv"),
                      !Common::DebugMode());

    NSStarmanLib::Inventory* inventory = NSStarmanLib::Inventory::GetObj();
    inventory->Init(CreateSaveFilePath("inventory.csv"), !Common::DebugMode());

    NSStarmanLib::Storehouse* storehouse = NSStarmanLib::Storehouse::GetObj();
    storehouse->Init(CreateSaveFilePath("storehouse.csv"), !Common::DebugMode());

    NSStarmanLib::WeaponManager* weaponManager = NSStarmanLib::WeaponManager::GetObj();
    weaponManager->Init(CreateOriginFilePath("weapon.csv"),
                        CreateSaveFilePath("weaponSave.csv"),
                        !Common::DebugMode());

    NSStarmanLib::EnemyInfoManager* enemyInfoManager = NSStarmanLib::EnemyInfoManager::GetObj();
    enemyInfoManager->Init(CreateOriginFilePath("enemyDef.csv"),
                           CreateSaveFilePath("enemy.csv"),
                           CreateSaveFilePath("enemyVisible.csv"),
                           !Common::DebugMode());
            
        
    NSStarmanLib::SkillManager* skillManager = NSStarmanLib::SkillManager::GetObj();
    skillManager->Init(CreateOriginFilePath("skill.csv"),
                       CreateSaveFilePath("skillSub.csv"),
                        !Common::DebugMode());
            
    NSStarmanLib::StatusManager* statusManager = NSStarmanLib::StatusManager::GetObj();
    statusManager->Init(CreateSaveFilePath("status.csv"), !Common::DebugMode());

    NSStarmanLib::Guide* guide = NSStarmanLib::Guide::GetObj();
    guide->Init(CreateSaveFilePath("guide.csv"), !Common::DebugMode());

    NSStarmanLib::PowereggDateTime* datetime = NSStarmanLib::PowereggDateTime::GetObj();
    datetime->Init(CreateSaveFilePath("datetime.csv"), !Common::DebugMode());

    NSStarmanLib::MapObjManager* mapObjManager = NSStarmanLib::MapObjManager::GetObj();
    mapObjManager->Init(CreateSaveFilePath("map_obj.csv"), !Common::DebugMode());

    m_savedataLoaded = true;
}

void SaveManager::DeleteSavedata()
{
}
