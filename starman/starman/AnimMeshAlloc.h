#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include <vector>
#include <memory>

struct AnimMeshFrame : public D3DXFRAME
{
    D3DXMATRIX m_combinedMatrix;
    explicit AnimMeshFrame(const std::string&);
};

struct AnimMeshContainer : public D3DXMESHCONTAINER
{
    std::vector<LPDIRECT3DTEXTURE9> m_vecTexture;
    AnimMeshContainer(
        const std::string&,
        const std::string&,
        LPD3DXMESH,
        const D3DXMATERIAL*,
        const DWORD,
        const DWORD*);
};

class AnimMeshAllocator : public ID3DXAllocateHierarchy
{
public:
    AnimMeshAllocator(const std::string&);
    STDMETHOD(CreateFrame)(THIS_ LPCTSTR, LPD3DXFRAME*);
    STDMETHOD(CreateMeshContainer)
        (THIS_ LPCSTR,
         CONST D3DXMESHDATA*,
         CONST D3DXMATERIAL*,
         CONST D3DXEFFECTINSTANCE*,
         DWORD,
         CONST DWORD*,
         LPD3DXSKININFO,
         LPD3DXMESHCONTAINER*);
    STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME);
    STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER);
private:
    std::string m_xFilename;
};

