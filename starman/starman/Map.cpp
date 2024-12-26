#include "Map.h"
#include "Light.h"
#include "SharedObj.h"
#include "KeyBoard.h"
#include "EnemyCube.h"
#include "EnemyDisk.h"
#include "EnemySphere.h"
#include "EnemyBigCube.h"
#include "EnemyEnban.h"
#include "EnemyEnchu.h"
#include "EnemyGhost.h"
#include "EnemyHanen.h"
#include "EnemyHankyuu.h"
#include "EnemyOrangeCube.h"
#include "EnemySmallCube.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/EnemyInfoManager.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/PowereggDateTime.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/MapObjManager.h"

Map::Map()
{
}

Map::~Map()
{
    for (auto& pair : m_meshMap)
    {
        SAFE_DELETE(pair.second);
    }
}

void Map::Init()
{
    {
        Mesh* mesh = new Mesh("res\\model\\prolitan\\prolitan.x",
                              D3DXVECTOR3(0.f, 0.f, 0.f),
                              D3DXVECTOR3(0.f, 0.f, 0.f),
                              1.0f);
        mesh->Init();
        m_meshMap["Map"] = mesh;
    }
    {
        Mesh* mesh = new Mesh("res\\model\\prolitan\\sea.x",
                              D3DXVECTOR3(10.f, 10.f, 10.f),
                              D3DXVECTOR3(0.f, 0.f, 0.f),
                              1.0f);
        mesh->Init();
        m_meshMap["sea"] = mesh;
    }
    {
        Mesh* mesh = new Mesh("res\\model\\cube6\\cube6.x",
                              D3DXVECTOR3(-10.f, 0.f, 0.f),
                              D3DXVECTOR3(0.f, 0.f, 0.f),
                              1.0f);
        mesh->Init();
        m_meshMap["cube61"] = mesh;

        mesh = new Mesh("res\\model\\cube6\\cube6.x",
                        D3DXVECTOR3(-13.f, 1.f, 0.f),
                        D3DXVECTOR3(0.f, 0.f, 0.f),
                        1.0f);
        mesh->Init();
        m_meshMap["cube62"] = mesh;

        mesh = new Mesh("res\\model\\cube6\\cube6.x",
                        D3DXVECTOR3(-16.f, 2.f, 0.f),
                        D3DXVECTOR3(0.f, 0.f, 0.f),
                        1.0f);
        mesh->Init();
        m_meshMap["cube63"] = mesh;

        mesh = new Mesh("res\\model\\collisionTest\\colli.x",
                        D3DXVECTOR3(0.f, 0.f, -20.f),
                        D3DXVECTOR3(0.f, 0.f, 0.f),
                        1.0f);
        mesh->Init();
        m_meshMap["colli"] = mesh;
    }
    // �x���ǂݍ���
    {
        m_lazyMesh.Init("res\\model\\prolitan\\prolitan1.x",
                        D3DXVECTOR3(0.f, 0.f, 0.f),
                        D3DXVECTOR3(0.f, 0.f, 0.f));
        m_lazyMesh.SetLoadPos(D3DXVECTOR3(367.f, 343.f, 755.f), 400.f);
    }

    {
        Mesh* mesh { nullptr };
        D3DXVECTOR3 b = D3DXVECTOR3(0.f, 0.f, 0.f);
        D3DXVECTOR3 c = D3DXVECTOR3(0.f, 0.f, 0.f);
        b.x = 15.f;
        b.y = -80.f;
        mesh = new Mesh("res\\model\\hemisphere\\hemisphere.x", b, c, 9000.0f);
        mesh->Init();
        m_meshMap["sky"] = mesh;

        // ����]��������B�Ȃ��Ă������ڂɈႢ���Ȃ��B
        //c.y = D3DX_PI;
        //mesh = new Mesh("res\\model\\hemisphere\\hemisphere.x", b, c, 3000.0f);
        //mesh->Init();
        //m_meshMap["sky2"] = mesh;
    }
    {
        D3DXVECTOR3 b = D3DXVECTOR3(-285.f, 14.f, 530.f);
        D3DXVECTOR3 c = D3DXVECTOR3(0.f, 0.f, 0.f);
        Mesh* mesh = new Mesh("res\\model\\tree1\\tree1.x", b, c, 1.f);
        mesh->Init();
        m_meshMap["tree"] = mesh;
    }
    {
        D3DXVECTOR3 b = D3DXVECTOR3(-285.f, 20.f, 520.f);
        D3DXVECTOR3 c = D3DXVECTOR3(0.f, 0.f, 0.f);
        Mesh* mesh = new Mesh("res\\model\\rock1\\rock1.x", b, c, 1.f);
        mesh->Init();
        m_meshMap["rock1"] = mesh;
    }
    {
        D3DXVECTOR3 b = D3DXVECTOR3(10.f, 0.f, 20.f);
        D3DXVECTOR3 c = D3DXVECTOR3(0.f, 0.f, 0.f);
        Mesh* mesh = new Mesh("res\\model\\cottage\\cottage.x", b, c, 1.f);
        mesh->Init();
        m_meshMap["cottage"] = mesh;
    }

    //--------------------------------------------
    // ���z
    //--------------------------------------------
    m_pSun = new MeshNoShade("res\\model\\sun\\sun.x",
                             D3DXVECTOR3(0.f, -10000.f, 0.f),
                             D3DXVECTOR3(0.f, 0.f, 0.f),
                             1000.0f);
    m_pSun->Init();

    //--------------------------------------------
    // NPC
    // TODO NpcManager�Ƃ����N���X������Ă�����ł��ׂ��B
    //--------------------------------------------
    {
        D3DXVECTOR3 b = D3DXVECTOR3(-285.f, 16.f, 541.f);
        D3DXVECTOR3 rot = D3DXVECTOR3(0, D3DX_PI, 0);
        AnimSetMap animSetMap;
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 0.f;
            animSetting.m_duration = 0.5f;
            animSetting.m_loop = true;
            animSetMap["Idle"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 1.f;
            animSetting.m_duration = 1.f;
            animSetting.m_loop = false;
            animSetMap["Walk"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 2.f;
            animSetting.m_duration = 1.f;
            animSetting.m_loop = false;
            animSetMap["Attack"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 3.f;
            animSetting.m_duration = 0.5f;
            animSetting.m_loop = false;
            animSetMap["Damaged"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 3.5f;
            animSetting.m_duration = 1.f;
            animSetting.m_loop = false;
            animSetMap["Dead"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 5.f;
            animSetting.m_duration = 2.f;
            animSetting.m_loop = false;
            animSetMap["Jump"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 7.1f;
            animSetting.m_duration = 0.5f;
            animSetting.m_loop = true;
            animSetMap["Sit"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 7.7f;
            animSetting.m_duration = 0.6f;
            animSetting.m_loop = true;
            animSetMap["LieDown"] = animSetting;
        }
        AnimMesh* daikeiman = new AnimMesh("res\\model\\daikeiman\\daikeiman.x", b, rot, 1.f, animSetMap);
        daikeiman->SetAnim("LieDown");
        m_NPC["�_�C�P�C�}��"] = daikeiman;
    }
    {

        D3DXVECTOR3 b = D3DXVECTOR3(-285.f, 16.f, 543.f);
        D3DXVECTOR3 rot = D3DXVECTOR3(0, D3DX_PI, 0);
        AnimSetMap animSetMap;
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 0.f;
            animSetting.m_duration = 0.5f;
            animSetting.m_loop = true;
            animSetMap["Idle"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 1.f;
            animSetting.m_duration = 1.f;
            animSetting.m_loop = false;
            animSetMap["Walk"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 2.f;
            animSetting.m_duration = 1.f;
            animSetting.m_loop = false;
            animSetMap["Attack"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 3.f;
            animSetting.m_duration = 0.5f;
            animSetting.m_loop = false;
            animSetMap["Damaged"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 3.5f;
            animSetting.m_duration = 1.f;
            animSetting.m_loop = false;
            animSetMap["Dead"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 5.f;
            animSetting.m_duration = 2.f;
            animSetting.m_loop = false;
            animSetMap["Jump"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 7.1f;
            animSetting.m_duration = 0.5f;
            animSetting.m_loop = true;
            animSetMap["Sit"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 7.7f;
            animSetting.m_duration = 0.6f;
            animSetting.m_loop = true;
            animSetMap["LieDown"] = animSetting;
        }
        AnimMesh* sankakuman = new AnimMesh("res\\model\\sankakuman\\sankakuman.x", b, rot, 0.66f, animSetMap);
        sankakuman->SetAnim("Sit");
        m_NPC["�T���J�N�}��"] = sankakuman;
    }
    {

        D3DXVECTOR3 b = D3DXVECTOR3(-285.f, 16.f, 545.f);
        D3DXVECTOR3 rot = D3DXVECTOR3(0, D3DX_PI, 0);
        AnimSetMap animSetMap;
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 0.f;
            animSetting.m_duration = 0.5f;
            animSetting.m_loop = true;
            animSetMap["Idle"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 1.f;
            animSetting.m_duration = 1.f;
            animSetting.m_loop = false;
            animSetMap["Walk"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 2.f;
            animSetting.m_duration = 1.f;
            animSetting.m_loop = false;
            animSetMap["Attack"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 3.f;
            animSetting.m_duration = 0.5f;
            animSetting.m_loop = false;
            animSetMap["Damaged"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 3.5f;
            animSetting.m_duration = 1.f;
            animSetting.m_loop = false;
            animSetMap["Dead"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 5.f;
            animSetting.m_duration = 2.f;
            animSetting.m_loop = false;
            animSetMap["Jump"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 7.1f;
            animSetting.m_duration = 0.5f;
            animSetting.m_loop = true;
            animSetMap["Sit"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 7.7f;
            animSetting.m_duration = 0.6f;
            animSetting.m_loop = true;
            animSetMap["LieDown"] = animSetting;
        }
        AnimMesh* shikakuman = new AnimMesh("res\\model\\shikakuman\\shikakuman.x", b, rot, 0.66f, animSetMap);
        shikakuman->SetAnim("Sit");
        m_NPC["�V�J�N�}��"] = shikakuman;
    }
    {

        D3DXVECTOR3 b = D3DXVECTOR3(-285.f, 16.f, 547.f);
        D3DXVECTOR3 rot = D3DXVECTOR3(0, D3DX_PI, 0);
        AnimSetMap animSetMap;
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 0.f;
            animSetting.m_duration = 0.5f;
            animSetting.m_loop = true;
            animSetMap["Idle"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 1.f;
            animSetting.m_duration = 1.f;
            animSetting.m_loop = false;
            animSetMap["Walk"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 2.f;
            animSetting.m_duration = 1.f;
            animSetting.m_loop = false;
            animSetMap["Attack"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 3.f;
            animSetting.m_duration = 0.5f;
            animSetting.m_loop = false;
            animSetMap["Damaged"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 3.5f;
            animSetting.m_duration = 1.f;
            animSetting.m_loop = false;
            animSetMap["Dead"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 5.f;
            animSetting.m_duration = 2.f;
            animSetting.m_loop = false;
            animSetMap["Jump"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 7.1f;
            animSetting.m_duration = 0.5f;
            animSetting.m_loop = true;
            animSetMap["Sit"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 7.7f;
            animSetting.m_duration = 0.6f;
            animSetting.m_loop = true;
            animSetMap["LieDown"] = animSetting;
        }
        AnimMesh* animMesh = new AnimMesh("res\\model\\vim\\vim.x", b, rot, 1.f, animSetMap);
        animMesh->SetAnim("Idle");
        m_NPC["�r��"] = animMesh;
    }
    {
        D3DXVECTOR3 b = D3DXVECTOR3(-285.f, 16.f, 549.f);
        D3DXVECTOR3 rot = D3DXVECTOR3(0, D3DX_PI, 0);
        AnimSetMap animSetMap;
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 0.f;
            animSetting.m_duration = 0.5f;
            animSetting.m_loop = true;
            animSetMap["Idle"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 1.f;
            animSetting.m_duration = 1.f;
            animSetting.m_loop = false;
            animSetMap["Walk"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 2.f;
            animSetting.m_duration = 1.f;
            animSetting.m_loop = false;
            animSetMap["Attack"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 3.f;
            animSetting.m_duration = 0.5f;
            animSetting.m_loop = false;
            animSetMap["Damaged"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 3.5f;
            animSetting.m_duration = 1.f;
            animSetting.m_loop = false;
            animSetMap["Dead"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 5.f;
            animSetting.m_duration = 2.f;
            animSetting.m_loop = false;
            animSetMap["Jump"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 7.1f;
            animSetting.m_duration = 0.5f;
            animSetting.m_loop = true;
            animSetMap["Sit"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 7.7f;
            animSetting.m_duration = 0.6f;
            animSetting.m_loop = true;
            animSetMap["LieDown"] = animSetting;
        }
        AnimMesh* animMesh = new AnimMesh("res\\model\\vsco\\vsco.x", b, rot, 1.f, animSetMap);
        animMesh->SetAnim("Idle");
        m_NPC["�u�C�G�X�R"] = animMesh;
    }
    {
        D3DXVECTOR3 b = D3DXVECTOR3(-285.f, 16.f, 551.f);
        D3DXVECTOR3 rot = D3DXVECTOR3(0, D3DX_PI, 0);
        AnimSetMap animSetMap;
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 0.f;
            animSetting.m_duration = 0.5f;
            animSetting.m_loop = true;
            animSetMap["Idle"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 1.f;
            animSetting.m_duration = 1.f;
            animSetting.m_loop = false;
            animSetMap["Walk"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 2.f;
            animSetting.m_duration = 1.f;
            animSetting.m_loop = false;
            animSetMap["Attack"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 3.f;
            animSetting.m_duration = 0.5f;
            animSetting.m_loop = false;
            animSetMap["Damaged"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 3.5f;
            animSetting.m_duration = 1.f;
            animSetting.m_loop = false;
            animSetMap["Dead"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 5.f;
            animSetting.m_duration = 2.f;
            animSetting.m_loop = false;
            animSetMap["Jump"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 7.1f;
            animSetting.m_duration = 0.5f;
            animSetting.m_loop = true;
            animSetMap["Sit"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 7.7f;
            animSetting.m_duration = 0.6f;
            animSetting.m_loop = true;
            animSetMap["LieDown"] = animSetting;
        }
        AnimMesh* animMesh = new AnimMesh("res\\model\\webnin\\webnin.x", b, rot, 1.f, animSetMap);
        animMesh->SetAnim("Idle");
        m_NPC["�E�F�u�j��"] = animMesh;
    }
    {
        D3DXVECTOR3 b = D3DXVECTOR3(-285.f, 16.f, 533.f);
        D3DXVECTOR3 rot = D3DXVECTOR3(0, D3DX_PI, 0);
        AnimSetMap animSetMap;
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 0.f;
            animSetting.m_duration = 2.0f;
            animSetting.m_loop = true;
            animSetMap["Idle"] = animSetting;
        }
        //AnimMesh* animMesh = new AnimMesh("res\\model\\xeoff\\xeoff.x", b, rot, 1.5f, animSetMap);
        //animMesh->SetAnim("Idle");
        //m_NPC["�W�[�I�t"] = animMesh;
    }
    {
        D3DXVECTOR3 b = D3DXVECTOR3(-285.f, 16.f, 555.f);
        D3DXVECTOR3 rot = D3DXVECTOR3(0, D3DX_PI, 0);
        AnimSetMap animSetMap;
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 0.f;
            animSetting.m_duration = 0.5f;
            animSetting.m_loop = true;
            animSetMap["Idle"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 1.f;
            animSetting.m_duration = 1.f;
            animSetting.m_loop = false;
            animSetMap["Walk"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 2.f;
            animSetting.m_duration = 1.f;
            animSetting.m_loop = false;
            animSetMap["Attack"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 3.f;
            animSetting.m_duration = 0.5f;
            animSetting.m_loop = false;
            animSetMap["Damaged"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 3.5f;
            animSetting.m_duration = 1.f;
            animSetting.m_loop = false;
            animSetMap["Dead"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 5.f;
            animSetting.m_duration = 2.f;
            animSetting.m_loop = false;
            animSetMap["Jump"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 7.1f;
            animSetting.m_duration = 0.5f;
            animSetting.m_loop = true;
            animSetMap["Sit"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 7.7f;
            animSetting.m_duration = 0.6f;
            animSetting.m_loop = true;
            animSetMap["LieDown"] = animSetting;
        }
        AnimMesh* animMesh = new AnimMesh("res\\model\\gain\\gain.x", b, rot, 1.f, animSetMap);
        animMesh->SetAnim("Idle");
        m_NPC["�K�C��"] = animMesh;
    }
    {
        D3DXVECTOR3 b = D3DXVECTOR3(-285.f, 16.f, 557.f);
        D3DXVECTOR3 rot = D3DXVECTOR3(0, D3DX_PI, 0);
        AnimSetMap animSetMap;
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 0.f;
            animSetting.m_duration = 0.5f;
            animSetting.m_loop = true;
            animSetMap["Idle"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 1.f;
            animSetting.m_duration = 1.f;
            animSetting.m_loop = false;
            animSetMap["Walk"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 2.f;
            animSetting.m_duration = 1.f;
            animSetting.m_loop = false;
            animSetMap["Attack"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 3.f;
            animSetting.m_duration = 0.5f;
            animSetting.m_loop = false;
            animSetMap["Damaged"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 3.5f;
            animSetting.m_duration = 1.f;
            animSetting.m_loop = false;
            animSetMap["Dead"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 5.f;
            animSetting.m_duration = 2.f;
            animSetting.m_loop = false;
            animSetMap["Jump"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 7.1f;
            animSetting.m_duration = 0.5f;
            animSetting.m_loop = true;
            animSetMap["Sit"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 7.7f;
            animSetting.m_duration = 0.6f;
            animSetting.m_loop = true;
            animSetMap["LieDown"] = animSetting;
        }
        AnimMesh* animMesh = new AnimMesh("res\\model\\nes\\nes.x", b, rot, 1.f, animSetMap);
        animMesh->SetAnim("Idle");
        m_NPC["�l�X"] = animMesh;
    }
    {
        D3DXVECTOR3 b = D3DXVECTOR3(-285.f, 16.f, 559.f);
        D3DXVECTOR3 rot = D3DXVECTOR3(0, D3DX_PI, 0);
        AnimSetMap animSetMap;
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 0.f;
            animSetting.m_duration = 0.5f;
            animSetting.m_loop = true;
            animSetMap["Idle"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 1.f;
            animSetting.m_duration = 1.f;
            animSetting.m_loop = false;
            animSetMap["Walk"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 2.f;
            animSetting.m_duration = 1.f;
            animSetting.m_loop = false;
            animSetMap["Attack"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 3.f;
            animSetting.m_duration = 0.5f;
            animSetting.m_loop = false;
            animSetMap["Damaged"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 3.5f;
            animSetting.m_duration = 1.f;
            animSetting.m_loop = false;
            animSetMap["Dead"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 5.f;
            animSetting.m_duration = 2.f;
            animSetting.m_loop = false;
            animSetMap["Jump"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 7.1f;
            animSetting.m_duration = 0.5f;
            animSetting.m_loop = true;
            animSetMap["Sit"] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 7.7f;
            animSetting.m_duration = 0.6f;
            animSetting.m_loop = true;
            animSetMap["LieDown"] = animSetting;
        }
        AnimMesh* animMesh = new AnimMesh("res\\model\\noble\\noble.x", b, rot, 1.f, animSetMap);
        animMesh->SetAnim("Idle");
        m_NPC["�m�[�u��"] = animMesh;
    }
}

void Map::Update()
{
    NSStarmanLib::EnemyInfoManager* enemyInfoManager = NSStarmanLib::EnemyInfoManager::GetObj();
    for (auto it = m_vecEnemy.begin(); it != m_vecEnemy.end();)
    {
        (*it)->Update();
        if ((*it)->GetState() == eEnemyState::DISABLE)
        {
            enemyInfoManager->SetDefeat((*it)->GetIdSub());
            (*it)->Finalize();
            it = m_vecEnemy.erase(it);
        }
        else
        {
            it++;
        }
    }

    Player* player = SharedObj::GetPlayer();
    D3DXVECTOR3 pos = player->GetPos();

    if (m_lazyMesh.IsLoadPos(pos))
    {
        if (m_lazyMesh.IsLoaded() == false)
        {
            m_lazyMesh.Load();
        }
    }
    else
    {
        if (m_lazyMesh.IsLoaded())
        {
            m_lazyMesh.Unload();
        }
    }

    // 60��Ɉ��i��1�b���Ɓj�̏���
    {
        static int counter = 0;
        counter++;
        if (counter >= 60)
        {
            counter = 0;
        }
        if (counter == 1)
        {
            //------------------------------------------------------------------------------
            // �G��XX���[�g���ȓ��ɂ�����ǂݍ���ŕ\��
            //------------------------------------------------------------------------------
            std::vector<NSStarmanLib::EnemyInfo> eneList = enemyInfoManager->GetEnemyInfo(pos.x, pos.y, pos.z, 30.f);

            for (int i = 0; i < (int)eneList.size(); ++i)
            {
                int id = eneList.at(i).GetID();
                auto it = std::find_if(m_vecEnemy.begin(), m_vecEnemy.end(),
                                       [&](const EnemyBase* x)
                                       {
                                           return x->GetIdSub() == id;
                                       });
                if (it == m_vecEnemy.end())
                {
                    if (eneList.at(i).GetDefeated() == false)
                    {
                        EnemyBase* enemy = nullptr;
                        if (eneList.at(i).GetBreed() == "���b�|�E�^�C")
                        {
                            enemy = new EnemyCube();
                        }
                        else if (eneList.at(i).GetBreed() == "�L���E")
                        {
                            enemy = new EnemySphere();
                        }
                        else if (eneList.at(i).GetBreed() == "�G���o��")
                        {
                            enemy = new EnemyEnban();
                        }
                        else if (eneList.at(i).GetBreed() == "�G���`���E")
                        {
                            enemy = new EnemyEnchu();
                        }
                        else if (eneList.at(i).GetBreed() == "�r�b�O���b�|�E�^�C")
                        {
                            enemy = new EnemyBigCube();
                        }
                        else if (eneList.at(i).GetBreed() == "�X���[�����b�|�E�^�C")
                        {
                            enemy = new EnemySmallCube();
                        }
                        else if (eneList.at(i).GetBreed() == "�n���G��")
                        {
                            enemy = new EnemyHanen();
                        }
                        else if (eneList.at(i).GetBreed() == "�n���L���E")
                        {
                            enemy = new EnemyHankyuu();
                        }
                        else if (eneList.at(i).GetBreed() == "�I�����W���b�|�E�^�C")
                        {
                            enemy = new EnemyOrangeCube();
                        }
                        else if (eneList.at(i).GetBreed() == "�����̗�")
                        {
                            enemy = new EnemyGhost();
                        }

                        enemy->SetIdSub(eneList.at(i).GetID());
                        D3DXVECTOR3 work;
                        work.x = eneList.at(i).GetX();
                        work.y = eneList.at(i).GetY();
                        work.z = eneList.at(i).GetZ();
                        enemy->SetPos(work);

                        work.x = eneList.at(i).GetRotX();
                        work.y = eneList.at(i).GetRotY();
                        work.z = eneList.at(i).GetRotZ();
                        enemy->SetRotate(work);

                        enemy->SetHP(eneList.at(i).GetHP());

                        m_vecEnemy.push_back(enemy);

                        // Init�֐��͕ʃX���b�h�œǂݍ��݂��s���̂�push_back������ɌĂԁB
                        auto it = m_vecEnemy.rbegin();
                        (*it)->Init();
                    }
                }
            }


            NSStarmanLib::PowereggDateTime* dateTime = NSStarmanLib::PowereggDateTime::GetObj();

            //-------------------------------------
            // ���z�̖��邳
            //-------------------------------------
            // ����12�����ł����邭�A���0�����ł��Â����ƂƂ���
            // TODO �T�C���J�[�u�ɂ����ق����ǂ��B
            float hour = (float)dateTime->GetHour();
            if (hour <= 12)
            {
                Light::SetBrightness(hour / 12);
            }
            else
            {
                Light::SetBrightness((24 - hour) / 12);
            }

            //-------------------------------------
            // �A�̕\��
            //-------------------------------------
            // ���z�͒���12���ɐ^��A���0���ɐ^���A�Ƃ���B
            // 0��0���`23��59����0.0f�`0.99999f�ƂȂ�悤�ɕϊ�
            // 0��0����0���A23��59����1439���ƌ��Ȃ��΂悢

            float hourAndMinute = 0.f;
            hourAndMinute += dateTime->GetHour() * 60;
            hourAndMinute += dateTime->GetMinute();
            hourAndMinute /= 1440;
            D3DXVECTOR4 vec;
            vec.x = std::sin(hourAndMinute * 2 * D3DX_PI);
            vec.y = std::cos(hourAndMinute * 2 * D3DX_PI) * -1;
            vec.z = std::cos(hourAndMinute * 2 * D3DX_PI);
            vec.w = 0.f;
            Light::SetLightNormal(vec);

            //-------------------------------------
            // ���z
            //-------------------------------------
            D3DXVECTOR3 sunPos(vec);
            sunPos *= 6000;
            m_pSun->SetPos(sunPos);

            //-------------------------------------
            // 3D���f���̒x���ǂݍ���
            //-------------------------------------
            {
                auto player = SharedObj::GetPlayer();
                auto mapObjManager = NSStarmanLib::MapObjManager::GetObj();
                std::vector<NSStarmanLib::stMapObj> needShow;
                std::vector<NSStarmanLib::stMapObj> needHide;
                mapObjManager->GetMapObjListShow(player->GetPos().x, player->GetPos().z, &needShow);

                for (int i = 0; i < (int)needShow.size(); ++i)
                {
                    D3DXVECTOR3 pos;
                    pos.x = needShow.at(i).m_x;
                    pos.y = needShow.at(i).m_y;
                    pos.z = needShow.at(i).m_z;

                    D3DXVECTOR3 rot;
                    rot.x = 0.f;
                    rot.y = needShow.at(i).m_yRot;
                    rot.z = 0.f;

                    std::string xName = mapObjManager->GetModelName(needShow.at(i).m_modelId);

                    auto meshClone = new MeshClone(xName, pos, rot, needShow.at(i).m_scale);
                    meshClone->Init();
                    m_meshCloneMap[needShow.at(i).m_id] = meshClone;
                    mapObjManager->SetShow(needShow.at(i).m_frameX,
                                           needShow.at(i).m_frameZ,
                                           needShow.at(i).m_id,
                                           true);
                }

                mapObjManager->GetMapObjListHide(player->GetPos().x, player->GetPos().z, &needHide);
                for (int i = 0; i < (int)needHide.size(); ++i)
                {
                    mapObjManager->SetShow(needHide.at(i).m_frameX,
                                           needHide.at(i).m_frameZ,
                                           needHide.at(i).m_id,
                                           false);
                    m_meshCloneMap.erase(needHide.at(i).m_id);
                }
            }

            // 100���[�g���ȏ㗣�ꂽ�G�͏���
            // ���������Œ��ׂ�Əd���Ȃ�̂ŗ����̂Œ��ׂ�
            // �����Y�������͖�������
            for (auto it = m_vecEnemy.begin(); it != m_vecEnemy.end();)
            {
                if ((*it)->GetPos().x + 100.f < player->GetPos().x ||
                    (*it)->GetPos().x - 100.f > player->GetPos().x)
                {
                    it = m_vecEnemy.erase(it);
                }
                else if ((*it)->GetPos().z + 100.f < player->GetPos().z ||
                         (*it)->GetPos().z - 100.f > player->GetPos().z)
                {
                    it = m_vecEnemy.erase(it);
                }
                else
                {
                    ++it;
                }
            }

            std::string work;
            work = "m_vecEnemy.size(): " + std::to_string(m_vecEnemy.size()) + "\n";
            OutputDebugString(work.c_str());
        }
    }
}

void Map::Render()
{
    {
        D3DXVECTOR4 norm = Light::GetLightNormal();
        D3DXVECTOR4 norm2 { norm };
        // �V���p�ɉ�������𓖂ĂĂ��邱�Ƃɂ���
        // �������Ȃ������B
        norm2.x = 0.f;
        norm2.y = -1.f;
        Light::SetLightNormal(norm2);
        m_meshMap["sky"]->Render();
        //m_meshMap["sky2"]->Render();
        Light::SetLightNormal(norm);
    }
    for (auto& pair : m_meshMap)
    {
        if (pair.first == "sky" || pair.first == "sky2")
        {
            continue;
        }
        pair.second->Render();
    }
    for (auto& pair : m_meshCloneMap)
    {
        pair.second->Render();
    }
    if (m_lazyMesh.IsLoaded())
    {
        //m_lazyMesh.Render();
    }
    for (std::size_t i = 0; i < m_vecEnemy.size(); i++)
    {
        m_vecEnemy.at(i)->Render();
    }

    m_pSun->Render();

    for (auto it = m_NPC.begin(); it != m_NPC.end(); ++it)
    {
        it->second->Render();
    }
}

std::vector<EnemyBase*> Map::GetEnemy()
{
    return m_vecEnemy;
}

void Map::SetEnemy(const std::vector<EnemyBase*>& vecEnemy)
{
    m_vecEnemy = vecEnemy;
}

bool Map::IntersectSub(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, Mesh* mesh)
{
    BOOL  bIsHit = false;
    D3DXVECTOR3 targetPos = pos - mesh->GetPos();

    targetPos /= mesh->GetScale();
    LPD3DXMESH d3dmesh = mesh->GetD3DMesh();
    float fLandDistance;
    DWORD dwHitIndex = -1;
    float fHitU;
    float fHitV;
    D3DXIntersect(d3dmesh, &targetPos, &rot, &bIsHit, &dwHitIndex,
        &fHitU, &fHitV, &fLandDistance, NULL, NULL);
    float judgeDistance = 1.f / mesh->GetScale();
    if (bIsHit && (fLandDistance <= judgeDistance))
    {
        bIsHit = true;
    }
    else
    {
        bIsHit = false;
    }
    return bIsHit;
}

bool Map::IntersectSub(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, MeshClone* mesh)
{
    BOOL  bIsHit = false;
    D3DXVECTOR3 targetPos = pos - mesh->GetPos();

    float radius = mesh->GetRadius() * mesh->GetScale();
    if (targetPos.x >= radius || targetPos.x <= -radius)
    {
        return false;
    }

    if (targetPos.z >= radius || targetPos.z <= -radius)
    {
        return false;
    }

    targetPos /= mesh->GetScale();
    LPD3DXMESH d3dmesh = mesh->GetD3DMesh();
    float fLandDistance;
    DWORD dwHitIndex = -1;
    float fHitU;
    float fHitV;
    D3DXIntersect(d3dmesh, &targetPos, &rot, &bIsHit, &dwHitIndex,
        &fHitU, &fHitV, &fLandDistance, NULL, NULL);

    float judgeDistance = 1.f / mesh->GetScale();
    if (bIsHit && (fLandDistance <= judgeDistance))
    {
        bIsHit = true;
    }
    else
    {
        bIsHit = false;
    }
    return bIsHit;
}

bool Map::Intersect(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
    // �X�e�[�W��̃I�u�W�F�N�g�����_�Ƃ����Ƃ���pos�̈ʒu
    BOOL  bIsHit = false;
    for (auto& pair : m_meshMap)
    {
        bIsHit = IntersectSub(pos, rot, pair.second);
        if (bIsHit)
        {
            break;
        }
    }
    BOOL  bIsHit2 = false;
    for (auto& pair : m_meshCloneMap)
    {
        bIsHit2 = IntersectSub(pos, rot, pair.second);
        if (bIsHit2)
        {
            break;
        }
    }
    return bIsHit || bIsHit2;
}

bool Map::CollisionGround(const D3DXVECTOR3& pos, const D3DXVECTOR3& move)
{
    // �X�e�[�W��̃I�u�W�F�N�g�����_�Ƃ����Ƃ���pos�̈ʒu
    BOOL  bIsHit = false;
    for (auto& pair : m_meshMap)
    {
        bIsHit = IntersectSub(pos, move, pair.second);
        if (bIsHit)
        {
            break;
        }
    }
    return bIsHit;
}

bool Map::CollisionGroundSub(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, Mesh* mesh)
{
    BOOL  bIsHit = false;
    D3DXVECTOR3 targetPos = pos - mesh->GetPos();
    targetPos /= mesh->GetScale();
    LPD3DXMESH d3dmesh = mesh->GetD3DMesh();
    float fLandDistance;
    DWORD dwHitIndex = -1;
    float fHitU;
    float fHitV;
    D3DXIntersect(d3dmesh, &targetPos, &rot, &bIsHit, &dwHitIndex,
        &fHitU, &fHitV, &fLandDistance, NULL, NULL);

    float judgeDistance = 2.f / mesh->GetScale();
    if (bIsHit && fLandDistance <= judgeDistance)
    {
        bIsHit = true;
    }
    else
    {
        bIsHit = false;
    }
    return bIsHit;
}

bool Map::CollisionGroundSub(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, MeshClone* mesh)
{
    BOOL  bIsHit = false;
    D3DXVECTOR3 targetPos = pos - mesh->GetPos();
    targetPos /= mesh->GetScale();
    LPD3DXMESH d3dmesh = mesh->GetD3DMesh();
    float fLandDistance;
    DWORD dwHitIndex = -1;
    float fHitU;
    float fHitV;
    D3DXIntersect(d3dmesh, &targetPos, &rot, &bIsHit, &dwHitIndex,
        &fHitU, &fHitV, &fLandDistance, NULL, NULL);

    float judgeDistance = 2.f / mesh->GetScale();
    if (bIsHit && fLandDistance <= judgeDistance)
    {
        bIsHit = true;
    }
    else
    {
        bIsHit = false;
    }
    return bIsHit;
}

D3DXVECTOR3 Map::WallSlide(const D3DXVECTOR3& pos, const D3DXVECTOR3& move, bool* bHit)
{
    D3DXVECTOR3 result { move };
    for (auto& pair : m_meshMap)
    {
        bool bIsHit = false;
        result = WallSlideSub(pos, pair.second, result, &bIsHit);
        if (bIsHit)
        {
            *bHit = true;
        }
    }

    for (auto& pair : m_meshCloneMap)
    {
        bool bIsHit = false;
        result = WallSlideSub(pos, pair.second, result, &bIsHit);
        if (bIsHit)
        {
            *bHit = true;
            break;
        }
    }

    // �ǂ��肵����ɕǂ��������Ƃ��̏����B
    // �ǂ����ɕǂ�����Ȃ�ړ��ʂ�0�ɂ���
    if (Intersect(pos, result))
    {
        return D3DXVECTOR3{ 0.f, 0.f, 0.f };
    }

    // �ڒn����
    // TODO �������ԈႢ�B�W�����v���t���O��ǉ����邱�ƁB
    // (move.y <= -0.008f �������̍���
    // move.y��-0.01�ȉ��ŁAresult.y��-0.001�ȏゾ������
    // �������x��10����1�ȉ��ɂȂ����Ƃ������ƂȂ̂ŕ���Ȓn�ʂɑ������Ă���Ƃ���B
    // move.y��0�ɂ���
    if (bHit && (- 0.001f <= result.y && result.y <= 0.f))
    {
        result.y = 0.f;
    }
    else
    {
        if (result.y <= 0.f)
        {
            int i = 0;
        }
    }

    // ���C
    // result.x��result.z���������Ȃ�0�ɂ���
    if (-0.005f <= result.x && result.x <= 0.005f)
    {
        if (-0.005f <= result.z && result.z <= 0.005f)
        {
            result.x = 0.f;
            result.z = 0.f;
        }
    }

    return result;
}

D3DXVECTOR3 Map::WallSlideSub(
    const D3DXVECTOR3& pos, Mesh* mesh, const D3DXVECTOR3& move, bool* bHit)
{
    D3DXVECTOR3 result {move};
    D3DXVECTOR3 targetPos = pos - mesh->GetPos();
    targetPos /= mesh->GetScale();
    LPD3DXMESH d3dmesh = mesh->GetD3DMesh();
    float fLandDistance;
    DWORD dwHitIndex = -1;
    BOOL  bIsHit = false;
    float fHitU;
    float fHitV;
    D3DXVECTOR3 rot2 { 0.f, 0.2f, 0.f };
    D3DXIntersect(d3dmesh, &targetPos, &move, &bIsHit, &dwHitIndex,
        &fHitU, &fHitV, &fLandDistance, NULL, NULL);
    float judgeDistance = 2.f / mesh->GetScale();
    if (bIsHit && fLandDistance <= judgeDistance)
    {
        *bHit = true;

        // ----- �L����Y���W�␳ -----
        // ���������C���f�b�N�X�o�b�t�@�擾
        WORD dwHitVertexNo[3] = {};
        WORD* pIndex = nullptr;
        HRESULT hr = d3dmesh->LockIndexBuffer(0, (void**)&pIndex);

        for (int nIdxIdx = 0; nIdxIdx < 3; nIdxIdx++)
        {
            dwHitVertexNo[nIdxIdx] = pIndex[dwHitIndex * 3 + nIdxIdx];
        }

        d3dmesh->UnlockIndexBuffer();

        // ���������|���S���擾
        struct VERTEX
        {
            FLOAT x, y, z; // ���_�̍��W
            FLOAT normX, normY, normZ; // �@���̍��W
            FLOAT u, v;   // ���_�̐F
        };
        VERTEX* pVertex = nullptr;
        hr = d3dmesh->LockVertexBuffer(0, (void**)&pVertex);

        // �n�ʂ̍����ɍ��킹��
        D3DXVECTOR3 p1 { pVertex[dwHitVertexNo[0]].x, pVertex[dwHitVertexNo[0]].y, pVertex[dwHitVertexNo[0]].z };
        D3DXVECTOR3 p2 { pVertex[dwHitVertexNo[1]].x, pVertex[dwHitVertexNo[1]].y, pVertex[dwHitVertexNo[1]].z };
        D3DXVECTOR3 p3 { pVertex[dwHitVertexNo[2]].x, pVertex[dwHitVertexNo[2]].y, pVertex[dwHitVertexNo[2]].z };

        D3DXVECTOR3 v1 = p2 - p1;
        D3DXVECTOR3 v2 = p3 - p1;

        D3DXVECTOR3 normal;

        D3DXVec3Cross(&normal, &v1, &v2);
        D3DXVECTOR3 normal_n;
        D3DXVec3Normalize(&normal_n, &normal);

        D3DXVECTOR3 front;
        front = move;

        result = (front - D3DXVec3Dot(&front, &normal_n) * normal_n);

        d3dmesh->UnlockVertexBuffer();
    }
    else
    {
        result = move;
    }
    return result;
}

D3DXVECTOR3 Map::WallSlideSub(const D3DXVECTOR3& pos, MeshClone* mesh, const D3DXVECTOR3& move, bool* bHit)
{
    D3DXVECTOR3 result {move};
    D3DXVECTOR3 targetPos = pos - mesh->GetPos();

    float radius = mesh->GetRadius() * mesh->GetScale();
    if (targetPos.x >= radius || targetPos.x <= -radius)
    {
        return result;
    }

    if (targetPos.z >= radius || targetPos.z <= -radius)
    {
        return result;
    }

    targetPos /= mesh->GetScale();
    LPD3DXMESH d3dmesh = mesh->GetD3DMesh();
    float fLandDistance;
    DWORD dwHitIndex = -1;
    BOOL  bIsHit = false;
    float fHitU;
    float fHitV;
    D3DXVECTOR3 rot2 { 0.f, 0.2f, 0.f };
    D3DXIntersect(d3dmesh, &targetPos, &move, &bIsHit, &dwHitIndex,
        &fHitU, &fHitV, &fLandDistance, NULL, NULL);
    float judgeDistance = 2.f / mesh->GetScale();
    if (bIsHit && fLandDistance <= judgeDistance)
    {
        *bHit = true;

        // ----- �L����Y���W�␳ -----
        // ���������C���f�b�N�X�o�b�t�@�擾
        WORD dwHitVertexNo[3] = {};
        WORD* pIndex = nullptr;
        HRESULT hr = d3dmesh->LockIndexBuffer(0, (void**)&pIndex);

        for (int nIdxIdx = 0; nIdxIdx < 3; nIdxIdx++)
        {
            dwHitVertexNo[nIdxIdx] = pIndex[dwHitIndex * 3 + nIdxIdx];
        }

        d3dmesh->UnlockIndexBuffer();

        // ���������|���S���擾
        struct VERTEX
        {
            FLOAT x, y, z; // ���_�̍��W
            FLOAT normX, normY, normZ; // �@���̍��W
            FLOAT u, v;   // ���_�̐F
        };
        VERTEX* pVertex = nullptr;
        hr = d3dmesh->LockVertexBuffer(0, (void**)&pVertex);

        // �n�ʂ̍����ɍ��킹��
        D3DXVECTOR3 p1 { pVertex[dwHitVertexNo[0]].x, pVertex[dwHitVertexNo[0]].y, pVertex[dwHitVertexNo[0]].z };
        D3DXVECTOR3 p2 { pVertex[dwHitVertexNo[1]].x, pVertex[dwHitVertexNo[1]].y, pVertex[dwHitVertexNo[1]].z };
        D3DXVECTOR3 p3 { pVertex[dwHitVertexNo[2]].x, pVertex[dwHitVertexNo[2]].y, pVertex[dwHitVertexNo[2]].z };

        D3DXVECTOR3 v1 = p2 - p1;
        D3DXVECTOR3 v2 = p3 - p1;

        D3DXVECTOR3 normal;

        D3DXVec3Cross(&normal, &v1, &v2);
        D3DXVECTOR3 normal_n;
        D3DXVec3Normalize(&normal_n, &normal);

        D3DXVECTOR3 front;
        front = move;

        result = (front - D3DXVec3Dot(&front, &normal_n) * normal_n);

        d3dmesh->UnlockVertexBuffer();
    }
    else
    {
        result = move;
    }
    return result;
}

void LazyMesh::Init(const std::string& xFilename,
                    const D3DXVECTOR3& position,
                    const D3DXVECTOR3& rotation)
{
    m_xFilename = xFilename;
    m_drawPos = position;
    m_rotation = rotation;
}

void LazyMesh::Load()
{
    m_bLoaded = true;
    m_Mesh = new Mesh(m_xFilename, m_drawPos, m_rotation, 1.0f);
    m_thread = new std::thread([&]{ m_Mesh->Init(); });
}

void LazyMesh::Unload()
{
    m_bLoaded = false;
    delete m_Mesh;
    m_Mesh = nullptr;
}

void LazyMesh::SetLoadPos(const D3DXVECTOR3& pos, const float r)
{
    m_loadingPos = pos;
    m_radius = r;
}

bool LazyMesh::IsLoadPos(const D3DXVECTOR3& pos)
{
    D3DXVECTOR3 diff = m_loadingPos - pos;
    float distance = D3DXVec3Length(&diff);

    return m_radius >= distance;
}

bool LazyMesh::IsLoaded() const
{
    return m_bLoaded;
}

void LazyMesh::Render()
{
    if (m_bLoaded)
    {
        m_Mesh->Render();
    }
}
