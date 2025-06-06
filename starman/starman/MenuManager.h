﻿#pragma once

#include "..\..\menulib\menulib\menulib\MenuLib.h"

class MenuManager
{
public:
    MenuManager();
    ~MenuManager();

    void InitMenu();
    void Finalize();
    void Draw();
    bool UseItem(const int id, const int subId);
    void AddItem(const int id, const int subId, const int durability = -1);
    void DeleteItem(const int id, const int subId);
    std::wstring OperateMenu();

private:

    NSMenulib::MenuLib m_menu;

    bool IsBagEquiped(const int id, const int subId);
    void Equip(const int id, const int subId);
    void Unequip(const int id, const int subId);
};

