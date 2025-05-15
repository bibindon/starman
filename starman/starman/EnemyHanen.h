#pragma once

#include "AnimMesh.h"
#include "Sprite.h"
#include "EnemyBase.h"

#include <thread>
#include <d3d9.h>
#include <mutex>

class EnemyHanen : public EnemyBase
{
public:
    EnemyHanen();
    ~EnemyHanen() override;
    bool Init() override;
    void Update() override;
};

