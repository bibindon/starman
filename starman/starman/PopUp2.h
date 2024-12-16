#pragma once

#include "Sprite.h"
#include <vector>
#include <string>
#include <queue>

// アイテムを拾ったときの演出
// 文字列を1.5秒表示する
// 表示が終わる前に次の文字列が渡されたらキューイングされる
class PopUp2
{
public:
    static void Init();
    static PopUp2* Get();
    void SetText(const std::string& arg);
    bool IsShow();
    void Update();
    void Render();
    void Cancel();
    ~PopUp2();

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


