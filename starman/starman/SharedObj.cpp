#include "SharedObj.h"
#include "Common.h"

LPDIRECT3DDEVICE9 SharedObj::m_D3DDevice { nullptr };
std::mt19937 SharedObj::m_Engine { };

LPDIRECT3DDEVICE9 SharedObj::GetD3DDevice()
{
    return m_D3DDevice;
}

void SharedObj::Finalize()
{
    SAFE_RELEASE(m_D3DDevice);
}

int SharedObj::GetRandom()
{
    return m_Engine();
}

void SharedObj::SetD3DDevice(const LPDIRECT3DDEVICE9 D3DDevice)
{
    m_D3DDevice = D3DDevice;
}

void SharedObj::Init()
{
    std::random_device seed_gen;
    m_Engine = std::mt19937(seed_gen());
}

