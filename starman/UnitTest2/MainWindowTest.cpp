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
    bool exist1 = Util::IsDirectory("res\\script\\save");
    bool exist2 = Util::IsDirectory("res\\script\\save.bak");

    if (exist1 && exist2)
    {
        Util::DeleteDirectory("res\\script\\save");
        int result1 = rename("res\\script\\save.bak", "res\\script\\save");
    }
    else if (!exist1 && exist2)
    {
        int result1 = rename("res\\script\\save.bak", "res\\script\\save");
    }
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

    // 単純にpublic関数を呼ぶだけのテスト
    // （コンストラクタ）
    TEST_METHOD(MainWindowTest_TestMethod01)
    {
        Sleep(1000);

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

        // DestroyWindowを行った後、メッセージ処理が完了するのを待つ
        MSG msg = { 0 };
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        hInstance = (HINSTANCE)GetModuleHandle(0);
        BOOL result2 = UnregisterClass("ホシマン", hInstance);
        assert(result2 == 1);

        int result3 = rename("res\\script\\save.bak", "res\\script\\save");
        assert(result3 == 0);
    }

    // 単純にpublic関数を呼ぶだけのテスト
    // MainLoop関数
    TEST_METHOD(MainWindowTest_TestMethod02)
    {
        Sleep(1000);

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
                                Sleep(20 * 1000);
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
        BOOL result2 = UnregisterClass("ホシマン", hInstance);
        assert(result2 == 1);

        bool exist1 = Util::IsDirectory("res\\script\\save");
        bool exist2 = Util::IsDirectory("res\\script\\save.bak");

        if (exist1 && exist2)
        {
            Util::DeleteDirectory("res\\script\\save");
            int result1 = rename("res\\script\\save.bak", "res\\script\\save");
            assert(result1 == 0);
        }
        else if (!exist1 && exist2)
        {
            int result1 = rename("res\\script\\save.bak", "res\\script\\save");
            assert(result1 == 0);
        }
    }

    //--------------------------------------------
    // ロード
    // →タイトル
    // →オープニング
    // →フィールド
    // →クエスト１開始
    // →クエスト１完了
    // の順でゲームを進めるたときにクラッシュしないことを確認するテスト
    // エンターキーを何度も押しているだけで到達できる
    //--------------------------------------------
    TEST_METHOD(MainWindowTest_TestMethod03)
    {
        Sleep(1000);

        int result1 = rename("res\\script\\save", "res\\script\\save.bak");
        assert(result1 == 0);

        MockKeyBoard keyboard;
        auto hInstance = (HINSTANCE)GetModuleHandle(0);

        try
        {
            MainWindow sut(hInstance, &keyboard);

            std::thread th1([&]
                            {
                                // 1分エンターを押し続ける
                                for (int i = 0; i < 60; ++i)
                                {
                                    Sleep(1000 * 1); 
                                    keyboard.SetKeyDownFirst(DIK_RETURN);
                                }

                                // エンターを押し続けるとクエスト１が完了する。
                                // しかしクエスト1が完了したことを知る方法がない。
                                // そこで、セーブを実行しセーブデータでクエスト１が完了となっているか確認する。
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

        Sleep(1000);
        BOOL result4 = Util::DeleteDirectory("res\\script\\save");
        assert(result4 == TRUE);

        // Target
        auto it = savedata.find("Q1,FINISHED");
        Assert::AreNotEqual(std::string::npos, it);

        hInstance = (HINSTANCE)GetModuleHandle(0);
        BOOL result2 = UnregisterClass("ホシマン", hInstance);
        assert(result2 == 1);

        int result3 = rename("res\\script\\save.bak", "res\\script\\save");
        assert(result3 == 0);
    }
};
}

