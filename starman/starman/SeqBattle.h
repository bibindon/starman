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

    Map* m_map = nullptr;
    EnemyManager* m_enemyManager = nullptr;

    void SaveLastStage(const int);

    NSTalkLib2::Talk* m_talk { nullptr };
    bool m_bTalking { false };

    std::deque<std::string> m_finishQuestQue;

    // ���ׂ�A���\�Ȏ�
    bool m_bShowExamine { false };
    Sprite* m_spriteExamine { nullptr };

    // �A�C�e�����E���鎞
    bool m_bObtainable = false;

    // �G���f�B���O���J�n���Ăق����Ƃ���true�ɂ���
    bool m_bEndingRequest { false };

    void OperateMenu(eSequence* sequence);

    //----------------------------------------------------------
    // ���j���[
    //----------------------------------------------------------
    void OperateTalk();
    bool m_bShowMenu { false };
    MenuManager m_menuManager;

    //----------------------------------------------------------
    // �X�e�[�^�X�\��
    //----------------------------------------------------------
    bool m_bShowHud { false };
    HudManager m_hudManager;

    //----------------------------------------------------------
    // �q�ɉ��
    //----------------------------------------------------------
    void OperateStorehouse();
    bool m_bShowStorehouse = false;
    NSStorehouseLib::StorehouseLib* m_storehouse = nullptr;

    //----------------------------------------------------------
    // �N���t�g���
    //----------------------------------------------------------
    void OperateCraft();
    bool m_bShowCraft = false;
    NSCraftLib::CraftLib* m_craft = nullptr;

    //----------------------------------------------------------
    // �R�}���h���
    //----------------------------------------------------------
    void OperateCommand();
    bool m_bShowCommand = false;
    CommandManager m_commandManager;

};

