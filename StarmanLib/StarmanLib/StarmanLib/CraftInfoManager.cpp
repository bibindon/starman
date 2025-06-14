#include "CraftInfoManager.h"
#include "ItemManager.h"
#include "Util.h"

using namespace NSStarmanLib;

CraftInfoManager* CraftInfoManager::obj { nullptr };

CraftInfoManager* CraftInfoManager::GetObj()
{
    if (obj == nullptr)
    {
        obj = new CraftInfoManager();
    }
    return obj;
}

void CraftInfoManager::Destroy()
{
    delete CraftInfoManager::obj;
    CraftInfoManager::obj = nullptr;
}

void CraftInfoManager::Init(const std::wstring& csvfileDefinition,
                            const bool decrypt)
{
    ItemManager* itemManager = ItemManager::GetObj();

    if (!itemManager->Inited())
    {
        throw std::exception("not inited");
    }

    std::vector<std::vector<std::wstring> > vvs = Util::ReadFromCsv(csvfileDefinition, decrypt);

    for (std::size_t i = 1; i < vvs.size(); ++i)
    {
        CraftInfo craftInfo;
        CraftOutput craftOutput;
        int work_i = 0;

        craftOutput.SetName(vvs.at(i).at(1));

        work_i = std::stoi(vvs.at(i).at(2));
        craftOutput.SetNumber(work_i);

        if (vvs.at(i).at(3).empty() == false)
        {
            work_i = std::stoi(vvs.at(i).at(3));
            craftOutput.SetLevel(work_i);
        }
        else
        {
            craftOutput.SetLevel(-1);
        }

        craftInfo.SetOutput(craftOutput);

        // 素材は最大で5種類まで
        for (std::size_t j = 0; j < 5; ++j)
        {
            CraftMaterial craftMaterial;

            // 必要素材が5種類の時もあれば1種類の時もある。
            // 必要素材の名前が空なら、もはや必要な素材はないということ
            if (vvs.at(i).at(4 + (j * 3)).empty())
            {
                break;
            }

            craftMaterial.SetName(vvs.at(i).at(4 + (j * 3)));

            work_i = std::stoi(vvs.at(i).at(5 + (j * 3)));
            craftMaterial.SetNumber(work_i);

            if (vvs.at(i).at(6 + (j * 3)).empty() == false)
            {
                work_i = std::stoi(vvs.at(i).at(6 + (j * 3)));
                craftMaterial.SetLevel(work_i);
            }
            else
            {
                craftMaterial.SetLevel(-1);
            }

            ItemDef itemDef = itemManager->GetItemDef(craftMaterial.GetName(),
                                                      craftMaterial.GetLevel());

            craftMaterial.SetId(itemDef.GetId());

            craftInfo.SetCraftMaterialDef((int)j, craftMaterial);
        }
        m_craftInfoList.push_back(craftInfo);
    }
    m_init = true;
}

bool NSStarmanLib::CraftInfoManager::Inited()
{
    return m_init;
}

std::vector<CraftOutput> CraftInfoManager::GetCraftItemList()
{
    std::vector<CraftOutput> craftItemList;
    for (std::size_t i = 0; i < m_craftInfoList.size(); ++i)
    {
        craftItemList.push_back(m_craftInfoList.at(i).GetOutput());
    }
    return craftItemList;
}

CraftInfo CraftInfoManager::GetCraftInfo(const CraftOutput& output)
{
    std::size_t i = 0;
    for (; i < m_craftInfoList.size(); ++i)
    {
        if (m_craftInfoList.at(i).GetOutput().GetName() == output.GetName())
        {
            if (m_craftInfoList.at(i).GetOutput().GetNumber() == output.GetNumber())
            {
                if (m_craftInfoList.at(i).GetOutput().GetLevel() == output.GetLevel())
                {
                    break;
                }
                // 完成品の強化値が無しなら-1
                else if (m_craftInfoList.at(i).GetOutput().GetLevel() == -1)
                {
                    break;
                }
            }
        }
    }
    return m_craftInfoList.at(i);
}

CraftInfo NSStarmanLib::CraftInfoManager::GetCraftInfo(const std::wstring& name,
                                                       const int num,
                                                       const int level)
{
    std::size_t i = 0;
    for (; i < m_craftInfoList.size(); ++i)
    {
        if (m_craftInfoList.at(i).GetOutput().GetName() == name)
        {
            if (m_craftInfoList.at(i).GetOutput().GetNumber() == num)
            {
                if (m_craftInfoList.at(i).GetOutput().GetLevel() == level)
                {
                    break;
                }
            }
        }
    }
    return m_craftInfoList.at(i);
}

void NSStarmanLib::CraftMaterial::SetId(const int arg)
{
    m_id = arg;
}

int NSStarmanLib::CraftMaterial::GetId() const
{
    return m_id;
}

void CraftMaterial::SetName(const std::wstring& arg)
{
    m_name = arg;
}

std::wstring CraftMaterial::GetName() const
{
    return m_name;
}

void CraftMaterial::SetNumber(const int arg)
{
    m_number = arg;
}

int CraftMaterial::GetNumber() const
{
    return m_number;
}

void CraftMaterial::SetLevel(const int arg)
{
    m_level = arg;
}

int CraftMaterial::GetLevel() const
{
    return m_level;
}

void CraftOutput::SetName(const std::wstring& arg)
{
    m_name = arg;
}

std::wstring CraftOutput::GetName() const
{
    return m_name;
}

void CraftOutput::SetNumber(const int arg)
{
    m_number = arg;
}

int CraftOutput::GetNumber() const
{
    return m_number;
}

void CraftOutput::SetLevel(const int arg)
{
    m_level = arg;
}

int CraftOutput::GetLevel() const
{
    return m_level;
}

void CraftInfo::SetOutput(const CraftOutput& arg)
{
    m_output = arg;
}

CraftOutput CraftInfo::GetOutput() const
{
    return m_output;
}

void CraftInfo::SetCraftMaterialDef(const int index, const CraftMaterial& arg)
{
    m_materialDefList[index] = arg;
}

CraftMaterial CraftInfo::GetCraftMaterialDef(const int index)
{
    return m_materialDefList.at(index);
}

std::vector<CraftMaterial> NSStarmanLib::CraftInfo::GetCraftMaterialDef()
{
    std::vector<CraftMaterial> result;
    
    for (auto it = m_materialDefList.begin(); it != m_materialDefList.end(); ++it)
    {
        result.push_back(it->second);
    }
    return result;
}

void NSStarmanLib::CraftInfo::SetCraftMaterial(std::vector<ItemInfo> items)
{
    m_materialList = items;
}

std::vector<ItemInfo> NSStarmanLib::CraftInfo::GetCraftMaterial()
{
    return m_materialList;
}
