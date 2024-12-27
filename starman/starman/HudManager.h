#pragma once

#include "..\..\hud\hud\hud.h"

class HudManager
{
public:
    void Init();
    void Finalize();
    void Update();
    void Draw();
private:
    NSHud::hud* m_hud = nullptr;
};

