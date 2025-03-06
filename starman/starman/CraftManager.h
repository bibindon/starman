#pragma once

#include <string>
#include "../../craftlib/craftlib/CraftLib.h"
#include "Common.h"

class CraftManager
{
public:

    void Init();
    void Finalize();
    void Operate(eBattleState* state);
    void Draw();

private:

    // ”ñ•\¦‚©‚ç•\¦ó‘Ô‚É•Ï‚í‚é‚Æ‚«‚É‰æ–Ê‚ğì‚è’¼‚·B
    void Build();

    NSCraftLib::CraftLib m_gui;
};

