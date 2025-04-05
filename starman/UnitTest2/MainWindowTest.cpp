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
    TEST_METHOD(TestMethod01)
    {
        MockKeyBoard keyboard;
        auto hInstance = (HINSTANCE)GetModuleHandle(0);

        MainWindow sut(hInstance, &keyboard);
    }

    // �P����public�֐����ĂԂ����̃e�X�g
    TEST_METHOD(TestMethod02)
    {
        MockKeyBoard keyboard;
        auto hInstance = (HINSTANCE)GetModuleHandle(0);
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
};
}

