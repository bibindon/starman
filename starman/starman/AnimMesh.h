#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include <vector>
#include <memory>
#include "AnimationStrategy.h"
#include "AnimController.h"

class AnimMeshAllocator;

class AnimMesh
{
public:
    AnimMesh(
        const std::string&,
        const D3DXVECTOR3&,
        const D3DXVECTOR3&,
        const float&,
        const AnimSetMap&);
    ~AnimMesh();

    void Render();
    void SetPos(const D3DXVECTOR3& pos);
    D3DXVECTOR3 GetPos() const;

    float GetScale() const;

    void SetRotate(const D3DXVECTOR3& rotate);
    void SetAnim(const std::string& animName, const DOUBLE& pos = -1.f);
    void SetAnimSpeed(const float speed);
    void SetTrackPos(const DOUBLE& pos);
    void SetCenterPos(const D3DXVECTOR3& pos);
    void SetRotateLocal(const D3DXVECTOR3& rotate);

private:

    struct frame_root_deleter_object
    {
        std::shared_ptr<AnimMeshAllocator> allocator_;

        void operator()(const LPD3DXFRAME);
        void release_mesh_allocator(const LPD3DXFRAME);
    };

    const std::string SHADER_FILENAME { "res\\shader\\animation_mesh_shader.fx" };
    std::shared_ptr<AnimMeshAllocator> m_allocator;
    std::unique_ptr<D3DXFRAME, frame_root_deleter_object> m_frameRoot;
    D3DXMATRIX m_rotationMatrix;
    D3DXMATRIX m_viewMatrix;
    D3DXMATRIX m_projMatrix;

    D3DXVECTOR3 m_centerPos;
    D3DXVECTOR3 m_rotateLocal{0.f, 0.f, 0.f};

    LPD3DXEFFECT m_D3DEffect { nullptr };

    D3DXHANDLE m_worldHandle { nullptr };
    D3DXHANDLE m_worldViewProjHandle { nullptr };

    void UpdateFrameMatrix(const LPD3DXFRAME, const LPD3DXMATRIX);
    void RenderFrame(const LPD3DXFRAME);
    void RenderMeshContainer(const LPD3DXMESHCONTAINER, const LPD3DXFRAME);


    D3DXVECTOR3 m_position;
    D3DXVECTOR3 m_rotation;
    float m_scale { 1.0f };
    std::string m_meshName;

    D3DXHANDLE m_meshTextureHandle { nullptr };
    D3DXHANDLE m_diffuseHandle { nullptr };
    D3DXHANDLE m_lightNormalHandle { nullptr };
    D3DXHANDLE m_brightnessHandle { nullptr };

    AnimController m_animCtrlr;
};

