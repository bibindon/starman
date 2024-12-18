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

void SaveManager::Save()
{
	NSStarmanLib::HumanInfoManager* him = NSStarmanLib::HumanInfoManager::GetObj();
	if (SharedObj::DebugMode())
	{
		him->Save(SAVEDATA_PATH + "humanInfoSub.debug.csv");
	}
	else
	{
		him->Save(SAVEDATA_PATH + "humanInfoSub.csv");
	}

	NSStarmanLib::MapInfoManager* mapManager = NSStarmanLib::MapInfoManager::GetObj();
	if (SharedObj::DebugMode())
	{
		mapManager->Save(SAVEDATA_PATH + "mapInfo.debug.csv");
	}
	else
	{
		mapManager->Save(SAVEDATA_PATH + "mapInfo.csv");
	}

	NSStarmanLib::ItemManager* itemManager = NSStarmanLib::ItemManager::GetObj();
	itemManager->Save(SAVEDATA_PATH + "item_pos.csv");

	NSStarmanLib::Inventory* inventory = NSStarmanLib::Inventory::GetObj();

	if (SharedObj::DebugMode())
	{
		inventory->Save(SAVEDATA_PATH + "inventory.debug.csv");
	}
	else
	{
		inventory->Save(SAVEDATA_PATH + "inventory.csv");
	}

	NSStarmanLib::Storehouse* storehouse = NSStarmanLib::Storehouse::GetObj();

	if (SharedObj::DebugMode())
	{
		storehouse->Save(SAVEDATA_PATH + "storehouse.debug.csv");
	}
	else
	{
		storehouse->Save(SAVEDATA_PATH + "storehouse.csv");
	}

	NSStarmanLib::WeaponManager* weaponManager = NSStarmanLib::WeaponManager::GetObj();

	if (SharedObj::DebugMode())
	{
		weaponManager->Save(SAVEDATA_PATH + "weaponSave.debug.csv");
	}
	else
	{
		weaponManager->Save(SAVEDATA_PATH + "weaponSave.csv");
	}

	NSStarmanLib::EnemyInfoManager* enemyInfoManager = NSStarmanLib::EnemyInfoManager::GetObj();

	if (SharedObj::DebugMode())
	{
		enemyInfoManager->Save(SAVEDATA_PATH + "enemy.debug.csv",
							   SAVEDATA_PATH + "enemyVisible.debug.csv");
	}
	else
	{
		enemyInfoManager->Save(SAVEDATA_PATH + "enemy.csv",
							   SAVEDATA_PATH + "enemyVisible.csv");
	}

	NSStarmanLib::SkillManager* skillManager = NSStarmanLib::SkillManager::GetObj();

	if (SharedObj::DebugMode())
	{
		skillManager->Save(SAVEDATA_PATH + "skillSub.debug.csv");
	}
	else
	{
		skillManager->Save(SAVEDATA_PATH + "skillSub.csv");
	}

	NSStarmanLib::StatusManager* statusManager = NSStarmanLib::StatusManager::GetObj();

	if (SharedObj::DebugMode())
	{
		statusManager->Save(SAVEDATA_PATH + "status.debug.csv",
                            SharedObj::GetPlayer()->GetPos().x,
                            SharedObj::GetPlayer()->GetPos().y,
                            SharedObj::GetPlayer()->GetPos().z );
	}
	else
	{
		statusManager->Save(SAVEDATA_PATH + "status.csv",
                            SharedObj::GetPlayer()->GetPos().x,
                            SharedObj::GetPlayer()->GetPos().y,
                            SharedObj::GetPlayer()->GetPos().z );
	}

	NSStarmanLib::Guide* guide = NSStarmanLib::Guide::GetObj();

	if (SharedObj::DebugMode())
	{
		guide->Save(SAVEDATA_PATH + "guide.debug.csv");
	}
	else
	{
		guide->Save(SAVEDATA_PATH + "guide.csv");
	}

	NSStarmanLib::PowereggDateTime* datetime = NSStarmanLib::PowereggDateTime::GetObj();

	if (SharedObj::DebugMode())
	{
		datetime->Save(SAVEDATA_PATH + "datetime.debug.csv");
	}
	else
	{
		datetime->Save(SAVEDATA_PATH + "datetime.csv");
	}

	NSStarmanLib::MapObjManager* mapObjManager = NSStarmanLib::MapObjManager::GetObj();

	mapObjManager->Save(SAVEDATA_PATH + "map_obj.csv");
}

