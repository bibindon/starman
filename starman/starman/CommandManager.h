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
    void Upsert(const std::string name, const bool visible);

private:

    // �R�}���h�̍쐬
    // 0.5�b�����ɌĂ΂��H
    void BuildCommand();

    NSCommand::CommandLib* m_commandLib = nullptr;

    // �R�}���h��ʂ̕\���J�E���g
    int m_counter = 0;
};

