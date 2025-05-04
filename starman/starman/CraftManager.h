#pragma once

#include <string>
#include "../../craftlib/craftlib/CraftLib.h"
#include "Common.h"

class CraftManager
{
public:

    void Init();

    void Finalize();
    void Update();
    void Operate(eBattleState* state);
    void Draw();

    // ‰æ–Ê‚ğì‚è’¼‚·B
    void Build();

private:

    NSCraftLib::CraftLib m_gui;
};

