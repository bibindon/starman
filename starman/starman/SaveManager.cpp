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
#include "../../StarmanLib/StarmanLib/StarmanLib/StatusManager.h"
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
    if (SharedObj::DebugMode())
    {
        originDataPath = ORIGIN_DATA_PATH_DEBUG;
        originDataPath += filename;
    }
    else
    {
        originDataPath = ORIGIN_DATA_PATH;
        originDataPath += filename + ".enc";
    }
    return originDataPath;
}

std::string SaveManager::CreateSaveFilePath(const std::string& filename)
{
    std::string saveDataPath;
    if (SharedObj::DebugMode())
    {
        saveDataPath = SAVEDATA_PATH_DEBUG;
        saveDataPath += filename;
    }
    else
    {
        saveDataPath = SAVEDATA_PATH;
        saveDataPath += filename + ".enc";
    }
    return saveDataPath;
}

void SaveManager::Save()
{

    NSStarmanLib::HumanInfoManager* him = NSStarmanLib::HumanInfoManager::GetObj();
    him->Save(CreateSaveFilePath("humanInfoSub.csv"), !SharedObj::DebugMode());

    NSStarmanLib::MapInfoManager* mapManager = NSStarmanLib::MapInfoManager::GetObj();
    mapManager->Save(CreateSaveFilePath("mapInfo.csv"), !SharedObj::DebugMode());

    NSStarmanLib::ItemManager* itemManager = NSStarmanLib::ItemManager::GetObj();
    itemManager->Save(CreateSaveFilePath("item_pos.csv"), !SharedObj::DebugMode());

    NSStarmanLib::Inventory* inventory = NSStarmanLib::Inventory::GetObj();
    inventory->Save(CreateSaveFilePath("inventory.csv"), !SharedObj::DebugMode());

    NSStarmanLib::Storehouse* storehouse = NSStarmanLib::Storehouse::GetObj();
    storehouse->Save(CreateSaveFilePath("storehouse.csv"), !SharedObj::DebugMode());

    NSStarmanLib::WeaponManager* weaponManager = NSStarmanLib::WeaponManager::GetObj();
    weaponManager->Save(CreateSaveFilePath("weaponSave.csv"), !SharedObj::DebugMode());

    NSStarmanLib::EnemyInfoManager* enemyInfoManager = NSStarmanLib::EnemyInfoManager::GetObj();
    enemyInfoManager->Save(CreateSaveFilePath("enemy.csv"),
                           CreateSaveFilePath("enemyVisible.csv"),
                           !SharedObj::DebugMode());

    NSStarmanLib::SkillManager* skillManager = NSStarmanLib::SkillManager::GetObj();
    skillManager->Save(CreateSaveFilePath("skillSub.csv"), !SharedObj::DebugMode());

    NSStarmanLib::StatusManager* statusManager = NSStarmanLib::StatusManager::GetObj();
    statusManager->Save(CreateSaveFilePath("status.csv"),
                        SharedObj::GetPlayer()->GetPos().x,
                        SharedObj::GetPlayer()->GetPos().y,
                        SharedObj::GetPlayer()->GetPos().z,
                        !SharedObj::DebugMode());

    NSStarmanLib::Guide* guide = NSStarmanLib::Guide::GetObj();
    guide->Save(CreateSaveFilePath("guide.csv"), !SharedObj::DebugMode());

    NSStarmanLib::PowereggDateTime* datetime = NSStarmanLib::PowereggDateTime::GetObj();
    datetime->Save(CreateSaveFilePath("datetime.csv"), !SharedObj::DebugMode());

    NSStarmanLib::MapObjManager* mapObjManager = NSStarmanLib::MapObjManager::GetObj();
    mapObjManager->Save(CreateSaveFilePath("map_obj.csv"), !SharedObj::DebugMode());
}

