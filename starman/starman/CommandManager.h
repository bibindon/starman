#pragma once

#include "..\..\command\command\command.h"
#include <string>

class CommandManager
{
public:
    enum class eType
    {
        Title,
        Main,
    };

    void Init(const eType type);
    std::string Operate();
    void Draw();
    void Finalize();
    void Upsert(const std::string name, const bool visible);

private:

    // コマンドの作成
    // 0.5秒おきに呼ばれる？
    void BuildCommand();
    void BuildOpeningCommand();

    NSCommand::CommandLib* m_commandLib = nullptr;

    // コマンド画面の表示カウント
    int m_counter = 0;

    eType m_eType = eType::Title;
};

