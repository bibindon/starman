#pragma once
#include "../../patchTestLib/patchTestLib/patchTestLib.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/PatchTestManager.h"

class PatchTestManager2
{

public:

    void Finalize();
    void InitPatch();
    std::wstring Operate();
    void Draw();
    void CreateList();

private:

    void QueueTest(const std::wstring& result);

    NSStarmanLib::PatchTestManager* GetPatchLib();

    NSPatchTestLib::PatchTestLib m_guiLib;

};

