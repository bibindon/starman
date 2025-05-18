
#pragma comment(lib, _T("dxguid.lib"))
#pragma comment(lib, _T("d3d9.lib"))
#if defined(NDEBUG)
#pragma comment(lib, _T("d3dx9.lib"))
#else
#pragma comment(lib, _T("d3dx9d.lib"))
#endif
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "dbghelp.lib")

#if defined(_WIN64)
#   if defined(NDEBUG)
#pragma comment(lib, _T("..\\..\\StoryTelling\\x64\\Release\\StoryTelling.lib"))
#pragma comment(lib, _T("..\\..\\talklib2\\x64\\Release\\talklib2.lib"))
#pragma comment(lib, _T("..\\..\\quest-system\\QuestSystem\\x64\\Release\\QuestSystem.lib"))
#pragma comment(lib, _T("..\\..\\menulib\\menulib\\x64\\Release\\menulib.lib"))
#pragma comment(lib, _T("..\\..\\StarmanLib\\StarmanLib\\x64\\Release\\StarmanLib.lib"))
#pragma comment(lib, "..\\..\\storehouselib\\x64\\Release\\storehouselib.lib")
#pragma comment(lib, _T("..\\..\\craftlib\\x64\\Release\\craftlib.lib"))
#pragma comment(lib, "..\\..\\hud\\x64\\Release\\hud.lib")
#pragma comment(lib, _T("..\\..\\command\\x64\\Release\\command.lib"))
#pragma comment(lib, "..\\..\\patchTestLib\\x64\\Release\\patchTestLib.lib")
#   else
#pragma comment(lib, _T("..\\..\\StoryTelling\\x64\\Debug\\StoryTelling.lib"))
#pragma comment(lib, _T("..\\..\\talklib2\\x64\\Debug\\talklib2.lib"))
#pragma comment(lib, _T("..\\..\\quest-system\\QuestSystem\\x64\\Debug\\QuestSystem.lib"))
#pragma comment(lib, _T("..\\..\\menulib\\menulib\\x64\\Debug\\menulib.lib"))
#pragma comment(lib, _T("..\\..\\StarmanLib\\StarmanLib\\x64\\Debug\\StarmanLib.lib"))
#pragma comment(lib, "..\\..\\storehouselib\\x64\\Debug\\storehouselib.lib")
#pragma comment(lib, _T("..\\..\\craftlib\\x64\\Debug\\craftlib.lib"))
#pragma comment(lib, "..\\..\\hud\\x64\\Debug\\hud.lib")
#pragma comment(lib, _T("..\\..\\command\\x64\\Debug\\command.lib"))
#pragma comment(lib, "..\\..\\patchTestLib\\x64\\Debug\\patchTestLib.lib")
#   endif
#else
#   if defined(NDEBUG)
#pragma comment(lib, _T("..\\..\\StoryTelling\\Release\\StoryTelling.lib"))
#pragma comment(lib, _T("..\\..\\talklib2\\Release\\talklib2.lib"))
#pragma comment(lib, _T("..\\..\\quest-system\\QuestSystem\\Release\\QuestSystem.lib"))
#pragma comment(lib, _T("..\\..\\menulib\\menulib\\Release\\menulib.lib"))
#pragma comment(lib, _T("..\\..\\StarmanLib\\StarmanLib\\Release\\StarmanLib.lib"))
#pragma comment(lib, "..\\..\\storehouselib\\Release\\storehouselib.lib")
#pragma comment(lib, _T("..\\..\\craftlib\\Release\\craftlib.lib"))
#pragma comment(lib, "..\\..\\hud\\Release\\hud.lib")
#pragma comment(lib, _T("..\\..\\command\\Release\\command.lib"))
#pragma comment(lib, "..\\..\\patchTestLib\\Release\\patchTestLib.lib")
#   else
#pragma comment(lib, _T("..\\..\\StoryTelling\\Debug\\StoryTelling.lib"))
#pragma comment(lib, _T("..\\..\\talklib2\\Debug\\talklib2.lib"))
#pragma comment(lib, _T("..\\..\\quest-system\\QuestSystem\\Debug\\QuestSystem.lib"))
#pragma comment(lib, _T("..\\..\\menulib\\menulib\\Debug\\menulib.lib"))
#pragma comment(lib, _T("..\\..\\StarmanLib\\StarmanLib\\Debug\\StarmanLib.lib"))
#pragma comment(lib, "..\\..\\storehouselib\\Debug\\storehouselib.lib")
#pragma comment(lib, _T("..\\..\\craftlib\\Debug\\craftlib.lib"))
#pragma comment(lib, "..\\..\\hud\\Debug\\hud.lib")
#pragma comment(lib, _T("..\\..\\command\\Debug\\command.lib"))
#pragma comment(lib, "..\\..\\patchTestLib\\Debug\\patchTestLib.lib")
#   endif
#endif

#include <windows.h>
#include "MainWindow.h"
#include "StackBackTrace.h"
#include <crtdbg.h>

// 例外で終了したときに、例外発生時のスタックトレースを出力する
static void se_translator(unsigned int u, _EXCEPTION_POINTERS* e)
{
    StackBackTrace stackBackTrace;
    std::wstring output = stackBackTrace.build();
    std::ofstream ofs(_T("error.log"));
    ofs << output;
}

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    // メモリリーク検出 一番最初に書かないと動作しない
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    // 例外で終了したときに、例外発生時のスタックトレースを出力する
    _set_se_translator(se_translator);

    HWND hWnd = FindWindow(_T("ホシマン"), nullptr);
    if (hWnd != nullptr)
    {
        MessageBox(NULL, _T("ホシマンはすでに起動済みです。"), _T("二重起動エラー"), MB_OK);
        return 0;
    }

    try
    {
        IKeyBoard* keyboard = NEW KeyBoard();
        MainWindow window(hInstance, keyboard);
        window.MainLoop();
    }
    catch (...)
    {
    }

    return 0;
}

