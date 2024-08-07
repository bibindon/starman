#pragma once
#include <d3dx9.h>
class Camera
{
public:
    static D3DXMATRIX GetViewMatrix();
    static D3DXMATRIX GetProjMatrix();
    static void SetPos(
        const D3DXVECTOR3& eyePos, const D3DXVECTOR3& lookAtPos, const float viewAngle);

private:

    const static D3DXVECTOR3 UPWARD;
    static D3DXVECTOR3 m_eyePos;
    static D3DXVECTOR3 m_lookAtPos;
    static float m_viewAngle;
};

