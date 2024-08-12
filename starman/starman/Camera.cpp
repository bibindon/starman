#include "Camera.h"
#include <cmath>
#include "Mouse.h"
#include "JoyStick.h"

const D3DXVECTOR3 Camera::UPWARD { 0.0f, 1.0f, 0.0f };
D3DXVECTOR3 Camera::m_eyePos { 0.0f, 3.0f, -2.0f };
D3DXVECTOR3 Camera::m_lookAtPos { 0.0f, 0.0f, 0.0f };
float Camera::m_viewAngle { D3DX_PI / 4 };
float Camera::m_radian { D3DX_PI*3 / 4 };
float Camera::m_y { 3.f };

D3DXMATRIX Camera::GetViewMatrix()
{
    D3DXMATRIX viewMatrix { };
    D3DXMatrixLookAtLH(&viewMatrix,
        &m_eyePos,
        &m_lookAtPos,
        &UPWARD);
    return viewMatrix;
}

D3DXMATRIX Camera::GetProjMatrix()
{
    D3DXMATRIX projection_matrix { };
    D3DXMatrixPerspectiveFovLH(
        &projection_matrix,
        m_viewAngle,
        static_cast<float>(1920) / 1080, /* TODO */
        0.1f,
        500.0f);
    return projection_matrix;
}

void Camera::SetPos(const D3DXVECTOR3& lookAtPos)
{
    m_lookAtPos = lookAtPos;
}

float Camera::GetRadian()
{
    return m_radian;
}

void Camera::Update()
{
    LONG x = Mouse::GetX();
    LONG y = Mouse::GetY();
    float joyX { 0.0f };
    if (JoyStick::IsHold(eJoyStickButtonType::Z_LEFT))
    {
        joyX = 0.1f;
    }
    else if (JoyStick::IsHold(eJoyStickButtonType::Z_RIGHT))
    {
        joyX = -0.1f;
    }
    m_radian += x/300.f;
    m_radian += joyX;
    //m_eyePos.x = m_lookAtPos.x + std::sin(m_radian)*10;
    //m_eyePos.z = m_lookAtPos.z + std::cos(m_radian)*10;
    //m_eyePos.y = m_lookAtPos.y + y/100.f;
    m_y += y / 100.f;
    m_eyePos.y = m_lookAtPos.y + m_y;
    if (m_eyePos.y <= -9.f)
    {
        m_eyePos.y = -9.f;
        m_y = -9.f;
    }
    else if (9.f <= m_eyePos.y)
    {
        m_eyePos.y = 9.f;
        m_y = 9.f;
    }
    if (0.f <= m_eyePos.y)
    {
        m_eyePos.x = m_lookAtPos.x + std::sin(m_radian)*(10-((m_y/3)*(m_y/3)));
        m_eyePos.z = m_lookAtPos.z + std::cos(m_radian)*(10-((m_y/3)*(m_y/3)));
    }
    else
    {
        m_eyePos.x = m_lookAtPos.x + std::sin(m_radian)*(10-((m_y/3)*(m_y/3)));
        m_eyePos.z = m_lookAtPos.z + std::cos(m_radian)*(10-((m_y/3)*(m_y/3)));
    }
}
