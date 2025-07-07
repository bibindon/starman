#include "CppUnitTest.h"
#include "Util.h"
#include "../starman/CraftManager.h"
#include "../starman/MainWindow.h"
#include "../starman/SaveManager.h"
#include "../starman/KeyBoard.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/CraftSystem.h"
#include "../../StarmanLib/StarmanLib/StarmanLib/CraftInfoManager.h"
#include "../starman/PopUp2.h"
#include "../starman/SharedObj.h"

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
        TEST_METHOD(CraftTest_TestMethod01)
        {
            // Target
            CraftManager craft;
        }
        
        // 単純にpublic関数を呼ぶだけのテスト
        TEST_METHOD(CraftTest_TestMethod02)
        {
            Util::InitWin_DX9_DI8();
            SaveManager::Get()->LoadOrigin();
            CraftManager craft;

            // Target
            craft.Init();

            craft.Finalize();
            Util::DestroyLibData();
            Util::ReleaseWin_DX9_DI8();
        }
        
        // 単純にpublic関数を呼ぶだけのテスト
        TEST_METHOD(CraftTest_TestMethod03)
        {
            Util::InitWin_DX9_DI8();
            CraftManager craft;

            // Target
            craft.Finalize();

            Util::ReleaseWin_DX9_DI8();
        }
        
        // 単純にpublic関数を呼ぶだけのテスト
        TEST_METHOD(CraftTest_TestMethod04)
        {
            Util::InitWin_DX9_DI8();
            CraftManager craft;
            eBattleState state;

            MockKeyBoard* keyboard = NEW MockKeyBoard();
            SharedObj::SetKeyBoard(keyboard);

            // Target
            craft.Operate(&state);

            craft.Finalize();
            Util::ReleaseWin_DX9_DI8();
        }
        
        // 単純にpublic関数を呼ぶだけのテスト
        TEST_METHOD(CraftTest_TestMethod05)
        {
            Util::InitWin_DX9_DI8(true);
            SaveManager::Get()->LoadOrigin();
            CraftManager craft;
            craft.Init();

            // Target
            SharedObj::GetD3DDevice()->Clear(0,
                                             NULL,
                                             D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                                             D3DCOLOR_XRGB(40, 40, 80),
                                             1.0f,
                                             0);

            SharedObj::GetD3DDevice()->BeginScene();

            // Target
            craft.Draw();

            SharedObj::GetD3DDevice()->EndScene();
            SharedObj::GetD3DDevice()->Present(NULL, NULL, NULL, NULL);

            craft.Finalize();
            Util::DestroyLibData();
            Util::ReleaseWin_DX9_DI8();
        }

        // 単純にpublic関数を呼ぶだけのテスト
        TEST_METHOD(CraftTest_TestMethod06)
        {
            Util::InitWin_DX9_DI8(true);
            SaveManager::Get()->LoadOrigin();
            CraftManager craft;

            // Target
            craft.Build();

            craft.Finalize();
            Util::DestroyLibData();
            Util::ReleaseWin_DX9_DI8();
        }

        // イカダをクラフト
        // クラフト用の素材が足りないケース。
        // キューに追加されず、クラフト用の素材が足りない、というポップアップが表示されること
        TEST_METHOD(CraftTest_CraftRaftTest01)
        {
            Util::InitWin_DX9_DI8(true);

            SaveManager::Get()->LoadOrigin();
            MockPopUpFont font;
            PopUp2::Init(&font);

            MockKeyBoard* keyboard = NEW MockKeyBoard();
            SharedObj::SetKeyBoard(keyboard);

            CraftManager craft;

            craft.Init();

            {
                auto reqList = NSStarmanLib::CraftSystem::GetObj()->GetCraftRequestList();
                Assert::AreEqual(true, reqList.empty());
            }

            eBattleState state = eBattleState::CRAFT;

            keyboard->SetKeyDownFirst(DIK_RETURN);
            keyboard->Update();

            craft.Operate(&state);

            SharedObj::GetD3DDevice()->Clear(0,
                                             NULL,
                                             D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                                             D3DCOLOR_XRGB(40, 40, 80),
                                             1.0f,
                                             0);

            SharedObj::GetD3DDevice()->BeginScene();

            // Target
            craft.Draw();

            SharedObj::GetD3DDevice()->EndScene();
            SharedObj::GetD3DDevice()->Present(NULL, NULL, NULL, NULL);

            keyboard->SetKeyDownFirst(DIK_RETURN);
            keyboard->Update();

            craft.Operate(&state);
            SharedObj::GetD3DDevice()->Clear(0,
                                             NULL,
                                             D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                                             D3DCOLOR_XRGB(40, 40, 80),
                                             1.0f,
                                             0);

            SharedObj::GetD3DDevice()->BeginScene();

            // Target
            craft.Draw();

            SharedObj::GetD3DDevice()->EndScene();
            SharedObj::GetD3DDevice()->Present(NULL, NULL, NULL, NULL);
            PopUp2::Get()->Render();

            // Target
            {
                auto reqList = NSStarmanLib::CraftSystem::GetObj()->GetCraftRequestList();
                Assert::AreEqual(true, reqList.empty());
            }

            Assert::AreEqual(_T("素材が足りない"), font.GetShowText().c_str());

            craft.Finalize();
            Util::DestroyLibData();
            Util::ReleaseWin_DX9_DI8();
        }

        // イカダをクラフト
        // クラフト用の素材が足りないケース2。
        // キューに追加されず、クラフト用の素材が足りない、というポップアップが表示されること
        TEST_METHOD(CraftTest_CraftRaftTest02)
        {
            Util::InitWin_DX9_DI8();

            SaveManager::Get()->LoadOrigin();
            auto storehouse = NSStarmanLib::StorehouseManager::Get()->GetStorehouse(1);
            storehouse->AddItem(_T("trunk"));
            storehouse->AddItem(_T("tsuta"));

            MockPopUpFont font;
            PopUp2::Init(&font);

            MockKeyBoard* keyboard = NEW MockKeyBoard();
            SharedObj::SetKeyBoard(keyboard);

            CraftManager craft;

            craft.Init();

            {
                auto reqList = NSStarmanLib::CraftSystem::GetObj()->GetCraftRequestList();
                Assert::AreEqual(true, reqList.empty());
            }

            eBattleState state = eBattleState::CRAFT;

            keyboard->SetKeyDownFirst(DIK_RETURN);
            keyboard->Update();

            craft.Operate(&state);

            SharedObj::GetD3DDevice()->Clear(0,
                                             NULL,
                                             D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                                             D3DCOLOR_XRGB(40, 40, 80),
                                             1.0f,
                                             0);

            SharedObj::GetD3DDevice()->BeginScene();

            // Target
            craft.Draw();

            SharedObj::GetD3DDevice()->EndScene();
            SharedObj::GetD3DDevice()->Present(NULL, NULL, NULL, NULL);

            keyboard->SetKeyDownFirst(DIK_RETURN);
            keyboard->Update();

            craft.Operate(&state);
            SharedObj::GetD3DDevice()->Clear(0,
                                             NULL,
                                             D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                                             D3DCOLOR_XRGB(40, 40, 80),
                                             1.0f,
                                             0);

            SharedObj::GetD3DDevice()->BeginScene();

            // Target
            craft.Draw();

            SharedObj::GetD3DDevice()->EndScene();
            SharedObj::GetD3DDevice()->Present(NULL, NULL, NULL, NULL);
            PopUp2::Get()->Render();

            {
                auto reqList = NSStarmanLib::CraftSystem::GetObj()->GetCraftRequestList();
                Assert::AreEqual(true, reqList.empty());
            }

            Assert::AreEqual(_T("素材が足りない"), font.GetShowText().c_str());

            craft.Finalize();
            Util::DestroyLibData();
            Util::ReleaseWin_DX9_DI8();
        }

        // イカダをクラフト
        // クラフト用の素材が足りているケース。
        // キューに追加されること
        TEST_METHOD(CraftTest_CraftRaftTest03)
        {
            Util::InitWin_DX9_DI8(true);

            SaveManager::Get()->LoadOrigin();
            auto storehouse = NSStarmanLib::StorehouseManager::Get()->GetStorehouse(1);

            for (int i = 0; i < 100; ++i)
            {
                storehouse->AddItem(_T("trunk"));
                storehouse->AddItem(_T("tsuta"));
            }

            MockPopUpFont font;
            PopUp2::Init(&font);

            MockKeyBoard* keyboard = NEW MockKeyBoard();
            SharedObj::SetKeyBoard(keyboard);

            CraftManager craft;

            craft.Init();

            {
                auto reqList = NSStarmanLib::CraftSystem::GetObj()->GetCraftRequestList();
                Assert::AreEqual(true, reqList.empty());
            }

            eBattleState state = eBattleState::CRAFT;

            keyboard->SetKeyDownFirst(DIK_RETURN);
            keyboard->Update();

            craft.Operate(&state);
            SharedObj::GetD3DDevice()->Clear(0,
                                             NULL,
                                             D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                                             D3DCOLOR_XRGB(40, 40, 80),
                                             1.0f,
                                             0);

            SharedObj::GetD3DDevice()->BeginScene();

            // Target
            craft.Draw();

            SharedObj::GetD3DDevice()->EndScene();
            SharedObj::GetD3DDevice()->Present(NULL, NULL, NULL, NULL);

            keyboard->SetKeyDownFirst(DIK_RETURN);
            keyboard->Update();

            craft.Operate(&state);
            SharedObj::GetD3DDevice()->Clear(0,
                                             NULL,
                                             D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                                             D3DCOLOR_XRGB(40, 40, 80),
                                             1.0f,
                                             0);

            SharedObj::GetD3DDevice()->BeginScene();

            // Target
            craft.Draw();

            SharedObj::GetD3DDevice()->EndScene();
            SharedObj::GetD3DDevice()->Present(NULL, NULL, NULL, NULL);
            PopUp2::Get()->Render();

            {
                auto reqList = NSStarmanLib::CraftSystem::GetObj()->GetCraftRequestList();
                Assert::AreEqual(false, reqList.empty());
                Assert::AreEqual(_T("raft"), reqList.front().GetId().c_str());
            }

            Assert::AreEqual(_T(""), font.GetShowText().c_str());

            craft.Finalize();
            Util::DestroyLibData();
            Util::ReleaseWin_DX9_DI8();
        }

        // イカダをクラフト
        // イカダが拠点にすでにある場合は、クラフトが開始されないこと
        // 警告を拠点のイカダを移動させるように促すポップアップが表示されること
        TEST_METHOD(CraftTest_CraftRaftTest04)
        {
            NSStarmanLib::CraftSystem::Destroy();
            Util::InitWin_DX9_DI8();

            SaveManager::Get()->LoadOrigin();
            auto storehouse = NSStarmanLib::StorehouseManager::Get()->GetStorehouse(1);

            NSStarmanLib::Raft raft;
            raft.SetXYZ(-4.f, 458.f, -711.f);
            NSStarmanLib::Voyage::Get()->AddRaft(raft);

            for (int i = 0; i < 50; ++i)
            {
                storehouse->AddItem(_T("trunk"));
                storehouse->AddItem(_T("tsuta"));
            }

            MockPopUpFont font;
            PopUp2::Init(&font);

            MockKeyBoard* keyboard = NEW MockKeyBoard();
            SharedObj::SetKeyBoard(keyboard);

            CraftManager craft;

            craft.Init();

            {
                auto reqList = NSStarmanLib::CraftSystem::GetObj()->GetCraftRequestList();
                Assert::AreEqual(true, reqList.empty());
            }

            eBattleState state = eBattleState::CRAFT;

            keyboard->SetKeyDownFirst(DIK_RETURN);
            keyboard->Update();

            craft.Operate(&state);
            SharedObj::GetD3DDevice()->Clear(0,
                                             NULL,
                                             D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                                             D3DCOLOR_XRGB(40, 40, 80),
                                             1.0f,
                                             0);

            SharedObj::GetD3DDevice()->BeginScene();

            // Target
            craft.Draw();

            SharedObj::GetD3DDevice()->EndScene();
            SharedObj::GetD3DDevice()->Present(NULL, NULL, NULL, NULL);

            keyboard->SetKeyDownFirst(DIK_RETURN);
            keyboard->Update();

            craft.Operate(&state);
            craft.Draw();
            PopUp2::Get()->Render();

            {
                auto reqList = NSStarmanLib::CraftSystem::GetObj()->GetCraftRequestList();
                Assert::AreEqual(true, reqList.empty());
            }

            craft.Finalize();
            Util::DestroyLibData();
            Util::ReleaseWin_DX9_DI8();
        }

        // イカダをクラフト
        // キューに追加され、２４時間後、クラフトが完了すること。
        // 倉庫に追加されないこと
        // イカダが拠点に配置されること
        TEST_METHOD(CraftTest_CraftRaftTest05)
        {
            NSStarmanLib::CraftSystem::Destroy();
            Util::InitWin_DX9_DI8();

            SaveManager::Get()->LoadOrigin();
            auto storehouse = NSStarmanLib::StorehouseManager::Get()->GetStorehouse(1);

            for (int i = 0; i < 50; ++i)
            {
                storehouse->AddItem(_T("trunk"));
                storehouse->AddItem(_T("tsuta"));
            }

            MockKeyBoard* keyboard = NEW MockKeyBoard();
            SharedObj::SetKeyBoard(keyboard);

            CraftManager craft;

            craft.Init();

            {
                auto reqList = NSStarmanLib::CraftSystem::GetObj()->GetCraftRequestList();
                Assert::AreEqual(true, reqList.empty());

                auto raftList = NSStarmanLib::Voyage::Get()->GetRaftList();
                Assert::AreEqual(true, raftList.empty());
            }

            eBattleState state = eBattleState::CRAFT;

            keyboard->SetKeyDownFirst(DIK_RETURN);
            keyboard->Update();

            craft.Operate(&state);
            SharedObj::GetD3DDevice()->Clear(0,
                                             NULL,
                                             D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                                             D3DCOLOR_XRGB(40, 40, 80),
                                             1.0f,
                                             0);

            SharedObj::GetD3DDevice()->BeginScene();

            // Target
            craft.Draw();

            SharedObj::GetD3DDevice()->EndScene();
            SharedObj::GetD3DDevice()->Present(NULL, NULL, NULL, NULL);

            keyboard->SetKeyDownFirst(DIK_RETURN);
            keyboard->Update();

            craft.Operate(&state);
            craft.Draw();

            {
                auto reqList = NSStarmanLib::CraftSystem::GetObj()->GetCraftRequestList();
                Assert::AreEqual(false, reqList.empty());
            }

            // Updateを60回実行しないと更新処理が走らないので60回呼ぶ
            for (int i = 0; i < 60; ++i)
            {
                craft.Update();
            }

            auto datetime = NSStarmanLib::PowereggDateTime::GetObj();
            datetime->IncreaseDateTime(0, 1, 1, 0, 0);

            // Updateを60回実行しないと更新処理が走らないので60回呼ぶ
            for (int i = 0; i < 60; ++i)
            {
                craft.Update();
            }

            {
                auto reqList = NSStarmanLib::CraftSystem::GetObj()->GetCraftRequestList();
                Assert::AreEqual(true, reqList.empty());

                auto raftList = NSStarmanLib::Voyage::Get()->GetRaftList();
                Assert::AreEqual(true, raftList.at(0).GetLevel() == -1);
                Assert::AreEqual(true, raftList.at(0).GetDurability() == 100);
            }

            craft.Finalize();
            Util::DestroyLibData();
            Util::ReleaseWin_DX9_DI8();
        }

        // イカダをクラフト
        // 強化値+1のイカダがクラフトできることが熟練度によって自動で決まること？
        TEST_METHOD(CraftTest_CraftRaftTest06)
        {
            NSStarmanLib::CraftSystem::Destroy();
            NSStarmanLib::Voyage::Destroy();
            Util::InitWin_DX9_DI8();

            SaveManager::Get()->LoadOrigin();
            auto storehouse = NSStarmanLib::StorehouseManager::Get()->GetStorehouse(1);

            for (int i = 0; i < 50; ++i)
            {
                storehouse->AddItem(_T("trunk"));
                storehouse->AddItem(_T("tsuta"));
                storehouse->AddItem(_T("bui"));
            }

            MockKeyBoard* keyboard = NEW MockKeyBoard();
            SharedObj::SetKeyBoard(keyboard);

            CraftManager craft;

            craft.Init();
            NSStarmanLib::CraftSystem::GetObj()->SetCraftsmanSkill(L"raft", 1);

            {
                auto reqList = NSStarmanLib::CraftSystem::GetObj()->GetCraftRequestList();
                Assert::AreEqual(true, reqList.empty());

                auto raftList = NSStarmanLib::Voyage::Get()->GetRaftList();
                Assert::AreEqual(true, raftList.empty());
            }

            eBattleState state = eBattleState::CRAFT;

            keyboard->SetKeyDownFirst(DIK_RETURN);
            keyboard->Update();

            craft.Operate(&state);
            SharedObj::GetD3DDevice()->Clear(0,
                                             NULL,
                                             D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                                             D3DCOLOR_XRGB(40, 40, 80),
                                             1.0f,
                                             0);

            SharedObj::GetD3DDevice()->BeginScene();

            // Target
            craft.Draw();

            SharedObj::GetD3DDevice()->EndScene();
            SharedObj::GetD3DDevice()->Present(NULL, NULL, NULL, NULL);

            keyboard->SetKeyDownFirst(DIK_RETURN);
            keyboard->Update();

            craft.Operate(&state);
            craft.Draw();

            {
                auto reqList = NSStarmanLib::CraftSystem::GetObj()->GetCraftRequestList();
                Assert::AreEqual(false, reqList.empty());
            }

            // Updateを60回実行しないと更新処理が走らないので60回呼ぶ
            for (int i = 0; i < 60; ++i)
            {
                craft.Update();
            }

            auto datetime = NSStarmanLib::PowereggDateTime::GetObj();
            datetime->IncreaseDateTime(0, 1, 1, 0, 0);

            // Updateを60回実行しないと更新処理が走らないので60回呼ぶ
            for (int i = 0; i < 60; ++i)
            {
                craft.Update();
            }

            {
                auto reqList = NSStarmanLib::CraftSystem::GetObj()->GetCraftRequestList();
                Assert::AreEqual(true, reqList.empty());

                auto raftList = NSStarmanLib::Voyage::Get()->GetRaftList();
                Assert::AreEqual(true, raftList.at(0).GetLevel() == 1);
                Assert::AreEqual(true, raftList.at(0).GetDurability() == 200);
            }

            craft.Finalize();
            Util::DestroyLibData();
            Util::ReleaseWin_DX9_DI8();
        }

        // 武器をクラフト
        // 強化値が熟練度によって自動で決まること？

        // 武器をクラフト
        //
        
        // 武器をクラフト
        //
    };
}
