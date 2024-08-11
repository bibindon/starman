#pragma once

#include "Common.h"
#include "Sprite.h"

class SeqTitle
{
public:
    SeqTitle();
    ~SeqTitle();
    void Update(eSequence* sequence);
    void Render();
private:
    Sprite* m_sprite1 { nullptr };
    Sprite* m_sprite2 { nullptr };
    Sprite* m_sprite3 { nullptr };
    Sprite* m_spriteCursor { nullptr };
    enum class eMenu
    {
        START,
        CONTINUE,
        EXIT,
    };
    eMenu m_eMenu { eMenu::START };
};

