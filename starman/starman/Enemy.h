#pragma once

#include "AnimMesh.h"
#include "Sprite.h"
#include "EnemyBase.h"

#include <thread>
#include <d3d9.h>
#include <mutex>

// TODO EnemyCube‚É‰ü–¼
class Enemy : public EnemyBase
{
public:
    Enemy();
    ~Enemy() override;
    bool Init() override;
    void Update() override;
};

