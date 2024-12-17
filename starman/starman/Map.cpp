#include "Map.h"
#include "Light.h"
#include "SharedObj.h"
#include "KeyBoard.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/EnemyInfoManager.h"
#include "EnemyCube.h"
#include "EnemyDisk.h"
#include "EnemySphere.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/PowereggDateTime.h"

Map::Map()
{
}

Map::~Map()
{
    for (auto pair : m_meshMap)
    {
        SAFE_DELETE(pair.second);
    }
    SAFE_DELETE(m_spriteStageName);
}

void Map::Init()
{
    // 拡大縮小すると衝突判定が効かなくなる！！！
    {
        Mesh* mesh = new Mesh(
            //"res\\model\\Map\\Map.x",
            "res\\model\\prolitan\\prolitan.x",
            D3DXVECTOR3(0.f, 0.f, 0.f),
            D3DXVECTOR3(0.f, 0.f, 0.f),
            1.0f);
        mesh->Init();
        m_meshMap["Map"] = mesh;
    }
    {
        Mesh* mesh = new Mesh(
            "res\\model\\prolitan\\sea.x",
            D3DXVECTOR3(10.f, 10.f, 10.f),
            D3DXVECTOR3(0.f, 0.f, 0.f),
            1.0f);
        mesh->Init();
        m_meshMap["sea"] = mesh;
    }
    {
        Mesh* mesh = new Mesh(
            "res\\model\\cube6\\cube6.x",
            D3DXVECTOR3(-10.f, 0.f, 0.f),
            D3DXVECTOR3(0.f, 0.f, 0.f),
            1.0f);
        mesh->Init();
        m_meshMap["cube61"] = mesh;

        mesh = new Mesh(
            "res\\model\\cube6\\cube6.x",
            D3DXVECTOR3(-13.f, 1.f, 0.f),
            D3DXVECTOR3(0.f, 0.f, 0.f),
            1.0f);
        mesh->Init();
        m_meshMap["cube62"] = mesh;

        mesh = new Mesh(
            "res\\model\\cube6\\cube6.x",
            D3DXVECTOR3(-16.f, 2.f, 0.f),
            D3DXVECTOR3(0.f, 0.f, 0.f),
            1.0f);
        mesh->Init();
        m_meshMap["cube63"] = mesh;

        mesh = new Mesh(
            "res\\model\\collisionTest\\colli.x",
            D3DXVECTOR3(0.f, 0.f, -20.f),
            D3DXVECTOR3(0.f, 0.f, 0.f),
            1.0f);
        mesh->Init();
        m_meshMap["colli"] = mesh;
    }
    // 遅延読み込み
    {
        m_lazyMesh.Init("res\\model\\prolitan\\prolitan1.x",
                        D3DXVECTOR3(0.f, 0.f, 0.f),
                        D3DXVECTOR3(0.f, 0.f, 0.f));
        m_lazyMesh.SetLoadPos(D3DXVECTOR3(367.f, 343.f, 755.f), 400.f);
    }

    m_spriteStageName = new Sprite("res\\image\\Map.png");
    m_nStagenameCount = 0;

    {
        Mesh* mesh { nullptr };
        D3DXVECTOR3 b = D3DXVECTOR3(0.f, 0.f, 0.f);
        D3DXVECTOR3 c = D3DXVECTOR3(0.f, 0.f, 0.f);
        b.x = 15.f;
        b.y = -80.f;
        mesh = new Mesh("res\\model\\hemisphere\\hemisphere.x", b, c, 3000.0f);
        mesh->Init();
        m_meshMap["sky"] = mesh;

        // sky2は何のために存在している？？
        c.y = D3DX_PI;
        mesh = new Mesh("res\\model\\hemisphere\\hemisphere.x", b, c, 3000.0f);
        mesh->Init();
        m_meshMap["sky2"] = mesh;
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
    // 太陽
    //--------------------------------------------
    m_pSun = new MeshNoShade("res\\model\\sun\\sun.x",
                             D3DXVECTOR3(0.f, -10000.f, 0.f),
                             D3DXVECTOR3(0.f, 0.f, 0.f),
                             500.0f);
    m_pSun->Init();

    //--------------------------------------------
    // NPC
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
        m_NPC["ダイケイマン"] = daikeiman;
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
        m_NPC["サンカクマン"] = sankakuman;
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
        m_NPC["シカクマン"] = shikakuman;
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
        m_NPC["ビム"] = animMesh;
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
        m_NPC["ブイエスコ"] = animMesh;
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
        m_NPC["ウェブニン"] = animMesh;
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
        //m_NPC["ジーオフ"] = animMesh;
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
        m_NPC["ガイン"] = animMesh;
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
        m_NPC["ネス"] = animMesh;
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
        m_NPC["ノーブル"] = animMesh;
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
    m_nStagenameCount++;

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

    // 敵が100ートル以内にいたら読み込んで表示
    //std::vector<NSStarmanLib::EnemyInfo> eneList = enemyInfoManager->GetEnemyInfo(pos.x, pos.y, pos.z, 100.f);
    std::vector<NSStarmanLib::EnemyInfo> eneList = enemyInfoManager->GetEnemyInfo(pos.x, pos.y, pos.z, 10.f);

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
                if (eneList.at(i).GetBreed() == "リッポウタイ")
                {
				    enemy = new EnemyCube();
                }
                else if (eneList.at(i).GetBreed() == "キュウ")
                {
				    enemy = new EnemySphere();
                }
                else if (eneList.at(i).GetBreed() == "エンバン")
                {
				    enemy = new EnemyDisk();
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

    // 60回に一回（＝1秒ごと）の処理
    {
        static int counter = 0;
        counter++;
        if (counter >= 60)
        {
            counter = 0;
        }
        if (counter == 0)
        {
            NSStarmanLib::PowereggDateTime* dateTime = NSStarmanLib::PowereggDateTime::GetObj();

            //-------------------------------------
            // 太陽の明るさ
            //-------------------------------------
            // 昼の12時が最も明るく、夜の0時が最も暗いこととする
            // TODO サインカーブにしたほうが良い。
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
            D3DXVECTOR3 sunPos(vec);
            sunPos *= 2000;
            m_pSun->SetPos(sunPos);
        }
    }
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
        m_meshMap["sky"]->Render();
        //m_meshMap["sky2"]->Render();
        Light::SetLightNormal(norm);
    }
    for (auto pair : m_meshMap)
    {
        if (pair.first == "sky" || pair.first == "sky2")
        {
            continue;
        }
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
    if (m_nStagenameCount <= 255)
    {
        m_spriteStageName->Render(D3DXVECTOR3 {0.f, 0.f, 0.f}, 255 - m_nStagenameCount);
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
    if (bIsHit && fLandDistance <= 1.f)
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
    // ステージ上のオブジェクトを原点としたときのposの位置
    BOOL  bIsHit = false;
    for (auto pair : m_meshMap)
    {
        bIsHit = IntersectSub(pos, rot, pair.second);
        if (bIsHit)
        {
            break;
        }
    }
    return bIsHit;
}

bool Map::CollisionGround(const D3DXVECTOR3& pos, const D3DXVECTOR3& move)
{
    // ステージ上のオブジェクトを原点としたときのposの位置
    BOOL  bIsHit = false;
    for (auto pair : m_meshMap)
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
    if (bIsHit && fLandDistance <= 2.f)
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
    for (auto pair : m_meshMap)
    {
        bool bIsHit = false;
        result = WallSlideSub(pos, pair.second, result, &bIsHit);
        if (bIsHit)
        {
            *bHit = true;
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
    if (bIsHit && fLandDistance <= 2.f)
    {
        OutputDebugString("IsHit\n");
        *bHit = true;

        // ----- キャラY座標補正 -----
        // 当たったインデックスバッファ取得
        WORD dwHitVertexNo[3];
        WORD* pIndex;
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
        VERTEX* pVertex;
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

bool LazyMesh::IsLoaded()
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
