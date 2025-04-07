#include "AnimController.h"
#include "Common.h"

AnimController::AnimController()
{
    m_animSpeed = Common::ANIMATION_SPEED;

    if (Common::X64Bit())
    {
        m_animSpeed /= 80;
    }
}

AnimController::~AnimController()
{
}

void AnimController::Init(const LPD3DXANIMATIONCONTROLLER controller, const AnimSetMap& animSetMap)
{
    m_controller = controller;
    m_animSettingMap = animSetMap;

    // DirectX9��64bit�Ńr���h�����80�{���ɂȂ��Ă��܂����ߒ��߂���
    if (Common::X64Bit())
    {
        for (auto& animSetting : m_animSettingMap)
        {
            animSetting.second.m_startPos /= 80;
            animSetting.second.m_duration /= 80;
        }
    }
}

void AnimController::SetAnim(const std::string& animName, const DOUBLE& pos)
{
    m_animName = animName;
    if (pos >= 0.f)
    {
        m_animationTime = 0.f;
    }
}

void AnimController::SetAnimSettings(const AnimSetMap& animSetMap)
{
    m_animSettingMap = animSetMap;

    if (Common::X64Bit())
    {
        for (auto& animSetting : m_animSettingMap)
        {
            animSetting.second.m_startPos /= 80;
            animSetting.second.m_duration /= 80;
        }
    }
}

void AnimController::Update()
{
    float workAnimTime = 0.f;
    workAnimTime = m_animationTime + m_animSpeed;

    // �ʏ�̍X�V�����B�A�j����i�߂�
    if (workAnimTime < m_animSettingMap[m_animName].m_duration)
    {
        m_animationTime += m_animSpeed;
        m_controller->SetTrackPosition(0, m_animSettingMap[m_animName].m_startPos);
        m_controller->AdvanceTime(m_animationTime, nullptr);
    }
    else
    {
        //-------------------------------------------------------------
        // �A�j���[�V�������Ō�܂Ŏ��s�����ꍇ�B
        // �ŏ��ɖ߂邩�A���[�v���邩�A�Ō�̏�ԂŎ~�܂�B
        // ��F
        // �U�����ŏ��ɖ߂�
        // ���������[�v
        // ���S���Ō�̏�ԂŎ~�܂�
        //-------------------------------------------------------------

        // �ŏ��ɖ߂�
        if (m_animSettingMap[m_animName].m_loop == false &&
            m_animSettingMap[m_animName].m_stopEnd == false)
        {
            m_animName = m_animSettingMap.begin()->first;
            m_animationTime = 0.f;
            m_controller->SetTrackPosition(0, m_animSettingMap[m_animName].m_startPos);
            m_controller->AdvanceTime(m_animationTime, nullptr);
        }
        // ���[�v
        else if (m_animSettingMap[m_animName].m_loop)
        {
            m_animationTime = 0.f;
            m_controller->SetTrackPosition(0, m_animSettingMap[m_animName].m_startPos);
            m_controller->AdvanceTime(m_animationTime, nullptr);
        }
        // �Ō�̏�ԂŎ~�܂�
        else if (m_animSettingMap[m_animName].m_stopEnd)
        {
            m_controller->SetTrackPosition(0, m_animSettingMap[m_animName].m_startPos);
            m_controller->AdvanceTime(m_animationTime, nullptr);
        }
    }
}

void AnimController::Finalize()
{
    SAFE_RELEASE(m_controller);
}

void AnimController::SetAnimSpeed(const float speed)
{
    if (!Common::X64Bit())
    {
        m_animSpeed = speed;
    }
    else
    {
        m_animSpeed = speed / 80;
    }
}
