#pragma once

#include "Common.h"
#include "..\..\command\command\command.h"
#include <string>

class CommandManager
{
public:
    enum class eType
    {
        Title,
        Title_Language,
        Main,
    };

    void Init(const eType type);
    std::string Operate();
    void Draw();
    void Finalize();
    void Upsert(const std::string name, const bool visible);
    void SetPreviousState(const eBattleState previousState);
    eBattleState GetPreviousState() const;

private:

    // �R�}���h�̍쐬
    // 0.5�b�����ɌĂ΂��H
    void BuildCommand();
    void BuildOpeningCommand();
    void BuildLangCommand();

    NSCommand::CommandLib* m_commandLib = nullptr;

    // �R�}���h��ʂ̕\���J�E���g
    int m_counter = 0;

    eType m_eType = eType::Title;

    eBattleState m_previousState = eBattleState::NONE;
};

