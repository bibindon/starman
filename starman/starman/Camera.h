#pragma once
#include <d3dx9.h>
class Camera
{
public:
    static D3DXMATRIX GetViewMatrix();
    // TODO remove
    static void SetViewMatrix(const D3DXMATRIX& matrix);
    static D3DXMATRIX GetProjMatrix();
    // TODO remove
    static void SetProjMatrix(const D3DXMATRIX& matrix);
    static void SetPos(
        const D3DXVECTOR3& eyePos, const D3DXVECTOR3& lookAtPos, const float viewAngle);

private:
    // TODO remove
    static D3DXMATRIX m_viewMatrix;
    // TODO remove
    static D3DXMATRIX m_projMatrix;

    const static D3DXVECTOR3 UPWARD;
    static D3DXVECTOR3 m_eyePos;
    static D3DXVECTOR3 m_lookAtPos;
    static float m_viewAngle;
};

