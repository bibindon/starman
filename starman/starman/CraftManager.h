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

    // 画面を作り直す。
    void Build();

    // テスト用。
    // 画像表示、文字描画、効果音再生をしないようにする。
    void SetTestMode();

private:

    NSCraftLib::CraftLib m_gui;

    bool m_testMode = false;
};

