#pragma once

#include <d3d9.h>
#include "Common.h"
#include "Sprite.h"
#include "Mesh.h"
#include "AnimMesh.h"
#include "Player.h"
#include "Enemy.h"

class SeqBattle
{
public:
    SeqBattle();
    ~SeqBattle();
    void Update(eSequence* sequence);
    void Render();
private:
    Player* m_player { nullptr };
    Enemy* m_enemy { nullptr };
    Mesh* m_mesh1 { nullptr };
    Mesh* m_meshSky { nullptr };
    Mesh* m_meshSky2 { nullptr };
    Mesh* m_test { nullptr };
    AnimMesh* m_test2 { nullptr };
};

