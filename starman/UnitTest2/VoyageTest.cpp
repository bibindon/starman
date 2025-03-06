#include "CppUnitTest.h"
#include "../starman/VoyageManager.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/Voyage.h"
#include "../starman/SharedObj.h"
#include "Util.h"

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3d9.lib")
#if defined(NDEBUG)
#pragma comment(lib, "d3dx9.lib")
#else
#pragma comment(lib, "d3dx9d.lib")
#endif

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest2
{
    TEST_CLASS(RaftTest)
    {
    public:

        void Init()
        {
        }
        
        // 通常ケースで例外が起きないことを確認するテスト。単純にpublic関数を実行するだけ
        TEST_METHOD(TestMethod01)
        {
            Raft2 raft;
        }

        // 通常ケースで例外が起きないことを確認するテスト。単純にpublic関数を実行するだけ
        TEST_METHOD(TestMethod02)
        {
            Util::InitWin_DX9_DI8();
            auto voyage = NSStarmanLib::Voyage::Get();
            voyage->Init("raft1.csv");


            Raft2 raft;

            // Target
            raft.Init(1);
        }

        // 通常ケースで例外が起きないことを確認するテスト。単純にpublic関数を実行するだけ
        TEST_METHOD(TestMethod03)
        {
            Raft2 raft;

            // Target
            raft.Finalize();
        }

        // 通常ケースで例外が起きないことを確認するテスト。単純にpublic関数を実行するだけ
        TEST_METHOD(TestMethod04)
        {
            Util::InitWin_DX9_DI8();
            NSStarmanLib::Voyage::Get()->Init("raft1.csv");
            NSStarmanLib::Voyage::Get()->SetRaftCurrentId(1);

            auto player = NEW Player();
            SharedObj::SetPlayer(player);

            Raft2 raft;
            raft.Init(1);

            // Target
            raft.Update();
        }

        // 通常ケースで例外が起きないことを確認するテスト。単純にpublic関数を実行するだけ
        TEST_METHOD(TestMethod05)
        {
            Util::InitWin_DX9_DI8();
            NSStarmanLib::Voyage::Get()->Init("raft1.csv");
            NSStarmanLib::Voyage::Get()->SetRaftCurrentId(1);

            Raft2 raft;
            raft.Init(1);

            // Target
            raft.Draw();
        }

        // 通常ケースで例外が起きないことを確認するテスト。単純にpublic関数を実行するだけ
        TEST_METHOD(TestMethod06)
        {
            Util::InitWin_DX9_DI8();
            NSStarmanLib::Voyage::Get()->Init("raft1.csv");
            NSStarmanLib::Voyage::Get()->SetRaftCurrentId(1);

            Raft2 raft;
            raft.Init(1);

            // Target
            raft.SetSail(true);
            raft.SetSail(false);
        }

        // 通常ケースで例外が起きないことを確認するテスト。単純にpublic関数を実行するだけ
        TEST_METHOD(TestMethod07)
        {
            Util::InitWin_DX9_DI8();
            NSStarmanLib::Voyage::Get()->Init("raft1.csv");
            NSStarmanLib::Voyage::Get()->SetRaftCurrentId(1);

            Raft2 raft;
            raft.Init(1);

            // Target
            raft.PullOarBoth();
        }

        // 通常ケースで例外が起きないことを確認するテスト。単純にpublic関数を実行するだけ
        TEST_METHOD(TestMethod08)
        {
            Util::InitWin_DX9_DI8();
            NSStarmanLib::Voyage::Get()->Init("raft1.csv");
            NSStarmanLib::Voyage::Get()->SetRaftCurrentId(1);

            Raft2 raft;
            raft.Init(1);

            // Target
            raft.PullOarLeft();
        }

        // 通常ケースで例外が起きないことを確認するテスト。単純にpublic関数を実行するだけ
        TEST_METHOD(TestMethod09)
        {
            Util::InitWin_DX9_DI8();
            NSStarmanLib::Voyage::Get()->Init("raft1.csv");
            NSStarmanLib::Voyage::Get()->SetRaftCurrentId(1);

            Raft2 raft;
            raft.Init(1);

            // Target
            raft.PullOarRight();
        }

        // 通常ケースで例外が起きないことを確認するテスト。単純にpublic関数を実行するだけ
        TEST_METHOD(TestMethod10)
        {
            Util::InitWin_DX9_DI8();
            NSStarmanLib::Voyage::Get()->Init("raft1.csv");
            NSStarmanLib::Voyage::Get()->SetRaftCurrentId(1);

            Raft2 raft;
            raft.Init(1);

            // Target
            raft.GetPos();
        }

        // 通常ケースで例外が起きないことを確認するテスト。単純にpublic関数を実行するだけ
        TEST_METHOD(TestMethod11)
        {
            Util::InitWin_DX9_DI8();
            NSStarmanLib::Voyage::Get()->Init("raft1.csv");
            NSStarmanLib::Voyage::Get()->SetRaftCurrentId(1);

            Raft2 raft;
            raft.Init(1);

            // Target
            D3DXVECTOR3 pos;
            raft.SetPos(pos);
        }

        // 通常ケースで例外が起きないことを確認するテスト。単純にpublic関数を実行するだけ
        TEST_METHOD(TestMethod12)
        {
            Util::InitWin_DX9_DI8();
            NSStarmanLib::Voyage::Get()->Init("raft1.csv");
            NSStarmanLib::Voyage::Get()->SetRaftCurrentId(1);

            Raft2 raft;
            raft.Init(1);

            // Target
            raft.GetRotate();
        }

        // 通常ケースで例外が起きないことを確認するテスト。単純にpublic関数を実行するだけ
        TEST_METHOD(TestMethod13)
        {
            Util::InitWin_DX9_DI8();
            NSStarmanLib::Voyage::Get()->Init("raft1.csv");
            NSStarmanLib::Voyage::Get()->SetRaftCurrentId(1);

            Raft2 raft;
            raft.Init(1);

            // Target
            D3DXVECTOR3 rot;
            raft.SetRotate(rot);
        }

        // 通常ケースで例外が起きないことを確認するテスト。単純にpublic関数を実行するだけ
        TEST_METHOD(TestMethod14)
        {
            Util::InitWin_DX9_DI8();
            NSStarmanLib::Voyage::Get()->Init("raft1.csv");
            NSStarmanLib::Voyage::Get()->SetRaftCurrentId(1);

            Raft2 raft;
            raft.Init(1);

            // Target
            raft.GetMesh();
        }
    };

    TEST_CLASS(VoyageTest)
    {
    public:
        
        // 通常ケースで例外が起きないことを確認するテスト。単純にpublic関数を実行するだけ
        TEST_METHOD(TestMethod01)
        {
            // Target
            auto voyageMgr = VoyageManager::Get();
        }

        // 通常ケースで例外が起きないことを確認するテスト。単純にpublic関数を実行するだけ
        TEST_METHOD(TestMethod02)
        {
            auto voyageMgr = VoyageManager::Get();

            // Target
            voyageMgr->Init();
        }

        // 通常ケースで例外が起きないことを確認するテスト。単純にpublic関数を実行するだけ
        TEST_METHOD(TestMethod03)
        {
            auto voyageMgr = VoyageManager::Get();

            // Target
            voyageMgr->Finalize();
        }

        // 通常ケースで例外が起きないことを確認するテスト。単純にpublic関数を実行するだけ
        TEST_METHOD(TestMethod04)
        {
            auto voyageMgr = VoyageManager::Get();

            // Target
            eBattleState state;
            voyageMgr->Update(&state);
        }

        // 通常ケースで例外が起きないことを確認するテスト。単純にpublic関数を実行するだけ
        TEST_METHOD(TestMethod05)
        {
            Util::InitWin_DX9_DI8();
            NSStarmanLib::Voyage::Get()->Init("raft1.csv");
            NSStarmanLib::Voyage::Get()->SetRaftCurrentId(1);

            auto voyageMgr = VoyageManager::Get();
            voyageMgr->Init();

            // Target
            voyageMgr->Draw();
        }

        // 通常ケースで例外が起きないことを確認するテスト。単純にpublic関数を実行するだけ
        TEST_METHOD(TestMethod06)
        {
            Util::InitWin_DX9_DI8();
            NSStarmanLib::Voyage::Get()->Init("raft1.csv");
            NSStarmanLib::Voyage::Get()->SetRaftCurrentId(1);

            auto voyageMgr = VoyageManager::Get();
            voyageMgr->Init();

            // Target
            voyageMgr->GetSail();
        }

        // 通常ケースで例外が起きないことを確認するテスト。単純にpublic関数を実行するだけ
        TEST_METHOD(TestMethod07)
        {
            Util::InitWin_DX9_DI8();
            NSStarmanLib::Voyage::Get()->Init("raft1.csv");
            NSStarmanLib::Voyage::Get()->SetRaftCurrentId(1);

            auto voyageMgr = VoyageManager::Get();
            voyageMgr->Init();

            // Target
            voyageMgr->SetSail(true);
            voyageMgr->SetSail(false);
        }

        // 通常ケースで例外が起きないことを確認するテスト。単純にpublic関数を実行するだけ
        TEST_METHOD(TestMethod08)
        {
            Util::InitWin_DX9_DI8();
            NSStarmanLib::Voyage::Get()->Init("raft1.csv");
            NSStarmanLib::Voyage::Get()->SetRaftCurrentId(1);

            auto voyageMgr = VoyageManager::Get();

            // Target
            voyageMgr->Set3HoursAuto();
        }

        // 通常ケースで例外が起きないことを確認するテスト。単純にpublic関数を実行するだけ
        TEST_METHOD(TestMethod09)
        {
            auto voyageMgr = VoyageManager::Get();

            // Target
            voyageMgr->SetRaftMode(true);
            voyageMgr->SetRaftMode(false);
        }

        // 通常ケースで例外が起きないことを確認するテスト。単純にpublic関数を実行するだけ
        TEST_METHOD(TestMethod10)
        {
            auto voyageMgr = VoyageManager::Get();

            // Target
            voyageMgr->GetRaftMode();
        }

        // 通常ケースで例外が起きないことを確認するテスト。単純にpublic関数を実行するだけ
        TEST_METHOD(TestMethod11)
        {
            Util::InitWin_DX9_DI8();
            NSStarmanLib::Voyage::Get()->Init("raft1.csv");
            NSStarmanLib::Voyage::Get()->SetRaftCurrentId(1);

            auto voyageMgr = VoyageManager::Get();

            // Target
            voyageMgr->GetPosType();
        }

        // 通常ケースで例外が起きないことを確認するテスト。単純にpublic関数を実行するだけ
        TEST_METHOD(TestMethod12)
        {
            Util::InitWin_DX9_DI8();
            NSStarmanLib::Voyage::Get()->Init("raft1.csv");
            NSStarmanLib::Voyage::Get()->SetRaftCurrentId(1);

            auto voyageMgr = VoyageManager::Get();

            // Target
            voyageMgr->SetPosType(NSStarmanLib::Raft::ePosType::River);
            voyageMgr->SetPosType(NSStarmanLib::Raft::ePosType::Sea);
        }

        // 通常ケースで例外が起きないことを確認するテスト。単純にpublic関数を実行するだけ
        TEST_METHOD(TestMethod13)
        {
            auto voyageMgr = VoyageManager::Get();

            // Target
            D3DXVECTOR3 pos;
            voyageMgr->CheckNearRaft(pos);
        }

        // 通常ケースで例外が起きないことを確認するテスト。単純にpublic関数を実行するだけ
        TEST_METHOD(TestMethod14)
        {
            auto voyageMgr = VoyageManager::Get();

            // Target
            D3DXVECTOR3 pos;
            voyageMgr->GetNearRaftId(pos);
        }

        // 通常ケースで例外が起きないことを確認するテスト。単純にpublic関数を実行するだけ
        TEST_METHOD(TestMethod17)
        {
            auto voyageMgr = VoyageManager::Get();

            // Target
            voyageMgr->GetRaftCount();
        }

        // 通常ケースで例外が起きないことを確認するテスト。単純にpublic関数を実行するだけ
        TEST_METHOD(TestMethod18)
        {
            auto voyageMgr = VoyageManager::Get();

            // Target
            D3DXVECTOR3 pos;
            D3DXVECTOR3 move;
            voyageMgr->Intersect(pos, move);
        }

        // 通常ケースで例外が起きないことを確認するテスト。単純にpublic関数を実行するだけ
        TEST_METHOD(TestMethod19)
        {
            auto voyageMgr = VoyageManager::Get();
            voyageMgr->Init();

            // Target
            D3DXVECTOR3 pos;
            D3DXVECTOR3 move;
            voyageMgr->WallSlide(pos, move);
        }

        // 通常ケースで例外が起きないことを確認するテスト。単純にpublic関数を実行するだけ
        TEST_METHOD(TestMethod20)
        {
            Util::InitWin_DX9_DI8();
            NSStarmanLib::Voyage::Get()->Init("raft1.csv");
            NSStarmanLib::Voyage::Get()->SetRaftCurrentId(1);

            auto voyageMgr = VoyageManager::Get();

            // Target
            voyageMgr->GetRaftXYZ(1);
        }

        // 通常ケースで例外が起きないことを確認するテスト。単純にpublic関数を実行するだけ
        TEST_METHOD(TestMethod21)
        {
            Util::InitWin_DX9_DI8();
            NSStarmanLib::Voyage::Get()->Init("raft1.csv");
            NSStarmanLib::Voyage::Get()->SetRaftCurrentId(1);

            auto voyageMgr = VoyageManager::Get();

            // Target
            voyageMgr->GetRaftRotateY(1);
        }

        // 通常ケースで例外が起きないことを確認するテスト。単純にpublic関数を実行するだけ
        TEST_METHOD(TestMethod22)
        {
            Util::InitWin_DX9_DI8();
            NSStarmanLib::Voyage::Get()->Init("raft1.csv");
            NSStarmanLib::Voyage::Get()->SetRaftCurrentId(1);

            auto voyageMgr = VoyageManager::Get();

            // Target
            voyageMgr->GetRaftDurability();
        }

        // 通常ケースで例外が起きないことを確認するテスト。単純にpublic関数を実行するだけ
        TEST_METHOD(TestMethod23)
        {
            Util::InitWin_DX9_DI8();
            NSStarmanLib::Voyage::Get()->Init("raft1.csv");
            NSStarmanLib::Voyage::Get()->SetRaftCurrentId(1);

            auto voyageMgr = VoyageManager::Get();

            // Target
            voyageMgr->GetRaftLevel();
        }
    };
}
