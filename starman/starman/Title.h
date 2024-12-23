#pragma once

#include "Common.h"
#include "Sprite.h"
#include "CommandManager.h"
#include "SeqBattle.h"
#include <thread>
#include <atomic>

//----------------------------------------------------------
// タイトル画面
//
// ・最初は島全体が見えるようにカメラが離れている。
// Continueを選んだらカメラがプレイヤーの位置に移動する
// カメラの移動が完了するまではタイトル画面の担当範囲。
// Startを選んだら暗転してOpening画面
// セーブデータがあったら、タイトル画面が表示されたときすでにセーブデータが読まれている。
// セーブデータがあるのに初めからを選んだら、初期データは読んでいないので
// くるくるを表示して読み直す必要がある。
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

