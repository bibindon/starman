#pragma once

#include "..\..\command\command\command.h"
#include <string>

class CommandManager
{
public:
    void Init();
    std::string Operate();
    void Draw();
private:
    NSCommand::CommandLib* m_commandLib = nullptr;
};

