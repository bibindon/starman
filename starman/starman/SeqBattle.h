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

#include <thread>
#include "Opening.h"
#include "Title.h"

class Title;

class SeqBattle
{
public:
    SeqBattle();
    ~SeqBattle();
    void Update(eSequence* sequence);
    void Render();
    void InputR1();
    void Confirm(eSequence* sequence);

    eBattleState GetState() const;
private:
    void UpdateCommon();
    void RenderCommon();
    void RenderNormal();

    void UpdateDebug();
    void UpdatePerSecond();
    void OperateNormal(eSequence* sequence);
    Player* m_player { nullptr };
    Sprite* m_spriteGameover { nullptr };
    eBattleState m_eState { eBattleState::NORMAL };
    int m_nGameoverCounter { 0 };

    Map* m_map = nullptr;
    EnemyManager* m_enemyManager = nullptr;

    void SaveLastStage(const int);

    std::deque<std::string> m_finishQuestQue;

    // エンディングを開始してほしいときにtrueにする
    bool m_bEndingRequest { false };

    //----------------------------------------------------------
    // 調べる
    //----------------------------------------------------------

    // 調べる、が可能な時
    bool m_bShowExamine { false };
    Sprite* m_spriteExamine { nullptr };

    // アイテムが拾える時
    bool m_bObtainable = false;

    //----------------------------------------------------------
    // 会話
    //----------------------------------------------------------
    NSTalkLib2::Talk* m_talk { nullptr };
    bool m_bTalking { false };
    void OperateTalk();

    //----------------------------------------------------------
    // メニュー
    //----------------------------------------------------------
    void OperateMenu(eSequence* sequence);
    MenuManager m_menuManager;

    //----------------------------------------------------------
    // ステータス表示
    //----------------------------------------------------------
    HudManager m_hudManager;

    //----------------------------------------------------------
    // 倉庫画面
    //----------------------------------------------------------
    void OperateStorehouse();
    NSStorehouseLib::StorehouseLib* m_storehouse = nullptr;

    //----------------------------------------------------------
    // クラフト画面
    //----------------------------------------------------------
    void OperateCraft();
    NSCraftLib::CraftLib* m_craft = nullptr;

    //----------------------------------------------------------
    // コマンド画面
    //----------------------------------------------------------
    void OperateCommand();
    CommandManager m_commandManager;

    //----------------------------------------------------------
    // タイトル画面
    //----------------------------------------------------------
    void OperateTitle(eSequence* sequence);
    Title* m_title = nullptr;

    //----------------------------------------------------------
    // ロード画面
    // TODO クラス化
    // 真っ暗画面で砂時計のくるくるを表示
    // セーブデータがなければ初期データを読み、
    // セーブデータがあればセーブデータを読む。
    //----------------------------------------------------------
    void InitLoad();
    void UpdateLoad();
    void InitializeAfterLoad();
    void RenderLoad();
    void FinalizeLoad();
    Sprite* m_sprLoadBack = nullptr;
    Sprite* m_sprLoadLogo = nullptr;
    Sprite* m_sprLoadLoading = nullptr;
    Sprite* m_sprLoadClock = nullptr;
    bool m_bSavedataExists = false;

    std::thread* m_loadThread = nullptr;
    std::atomic<bool> m_loadLoaded = false;

    //----------------------------------------------------------
    // オープニング画面
    // TODO 名前を変更
    //----------------------------------------------------------
    void OperateOpening();
    Opening* m_Opening = nullptr;
};

