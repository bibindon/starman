#include "Map.h"
#include "Light.h"
#include "SharedObj.h"
#include "KeyBoard.h"
#include "EnemyCube.h"
#include "EnemyDisk.h"
#include "EnemySphere.h"
#include "EnemyBigCube.h"
#include "EnemyDisk.h"
#include "EnemyEnchu.h"
#include "EnemyGhost.h"
#include "EnemyHanen.h"
#include "EnemyHankyuu.h"
#include "EnemyOrangeCube.h"
#include "EnemySmallCube.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/EnemyInfoManager.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/PowereggDateTime.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/MapObjManager.h"
#include "NpcManager.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/WeaponManager.h"
#include "SoundEffect.h"
#include "Camera.h"
#include "BGM.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/Storehouse.h"
#include "Rain.h"
#include "resource.h"

Map::Map()
{
}

Map::~Map()
{
    for (auto& pair : m_meshMap)
    {
        SAFE_DELETE(pair.second);
    }

    for (auto& item : m_vecEnemy)
    {
        SAFE_DELETE(item);
    }

    SAFE_DELETE(m_pSun);

    for (auto& item : m_NPC)
    {
        SAFE_DELETE(item.second);
    }

    for (auto& item : m_meshCloneMap)
    {
        SAFE_DELETE(item.second);
    }
}

