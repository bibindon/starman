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

    // 非表示から表示状態に変わるときに画面を作り直す。
    void Build();

    NSCraftLib::CraftLib m_gui;
};

