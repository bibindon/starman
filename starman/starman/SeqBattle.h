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

    // �G���f�B���O���J�n���Ăق����Ƃ���true�ɂ���
    bool m_bEndingRequest { false };

    //----------------------------------------------------------
    // ���ׂ�
    //----------------------------------------------------------

    // ���ׂ�A���\�Ȏ�
    bool m_bShowExamine { false };
    Sprite* m_spriteExamine { nullptr };

    // �A�C�e�����E���鎞
    bool m_bObtainable = false;

    //----------------------------------------------------------
    // ��b
    //----------------------------------------------------------
    NSTalkLib2::Talk* m_talk { nullptr };
    bool m_bTalking { false };
    void OperateTalk();

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
};

