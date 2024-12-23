#pragma once

#include "Common.h"
#include "Sprite.h"
#include "CommandManager.h"
#include <thread>
#include <atomic>

class SeqTitle
{
public:
    SeqTitle();
    ~SeqTitle();
    void Update(eSequence* sequence);
    void Render();
private:
    Sprite* m_sprite1 { nullptr };
    Sprite* m_sprite3 { nullptr };
    Sprite* m_spriteBlack { nullptr };
    Sprite* m_spriteTimer { nullptr };
    enum class eMenu
    {
        START,
        CONTINUE,
        EXIT,
    };
    eMenu m_eMenu { eMenu::START };
    bool m_bFadeIn { true };
    bool m_bFadeOut { false };
    int m_fadeInCount { 0 };
    int m_fadeOutCount { 0 };
    const int FADE_IN_COUNT { 60 };
    const int FADE_OUT_COUNT { 60 };

    CommandManager m_commandManager;

    std::thread* m_thread = nullptr;
    std::atomic<bool> m_loaded = false;
    LPD3DXFONT m_font = nullptr;
};

