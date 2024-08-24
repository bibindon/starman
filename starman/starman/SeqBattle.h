#pragma once

#include <d3d9.h>
#include "Common.h"
#include "Sprite.h"
#include "Mesh.h"
#include "AnimMesh.h"
#include "Player.h"
#include "Enemy.h"
#include "Stage1.h"

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
//    std::vector<Enemy> m_vecEnemy { };
    Enemy* m_enemy { nullptr };
    Mesh* m_mesh1 { nullptr };
    Mesh* m_mesh2 { nullptr };
    Mesh* m_meshSky { nullptr };
    Mesh* m_meshSky2 { nullptr };
    Mesh* m_meshTest { nullptr };
    Sprite* m_spriteGameover { nullptr };
    eBattleState m_eState { eBattleState::NORMAL };
    int m_nGameoverCounter { 0 };
    Stage1 m_stage1 { };
};

