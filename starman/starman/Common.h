#pragma once

#include <vector>
#include <string>

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
template <typename T>
inline void SAFE_RELEASE(T*& p)
{
    if (p)
    {
        p->Release();
        p = nullptr;
    }
}

class Common
{
public:
    static std::vector<char> get_model_texture_resource(
        const std::string& model_name, const std::string& texture_name);
};

