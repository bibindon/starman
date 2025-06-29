﻿#pragma once

#include "Common.h"
#include "Sprite.h"

#include "..\..\StoryTelling\StoryTelling\StoryTelling.h"

class SeqEnding
{
public:
    SeqEnding();
    ~SeqEnding();
    void Update(eSequence* sequence);
    void Render();
private:

    NSStoryTelling::StoryTelling* m_storyTelling { nullptr };

    bool m_firstPage { true };
    bool m_bPlay { false };
    bool m_bFinish { false };
    bool m_bTrueEnding = false;

    ::Sprite* m_spriteEnd = nullptr;
};

