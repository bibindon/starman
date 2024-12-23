#pragma once

#include <d3d9.h>
#include "Common.h"
#include "Sprite.h"
#include "Mesh.h"
#include "AnimMesh.h"
#include "Player.h"
#include "EnemyCube.h"
#include "Map.h"

#include "..\..\talklib2\talklib2\talklib2.h"
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
#include "HudManager.h"
#include "CommandManager.h"
#include "MeshNoShade.h"
#include "EnemyManager.h"

enum class eBattleState
{
    NORMAL,
    GAMEOVER,
    TITLE,
    MENU,
    COMMAND,
    CRAFT,
    STOREHOUSE,
    TALK,
    LOADING,
};

class SeqBattle
{
public:
    SeqBattle(const bool isContinue);
    ~SeqBattle();
    void Update(eSequence* sequence);
    void Render();
    void InputR1();
    void Confirm(eSequence* sequence);
private:
    void UpdateDebug();
    void UpdatePerSecond();
    void Operate(eSequence* sequence);
    Player* m_player { nullptr };
    Sprite* m_spriteGameover { nullptr };
    eBattleState m_eState { eBattleState::NORMAL };
    int m_nGameoverCounter { 0 };

    Map* m_map = nullptr;
    EnemyManager* m_enemyManager = nullptr;

    void SaveLastStage(const int);

    NSTalkLib2::Talk* m_talk { nullptr };
    bool m_bTalking { false };

    std::deque<std::string> m_finishQuestQue;

    // 調べる、が可能な時
    bool m_bShowExamine { false };
    Sprite* m_spriteExamine { nullptr };

    // アイテムが拾える時
    bool m_bObtainable = false;

    // エンディングを開始してほしいときにtrueにする
    bool m_bEndingRequest { false };

    void OperateMenu(eSequence* sequence);

    //----------------------------------------------------------
    // メニュー
    //----------------------------------------------------------
    void OperateTalk();
    bool m_bShowMenu { false }; // TODO eBattleStateを使うべし
    MenuManager m_menuManager;

    //----------------------------------------------------------
    // ステータス表示
    //----------------------------------------------------------
    bool m_bShowHud { false }; // TODO eBattleStateを使うべし
    HudManager m_hudManager;

    //----------------------------------------------------------
    // 倉庫画面
    //----------------------------------------------------------
    void OperateStorehouse();
    bool m_bShowStorehouse = false; // TODO eBattleStateを使うべし
    NSStorehouseLib::StorehouseLib* m_storehouse = nullptr;

    //----------------------------------------------------------
    // クラフト画面
    //----------------------------------------------------------
    void OperateCraft();
    bool m_bShowCraft = false; // TODO eBattleStateを使うべし
    NSCraftLib::CraftLib* m_craft = nullptr;

    //----------------------------------------------------------
    // コマンド画面
    //----------------------------------------------------------
    void OperateCommand();
    bool m_bShowCommand = false; // TODO eBattleStateを使うべし
    CommandManager m_commandManager;

    //----------------------------------------------------------
    // タイトル画面
    // カメラの移動が完了するまではタイトル画面の担当範囲。
    // セーブデータがあったら、タイトル画面が表示されたときすでにセーブデータが読まれている。
    // セーブデータがあるのに初めからを選んだら、初期データは読んでいないので
    // くるくるを表示して読み直す必要がある。
    //----------------------------------------------------------
    void OperateTitle();
    CommandManager m_title;
    Sprite* m_sprTitle = nullptr;
    Sprite* m_sprTitleClock = nullptr;

    //----------------------------------------------------------
    // ロード画面
    // 真っ暗画面で砂時計のくるくるを表示
    // セーブデータがなければ初期データを読み、
    // セーブデータがあればセーブデータを読む。
    //----------------------------------------------------------
    void OperateLoad();
    Sprite* m_sprLoadBack = nullptr;
    Sprite* m_sprLoadClock = nullptr;

};

