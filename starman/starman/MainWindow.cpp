#include "MainWindow.h"
#include <exception>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include "KeyBoard.h"
#include "BGM.h"
#include "SoundEffect.h"

_TCHAR gName[100] = _T("3D�I�u�W�F�N�g�`��T���v���v���O����");

// �E�B���h�E�v���V�[�W��
LRESULT CALLBACK WndProc(HWND hWnd, UINT mes, WPARAM wParam, LPARAM lParam)
{
    if (mes == WM_DESTROY)
    {
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, mes, wParam, lParam);
}

MainWindow::MainWindow(const HINSTANCE& hInstance)
{
    // �A�v���P�[�V�����̏�����
    WNDCLASSEX wcex = {
        sizeof(WNDCLASSEX),
        CS_HREDRAW | CS_VREDRAW,
        WndProc,
        0,
        0,
        hInstance,
        NULL,
        NULL,
        (HBRUSH)(COLOR_WINDOW + 1),
        NULL,
        (_TCHAR*)gName, NULL
    };

    if (!RegisterClassEx(&wcex))
    {
        throw std::exception("");
    }

    if (!(hWnd = CreateWindow(gName, gName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0,
        NULL, NULL, hInstance, NULL)))
    {
        throw std::exception("");
    }

    // Direct3D�̏�����
    if (!(g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
    {
        throw std::exception("");
    }

    D3DPRESENT_PARAMETERS d3dpp = {
        0,
        0,
        D3DFMT_UNKNOWN,
        0,
        D3DMULTISAMPLE_NONE,
        0,
        D3DSWAPEFFECT_DISCARD,
        NULL,
        TRUE,
        TRUE,
        D3DFMT_D24S8,
        0,
        D3DPRESENT_RATE_DEFAULT,
        D3DPRESENT_INTERVAL_DEFAULT
    };

    if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDev)))
    {
        if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDev)))
        {
            g_pD3D->Release();
            throw std::exception("");
        }
    }

    // �����̃I�u�W�F�N�g����
    if (FAILED(D3DXLoadMeshFromX(_T("Cube2.x"), D3DXMESH_MANAGED, g_pD3DDev, NULL, &pMaterials, NULL, &NumMaterials, &pMesh)))
    {
        g_pD3DDev->Release();
        g_pD3D->Release();
        throw std::exception("");
    }

    // ���I�u�W�F�N�g����
    if (FAILED(D3DXLoadMeshFromX(_T("Axis.x"), D3DXMESH_MANAGED, g_pD3DDev, NULL, &pAX_Materials, NULL, &AX_NumMaterials, &pAX_Mesh)))
    {
        pMesh->Release();
        g_pD3DDev->Release();
        g_pD3D->Release();
        throw std::exception("");
    }

    // ���C�g
    ZeroMemory(&light, sizeof(D3DLIGHT9));
    light.Direction = D3DXVECTOR3(-1, -20, 0);
    light.Type = D3DLIGHT_DIRECTIONAL;
    light.Diffuse.r = 1.0f;
    light.Diffuse.g = 1.0f;
    light.Diffuse.b = 1.0f;
    light.Ambient.r = 0.5f;
    light.Ambient.g = 0.5f;
    light.Ambient.b = 0.5f;
    light.Range = 1000;

    g_pD3DDev->SetLight(0, &light);
    g_pD3DDev->LightEnable(0, true);
    g_pD3DDev->SetRenderState(D3DRS_LIGHTING, TRUE);
    g_pD3DDev->SetRenderState(D3DRS_AMBIENT, 0x00808080);   // �A���r�G���g���C�g

    // directinput
    HRESULT ret = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&m_directInput, NULL);

    KeyBoard::Init(m_directInput, hWnd);
    BGM::initialize(hWnd);
    SoundEffect::initialize(hWnd);

    // �E�B���h�E�\��
    ShowWindow(hWnd, SW_SHOW);
}

MainWindow::~MainWindow()
{
    BGM::finalize();
    SoundEffect::finalize();
    pMesh->Release();
    pAX_Mesh->Release();
    g_pD3DDev->Release();
    g_pD3D->Release();
}

