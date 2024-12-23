#pragma once
#include <d3dx9.h>
class Camera
{
public:
    static D3DXMATRIX GetViewMatrix();
    static D3DXMATRIX GetProjMatrix();
    static void SetLookAtPos(const D3DXVECTOR3& lookAtPos);
    static D3DXVECTOR3 GetEyePos();
    static float GetRadian();
    static void SetRadian(const float radian);
    static void Update();
    static POINT GetScreenPos(const D3DXVECTOR3& world);

    // �}�E�X�𓮂����Ă��J�����������Ȃ��悤�ɂ���B
    // ���j���[��ʂ�\�����Ă���Ƃ��̓}�E�X�𓮂����Ă��J�����������Ȃ��ŗ~�������߁B
    static void SleepModeON();
    static void SleepModeOFF();

    static void SetTitleMode(const bool arg);

private:

    const static D3DXVECTOR3 UPWARD;
    static D3DXVECTOR3 m_eyePos;
    static D3DXVECTOR3 m_lookAtPos;
    static float m_viewAngle;
    static float m_radian;
    static float m_y;
    static bool m_sleepMode;

    static bool m_titleMode;
};

