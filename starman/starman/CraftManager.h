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

    // ��ʂ���蒼���B
    void Build();

    // �e�X�g�p�B
    // �摜�\���A�����`��A���ʉ��Đ������Ȃ��悤�ɂ���B
    void SetTestMode();

private:

    NSCraftLib::CraftLib m_gui;

    bool m_testMode = false;
};

