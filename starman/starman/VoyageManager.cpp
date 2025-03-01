#include "VoyageManager.h"
#include "KeyBoard.h"
#include "SharedObj.h"
#include "Mouse.h"
#include "GamePad.h"

void VoyageManager::Update(eBattleState* state)
{
    auto ppos = SharedObj::GetPlayer()->GetPos();

    Voyage()->SetCurrentRaftCoord(ppos.x, ppos.y, ppos.z);

    //----------------------------------------------------
    // マウス・キーボード操作
    //----------------------------------------------------
    if (KeyBoard::IsDownFirstFrame(DIK_C))
    {
        *state = eBattleState::COMMAND;
    }

    if (KeyBoard::IsDownFirstFrame(DIK_ESCAPE))
    {
        *state = eBattleState::NORMAL;

        Voyage()->SetRaftMode(false);
    }

    //--------------------------------------------
    // 右クリックで右のオールを漕ぐ
    // 左クリックで左のオールを漕ぐ
    // 左右同時押しで両方のオールを漕ぐ
    // 同時押しの猶予を0.33秒持たせる。
    // 長押しは扱わない。一回漕ぐたびに一回クリックする必要がある。
    //--------------------------------------------

    static int counterLeft = 0;
    static int counterRight = 0;

    static bool pendingLeft = false;
    static bool pendingRight = false;

    bool bothClick = false;

    if (Mouse::IsDownLeft())
    {
        counterLeft = 0;
        pendingLeft = true;

        if (pendingRight)
        {
            bothClick = true;
        }
    }

    if (Mouse::IsDownRight())
    {
        counterRight = 0;
        pendingRight = true;

        if (pendingLeft)
        {
            bothClick = true;
        }
    }

    //----------------------------------------------------
    // ゲームパッド操作
    //----------------------------------------------------

    if (GamePad::IsDown(eGamePadButtonType::L1))
    {
        counterLeft = 0;
        pendingLeft = true;

        if (pendingRight)
        {
            bothClick = true;
        }
    }

    if (GamePad::IsDown(eGamePadButtonType::R1))
    {
        counterRight = 0;
        pendingRight = true;

        if (pendingLeft)
        {
            bothClick = true;
        }
    }

    if (GamePad::IsDown(eGamePadButtonType::BACK))
    {
        *state = eBattleState::COMMAND;
    }

    if (GamePad::IsDown(eGamePadButtonType::B))
    {
        *state = eBattleState::NORMAL;

        Voyage()->SetRaftMode(false);
    }

    if (pendingLeft)
    {
        ++counterLeft;
        if (counterLeft >= 20)
        {
            if (bothClick)
            {
                Voyage()->PullBothOar();
            }
            else
            {
                Voyage()->PullLeftOar();
            }

            pendingLeft = false;
            counterLeft = 0;

            pendingRight = false;
            counterRight = 0;
        }
    }

    if (pendingRight)
    {
        ++counterRight;
        if (counterRight >= 20)
        {
            if (bothClick)
            {
                Voyage()->PullBothOar();
            }
            else
            {
                Voyage()->PullRightOar();
            }

            pendingLeft = false;
            counterLeft = 0;

            pendingRight = false;
            counterRight = 0;
        }
    }



    // 風の強さと方向により流される


    // 潮の強さと方向により流される


    // オールを漕いだことによりイカダが進む

    // 衝突判定

    // イカダで川を進むことも出来ることに注意
}

void VoyageManager::Draw()
{
    // TODO
}

NSStarmanLib::Voyage* VoyageManager::Voyage()
{
    return NSStarmanLib::Voyage::Get();
}
