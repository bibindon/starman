#pragma once

#include "AnimMesh.h"

class Player
{
public:
    Player();
    ~Player();
    void Render();
private:
    AnimMesh* m_AnimMesh2 { nullptr };
};

