#include "NpcManager.h"
#include "Common.h"
#include "SharedObj.h"

NpcManager* NpcManager::m_ton = nullptr;

NpcManager* NpcManager::Get()
{
    if (m_ton == nullptr)
    {
        m_ton = NEW NpcManager();
    }
    return m_ton;
}

void NpcManager::SetPos(const std::string& npcName, const float fx, const float fy, const float fz)
{
    auto npcStatusMgr = NSStarmanLib::NpcStatusManager::GetObj();
    auto status = npcStatusMgr->GetNpcStatus(npcName);
    status.SetX(fx);
    status.SetY(fy);
    status.SetZ(fz);
    npcStatusMgr->SetNpcStatus(npcName, status);

    auto map = SharedObj::GetMap();

    D3DXVECTOR3 vec(fx, fy, fz);
    map->SetNpcPos(npcName, vec);
}

void NpcManager::SetRot(const std::string& npcName, const float fRot)
{
    auto npcStatusMgr = NSStarmanLib::NpcStatusManager::GetObj();
    auto status = npcStatusMgr->GetNpcStatus(npcName);
    status.SetRotY(fRot);
    npcStatusMgr->SetNpcStatus(npcName, status);

    auto map = SharedObj::GetMap();
    map->SetNpcRot(npcName, fRot);
}

void NpcManager::SetTalkEnable(const std::string& npcName, const bool arg)
{
    auto npcStatusMgr = NSStarmanLib::NpcStatusManager::GetObj();
    auto status = npcStatusMgr->GetNpcStatus(npcName);
    status.SetHasTalk(arg);
    npcStatusMgr->SetNpcStatus(npcName, status);
}

void NpcManager::SetTalkScript(const std::string& npcName, const std::string& arg)
{
    auto npcStatusMgr = NSStarmanLib::NpcStatusManager::GetObj();
    auto status = npcStatusMgr->GetNpcStatus(npcName);
    status.SetTalkCsv(arg);
    npcStatusMgr->SetNpcStatus(npcName, status);
}

void NpcManager::SetEnableFeature(const std::string& npcName,
                                  const bool arg)
{
    auto npcStatusMgr = NSStarmanLib::NpcStatusManager::GetObj();
    auto status = npcStatusMgr->GetNpcStatus(npcName);
    status.SetFeatureEnable(arg);
    npcStatusMgr->SetNpcStatus(npcName, status);
}

void NpcManager::SetShowMenu(const std::string& npcName, const bool arg)
{
    auto npcStatusMgr = NSStarmanLib::NpcStatusManager::GetObj();
    auto status = npcStatusMgr->GetNpcStatus(npcName);
    status.SetMenuShow(arg);
    npcStatusMgr->SetNpcStatus(npcName, status);
}

NSStarmanLib::NpcStatus NpcManager::GetNpcStatus(const std::string& npcName)
{
    auto npcStatusMgr = NSStarmanLib::NpcStatusManager::GetObj();
    auto status = npcStatusMgr->GetNpcStatus(npcName);

    return status;
}
