#include "Mesh.h"

using std::shared_ptr;
using std::make_shared;
using std::string;
using std::unique_ptr;
using std::vector;

const string Mesh::SHADER_FILENAME = "mesh_shader.fx";

Mesh::Mesh(
    const LPDIRECT3DDEVICE9 d3d_device,
    const string& x_filename,
    const D3DXVECTOR3& position,
    const D3DXVECTOR3& rotation,
    const float& scale)
{
    animation_strategy_.reset(new no_animation { });

    HRESULT result { 0 };
    world_view_proj_handle_ = effect_->GetParameterByName(nullptr, "g_world_view_projection");

    LPD3DXBUFFER adjacency_buffer { nullptr };
    LPD3DXBUFFER material_buffer { nullptr };
    LPD3DXMESH temp_mesh { nullptr };

//    vector<char> buffer { util::get_model_resource(x_filename) };

    result = D3DXLoadMeshFromX(
        x_filename.c_str(),
        D3DXMESH_SYSTEMMEM,
        d3d_device_,
        &adjacency_buffer,
        &material_buffer,
        nullptr,
        &materials_count_,
        &temp_mesh);

    if (FAILED(result))
    {
        throw std::exception("Failed to load a x-file.");
    }

    SAFE_RELEASE(d3dx_mesh_);
    d3dx_mesh_ = temp_mesh;

    D3DVERTEXELEMENT9 decl[] = {
        {
            0,
            0,
            D3DDECLTYPE_FLOAT3,
            D3DDECLMETHOD_DEFAULT,
            D3DDECLUSAGE_POSITION,
            0},
        {
            0,
            12,
            D3DDECLTYPE_FLOAT3,
            D3DDECLMETHOD_DEFAULT,
            D3DDECLUSAGE_NORMAL,
            0
        },
        {
            0,
            24,
            D3DDECLTYPE_FLOAT2,
            D3DDECLMETHOD_DEFAULT,
            D3DDECLUSAGE_TEXCOORD,
            0
        },
        D3DDECL_END(),
    };

    result = d3dx_mesh_->CloneMesh(D3DXMESH_MANAGED, decl, d3d_device_, &temp_mesh);

    if (FAILED(result))
    {
        throw std::exception("Failed 'CloneMesh' function.");
    }
    SAFE_RELEASE(d3dx_mesh_);
    d3dx_mesh_ = temp_mesh;
    DWORD* word_buffer { static_cast<DWORD*>(adjacency_buffer->GetBufferPointer()) };

    result = D3DXComputeNormals(d3dx_mesh_, word_buffer);

    if (FAILED(result))
    {
        throw std::exception("Failed 'D3DXComputeNormals' function.");
    }

    result = d3dx_mesh_->OptimizeInplace(
        D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
        word_buffer,
        nullptr,
        nullptr,
        nullptr);

    SAFE_RELEASE(adjacency_buffer);

    if (FAILED(result))
    {
        throw std::exception("Failed 'OptimizeInplace' function.");
    }

    colors_.insert(begin(colors_), materials_count_, D3DCOLORVALUE { });
    vector<LPDIRECT3DTEXTURE9> temp_textures { materials_count_ };
    textures_.swap(temp_textures);

    D3DXMATERIAL* materials { static_cast<D3DXMATERIAL*>(material_buffer->GetBufferPointer()) };

    for (DWORD i { 0 }; i < materials_count_; ++i)
    {
        colors_.at(i) = materials[i].MatD3D.Diffuse;
        if (materials[i].pTextureFilename != nullptr)
        {
//            buffer = util::get_model_texture_resource(x_filename, materials[i].pTextureFilename);
            LPDIRECT3DTEXTURE9 temp_texture { };
            if (FAILED(D3DXCreateTextureFromFile(
                d3d_device_,
                x_filename.c_str(),
                &temp_texture)))
            {
                throw std::exception("texture file is not found.");
            }
            else
            {
                SAFE_RELEASE(textures_.at(i));
                textures_.at(i) = temp_texture;
            }
        }
    }
    SAFE_RELEASE(material_buffer);

    scale_ = scale;
}

Mesh::~Mesh()
{
}

void Mesh::render_impl(const D3DXMATRIX& view_matrix, const D3DXMATRIX& projection_matrix)
{
    D3DXMATRIX world_view_projection_matrix { };
    D3DXMatrixIdentity(&world_view_projection_matrix);
    {
        D3DXMATRIX mat { };

        D3DXMatrixTranslation(
            &mat, -center_coodinate_.x, -center_coodinate_.y, -center_coodinate_.z);
        world_view_projection_matrix *= mat;

        D3DXMatrixScaling(&mat, scale_, scale_, scale_);
        world_view_projection_matrix *= mat;

        D3DXMatrixRotationYawPitchRoll(&mat, rotation_.x, rotation_.y, rotation_.z);
        world_view_projection_matrix *= mat;

        D3DXMatrixTranslation(&mat, position_.x, position_.y, position_.z);
        world_view_projection_matrix *= mat;
    }
    world_view_projection_matrix *= view_matrix;
    world_view_projection_matrix *= projection_matrix;

    effect_->SetMatrix(world_view_proj_handle_, &world_view_projection_matrix);

    effect_->Begin(nullptr, 0);

    HRESULT result { };
    if (FAILED(result = effect_->BeginPass(0)))
    {
        effect_->End();
        throw std::exception("Failed 'BeginPass' function.");
    }

    for (DWORD i { 0 }; i < materials_count_; ++i)
    {
        // TODO : remove redundant set****.
        D3DXVECTOR4 vec4_color {
            colors_.at(i).r,
            colors_.at(i).g,
            colors_.at(i).b,
            colors_.at(i).a
        };
        effect_->SetVector(diffuse_handle_, &vec4_color);
        effect_->SetTexture(mesh_texture_handle_, textures_.at(i));
        effect_->CommitChanges();
        d3dx_mesh_->DrawSubset(i);
    }
    effect_->EndPass();
    effect_->End();
}



