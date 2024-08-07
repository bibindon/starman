#include "Sprite.h"

#include <vector>

using std::string;
using std::vector;

Sprite::Sprite(const LPDIRECT3DDEVICE9 D3DDevice, string filename)
    : m_D3DDevice { D3DDevice }
{
    // sprite setup
    LPD3DXSPRITE tempSprite { nullptr };
    if (FAILED(D3DXCreateSprite(m_D3DDevice, &m_D3DSprite)))
    {
        throw std::exception("Failed to create a sprite.");
    }

    // texture setup
    if (FAILED(D3DXCreateTextureFromFile(
        m_D3DDevice,
        filename.c_str(),
        &m_D3DTexture)))
    {
        throw std::exception("Failed to create a texture.");
    }

    D3DSURFACE_DESC desc { };
    if (FAILED(m_D3DTexture->GetLevelDesc(0, &desc)))
    {
        throw std::exception("Failed to create a texture.");
    }
    m_width = desc.Width;
    m_height = desc.Height;
}
void Sprite::Render(const D3DXVECTOR3& pos, const BYTE transparency)
{
    m_D3DSprite->Begin(D3DXSPRITE_ALPHABLEND);
    RECT rect = {
        0,
        0,
        static_cast<LONG>(m_width),
        static_cast<LONG>(m_height) };
    D3DXVECTOR3 center { 0, 0, 0 };
    m_D3DSprite->Draw(
        m_D3DTexture,
        &rect,
        &center,
        &pos,
        D3DCOLOR_ARGB(transparency, 255, 255, 255));
    m_D3DSprite->End();
}


