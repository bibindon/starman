#include "AnimController.h"
#include "Common.h"

AnimController::AnimController()
{
    m_animSpeed = Common::ANIMATION_SPEED;
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
    float workAnimTime = 0.f;
    workAnimTime = m_animationTime + m_animSpeed;

    // 通常の更新処理。アニメを進める
    if (workAnimTime < m_animSettingMap[m_animName].m_duration)
    {
        m_animationTime += m_animSpeed;
        m_controller->SetTrackPosition(0, m_animSettingMap[m_animName].m_startPos);
        m_controller->AdvanceTime(m_animationTime, nullptr);
    }
    else
    {
        //-------------------------------------------------------------
        // アニメーションを最後まで実行した場合。
        // 最初に戻るか、ループするか、最後の状態で止まる。
        // 例：
        // 攻撃→最初に戻る
        // 歩く→ループ
        // 死亡→最後の状態で止まる
        //-------------------------------------------------------------

        // 最初に戻る
        if (m_animSettingMap[m_animName].m_loop == false &&
            m_animSettingMap[m_animName].m_stopEnd == false)
        {
            m_animName = m_animSettingMap.begin()->first;
            m_animationTime = 0.f;
            m_controller->SetTrackPosition(0, m_animSettingMap[m_animName].m_startPos);
            m_controller->AdvanceTime(m_animationTime, nullptr);
        }
        // ループ
        else if (m_animSettingMap[m_animName].m_loop)
        {
            m_animationTime = 0.f;
            m_controller->SetTrackPosition(0, m_animSettingMap[m_animName].m_startPos);
            m_controller->AdvanceTime(m_animationTime, nullptr);
        }
        // 最後の状態で止まる
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
    m_animSpeed = speed;
}
