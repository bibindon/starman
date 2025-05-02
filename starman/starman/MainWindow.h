#pragma once

#include <windows.h>
#include <tchar.h>
#include <d3d9.h>
#include <d3dx9.h>

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif

#include <dinput.h>
#include <string>
#include <chrono>
#include <deque>

#include "Mesh.h"
#include "Sprite.h"
#include "AnimMesh.h"
#include "SeqBattle.h"
#include "Opening.h"
#include "SeqEnding.h"
#include "KeyBoard.h"

class MainWindow
{
public:

    explicit MainWindow(const HINSTANCE&, IKeyBoard* keyboard);
    ~MainWindow();
    int MainLoop();

    // WM_CLOSE���b�Z�[�W����M�����Ƃ��ɑΏ�����K�v���邽�߂̊֐�
    static SeqBattle* GetBattleSequence();

private:

    std::string m_title;
    MSG m_msg { };
    HWND m_hWnd = nullptr;
    LPDIRECT3D9 m_D3D = nullptr;
    LPDIRECTINPUT8 m_directInput = nullptr;

    eSequence m_sequence = eSequence::BATTLE;

    // WM_CLOSE���b�Z�[�W����M�����Ƃ��ɑΏ�����K�v�����邽��static�ɂ���
    static SeqBattle* m_seqBattle;

    SeqEnding* m_seqEnding = nullptr;

    // �f�o�b�O�p
    void ShowDebugInfo(const int fps);

    // �f�o�b�O�p
    int CalcFPS();

    // �f�o�b�O�p
    std::vector<std::chrono::system_clock::time_point> m_vecTime;

    // �f�o�b�O�p
    LPD3DXFONT m_D3DFont = nullptr;
};

