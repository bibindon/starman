#include "AnimMeshAlloc.h"
#include "Common.h"

#include <algorithm>

using std::string;
using std::vector;

AnimMeshFrame::AnimMeshFrame(const string& name)
    : D3DXFRAME { },
    combined_matrix_ { }
{
    Name = new char[name.length() + 1];
    strcpy_s(Name, name.length() + 1, name.c_str());

    D3DXMatrixIdentity(&TransformationMatrix);
    D3DXMatrixIdentity(&combined_matrix_);
}

AnimMeshContainer::AnimMeshContainer(
    const string& x_filename,
    const string& mesh_name,
    LPD3DXMESH mesh,
    const D3DXMATERIAL* materials,
    const DWORD materials_count,
    const DWORD* adjacency)
    : D3DXMESHCONTAINER { }
{
    Name = new char[mesh_name.length() + 1];
    strcpy_s(Name, mesh_name.length() + 1, mesh_name.c_str());

    LPDIRECT3DDEVICE9 temp_d3d_device { nullptr };
    mesh->GetDevice(&temp_d3d_device);

    if (!(mesh->GetFVF() & D3DFVF_NORMAL))
    {
        MeshData.Type = D3DXMESHTYPE_MESH;
        HRESULT result { mesh->CloneMeshFVF(
            mesh->GetOptions(),
            mesh->GetFVF() | D3DFVF_NORMAL,
            temp_d3d_device,
            &MeshData.pMesh) };
        if (FAILED(result))
        {
            throw std::exception("Failed 'CloneMeshFVF' function.");
        }
        mesh = MeshData.pMesh;
        D3DXComputeNormals(mesh, nullptr);
    }
    else
    {
        MeshData.pMesh = mesh;
        MeshData.Type = D3DXMESHTYPE_MESH;
        mesh->AddRef();
    }

    NumMaterials = (std::max)(1UL, materials_count);
    pMaterials = new D3DXMATERIAL[NumMaterials];
    vector<LPDIRECT3DTEXTURE9> temp_texture(NumMaterials);
    textures_.swap(temp_texture);

    DWORD adjacency_count { mesh->GetNumFaces() * 3 };
    pAdjacency = new DWORD[adjacency_count];

    for (DWORD i { 0 }; i < adjacency_count; ++i)
    {
        pAdjacency[i] = adjacency[i];
    }

    if (materials_count > 0)
    {
        for (DWORD i { 0 }; i < materials_count; ++i)
        {
            pMaterials[i] = materials[i];
        }

        for (DWORD i { 0 }; i < materials_count; ++i)
        {
            pMaterials[i].MatD3D.Ambient = D3DCOLORVALUE { 0.2f, 0.2f, 0.2f, 0 };
            if (pMaterials[i].pTextureFilename != nullptr)
            {
                vector<char> buffer =
                   Common::get_model_texture_resource(x_filename, pMaterials[i].pTextureFilename);

                LPDIRECT3DTEXTURE9 temp_texture { };
                if (FAILED(D3DXCreateTextureFromFileInMemory(
                    temp_d3d_device,
                    &buffer[0],
                    static_cast<UINT>(buffer.size()),
                    &temp_texture)))
                {
                    throw std::exception("texture file is not found.");
                }
                else
                {
                    textures_.at(i)->Release();
                    textures_.at(i) = temp_texture;
                }
            }
        }
    }
    else
    {
        pMaterials[0].MatD3D.Diffuse = D3DCOLORVALUE { 0.5f, 0.5f, 0.5f, 0 };
        pMaterials[0].MatD3D.Ambient = D3DCOLORVALUE { 0.5f, 0.5f, 0.5f, 0 };
        pMaterials[0].MatD3D.Specular = pMaterials[0].MatD3D.Diffuse;
    }
}

AnimMeshAllocator::AnimMeshAllocator(const string& x_filename)
    : ID3DXAllocateHierarchy { },
    x_filename_(x_filename)
{
    // do nothing
}

STDMETHODIMP AnimMeshAllocator::CreateFrame(LPCTSTR name, LPD3DXFRAME* new_frame)
{
    *new_frame = new AnimMeshFrame { name };
    return S_OK;
}

STDMETHODIMP AnimMeshAllocator::CreateMeshContainer(
    LPCSTR mesh_name,
    CONST D3DXMESHDATA* mesh_data,
    CONST D3DXMATERIAL* materials,
    CONST D3DXEFFECTINSTANCE*,
    DWORD materials_count,
    CONST DWORD* adjacency,
    LPD3DXSKININFO,
    LPD3DXMESHCONTAINER* mesh_container)
{
    try
    {
        *mesh_container = new AnimMeshContainer {
            x_filename_,
            mesh_name,
            mesh_data->pMesh,
            materials,
            materials_count,
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
    LPD3DXMESHCONTAINER mesh_container_base)
{
    AnimMeshContainer* mesh_container {
        static_cast<AnimMeshContainer*>(mesh_container_base) };

    SAFE_RELEASE(mesh_container->pSkinInfo);
    SAFE_DELETE_ARRAY(mesh_container->Name);
    SAFE_DELETE_ARRAY(mesh_container->pAdjacency);
    SAFE_DELETE_ARRAY(mesh_container->pMaterials);
    SAFE_RELEASE(mesh_container->MeshData.pMesh);
    SAFE_DELETE(mesh_container);

    return S_OK;
}

