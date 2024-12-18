#pragma once

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

};

