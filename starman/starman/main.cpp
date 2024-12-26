
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3d9.lib")
#if defined(NDEBUG)
#pragma comment(lib, "d3dx9.lib")
#else
#pragma comment(lib, "d3dx9d.lib")
#endif
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "dbghelp.lib")

#if defined(NDEBUG)
#pragma comment(lib, "..\\..\\StoryTelling\\Release\\StoryTelling.lib")
#pragma comment(lib, "..\\..\\talklib2\\Release\\talklib2.lib")
#pragma comment(lib, "..\\..\\quest-system\\QuestSystem\\Release\\QuestSystem.lib")
#pragma comment(lib, "..\\..\\menulib\\menulib\\Release\\menulib.lib")
#pragma comment(lib, "..\\..\\StarmanLib\\StarmanLib\\Release\\StarmanLib.lib")
#pragma comment(lib, "..\\..\\storehouselib\\Release\\storehouselib.lib")
#pragma comment(lib, "..\\..\\craftlib\\Release\\craftlib.lib")
#pragma comment(lib, "..\\..\\hud\\Release\\hud.lib")
#pragma comment(lib, "..\\..\\command\\Release\\command.lib")
#else
#pragma comment(lib, "..\\..\\StoryTelling\\Debug\\StoryTelling.lib")
#pragma comment(lib, "..\\..\\talklib2\\Debug\\talklib2.lib")
#pragma comment(lib, "..\\..\\quest-system\\QuestSystem\\Debug\\QuestSystem.lib")
#pragma comment(lib, "..\\..\\menulib\\menulib\\Debug\\menulib.lib")
#pragma comment(lib, "..\\..\\StarmanLib\\StarmanLib\\Debug\\StarmanLib.lib")
#pragma comment(lib, "..\\..\\storehouselib\\Debug\\storehouselib.lib")
#pragma comment(lib, "..\\..\\craftlib\\Debug\\craftlib.lib")
#pragma comment(lib, "..\\..\\hud\\Debug\\hud.lib")
#pragma comment(lib, "..\\..\\command\\Debug\\command.lib")
#endif

#include <windows.h>
#include "MainWindow.h"
#include "StackBackTrace.h"
#include <crtdbg.h>

// 例外で終了したときに、例外発生時のスタックトレースを出力する
static void se_translator(unsigned int u, _EXCEPTION_POINTERS* e)
{
    StackBackTrace stackBackTrace;
    std::string output = stackBackTrace.build();
    std::ofstream ofs("error.log");
    ofs << output;
}

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    // メモリリーク検出 一番最初に書かないと動作しない
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    // 例外で終了したときに、例外発生時のスタックトレースを出力する
    _set_se_translator(se_translator);

    HWND hWnd = FindWindow("ホシマン", nullptr);
    if (hWnd != nullptr)
    {
        MessageBox(NULL, "ホシマンはすでに起動済みです。", "二重起動エラー", MB_OK);
        return 0;
    }

    {
        MainWindow window(hInstance);
        window.MainLoop();
    }

    return 0;
}

