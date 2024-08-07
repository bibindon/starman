#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <string>

class Sprite
{
public:
    Sprite(const LPDIRECT3DDEVICE9 D3DDevice, std::string filename);
    // ??? pos.x shows z-order. That should be 0.0f ~ 1.0f.
    void Render(const D3DXVECTOR3& pos, const BYTE transparency = 255);
private:
    LPDIRECT3DDEVICE9 m_D3DDevice;
    LPD3DXSPRITE m_D3DSprite;
    LPDIRECT3DTEXTURE9 m_D3DTexture;

    UINT m_width { };
    UINT m_height { };
};

