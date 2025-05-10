// �J�������ɓK���ɍ�����܂�
// ���낻�낿���Ƃ����ق��������C������B


#include "Camera.h"

#include "SharedObj.h"
#include <cmath>
#include "Mouse.h"
#include "GamePad.h"
#include <string>

const D3DXVECTOR3 Camera::UPWARD (0.0f, 1.0f, 0.0f);
// m_eyePos�ɉ����Z�b�g���Ă����_�͕ς��Ȃ��B���_��m_radian�ɂ���Č��܂�B
D3DXVECTOR3 Camera::m_eyePos(0.f, 0.f, 0.f);
D3DXVECTOR3 Camera::m_lookAtPos(0.0f, 0.0f, 0.0f);
float Camera::m_viewAngle = D3DX_PI / 4;
// m_radian == D3DX_PI * 3 / 2�̎��i270�x�̎��j�J�����͐��ʂ�����
float Camera::m_radian = D3DX_PI * 3 / 2;
float Camera::m_y = 3.f;
bool Camera::m_sleepMode = false;
bool Camera::m_houseMode = false;

eCameraMode Camera::m_eCameraMode;

int Camera::m_counter = 0;
int Camera::MOVE_COUNT_MAX = 240;

D3DXMATRIX Camera::GetViewMatrix()
{
    D3DXMATRIX viewMatrix { };
    D3DXMatrixLookAtLH(&viewMatrix, &m_eyePos, &m_lookAtPos, &UPWARD);
    return viewMatrix;
}

D3DXMATRIX Camera::GetProjMatrix()
{
    D3DXMATRIX projection_matrix { };
    D3DXMatrixPerspectiveFovLH(&projection_matrix,
                               m_viewAngle,
                               static_cast<float>(1920) / 1080, /* TODO */
                               1.f,
                               1000000.0f);

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
    if (m_eCameraMode == eCameraMode::SLEEP)
    {
        // do nothing
    }
    else if (m_eCameraMode == eCameraMode::TITLE)
    {
        m_eyePos.x = -4000.f;
        m_eyePos.z = -1000.f;
        m_eyePos.y = 300.f;
    }
    else if (m_eCameraMode == eCameraMode::BATTLE)
    {
        LONG x = Mouse::GetXDelta();
        float y = (float)Mouse::GetYDelta();
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
        x *= -1; // �������t�ɂ���

        // x��y��10����1�ɂ���΃����[�g�f�X�N�g�b�v�ł��܂Ƃ��ɑ���ł���
        if (Common::IsRemoteSession())
        {
            x /= 20;
            y /= 20;
        }

        m_radian += x/500.f;
        m_radian += joyX;

        y /= 100.f;

        float temp = m_lookAtPos.y + m_y + y;

        if (!m_houseMode)
        {

            if ( m_lookAtPos.y - 5.f < temp && temp < m_lookAtPos.y + 5.f)
            {
                m_y += y;
            }
            else if (temp <= m_lookAtPos.y - 5.f)
            {
                m_y = -5.f;
            }
            else if (m_lookAtPos.y + 5.f <= temp)
            {
                m_y = 5.f;
            }

            m_eyePos.y = m_lookAtPos.y + m_y;

            m_eyePos.x = m_lookAtPos.x + std::cos(m_radian)*(5-((m_y*0.44f)*(m_y*0.44f)));
            m_eyePos.z = m_lookAtPos.z + std::sin(m_radian)*(5-((m_y*0.44f)*(m_y*0.44f)));
        }
        else
        {
            if ( m_lookAtPos.y - 3.f < temp && temp < m_lookAtPos.y + 3.f)
            {
                m_y += y;
            }
            else if (temp <= m_lookAtPos.y - 3.f)
            {
                m_y = -3.f;
            }
            else if (m_lookAtPos.y + 3.f <= temp)
            {
                m_y = 3.f;
            }

            m_eyePos.y = m_lookAtPos.y + m_y;

            m_eyePos.x = m_lookAtPos.x + std::cos(m_radian)*(3-((m_y*0.33f)*(m_y*0.33f)));
            m_eyePos.z = m_lookAtPos.z + std::sin(m_radian)*(3-((m_y*0.33f)*(m_y*0.33f)));
        }
    }
    else if (m_eCameraMode == eCameraMode::TITLE_TO_BATTLE)
    {
        ++m_counter;

        auto playerPos = SharedObj::GetPlayer()->GetPos();
        playerPos.x += 0;
        playerPos.z += -4;
        playerPos.y += 4;
        float temp = std::sin((float)m_counter / MOVE_COUNT_MAX * D3DX_PI / 2);

        float x = -4000 - ((-4000 - playerPos.x)  * temp);
        float z = -1000 - ((-1000 - playerPos.z)  * temp);
        float y = 300 - ((300 - playerPos.y)  * temp);

        m_eyePos.x = x;
        m_eyePos.z = z;
        m_eyePos.y = y;
    }
    else if (m_eCameraMode == eCameraMode::BATTLE_TO_TITLE)
    {
        ++m_counter;

        auto playerPos = SharedObj::GetPlayer()->GetPos();
        playerPos.x += 0;
        playerPos.z += -4;
        playerPos.y += 4;
        float temp = std::sin((float)m_counter / MOVE_COUNT_MAX * D3DX_PI / 2);

        float x = playerPos.x - ((playerPos.x - (-4000))  * temp);
        float z = playerPos.z - ((playerPos.z - (-1000))  * temp);
        float y = playerPos.y - ((playerPos.y - 300)  * temp);

        m_eyePos.x = x;
        m_eyePos.z = z;
        m_eyePos.y = y;
    }

    if (!"Output camera eyepos")
    {
        std::string work;
        work += "m_eyePos.x: " + std::to_string(m_eyePos.x) + ", ";
        work += "m_eyePos.y: " + std::to_string(m_eyePos.y) + ", ";
        work += "m_eyePos.z: " + std::to_string(m_eyePos.z) + "\n";
        OutputDebugString(work.c_str());
    }
    
    if (!"Output camera lookAtPos")
    {
        std::string work;
        work += "m_lookAtPos.x: " + std::to_string(m_lookAtPos.x) + ", ";
        work += "m_lookAtPos.y: " + std::to_string(m_lookAtPos.y) + ", ";
        work += "m_lookAtPos.z: " + std::to_string(m_lookAtPos.z) + "\n";
        OutputDebugString(work.c_str());
    }
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

    POINT p { };
    if (matrix._44 < 0.f)
    {
        p.x = -1;
        p.y = -1;
    }
    else
    {
        p.x = static_cast<int>(matrix._41 / matrix._44);
        p.y = static_cast<int>(matrix._42 / matrix._44);
    }
    return p;
}

void Camera::SetCameraMode(const eCameraMode mode)
{
    m_eCameraMode = mode;
    m_counter = 0;
}

void Camera::SetHouseMode(const bool arg)
{
    m_houseMode = arg;
}