int MainWindow::MainLoop()
{
    // ���b�Z�[�W���[�v
    D3DXMATRIX World;          // �����̃��[���h�ϊ��s��
    D3DXMATRIX Rot_X, Rot_Y;   // �����̉�]�s��
    D3DXMATRIX Offset;         // �����̃I�t�Z�b�g�s��
    D3DXMATRIX AXWorld;   // �����[���h�ϊ��s��
    D3DXMatrixIdentity(&AXWorld);   // ���͌��_�Ɉʒu����̂ŒP�ʍs��
    D3DXMATRIX View;   // �r���[�ϊ��s��
    D3DXMATRIX Persp;   // �ˉe�ϊ��s��
    D3DCOLORVALUE MAmbient = { 0.2f, 0.2f, 0.2f, 1.0f };   // �}�e���A���̃A���r�G���g���˗�

    FLOAT Ang = 0.0f;   // ��]�p�x
    unsigned int i;
    do {
        Sleep(1);
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            DispatchMessage(&msg);
        }

        KeyBoard::Update();

        if (KeyBoard::IsDown(DIK_ESCAPE))
        {
            MessageBox(NULL, TEXT("aaa"), TEXT("bbb"), 0);
        }
        if (KeyBoard::IsDown(DIK_Q))
        {
            //BGM::get_ton()->load("res\\sound\\title.wav");
            SoundEffect::get_ton()->load("res\\sound\\damage01.wav");
            MessageBox(NULL, TEXT("aaa"), TEXT("bbb"), 0);
        }
        if (KeyBoard::IsDown(DIK_E))
        {
            //BGM::get_ton()->play();
            SoundEffect::get_ton()->play("res\\sound\\damage01.wav");
            MessageBox(NULL, TEXT("aaa"), TEXT("bbb"), 0);
        }

        g_pD3DDev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(40, 40, 80), 1.0f, 0);
        g_pD3DDev->BeginScene();

//        Ang += 1;

        ///////////////////////////
        // ���C�g
        ////////
            // ���C�g�̕�����]
        light.Direction.x = 20 * sinf(D3DXToRadian(Ang));
        light.Direction.z = 20 * cosf(D3DXToRadian(Ang));
        g_pD3DDev->SetLight(0, &light);

        ///////////////////////////
        // ������
        ////////
        D3DXMatrixRotationX(&Rot_X, D3DXToRadian(Ang * 0.15f));
        D3DXMatrixRotationY(&Rot_Y, D3DXToRadian(Ang * 0.2f) + 70.0f);
        D3DXMatrixTranslation(&Offset, 10, 10, 10);

        D3DXMatrixIdentity(&World);          // �P�ʍs��
        D3DXMatrixMultiply(&World, &World, &Offset);   // �܂��I�t�Z�b�g���āA
        D3DXMatrixMultiply(&World, &World, &Rot_X);    // X����]��
        D3DXMatrixMultiply(&World, &World, &Rot_Y);    // Y����]�����܂��B

        // �r���[�ϊ�
        // ���_�͌��_�Œ�ł����A�J�����̈ʒu�͓K���ł�
        D3DXVECTOR3 a = D3DXVECTOR3(47.5f * cosf(D3DXToRadian(Ang * 0.7f)), 70.2f * cosf(D3DXToRadian(Ang * 0.2f)), 56.7f * sinf(D3DXToRadian(Ang * 1.2f)));
        D3DXVECTOR3 b = D3DXVECTOR3(0, 0, 0);
        D3DXVECTOR3 c = D3DXVECTOR3(sinf(D3DXToRadian(Ang * 0.4f)), 1, 0);
        D3DXMatrixLookAtLH(
            &View,
            &a,
            &b,
            &c
        );

        // �ˉe�ϊ�
        D3DXMatrixPerspectiveFovLH(&Persp, D3DXToRadian(45), 640.0f / 480.0f, 1.0f, 10000.0f);

        // �s��o�^
        g_pD3DDev->SetTransform(D3DTS_WORLD, &World);
        g_pD3DDev->SetTransform(D3DTS_VIEW, &View);
        g_pD3DDev->SetTransform(D3DTS_PROJECTION, &Persp);

        // �����̕`��
        for (i = 0; i < NumMaterials; i++) {
            D3DXMATERIAL* mtrl = (D3DXMATERIAL*)(pMaterials->GetBufferPointer());
            mtrl->MatD3D.Ambient = MAmbient;
            g_pD3DDev->SetMaterial(&mtrl->MatD3D);
            pMesh->DrawSubset(i);
        };

        ///////////////////////////
        // ��
        ////////
        // ���[���h�ϊ��i�P�ʍs��̂݁j
        g_pD3DDev->SetTransform(D3DTS_WORLD, &AXWorld);

        for (i = 0; i < AX_NumMaterials; i++)
        {
            // ���`��
            D3DXMATERIAL* mtrl = ((D3DXMATERIAL*)(pAX_Materials->GetBufferPointer()) + i);
            mtrl->MatD3D.Ambient = MAmbient;
            g_pD3DDev->SetMaterial(&mtrl->MatD3D);
            pAX_Mesh->DrawSubset(i);
        };

        g_pD3DDev->EndScene();
        g_pD3DDev->Present(NULL, NULL, NULL, NULL);


    } while (msg.message != WM_QUIT);
    return 0;
}
