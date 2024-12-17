#pragma once
#include <unordered_map>
#include <thread>
#include "Mesh.h"
#include "EnemyBase.h"
#include "MeshNoShade.h"
#include <map>

// ��l�����߂Â����烁�b�V����ǂݍ��ݕ\�����邽�߂̃��b�V���N���X
// �ʃX���b�h�œǂށBDirectX9�̓}���`�X���b�h�ɑΉ����Ă��Ȃ��炵����
// �Ȃ������Ȃ������B
// TODO �ʂ̃t�@�C���Ɉړ����ׂ��H
// TODO LazyMesh�͍폜����ׂ��BLoad
class LazyMesh
{
public:
    void Init(const std::string& xFilename,
              const D3DXVECTOR3& position,
              const D3DXVECTOR3& rotation);

    void Load();

    void Unload();

    // �ǂݍ��݂��J�n�������W�Ɣ��a
    void SetLoadPos(const D3DXVECTOR3& pos, const float r);

    // �ǂݍ��݂��J�n�������W���m�F
    bool IsLoadPos(const D3DXVECTOR3& pos);

    // �ǂݍ��ݍς݂�
    bool IsLoaded();

    void Render();

private:
    Mesh* m_Mesh { nullptr };
    bool m_bLoaded { false};
    std::thread* m_thread { nullptr };
    D3DXVECTOR3 m_loadingPos { 0.f, 0.f, 0.f };
    float m_radius { 0.f };

    std::string m_xFilename;
    D3DXVECTOR3 m_drawPos { 0.f, 0.f, 0.f };
    D3DXVECTOR3 m_rotation { 0.f, 0.f, 0.f };
};

class Map
{
public:
    Map();
    ~Map();
    void Init();
    void Update();
    void Render();
    std::vector<EnemyBase*> GetEnemy();
    void SetEnemy(const std::vector<EnemyBase*>& vecEnemy);

    // �Փ˔���
    // TODO �����Ɋg�嗦�����悤�ɂ���
    bool Intersect(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);
    bool IntersectSub(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, Mesh* mesh);

    // �ݒu����
    // TODO �����Ɋg�嗦�����悤�ɂ���
    bool CollisionGround(const D3DXVECTOR3& pos, const D3DXVECTOR3& move);
    bool CollisionGroundSub(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, Mesh* mesh);

    // �ǂ��菈��
    // TODO �����Ɋg�嗦�����悤�ɂ���
    D3DXVECTOR3 WallSlide(const D3DXVECTOR3& pos, const D3DXVECTOR3& move, bool* bHit);
private:
    D3DXVECTOR3 WallSlideSub(
        const D3DXVECTOR3& pos, Mesh* mesh, const D3DXVECTOR3& move, bool* bHit);
    std::unordered_map<std::string, Mesh*> m_meshMap;

    std::vector<EnemyBase*> m_vecEnemy { };

    int m_nStagenameCount { 0 };
    Sprite* m_spriteStageName { nullptr };

    LazyMesh m_lazyMesh;

    // ���z
    MeshNoShade* m_pSun = nullptr;

    // NPC
    std::map<std::string, AnimMesh*> m_NPC;
};

