#pragma once

#include "../../StarmanLib/StarmanLib/StarmanLib/Voyage.h"
#include "Common.h"

class VoyageManager
{
public:
    void Update(eBattleState* state);

private:

    NSStarmanLib::Voyage* Voyage();

};

