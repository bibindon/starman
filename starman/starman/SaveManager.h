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
private:
	static SaveManager* m_obj;

	const std::string ORIGIN_DATA_PATH = "res\\script\\origin\\";
	const std::string SAVEDATA_PATH = "res\\script\\save\\";
};

