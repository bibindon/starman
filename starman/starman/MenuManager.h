#pragma once

#include "..\..\menulib\menulib\menulib\MenuLib.h"

class MenuManager
{
public:

    void InitMenu();
    void Draw();
    std::string OperateMenu();

private:

    NSMenulib::MenuLib m_menu;
};

