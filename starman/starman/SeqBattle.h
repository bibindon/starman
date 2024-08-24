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

enum class eBattleState
{
    NORMAL,
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
};

