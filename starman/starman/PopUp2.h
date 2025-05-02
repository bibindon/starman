#pragma once

#include "Sprite.h"
#include <vector>
#include <string>
#include <queue>

class IPopUpFont
{
public:
    virtual void Draw(const std::string& text, const int transparent) = 0;
    virtual ~IPopUpFont() { };
};

class PopUpFont : public IPopUpFont
{
public:
    PopUpFont(LPDIRECT3DDEVICE9 device, const bool bEnglish);
    void Draw(const std::string& text, const int transparent);
    ~PopUpFont();

private:
    LPD3DXFONT m_D3DFont = nullptr;
};

// �A�C�e�����E�����Ƃ��̉��o
// �������1.5�b�\������
// �\�����I���O�Ɏ��̕����񂪓n���ꂽ��L���[�C���O�����
class PopUp2
{
public:
    static void Init(IPopUpFont* font);
    static PopUp2* Get();
    static void Finalize();
    void SetText(const std::string& arg);
    void SetText(const UINT res);
    bool IsShow();
    void Update();
    void Render();
    void Cancel();
    ~PopUp2();

private:
    static PopUp2* m_singleTonObj;

    std::queue<std::string> m_textQ;
    int m_counter { 0 };
    bool m_isShow { false };
    const int FADE_IN = 30;
    const int SHOW = 30;
    const int FADE_OUT= 30;
    int m_transparent = 0;

    IPopUpFont* m_font = nullptr;
};


