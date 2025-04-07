#include <cassert>

#include "CppUnitTest.h"
#include "Util.h"
#include "../starman/SharedObj.h"
#include "../starman/MainWindow.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest2
{
TEST_CLASS(MainWindowTest)
{
public:

    TEST_CLASS_INITIALIZE(Initialize)
    {
        // あったら消す
        Util::DeleteDirectory("res\\script\\save.bak");
    }

    TEST_CLASS_CLEANUP(CleanUp)
    {
    }

    // 単純にpublic関数を呼ぶだけのテスト
    // （コンストラクタ）
    TEST_METHOD(MainWindowTest_TestMethod01)
    {
        int result1 = rename("res\\script\\save", "res\\script\\save.bak");
        assert(result1 == 0);
        MockKeyBoard keyboard;
        auto hInstance = (HINSTANCE)GetModuleHandle(0);

        // Target
        try
        {
            MainWindow sut(hInstance, &keyboard);
        }
        catch (...)
        {
            int result2 = rename("res\\script\\save.bak", "res\\script\\save");
            assert(result2 == 0);
            Assert::Fail();
        }

        BOOL result = DestroyWindow(SharedObj::GetWindowHandle());
        assert(result == 1);

        hInstance = (HINSTANCE)GetModuleHandle(0);
        BOOL result2 = UnregisterClass("ホシマン", hInstance);
        assert(result2 == 1);

        int result3 = rename("res\\script\\save.bak", "res\\script\\save");
        assert(result3 == 0);
    }

    // 単純にpublic関数を呼ぶだけのテスト
    TEST_METHOD(MainWindowTest_TestMethod02)
    {
        int result1 = rename("res\\script\\save", "res\\script\\save.bak");
        assert(result1 == 0);

        MockKeyBoard keyboard;
        auto hInstance = (HINSTANCE)GetModuleHandle(0);

        try
        {
            MainWindow sut(hInstance, &keyboard);

            std::thread th1([&]
                            {
#ifdef _DEBUG
                                Sleep(30 * 1000);
#else
                                Sleep(10 * 1000);
#endif

#ifdef DEPLOY
                                keyboard.SetAltF4();
#else
                                keyboard.SetKeyDownFirst(DIK_Q);
#endif
                            });

            // Target
            sut.MainLoop();

            th1.join();
        }
        catch (...)
        {
            int result2 = rename("res\\script\\save.bak", "res\\script\\save");
            assert(result2 == 0);
            Assert::Fail();
        }

        BOOL result = DestroyWindow(SharedObj::GetWindowHandle());
        assert(result == 1);

        hInstance = (HINSTANCE)GetModuleHandle(0);
        BOOL result2 = UnregisterClass("ホシマン", hInstance);
        assert(result2 == 1);

        int result3 = rename("res\\script\\save.bak", "res\\script\\save");
        assert(result3 == 0);
    }
};
}

