#pragma once
#include <unordered_map>
#include "Mesh.h"
#include "Enemy.h"
class Stage1
{
public:
    Stage1();
    ~Stage1();
    void Init();
    void Update();
    void Render();
    std::vector<Enemy> GetEnemy();
    void SetEnemy(const std::vector<Enemy>& vecEnemy);

    // �Փ˔���
    bool Intersect(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);
    bool IntersectSub(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, Mesh* mesh);

    // �ݒu����
    bool CollisionGround(const D3DXVECTOR3& pos, const D3DXVECTOR3& move);
    bool CollisionGroundSub(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, Mesh* mesh);

    // �ǂ��菈��
    // ���݁A�Ζʂ�o�邽�߂ɁA�n�ʂɑ΂��Ă͕ǂ��菈�������邪
    // �ǂɑ΂��ĕǂ��������\��͂Ȃ��B
    D3DXVECTOR3 WallSlide(const D3DXVECTOR3& pos, const D3DXVECTOR3& move);
private:
    void WallSlideSub(const D3DXVECTOR3& pos, Mesh* mesh, const D3DXVECTOR3& move, D3DXVECTOR3& result);
    std::unordered_map<std::string, Mesh*> m_meshMap;
    std::vector<Enemy> m_vecEnemy { };
    int m_nStagenameCount { 0 };
    Sprite* m_spriteStageName { nullptr };
};

