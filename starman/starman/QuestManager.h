#pragma once

#include <d3dx9math.h>
#include "../../quest-system/QuestSystem/QuestSystem/QuestSystem.h"

class QuestManager
{
public:

    static QuestManager* Get();

    void Init(const std::string& originFile, const std::string& saveFile);
    void Save(const std::string& originFile);

    void SetExamine();
    void SetTalk(const std::string& npc);
    std::string GetQuestIdStartByExamine();
    std::string GetQuestIdFinishByExamine();

    std::vector<std::string> GetStartQuest();
    std::vector<std::string> GetQuestStartEvent(const std::string& quest);

    std::vector<std::string> GetFinishQuest();
    std::vector<std::string> GetQuestFinishEvent(const std::string& quest);

    void SetQuestFinish(const std::string& quest);

    void Update();

private:

    static QuestManager* m_single;
    NSQuestSystem::QuestSystem m_questSystem;

};

