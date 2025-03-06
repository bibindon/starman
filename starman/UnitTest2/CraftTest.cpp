#include "CppUnitTest.h"
#include "../starman/CraftManager.h"
#include "../starman/MainWindow.h"
#include "Util.h"

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
        
        TEST_METHOD(TestMethod01)
        {
            CraftManager craft;
        }
        
        TEST_METHOD(TestMethod02)
        {
            Util::InitWin_DX9_DI8();
            CraftManager craft;

            craft.Init();

            Util::ReleaseWin_DX9_DI8();
        }
    };
}
