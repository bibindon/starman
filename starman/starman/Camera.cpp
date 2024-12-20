#include "Camera.h"
#include <cmath>
#include "Mouse.h"
#include "GamePad.h"
#include <string>

const D3DXVECTOR3 Camera::UPWARD { 0.0f, 1.0f, 0.0f };
// m_eyePosに何をセットしても視点は変わらない。視点はm_radianによって決まる。
D3DXVECTOR3 Camera::m_eyePos { 0.f, 0.f, 0.f };
D3DXVECTOR3 Camera::m_lookAtPos { 0.0f, 0.0f, 0.0f };
float Camera::m_viewAngle { D3DX_PI / 4 };
// m_radian == D3DX_PI * 3 / 2の時（270度の時）カメラは正面を向く
float Camera::m_radian { D3DX_PI * 3 / 2 };
float Camera::m_y { 3.f };
bool Camera::m_sleepMode { false };

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
        10000.0f);
    return projection_matrix;
}

void Camera::SetLookAtPos(const D3DXVECTOR3& lookAtPos)
{
    m_lookAtPos = lookAtPos;
}

D3DXVECTOR3 Camera::GetEyePos()
{
    return m_eyePos;
}

float Camera::GetRadian()
{
    return m_radian;
}

void Camera::SetRadian(const float radian)
{
    m_radian = radian;
}

void Camera::Update()
{
    if (m_sleepMode)
    {
        return;
    }
    LONG x = Mouse::GetXDelta();
    LONG y = Mouse::GetYDelta();
    float joyX { 0.0f };
    if (GamePad::IsHold(eGamePadButtonType::Z_LEFT))
    {
        joyX = 0.05f;
    }
    else if (GamePad::IsHold(eGamePadButtonType::Z_RIGHT))
    {
        joyX = -0.05f;
    }
    if (GamePad::IsHold(eGamePadButtonType::Z_UP))
    {
        y = -10;
    }
    else if (GamePad::IsHold(eGamePadButtonType::Z_DOWN))
    {
        y = 10;
    }
    x *= -1; // 正負を逆にする
    // xとyを10分の1にすればリモートデスクトップでもまともに操作できる
    x /= 10;
    y /= 10;
    m_radian += x/500.f;
    m_radian += joyX;
    m_y += y / 100.f;
    m_eyePos.y = m_lookAtPos.y + m_y;
    if (m_eyePos.y <= -9.f)
    {
        m_eyePos.y = -9.f;
        m_y = -9.f;
    }
    std::string radian = std::to_string(m_radian);
    radian = "radian: " + radian + "\n";
    if (0.f <= m_eyePos.y)
    {
        m_eyePos.x = m_lookAtPos.x + std::cos(m_radian)*(5-((m_y/3)*(m_y/3)));
        m_eyePos.z = m_lookAtPos.z + std::sin(m_radian)*(5-((m_y/3)*(m_y/3)));
    }
    else
    {
        m_eyePos.x = m_lookAtPos.x + std::cos(m_radian)*(5-((m_y/3)*(m_y/3)));
        m_eyePos.z = m_lookAtPos.z + std::sin(m_radian)*(5-((m_y/3)*(m_y/3)));
    }
    std::string eysPosX = std::to_string(m_eyePos.x);
    eysPosX = "eysPosX: " + eysPosX + "\n";

    std::string eysPosZ = std::to_string(m_eyePos.z);
    eysPosZ = "eysPosZ: " + eysPosZ + "\n";
}

POINT Camera::GetScreenPos(const D3DXVECTOR3& world)
{
    const D3DXMATRIX view_matrix { GetViewMatrix() };
    const D3DXMATRIX projection_matrix { GetProjMatrix() };
    static const D3DXMATRIX viewport_matrix {
        1600 / 2.0f, 0.0f, 0.0f, 0.0f,
        0.0f, -900 / 2.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        1600 / 2.0f, 900 / 2.0f, 0.0f, 1.0f };
    D3DXMATRIX matrix { };
    D3DXMatrixTranslation(&matrix, world.x, world.y, world.z);
    matrix = matrix * view_matrix * projection_matrix * viewport_matrix;
    return POINT {
        static_cast<int>(matrix._41 / matrix._44),
        static_cast<int>(matrix._42 / matrix._44) };
}

void Camera::SleepModeON()
{
    m_sleepMode = true;
}

void Camera::SleepModeOFF()
{
    m_sleepMode = false;
}
