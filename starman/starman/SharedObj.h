#pragma once

#include <d3d9.h>
#include <random>
#include "Player.h"

#include "..\..\quest-system\QuestSystem\QuestSystem\QuestSystem.h"

class SharedObj
{
public:
    static HWND GetWindowHandle();
    static void SetWindowHandle(const HWND hWnd);
    static LPDIRECT3DDEVICE9 GetD3DDevice();
    static void SetD3DDevice(const LPDIRECT3DDEVICE9 D3DDevice);
    static void Init();
    static void Finalize();
    static int GetRandom();
    static Player* GetPlayer();
    static void SetPlayer(Player* player);
    static NSQuestSystem::QuestSystem* GetQuestSystem();
    static bool DebugMode();
    static Map* GetMap();
    static void SetMap(Map* map);
    static D3DXMATRIX GetRightHandMat();
    static void SetRightHandMat(D3DXMATRIX mat);

private:
    static LPDIRECT3DDEVICE9 m_D3DDevice;
    static HWND m_hWnd;
    static std::mt19937 m_Engine;
    static Player* m_player;
    static NSQuestSystem::QuestSystem* m_questSystem;
    static bool m_debugMode;
    static Map* m_map;
    static D3DXMATRIX m_rightHandMat;
};

