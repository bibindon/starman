#include "Camera.h"

D3DXMATRIX Camera::m_viewMatrix;
D3DXMATRIX Camera::m_projMatrix;
const D3DXVECTOR3 Camera::UPWARD { 0.0f, 1.0f, 0.0f };
D3DXVECTOR3 Camera::m_eyePos { 0.0f, 2.0f, -2.0f };
D3DXVECTOR3 Camera::m_lookAtPos { 0.0f, 0.0f, 0.0f };
float Camera::m_viewAngle { D3DX_PI / 4 };

D3DXMATRIX Camera::GetViewMatrix()
{
    D3DXMATRIX viewMatrix { };
    D3DXMatrixLookAtLH(&viewMatrix,
        &m_eyePos,
        &m_lookAtPos,
        &UPWARD);
    return viewMatrix;

//    return m_viewMatrix;
}

void Camera::SetViewMatrix(const D3DXMATRIX& matrix)
{
    m_viewMatrix = matrix;
}

D3DXMATRIX Camera::GetProjMatrix()
{
    D3DXMATRIX projection_matrix { };
    D3DXMatrixPerspectiveFovLH(
        &projection_matrix,
        m_viewAngle,
        static_cast<float>(1920) / 1080, /* TODO */
        0.1f,
        3000.0f);
    return projection_matrix;

//    return m_projMatrix;
}

void Camera::SetProjMatrix(const D3DXMATRIX& matrix)
{
    m_projMatrix = matrix;
}

void Camera::SetPos(
    const D3DXVECTOR3& eyePos, const D3DXVECTOR3& lookAtPos, const float viewAngle)
{
    m_eyePos = eyePos;
    m_lookAtPos = lookAtPos;
    m_viewAngle = viewAngle;
}
