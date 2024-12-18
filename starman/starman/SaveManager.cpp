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
		him->Save("res\\script\\humanInfoSub.debug.csv");
	}
	else
	{
		him->Save("res\\script\\humanInfoSub.csv");
	}

	NSStarmanLib::MapInfoManager* mapManager = NSStarmanLib::MapInfoManager::GetObj();
	if (SharedObj::DebugMode())
	{
		mapManager->Save("res\\script\\mapInfo.debug.csv");
	}
	else
	{
		mapManager->Save("res\\script\\mapInfo.csv");
	}

	NSStarmanLib::ItemManager* itemManager = NSStarmanLib::ItemManager::GetObj();
	itemManager->Save("res\\script\\item_pos.csv");

	NSStarmanLib::Inventory* inventory = NSStarmanLib::Inventory::GetObj();

	if (SharedObj::DebugMode())
	{
		inventory->Save("res\\script\\inventory.debug.csv");
	}
	else
	{
		inventory->Save("res\\script\\inventory.csv");
	}

	NSStarmanLib::Storehouse* storehouse = NSStarmanLib::Storehouse::GetObj();

	if (SharedObj::DebugMode())
	{
		storehouse->Save("res\\script\\storehouse.debug.csv");
	}
	else
	{
		storehouse->Save("res\\script\\storehouse.csv");
	}

	NSStarmanLib::WeaponManager* weaponManager = NSStarmanLib::WeaponManager::GetObj();

	if (SharedObj::DebugMode())
	{
		weaponManager->Save("res\\script\\weaponSave.debug.csv");
	}
	else
	{
		weaponManager->Save("res\\script\\weaponSave.csv");
	}

	NSStarmanLib::EnemyInfoManager* enemyInfoManager = NSStarmanLib::EnemyInfoManager::GetObj();

	if (SharedObj::DebugMode())
	{
		enemyInfoManager->Save("res\\script\\enemy.debug.csv",
							   "res\\script\\enemyVisible.debug.csv");
	}
	else
	{
		enemyInfoManager->Save("res\\script\\enemy.csv",
							   "res\\script\\enemyVisible.csv");
	}

	NSStarmanLib::SkillManager* skillManager = NSStarmanLib::SkillManager::GetObj();

	if (SharedObj::DebugMode())
	{
		skillManager->Save("res\\script\\skillSub.debug.csv");
	}
	else
	{
		skillManager->Save("res\\script\\skillSub.csv");
	}

	NSStarmanLib::StatusManager* statusManager = NSStarmanLib::StatusManager::GetObj();

	if (SharedObj::DebugMode())
	{
		statusManager->Save("res\\script\\status.debug.csv",
                            SharedObj::GetPlayer()->GetPos().x,
                            SharedObj::GetPlayer()->GetPos().y,
                            SharedObj::GetPlayer()->GetPos().z );
	}
	else
	{
		statusManager->Save("res\\script\\status.csv",
                            SharedObj::GetPlayer()->GetPos().x,
                            SharedObj::GetPlayer()->GetPos().y,
                            SharedObj::GetPlayer()->GetPos().z );
	}

	NSStarmanLib::Guide* guide = NSStarmanLib::Guide::GetObj();

	if (SharedObj::DebugMode())
	{
		guide->Save("res\\script\\guide.debug.csv");
	}
	else
	{
		guide->Save("res\\script\\guide.csv");
	}

	NSStarmanLib::PowereggDateTime* datetime = NSStarmanLib::PowereggDateTime::GetObj();

	if (SharedObj::DebugMode())
	{
		datetime->Save("res\\script\\datetime.debug.csv");
	}
	else
	{
		datetime->Save("res\\script\\datetime.csv");
	}

	NSStarmanLib::MapObjManager* mapObjManager = NSStarmanLib::MapObjManager::GetObj();

	mapObjManager->Save("res\\script\\map_obj.csv");
}

void SaveManager::LoadOrigin()
{
    // TODO
    Load();
}

