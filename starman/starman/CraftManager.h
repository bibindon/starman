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

    // ��\������\����Ԃɕς��Ƃ��ɉ�ʂ���蒼���B
    void Build();

    NSCraftLib::CraftLib m_gui;
};

