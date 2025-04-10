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
#include "../../StarmanLib/StarmanLib/StarmanLib/NpcStatusManager.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/Rynen.h"
#include "QuestManager.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/PatchTestManager.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/Voyage.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/ActivityBase.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/CraftInfoManager.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/CraftSystem.h"

SaveManager* SaveManager::m_obj = nullptr;

SaveManager* SaveManager::Get()
{
    if (m_obj == nullptr)
    {
        m_obj = NEW SaveManager();

        if (Common::DeployEncryptMode())
        {
            m_obj->m_encrypt = true;
        }
        else
        {
            m_obj->m_encrypt = false;
        }
    }

    return m_obj;
}

void SaveManager::Destroy()
{
    SAFE_DELETE(m_obj);
}

std::string SaveManager::CreateOriginFilePath(const std::string& filename)
{
    std::string originDataPath;

    originDataPath = ORIGIN_DATA_PATH;
    originDataPath += filename;
    if (Common::DeployEncryptMode())
    {
        if (m_encrypt)
        {
            originDataPath.replace(originDataPath.size() - 3, 3, "enc");
        }
    }
    return originDataPath;
}

std::string SaveManager::CreateSaveFilePath(const std::string& filename)
{
    std::string saveDataPath;
    saveDataPath = SAVEDATA_PATH;
    saveDataPath += filename;

    if (Common::DeployEncryptMode())
    {
        if (m_encrypt)
        {
            saveDataPath.replace(saveDataPath.size() - 3, 3, "enc");
        }
    }
    return saveDataPath;
}

std::string SaveManager::GetOriginMapPath()
{
    std::string path= ORIGIN_DATA_PATH + "map_obj.bin";
    return path;
}

std::string SaveManager::GetSavefileMapPath()
{
    std::string path = SAVEDATA_PATH + "map_obj.bin";
    return path;
}

void SaveManager::Save()
{
    // フォルダがなければ作る
    std::string savedir = "res\\script\\save";

    if (PathFileExists(savedir.c_str()) == FALSE)
    {
        BOOL result = CreateDirectory(savedir.c_str(), NULL);
        if (result == FALSE)
        {
            throw std::exception();
        }
    }

    auto rynen = NSStarmanLib::Rynen::GetObj();
    rynen->Save(CreateSaveFilePath("rynen.csv"), m_encrypt);

    NSStarmanLib::NpcStatusManager* mgr = NSStarmanLib::NpcStatusManager::GetObj();
    mgr->Save(CreateSaveFilePath("npcStatus.csv"), m_encrypt);

    NSStarmanLib::HumanInfoManager* him = NSStarmanLib::HumanInfoManager::GetObj();
    him->Save(CreateSaveFilePath("humanInfoSub.csv"), m_encrypt);

    NSStarmanLib::MapInfoManager* mapManager = NSStarmanLib::MapInfoManager::GetObj();
    mapManager->Save(CreateSaveFilePath("mapInfo.csv"), m_encrypt);

    NSStarmanLib::ItemManager* itemManager = NSStarmanLib::ItemManager::GetObj();
    itemManager->Save(CreateSaveFilePath("item_pos.csv"), m_encrypt);

    NSStarmanLib::Inventory* inventory = NSStarmanLib::Inventory::GetObj();
    inventory->Save(CreateSaveFilePath("inventory.csv"), m_encrypt);

    auto storehouseManager = NSStarmanLib::StorehouseManager::Get();
    storehouseManager->Save("storehouseListSave.csv", SAVEDATA_FOLDER);

    NSStarmanLib::WeaponManager* weaponManager = NSStarmanLib::WeaponManager::GetObj();
    weaponManager->Save(CreateSaveFilePath("weaponSave.csv"), m_encrypt);

    NSStarmanLib::EnemyInfoManager* enemyInfoManager = NSStarmanLib::EnemyInfoManager::GetObj();
    enemyInfoManager->Save(CreateSaveFilePath("enemy.csv"),
                           CreateSaveFilePath("enemyVisible.csv"),
                           m_encrypt);

    NSStarmanLib::SkillManager* skillManager = NSStarmanLib::SkillManager::GetObj();
    skillManager->Save(CreateSaveFilePath("skillSub.csv"), m_encrypt);

    NSStarmanLib::StatusManager* statusManager = NSStarmanLib::StatusManager::GetObj();
    statusManager->Save(CreateSaveFilePath("status.csv"),
                        SharedObj::GetPlayer()->GetPos().x,
                        SharedObj::GetPlayer()->GetPos().y,
                        SharedObj::GetPlayer()->GetPos().z,
                        m_encrypt);

    NSStarmanLib::Guide* guide = NSStarmanLib::Guide::GetObj();
    guide->Save(CreateSaveFilePath("guide.csv"), m_encrypt);

    NSStarmanLib::PowereggDateTime* datetime = NSStarmanLib::PowereggDateTime::GetObj();
    datetime->Save(CreateSaveFilePath("datetime.csv"), m_encrypt);

    // TODO 保存すると壊れる
    NSStarmanLib::MapObjManager* mapObjManager = NSStarmanLib::MapObjManager::GetObj();
    //mapObjManager->Save(CreateSaveFilePath("map_obj.csv"), m_encrypt);
    mapObjManager->SaveWithBinary(GetSavefileMapPath());

    QuestManager::Get()->Save(CreateSaveFilePath("questSave.csv"));

    NSStarmanLib::PatchTestManager* patchTestManager = NSStarmanLib::PatchTestManager::Get();
    patchTestManager->Save(CreateSaveFilePath("patchTestQueSave.csv"),
                           CreateSaveFilePath("patchTestInfoSave.csv"));

    auto voyage = NSStarmanLib::Voyage::Get();
    voyage->Save(CreateSaveFilePath("raftSave.csv"));

    auto activityBase = NSStarmanLib::ActivityBase::Get();
    activityBase->Save(CreateSaveFilePath("activityBase.csv"));

    NSStarmanLib::CraftSystem::GetObj()->Save(CreateSaveFilePath("craftsmanSkillSave.csv"),
                                              CreateSaveFilePath("craftsmanQueueSave.csv"));
}

