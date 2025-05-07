#pragma once

#include <string>
#include <atomic>

class SaveManager
{
public:
    static SaveManager* Get();
    static void Destroy();
    void Save();
    void LoadOrigin();
    void Load();
    void DeleteSavedata();

    // 別スレッドから進捗を取得できるようにする
    int GetProgress();

    std::string GetLangFile();
    void SetLangFile(const std::string lang);

    bool SaveFolderExists();

private:

    SaveManager();

    bool DeleteFolderContents(const std::string& folderPath);
    bool DeleteFolder(const std::string& folderPath);

    static SaveManager* m_obj;

    const std::string ORIGIN_DATA_PATH = "res\\script\\origin\\";
    const std::string SAVEDATA_PATH = "res\\script\\save\\";
    const std::string SAVEDATA_FOLDER = "res\\script\\save";

    std::string m_savedata_path;
    std::string m_savedata_folder;

    std::string CreateOriginFilePath(const std::string& filename);
    std::string CreateSaveFilePath(const std::string& filename);

    std::string GetOriginMapPath();
    std::string GetSavefileMapPath();

    bool m_savedataLoaded = false;

    bool m_encrypt = true;

    std::atomic<int> m_progress = 0;
};

