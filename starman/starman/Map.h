#pragma once
#include <unordered_map>
#include <thread>
#include "Mesh.h"
#include "EnemyBase.h"
#include "MeshNoShade.h"
#include "MeshClone.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/ItemManager.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/StatusManager.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/MapObjManager.h"

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
    bool Intersect(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);
    bool IntersectSub(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, Mesh* mesh);

    // �������B�������Bpos���_���B
    bool IntersectSub2(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, Mesh* mesh);
    bool IntersectSub(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, MeshClone* mesh);

    // �C�Ɛڂ��Ă��邩
    bool IntersectWater(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);

    //--------------------------------------------------
    // �ݒu����
    //--------------------------------------------------

    // �n�ʂ̂�
    bool CollisionGround(const D3DXVECTOR3& pos, const D3DXVECTOR3& move);
    bool CollisionGroundSub(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, Mesh* mesh);
    bool CollisionGroundSub(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, MeshClone* mesh);

    // �ǂ��菈��
    D3DXVECTOR3 WallSlide(const D3DXVECTOR3& pos, const D3DXVECTOR3& move, bool* bHit);

    void SetNpcPos(const std::string& name, const D3DXVECTOR3& pos);
    void SetNpcRot(const std::string& name, const float yRot);

    //------------------------------------
    // ���̓���
    //------------------------------------

    void AddThrownItem(const D3DXVECTOR3& pos,
                       const D3DXVECTOR3& move,
                       const std::string& weaponName,
                       const NSStarmanLib::ItemInfo& itemInfo,
                       const float scale = 1.f,
                       const float power = 10.f,
                       const float rotY = 0.f);

    // �����Ēn�ʂɗ���������̂����߂��̂��̂�1�擾�i���a2���[�g���ȓ��j
    NSStarmanLib::ItemInfo GetThrownItem(const D3DXVECTOR3& pos);

    // �����Ēn�ʂɗ�����������폜
    void DeleteThrownItem(const NSStarmanLib::ItemInfo& thrownItem);

    //------------------------------------
    // ���@�U��
    //------------------------------------

    // ���@�U���͕���𓊂���̂Ɠ����悤�ȏ������s��
    // ����𓊂������ƈႢ�A�n�ʂɗ������������B
    void SetThrownMagic(const D3DXVECTOR3& pos,
                        const D3DXVECTOR3& move,
                        const NSStarmanLib::eMagicType& magicType,
                        const float power);

    // ���̋߂��ɂ��邩
    bool NearPlant(const D3DXVECTOR3& pos);

    // �؂̋߂��ɂ��邩
    bool NearTree(const D3DXVECTOR3& pos);

    // �Q���[�g���ȓ��̖؂��P�{����
    void DeleteTree(const D3DXVECTOR3& pos);

    // �Q���[�g���ȓ��̑����P�{����
    void DeletePlant(const D3DXVECTOR3& pos);

private:
    NSStarmanLib::MapObjManager* MapLib();

    D3DXVECTOR3 WallSlideSub(
        const D3DXVECTOR3& pos, Mesh* mesh, const D3DXVECTOR3& move, bool* bHit);

    D3DXVECTOR3 WallSlideSub(
        const D3DXVECTOR3& pos, MeshClone* mesh, const D3DXVECTOR3& move, bool* bHit);

    std::unordered_map<std::string, Mesh*> m_meshMap;

    std::vector<EnemyBase*> m_vecEnemy { };

    // ���z
    MeshNoShade* m_pSun = nullptr;

    // NPC
    std::unordered_map<std::string, AnimMesh*> m_NPC;

    std::unordered_map<int, MeshClone*> m_meshCloneMap;

    //-------------------------------------------------------
    // ������ꂽ���́i����j
    //-------------------------------------------------------

    // ���������
    struct ThrownItem
    {
        NSStarmanLib::ItemInfo m_itemInfo;
        D3DXVECTOR3 m_move;
        MeshClone* m_mesh;

        // ��x�ł��G�Ƀq�b�g����
        bool m_bHit = false;

        // �n�ʂɗ�����
        bool m_bStop = false;

        // 0.5�b�҂��Ă��甭��
        int m_counter = 0;

        // �З�
        float m_power = 0.f;
    };

    std::vector<ThrownItem> m_thrownList;

    //-------------------------------------------------------
    // ������ꂽ���́i���@�j
    //-------------------------------------------------------

    // ��������́i���@�j
    struct ThrownMagic
    {
        NSStarmanLib::eMagicType m_eMagicType;
        D3DXVECTOR3 m_move;

        MeshNoShade* m_mesh;

        // ��x�ł��G�Ƀq�b�g����
        bool m_bHit = false;

        // 1�b�҂��Ă��甭��
        int m_counter = 0;

        // �З�
        float m_magicPower = 0.f;
    };

    std::vector<ThrownMagic> m_thrownMagicList;

};

