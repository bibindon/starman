#include "Common.h"
#include <sstream>
#include <iomanip>
#include <Windows.h>
#include "SharedObj.h"

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

bool Common::ReleaseMode()
{
    if (m_debugMode)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void Common::SetCursorVisibility(const bool visibility)
{
    // Win32API��ShowCursor�֐��͓����ŃJ�E���^�[�������Ă���A
    // �����J�E���^�[��0�ȉ��ɂȂ�Ȃ��Ɣ�\���ɂȂ�Ȃ��B
    // 1�ȏ�ɂȂ�Ȃ��ƕ\���ɂȂ�Ȃ��B
    // Directx9�ɂ�ShowCursor�֐�������A������̓J�E���^�[�������Ă��Ȃ��B
    // �������AD3DCREATE_SOFTWARE_VERTEXPROCESSING���w�肵��
    // ���_�������\�t�g�E�F�A�ŏ������Ȃ��ƃJ�[�\�����\���ł��Ȃ��炵���B

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

            // CursorOn�����ŃQ�[���p�b�h�Ǝ�荇���ɂȂ�̂�
            // �}�E�X�J�[�\��������Ɉړ����Ă���
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

            // CursorOn�����ŃQ�[���p�b�h�Ǝ�荇���ɂȂ�̂�
            // �}�E�X�J�[�\��������Ɉړ����Ă���
            SetCursorPos(rectClient.left, rectClient.top);
        }
    }
}

POINT Common::GetScreenPos()
{
    POINT p;
    GetCursorPos(&p);
    ScreenToClient(FindWindow("�z�V�}��", nullptr), &p);
    return p;
}

