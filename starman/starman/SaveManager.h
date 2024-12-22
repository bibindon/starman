#pragma once

#include <string>

class SaveManager
{
public:
    static SaveManager* Get();
    static void Destroy();
    void Save();
    void LoadOrigin();
    void Load();
    void DeleteSavedata();

private:

    static SaveManager* m_obj;

    const std::string ORIGIN_DATA_PATH = "res\\script\\origin\\";
    const std::string ORIGIN_DATA_PATH_DEBUG = "res\\script\\origin_debug\\";
    const std::string SAVEDATA_PATH = "res\\script\\save\\";
    const std::string SAVEDATA_PATH_DEBUG = "res\\script\\save_debug\\";

    std::string CreateOriginFilePath(const std::string& filename);
    std::string CreateSaveFilePath(const std::string& filename);
};

