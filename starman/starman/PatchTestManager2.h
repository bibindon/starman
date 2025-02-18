#pragma once
#include "../../patchTestLib/patchTestLib/patchTestLib.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/PatchTestManager.h"

class PatchTestManager2
{

public:

    void Finalize();
    void InitPatch();
    std::string Operate();
    void Draw();

private:

    NSStarmanLib::PatchTestManager* GetPatchLib();

    NSPatchTestLib::PatchTestLib m_guiLib;

};