void SaveManager::Load()
{
    {
        NSStarmanLib::HumanInfoManager* him = NSStarmanLib::HumanInfoManager::GetObj();
        if (SharedObj::DebugMode())
        {
            him->Init("res\\script\\humanInfo.csv", "res\\script\\humanInfoSub.debug.csv");
        }
        else
        {
            him->Init("res\\script\\humanInfo.csv", "res\\script\\humanInfoSub.csv");
        }

        NSStarmanLib::MapInfoManager* mapManager = NSStarmanLib::MapInfoManager::GetObj();
        if (SharedObj::DebugMode())
        {
            mapManager->Init("res\\script\\mapInfo.debug.csv");
        }
        else
        {
            mapManager->Init("res\\script\\mapInfo.csv");
        }

        NSStarmanLib::ItemManager* itemManager = NSStarmanLib::ItemManager::GetObj();
        itemManager->Init("res\\script\\item.csv", "res\\script\\item_pos.csv");

        NSStarmanLib::Inventory* inventory = NSStarmanLib::Inventory::GetObj();

        if (SharedObj::DebugMode())
        {
            inventory->Init("res\\script\\inventory.debug.csv");
        }
        else
        {
            inventory->Init("res\\script\\inventory.csv");
        }

        NSStarmanLib::Storehouse* storehouse = NSStarmanLib::Storehouse::GetObj();

        if (SharedObj::DebugMode())
        {
            storehouse->Init("res\\script\\storehouse.debug.csv");
        }
        else
        {
            storehouse->Init("res\\script\\storehouse.csv");
        }

        NSStarmanLib::WeaponManager* weaponManager = NSStarmanLib::WeaponManager::GetObj();

        if (SharedObj::DebugMode())
        {
            weaponManager->Init("res\\script\\weapon.csv", "res\\script\\weaponSave.debug.csv");
        }
        else
        {
            weaponManager->Init("res\\script\\weapon.csv", "res\\script\\weaponSave.csv");
        }

        NSStarmanLib::EnemyInfoManager* enemyInfoManager = NSStarmanLib::EnemyInfoManager::GetObj();

        if (SharedObj::DebugMode())
        {
            enemyInfoManager->Init("res\\script\\enemyDef.csv",
                                   "res\\script\\enemy.debug.csv",
                                   "res\\script\\enemyVisible.debug.csv");
        }
        else
        {
            enemyInfoManager->Init("res\\script\\enemyDef.csv",
                                   "res\\script\\enemy.csv",
                                   "res\\script\\enemyVisible.csv");
        }

        NSStarmanLib::SkillManager* skillManager = NSStarmanLib::SkillManager::GetObj();

        if (SharedObj::DebugMode())
        {
            skillManager->Init("res\\script\\skill.csv",
                               "res\\script\\skillSub.debug.csv");
        }
        else
        {
            skillManager->Init("res\\script\\skill.csv",
                               "res\\script\\skillSub.csv");
        }

        NSStarmanLib::StatusManager* statusManager = NSStarmanLib::StatusManager::GetObj();

        if (SharedObj::DebugMode())
        {
            statusManager->Init("res\\script\\status.debug.csv");
        }
        else
        {
            statusManager->Init("res\\script\\status.csv");
        }

        NSStarmanLib::Guide* guide = NSStarmanLib::Guide::GetObj();

        if (SharedObj::DebugMode())
        {
            guide->Init("res\\script\\guide.debug.csv");
        }
        else
        {
            guide->Init("res\\script\\guide.csv");
        }

        NSStarmanLib::PowereggDateTime* datetime = NSStarmanLib::PowereggDateTime::GetObj();

        if (SharedObj::DebugMode())
        {
            datetime->Init("res\\script\\datetime.debug.csv");
        }
        else
        {
            datetime->Init("res\\script\\datetime.csv");
        }

        NSStarmanLib::MapObjManager* mapObjManager = NSStarmanLib::MapObjManager::GetObj();

		mapObjManager->Init("res\\script\\map_obj.csv");
    }
}
