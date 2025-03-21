#include "CppUnitTest.h"
#include "../starman/CraftManager.h"
#include "../starman/MainWindow.h"
#include "Util.h"
#include "../starman/SaveManager.h"
#include "../starman/KeyBoard.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/CraftSystem.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/CraftInfoManager.h"
#include "../starman/PopUp2.h"

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
    TEST_CLASS(CraftTest)
    {
    public:
        
        // 単純にpublic関数を呼ぶだけのテスト
        TEST_METHOD(TestMethod01)
        {
            CraftManager craft;
        }
        
        // 単純にpublic関数を呼ぶだけのテスト
        TEST_METHOD(TestMethod02)
        {
            Util::InitWin_DX9_DI8();
            CraftManager craft;

            craft.Init();

            Util::ReleaseWin_DX9_DI8();
        }
        
        // 単純にpublic関数を呼ぶだけのテスト
        TEST_METHOD(TestMethod03)
        {
            Util::InitWin_DX9_DI8();
            CraftManager craft;

            craft.Finalize();

            Util::ReleaseWin_DX9_DI8();
        }
        
        // 単純にpublic関数を呼ぶだけのテスト
        TEST_METHOD(TestMethod04)
        {
            Util::InitWin_DX9_DI8();
            CraftManager craft;

            eBattleState state;
            craft.Operate(&state);

            Util::ReleaseWin_DX9_DI8();
        }
        
        // 単純にpublic関数を呼ぶだけのテスト
        TEST_METHOD(TestMethod05)
        {
            Util::InitWin_DX9_DI8();

            SaveManager::Get()->Load();

            CraftManager craft;

            craft.SetTestMode();
            craft.Init();

            craft.Draw();

            Util::ReleaseWin_DX9_DI8();
        }

        // 単純にpublic関数を呼ぶだけのテスト
        TEST_METHOD(TestMethod06)
        {
            Util::InitWin_DX9_DI8();
            CraftManager craft;

            craft.Build();

            Util::ReleaseWin_DX9_DI8();
        }

        // イカダをクラフト
        // クラフト用の素材が足りないケース。
        // キューに追加されず、クラフト用の素材が足りない、というポップアップが表示されること
        TEST_METHOD(CraftRaftTest01)
        {
            Util::InitWin_DX9_DI8();
            KeyBoard::ForTest_SetTestMode();

            SaveManager::Get()->Load();
            PopUp2::Init();

            CraftManager craft;

            craft.SetTestMode();
            craft.Init();

            {
                auto reqList = NSStarmanLib::CraftSystem::GetObj()->GetCraftRequestList();
                Assert::AreEqual(true, reqList.empty());
            }

            eBattleState state;

            KeyBoard::ForTest_SetIsDownFirstFrame(DIK_RETURN);
            craft.Operate(&state);
            craft.Draw();

            KeyBoard::ForTest_SetIsDownFirstFrame(DIK_RETURN);
            craft.Operate(&state);
            craft.Draw();

            {
                auto reqList = NSStarmanLib::CraftSystem::GetObj()->GetCraftRequestList();
                Assert::AreEqual(true, reqList.empty());
            }

            Assert::AreEqual("クラフト用の素材が足りない", PopUp2::Get()->ForTest_GetText().c_str());

            Util::ReleaseWin_DX9_DI8();
        }

        // イカダをクラフト
        // クラフト用の素材が足りないケース2。
        // キューに追加されず、クラフト用の素材が足りない、というポップアップが表示されること
        TEST_METHOD(CraftRaftTest02)
        {
            Util::InitWin_DX9_DI8();
            KeyBoard::ForTest_SetTestMode();

            SaveManager::Get()->Load();
            auto storehouse = NSStarmanLib::StorehouseManager::Get()->GetStorehouse(1);
            storehouse->AddItem("細い木の幹");
            storehouse->AddItem("ツタ");
            PopUp2::Init();

            CraftManager craft;

            craft.SetTestMode();
            craft.Init();

            {
                auto reqList = NSStarmanLib::CraftSystem::GetObj()->GetCraftRequestList();
                Assert::AreEqual(true, reqList.empty());
            }

            eBattleState state;

            KeyBoard::ForTest_SetIsDownFirstFrame(DIK_RETURN);
            craft.Operate(&state);
            craft.Draw();

            KeyBoard::ForTest_SetIsDownFirstFrame(DIK_RETURN);
            craft.Operate(&state);
            craft.Draw();

            {
                auto reqList = NSStarmanLib::CraftSystem::GetObj()->GetCraftRequestList();
                Assert::AreEqual(true, reqList.empty());
            }

            Assert::AreEqual("クラフト用の素材が足りない", PopUp2::Get()->ForTest_GetText().c_str());

            Util::ReleaseWin_DX9_DI8();
        }

        // イカダをクラフト
        // クラフト用の素材が足りているケース。
        // キューに追加されること
        TEST_METHOD(CraftRaftTest03)
        {
            Util::InitWin_DX9_DI8();
            KeyBoard::ForTest_SetTestMode();

            SaveManager::Get()->Load();
            auto storehouse = NSStarmanLib::StorehouseManager::Get()->GetStorehouse(1);

            for (int i = 0; i < 100; ++i)
            {
                storehouse->AddItem("細い木の幹");
                storehouse->AddItem("ツタ");
            }

            PopUp2::Init();

            CraftManager craft;

            craft.SetTestMode();
            craft.Init();

            {
                auto reqList = NSStarmanLib::CraftSystem::GetObj()->GetCraftRequestList();
                Assert::AreEqual(true, reqList.empty());
            }

            eBattleState state;

            KeyBoard::ForTest_SetIsDownFirstFrame(DIK_RETURN);
            craft.Operate(&state);
            craft.Draw();

            KeyBoard::ForTest_SetIsDownFirstFrame(DIK_RETURN);
            craft.Operate(&state);
            craft.Draw();

            {
                auto reqList = NSStarmanLib::CraftSystem::GetObj()->GetCraftRequestList();
                Assert::AreEqual(false, reqList.empty());
                Assert::AreEqual("イカダ", reqList.front().GetName().c_str());
            }

            Assert::AreEqual("", PopUp2::Get()->ForTest_GetText().c_str());

            Util::ReleaseWin_DX9_DI8();
        }

        // イカダをクラフト
        // イカダが拠点にすでにある場合は、クラフトが開始されないこと
        // 警告を拠点のイカダを移動させるように促すポップアップが表示されること
        TEST_METHOD(CraftRaftTest04)
        {
            Util::InitWin_DX9_DI8();
            KeyBoard::ForTest_SetTestMode();

            SaveManager::Get()->Load();
            auto storehouse = NSStarmanLib::StorehouseManager::Get()->GetStorehouse(1);

            NSStarmanLib::Raft raft;
            raft.SetXYZ(-305.f, 11.f, 540.f);
            NSStarmanLib::Voyage::Get()->AddRaft(raft);

            for (int i = 0; i < 100; ++i)
            {
                storehouse->AddItem("細い木の幹");
                storehouse->AddItem("ツタ");
            }

            PopUp2::Init();

            CraftManager craft;

            craft.SetTestMode();
            craft.Init();

            {
                auto reqList = NSStarmanLib::CraftSystem::GetObj()->GetCraftRequestList();
                Assert::AreEqual(true, reqList.empty());
            }

            eBattleState state;

            KeyBoard::ForTest_SetIsDownFirstFrame(DIK_RETURN);
            craft.Operate(&state);
            craft.Draw();

            KeyBoard::ForTest_SetIsDownFirstFrame(DIK_RETURN);
            craft.Operate(&state);
            craft.Draw();

            {
                auto reqList = NSStarmanLib::CraftSystem::GetObj()->GetCraftRequestList();
                Assert::AreEqual(true, reqList.empty());
            }

            Assert::AreEqual("船着き場にイカダがある", PopUp2::Get()->ForTest_GetText().c_str());

            Util::ReleaseWin_DX9_DI8();
        }

        // イカダをクラフト
        // キューに追加され、２４時間後、クラフトが完了すること。
        // 倉庫に追加されないこと
        // イカダが拠点に配置されること
        TEST_METHOD(CraftRaftTest05)
        {

        }

        // イカダをクラフト
        // 強化値が熟練度によって自動で決まること？

        // 武器をクラフト
        // 強化値が熟練度によって自動で決まること？

        // 武器をクラフト
        //
        
        // 武器をクラフト
        //
    };
}
