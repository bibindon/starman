#pragma once
#include "Sprite.h"
class SeqTitle
{
public:
    SeqTitle(LPDIRECT3DDEVICE9 D3DDevice);
    ~SeqTitle();
    void Update();
    void Render();
private:
    LPDIRECT3DDEVICE9 m_D3DDevice;
    Sprite* m_sprite1 { nullptr };
    Sprite* m_sprite2 { nullptr };
    Sprite* m_spriteCursor { nullptr };
};

