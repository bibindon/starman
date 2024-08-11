#pragma once

#include "Common.h"
#include "Sprite.h"

class SeqOpening
{
public:
    SeqOpening();
    ~SeqOpening();
    void Update(eSequence* sequence);
    void Render();
private:
    Sprite* m_sprite1 { nullptr };
    Sprite* m_sprite2 { nullptr };
    Sprite* m_sprite3 { nullptr };
    int m_nIndex { 0 };
};

