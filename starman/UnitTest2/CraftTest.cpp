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
        TEST_METHOD(CraftRaftTest)
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
            }

            Util::ReleaseWin_DX9_DI8();
        }
    };
}
