
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3d9.lib")
#if defined(DEBUG) || defined(_DEBUG)
#pragma comment(lib, "d3dx9d.lib")
#else
#pragma comment(lib, "d3dx9.lib")
#endif

#pragma comment(lib, "..\\..\\StoryTelling\\StoryTelling.lib")
#pragma comment(lib, "..\\..\\talklib2\\talklib2.lib")
#pragma comment(lib, "..\\..\\quest-system\\QuestSystem\\QuestSystem.lib")
#pragma comment(lib, "..\\..\\menulib\\menulib\\menulib.lib")

#include <windows.h>
#include "MainWindow.h"

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    MainWindow window(hInstance);
    window.MainLoop();

    return 0;
}

