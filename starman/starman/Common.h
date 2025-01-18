#pragma once

#include <vector>
#include <string>
#include <Windows.h>
#include <d3dx9.h>

#if defined(_DEBUG)
#define NEW ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
#else
#define NEW new
#endif

enum class eSequence
{
    BATTLE,
    ENDING,
    EXIT
};

enum class eBattleState
{
    LOAD,
    TITLE,
    OPENING,
    NORMAL,
    STORY,
    MENU,
    COMMAND,
    CRAFT,
    STOREHOUSE,
    TALK,
    DEAD,
    SLEEP,
};

enum class eTitleMenu
{
    NOT_DECIDE,
    START,
    CONTINUE,
    EXIT,
};

enum class eBuildMode
{
    Debug,
    Release,
    Deploy,
};

class Common
{
public:
    static std::vector<char> get_model_texture_resource(
        const std::string& model_name, const std::string& texture_name);
    static std::vector<char> get_model_resource(const std::string& model_name);
    static std::vector<char> get_sound_resource(const std::string& filename);

    static std::vector<std::string> split(const std::string& s, char delim);
    static std::string ToStringWithPrecision(const float value, const int precision = 1);

    static constexpr int KEY_DEQUE_MAX_SIZE { 300 };
    static constexpr int SIMULTANEOUS_ALLOW_FRAME { 3 };
    static constexpr float ANIMATION_SPEED { 1.0f / 60 };

    static bool DebugMode();
    static bool ReleaseMode();
    static bool DeployMode();

    static void SetCursorVisibility(const bool visibility);
    static POINT GetScreenPos();

    static void OutputMsg(const std::string&, const int arg);
    static void OutputMsg(const std::string&, const D3DXVECTOR3& arg);

    static std::string RemoveSubstring(const std::string& str, const std::string& toRemove);

    static bool EqualF(const float arg1, const float arg2);

    // ì_Ç∆ê¸ï™ÇÃãóó£
    static float PointToSegmentDistance(const D3DXVECTOR3& p1,
                                        const D3DXVECTOR3& p2,
                                        const D3DXVECTOR3& point);


private:

    static eBuildMode m_buildMode;
};

template <typename T>
inline void SAFE_RELEASE(T*& p)
{
    if (p == nullptr)
    {
        return;
    }

    while (true)
    {
        auto refCnt = p->Release();
        if (refCnt == 0)
        {
            break;
        }
    }
    p = nullptr;
}

template <typename T>
inline void SAFE_DELETE(T*& p)
{
    delete p;
    p = nullptr;
}

template <typename T>
inline void SAFE_DELETE_ARRAY(T*& p)
{
    delete[] p;
    p = nullptr;
}

