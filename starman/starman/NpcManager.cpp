#include "NpcManager.h"
#include "Common.h"
#include "SharedObj.h"
#include "PopUp2.h"

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
    status.SetTalkCsv(Common::ModExt(arg));
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

void NpcManager::SetNpcStatus(const std::string& npcName, const NSStarmanLib::NpcStatus& status)
{
    auto npcStatusMgr = NSStarmanLib::NpcStatusManager::GetObj();
    npcStatusMgr->SetNpcStatus(npcName, status);
}

bool NpcManager::GetNpcTalkable(const D3DXVECTOR3& pos, std::string* npcName)
{
    // 3メートル以内に話しかけられるNPCがいるか
    // 球だと重いのでバウンディングボックス

    bool exist = false;
    auto npcStatusMgr = NSStarmanLib::NpcStatusManager::GetObj();
    auto nameList = npcStatusMgr->GetNameList();

    for (size_t i = 0; i < nameList.size(); ++i)
    {
        auto status = npcStatusMgr->GetNpcStatus(nameList.at(i));
        float work = 0.f;

        work = std::abs(status.GetX() - pos.x);
        if (work <= 3.f)
        {
            work = std::abs(status.GetZ() - pos.z);
            if (work <= 3.f)
            {
                work = std::abs(status.GetY() - pos.y);
                if (work <= 3.f)
                {
                    if (status.GetHasTalk())
                    {
                        exist = true;
                        if (npcName != nullptr)
                        {
                            *npcName = nameList.at(i);
                        }
                        break;
                    }
                }
            }
        }
    }

    return exist;
}

void NpcManager::Update()
{
    // 死亡していない→死亡しているに変わったらポップアップを表示
    auto daikei = GetNpcStatus("daikeiman").GetDead();
    auto sankaku = GetNpcStatus("sankaku").GetDead();
    auto shikaku = GetNpcStatus("shikaku").GetDead();

    NSStarmanLib::NpcStatusManager::GetObj()->Update();

    auto daikei2 = GetNpcStatus("daikeiman").GetDead();
    auto sankaku2 = GetNpcStatus("sankaku").GetDead();
    auto shikaku2 = GetNpcStatus("shikaku").GetDead();

    if (daikei != daikei2 && daikei2)
    {
        PopUp2::Get()->SetText("ダイケイマンが死亡した");
    }

    if (sankaku != sankaku2 && sankaku2)
    {
        PopUp2::Get()->SetText("サンカクマンが死亡した");
    }

    if (shikaku != shikaku2 && shikaku2)
    {
        PopUp2::Get()->SetText("シカクマンが死亡した");
    }
}
