#pragma once

#include "..\..\menulib\menulib\menulib\MenuLib.h"

class MenuManager
{
public:

    void InitMenu();
    void Draw();
    bool UseItem(const int id, const int subId);
    std::string OperateMenu();

private:

    NSMenulib::MenuLib m_menu;
};

