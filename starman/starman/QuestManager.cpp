#include "QuestManager.h"
#include "SharedObj.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/Storehouse.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/PowereggDateTime.h"
#include "NpcManager.h"

QuestManager* QuestManager::m_single = nullptr;

void QuestManager::Init(const std::string& originFile, const std::string& saveFile)
{
    m_single->m_questSystem.Init(originFile, saveFile, false);
}

void QuestManager::Save(const std::string& originFile)
{
    m_single->m_questSystem.Save(originFile, false);
}

QuestManager* QuestManager::Get()
{
    if (m_single == nullptr)
    {
        m_single = new QuestManager();
    }

    return m_single;
}

void QuestManager::Finalize()
{
    SAFE_DELETE(m_single);
    return ;
}

void QuestManager::SetExamine()
{
    D3DXVECTOR3 playerPos = SharedObj::GetPlayer()->GetPos();
    m_questSystem.SetExamine(playerPos.x, playerPos.y, playerPos.z);
}

void QuestManager::SetTalk(const std::string& npc)
{
    m_questSystem.SetTalk(npc);
}

std::string QuestManager::GetQuestIdStartByExamine()
{
    D3DXVECTOR3 playerPos = SharedObj::GetPlayer()->GetPos();
    return m_questSystem.GetQuestIdStartByExamine(playerPos.x, playerPos.y, playerPos.z);
}

std::string QuestManager::GetQuestIdFinishByExamine()
{
    D3DXVECTOR3 playerPos = SharedObj::GetPlayer()->GetPos();
    return m_questSystem.GetQuestIdFinishByExamine(playerPos.x, playerPos.y, playerPos.z);
}

std::vector<std::string> QuestManager::GetStartQuest()
{
    auto startQuest = m_questSystem.GetStartQuest();
    return startQuest;
}

std::vector<std::string> QuestManager::GetQuestStartEvent(const std::string& quest)
{
    auto startEvent = m_questSystem.GetQuestStartEvent(quest);
    return startEvent;
}

std::vector<std::string> QuestManager::GetFinishQuest()
{
    auto questList = m_questSystem.GetFinishQuest();
    return questList;
}

std::vector<std::string> QuestManager::GetQuestFinishEvent(const std::string& quest)
{
    auto eventList = m_questSystem.GetQuestFinishEvent(quest);
    return eventList;
}

void QuestManager::SetQuestFinish(const std::string& quest)
{
    m_questSystem.SetQuestFinish(quest);
}

void QuestManager::Update()
{
    //-------------------------------------
    // クエストシステムにプレイヤーの現在地を知らせる。
    //-------------------------------------
    {
        D3DXVECTOR3 playerPos = SharedObj::GetPlayer()->GetPos();
        m_questSystem.SetPos(playerPos.x, playerPos.y, playerPos.z, false);
    }

    //-------------------------------------
    // クエストシステムに現在時刻を設定。
    //-------------------------------------
    {
        auto datetime = NSStarmanLib::PowereggDateTime::GetObj();
        m_questSystem.SetCurrentDateTime(datetime->GetYear(),
                                         datetime->GetMonth(),
                                         datetime->GetDay(),
                                         datetime->GetHour(),
                                         datetime->GetMinute(),
                                         datetime->GetSecond(),
                                         false);
    }

    //---------------------------------------------------------------------
    // クエストシステムに登録されている所持品情報を更新
    //---------------------------------------------------------------------

    // インベントリ
    {
        auto inventory = NSStarmanLib::Inventory::GetObj();
        auto itemManager = NSStarmanLib::ItemManager::GetObj();
        auto allItem = inventory->GetAllItem();

        std::vector<NSQuestSystem::ItemInfo> itemList;
        for (auto it = allItem.begin(); it != allItem.end(); ++it)
        {
            NSQuestSystem::ItemInfo itemInfo;
            auto itemDef = itemManager->GetItemDef(it->GetId());
            itemInfo.m_itemName = itemDef.GetName();
            itemInfo.m_level = itemDef.GetLevel();
            itemList.push_back(itemInfo);
        }

        m_questSystem.SetInventoryContent(itemList, false);
    }

    // 倉庫
    {
        auto storehouseIdList = NSStarmanLib::StorehouseManager::Get()->GetStorehouseIdList();
        auto itemManager = NSStarmanLib::ItemManager::GetObj();

        for (size_t i = 0; i < storehouseIdList.size(); ++i)
        {
            auto id = storehouseIdList.at(i);
            auto storehouse = NSStarmanLib::StorehouseManager::Get()->GetStorehouse(id);
            auto allItem = storehouse->GetAllItem();

            std::vector<NSQuestSystem::ItemInfo> itemList;
            for (auto it = allItem.begin(); it != allItem.end(); ++it)
            {
                NSQuestSystem::ItemInfo itemInfo;
                auto itemDef = itemManager->GetItemDef(it->GetId());
                itemInfo.m_itemName = itemDef.GetName();
                itemInfo.m_level = itemDef.GetLevel();
                itemList.push_back(itemInfo);
            }

            m_questSystem.SetStorehouseContent(id, itemList, false);
        }
    }

    //-------------------------------------
    // クエストシステムにNPCの生存状況を設定
    //-------------------------------------
    {
        auto bAlive = false;
        auto bDead = false;

        bDead = NpcManager::Get()->GetNpcStatus("daikeiman").GetDead();
        bAlive = !bDead;
        m_questSystem.SetNpcIsAlive("daikeiman", bAlive, false);

        bDead = NpcManager::Get()->GetNpcStatus("sankakuman").GetDead();
        bAlive = !bDead;
        m_questSystem.SetNpcIsAlive("sankakuman", bAlive, false);

        bDead = NpcManager::Get()->GetNpcStatus("shikakuman").GetDead();
        bAlive = !bDead;
        m_questSystem.SetNpcIsAlive("shikakuman", bAlive, false);
    }

    m_questSystem.UpdateQuestStatus();
}

void QuestManager::SetHint(const std::string& hint)
{
    m_hint = hint;
}

std::string QuestManager::GetHint() const
{
    return m_hint;
}

