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
#include "StoryManager.h"

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

    // 2D要素は一番最後に描画する必要がある
    void RenderCommon2D();

    void RenderNormal();
    void RenderGameover();

    void UpdateDebug();
    void UpdatePerSecond();
    void OperateNormal(eSequence* sequence);
    Player* m_player { nullptr };
    eBattleState m_eState { eBattleState::NORMAL };

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
    bool m_bShowExamine = false;
    Sprite* m_spriteExamine = nullptr;

    // アイテムが拾える時
    bool m_bObtainable = false;

    // 投げて地面に落ちた武器を拾える時
    bool m_bObtainWeapon = false;

    // 話しかけられるとき
    bool m_bTalkable = false;

    //----------------------------------------------------------
    // 会話
    // TalkManagerがあったほうがよい？
    //----------------------------------------------------------
    NSTalkLib2::Talk* m_talk { nullptr };
    void OperateTalk();

    //----------------------------------------------------------
    // 紙芝居
    //----------------------------------------------------------
    StoryManager* m_story = nullptr;
    void OperateStory();

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
    bool m_bShowStorehouse = false;
    void ShowStorehouse();

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

    // コマンド画面が1分表示されていたら脱出コマンドを追加する
    int m_commandCounter = 0;
    bool m_commandShowEscape = false;

    //----------------------------------------------------------
    // タイトル画面
    //----------------------------------------------------------
    void OperateTitle(eSequence* sequence);
    Title* m_title = nullptr;

    //----------------------------------------------------------
    // ゲームオーバー画面
    //----------------------------------------------------------
    void OperateGameover(eSequence* sequence);
    Sprite* m_spriteGameover = nullptr;
    int m_nDeadCounter = 0;

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

    //----------------------------------------------------------
    // 睡眠・気絶
    // 現実時間で5秒攻撃されなければパワーエッグ星の6時間後に移動する。
    // 睡眠中に攻撃されたら即死
    //----------------------------------------------------------
    void OperateSleep();
    void RenderSleep();
    enum class eSleepSeq
    {
        NotStart, // 睡眠中ではない
        FadeOut, // 5秒かけてフェードアウト
        Sleep, // 睡眠。黒背景
        FadeIn, // 目覚め。フェードイン
        Finish, // 完了
    };

    eSleepSeq m_eSleepSeq = eSleepSeq::NotStart;
    int m_sleepFadeOut = 0;
    int m_sleepBlack = 0;
    int m_sleepFadeIn = 0;

    Sprite* m_sprBlack = nullptr;
};