void Map::Init()
{
    m_progress.store(0);

    {
        Mesh* mesh = NEW Mesh(_T("res\\model\\prolitan.x"),
                              D3DXVECTOR3(0.f, 0.f, 0.f),
                              D3DXVECTOR3(0.f, 0.f, 0.f),
                              1.0f);
        mesh->Init();
        m_meshMap[_T("Map")] = mesh;
    }
    {
        Mesh* mesh = NEW Mesh(_T("res\\model\\river.x"),
                              D3DXVECTOR3(0.f, 0.f, 0.f),
                              D3DXVECTOR3(0.f, 0.f, 0.f),
                              1.0f);
        mesh->Init();
        m_meshMap[_T("river")] = mesh;
    }
    {
        Mesh* mesh = NEW Mesh(_T("res\\model\\sea.x"),
                              D3DXVECTOR3(0.f, -90.f, 0.f),
                              D3DXVECTOR3(0.f, 0.f, 0.f),
                              10.0f);
        mesh->Init();
        m_meshMap[_T("sea")] = mesh;
    }

    // 主人公が島から大きく離れた場所にいるとき、タイトル画面で海が表示されなくなる。
    // その対策として海の下に、海を表示しておく
    {
        Mesh* mesh = NEW Mesh(_T("res\\model\\sea.x"),
                              D3DXVECTOR3(0.f, -110.f, 0.f),
                              D3DXVECTOR3(0.f, 0.f, 0.f),
                              10.0f);
        mesh->Init();
        m_meshMap[_T("sea2")] = mesh;
    }

//    {
//        Mesh* mesh = NEW Mesh(_T("res\\model\\testcube\\testcube.x"),
//                              D3DXVECTOR3(-290.f, 15.f, 540.f),
//                              D3DXVECTOR3(0.f, 0.f, 0.f),
//                              1.0f);
//        mesh->Init();
//        m_meshMap[_T("testcube")] = mesh;
//    }
    //{
    //    Mesh* mesh = NEW Mesh(_T("res\\model\\cube6\\cube6.x"),
    //                          D3DXVECTOR3(-10.f, 0.f, 0.f),
    //                          D3DXVECTOR3(0.f, 0.f, 0.f),
    //                          1.0f);
    //    mesh->Init();
    //    m_meshMap[_T("cube61")] = mesh;

    //    mesh = NEW Mesh(_T("res\\model\\cube6\\cube6.x"),
    //                    D3DXVECTOR3(-13.f, 1.f, 0.f),
    //                    D3DXVECTOR3(0.f, 0.f, 0.f),
    //                    1.0f);
    //    mesh->Init();
    //    m_meshMap[_T("cube62")] = mesh;

    //    mesh = NEW Mesh(_T("res\\model\\cube6\\cube6.x"),
    //                    D3DXVECTOR3(-16.f, 2.f, 0.f),
    //                    D3DXVECTOR3(0.f, 0.f, 0.f),
    //                    1.0f);
    //    mesh->Init();
    //    m_meshMap[_T("cube63")] = mesh;

    //    mesh = NEW Mesh(_T("res\\model\\collisionTest\\colli.x"),
    //                    D3DXVECTOR3(0.f, 0.f, -20.f),
    //                    D3DXVECTOR3(0.f, 0.f, 0.f),
    //                    1.0f);
    //    mesh->Init();
    //    m_meshMap[_T("colli")] = mesh;
    //}

    {
        Mesh* mesh { nullptr };
        D3DXVECTOR3 pos = D3DXVECTOR3(0.f, 0.f, 0.f);
        D3DXVECTOR3 rot = D3DXVECTOR3(0.f, 0.f, 0.f);
        mesh = NEW Mesh(_T("res\\model\\hemisphere.x"), pos, rot, 180000.0f);
        mesh->Init();
        m_meshMap[_T("sky")] = mesh;
    }
    {
        D3DXVECTOR3 pos = D3DXVECTOR3(11.f, 491.5f, -572.f);
        D3DXVECTOR3 rot = D3DXVECTOR3(0.f, 0.f, 0.f);
        Mesh* mesh = NEW Mesh(_T("res\\shader\\mesh_shader_cull_none.fx"),
                              _T("res\\model\\precision.x"),
                              pos,
                              rot,
                              1.3f);
        mesh->Init();
        m_meshMap[_T("precision")] = mesh;
    }
    {
        D3DXVECTOR3 pos = D3DXVECTOR3(15.84f, 491.5f, -570.95f);
        D3DXVECTOR3 rot = D3DXVECTOR3(0.f, D3DX_PI, 0.f);
        Mesh* mesh = NEW Mesh(_T("res\\shader\\mesh_shader.fx"),
                              _T("res\\model\\chest.x"),
                              pos,
                              rot,
                              1.0f);
        mesh->Init();
        m_meshMap[_T("chest")] = mesh;
    }
    //{
    //    D3DXVECTOR3 b = D3DXVECTOR3(-285.f, 20.f, 520.f);
    //    D3DXVECTOR3 c = D3DXVECTOR3(0.f, 0.f, 0.f);
    //    Mesh* mesh = NEW Mesh(_T("res\\model\\rock1\\rock1.x"), b, c, 1.f);
    //    mesh->Init();
    //    m_meshMap[_T("rock1")] = mesh;
    //}

    // 灯台
    {
        D3DXVECTOR3 pos = D3DXVECTOR3(-1361.f, 625.f, -1521.f);
        D3DXVECTOR3 rot = D3DXVECTOR3(0.f, 0.f, 0.f);
        Mesh* mesh = NEW Mesh(_T("res\\model\\lighthouse.blend.x"), pos, rot, 1.0f);
        mesh->Init();
        m_meshMap[_T("lighthouse")] = mesh;
    }

    // 神社
    {
        D3DXVECTOR3 pos = D3DXVECTOR3(0.8f, 492.8f, -1221.2f);
        D3DXVECTOR3 rot = D3DXVECTOR3(0.f, 0.f, 0.f);
        Mesh* mesh = NEW Mesh(_T("res\\model\\shrine.blend.x"), pos, rot, 1.f);
        mesh->Init();
        m_meshMap[_T("shrine")] = mesh;
    }

    // 洞窟
    {
        D3DXVECTOR3 pos = D3DXVECTOR3(115.6f, 590.5f, -1484.2f);
        D3DXVECTOR3 rot = D3DXVECTOR3(0.f, 0.f, 0.f);
        Mesh* mesh = NEW Mesh(_T("res\\model\\cottage.x"), pos, rot, 1.f);
        mesh->Init();
        m_meshMap[_T("cottage3")] = mesh;
    }

    // 苔庭
    {
        D3DXVECTOR3 pos = D3DXVECTOR3(628.9f, 773.9f, -1861.8f);
        D3DXVECTOR3 rot = D3DXVECTOR3(0.f, 0.f, 0.f);
        Mesh* mesh = NEW Mesh(_T("res\\model\\cottage.x"), pos, rot, 1.f);
        mesh->Init();
        m_meshMap[_T("cottage4")] = mesh;
    }

    // 港跡
    {
        D3DXVECTOR3 pos = D3DXVECTOR3(1814.3f, 28.5f, -899.2f);
        D3DXVECTOR3 rot = D3DXVECTOR3(0.f, 0.f, 0.f);
        Mesh* mesh = NEW Mesh(_T("res\\model\\cottage.x"), pos, rot, 1.f);
        mesh->Init();
        m_meshMap[_T("cottage5")] = mesh;
    }

    // 海岸洞窟
    {
        D3DXVECTOR3 pos = D3DXVECTOR3(1092.f, 40.8f, 504.1f);
        D3DXVECTOR3 rot = D3DXVECTOR3(0.f, 0.f, 0.f);
        Mesh* mesh = NEW Mesh(_T("res\\model\\cottage.x"), pos, rot, 1.f);
        mesh->Init();
        m_meshMap[_T("cottage6")] = mesh;
    }

    // ソテツの場所
    {
        D3DXVECTOR3 pos = D3DXVECTOR3(1092.f, 40.8f, 504.1f);
        D3DXVECTOR3 rot = D3DXVECTOR3(0.f, 0.f, 0.f);
        Mesh* mesh = NEW Mesh(_T("res\\model\\cottage.x"), pos, rot, 1.f);
        mesh->Init();
        m_meshMap[_T("cottage7")] = mesh;
    }

    // 大陸
    {
        D3DXVECTOR3 pos = D3DXVECTOR3(-2000000.f, 0.f, 0.f);
        D3DXVECTOR3 rot = D3DXVECTOR3(0.f, 0.f, 0.f);
        Mesh* mesh = NEW Mesh(_T("res\\model\\continent.x"),
                              pos,
                              rot,
                              1000000.f);

        mesh->Init();
        m_meshMap[_T("continent")] = mesh;
    }

    //------------------------------------------
    // ビムが登場するときの岩
    //------------------------------------------
    {
        D3DXVECTOR3 pos = D3DXVECTOR3(-1425.4f, 20.1f, -598.f);
        D3DXVECTOR3 rot = D3DXVECTOR3(0.f, 0.f, 0.f);
        Mesh* mesh = NEW Mesh(_T("res\\model\\rock2.x"), pos, rot, 10.f);
        mesh->Init();
        m_meshMap[_T("rock2")] = mesh;
    }

    //--------------------------------------------
    // 太陽
    //--------------------------------------------
    m_pSun = NEW MeshNoShade(_T("res\\model\\sun.x"),
                             D3DXVECTOR3(0.f, -10000.f, 0.f),
                             D3DXVECTOR3(0.f, 0.f, 0.f),
                             15000.0f);
    m_pSun->Init();

    m_progress.store(20);

    //--------------------------------------------
    // NPC
    //--------------------------------------------
    auto npcManager = NpcManager::Get();
    {
        auto status = npcManager->GetNpcStatus(_T("daikeiman"));
        D3DXVECTOR3 pos = D3DXVECTOR3(status.GetX(), status.GetY(), status.GetZ());
        D3DXVECTOR3 rot = D3DXVECTOR3(0, status.GetRotY(), 0);
        AnimSetMap animSetMap;
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 0.f;
            animSetting.m_duration = 0.5f;
            animSetting.m_loop = true;
            animSetMap[_T("0_Idle")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 1.f;
            animSetting.m_duration = 1.f;
            animSetting.m_loop = false;
            animSetMap[_T("Walk")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 2.f;
            animSetting.m_duration = 1.f;
            animSetting.m_loop = false;
            animSetMap[_T("Attack")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 3.f;
            animSetting.m_duration = 0.5f;
            animSetting.m_loop = false;
            animSetMap[_T("Damaged")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 3.5f;
            animSetting.m_duration = 1.f;
            animSetting.m_loop = false;
            animSetMap[_T("Dead")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 5.f;
            animSetting.m_duration = 2.f;
            animSetting.m_loop = false;
            animSetMap[_T("Jump")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 7.1f;
            animSetting.m_duration = 0.5f;
            animSetting.m_loop = true;
            animSetMap[_T("Sit")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 7.7f;
            animSetting.m_duration = 0.6f;
            animSetting.m_loop = true;
            animSetMap[_T("LieDown")] = animSetting;
        }
        AnimMesh* daikeiman = NEW AnimMesh(_T("res\\model\\daikeiman.x"),
                                           pos,
                                           rot,
                                           1.f,
                                           animSetMap);
        daikeiman->SetAnim(_T("LieDown"));
        m_NPC[_T("daikeiman")] = daikeiman;
    }

    m_progress.store(40);

    {
        auto status = npcManager->GetNpcStatus(_T("sankakuman"));
        D3DXVECTOR3 pos = D3DXVECTOR3(status.GetX(), status.GetY(), status.GetZ());
        D3DXVECTOR3 rot = D3DXVECTOR3(0, status.GetRotY(), 0);
        AnimSetMap animSetMap;
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 0.f;
            animSetting.m_duration = 0.5f;
            animSetting.m_loop = true;
            animSetMap[_T("0_Idle")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 1.f;
            animSetting.m_duration = 1.f;
            animSetting.m_loop = false;
            animSetMap[_T("Walk")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 2.f;
            animSetting.m_duration = 1.f;
            animSetting.m_loop = false;
            animSetMap[_T("Attack")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 3.f;
            animSetting.m_duration = 0.5f;
            animSetting.m_loop = false;
            animSetMap[_T("Damaged")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 3.5f;
            animSetting.m_duration = 1.f;
            animSetting.m_loop = false;
            animSetMap[_T("Dead")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 5.f;
            animSetting.m_duration = 2.f;
            animSetting.m_loop = false;
            animSetMap[_T("Jump")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 7.1f;
            animSetting.m_duration = 0.5f;
            animSetting.m_loop = true;
            animSetMap[_T("Sit")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 7.7f;
            animSetting.m_duration = 0.6f;
            animSetting.m_loop = true;
            animSetMap[_T("LieDown")] = animSetting;
        }
        AnimMesh* sankakuman = NEW AnimMesh(_T("res\\model\\sankakuman.x"), pos, rot, 0.66f, animSetMap);
        sankakuman->SetAnim(_T("Sit"));
        m_NPC[_T("sankakuman")] = sankakuman;
    }
    {
        auto status = npcManager->GetNpcStatus(_T("shikakuman"));
        D3DXVECTOR3 pos = D3DXVECTOR3(status.GetX(), status.GetY(), status.GetZ());
        D3DXVECTOR3 rot = D3DXVECTOR3(0, status.GetRotY(), 0);
        AnimSetMap animSetMap;
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 0.f;
            animSetting.m_duration = 0.5f;
            animSetting.m_loop = true;
            animSetMap[_T("0_Idle")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 1.f;
            animSetting.m_duration = 1.f;
            animSetting.m_loop = false;
            animSetMap[_T("Walk")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 2.f;
            animSetting.m_duration = 1.f;
            animSetting.m_loop = false;
            animSetMap[_T("Attack")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 3.f;
            animSetting.m_duration = 0.5f;
            animSetting.m_loop = false;
            animSetMap[_T("Damaged")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 3.5f;
            animSetting.m_duration = 1.f;
            animSetting.m_loop = false;
            animSetMap[_T("Dead")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 5.f;
            animSetting.m_duration = 2.f;
            animSetting.m_loop = false;
            animSetMap[_T("Jump")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 7.1f;
            animSetting.m_duration = 0.5f;
            animSetting.m_loop = true;
            animSetMap[_T("Sit")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 7.7f;
            animSetting.m_duration = 0.6f;
            animSetting.m_loop = true;
            animSetMap[_T("LieDown")] = animSetting;
        }
        AnimMesh* shikakuman = NEW AnimMesh(_T("res\\model\\shikakuman.x"),
                                            pos,
                                            rot,
                                            0.66f,
                                            animSetMap);
        shikakuman->SetAnim(_T("Sit"));
        m_NPC[_T("shikakuman")] = shikakuman;
    }

    m_progress.store(60);

    {
        auto status = npcManager->GetNpcStatus(_T("vim"));
        D3DXVECTOR3 pos = D3DXVECTOR3(status.GetX(), status.GetY(), status.GetZ());
        D3DXVECTOR3 rot = D3DXVECTOR3(0, status.GetRotY(), 0);
        AnimSetMap animSetMap;
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 0.f;
            animSetting.m_duration = 0.5f;
            animSetting.m_loop = true;
            animSetMap[_T("0_Idle")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 1.f;
            animSetting.m_duration = 1.f;
            animSetting.m_loop = false;
            animSetMap[_T("Walk")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 2.f;
            animSetting.m_duration = 1.f;
            animSetting.m_loop = false;
            animSetMap[_T("Attack")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 3.f;
            animSetting.m_duration = 0.5f;
            animSetting.m_loop = false;
            animSetMap[_T("Damaged")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 3.5f;
            animSetting.m_duration = 1.f;
            animSetting.m_loop = false;
            animSetMap[_T("Dead")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 5.f;
            animSetting.m_duration = 2.f;
            animSetting.m_loop = false;
            animSetMap[_T("Jump")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 7.1f;
            animSetting.m_duration = 0.5f;
            animSetting.m_loop = true;
            animSetMap[_T("Sit")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 7.7f;
            animSetting.m_duration = 0.6f;
            animSetting.m_loop = true;
            animSetMap[_T("LieDown")] = animSetting;
        }
        AnimMesh* animMesh = NEW AnimMesh(_T("res\\model\\vim.x"), pos, rot, 1.f, animSetMap);
        animMesh->SetAnim(_T("0_Idle"));
        m_NPC[_T("vim")] = animMesh;
    }

    m_progress.store(80);

    /*
    {
        D3DXVECTOR3 b = D3DXVECTOR3(-285.f, 16.f, 549.f);
        D3DXVECTOR3 move = D3DXVECTOR3(0, D3DX_PI, 0);
        AnimSetMap animSetMap;
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 0.f;
            animSetting.m_duration = 0.5f;
            animSetting.m_loop = true;
            animSetMap[_T("0_Idle")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 1.f;
            animSetting.m_duration = 1.f;
            animSetting.m_loop = false;
            animSetMap[_T("Walk")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 2.f;
            animSetting.m_duration = 1.f;
            animSetting.m_loop = false;
            animSetMap[_T("Attack")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 3.f;
            animSetting.m_duration = 0.5f;
            animSetting.m_loop = false;
            animSetMap[_T("Damaged")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 3.5f;
            animSetting.m_duration = 1.f;
            animSetting.m_loop = false;
            animSetMap[_T("Dead")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 5.f;
            animSetting.m_duration = 2.f;
            animSetting.m_loop = false;
            animSetMap[_T("Jump")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 7.1f;
            animSetting.m_duration = 0.5f;
            animSetting.m_loop = true;
            animSetMap[_T("Sit")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 7.7f;
            animSetting.m_duration = 0.6f;
            animSetting.m_loop = true;
            animSetMap[_T("LieDown")] = animSetting;
        }
        AnimMesh* animMesh = NEW AnimMesh(_T("res\\model\\vsco\\vsco.x"), b, move, 1.f, animSetMap);
        animMesh->SetAnim(_T("0_Idle"));
        m_NPC[_T("ブイエスコ")] = animMesh;
    }
    {
        D3DXVECTOR3 b = D3DXVECTOR3(-285.f, 16.f, 551.f);
        D3DXVECTOR3 move = D3DXVECTOR3(0, D3DX_PI, 0);
        AnimSetMap animSetMap;
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 0.f;
            animSetting.m_duration = 0.5f;
            animSetting.m_loop = true;
            animSetMap[_T("0_Idle")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 1.f;
            animSetting.m_duration = 1.f;
            animSetting.m_loop = false;
            animSetMap[_T("Walk")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 2.f;
            animSetting.m_duration = 1.f;
            animSetting.m_loop = false;
            animSetMap[_T("Attack")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 3.f;
            animSetting.m_duration = 0.5f;
            animSetting.m_loop = false;
            animSetMap[_T("Damaged")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 3.5f;
            animSetting.m_duration = 1.f;
            animSetting.m_loop = false;
            animSetMap[_T("Dead")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 5.f;
            animSetting.m_duration = 2.f;
            animSetting.m_loop = false;
            animSetMap[_T("Jump")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 7.1f;
            animSetting.m_duration = 0.5f;
            animSetting.m_loop = true;
            animSetMap[_T("Sit")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 7.7f;
            animSetting.m_duration = 0.6f;
            animSetting.m_loop = true;
            animSetMap[_T("LieDown")] = animSetting;
        }
        AnimMesh* animMesh = NEW AnimMesh(_T("res\\model\\webnin\\webnin.x"), b, move, 1.f, animSetMap);
        animMesh->SetAnim(_T("0_Idle"));
        m_NPC[_T("ウェブニン")] = animMesh;
    }
    {
        D3DXVECTOR3 b = D3DXVECTOR3(-285.f, 16.f, 533.f);
        D3DXVECTOR3 move = D3DXVECTOR3(0, D3DX_PI, 0);
        AnimSetMap animSetMap;
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 0.f;
            animSetting.m_duration = 2.0f;
            animSetting.m_loop = true;
            animSetMap[_T("0_Idle")] = animSetting;
        }
        //AnimMesh* animMesh = NEW AnimMesh(_T("res\\model\\xeoff\\xeoff.x"), b, move, 1.5f, animSetMap);
        //animMesh->SetAnim(_T("0_Idle"));
        //m_NPC[_T("ジーオフ")] = animMesh;
    }
    {
        D3DXVECTOR3 b = D3DXVECTOR3(-285.f, 16.f, 555.f);
        D3DXVECTOR3 move = D3DXVECTOR3(0, D3DX_PI, 0);
        AnimSetMap animSetMap;
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 0.f;
            animSetting.m_duration = 0.5f;
            animSetting.m_loop = true;
            animSetMap[_T("0_Idle")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 1.f;
            animSetting.m_duration = 1.f;
            animSetting.m_loop = false;
            animSetMap[_T("Walk")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 2.f;
            animSetting.m_duration = 1.f;
            animSetting.m_loop = false;
            animSetMap[_T("Attack")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 3.f;
            animSetting.m_duration = 0.5f;
            animSetting.m_loop = false;
            animSetMap[_T("Damaged")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 3.5f;
            animSetting.m_duration = 1.f;
            animSetting.m_loop = false;
            animSetMap[_T("Dead")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 5.f;
            animSetting.m_duration = 2.f;
            animSetting.m_loop = false;
            animSetMap[_T("Jump")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 7.1f;
            animSetting.m_duration = 0.5f;
            animSetting.m_loop = true;
            animSetMap[_T("Sit")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 7.7f;
            animSetting.m_duration = 0.6f;
            animSetting.m_loop = true;
            animSetMap[_T("LieDown")] = animSetting;
        }
        AnimMesh* animMesh = NEW AnimMesh(_T("res\\model\\gain\\gain.x"), b, move, 1.f, animSetMap);
        animMesh->SetAnim(_T("0_Idle"));
        m_NPC[_T("ガイン")] = animMesh;
    }
    {
        D3DXVECTOR3 b = D3DXVECTOR3(-285.f, 16.f, 557.f);
        D3DXVECTOR3 move = D3DXVECTOR3(0, D3DX_PI, 0);
        AnimSetMap animSetMap;
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 0.f;
            animSetting.m_duration = 0.5f;
            animSetting.m_loop = true;
            animSetMap[_T("0_Idle")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 1.f;
            animSetting.m_duration = 1.f;
            animSetting.m_loop = false;
            animSetMap[_T("Walk")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 2.f;
            animSetting.m_duration = 1.f;
            animSetting.m_loop = false;
            animSetMap[_T("Attack")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 3.f;
            animSetting.m_duration = 0.5f;
            animSetting.m_loop = false;
            animSetMap[_T("Damaged")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 3.5f;
            animSetting.m_duration = 1.f;
            animSetting.m_loop = false;
            animSetMap[_T("Dead")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 5.f;
            animSetting.m_duration = 2.f;
            animSetting.m_loop = false;
            animSetMap[_T("Jump")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 7.1f;
            animSetting.m_duration = 0.5f;
            animSetting.m_loop = true;
            animSetMap[_T("Sit")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 7.7f;
            animSetting.m_duration = 0.6f;
            animSetting.m_loop = true;
            animSetMap[_T("LieDown")] = animSetting;
        }
        AnimMesh* animMesh = NEW AnimMesh(_T("res\\model\\nes\\nes.x"), b, move, 1.f, animSetMap);
        animMesh->SetAnim(_T("0_Idle"));
        m_NPC[_T("ネス")] = animMesh;
    }
    {
        D3DXVECTOR3 b = D3DXVECTOR3(-285.f, 16.f, 559.f);
        D3DXVECTOR3 move = D3DXVECTOR3(0, D3DX_PI, 0);
        AnimSetMap animSetMap;
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 0.f;
            animSetting.m_duration = 0.5f;
            animSetting.m_loop = true;
            animSetMap[_T("0_Idle")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 1.f;
            animSetting.m_duration = 1.f;
            animSetting.m_loop = false;
            animSetMap[_T("Walk")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 2.f;
            animSetting.m_duration = 1.f;
            animSetting.m_loop = false;
            animSetMap[_T("Attack")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 3.f;
            animSetting.m_duration = 0.5f;
            animSetting.m_loop = false;
            animSetMap[_T("Damaged")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 3.5f;
            animSetting.m_duration = 1.f;
            animSetting.m_loop = false;
            animSetMap[_T("Dead")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 5.f;
            animSetting.m_duration = 2.f;
            animSetting.m_loop = false;
            animSetMap[_T("Jump")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 7.1f;
            animSetting.m_duration = 0.5f;
            animSetting.m_loop = true;
            animSetMap[_T("Sit")] = animSetting;
        }
        {
            AnimSetting animSetting { };
            animSetting.m_startPos = 7.7f;
            animSetting.m_duration = 0.6f;
            animSetting.m_loop = true;
            animSetMap[_T("LieDown")] = animSetting;
        }
        AnimMesh* animMesh = NEW AnimMesh(_T("res\\model\\noble\\noble.x"), b, move, 1.f, animSetMap);
        animMesh->SetAnim(_T("0_Idle"));
        m_NPC[_T("ノーブル")] = animMesh;
    }
    */
    SoundEffect::get_ton()->load(_T("res\\sound\\fireHit.wav"));
    SoundEffect::get_ton()->load(_T("res\\sound\\iceHit.wav"));
    SoundEffect::get_ton()->load(_T("res\\sound\\darkHit.wav"));

    MapLib()->ResetShow();

    m_progress.store(100);
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
            SAFE_DELETE(*it);
            it = m_vecEnemy.erase(it);
        }
        else
        {
            it++;
        }
    }

    Player* player = SharedObj::GetPlayer();
    D3DXVECTOR3 pos = player->GetPos();

    {
        //-------------------------------------
        // 太陽の明るさ
        //-------------------------------------
        // 昼の12時が最も明るく、夜の0時が最も暗いこととする
        auto dateTime = NSStarmanLib::PowereggDateTime::GetObj();
        float hour = (float)dateTime->GetHour();
        float work1 = 0.f;
        if (hour <= 12)
        {
            work1 = hour / 12;
        }
        else
        {
            work1 = (24 - hour) / 12;
        }

        // プレイヤーが洞窟内にいたら明るさ0
        if (SharedObj::GetPlayer()->IsInCave())
        {
            // 3秒くらいかけて暗くする
            auto brightness = Light::GetBrightness();
            work1 /= (60 * 3);
            brightness -= work1;
            if (brightness < 0.f)
            {
                brightness = 0.f;
            }

            Light::SetBrightness(brightness);
        }
        else
        {
            // 3秒くらいかけて明るくする
            auto brightness = Light::GetBrightness();
            work1 /= (60 * 3);
            brightness += work1;
            if (brightness > work1)
            {
                brightness = work1;
            }

            Light::SetBrightness(brightness);
        }
    }

    // 60回に一回（＝1秒ごと）の処理
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
            // 敵がXXメートル以内にいたら読み込んで表示
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
                        if (eneList.at(i).GetBreed() == _T("リッポウタイ"))
                        {
                            enemy = NEW EnemyCube();
                        }
                        else if (eneList.at(i).GetBreed() == _T("キュウ"))
                        {
                            enemy = NEW EnemySphere();
                        }
                        else if (eneList.at(i).GetBreed() == _T("エンバン"))
                        {
                            enemy = NEW EnemyDisk();
                        }
                        else if (eneList.at(i).GetBreed() == _T("エンチュウ"))
                        {
                            enemy = NEW EnemyEnchu();
                        }
                        else if (eneList.at(i).GetBreed() == _T("ビッグリッポウタイ"))
                        {
                            enemy = NEW EnemyBigCube();
                        }
                        else if (eneList.at(i).GetBreed() == _T("スモールリッポウタイ"))
                        {
                            enemy = NEW EnemySmallCube();
                        }
                        else if (eneList.at(i).GetBreed() == _T("ハンエン"))
                        {
                            enemy = NEW EnemyHanen();
                        }
                        else if (eneList.at(i).GetBreed() == _T("ハンキュウ"))
                        {
                            enemy = NEW EnemyHankyuu();
                        }
                        else if (eneList.at(i).GetBreed() == _T("オレンジリッポウタイ"))
                        {
                            enemy = NEW EnemyOrangeCube();
                        }
                        else if (eneList.at(i).GetBreed() == _T("島民の霊"))
                        {
                            enemy = NEW EnemyGhost();
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

                        // Init関数は別スレッドで読み込みを行うのでpush_backした後に呼ぶ。
                        auto it = m_vecEnemy.rbegin();
                        (*it)->Init();
                    }
                }
            }

            //-------------------------------------
            // 空
            //-------------------------------------

            // 主人公が東に移動したら海も東に移動する
            // カメラが東に移動したら空も東に移動する
            // 主人公とカメラで分ける必要があるのは、タイトルで主人公とカメラの位置が大きく異なるから
            {
                auto cameraPos = Camera::GetEyePos();
                m_meshMap[_T("sky")]->SetPos(cameraPos);

                auto player = SharedObj::GetPlayer();
                auto work1 = m_meshMap[_T("sea")]->GetPos();
                auto work2 = player->GetPos();
                work1.x = work2.x;
                work1.z = work2.z;

                m_meshMap[_T("sea")]->SetPos(work1);
            }

            NSStarmanLib::PowereggDateTime* dateTime = NSStarmanLib::PowereggDateTime::GetObj();

            //-------------------------------------
            // 陰の表示
            //-------------------------------------
            // 太陽は昼の12時に真上、夜の0時に真下、とする。
            // 0時0分～23時59分を0.0f～0.99999fとなるように変換
            // 0時0分を0分、23時59分を1439分と見なせばよい

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
            // 太陽
            //-------------------------------------

            {
                // 主人公が東に移動したら太陽も東に移動する
                auto player = SharedObj::GetPlayer();
                auto ppos = player->GetPos();

                D3DXVECTOR3 sunPos(vec);
                sunPos *= 100000;
                sunPos += ppos;
                m_pSun->SetPos(sunPos);
            }

            //-------------------------------------
            // 3Dモデルの遅延読み込み
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

                    std::wstring xName = mapObjManager->GetModelName(needShow.at(i).m_modelId);

                    auto meshClone = NEW MeshClone(xName, pos, rot, needShow.at(i).m_scale);
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

                    delete m_meshCloneMap.at(needHide.at(i).m_id);
                    m_meshCloneMap.erase(needHide.at(i).m_id);
                }
            }

            // 100メートル以上離れた敵は消す
            // 距離を球で調べると重くなるので立方体で調べる
            // さらにY軸方向は無視する
            for (auto it = m_vecEnemy.begin(); it != m_vecEnemy.end();)
            {
                if ((*it)->GetPos().x + 100.f < player->GetPos().x ||
                    (*it)->GetPos().x - 100.f > player->GetPos().x)
                {
                    SAFE_DELETE(*it);
                    it = m_vecEnemy.erase(it);
                }
                else if ((*it)->GetPos().z + 100.f < player->GetPos().z ||
                         (*it)->GetPos().z - 100.f > player->GetPos().z)
                {
                    SAFE_DELETE(*it);
                    it = m_vecEnemy.erase(it);
                }
                else
                {
                    ++it;
                }
            }

            // プレイヤーが廃墟に近づいたらカメラモードを変える
            {
                float work = 0.f;
                work = pos.x - m_meshMap[_T("precision")]->GetPos().x;
                work = std::abs(work);
                if (work < 20.f)
                {
                    work = pos.z - m_meshMap[_T("precision")]->GetPos().z;
                    work = std::abs(work);
                    if (work < 20.f)
                    {
                        Camera::SetHouseMode(true);
                    }
                    else
                    {
                        Camera::SetHouseMode(false);
                    }
                }
                else
                {
                    Camera::SetHouseMode(false);
                }
            }

            if (!_T("m_vecEnemy.size()"))
            {
                std::wstring work;
                work = _T("m_vecEnemy.size(): ") + std::to_wstring(m_vecEnemy.size()) + _T("\n");
                OutputDebugString(work.c_str());
            }

            // NPCが死んでいたら、座標を(0, 0, 0)に移動
            {
                auto npcManager = NpcManager::Get();
                if (npcManager->GetNpcStatus(_T("daikeiman")).GetDead())
                {
                    // TODO 全角カタカナ、直すべし
                    SetNpcPos(_T("daikeiman"), D3DXVECTOR3(0.f, 0.f, 0.f));
                    npcManager->SetPos(_T("daikeiman"), 0.f, 0.f, 0.f);
                }

                if (npcManager->GetNpcStatus(_T("sankakuman")).GetDead())
                {
                    SetNpcPos(_T("sankakuman"), D3DXVECTOR3(0.f, 0.f, 0.f));
                    npcManager->SetPos(_T("sankakuman"), 0.f, 0.f, 0.f);
                }

                if (npcManager->GetNpcStatus(_T("shikakuman")).GetDead())
                {
                    SetNpcPos(_T("shikakuman"), D3DXVECTOR3(0.f, 0.f, 0.f));
                    npcManager->SetPos(_T("shikakuman"), 0.f, 0.f, 0.f);
                }
            }
        }
    }

    //-------------------------------------------------------
    // 投げ物
    //-------------------------------------------------------
    for (auto it = m_thrownList.begin(); it != m_thrownList.end(); ++it)
    {
        ++it->m_counter;

        // 0.5秒経過したら発射
        if (it->m_counter <= 60)
        {
            continue;
        }

        auto pos = it->m_mesh->GetPos();
        it->m_move.y -= 0.005f;

        // 衝突判定
        // 地面などにぶつかったら移動量0にする
        if (Intersect(pos, it->m_move))
        {
            it->m_move = D3DXVECTOR3(0.f, 0.f, 0.f);
            it->m_bStop = true;
            continue;
        }

        // モンスターに当たったらダメージを与える
        if (it->m_bHit == false)
        {
            auto enemyInfoManager = NSStarmanLib::EnemyInfoManager::GetObj();
            for (auto it2 = m_vecEnemy.begin(); it2 != m_vecEnemy.end(); ++it2)
            {
                auto enemyPos = (*it2)->GetPos();
                float dist = Common::PointToSegmentDistance(pos, pos + it->m_move, enemyPos);
                if (dist <= 2.f)
                {
                    auto hp = (*it2)->GetHP();
                    (*it2)->SetHP(hp - (int)it->m_power);
                    it->m_bHit = true;

                    BGMManager::Get()->SetBattle(true);
                }
            }
        }

        pos += it->m_move;

        it->m_mesh->SetPos(pos);
    }

    //-------------------------------------------------------
    // 投げ物（魔法）
    //-------------------------------------------------------
    for (auto it = m_thrownMagicList.begin(); it != m_thrownMagicList.end();)
    {
        ++it->m_counter;

        // 1秒経過したら発射
        if (it->m_counter <= 60)
        {
            ++it;
            continue;
        }

        auto pos = it->m_mesh->GetPos();
        it->m_move.y -= 0.002f;

        // 衝突判定
        // 地面などにぶつかったら消す
        if (Intersect(pos, it->m_move))
        {
            if (it->m_eMagicType == NSStarmanLib::eMagicType::Fire)
            {
                SoundEffect::get_ton()->play(_T("res\\sound\\fireHit.wav"), 20);
            }
            else if (it->m_eMagicType == NSStarmanLib::eMagicType::Ice)
            {
                SoundEffect::get_ton()->play(_T("res\\sound\\iceHit.wav"), 20);
            }
            else if (it->m_eMagicType == NSStarmanLib::eMagicType::Dark)
            {
                SoundEffect::get_ton()->play(_T("res\\sound\\darkHit.wav"), 20);
            }

            delete it->m_mesh;
            it = m_thrownMagicList.erase(it);

            continue;
        }

        // モンスターに当たったらダメージを与える
        bool bHit = false;
        if (it->m_bHit == false)
        {
            auto enemyInfoManager = NSStarmanLib::EnemyInfoManager::GetObj();
            for (auto it2 = m_vecEnemy.begin(); it2 != m_vecEnemy.end(); ++it2)
            {
                // 島民の霊は闇魔法でのみダメージを与えられる。
                if (it->m_eMagicType != NSStarmanLib::eMagicType::Dark)
                {
                    if ((*it2)->GetEnemyType() == eEnemyType::Ghost)
                    {
                        continue;
                    }
                }

                auto enemyPos = (*it2)->GetPos();
                float dist = Common::PointToSegmentDistance(pos, pos + it->m_move, enemyPos);
                if (dist <= 2.f)
                {
                    auto hp = (*it2)->GetHP();
                    (*it2)->SetHP(hp - (int)it->m_magicPower);
                    it->m_bHit = true;
                    bHit = true;

                    BGMManager::Get()->SetBattle(true);

                    if (it->m_eMagicType == NSStarmanLib::eMagicType::Fire)
                    {
                        SoundEffect::get_ton()->play(_T("res\\sound\\fireHit.wav"));
                    }
                    else if (it->m_eMagicType == NSStarmanLib::eMagicType::Ice)
                    {
                        SoundEffect::get_ton()->play(_T("res\\sound\\iceHit.wav"));
                    }
                    else if (it->m_eMagicType == NSStarmanLib::eMagicType::Dark)
                    {
                        SoundEffect::get_ton()->play(_T("res\\sound\\darkHit.wav"));
                    }

                    delete it->m_mesh;
                    it = m_thrownMagicList.erase(it);
                    break;
                }
            }
        }

        if (bHit)
        {
            continue;
        }
        else
        {
            pos += it->m_move;

            it->m_mesh->SetPos(pos);
            ++it;
        }
    }

    // 雨の表示
    Rain::Get()->Update();
}

void Map::Render()
{
    {
        D3DXVECTOR4 norm = Light::GetLightNormal();
        D3DXVECTOR4 norm2 { norm };
        // 天球用に下から光を当てていることにする
        // 正しくないやり方。
        norm2.x = 0.f;
        norm2.y = -1.f;
        Light::SetLightNormal(norm2);
        m_meshMap[_T("sky")]->Render();
        Light::SetLightNormal(norm);
    }

    for (auto& pair : m_meshMap)
    {
        if (pair.first == _T("sky"))
        {
            continue;
        }
        pair.second->Render();
    }

    for (auto& pair : m_meshCloneMap)
    {
        if (pair.second->GetMeshType() == MeshClone::eMeshType::OTHER)
        {
            pair.second->Render();
        }
    }

    {
        MeshClone* _begin = nullptr;
        MeshClone* _end = nullptr;
        for (auto& pair : m_meshCloneMap)
        {
            if (pair.second->GetMeshType() == MeshClone::eMeshType::GRASS)
            {
                if (_begin == nullptr)
                {
                    _begin = pair.second;
                    _begin->Begin();
                }

                pair.second->Render2();

                _end = pair.second;
            }
        }
        
        if (_end != nullptr)
        {
            _end->End();
        }
    }

    {
        MeshClone* _begin = nullptr;
        MeshClone* _end = nullptr;
        for (auto& pair : m_meshCloneMap)
        {
            if (pair.second->GetMeshType() == MeshClone::eMeshType::TREE)
            {
                if (_begin == nullptr)
                {
                    _begin = pair.second;
                    _begin->Begin();
                }

                pair.second->Render2();

                _end = pair.second;
            }
        }
        if (_end != nullptr)
        {
            _end->End();
        }
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

    for (auto it = m_thrownList.begin(); it != m_thrownList.end(); ++it)
    {
        it->m_mesh->Render();
    }

    for (auto it = m_thrownMagicList.begin(); it != m_thrownMagicList.end(); ++it)
    {
        it->m_mesh->Render();
    }

    // 雨の表示
    Rain::Get()->Draw();
}

std::vector<EnemyBase*> Map::GetEnemy()
{
    return m_vecEnemy;
}

void Map::SetEnemy(const std::vector<EnemyBase*>& vecEnemy)
{
    m_vecEnemy = vecEnemy;
}

bool Map::IntersectSub(const D3DXVECTOR3& pos, const D3DXVECTOR3& move, Mesh* mesh)
{
    BOOL  bIsHit = false;
    D3DXVECTOR3 targetPos = pos - mesh->GetPos();

    targetPos /= mesh->GetScale();
    LPD3DXMESH d3dmesh = mesh->GetD3DMesh();
    float fLandDistance;
    DWORD dwHitIndex = -1;
    float fHitU;
    float fHitV;
    D3DXIntersect(d3dmesh,
                  &targetPos,
                  &move,
                  &bIsHit,
                  &dwHitIndex,
                  &fHitU,
                  &fHitV,
                  &fLandDistance,
                  NULL,
                  NULL);

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

bool Map::IntersectSub2(const D3DXVECTOR3& pos, const D3DXVECTOR3& move, Mesh* mesh)
{
    BOOL  bIsHit = false;

    D3DXVECTOR3 targetPos = pos - mesh->GetPos();
    targetPos /= mesh->GetScale();

    BYTE* pVertices = nullptr;
    DWORD* pIndices = nullptr;

    // 頂点バッファのロック
    mesh->GetD3DMesh()->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVertices);

    // インデックスバッファのロック
    mesh->GetD3DMesh()->LockIndexBuffer(D3DLOCK_READONLY, (void**)&pIndices);

    DWORD numFaces = mesh->GetD3DMesh()->GetNumFaces();
    DWORD stride = mesh->GetD3DMesh()->GetNumBytesPerVertex();

    float minDistance = FLT_MAX;
    int hitFaceIndex = -1;

    for (DWORD i = 0; i < numFaces; ++i)
    {
        // 三角形の3頂点のインデックスを取得
        DWORD i0 = pIndices[i * 3 + 0];

        // 頂点データから位置だけ抽出（先頭にD3DXVECTOR3がある構造体と仮定）
        D3DXVECTOR3* v0 = (D3DXVECTOR3*)(pVertices + size_t(i0) * stride);

        // あまりに離れているなら交差しているか調べる必要はない。
        if (v0->x <= targetPos.x - 100.f || targetPos.x + 100.f <= v0->x)
        {
            continue;
        }

        if (v0->z <= targetPos.z - 100.f || targetPos.z + 100.f <= v0->z)
        {
            continue;
        }

        DWORD i1 = pIndices[i * 3 + 1];
        DWORD i2 = pIndices[i * 3 + 2];
        D3DXVECTOR3* v1 = (D3DXVECTOR3*)(pVertices + size_t(i1) * stride);
        D3DXVECTOR3* v2 = (D3DXVECTOR3*)(pVertices + size_t(i2) * stride);

        float u, v, dist;
        if (D3DXIntersectTri(v0, v1, v2, &targetPos, &move, &u, &v, &dist))
        {
            float judgeDistance = 1.f / mesh->GetScale();
            if (dist < judgeDistance)
            {
                minDistance = dist;
                hitFaceIndex = (int)i;
                break;
            }
        }
    }

    // バッファのアンロック
    mesh->GetD3DMesh()->UnlockVertexBuffer();
    mesh->GetD3DMesh()->UnlockIndexBuffer();

    if (hitFaceIndex >= 0)
    {
        bIsHit = TRUE;
    }
    else
    {
        bIsHit = FALSE;
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

bool Map::IntersectWater(const D3DXVECTOR3& pos, const D3DXVECTOR3& move)
{
    BOOL  bIsHit = false;
    auto sea = m_meshMap[_T("sea")];
    bIsHit = IntersectSub(pos, move, sea);

    if (bIsHit)
    {
        return true;
    }

    auto river = m_meshMap[_T("river")];
    bIsHit = IntersectSub(pos, move, river);

    return bIsHit;
}

bool Map::Intersect(const D3DXVECTOR3& pos, const D3DXVECTOR3& move)
{
    // ステージ上のオブジェクトを原点としたときのposの位置
    BOOL  bIsHit = false;
    for (auto& pair : m_meshMap)
    {
        bIsHit = IntersectSub(pos, move, pair.second);
        //bIsHit = IntersectSub2(pos, move, pair.second);
        if (bIsHit)
        {
            break;
        }
    }

    if (bIsHit)
    {
        return true;
    }

    BOOL  bIsHit2 = false;
    for (auto& pair : m_meshCloneMap)
    {
        bIsHit2 = IntersectSub(pos, move, pair.second);
        if (bIsHit2)
        {
            break;
        }
    }

    return bIsHit2;
}

bool Map::CollisionGround(const D3DXVECTOR3& pos, const D3DXVECTOR3& move)
{
    BOOL  bIsHit = false;
    bIsHit = IntersectSub(pos, move, m_meshMap[_T("Map")]);
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

D3DXVECTOR3 Map::WallSlide(const D3DXVECTOR3& pos, const D3DXVECTOR3& move, bool* bHit, bool* bInside)
{
    D3DXVECTOR3 result { move };

    bool bInside1 = false;
    bool bInside2 = false;

    for (auto& pair : m_meshMap)
    {
        if (
            pair.first == _T("Map") ||
            pair.first == _T("river") ||
            pair.first == _T("sea") ||
            pair.first == _T("precision") ||
            pair.first == _T("chest") ||
            pair.first == _T("rock2") ||
            pair.first == _T("lighthouse") ||
            pair.first == _T("shrine") ||
            pair.first == _T("testcube")
            )
        {
            bool bIsHit = false;
            bool bEnableWallWalk = true;

            if (pair.first == _T("precision"))
            {
                bEnableWallWalk = false;
            }

            result = WallSlideSub(pos, pair.second, result, &bIsHit, &bInside1, bEnableWallWalk);
            if (bIsHit)
            {
                *bHit = true;
            }
        }
    }

    for (auto& pair : m_meshCloneMap)
    {
        bool bIsHit = false;
        result = WallSlideSub(pos, pair.second, result, &bIsHit, &bInside2);
        if (bIsHit)
        {
            *bHit = true;
            break;
        }
    }

    if (bInside1 || bInside2)
    {
        *bInside = true;
    }

    if (!bInside)
    {
        // 壁ずりした先に壁があったときの処理。
        // 壁ずり先に壁があるなら移動量を0にする
        if (Intersect(pos, result))
        {
            return D3DXVECTOR3{ 0.f, 0.f, 0.f };
        }
    }

    // 接地判定
    // TODO ここが間違い。ジャンプ中フラグを追加すること。
    // (move.y <= -0.008f が諸悪の根源
    // move.yが-0.01以下で、result.yが-0.001以上だったら
    // 落下速度が10分の1以下になったということなので平らな地面に足がついているとする。
    // move.yを0にする
    if (bHit && (- 0.003f <= result.y && result.y <= 0.f))
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

    // 摩擦
    // result.xとresult.zが小さいなら0にする
    //if (-0.005f <= result.x && result.x <= 0.005f)
    if (-0.01f <= result.x && result.x <= 0.01f)
    {
        if (-0.01f <= result.z && result.z <= 0.01f)
        {
            result.x = 0.f;
            result.z = 0.f;
        }
    }

    return result;
}

void Map::SetNpcPos(const std::wstring& name, const D3DXVECTOR3& pos)
{
    m_NPC.at(name)->SetPos(pos);
}

void Map::SetNpcRot(const std::wstring& name, const float yRot)
{
    D3DXVECTOR3 rot(0.f, 0.f, 0.f);
    rot.y = yRot;

    m_NPC.at(name)->SetRotate(rot);
}

void Map::AddThrownItem(const D3DXVECTOR3& pos,
                        const D3DXVECTOR3& move,
                        const std::wstring& weaponName,
                        const NSStarmanLib::ItemInfo& itemInfo,
                        const float scale,
                        const float power,
                        const float rotY)
{
    ThrownItem work;
    work.m_itemInfo = itemInfo;
    work.m_move = move;
    work.m_power = power;

    auto weaponManager = NSStarmanLib::WeaponManager::GetObj();
    std::wstring xfilename = weaponManager->GetXfilename(weaponName);

    D3DXVECTOR3 rot(0.f, rotY, 0.f);

    auto meshClone = NEW MeshClone(xfilename, pos, rot, scale);
    meshClone->Init();
    work.m_mesh = meshClone;

    m_thrownList.push_back(work);
}

// 見つからなければidが-1のItemInfoが返る。
NSStarmanLib::ItemInfo Map::GetThrownItem(const D3DXVECTOR3& pos)
{
    bool exist = false;
    NSStarmanLib::ItemInfo result;
    result.SetId(-1);

    // 2メートル以内のアイテムを拾えるようにする
    // ラクするために円ではなく四角で範囲チェック
    for (auto it = m_thrownList.begin(); it != m_thrownList.end(); ++it)
    {
        if (it->m_bStop)
        {
            auto itemPos = it->m_mesh->GetPos();
            if (std::abs(itemPos.x - pos.x) < 2.f)
            {
                if (std::abs(itemPos.z - pos.z) < 2.f)
                {
                    result = it->m_itemInfo;
                    exist = true;
                    break;
                }
            }
        }
    }

    return result;
}

void Map::DeleteThrownItem(const NSStarmanLib::ItemInfo& thrownItem)
{
    for (auto it = m_thrownList.begin(); it != m_thrownList.end(); ++it)
    {
        if (thrownItem.GetId() == it->m_itemInfo.GetId() &&
            thrownItem.GetSubId() == it->m_itemInfo.GetSubId())
        {
            m_thrownList.erase(it);
            break;
        }
    }
}

void Map::SetThrownMagic(const D3DXVECTOR3& pos,
                         const D3DXVECTOR3& move,
                         const NSStarmanLib::eMagicType& magicType,
                         const float power)
{
    ThrownMagic work;
    work.m_eMagicType = magicType;
    work.m_move = move;
    work.m_magicPower = power;

    std::wstring xfilename;

    if (magicType == NSStarmanLib::eMagicType::Fire)
    {
        xfilename = _T("res\\model\\MagicFire.x");
    }
    else if (magicType == NSStarmanLib::eMagicType::Ice)
    {
        xfilename = _T("res\\model\\MagicIce.x");
    }
    else if (magicType == NSStarmanLib::eMagicType::Dark)
    {
        xfilename = _T("res\\model\\MagicDark.x");
    }

    D3DXVECTOR3 rot(0.f, 0.f, 0.f);

    auto mesh = NEW MeshNoShade(xfilename, pos, rot, 1.f);
    mesh->Init();
    work.m_mesh = mesh;

    work.m_counter = 0;

    m_thrownMagicList.push_back(work);
}

bool Map::NearPlant(const D3DXVECTOR3& pos)
{
    bool isHit = false;

    auto list = MapLib()->GetMapObjListR(pos.x, pos.z, 2.f);

    int modelId = MapLib()->GetModelId(_T("res\\\\model\\\\plant.x"));
    int modelId2 = MapLib()->GetModelId(_T("res\\\\model\\\\grass.x"));

    for (size_t i = 0; i < list.size(); ++i)
    {
        if (list.at(i).m_modelId == modelId || list.at(i).m_modelId == modelId2)
        {
            if (list.at(i).m_visible)
            {
                isHit = true;
            }
        }
    }

    return isHit;
}

bool Map::NearCoconut(const D3DXVECTOR3& pos)
{
    bool isHit = false;

    auto list = MapLib()->GetMapObjListR(pos.x, pos.z, 20.f);

    int modelId = MapLib()->GetModelId(_T("res\\\\model\\\\yashi.x"));

    for (size_t i = 0; i < list.size(); ++i)
    {
        if (list.at(i).m_modelId == modelId)
        {
            if (list.at(i).m_visible)
            {
                isHit = true;
            }
        }
    }

    return isHit;
}

bool Map::NearSotetsu(const D3DXVECTOR3& pos)
{
    bool isHit = false;

    auto list = MapLib()->GetMapObjListR(pos.x, pos.z, 20.f);

    int modelId = MapLib()->GetModelId(_T("res\\\\model\\\\sotetsu.x"));

    for (size_t i = 0; i < list.size(); ++i)
    {
        if (list.at(i).m_modelId == modelId)
        {
            if (list.at(i).m_visible)
            {
                isHit = true;
            }
        }
    }

    return isHit;
}

bool Map::NearThinTree(const D3DXVECTOR3& pos)
{
    bool isHit = false;

    auto list = MapLib()->GetMapObjListR(pos.x, pos.z, 2.f);

    int modelId = MapLib()->GetModelId(_T("res\\\\model\\\\treeThin.blend.x"));

    for (size_t i = 0; i < list.size(); ++i)
    {
        if (list.at(i).m_modelId == modelId && list.at(i).m_visible)
        {
            isHit = true;
        }
    }

    return isHit;
}

bool Map::NearTree(const D3DXVECTOR3& pos)
{
    bool isHit = false;

    auto list = MapLib()->GetMapObjListR(pos.x, pos.z, 2.f);

    int modelId = MapLib()->GetModelId(_T("res\\\\model\\\\tree1.x"));

    for (size_t i = 0; i < list.size(); ++i)
    {
        if (list.at(i).m_modelId == modelId && list.at(i).m_visible)
        {
            isHit = true;
        }
    }

    return isHit;
}

bool Map::NearDonguri(const D3DXVECTOR3& pos)
{
    bool isHit = false;

    auto list = MapLib()->GetMapObjListR(pos.x, pos.z, 2.f);

    int modelId = MapLib()->GetModelId(_T("res\\\\model\\\\donguri.x"));

    for (size_t i = 0; i < list.size(); ++i)
    {
        if (list.at(i).m_modelId == modelId && list.at(i).m_visible)
        {
            isHit = true;
        }
    }

    return isHit;
}

bool Map::NearStone(const D3DXVECTOR3& pos)
{
    bool isHit = false;

    auto list = MapLib()->GetMapObjListR(pos.x, pos.z, 2.f);

    int modelId = MapLib()->GetModelId(_T("res\\\\model\\\\rocks.x"));

    for (size_t i = 0; i < list.size(); ++i)
    {
        if (list.at(i).m_modelId == modelId && list.at(i).m_visible)
        {
            isHit = true;
        }
    }

    return isHit;
}

void Map::DeleteThinTree(const D3DXVECTOR3& pos)
{
    std::vector<NSStarmanLib::stMapObj> mapObjs = MapLib()->GetMapObjListR(pos.x, pos.z, 2.f);

    int id = MapLib()->GetModelId(_T("res\\\\model\\\\treeThin.blend.x"));

    for (size_t i = 0; i < mapObjs.size(); ++i)
    {
        if (id == mapObjs.at(i).m_modelId)
        {
            MapLib()->SetVisible(mapObjs.at(i).m_frameX,
                                 mapObjs.at(i).m_frameZ,
                                 mapObjs.at(i).m_id,
                                 false);
            break;
        }
    }
}

void Map::DeletePlant(const D3DXVECTOR3& pos)
{
    std::vector<NSStarmanLib::stMapObj> mapObjs = MapLib()->GetMapObjListR(pos.x, pos.z, 2.f);

    int modelId1 = MapLib()->GetModelId(_T("res\\\\model\\\\plant.x"));
    int modelId2 = MapLib()->GetModelId(_T("res\\\\model\\\\grass.x"));

    for (size_t i = 0; i < mapObjs.size(); ++i)
    {
        if (modelId1 == mapObjs.at(i).m_modelId || modelId2 == mapObjs.at(i).m_modelId)
        {
            MapLib()->SetVisible(mapObjs.at(i).m_frameX,
                                 mapObjs.at(i).m_frameZ,
                                 mapObjs.at(i).m_id,
                                 false);
            break;
        }
    }
}

NSStarmanLib::MapObjManager* Map::MapLib()
{
    auto mapObjManager = NSStarmanLib::MapObjManager::GetObj();
    return mapObjManager;
}

D3DXVECTOR3 Map::WallSlideSub(const D3DXVECTOR3& pos,
                              Mesh* mesh,
                              const D3DXVECTOR3& move,
                              bool* bHit,
                              bool* bInside,
                              const bool bEnableWallWalk)
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
    D3DXIntersect(d3dmesh,
                  &targetPos,
                  &move,
                  &bIsHit,
                  &dwHitIndex,
                  &fHitU,
                  &fHitV,
                  &fLandDistance,
                  NULL,
                  NULL);

    float judgeDistance = 2.f / mesh->GetScale();
    if (bIsHit && fLandDistance <= judgeDistance)
    {
        *bHit = true;

        // ----- キャラY座標補正 -----
        // 当たったインデックスバッファ取得
        DWORD dwHitVertexNo[3] = {};
        DWORD* pIndex = nullptr;
        HRESULT hr = d3dmesh->LockIndexBuffer(0, (void**)&pIndex);

        for (int nIdxIdx = 0; nIdxIdx < 3; nIdxIdx++)
        {
            dwHitVertexNo[nIdxIdx] = pIndex[dwHitIndex * 3 + nIdxIdx];
        }

        d3dmesh->UnlockIndexBuffer();

        // 当たったポリゴン取得
        struct VERTEX
        {
            FLOAT x, y, z; // 頂点の座標
            FLOAT normX, normY, normZ; // 法線の座標
            FLOAT u, v;   // 頂点の色
        };
        VERTEX* pVertex = nullptr;
        hr = d3dmesh->LockVertexBuffer(0, (void**)&pVertex);

        // 地面の高さに合わせる
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

        // 内側から外側への衝突判定だった場合は無視する
        // 岩の中に入ってしまって、岩から外に出ようとしている場合など。
        float dot = D3DXVec3Dot(&front, &normal_n);

        if (dot < 0 || !bEnableWallWalk)
        {
            result = (front - D3DXVec3Dot(&front, &normal_n) * normal_n);

            // 急な上り坂だったら移動速度を下げる
            if (Common::DebugMode() && Common::FasterMode())
            {
                // do nothing
            }
            else
            {
                if (result.y > 0.1f)
                {
                    // yが1.0なら移動速度を1/10、yが0.1なら移動速度を1倍とする
                    auto workY = result.y;
                    workY *= 10.f;

                    // 減速は1/10までとする。
                    if (workY >= 10.f)
                    {
                        workY = 10.f;
                    }
                    result /= workY;
                }
            }
        }
        else
        {
            result = move;
            *bInside = true;
        }

        d3dmesh->UnlockVertexBuffer();
    }
    else
    {
        result = move;
    }

    return result;
}

D3DXVECTOR3 Map::WallSlideSub(const D3DXVECTOR3& pos,
                              MeshClone* mesh,
                              const D3DXVECTOR3& move,
                              bool* bHit,
                              bool* bInside)
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

        // ----- キャラY座標補正 -----
        // 当たったインデックスバッファ取得
        WORD dwHitVertexNo[3] = {};
        WORD* pIndex = nullptr;
        HRESULT hr = d3dmesh->LockIndexBuffer(0, (void**)&pIndex);

        for (int nIdxIdx = 0; nIdxIdx < 3; nIdxIdx++)
        {
            dwHitVertexNo[nIdxIdx] = pIndex[dwHitIndex * 3 + nIdxIdx];
        }

        d3dmesh->UnlockIndexBuffer();

        // 当たったポリゴン取得
        struct VERTEX
        {
            FLOAT x, y, z; // 頂点の座標
            FLOAT normX, normY, normZ; // 法線の座標
            FLOAT u, v;   // 頂点の色
        };
        VERTEX* pVertex = nullptr;
        hr = d3dmesh->LockVertexBuffer(0, (void**)&pVertex);

        // 地面の高さに合わせる
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

        // 内側から外側への衝突判定だった場合は無視する
        // 岩の中に入ってしまって、岩から外に出ようとしている場合など。
        float dot = D3DXVec3Dot(&front, &normal_n);

        if (dot < 0)
        {
            result = (front - D3DXVec3Dot(&front, &normal_n) * normal_n);

            // 急な上り坂だったら移動速度を下げる
            if (Common::DebugMode() && Common::FasterMode())
            {
                // do nothing
            }
            else
            {
                // 急な上り坂だったら移動速度を下げる
                if (result.y > 0.1f)
                {
                    // yが1.0なら移動速度を1/10、yが0.1なら移動速度を1倍とする
                    auto workY = result.y;
                    workY *= 10.f;

                    // 減速は1/10までとする。
                    if (workY >= 10.f)
                    {
                        workY = 10.f;
                    }
                    result /= workY;
                }
            }
        }
        else
        {
            result = move;
            *bInside = true;
        }

        d3dmesh->UnlockVertexBuffer();
    }
    else
    {
        result = move;
    }
    return result;
}

void Map::DeleteObj(const D3DXVECTOR3& pos, const eMapObjType eMapObjType_)
{
    std::vector<NSStarmanLib::stMapObj> mapObjs = MapLib()->GetMapObjListR(pos.x, pos.z, 2.f);

    int id = (int)eMapObjType_;

    for (size_t i = 0; i < mapObjs.size(); ++i)
    {
        if (id == mapObjs.at(i).m_modelId)
        {
            MapLib()->SetVisible(mapObjs.at(i).m_frameX,
                                 mapObjs.at(i).m_frameZ,
                                 mapObjs.at(i).m_id,
                                 false);
            break;
        }
    }
}

int Map::GetProgress()
{
    return m_progress.load();
}

