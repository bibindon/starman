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
        
        // �ʏ�P�[�X�ŗ�O���N���Ȃ����Ƃ��m�F����e�X�g�B�P����public�֐������s���邾��
        TEST_METHOD(TestMethod01)
        {
            Raft2 raft;
        }

        // �ʏ�P�[�X�ŗ�O���N���Ȃ����Ƃ��m�F����e�X�g�B�P����public�֐������s���邾��
        TEST_METHOD(TestMethod02)
        {
            Util::InitWin_DX9_DI8();
            auto voyage = NSStarmanLib::Voyage::Get();
            voyage->Init("raft1.csv");


            Raft2 raft;

            // Target
            raft.Init(1);
        }

        // �ʏ�P�[�X�ŗ�O���N���Ȃ����Ƃ��m�F����e�X�g�B�P����public�֐������s���邾��
        TEST_METHOD(TestMethod03)
        {
            Raft2 raft;

            // Target
            raft.Finalize();
        }

        // �ʏ�P�[�X�ŗ�O���N���Ȃ����Ƃ��m�F����e�X�g�B�P����public�֐������s���邾��
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

        // �ʏ�P�[�X�ŗ�O���N���Ȃ����Ƃ��m�F����e�X�g�B�P����public�֐������s���邾��
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

        // �ʏ�P�[�X�ŗ�O���N���Ȃ����Ƃ��m�F����e�X�g�B�P����public�֐������s���邾��
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

        // �ʏ�P�[�X�ŗ�O���N���Ȃ����Ƃ��m�F����e�X�g�B�P����public�֐������s���邾��
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

        // �ʏ�P�[�X�ŗ�O���N���Ȃ����Ƃ��m�F����e�X�g�B�P����public�֐������s���邾��
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

        // �ʏ�P�[�X�ŗ�O���N���Ȃ����Ƃ��m�F����e�X�g�B�P����public�֐������s���邾��
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

        // �ʏ�P�[�X�ŗ�O���N���Ȃ����Ƃ��m�F����e�X�g�B�P����public�֐������s���邾��
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

        // �ʏ�P�[�X�ŗ�O���N���Ȃ����Ƃ��m�F����e�X�g�B�P����public�֐������s���邾��
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

        // �ʏ�P�[�X�ŗ�O���N���Ȃ����Ƃ��m�F����e�X�g�B�P����public�֐������s���邾��
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

        // �ʏ�P�[�X�ŗ�O���N���Ȃ����Ƃ��m�F����e�X�g�B�P����public�֐������s���邾��
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

        // �ʏ�P�[�X�ŗ�O���N���Ȃ����Ƃ��m�F����e�X�g�B�P����public�֐������s���邾��
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
        
        // �ʏ�P�[�X�ŗ�O���N���Ȃ����Ƃ��m�F����e�X�g�B�P����public�֐������s���邾��
        TEST_METHOD(TestMethod01)
        {
            // Target
            auto voyageMgr = VoyageManager::Get();
        }

        // �ʏ�P�[�X�ŗ�O���N���Ȃ����Ƃ��m�F����e�X�g�B�P����public�֐������s���邾��
        TEST_METHOD(TestMethod02)
        {
            auto voyageMgr = VoyageManager::Get();

            // Target
            voyageMgr->Init();
        }

        // �ʏ�P�[�X�ŗ�O���N���Ȃ����Ƃ��m�F����e�X�g�B�P����public�֐������s���邾��
        TEST_METHOD(TestMethod03)
        {
            auto voyageMgr = VoyageManager::Get();

            // Target
            voyageMgr->Finalize();
        }

        // �ʏ�P�[�X�ŗ�O���N���Ȃ����Ƃ��m�F����e�X�g�B�P����public�֐������s���邾��
        TEST_METHOD(TestMethod04)
        {
            auto voyageMgr = VoyageManager::Get();

            // Target
            eBattleState state;
            voyageMgr->Update(&state);
        }

        // �ʏ�P�[�X�ŗ�O���N���Ȃ����Ƃ��m�F����e�X�g�B�P����public�֐������s���邾��
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

        // �ʏ�P�[�X�ŗ�O���N���Ȃ����Ƃ��m�F����e�X�g�B�P����public�֐������s���邾��
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

        // �ʏ�P�[�X�ŗ�O���N���Ȃ����Ƃ��m�F����e�X�g�B�P����public�֐������s���邾��
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

        // �ʏ�P�[�X�ŗ�O���N���Ȃ����Ƃ��m�F����e�X�g�B�P����public�֐������s���邾��
        TEST_METHOD(TestMethod08)
        {
            Util::InitWin_DX9_DI8();
            NSStarmanLib::Voyage::Get()->Init("raft1.csv");
            NSStarmanLib::Voyage::Get()->SetRaftCurrentId(1);

            auto voyageMgr = VoyageManager::Get();

            // Target
            voyageMgr->Set3HoursAuto();
        }

        // �ʏ�P�[�X�ŗ�O���N���Ȃ����Ƃ��m�F����e�X�g�B�P����public�֐������s���邾��
        TEST_METHOD(TestMethod09)
        {
            auto voyageMgr = VoyageManager::Get();

            // Target
            voyageMgr->SetRaftMode(true);
            voyageMgr->SetRaftMode(false);
        }

        // �ʏ�P�[�X�ŗ�O���N���Ȃ����Ƃ��m�F����e�X�g�B�P����public�֐������s���邾��
        TEST_METHOD(TestMethod10)
        {
            auto voyageMgr = VoyageManager::Get();

            // Target
            voyageMgr->GetRaftMode();
        }

        // �ʏ�P�[�X�ŗ�O���N���Ȃ����Ƃ��m�F����e�X�g�B�P����public�֐������s���邾��
        TEST_METHOD(TestMethod11)
        {
            Util::InitWin_DX9_DI8();
            NSStarmanLib::Voyage::Get()->Init("raft1.csv");
            NSStarmanLib::Voyage::Get()->SetRaftCurrentId(1);

            auto voyageMgr = VoyageManager::Get();

            // Target
            voyageMgr->GetPosType();
        }

        // �ʏ�P�[�X�ŗ�O���N���Ȃ����Ƃ��m�F����e�X�g�B�P����public�֐������s���邾��
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

        // �ʏ�P�[�X�ŗ�O���N���Ȃ����Ƃ��m�F����e�X�g�B�P����public�֐������s���邾��
        TEST_METHOD(TestMethod13)
        {
            auto voyageMgr = VoyageManager::Get();

            // Target
            D3DXVECTOR3 pos;
            voyageMgr->CheckNearRaft(pos);
        }

        // �ʏ�P�[�X�ŗ�O���N���Ȃ����Ƃ��m�F����e�X�g�B�P����public�֐������s���邾��
        TEST_METHOD(TestMethod14)
        {
            auto voyageMgr = VoyageManager::Get();

            // Target
            D3DXVECTOR3 pos;
            voyageMgr->GetNearRaftId(pos);
        }

        // �ʏ�P�[�X�ŗ�O���N���Ȃ����Ƃ��m�F����e�X�g�B�P����public�֐������s���邾��
        TEST_METHOD(TestMethod17)
        {
            auto voyageMgr = VoyageManager::Get();

            // Target
            voyageMgr->GetRaftCount();
        }

        // �ʏ�P�[�X�ŗ�O���N���Ȃ����Ƃ��m�F����e�X�g�B�P����public�֐������s���邾��
        TEST_METHOD(TestMethod18)
        {
            auto voyageMgr = VoyageManager::Get();

            // Target
            D3DXVECTOR3 pos;
            D3DXVECTOR3 move;
            voyageMgr->Intersect(pos, move);
        }

        // �ʏ�P�[�X�ŗ�O���N���Ȃ����Ƃ��m�F����e�X�g�B�P����public�֐������s���邾��
        TEST_METHOD(TestMethod19)
        {
            auto voyageMgr = VoyageManager::Get();
            voyageMgr->Init();

            // Target
            D3DXVECTOR3 pos;
            D3DXVECTOR3 move;
            voyageMgr->WallSlide(pos, move);
        }

        // �ʏ�P�[�X�ŗ�O���N���Ȃ����Ƃ��m�F����e�X�g�B�P����public�֐������s���邾��
        TEST_METHOD(TestMethod20)
        {
            Util::InitWin_DX9_DI8();
            NSStarmanLib::Voyage::Get()->Init("raft1.csv");
            NSStarmanLib::Voyage::Get()->SetRaftCurrentId(1);

            auto voyageMgr = VoyageManager::Get();

            // Target
            voyageMgr->GetRaftXYZ(1);
        }

        // �ʏ�P�[�X�ŗ�O���N���Ȃ����Ƃ��m�F����e�X�g�B�P����public�֐������s���邾��
        TEST_METHOD(TestMethod21)
        {
            Util::InitWin_DX9_DI8();
            NSStarmanLib::Voyage::Get()->Init("raft1.csv");
            NSStarmanLib::Voyage::Get()->SetRaftCurrentId(1);

            auto voyageMgr = VoyageManager::Get();

            // Target
            voyageMgr->GetRaftRotateY(1);
        }

        // �ʏ�P�[�X�ŗ�O���N���Ȃ����Ƃ��m�F����e�X�g�B�P����public�֐������s���邾��
        TEST_METHOD(TestMethod22)
        {
            Util::InitWin_DX9_DI8();
            NSStarmanLib::Voyage::Get()->Init("raft1.csv");
            NSStarmanLib::Voyage::Get()->SetRaftCurrentId(1);

            auto voyageMgr = VoyageManager::Get();

            // Target
            voyageMgr->GetRaftDurability();
        }

        // �ʏ�P�[�X�ŗ�O���N���Ȃ����Ƃ��m�F����e�X�g�B�P����public�֐������s���邾��
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
