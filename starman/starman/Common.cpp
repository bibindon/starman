#include "Common.h"
#include <sstream>
#include <iomanip>
#include <Windows.h>

#if defined(NDEBUG)
bool Common::m_debugMode { false };
#else
bool Common::m_debugMode { true };
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
    return m_debugMode;
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
    }
    else
    {
        do
        {
            work = ShowCursor(false);
        }
        while (work >= 0);

        RECT rect { };
        rect.left = 150;
        rect.top = 150;
        rect.right = 150 + 100;
        rect.bottom = 150 + 100;
        ClipCursor(&rect);
    }
}

