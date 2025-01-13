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

    // �G���f�B���O���J�n���Ăق����Ƃ���true�ɂ���
    bool m_bEndingRequest { false };

    //----------------------------------------------------------
    // ���ׂ�
    //----------------------------------------------------------

    // ���ׂ�A���\�Ȏ�
    bool m_bShowExamine = false;
    Sprite* m_spriteExamine = nullptr;

    // �A�C�e�����E���鎞
    bool m_bObtainable = false;

    // �b����������Ƃ�
    bool m_bTalkable = false;

    //----------------------------------------------------------
    // ��b
    // TalkManager���������ق����悢�H
    //----------------------------------------------------------
    NSTalkLib2::Talk* m_talk { nullptr };
    void OperateTalk();

    //----------------------------------------------------------
    // ���ŋ�
    //----------------------------------------------------------
    StoryManager* m_story = nullptr;
    void OperateStory();

    //----------------------------------------------------------
    // ���j���[
    //----------------------------------------------------------
    void OperateMenu(eSequence* sequence);
    MenuManager m_menuManager;

    //----------------------------------------------------------
    // �X�e�[�^�X�\��
    //----------------------------------------------------------
    HudManager m_hudManager;

    //----------------------------------------------------------
    // �q�ɉ��
    //----------------------------------------------------------
    void OperateStorehouse();
    NSStorehouseLib::StorehouseLib* m_storehouse = nullptr;

    //----------------------------------------------------------
    // �N���t�g���
    //----------------------------------------------------------
    void OperateCraft();
    NSCraftLib::CraftLib* m_craft = nullptr;

    //----------------------------------------------------------
    // �R�}���h���
    //----------------------------------------------------------
    void OperateCommand();
    CommandManager m_commandManager;

    //----------------------------------------------------------
    // �^�C�g�����
    //----------------------------------------------------------
    void OperateTitle(eSequence* sequence);
    Title* m_title = nullptr;

    //----------------------------------------------------------
    // �Q�[���I�[�o�[���
    //----------------------------------------------------------
    void OperateGameover(eSequence* sequence);
    Sprite* m_spriteGameover = nullptr;
    int m_nGameoverCounter = 0;

    //----------------------------------------------------------
    // ���[�h���
    // TODO �N���X��
    // �^���É�ʂō����v�̂��邭���\��
    // �Z�[�u�f�[�^���Ȃ���Ώ����f�[�^��ǂ݁A
    // �Z�[�u�f�[�^������΃Z�[�u�f�[�^��ǂށB
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
    // �I�[�v�j���O���
    // TODO ���O��ύX
    //----------------------------------------------------------
    void OperateOpening();
    Opening* m_Opening = nullptr;

    //----------------------------------------------------------
    // �����E�C��
    // �������Ԃ�5�b�U������Ȃ���΃p���[�G�b�O����6���Ԍ�Ɉړ�����B
    // �������ɍU�����ꂽ�瑦��
    //----------------------------------------------------------
    void OperateSleep();
    void RenderSleep();
    enum class eSleepSeq
    {
        NotStart, // �������ł͂Ȃ�
        FadeOut, // 5�b�����ăt�F�[�h�A�E�g
        Sleep, // �����B���w�i
        FadeIn, // �ڊo�߁B�t�F�[�h�C��
        Finish, // ����
    };

    eSleepSeq m_eSleepSeq = eSleepSeq::NotStart;
    int m_sleepFadeOut = 0;
    int m_sleepBlack = 0;
    int m_sleepFadeIn = 0;
    Sprite* m_sprSleepBlack = nullptr;
};

