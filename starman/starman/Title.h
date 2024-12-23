#pragma once

#include "Common.h"
#include "Sprite.h"
#include "CommandManager.h"
#include "SeqBattle.h"
#include <thread>
#include <atomic>

//----------------------------------------------------------
// �^�C�g�����
//
// �E�ŏ��͓��S�̂�������悤�ɃJ����������Ă���B
// Continue��I�񂾂�J�������v���C���[�̈ʒu�Ɉړ�����
// �J�����̈ړ�����������܂ł̓^�C�g����ʂ̒S���͈́B
// Start��I�񂾂�Ó]����Opening���
// �Z�[�u�f�[�^����������A�^�C�g����ʂ��\�����ꂽ�Ƃ����łɃZ�[�u�f�[�^���ǂ܂�Ă���B
// �Z�[�u�f�[�^������̂ɏ��߂����I�񂾂�A�����f�[�^�͓ǂ�ł��Ȃ��̂�
// ���邭���\�����ēǂݒ����K�v������B
//----------------------------------------------------------

class Title
{
public:
    Title();
    ~Title();
    void Update(eSequence* sequence, eBattleState* eState);
    void Render();
private:
    Sprite* m_sprite1 { nullptr };
    Sprite* m_sprite3 { nullptr };
    Sprite* m_spriteBlack { nullptr };
    Sprite* m_spriteTimer { nullptr };

    CommandManager* m_titleCommand;

    Sprite* m_sprBack = nullptr;
    Sprite* m_sprLogo = nullptr;
    Sprite* m_sprClock = nullptr;
    Sprite* m_sprLoading = nullptr;
    LPD3DXFONT m_font = nullptr;

    bool m_bSavedataExists = false;

    const int FADE_IN = 60;
    int m_fadeInCount = 0;
    int m_fadeInAlpha = 0;
    bool m_bFadeIn = false;

    std::thread* m_thread = nullptr;
    std::atomic<bool> m_loaded = false;
    bool m_bLoading = false;

    eTitleMenu m_eMenu = eTitleMenu::NOT_DECIDE;

    int m_cameraFadeCount = 0;
    bool m_bCameraFade = false;

    bool m_bFadeOut = false;
    const int FADE_OUT = 60;
    int m_fadeOutCount = 0;
};