void SaveManager::LoadOrigin()
{
    m_progress.store(0);
    if (m_savedataLoaded)
    {
        NSStarmanLib::Rynen::Destroy();
        NSStarmanLib::NpcStatusManager::Destroy();
        NSStarmanLib::HumanInfoManager::Destroy();
        NSStarmanLib::MapInfoManager::Destroy();
        NSStarmanLib::ItemManager::Destroy();
        NSStarmanLib::Inventory::Destroy();
        NSStarmanLib::StorehouseManager::Destroy();
        NSStarmanLib::WeaponManager::Destroy();
        NSStarmanLib::EnemyInfoManager::Destroy();
        NSStarmanLib::SkillManager::Destroy();
        NSStarmanLib::StatusManager::Destroy();
        NSStarmanLib::Guide::Destroy();
        NSStarmanLib::PowereggDateTime::Destroy();
        NSStarmanLib::MapObjManager::Destroy();
        NSStarmanLib::ActivityBase::Get()->Finalize();
        NSStarmanLib::CraftInfoManager::Destroy();
        NSStarmanLib::CraftSystem::Destroy();
    }

    auto rynen = NSStarmanLib::Rynen::GetObj();
    rynen->Init(CreateOriginFilePath("rynen.csv"), m_encrypt);

    m_progress.store(10);
    NSStarmanLib::NpcStatusManager* mgr = NSStarmanLib::NpcStatusManager::GetObj();
    mgr->Init(CreateOriginFilePath("npcStatus.csv"), m_encrypt);

    NSStarmanLib::HumanInfoManager* him = NSStarmanLib::HumanInfoManager::GetObj();
    him->Init(CreateOriginFilePath("humanInfo.csv"),
              CreateOriginFilePath("humanInfoSub.csv"),
              m_encrypt);

    m_progress.store(20);
    NSStarmanLib::MapInfoManager* mapManager = NSStarmanLib::MapInfoManager::GetObj();
    mapManager->Init(CreateOriginFilePath("mapInfo.csv"), m_encrypt);

    NSStarmanLib::ItemManager* itemManager = NSStarmanLib::ItemManager::GetObj();
    itemManager->Init(CreateOriginFilePath("item.csv"),
                      CreateOriginFilePath("item_pos.csv"),
                      m_encrypt);

    m_progress.store(30);
    NSStarmanLib::Inventory* inventory = NSStarmanLib::Inventory::GetObj();
    inventory->Init(CreateOriginFilePath("inventory.csv"), m_encrypt);

    auto storehouseManager = NSStarmanLib::StorehouseManager::Get();
    storehouseManager->Init(CreateOriginFilePath("storehouseListOrigin.csv"));

    NSStarmanLib::WeaponManager* weaponManager = NSStarmanLib::WeaponManager::GetObj();
    weaponManager->Init(CreateOriginFilePath("weapon.csv"),
                        CreateOriginFilePath("weaponSave.csv"),
                        m_encrypt);

    m_progress.store(40);
    NSStarmanLib::EnemyInfoManager* enemyInfoManager = NSStarmanLib::EnemyInfoManager::GetObj();
    enemyInfoManager->Init(CreateOriginFilePath("enemyDef.csv"),
                           CreateOriginFilePath("enemy.csv"),
                           CreateOriginFilePath("enemyVisible.csv"),
                           m_encrypt);
            
        
    NSStarmanLib::SkillManager* skillManager = NSStarmanLib::SkillManager::GetObj();
    skillManager->Init(CreateOriginFilePath("skill.csv"),
                       CreateOriginFilePath("skillSub.csv"),
                        m_encrypt);
            
    m_progress.store(50);
    NSStarmanLib::StatusManager* statusManager = NSStarmanLib::StatusManager::GetObj();
    statusManager->Init(CreateOriginFilePath("status.csv"), m_encrypt);

    NSStarmanLib::Guide* guide = NSStarmanLib::Guide::GetObj();
    guide->Init(CreateOriginFilePath("guide.csv"), m_encrypt);

    NSStarmanLib::PowereggDateTime* datetime = NSStarmanLib::PowereggDateTime::GetObj();
    datetime->Init(CreateOriginFilePath("datetime.csv"), m_encrypt);

    m_progress.store(60);
    NSStarmanLib::MapObjManager* mapObjManager = NSStarmanLib::MapObjManager::GetObj();
//    mapObjManager->Init(CreateOriginFilePath("map_obj.csv"),
//                        CreateOriginFilePath("map_obj_type.csv"), m_encrypt);

    mapObjManager->InitWithBinary(GetOriginMapPath(),
                                  CreateOriginFilePath("map_obj_type.csv"), m_encrypt);

    QuestManager::Get()->Init(CreateOriginFilePath("quest.csv"), "");

    m_progress.store(70);
    NSStarmanLib::PatchTestManager* patchTestManager = NSStarmanLib::PatchTestManager::Get();
    patchTestManager->Init(CreateOriginFilePath("patchTestOrigin.csv"),
                           "",
                           "");

    auto voyage = NSStarmanLib::Voyage::Get();
    voyage->Init("");

    m_progress.store(80);
    auto activityBase = NSStarmanLib::ActivityBase::Get();
    activityBase->Init(CreateOriginFilePath("activityBase.csv"));

    NSStarmanLib::CraftInfoManager::GetObj()->Init(CreateOriginFilePath("craftDef.csv"));

    NSStarmanLib::CraftSystem::GetObj()->Init(CreateOriginFilePath("craftsmanSkill.csv"),
                                              CreateOriginFilePath("craftsmanQueue.csv"));
}

