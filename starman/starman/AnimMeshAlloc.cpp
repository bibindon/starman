#include "AnimMeshAlloc.h"
#include "Common.h"

#include <algorithm>

using std::string;
using std::vector;

AnimMeshFrame::AnimMeshFrame(const string& name)
    : D3DXFRAME { },
    m_combinedMatrix { }
{
    Name = new char[name.length() + 1];
    strcpy_s(Name, name.length() + 1, name.c_str());

    D3DXMatrixIdentity(&TransformationMatrix);
    D3DXMatrixIdentity(&m_combinedMatrix);
}

AnimMeshContainer::AnimMeshContainer(
    const string& xFilename,
    const string& meshName,
    LPD3DXMESH d3dMesh,
    const D3DXMATERIAL* materials,
    const DWORD materialsCount,
    const DWORD* adjacency)
    : D3DXMESHCONTAINER { }
{
    Name = new char[meshName.length() + 1];
    strcpy_s(Name, meshName.length() + 1, meshName.c_str());

    LPDIRECT3DDEVICE9 d3dDevice { nullptr };
    d3dMesh->GetDevice(&d3dDevice);

    if (!(d3dMesh->GetFVF() & D3DFVF_NORMAL))
    {
        MeshData.Type = D3DXMESHTYPE_MESH;
        HRESULT result { d3dMesh->CloneMeshFVF(
            d3dMesh->GetOptions(),
            d3dMesh->GetFVF() | D3DFVF_NORMAL,
            d3dDevice,
            &MeshData.pMesh) };
        if (FAILED(result))
        {
            throw std::exception("Failed 'CloneMeshFVF' function.");
        }
        d3dMesh = MeshData.pMesh;
        D3DXComputeNormals(d3dMesh, nullptr);
    }
    else
    {
        MeshData.pMesh = d3dMesh;
        MeshData.Type = D3DXMESHTYPE_MESH;
        d3dMesh->AddRef();
    }

    NumMaterials = (std::max)(1UL, materialsCount);
    pMaterials = new D3DXMATERIAL[NumMaterials];
    vector<LPDIRECT3DTEXTURE9> tempTexture(NumMaterials);
    m_vecTexture.swap(tempTexture);

    DWORD adjacencyCount { d3dMesh->GetNumFaces() * 3 };
    pAdjacency = new DWORD[adjacencyCount];

    for (DWORD i { 0 }; i < adjacencyCount; ++i)
    {
        pAdjacency[i] = adjacency[i];
    }

    if (materialsCount > 0)
    {
        for (DWORD i { 0 }; i < materialsCount; ++i)
        {
            pMaterials[i] = materials[i];
        }

        std::string xFileDir = xFilename;
        std::size_t lastPos = xFileDir.find_last_of("\\");
        xFileDir = xFileDir.substr(0, lastPos + 1);

        for (DWORD i { 0 }; i < materialsCount; ++i)
        {
            pMaterials[i].MatD3D.Ambient = D3DCOLORVALUE { 0.2f, 0.2f, 0.2f, 0 };
            if (pMaterials[i].pTextureFilename != nullptr)
            {
                std::string texPath = xFileDir;
                texPath += materials[i].pTextureFilename;
                LPDIRECT3DTEXTURE9 tempTexture { nullptr };

                if (FAILED(D3DXCreateTextureFromFile(
                    d3dDevice,
                    texPath.c_str(),
                    &tempTexture)))
                {
                    throw std::exception("texture file is not found.");
                }
                else
                {
                    SAFE_RELEASE(m_vecTexture.at(i));
                    m_vecTexture.at(i) = tempTexture;
                }
            }
        }
    }
    else
    {
        pMaterials[0].MatD3D.Diffuse = D3DCOLORVALUE { 0.5f, 0.5f, 0.5f, 0 };
        pMaterials[0].MatD3D.Ambient = D3DCOLORVALUE { 0.5f, 0.5f, 0.5f, 0 };
        pMaterials[0].MatD3D.Specular = D3DCOLORVALUE { 0.5f, 0.5f, 0.5f, 0 };
    }
}

AnimMeshAllocator::AnimMeshAllocator(const string& xFilename)
    : ID3DXAllocateHierarchy { },
    m_xFilename(xFilename)
{
    // do nothing
}

STDMETHODIMP AnimMeshAllocator::CreateFrame(LPCTSTR name, LPD3DXFRAME* newFrame)
{
    *newFrame = new AnimMeshFrame { name };
    return S_OK;
}

STDMETHODIMP AnimMeshAllocator::CreateMeshContainer(
    LPCSTR meshName,
    CONST D3DXMESHDATA* meshData,
    CONST D3DXMATERIAL* materials,
    CONST D3DXEFFECTINSTANCE*,
    DWORD materialCount,
    CONST DWORD* adjacency,
    LPD3DXSKININFO,
    LPD3DXMESHCONTAINER* meshContainer)
{
    try
    {
        *meshContainer = new AnimMeshContainer {
            m_xFilename,
            meshName,
            meshData->pMesh,
            materials,
            materialCount,
            adjacency };
    }
    catch (...)
    {
        return E_FAIL;
    }

    return S_OK;
}

STDMETHODIMP AnimMeshAllocator::DestroyFrame(LPD3DXFRAME frame)
{
    SAFE_DELETE_ARRAY(frame->Name);
    frame->~D3DXFRAME();
    SAFE_DELETE(frame);
    return S_OK;
}

STDMETHODIMP AnimMeshAllocator::DestroyMeshContainer(
    LPD3DXMESHCONTAINER meshContainerBase)
{
    AnimMeshContainer* meshContainer { static_cast<AnimMeshContainer*>(meshContainerBase) };

    SAFE_RELEASE(meshContainer->pSkinInfo);
    SAFE_DELETE_ARRAY(meshContainer->Name);
    SAFE_DELETE_ARRAY(meshContainer->pAdjacency);
    SAFE_DELETE_ARRAY(meshContainer->pMaterials);
    SAFE_RELEASE(meshContainer->MeshData.pMesh);
    SAFE_DELETE(meshContainer);

    return S_OK;
}

