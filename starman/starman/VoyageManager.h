#pragma once

#include "../../StarmanLib/StarmanLib/StarmanLib/Voyage.h"
#include "Common.h"

class VoyageManager
{
public:
    void Update(eBattleState* state);
    void Draw();

private:

    NSStarmanLib::Voyage* Voyage();

};

