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
    }

    TEST_CLASS_CLEANUP(CleanUp)
    {
    }

    // 単純にpublic関数を呼ぶだけのテスト
    // （コンストラクタ）
    TEST_METHOD(TestMethod01)
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
        }
        int result2 = rename("res\\script\\save.bak", "res\\script\\save");
        assert(result2 == 0);
    }

    // 単純にpublic関数を呼ぶだけのテスト
    TEST_METHOD(TestMethod02)
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
                                Sleep(30 * 1000);
                                keyboard.SetKeyDownFirst(DIK_Q);
                            });

            // Target
            sut.MainLoop();

            th1.join();
        }
        catch (...)
        {
        }
        int result2 = rename("res\\script\\save.bak", "res\\script\\save");
        assert(result2 == 0);
    }
};
}

