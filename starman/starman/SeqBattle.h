#pragma once

#include <d3d9.h>
#include "Common.h"
#include "Sprite.h"
#include "Mesh.h"
#include "AnimMesh.h"

class SeqBattle
{
public:
    SeqBattle(LPDIRECT3DDEVICE9 D3DDevice);
    ~SeqBattle();
    void Update(eSequence* sequence);
    void Render();
private:
    LPDIRECT3DDEVICE9 m_D3DDevice { nullptr };
    Mesh* m_mesh1 { nullptr };
    AnimMesh* m_AnimMesh1 { nullptr };
};

