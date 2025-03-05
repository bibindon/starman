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

    // コマンドの作成
    // 0.5秒おきに呼ばれる？
    void BuildCommand();

    NSCommand::CommandLib* m_commandLib = nullptr;

    // コマンド画面の表示カウント
    int m_counter = 0;
};

