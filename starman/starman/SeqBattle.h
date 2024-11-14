#pragma once

#include <d3d9.h>
#include "Common.h"
#include "Sprite.h"
#include "Mesh.h"
#include "AnimMesh.h"
#include "Player.h"
#include "Enemy.h"
#include "Stage1.h"
#include "Stage2.h"
#include "Stage3.h"
#include "Stage4.h"
#include "Stage5.h"
#include "Stage8.h"
#include "Stage7.h"
#include "Stage6.h"
#include "Stage10.h"
#include "Stage9.h"

#include "..\..\talklib2\talklib2\talklib2.h"
#include "..\..\quest-system\QuestSystem\QuestSystem\QuestSystem.h"

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
    Stage1* m_stage1 { };
    Stage2* m_stage2 { };
    Stage3* m_stage3 { };
    Stage4* m_stage4 { };
    Stage5* m_stage5 { };
    Stage6* m_stage6 { };
    Stage7* m_stage7 { };
    Stage8* m_stage8 { };
    Stage9* m_stage9 { };
    Stage10* m_stage10 { };

    void SaveLastStage(const int);

    NSTalkLib2::Talk* m_talk { nullptr };
    bool m_bTalkFinish { false };
};

