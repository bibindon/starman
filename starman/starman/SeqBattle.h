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
#include "SeqOpening.h"

enum class eBattleState
{
    LOAD,
    TITLE,
    OPENING,
    NORMAL,
    MENU,
    COMMAND,
    CRAFT,
    STOREHOUSE,
    TALK,
    GAMEOVER,
};

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
    // TODO �N���X��
    // �ŏ��͓��S�̂�������悤�ɃJ����������Ă���B
    // Continue��I�񂾂�J�������v���C���[�̈ʒu�Ɉړ�����
    // �J�����̈ړ�����������܂ł̓^�C�g����ʂ̒S���͈́B
    // Start��I�񂾂�Ó]����Opening���
    // �Z�[�u�f�[�^����������A�^�C�g����ʂ��\�����ꂽ�Ƃ����łɃZ�[�u�f�[�^���ǂ܂�Ă���B
    // �Z�[�u�f�[�^������̂ɏ��߂����I�񂾂�A�����f�[�^�͓ǂ�ł��Ȃ��̂�
    // ���邭���\�����ēǂݒ����K�v������B
    //----------------------------------------------------------
    enum class eTitleMenu
    {
        NOT_DECIDE,
        START,
        CONTINUE,
        EXIT,
    };
    void InitTitle();
    void OperateTitle(eSequence* sequence);
    void RenderTitle();
    void FinalizeTitle();
    CommandManager* m_titleCommand;
    Sprite* m_sprTitleBack = nullptr;
    Sprite* m_sprTitleLogo = nullptr;
    Sprite* m_sprTitleClock = nullptr;
    Sprite* m_sprTitleLoading = nullptr;
    LPD3DXFONT m_titleFont = nullptr;
    const int TITLE_FADE_IN = 60;
    int m_titleFadeInCount = 0;
    int m_titleFadeInAlpha = 0;
    bool m_bTitleFadeIn = false;
    std::thread* m_titleThread = nullptr;
    std::atomic<bool> m_titleLoaded = false;
    eTitleMenu m_eTitleMenu = eTitleMenu::NOT_DECIDE;
    int m_titleCameraFadeCount = 0;
    bool m_bTitleCameraFade = false;
    bool m_bTitleLoading = false;
    bool m_bTitleFadeOut = false;
    const int TITLE_FADE_OUT = 60;
    int m_titleFadeOutCount = 0;

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
    //----------------------------------------------------------
    SeqOpening* m_Opening = nullptr;
};