void SaveManager::LoadOrigin()
{
	NSStarmanLib::HumanInfoManager* him = NSStarmanLib::HumanInfoManager::GetObj();
	if (SharedObj::DebugMode())
	{
		him->Init(ORIGIN_DATA_PATH + "humanInfo.csv", ORIGIN_DATA_PATH + "humanInfoSub.debug.csv");
	}
	else
	{
		him->Init(ORIGIN_DATA_PATH + "humanInfo.csv", ORIGIN_DATA_PATH + "humanInfoSub.csv");
	}

	NSStarmanLib::MapInfoManager* mapManager = NSStarmanLib::MapInfoManager::GetObj();
	if (SharedObj::DebugMode())
	{
		mapManager->Init(ORIGIN_DATA_PATH + "mapInfo.debug.csv");
	}
	else
	{
		mapManager->Init(ORIGIN_DATA_PATH + "mapInfo.csv");
	}

	NSStarmanLib::ItemManager* itemManager = NSStarmanLib::ItemManager::GetObj();
	itemManager->Init(ORIGIN_DATA_PATH + "item.csv", ORIGIN_DATA_PATH + "item_pos.csv");

	NSStarmanLib::Inventory* inventory = NSStarmanLib::Inventory::GetObj();

	if (SharedObj::DebugMode())
	{
		inventory->Init(ORIGIN_DATA_PATH + "inventory.debug.csv");
	}
	else
	{
		inventory->Init(ORIGIN_DATA_PATH + "inventory.csv");
	}

	NSStarmanLib::Storehouse* storehouse = NSStarmanLib::Storehouse::GetObj();

	if (SharedObj::DebugMode())
	{
		storehouse->Init(ORIGIN_DATA_PATH + "storehouse.debug.csv");
	}
	else
	{
		storehouse->Init(ORIGIN_DATA_PATH + "storehouse.csv");
	}

	NSStarmanLib::WeaponManager* weaponManager = NSStarmanLib::WeaponManager::GetObj();

	if (SharedObj::DebugMode())
	{
		weaponManager->Init(ORIGIN_DATA_PATH + "weapon.csv", ORIGIN_DATA_PATH + "weaponSave.debug.csv");
	}
	else
	{
		weaponManager->Init(ORIGIN_DATA_PATH + "weapon.csv", ORIGIN_DATA_PATH + "weaponSave.csv");
	}

	NSStarmanLib::EnemyInfoManager* enemyInfoManager = NSStarmanLib::EnemyInfoManager::GetObj();

	if (SharedObj::DebugMode())
	{
		enemyInfoManager->Init(ORIGIN_DATA_PATH + "enemyDef.csv",
							   ORIGIN_DATA_PATH + "enemy.debug.csv",
							   ORIGIN_DATA_PATH + "enemyVisible.debug.csv");
	}
	else
	{
		enemyInfoManager->Init(ORIGIN_DATA_PATH + "enemyDef.csv",
							   ORIGIN_DATA_PATH + "enemy.csv",
							   ORIGIN_DATA_PATH + "enemyVisible.csv");
	}

	NSStarmanLib::SkillManager* skillManager = NSStarmanLib::SkillManager::GetObj();

	if (SharedObj::DebugMode())
	{
		skillManager->Init(ORIGIN_DATA_PATH + "skill.csv",
						   ORIGIN_DATA_PATH + "skillSub.debug.csv");
	}
	else
	{
		skillManager->Init(ORIGIN_DATA_PATH + "skill.csv",
						   ORIGIN_DATA_PATH + "skillSub.csv");
	}

	NSStarmanLib::StatusManager* statusManager = NSStarmanLib::StatusManager::GetObj();

	if (SharedObj::DebugMode())
	{
		statusManager->Init(ORIGIN_DATA_PATH + "status.debug.csv");
	}
	else
	{
		statusManager->Init(ORIGIN_DATA_PATH + "status.csv");
	}

	NSStarmanLib::Guide* guide = NSStarmanLib::Guide::GetObj();

	if (SharedObj::DebugMode())
	{
		guide->Init(ORIGIN_DATA_PATH + "guide.debug.csv");
	}
	else
	{
		guide->Init(ORIGIN_DATA_PATH + "guide.csv");
	}

	NSStarmanLib::PowereggDateTime* datetime = NSStarmanLib::PowereggDateTime::GetObj();

	if (SharedObj::DebugMode())
	{
		datetime->Init(ORIGIN_DATA_PATH + "datetime.debug.csv");
	}
	else
	{
		datetime->Init(ORIGIN_DATA_PATH + "datetime.csv");
	}

	NSStarmanLib::MapObjManager* mapObjManager = NSStarmanLib::MapObjManager::GetObj();

	mapObjManager->Init(ORIGIN_DATA_PATH + "map_obj.csv");
}

