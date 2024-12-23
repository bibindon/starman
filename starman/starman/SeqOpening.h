#pragma once

#include "Common.h"
#include "Sprite.h"

#include "..\..\StoryTelling\StoryTelling\StoryTelling.h"

class SeqOpening
{
public:
    SeqOpening();
    ~SeqOpening();
    void Update(eBattleState* eState);
    void Render();
private:

    NSStoryTelling::StoryTelling* m_storyTelling { nullptr };

    bool m_firstPage { true };
    bool m_bPlay { false };
    bool bFinish { false };
};

