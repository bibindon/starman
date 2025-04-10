#include <cassert>

#include "CppUnitTest.h"
#include "Util.h"
#include "../starman/SharedObj.h"
#include "../starman/MainWindow.h"
#include "../starman/QuestManager.h"
#include "../starman/SaveManager.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest2
{

TEST_MODULE_INITIALIZE(ModuleInit)
{
    // �e�X�g�̈�ԍŏ���save.bak�t�H���_��save�t�H���_�Ƀ��l�[������B
    //
    // �O��A�e�X�g�Ɏ��s�����ꍇ�Asave�t�H���_��save.bak�Ƀ��l�[�����ꂽ�܂܂�
    // save�t�H���_���Ȃ��A�Ƃ����󋵂��N���肦�邽�߁B
    int result1 = rename("res\\script\\save.bak", "res\\script\\save");
}

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

        auto hWnd = SharedObj::GetWindowHandle();
        BOOL result = DestroyWindow(hWnd);
        assert(result == 1);

        // DestroyWindow���s������A���b�Z�[�W��������������̂�҂�
        MSG msg = { 0 };
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        hInstance = (HINSTANCE)GetModuleHandle(0);
        BOOL result2 = UnregisterClass("�z�V�}��", hInstance);
        assert(result2 == 1);

        int result3 = rename("res\\script\\save.bak", "res\\script\\save");
        assert(result3 == 0);
    }

    // �P����public�֐����ĂԂ����̃e�X�g
    // MainLoop�֐�
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

                                keyboard.SetAltF4();
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

        hInstance = (HINSTANCE)GetModuleHandle(0);
        BOOL result2 = UnregisterClass("�z�V�}��", hInstance);
        assert(result2 == 1);

        int result3 = rename("res\\script\\save.bak", "res\\script\\save");
        assert(result3 == 0);
    }

    //--------------------------------------------
    // ���[�h
    // ���^�C�g��
    // ���I�[�v�j���O
    // ���t�B�[���h
    // ���N�G�X�g�P�J�n
    // ���N�G�X�g�P����
    // �̏��ŃQ�[����i�߂邽�Ƃ��ɃN���b�V�����Ȃ����Ƃ��m�F����e�X�g
    // �G���^�[�L�[�����x�������Ă��邾���œ��B�ł���
    //--------------------------------------------
    TEST_METHOD(MainWindowTest_TestMethod03)
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
                                // 1���G���^�[������������
                                for (int i = 0; i < 60 * 1; ++i)
                                {
                                    Sleep(1 * 1000); 
                                    keyboard.SetKeyDownFirst(DIK_RETURN);
                                }

                                // �G���^�[������������ƃN�G�X�g�P����������B
                                // �������N�G�X�g1�������������Ƃ�m����@���Ȃ��B
                                // �����ŁA�Z�[�u�����s���Z�[�u�f�[�^�ŃN�G�X�g�P�������ƂȂ��Ă��邩�m�F����B
                                SaveManager::Get()->Save();

                                keyboard.SetAltF4();
                            });

            sut.MainLoop();

            th1.join();
        }
        catch (...)
        {
            Util::DeleteDirectory("res\\script\\save");
            int result2 = rename("res\\script\\save.bak", "res\\script\\save");
            assert(result2 == 0);
            Assert::Fail();
        }

        std::string savedata;

        {
            std::ifstream ifs("res\\script\\save\\questSave.csv");
            ifs >> savedata;
        }

        Util::DeleteDirectory("res\\script\\save");

        // Target
        auto it = savedata.find("Q1,FINISHED");
        Assert::AreNotEqual(std::string::npos, it);

        hInstance = (HINSTANCE)GetModuleHandle(0);
        BOOL result2 = UnregisterClass("�z�V�}��", hInstance);
        assert(result2 == 1);

        int result3 = rename("res\\script\\save.bak", "res\\script\\save");
        assert(result3 == 0);
    }
};
}

