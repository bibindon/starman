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

    // �P����public�֐����ĂԂ����̃e�X�g
    // �i�R���X�g���N�^�j
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
        int result2 = rename("res\\script\\save.bak", "res\\script\\save");
        assert(result2 == 0);
    }

    // �P����public�֐����ĂԂ����̃e�X�g
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
                                keyboard.SetKeyDownFirst(DIK_Q);
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
        int result2 = rename("res\\script\\save.bak", "res\\script\\save");
        assert(result2 == 0);
    }
};
}