void SaveManager::LoadOrigin()
{
    NSStarmanLib::HumanInfoManager* him = NSStarmanLib::HumanInfoManager::GetObj();
    him->Init(CreateOriginFilePath("humanInfo.csv"),
              CreateOriginFilePath("humanInfoSub.csv"),
              !SharedObj::DebugMode());

    NSStarmanLib::MapInfoManager* mapManager = NSStarmanLib::MapInfoManager::GetObj();
    mapManager->Init(CreateOriginFilePath("mapInfo.csv"), !SharedObj::DebugMode());

    NSStarmanLib::ItemManager* itemManager = NSStarmanLib::ItemManager::GetObj();
    itemManager->Init(CreateOriginFilePath("item.csv"),
                      CreateOriginFilePath("item_pos.csv"),
                      !SharedObj::DebugMode());

    NSStarmanLib::Inventory* inventory = NSStarmanLib::Inventory::GetObj();
    inventory->Init(CreateOriginFilePath("inventory.csv"), !SharedObj::DebugMode());

    NSStarmanLib::Storehouse* storehouse = NSStarmanLib::Storehouse::GetObj();
    storehouse->Init(CreateOriginFilePath("storehouse.csv"), !SharedObj::DebugMode());

    NSStarmanLib::WeaponManager* weaponManager = NSStarmanLib::WeaponManager::GetObj();
    weaponManager->Init(CreateOriginFilePath("weapon.csv"),
                        CreateOriginFilePath("weaponSave.csv"),
                        !SharedObj::DebugMode());

    NSStarmanLib::EnemyInfoManager* enemyInfoManager = NSStarmanLib::EnemyInfoManager::GetObj();
    enemyInfoManager->Init(CreateOriginFilePath("enemyDef.csv"),
                           CreateOriginFilePath("enemy.csv"),
                           CreateOriginFilePath("enemyVisible.csv"),
                           !SharedObj::DebugMode());
            
        
    NSStarmanLib::SkillManager* skillManager = NSStarmanLib::SkillManager::GetObj();
    skillManager->Init(CreateOriginFilePath("skill.csv"),
                       CreateOriginFilePath("skillSub.csv"),
                        !SharedObj::DebugMode());
            
    NSStarmanLib::StatusManager* statusManager = NSStarmanLib::StatusManager::GetObj();
    statusManager->Init(CreateOriginFilePath("status.csv"), !SharedObj::DebugMode());

    NSStarmanLib::Guide* guide = NSStarmanLib::Guide::GetObj();
    guide->Init(CreateOriginFilePath("guide.csv"), !SharedObj::DebugMode());

    NSStarmanLib::PowereggDateTime* datetime = NSStarmanLib::PowereggDateTime::GetObj();
    datetime->Init(CreateOriginFilePath("datetime.csv"), !SharedObj::DebugMode());

    NSStarmanLib::MapObjManager* mapObjManager = NSStarmanLib::MapObjManager::GetObj();
    mapObjManager->Init(CreateOriginFilePath("map_obj.csv"), !SharedObj::DebugMode());
}

void SaveManager::Load()
{
    NSStarmanLib::HumanInfoManager* him = NSStarmanLib::HumanInfoManager::GetObj();
    him->Init(CreateSaveFilePath("humanInfo.csv"),
              CreateSaveFilePath("humanInfoSub.csv"),
              !SharedObj::DebugMode());

    NSStarmanLib::MapInfoManager* mapManager = NSStarmanLib::MapInfoManager::GetObj();
    mapManager->Init(CreateSaveFilePath("mapInfo.csv"), !SharedObj::DebugMode());

    NSStarmanLib::ItemManager* itemManager = NSStarmanLib::ItemManager::GetObj();
    itemManager->Init(CreateSaveFilePath("item.csv"),
                      CreateSaveFilePath("item_pos.csv"),
                      !SharedObj::DebugMode());

    NSStarmanLib::Inventory* inventory = NSStarmanLib::Inventory::GetObj();
    inventory->Init(CreateSaveFilePath("inventory.csv"), !SharedObj::DebugMode());

    NSStarmanLib::Storehouse* storehouse = NSStarmanLib::Storehouse::GetObj();
    storehouse->Init(CreateSaveFilePath("storehouse.csv"), !SharedObj::DebugMode());

    NSStarmanLib::WeaponManager* weaponManager = NSStarmanLib::WeaponManager::GetObj();
    weaponManager->Init(CreateSaveFilePath("weapon.csv"),
                        CreateSaveFilePath("weaponSave.csv"),
                        !SharedObj::DebugMode());

    NSStarmanLib::EnemyInfoManager* enemyInfoManager = NSStarmanLib::EnemyInfoManager::GetObj();
    enemyInfoManager->Init(CreateSaveFilePath("enemyDef.csv"),
                           CreateSaveFilePath("enemy.csv"),
                           CreateSaveFilePath("enemyVisible.csv"),
                           !SharedObj::DebugMode());
            
        
    NSStarmanLib::SkillManager* skillManager = NSStarmanLib::SkillManager::GetObj();
    skillManager->Init(CreateSaveFilePath("skill.csv"),
                       CreateSaveFilePath("skillSub.csv"),
                        !SharedObj::DebugMode());
            
    NSStarmanLib::StatusManager* statusManager = NSStarmanLib::StatusManager::GetObj();
    statusManager->Init(CreateSaveFilePath("status.csv"), !SharedObj::DebugMode());

    NSStarmanLib::Guide* guide = NSStarmanLib::Guide::GetObj();
    guide->Init(CreateSaveFilePath("guide.csv"), !SharedObj::DebugMode());

    NSStarmanLib::PowereggDateTime* datetime = NSStarmanLib::PowereggDateTime::GetObj();
    datetime->Init(CreateSaveFilePath("datetime.csv"), !SharedObj::DebugMode());

    NSStarmanLib::MapObjManager* mapObjManager = NSStarmanLib::MapObjManager::GetObj();
    mapObjManager->Init(CreateSaveFilePath("map_obj.csv"), !SharedObj::DebugMode());
}
