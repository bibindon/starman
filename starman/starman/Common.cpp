#include "Common.h"
#include <sstream>
#include <iomanip>
#include "SharedObj.h"

#if defined(_DEBUG)
eBuildMode Common::m_buildMode = eBuildMode::Debug;
#elif defined(DEPLOY)
eBuildMode Common::m_buildMode = eBuildMode::Deploy;
#else
eBuildMode Common::m_buildMode = eBuildMode::Release;
#endif

std::vector<char> Common::get_model_texture_resource(
    const std::string& model_name, const std::string& texture_name)
{
    return std::vector<char>();
}

std::vector<char> Common::get_model_resource(const std::string& model_name)
{
    return std::vector<char>();
}

std::vector<char> Common::get_sound_resource(const std::string& filename)
{
    return std::vector<char>();
}

std::vector<std::string> Common::split(const std::string& s, char delim)
{
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string item;

    while (getline(ss, item, delim))
    {
        result.push_back(item);
    }

    return result;
}

std::string Common::ToStringWithPrecision(const float value, const int precision)
{
    std::ostringstream out;
    out << std::fixed << std::setprecision(precision) << value;
    return out.str();
}

bool Common::DebugMode()
{
    if (m_buildMode == eBuildMode::Debug)
    {
        return true;
    }
    return false;
}

bool Common::ReleaseMode()
{
    if (m_buildMode == eBuildMode::Release)
    {
        return true;
    }
    return false;
}

bool Common::DeployMode()
{
    if (m_buildMode == eBuildMode::Deploy)
    {
        return true;
    }
    return false;
}

void Common::SetCursorVisibility(const bool visibility)
{
    // Win32APIのShowCursor関数は内部でカウンターを持っており、
    // 内部カウンターが0以下にならないと非表示にならない。
    // 1以上にならないと表示にならない。
    // Directx9にもShowCursor関数があり、こちらはカウンターを持っていない。
    // しかし、D3DCREATE_SOFTWARE_VERTEXPROCESSINGを指定して
    // 頂点処理をソフトウェアで処理しないとカーソルが表示できないらしい。

    int work = 0;
    if (visibility)
    {
        do
        {
            work = ShowCursor(true);
        }
        while (work <= -1);

        ClipCursor(NULL);

        auto hWnd = SharedObj::GetWindowHandle();

        if (hWnd != nullptr)
        {
            RECT rectClient { };
            GetClientRect(hWnd, &rectClient);

            POINT pt { };
            ClientToScreen(hWnd, &pt);

            rectClient.top += pt.y;
            rectClient.left += pt.x;

            // CursorOn処理でゲームパッドと取り合いになるので
            // マウスカーソルを左上に移動しておく
            SetCursorPos(rectClient.left, rectClient.top);
        }
    }
    else
    {
        do
        {
            work = ShowCursor(false);
        }
        while (work >= 0);

        auto hWnd = SharedObj::GetWindowHandle();

        if (hWnd != nullptr)
        {
            RECT rectClient { };
            GetClientRect(hWnd, &rectClient);

            POINT pt { };
            ClientToScreen(hWnd, &pt);

            rectClient.top += pt.y;
            rectClient.left += pt.x;
            rectClient.bottom += pt.y;
            rectClient.right += pt.x;

            ClipCursor(&rectClient);

            // CursorOn処理でゲームパッドと取り合いになるので
            // マウスカーソルを左上に移動しておく
            SetCursorPos(rectClient.left, rectClient.top);
        }
    }
}

POINT Common::GetScreenPos()
{
    POINT p;
    GetCursorPos(&p);
    ScreenToClient(FindWindow("ホシマン", nullptr), &p);
    return p;
}

