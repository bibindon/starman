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
// �EContinue��I�񂾂�J�������v���C���[�̈ʒu�Ɉړ�����
// �E�J�����̈ړ�����������܂ł̓^�C�g����ʂ̒S���͈́B
// �EStart��I�񂾂�Ó]����Opening���
// �E�Z�[�u�f�[�^����������A�^�C�g����ʂ��\�����ꂽ�Ƃ����łɃZ�[�u�f�[�^���ǂ܂�Ă���B
// �E�Z�[�u�f�[�^������̂ɏ��߂����I�񂾂�A�����f�[�^�͓ǂ�ł��Ȃ��̂�
//   ���邭���\�����ēǂݒ����K�v������B
// �E�J�����ړ��̃t�F�[�h�C���E�t�F�[�h�A�E�g�ƈÓ]�ɂ��t�F�[�h�C���E�t�F�[�h�A�E�g������B
//----------------------------------------------------------

class Title
{
public:
    // blackFadeIn: �Ó]�ɂ��t�F�[�h�C�����A�J�����ɂ��t�F�[�h�C�����B
    Title(const bool blackFadeIn);
    ~Title();
    void Update(eSequence* sequence, eBattleState* eState);
    void Render();

    bool GetLoading() const;

private:
    Sprite* m_sprite1 { nullptr };
    Sprite* m_sprite3 { nullptr };
    Sprite* m_spriteBlack { nullptr };
    Sprite* m_spriteTimer { nullptr };

    CommandManager* m_titleCommand = nullptr;

    Sprite* m_sprBack = nullptr;
    Sprite* m_sprLogo = nullptr;
    Sprite* m_sprClock = nullptr;
    Sprite* m_sprLoading = nullptr;
    LPD3DXFONT m_font = nullptr;

    const int FADE_IN = 60;
    bool m_bFadeIn = false;
    int m_fadeInCount = 0;
    int m_fadeInAlpha = 0;

    const int FADE_OUT = 1;
    bool m_bFadeOut = false;
    int m_fadeOutCount = 0;
    int m_fadeOutAlpha = 0;

    bool m_bCameraFadeIn = false;
    int m_cameraFadeInCount = 0;

    bool m_bCameraFadeOut = false;
    int m_cameraFadeOutCount = 0;

    std::thread* m_thread = nullptr;
    std::atomic<bool> m_loaded = false;
    bool m_bLoading = false;

    eTitleMenu m_eMenu = eTitleMenu::NOT_DECIDE;

};

