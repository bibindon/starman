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

void AnimController::SetAnim(const std::string& animName)
{
    m_animName = animName;
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
            m_animName = "Idle";
        }
    }
    m_controller->AdvanceTime(m_animationTime, nullptr);

    /*
    if (is_playing_)
    {
        float duration { animation_configs_.at(playing_animation_).duration_ };
        if (animation_time_ + 0.001f >= duration)
        {
            set_animation(default_animation_);
            is_playing_ = false;
            animation_time_ = 0;
        }
    }
    */
}