void SaveManager::Load()
{
    m_progress.store(0);

    auto rynen = NSStarmanLib::Rynen::GetObj();
    rynen->Init(CreateSaveFilePath("rynen.csv"), m_encrypt);

    NSStarmanLib::NpcStatusManager* mgr = NSStarmanLib::NpcStatusManager::GetObj();
    mgr->Init(CreateSaveFilePath("npcStatus.csv"), m_encrypt);

    NSStarmanLib::HumanInfoManager* him = NSStarmanLib::HumanInfoManager::GetObj();
    him->Init(CreateOriginFilePath("humanInfo.csv"),
              CreateSaveFilePath("humanInfoSub.csv"),
              m_encrypt);

    m_progress.store(5);
    NSStarmanLib::MapInfoManager* mapManager = NSStarmanLib::MapInfoManager::GetObj();
    mapManager->Init(CreateSaveFilePath("mapInfo.csv"), m_encrypt);

    m_progress.store(10);
    NSStarmanLib::ItemManager* itemManager = NSStarmanLib::ItemManager::GetObj();
    itemManager->Init(CreateOriginFilePath("item.csv"),
                      CreateSaveFilePath("item_pos.csv"),
                      m_encrypt);

    m_progress.store(15);
    NSStarmanLib::Inventory* inventory = NSStarmanLib::Inventory::GetObj();
    inventory->Init(CreateSaveFilePath("inventory.csv"), m_encrypt);

    m_progress.store(20);

    auto storehouseManager = NSStarmanLib::StorehouseManager::Get();
    storehouseManager->Init(CreateSaveFilePath("storehouseListSave.csv"));

    m_progress.store(25);
    NSStarmanLib::WeaponManager* weaponManager = NSStarmanLib::WeaponManager::GetObj();
    weaponManager->Init(CreateOriginFilePath("weapon.csv"),
                        CreateSaveFilePath("weaponSave.csv"),
                        m_encrypt);

    m_progress.store(30);
    NSStarmanLib::EnemyInfoManager* enemyInfoManager = NSStarmanLib::EnemyInfoManager::GetObj();
    enemyInfoManager->Init(CreateOriginFilePath("enemyDef.csv"),
                           CreateSaveFilePath("enemy.csv"),
                           CreateSaveFilePath("enemyVisible.csv"),
                           m_encrypt);
            
    m_progress.store(40);
        
    NSStarmanLib::SkillManager* skillManager = NSStarmanLib::SkillManager::GetObj();
    skillManager->Init(CreateOriginFilePath("skill.csv"),
                       CreateSaveFilePath("skillSub.csv"),
                        m_encrypt);
            
    m_progress.store(50);

    NSStarmanLib::StatusManager* statusManager = NSStarmanLib::StatusManager::GetObj();
    statusManager->Init(CreateSaveFilePath("status.csv"), m_encrypt);

    m_progress.store(60);

    NSStarmanLib::Guide* guide = NSStarmanLib::Guide::GetObj();
    guide->Init(CreateSaveFilePath("guide.csv"), m_encrypt);

    m_progress.store(70);

    NSStarmanLib::PowereggDateTime* datetime = NSStarmanLib::PowereggDateTime::GetObj();
    datetime->Init(CreateSaveFilePath("datetime.csv"), m_encrypt);

    m_progress.store(80);

    NSStarmanLib::MapObjManager* mapObjManager = NSStarmanLib::MapObjManager::GetObj();
//    mapObjManager->Init(CreateSaveFilePath("map_obj.csv"),
//                        CreateSaveFilePath("map_obj_type.csv"), m_encrypt);

    mapObjManager->InitWithBinary(GetSavefileMapPath(),
                                  CreateOriginFilePath("map_obj_type.csv"), m_encrypt);

    m_progress.store(90);

    QuestManager::Get()->Init(CreateOriginFilePath("quest.csv"),
                              CreateSaveFilePath("questSave.csv"));

    m_progress.store(95);

    NSStarmanLib::PatchTestManager* patchTestManager = NSStarmanLib::PatchTestManager::Get();
    patchTestManager->Init(CreateOriginFilePath("patchTestOrigin.csv"),
                           CreateSaveFilePath("patchTestInfoSave.csv"),
                           CreateSaveFilePath("patchTestQueSave.csv"));

    auto voyage = NSStarmanLib::Voyage::Get();
    voyage->Init(CreateSaveFilePath("raftSave.csv"));

    auto activityBase = NSStarmanLib::ActivityBase::Get();
    activityBase->Init(CreateSaveFilePath("activityBase.csv"));

    NSStarmanLib::CraftInfoManager::GetObj()->Init(CreateOriginFilePath("craftDef.csv"));

    NSStarmanLib::CraftSystem::GetObj()->Init(CreateSaveFilePath("craftsmanSkill.csv"),
                                              CreateSaveFilePath("craftsmanQueue.csv"));

    m_savedataLoaded = true;
}


