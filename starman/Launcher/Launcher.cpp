
#include <iostream>
#include <Windows.h>
#include <WinBase.h>
#include <WinUser.h>

int main()
{
    STARTUPINFO si {};
    PROCESS_INFORMATION pi {};
    si.cb = sizeof(si);

    CreateProcess(".\\starman\\game.exe", nullptr, nullptr, nullptr, false, 0, nullptr, ".\\starman", &si, &pi);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return EXIT_SUCCESS;
}

