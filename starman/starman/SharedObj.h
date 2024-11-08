#pragma once

#include <d3d9.h>
#include <random>
#include "Player.h"

class SharedObj
{
public:
    static LPDIRECT3DDEVICE9 GetD3DDevice();
    static void SetD3DDevice(const LPDIRECT3DDEVICE9 D3DDevice);
    static void Init();
    static void Finalize();
    static int GetRandom();
    static Player* GetPlayer();
    static void SetPlayer(Player* player);

private:
    static LPDIRECT3DDEVICE9 m_D3DDevice;
    static std::mt19937 m_Engine;
    static Player* m_player;
};

