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
        
        // �P����public�֐����ĂԂ����̃e�X�g
        TEST_METHOD(TestMethod01)
        {
            // Target
            CraftManager craft;
        }
        
        // �P����public�֐����ĂԂ����̃e�X�g
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
        
        // �P����public�֐����ĂԂ����̃e�X�g
        TEST_METHOD(TestMethod03)
        {
            Util::InitWin_DX9_DI8();
            CraftManager craft;

            // Target
            craft.Finalize();

            Util::ReleaseWin_DX9_DI8();
        }
        
        // �P����public�֐����ĂԂ����̃e�X�g
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
        
        // �P����public�֐����ĂԂ����̃e�X�g
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

        // �P����public�֐����ĂԂ����̃e�X�g
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

        // �C�J�_���N���t�g
        // �N���t�g�p�̑f�ނ�����Ȃ��P�[�X�B
        // �L���[�ɒǉ����ꂸ�A�N���t�g�p�̑f�ނ�����Ȃ��A�Ƃ����|�b�v�A�b�v���\������邱��
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

            Assert::AreEqual("�f�ނ�����Ȃ�", font.GetShowText().c_str());

            craft.Finalize();
            Util::DestroyLibData();
            Util::ReleaseWin_DX9_DI8();
        }

        // �C�J�_���N���t�g
        // �N���t�g�p�̑f�ނ�����Ȃ��P�[�X2�B
        // �L���[�ɒǉ����ꂸ�A�N���t�g�p�̑f�ނ�����Ȃ��A�Ƃ����|�b�v�A�b�v���\������邱��
        TEST_METHOD(CraftRaftTest02)
        {
            Util::InitWin_DX9_DI8();

            SaveManager::Get()->Load();
            auto storehouse = NSStarmanLib::StorehouseManager::Get()->GetStorehouse(1);
            storehouse->AddItem("�ׂ��؂̊�");
            storehouse->AddItem("�c�^");

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

            Assert::AreEqual("�f�ނ�����Ȃ�", font.GetShowText().c_str());

            craft.Finalize();
            Util::DestroyLibData();
            Util::ReleaseWin_DX9_DI8();
        }

        // �C�J�_���N���t�g
        // �N���t�g�p�̑f�ނ�����Ă���P�[�X�B
        // �L���[�ɒǉ�����邱��
        TEST_METHOD(CraftRaftTest03)
        {
            Util::InitWin_DX9_DI8(true);

            SaveManager::Get()->Load();
            auto storehouse = NSStarmanLib::StorehouseManager::Get()->GetStorehouse(1);

            for (int i = 0; i < 100; ++i)
            {
                storehouse->AddItem("�ׂ��؂̊�");
                storehouse->AddItem("�c�^");
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
                Assert::AreEqual("�C�J�_", reqList.front().GetName().c_str());
            }

            Assert::AreEqual("", font.GetShowText().c_str());

            craft.Finalize();
            Util::DestroyLibData();
            Util::ReleaseWin_DX9_DI8();
        }

        // �C�J�_���N���t�g
        // �C�J�_�����_�ɂ��łɂ���ꍇ�́A�N���t�g���J�n����Ȃ�����
        // �x�������_�̃C�J�_���ړ�������悤�ɑ����|�b�v�A�b�v���\������邱��
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
                storehouse->AddItem("�ׂ��؂̊�");
                storehouse->AddItem("�c�^");
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

            Assert::AreEqual("�D������ɃC�J�_������", font.GetShowText().c_str());

            craft.Finalize();
            Util::DestroyLibData();
            Util::ReleaseWin_DX9_DI8();
        }

        // �C�J�_���N���t�g
        // �L���[�ɒǉ�����A�Q�S���Ԍ�A�N���t�g���������邱�ƁB
        // �q�ɂɒǉ�����Ȃ�����
        // �C�J�_�����_�ɔz�u����邱��
        TEST_METHOD(CraftRaftTest05)
        {

        }

        // �C�J�_���N���t�g
        // �����l���n���x�ɂ���Ď����Ō��܂邱�ƁH

        // ������N���t�g
        // �����l���n���x�ɂ���Ď����Ō��܂邱�ƁH

        // ������N���t�g
        //
        
        // ������N���t�g
        //
    };
}
