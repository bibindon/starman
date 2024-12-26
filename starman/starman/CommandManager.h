#pragma once

#include "..\..\command\command\command.h"
#include <string>

class CommandManager
{
public:
    void Init(std::vector<std::string> commands, std::vector<bool> visibles);
    std::string Operate();
    void Draw();
    void Finalize();
private:
    NSCommand::CommandLib* m_commandLib = nullptr;
};

