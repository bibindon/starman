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
        TEST_METHOD(TestMethod01)
        {
            // Target
            CraftManager craft;
        }
        
        // 単純にpublic関数を呼ぶだけのテスト
        TEST_METHOD(TestMethod02)
        {
            Util::InitWin_DX9_DI8();
            SaveManager::Get()->Load();
            CraftManager craft;

            // Target
            craft.Init();

            craft.Finalize();
            Util::DestroyLibData();
            Util::ReleaseWin_DX9_DI8();
        }
        
        // 単純にpublic関数を呼ぶだけのテスト
        TEST_METHOD(TestMethod03)
        {
            Util::InitWin_DX9_DI8();
            CraftManager craft;

            // Target
            craft.Finalize();

            Util::ReleaseWin_DX9_DI8();
        }
        
        // 単純にpublic関数を呼ぶだけのテスト
        TEST_METHOD(TestMethod04)
        {
            Util::InitWin_DX9_DI8();
            CraftManager craft;
            eBattleState state;

            // Target
            craft.Operate(&state);

            craft.Finalize();
            Util::ReleaseWin_DX9_DI8();
        }
        
        // 単純にpublic関数を呼ぶだけのテスト
        TEST_METHOD(TestMethod05)
        {
            Util::InitWin_DX9_DI8(true);
            SaveManager::Get()->Load();
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
        TEST_METHOD(TestMethod06)
        {
            Util::InitWin_DX9_DI8(true);
            SaveManager::Get()->Load();
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
        TEST_METHOD(CraftRaftTest01)
        {
            Util::InitWin_DX9_DI8(true);

            SaveManager::Get()->Load();
            MockPopUpFont font;
            PopUp2::Init(&font);

            MockKeyBoard keyboard;
            SharedObj::SetKeyBoard(&keyboard);

            CraftManager craft;

            craft.Init();

            {
                auto reqList = NSStarmanLib::CraftSystem::GetObj()->GetCraftRequestList();
                Assert::AreEqual(true, reqList.empty());
            }

            eBattleState state = eBattleState::CRAFT;

            keyboard.ClearAll();
            keyboard.SetKeyDownFirst(DIK_RETURN);

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

            keyboard.ClearAll();
            keyboard.SetKeyDownFirst(DIK_RETURN);

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

            Assert::AreEqual("素材が足りない", font.GetShowText().c_str());

            craft.Finalize();
            Util::DestroyLibData();
            Util::ReleaseWin_DX9_DI8();
        }

        // イカダをクラフト
        // クラフト用の素材が足りないケース2。
        // キューに追加されず、クラフト用の素材が足りない、というポップアップが表示されること
        TEST_METHOD(CraftRaftTest02)
        {
            Util::InitWin_DX9_DI8();

            SaveManager::Get()->Load();
            auto storehouse = NSStarmanLib::StorehouseManager::Get()->GetStorehouse(1);
            storehouse->AddItem("細い木の幹");
            storehouse->AddItem("ツタ");

            MockPopUpFont font;
            PopUp2::Init(&font);

            MockKeyBoard keyboard;
            SharedObj::SetKeyBoard(&keyboard);

            CraftManager craft;

            craft.Init();

            {
                auto reqList = NSStarmanLib::CraftSystem::GetObj()->GetCraftRequestList();
                Assert::AreEqual(true, reqList.empty());
            }

            eBattleState state = eBattleState::CRAFT;

            keyboard.ClearAll();
            keyboard.SetKeyDownFirst(DIK_RETURN);

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

            keyboard.ClearAll();
            keyboard.SetKeyDownFirst(DIK_RETURN);

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

            Assert::AreEqual("素材が足りない", font.GetShowText().c_str());

            craft.Finalize();
            Util::DestroyLibData();
            Util::ReleaseWin_DX9_DI8();
        }

        // イカダをクラフト
        // クラフト用の素材が足りているケース。
        // キューに追加されること
        TEST_METHOD(CraftRaftTest03)
        {
            Util::InitWin_DX9_DI8(true);

            SaveManager::Get()->Load();
            auto storehouse = NSStarmanLib::StorehouseManager::Get()->GetStorehouse(1);

            for (int i = 0; i < 100; ++i)
            {
                storehouse->AddItem("細い木の幹");
                storehouse->AddItem("ツタ");
            }

            MockPopUpFont font;
            PopUp2::Init(&font);

            MockKeyBoard keyboard;
            SharedObj::SetKeyBoard(&keyboard);

            CraftManager craft;

            craft.Init();

            {
                auto reqList = NSStarmanLib::CraftSystem::GetObj()->GetCraftRequestList();
                Assert::AreEqual(true, reqList.empty());
            }

            eBattleState state = eBattleState::CRAFT;

            keyboard.ClearAll();
            keyboard.SetKeyDownFirst(DIK_RETURN);

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

            keyboard.ClearAll();
            keyboard.SetKeyDownFirst(DIK_RETURN);

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
                Assert::AreEqual("イカダ", reqList.front().GetName().c_str());
            }

            Assert::AreEqual("", font.GetShowText().c_str());

            craft.Finalize();
            Util::DestroyLibData();
            Util::ReleaseWin_DX9_DI8();
        }

        // イカダをクラフト
        // イカダが拠点にすでにある場合は、クラフトが開始されないこと
        // 警告を拠点のイカダを移動させるように促すポップアップが表示されること
        TEST_METHOD(CraftRaftTest04)
        {
            NSStarmanLib::CraftSystem::Destroy();
            Util::InitWin_DX9_DI8();

            SaveManager::Get()->Load();
            auto storehouse = NSStarmanLib::StorehouseManager::Get()->GetStorehouse(1);

            NSStarmanLib::Raft raft;
            raft.SetXYZ(-317.f, 11.f, 540.f);
            NSStarmanLib::Voyage::Get()->AddRaft(raft);

            for (int i = 0; i < 100; ++i)
            {
                storehouse->AddItem("細い木の幹");
                storehouse->AddItem("ツタ");
            }

            MockPopUpFont font;
            PopUp2::Init(&font);

            MockKeyBoard keyboard;
            SharedObj::SetKeyBoard(&keyboard);

            CraftManager craft;

            craft.Init();

            {
                auto reqList = NSStarmanLib::CraftSystem::GetObj()->GetCraftRequestList();
                Assert::AreEqual(true, reqList.empty());
            }

            eBattleState state = eBattleState::CRAFT;

            keyboard.ClearAll();
            keyboard.SetKeyDownFirst(DIK_RETURN);

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

            keyboard.ClearAll();
            keyboard.SetKeyDownFirst(DIK_RETURN);

            craft.Operate(&state);
            craft.Draw();
            PopUp2::Get()->Render();

            {
                auto reqList = NSStarmanLib::CraftSystem::GetObj()->GetCraftRequestList();
                Assert::AreEqual(true, reqList.empty());
            }

            Assert::AreEqual("船着き場にイカダがある", font.GetShowText().c_str());

            craft.Finalize();
            Util::DestroyLibData();
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
