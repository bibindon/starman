#include "VoyageManager.h"
#include "KeyBoard.h"
#include "SharedObj.h"
#include "Mouse.h"

void VoyageManager::Update(eBattleState* state)
{
    auto ppos = SharedObj::GetPlayer()->GetPos();

    Voyage()->SetCurrentRaftCoord(ppos.x, ppos.y, ppos.z);

    //----------------------------------------------------
    // �}�E�X�E�L�[�{�[�h����
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
    // �E�N���b�N�ŉE�̃I�[���𑆂�
    // ���N���b�N�ō��̃I�[���𑆂�
    // ���E���������ŗ����̃I�[���𑆂�
    // ���������̗P�\��0.33�b��������B
    // �������͈���Ȃ��B��񑆂����тɈ��N���b�N����K�v������B
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

    //----------------------------------------------------
    // �Q�[���p�b�h����
    //----------------------------------------------------







    // ���̋����ƕ����ɂ�藬�����


    // ���̋����ƕ����ɂ�藬�����


    // �I�[���𑆂������Ƃɂ��C�J�_���i��

    // 
}

NSStarmanLib::Voyage* VoyageManager::Voyage()
{
    return NSStarmanLib::Voyage::Get();
}
