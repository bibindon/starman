#pragma once

#include "../../StarmanLib/StarmanLib/StarmanLib/NpcStatusManager.h"
class NpcManager
{
public:
    static NpcManager* Get();
    void SetPos(const std::string& npcName,
                const float fx,
                const float fy,
                const float fz);

    void SetRot(const std::string& npcName,
                const float fRot);

    void SetTalkEnable(const std::string& npcName,
                       const bool arg);

    void SetTalkScript(const std::string& npcName,
                       const std::string& arg);

    void SetEnableFeature(const std::string& npcName,
                          const bool arg);

    void SetShowMenu(const std::string& npcName,
                     const bool arg);

    NSStarmanLib::NpcStatus GetNpcStatus(const std::string& npcName);

private:
    static NpcManager* m_ton;

};

