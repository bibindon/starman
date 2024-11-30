#include "SharedObj.h"
#include "Common.h"

LPDIRECT3DDEVICE9 SharedObj::m_D3DDevice { nullptr };
std::mt19937 SharedObj::m_Engine { };
Player* SharedObj::m_player { nullptr };
NSQuestSystem::QuestSystem* SharedObj::m_questSystem { nullptr };

#if defined(NDEBUG)
bool SharedObj::m_debugMode { false };
#else
bool SharedObj::m_debugMode { true };
#endif

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

Player* SharedObj::GetPlayer()
{
    return m_player;
}

void SharedObj::SetPlayer(Player* player)
{
    m_player = player;
}

NSQuestSystem::QuestSystem* SharedObj::GetQuestSystem()
{
    return m_questSystem;
}

bool SharedObj::DebugMode()
{
    return m_debugMode;
}

void SharedObj::SetD3DDevice(const LPDIRECT3DDEVICE9 D3DDevice)
{
    m_D3DDevice = D3DDevice;
}

void SharedObj::Init()
{
    std::random_device seed_gen;
    m_Engine = std::mt19937(seed_gen());

    m_questSystem = new NSQuestSystem::QuestSystem();

    if (SharedObj::DebugMode())
    {
        m_questSystem->Init("res\\script\\questSample.debug.csv");
    }
    else
    {
        m_questSystem->Init("res\\script\\questSample.csv");
    }
}

