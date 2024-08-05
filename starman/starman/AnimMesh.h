#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include <vector>
#include <memory>
#include "AnimationStrategy.h"

class AnimMeshAllocator;

class AnimMesh
{
public:
    AnimMesh(
        const std::shared_ptr<IDirect3DDevice9>&,
        const std::string&,
        const D3DXVECTOR3&,
        const D3DXVECTOR3&,
        const float&);
    ~AnimMesh();

private:
    void render_impl(const D3DXMATRIX&, const D3DXMATRIX&);

    struct frame_root_deleter_object
    {
        std::shared_ptr<AnimMeshAllocator> allocator_;

        void operator()(const LPD3DXFRAME);
        void release_mesh_allocator(const LPD3DXFRAME);
    };

    const static std::string SHADER_FILENAME;
    std::shared_ptr<IDirect3DDevice9> d3d_device_;
    std::shared_ptr<AnimMeshAllocator> allocator_;
    std::unique_ptr<D3DXFRAME, frame_root_deleter_object> frame_root_;
    D3DXMATRIX rotation_matrix_;
    D3DXMATRIX view_matrix_;
    D3DXMATRIX projection_matrix_;

    D3DXVECTOR3 center_coodinate_;

    // For effect.

    LPD3DXEFFECT effect_;

    D3DXHANDLE world_handle_;
    D3DXHANDLE world_view_proj_handle_;

    void update_frame_matrix(const LPD3DXFRAME, const LPD3DXMATRIX);
    void render_frame(const LPD3DXFRAME);
    void render_mesh_container(const LPD3DXMESHCONTAINER, const LPD3DXFRAME);


    D3DXVECTOR3 position_;
    D3DXVECTOR3 rotation_;
    float scale_;
    std::string mesh_name_;

    D3DXHANDLE mesh_texture_handle_;
    D3DXHANDLE diffuse_handle_;

    std::unique_ptr<animation_strategy> animation_strategy_;
};

