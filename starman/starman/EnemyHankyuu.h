#pragma once

#include "AnimMesh.h"
#include "Sprite.h"
#include "EnemyBase.h"

#include <thread>
#include <d3d9.h>
#include <mutex>

class EnemyHankyuu : public EnemyBase
{
public:
    EnemyHankyuu();
    ~EnemyHankyuu() override;
    bool Init() override;
    void Update() override;
};

