#pragma once


#include "Common.h"
#include "Sprite.h"

#include "..\..\StoryTelling\StoryTelling\StoryTelling.h"

class StoryManager
{
public:
    StoryManager(const std::string& csvFile);
    ~StoryManager();
    void Update();
    void Render();
    bool IsFinish();

private:

    NSStoryTelling::StoryTelling* m_storyTelling { nullptr };

    bool m_firstPage { true };
    bool m_bPlay { false };
    bool bFinish { false };
};

