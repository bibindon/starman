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

    // ��ʂ���蒼���B
    void Build();

private:

    NSCraftLib::CraftLib m_gui;
};

