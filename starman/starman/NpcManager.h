#pragma once

#include <d3dx9.h>
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

    void SetNpcStatus(const std::string& npcName, const NSStarmanLib::NpcStatus& status);

    bool GetNpcTalkable(const D3DXVECTOR3& pos, std::string* npcName = nullptr);

private:
    static NpcManager* m_ton;

};

