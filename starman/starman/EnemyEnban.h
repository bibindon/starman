#pragma once

#include "AnimMesh.h"
#include "Sprite.h"
#include "EnemyBase.h"

#include <thread>
#include <d3d9.h>
#include <mutex>

class EnemyEnban : public EnemyBase
{
public:
    EnemyEnban();
    ~EnemyEnban() override;
    bool Init() override;
    void Update() override;
};