void SaveManager::Load()
{
	NSStarmanLib::HumanInfoManager* him = NSStarmanLib::HumanInfoManager::GetObj();
	if (SharedObj::DebugMode())
	{
		him->Init(SAVEDATA_PATH + "humanInfo.csv", SAVEDATA_PATH + "humanInfoSub.debug.csv");
	}
	else
	{
		him->Init(SAVEDATA_PATH + "humanInfo.csv", SAVEDATA_PATH + "humanInfoSub.csv");
	}

	NSStarmanLib::MapInfoManager* mapManager = NSStarmanLib::MapInfoManager::GetObj();
	if (SharedObj::DebugMode())
	{
		mapManager->Init(SAVEDATA_PATH + "mapInfo.debug.csv");
	}
	else
	{
		mapManager->Init(SAVEDATA_PATH + "mapInfo.csv");
	}

	NSStarmanLib::ItemManager* itemManager = NSStarmanLib::ItemManager::GetObj();
	itemManager->Init(SAVEDATA_PATH + "item.csv", SAVEDATA_PATH + "item_pos.csv");

	NSStarmanLib::Inventory* inventory = NSStarmanLib::Inventory::GetObj();

	if (SharedObj::DebugMode())
	{
		inventory->Init(SAVEDATA_PATH + "inventory.debug.csv");
	}
	else
	{
		inventory->Init(SAVEDATA_PATH + "inventory.csv");
	}

	NSStarmanLib::Storehouse* storehouse = NSStarmanLib::Storehouse::GetObj();

	if (SharedObj::DebugMode())
	{
		storehouse->Init(SAVEDATA_PATH + "storehouse.debug.csv");
	}
	else
	{
		storehouse->Init(SAVEDATA_PATH + "storehouse.csv");
	}

	NSStarmanLib::WeaponManager* weaponManager = NSStarmanLib::WeaponManager::GetObj();

	if (SharedObj::DebugMode())
	{
		weaponManager->Init(SAVEDATA_PATH + "weapon.csv", SAVEDATA_PATH + "weaponSave.debug.csv");
	}
	else
	{
		weaponManager->Init(SAVEDATA_PATH + "weapon.csv", SAVEDATA_PATH + "weaponSave.csv");
	}

	NSStarmanLib::EnemyInfoManager* enemyInfoManager = NSStarmanLib::EnemyInfoManager::GetObj();

	if (SharedObj::DebugMode())
	{
		enemyInfoManager->Init(SAVEDATA_PATH + "enemyDef.csv",
							   SAVEDATA_PATH + "enemy.debug.csv",
							   SAVEDATA_PATH + "enemyVisible.debug.csv");
	}
	else
	{
		enemyInfoManager->Init(SAVEDATA_PATH + "enemyDef.csv",
							   SAVEDATA_PATH + "enemy.csv",
							   SAVEDATA_PATH + "enemyVisible.csv");
	}

	NSStarmanLib::SkillManager* skillManager = NSStarmanLib::SkillManager::GetObj();

	if (SharedObj::DebugMode())
	{
		skillManager->Init(SAVEDATA_PATH + "skill.csv",
						   SAVEDATA_PATH + "skillSub.debug.csv");
	}
	else
	{
		skillManager->Init(SAVEDATA_PATH + "skill.csv",
						   SAVEDATA_PATH + "skillSub.csv");
	}

	NSStarmanLib::StatusManager* statusManager = NSStarmanLib::StatusManager::GetObj();

	if (SharedObj::DebugMode())
	{
		statusManager->Init(SAVEDATA_PATH + "status.debug.csv");
	}
	else
	{
		statusManager->Init(SAVEDATA_PATH + "status.csv");
	}

	NSStarmanLib::Guide* guide = NSStarmanLib::Guide::GetObj();

	if (SharedObj::DebugMode())
	{
		guide->Init(SAVEDATA_PATH + "guide.debug.csv");
	}
	else
	{
		guide->Init(SAVEDATA_PATH + "guide.csv");
	}

	NSStarmanLib::PowereggDateTime* datetime = NSStarmanLib::PowereggDateTime::GetObj();

	if (SharedObj::DebugMode())
	{
		datetime->Init(SAVEDATA_PATH + "datetime.debug.csv");
	}
	else
	{
		datetime->Init(SAVEDATA_PATH + "datetime.csv");
	}

	NSStarmanLib::MapObjManager* mapObjManager = NSStarmanLib::MapObjManager::GetObj();

	mapObjManager->Init(SAVEDATA_PATH + "map_obj.csv");
}
