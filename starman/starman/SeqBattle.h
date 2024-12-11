#pragma once

#include <d3d9.h>
#include "Common.h"
#include "Sprite.h"
#include "Mesh.h"
#include "AnimMesh.h"
#include "Player.h"
#include "Enemy.h"
#include "Map.h"

#include "..\..\talklib2\talklib2\talklib2.h"
#include "..\..\menulib\menulib\menulib\MenuLib.h"
#include "..\..\hud\hud\hud.h"
#include "..\..\quest-system\QuestSystem\QuestSystem\QuestSystem.h"
#include "..\..\StarmanLib\StarmanLib\StarmanLib\WeaponManager.h"
#include "..\..\StarmanLib\StarmanLib\StarmanLib\StatusManager.h"
#include "..\..\StarmanLib\StarmanLib\StarmanLib\PowereggDateTime.h"
#include "..\..\StarmanLib\StarmanLib\StarmanLib\Storehouse.h"
#include "..\..\StarmanLib\StarmanLib\StarmanLib\Inventory.h"
#include "..\..\StarmanLib\StarmanLib\StarmanLib\ItemManager.h"
#include "..\..\storehouselib\storehouselib\StorehouseLib.h"
#include "..\..\craftlib\craftlib\CraftLib.h"
#include "MenuManager.h"

enum class eBattleState
{
    NORMAL,
    GAMEOVER,
};

class SeqBattle
{
public:
    SeqBattle(const bool isContinue);
    ~SeqBattle();
    void Update(eSequence* sequence);
    void Render();
    void InputR1();
    void InputA();
    void InputB(eSequence* sequence);
private:
    Player* m_player { nullptr };
    Sprite* m_spriteGameover { nullptr };
    eBattleState m_eState { eBattleState::NORMAL };
    int m_nGameoverCounter { 0 };
    int m_nCurrentStage { 1 };
    Map* m_map { };

    void SaveLastStage(const int);

    NSTalkLib2::Talk* m_talk { nullptr };
    bool m_bTalking { false };

    std::deque<std::string> m_finishQuestQue;

    bool m_bShowExamine { false };
    Sprite* m_spriteExamine { nullptr };

    // エンディングを開始してほしいときにtrueにする
    bool m_bEndingRequest { false };

    void OperateMenu(eSequence* sequence);

    //----------------------------------------------------------
    // メニュー
    //----------------------------------------------------------
    void OperateTalk();
    bool m_bShowMenu { false };
    MenuManager m_menuManager;

    //----------------------------------------------------------
    // ステータス表示
    //----------------------------------------------------------
    bool m_bShowHud { false };
    NSHud::hud* m_hud = nullptr;

    //----------------------------------------------------------
    // 倉庫画面
    //----------------------------------------------------------
    void OperateStorehouse();
    bool m_bShowStorehouse = false;
    NSStorehouseLib::StorehouseLib* m_storehouse = nullptr;

    //----------------------------------------------------------
    // クラフト画面
    //----------------------------------------------------------
    void OperateCraft();
    bool m_bShowCraft = false;
    NSCraftLib::CraftLib* m_craft = nullptr;


};

