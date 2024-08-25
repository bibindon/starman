#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <WinBase.h>
#include <WinUser.h>

void InstallDirectX()
{
    {
        std::ofstream outputfile("dxinstall.bat");
        outputfile << "start /wait .\\Redist\\DXSETUP.exe /silent";
        outputfile.close();
    }

    STARTUPINFO si {};
    PROCESS_INFORMATION pi {};
    si.cb = sizeof(si);

    BOOL ret = CreateProcess("dxinstall.bat", nullptr, nullptr, nullptr, false, 0, nullptr, nullptr, &si, &pi);
    WaitForSingleObject(pi.hProcess, INFINITE);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    {
        std::ofstream outputfile("dxinstalled.txt");
        outputfile << "installed";
        outputfile.close();
    }
}

int main()
{
    // 初回起動時はDirectX9のランタイムのインストールを行う
    // 「.\\starman\\dxinstalled.txt」というファイルがあればインストール済み、とする
    std::string dxinstalled = ".\\dxinstalled.txt";

    struct _stat s;
    bool exist = (_stat(dxinstalled.c_str(), &s) == 0);

    if (exist == false)
    {
        InstallDirectX();
    }

    STARTUPINFO si {};
    PROCESS_INFORMATION pi {};
    si.cb = sizeof(si);
    CreateProcess(".\\starman\\game.exe", nullptr, nullptr, nullptr, false, 0, nullptr, ".\\starman", &si, &pi);

    return EXIT_SUCCESS;
}

