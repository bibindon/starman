#pragma once

#include "Common.h"
#include "Sprite.h"

class SeqEnding
{
public:
    SeqEnding();
    ~SeqEnding();
    void Update(eSequence* sequence);
    void Render();
private:
    Sprite* m_sprite1 { nullptr };
    Sprite* m_sprite2 { nullptr };
    Sprite* m_sprite3 { nullptr };
    Sprite* m_spriteBlack { nullptr };
    int m_nIndex { 0 };
    bool m_bFadeIn { true };
    bool m_bFadeOut { false };
    int m_fadeInCount { 0 };
    int m_fadeOutCount { 0 };
    const int FADE_IN_COUNT { 30 };
    const int FADE_OUT_COUNT { 30 };
};

