#pragma once

#include "Sprite.h"
#include <vector>
#include <string>
#include <queue>

// �A�C�e�����E�����Ƃ��̉��o
// �������1.5�b�\������
// �\�����I���O�Ɏ��̕����񂪓n���ꂽ��L���[�C���O�����
class PopUp2
{
public:
    static void Init();
    static PopUp2* Get();
    static void Finalize();
    void SetText(const std::string& arg);
    bool IsShow();
    void Update();
    void Render();
    void Cancel();
    ~PopUp2();

    // �e�X�g�p
    std::string ForTest_GetText();

private:
    static PopUp2* m_singleTonObj;

    LPD3DXFONT m_D3DFont { nullptr };
    std::queue<std::string> m_textQ;
    int m_counter { 0 };
    bool m_isShow { false };
    const int FADE_IN = 30;
    const int SHOW = 30;
    const int FADE_OUT= 30;
    int m_transparent = 0;
};


