#pragma once

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
    std::string OperateMenu();

private:

    NSMenulib::MenuLib m_menu;

    bool IsBagEquiped(const int id, const int subId);
    void EquipBag(const int id, const int subId);
};

