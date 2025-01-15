#include "Common.h"
#include <sstream>
#include <iomanip>
#include "SharedObj.h"
#include <cfloat>

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

void Common::OutputMsg(const std::string& str, const int arg)
{
    std::string work;
    work = str + std::to_string(arg) + "\n";
    OutputDebugString(work.c_str());
}

void Common::OutputMsg(const std::string& str, const D3DXVECTOR3& arg)
{
    std::string work;
    work = str + "x: " + std::to_string(arg.x) + ", ";
    work += "y: " + std::to_string(arg.y) + ", ";
    work += "z: " + std::to_string(arg.z) + "\n";
    OutputDebugString(work.c_str());
}

std::string Common::RemoveSubstring(const std::string& str, const std::string& toRemove)
{
    std::string result = str;
    size_t pos = std::string::npos;

    // �w�肳�ꂽ�����񂪌�������胋�[�v
    while ((pos = result.find(toRemove)) != std::string::npos)
    {
        result.erase(pos, toRemove.length());
    }
    return result;
}

bool Common::EqualF(const float arg1, const float arg2)
{
    return abs(arg1 - arg2) < FLT_EPSILON;
}

    // �_�Ɛ����̋������v�Z����֐�
float Common::PointToSegmentDistance(const D3DXVECTOR3& p1, const D3DXVECTOR3& p2, const D3DXVECTOR3& point)
{
    // �������x�N�g���Ƃ��Ĉ���
    D3DXVECTOR3 segment = p2 - p1;
    D3DXVECTOR3 pointToStart = point - p1;

    // �����̒�����2��
    float segmentLengthSquared = D3DXVec3LengthSq(&segment);

    if (segmentLengthSquared == 0.0f)
    {
        // �������_�̏ꍇ�A�_�Ǝn�_�i�܂��͏I�_�j�̋�����Ԃ�
        auto temp1(point - p1);
        return D3DXVec3Length(&temp1);
    }

    // �_������ɓ��e
    float t = D3DXVec3Dot(&pointToStart, &segment) / segmentLengthSquared;

    // t �� 0 �` 1 �ɃN�����v�i���e�_��������ɂ��邩�m�F�j
    t = (std::max)(0.0f, (std::min)(1.0f, t));

    // ���e�_�̍��W���v�Z
    D3DXVECTOR3 projection = p1 + t * segment;

    // �_�Ɠ��e�_�̋������v�Z
    auto temp2(point - projection);
    return D3DXVec3Length(&temp2);
}