bool SaveManager::DeleteFolderContents(const std::string& folderPath)
{
    std::string work;
    std::string searchPath = folderPath + "\\*";
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile(searchPath.c_str(), &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        work = "Error: Unable to access directory: " + folderPath + "\n";
        throw std::exception(work.c_str());
        return false;
    }

    do {
        std::string fileName = findFileData.cFileName;

        // スキップする項目 ("." と "..")
        if (fileName == "." || fileName == "..") {
            continue;
        }

        std::string fullPath = folderPath + "\\" + fileName;

        // ディレクトリの場合
        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            if (!DeleteFolderContents(fullPath)) {
                FindClose(hFind);
                return false;
            }
            if (!RemoveDirectory(fullPath.c_str())) {
                FindClose(hFind);

                work = "Failed to delete directory: " + fullPath + "\n";
                throw std::exception(work.c_str());

                return false;
            }
        }
        else { // ファイルの場合
            if (!DeleteFile(fullPath.c_str())) {
                FindClose(hFind);

                work = "Failed to delete file: " + fullPath + "\n";
                throw std::exception(work.c_str());

                return false;
            }
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
    return true;
}

bool SaveManager::DeleteFolder(const std::string& folderPath)
{
    if (!DeleteFolderContents(folderPath))
    {
        return false;
    }

    if (!RemoveDirectory(folderPath.c_str()))
    {
        std::string work = "Failed to delete folder: " + folderPath + "\n";
        throw std::exception(work.c_str());
        return false;
    }
    return true;
}

void SaveManager::DeleteSavedata()
{
    DeleteFolder(SAVEDATA_FOLDER);
}

int SaveManager::GetProgress()
{
    return m_progress.load();
}
