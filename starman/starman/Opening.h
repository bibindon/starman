#pragma once

#include "Common.h"
#include "Sprite.h"

#include "..\..\StoryTelling\StoryTelling\StoryTelling.h"

class Opening
{
public:
    Opening();
    ~Opening();
    void Update(eBattleState* eState);
    void Render();
private:

    NSStoryTelling::StoryTelling* m_storyTelling { nullptr };

    bool m_firstPage = true;
    bool m_bPlay = false;

    // ƒJƒƒ‰ˆÚ“®‘O‚É2•b‘Ò‚Â
    bool m_bStopBeforeCameraFade = false;
    int m_stopBeforeCameraFade = 0;
    const int WAIT_MAX = 120;

    bool m_bCameraFadeOut = false;
    int m_cameraFadeOutCnt = 0;

};

