#include "AnimController.h"
#include "Common.h"

AnimController::AnimController()
{
}

AnimController::~AnimController()
{
}

void AnimController::Init(const LPD3DXANIMATIONCONTROLLER controller, const AnimSetMap& animSetMap)
{
    m_controller = controller;
    m_animSettingMap = animSetMap;
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
}

void AnimController::Update()
{
    m_animationTime += Common::ANIMATION_SPEED;
    m_controller->SetTrackPosition(0, m_animSettingMap[m_animName].m_startPos);

    if (m_animSettingMap[m_animName].m_duration <= m_animationTime)
    {
        m_animationTime = 0.f;
        if (m_animSettingMap[m_animName].m_loop == false)
        {
            m_animName = m_animSettingMap.begin()->first;
        }
    }
    m_controller->AdvanceTime(m_animationTime, nullptr);
}
