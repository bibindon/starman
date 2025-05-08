// GenerateWixScript.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <Windows.h>

std::vector<std::string> ListFilesInFolder(const std::string& folder)
{
    std::vector<std::string> files;
    std::string search_path = folder + "\\*.*";

    WIN32_FIND_DATA fd;
    HANDLE hFind = FindFirstFile(search_path.c_str(), &fd);

    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                files.push_back(fd.cFileName);
            }
        } while (FindNextFile(hFind, &fd));

        FindClose(hFind);
    }

    return files;
}

int main()
{
    std::string text = R"Gene(<?xml version="1.0" encoding="utf-8"?>
<Wix xmlns="http://schemas.microsoft.com/wix/2006/wi">
    <Product Id="*" Name="starman" Language="1033" Version="1.0.0.0" Manufacturer="bibindon" UpgradeCode="cea4510f-a56d-42cd-a38c-a03f64ae6630">
        <Package InstallerVersion="200" Compressed="yes" InstallScope="perMachine" Platform="x64" />

        <MajorUpgrade DowngradeErrorMessage="A newer version of [ProductName] is already installed." />
        <MediaTemplate EmbedCab="yes" />

        <Feature Id="ProductFeature" Title="Installer" Level="1">
            <ComponentGroupRef Id="ProductComponents" />
            <ComponentGroupRef Id="MyAppFiles" />
            <ComponentRef Id="RemoveAppDataComponent" />
        </Feature>
        
        <UIRef Id="WixUI_Minimal"/>
        <WixVariable Id="WixUILicenseRtf" Value="license.rtf" />

        <Binary Id="PowerShellExe" SourceFile="C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe" />

        <CustomAction Id="RemoveAppDataFolder"
                      BinaryKey="PowerShellExe"
                      ExeCommand="-ExecutionPolicy Bypass -NoExit -Command &quot;Remove-Item -Path ($env:APPDATA + '\Starman') -Recurse -Force -ErrorAction SilentlyContinue&quot;"
                      Execute="deferred"
                      Impersonate="yes"
                      Return="ignore" />

        <InstallExecuteSequence>
            <Custom Action="RemoveAppDataFolder" After="RemoveFiles">REMOVE="ALL"</Custom>
        </InstallExecuteSequence>

    </Product>

    <Fragment>
        <Directory Id="TARGETDIR" Name="SourceDir">
            <Directory Id="AppDataFolder">
                <Directory Id="StarmanAppDataDir" Name="Starman">
                    <Component Id="RemoveAppDataComponent" Guid="FBB7D418-AE56-4B63-A81C-31F43E027BD7">

                        <RemoveFile Id="RemoveAllFiles"
                                    Name="*"
                                    On="uninstall"
                                    Directory="StarmanAppDataDir" />

                        <RemoveFolder Id="RemoveStarmanFolder" On="uninstall"  Directory="StarmanAppDataDir" />

                        <RegistryValue Root="HKCU"
                                       Key="Software\StarmanInstaller"
                                       Name="CleanupMarker"
                                       Type="integer"
                                       Value="1"
                                       KeyPath="yes"/>
                    </Component>
                </Directory>
            </Directory>

            
            <Directory Id="ProgramMenuFolder">
            </Directory>
            <Directory Id="ProgramFiles64Folder">
                <Directory Id="INSTALLFOLDER" Name="starman">
                    <Directory Id="RESOURCE" Name="res">
                        <Directory Id="IMAGE" Name="image"/>
                        <Directory Id="SHADER" Name="shader"/>
                        <Directory Id="MODEL" Name="model"/>
                        <Directory Id="SCRIPT_" Name="script">
                            <Directory Id="SCRIPT_ORIGIN" Name="origin"/>
                        </Directory>
                        <Directory Id="SOUND" Name="sound"/>
                    </Directory>
                </Directory>
            </Directory>
        </Directory>
    </Fragment>

    <Fragment>
        <ComponentGroup Id="ProductComponents" Directory="INSTALLFOLDER">
            <Component Id="ProductComponent" Guid="*">
                <File Id="WixInstallerEXE" Name="starman.exe" Source="$(var.starman.TargetPath)" KeyPath="yes">
                    <Shortcut Id="WixInstStartmenu" Directory="ProgramMenuFolder"
                              Name="starman" WorkingDirectory="INSTALLFOLDER"
                              Advertise="yes">
                        <Icon Id="WixInstIco.exe" SourceFile="$(var.starman.TargetPath)"/>
                    </Shortcut>
                </File>
            </Component>
            <Component Guid="*">
                <?if $(var.Configuration) = Debug ?>
                <File Source="$(var.starman.TargetDir)d3dx9d_43.dll" KeyPath="yes"/>
                <?else?>
                <File Source="$(var.starman.TargetDir)d3dx9_43.dll" KeyPath="yes"/>
                <?endif?>
            </Component>
            <Component Guid="*">
                <File Source="$(var.starman.TargetDir)D3DCompiler_43.dll" KeyPath="yes"/>
            </Component>
        </ComponentGroup>
)Gene";

    text += R"Gene(        <ComponentGroup Id = "MyAppFiles">
)Gene";

    // ここでリソースファイルを列挙
    {
        auto fileList = ListFilesInFolder("..\\starman\\res\\image");
        for (auto& filename : fileList)
        {
            text += "            <Component Directory=\"IMAGE\" Guid=\"*\" Win64=\"yes\">\n";
            text += "                <File Source=\"../starman/res/image/" + filename + "\" />\n";
            text += "            </Component>\n";
        }
    }

    {
        auto fileList = ListFilesInFolder("..\\starman\\res\\shader");
        for (auto& filename : fileList)
        {
            text += "            <Component Directory=\"SHADER\" Guid=\"*\" Win64=\"yes\">\n";
            text += "                <File Source=\"../starman/res/shader/" + filename + "\" />\n";
            text += "            </Component>\n";
        }
    }

    {
        auto fileList = ListFilesInFolder("..\\starman\\res\\model");
        for (auto& filename : fileList)
        {
            text += "            <Component Directory=\"MODEL\" Guid=\"*\" Win64=\"yes\">\n";
            text += "                <File Source=\"../starman/res/model/" + filename + "\" />\n";
            text += "            </Component>\n";
        }
    }

    {
        auto fileList = ListFilesInFolder("..\\starman\\res\\script\\origin");
        for (auto& filename : fileList)
        {
            text += "            <Component Directory=\"SCRIPT_ORIGIN\" Guid=\"*\" Win64=\"yes\">\n";
            text += "                <File Source=\"../starman/res/script/origin/" + filename + "\" />\n";
            text += "            </Component>\n";
        }
    }

    {
        auto fileList = ListFilesInFolder("..\\starman\\res\\sound");
        for (auto& filename : fileList)
        {
            text += "            <Component Directory=\"SOUND\" Guid=\"*\" Win64=\"yes\">\n";
            text += "                <File Source=\"../starman/res/sound/" + filename + "\" />\n";
            text += "            </Component>\n";
        }
    }

    text += R"Gene(
        </ComponentGroup>
    </Fragment>
</Wix>
)Gene";

    std::ofstream outfile("Product.wxs");
    if (!outfile)
    {
        std::cerr << "ファイルを開けませんでした\n";
        return 1;
    }

    outfile << text;
    outfile.close();

}

// プログラムの実行: Ctrl + F5 または [デバッグ] > [デバッグなしで開始] メニュー
// プログラムのデバッグ: F5 または [デバッグ] > [デバッグの開始] メニュー

// 作業を開始するためのヒント: 
//    1. ソリューション エクスプローラー ウィンドウを使用してファイルを追加/管理します 
//   2. チーム エクスプローラー ウィンドウを使用してソース管理に接続します
//   3. 出力ウィンドウを使用して、ビルド出力とその他のメッセージを表示します
//   4. エラー一覧ウィンドウを使用してエラーを表示します
//   5. [プロジェクト] > [新しい項目の追加] と移動して新しいコード ファイルを作成するか、[プロジェクト] > [既存の項目の追加] と移動して既存のコード ファイルをプロジェクトに追加します
//   6. 後ほどこのプロジェクトを再び開く場合、[ファイル] > [開く] > [プロジェクト] と移動して .sln ファイルを選択します
