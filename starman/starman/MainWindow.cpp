#include "MainWindow.h"
#include <exception>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include "KeyBoard.h"
#include "Mouse.h"
#include "JoyStick.h"
#include "BGM.h"
#include "SoundEffect.h"

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
        TITLE.c_str(), NULL
    };

    if (!RegisterClassEx(&wcex))
    {
        throw std::exception("");
    }

    if (!(m_hWnd = CreateWindow(TITLE.c_str(), TITLE.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0,
        CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL)))
    {
        throw std::exception("");
    }

    // Direct3D�̏�����
    if (!(m_D3D = Direct3DCreate9(D3D_SDK_VERSION)))
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

    if (FAILED(m_D3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,
        D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_D3DDevice)))
    {
        if (FAILED(m_D3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,
            D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_D3DDevice)))
        {
            m_D3D->Release();
            throw std::exception("");
        }
    }

//    // �����̃I�u�W�F�N�g����
//    if (FAILED(D3DXLoadMeshFromX(_T("Cube2.x"), D3DXMESH_MANAGED, m_D3DDevice, NULL, &pMaterials, NULL, &NumMaterials, &pMesh)))
//    {
//        m_D3DDevice->Release();
//        m_D3D->Release();
//        throw std::exception("");
//    }
//
//    // ���I�u�W�F�N�g����
//    if (FAILED(D3DXLoadMeshFromX(_T("Axis.x"), D3DXMESH_MANAGED, m_D3DDevice, NULL, &pAX_Materials, NULL, &AX_NumMaterials, &pAX_Mesh)))
//    {
//        pMesh->Release();
//        m_D3DDevice->Release();
//        m_D3D->Release();
//        throw std::exception("");
//    }

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

    m_D3DDevice->SetLight(0, &light);
    m_D3DDevice->LightEnable(0, true);
    m_D3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
    m_D3DDevice->SetRenderState(D3DRS_AMBIENT, 0x00808080);   // �A���r�G���g���C�g

    // directinput
    HRESULT ret = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
        (LPVOID*)&m_directInput, NULL);

    KeyBoard::Init(m_directInput, m_hWnd);
    Mouse::Init(m_directInput, m_hWnd);
    JoyStick::Init(m_directInput, m_hWnd);
    BGM::initialize(m_hWnd);
    SoundEffect::initialize(m_hWnd);

    D3DXVECTOR3 b = D3DXVECTOR3(0, 0, 0);
    m_Mesh1 = new Mesh(m_D3DDevice, "res\\model\\tiger\\tiger.x", b, b, 10.0f);

    // �E�B���h�E�\��
    ShowWindow(m_hWnd, SW_SHOW);
}

MainWindow::~MainWindow()
{
    BGM::finalize();
    SoundEffect::finalize();
    m_D3DDevice->Release();
    m_D3D->Release();
}

int MainWindow::MainLoop()
{
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
        if (PeekMessage(&m_msg, NULL, 0, 0, PM_REMOVE))
        {
            DispatchMessage(&m_msg);
        }

        KeyBoard::Update();
        Mouse::Update();
        JoyStick::Update();

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
        if (Mouse::IsDownLeft())
        {
            MessageBox(NULL, TEXT("aaaaaaa"), TEXT("aaaaaaa"), 0);
        }

        m_D3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(40, 40, 80),
            1.0f, 0);
        m_D3DDevice->BeginScene();

//        Ang += 1;

        ///////////////////////////
        // ���C�g
        ////////
            // ���C�g�̕�����]
        light.Direction.x = 20 * sinf(D3DXToRadian(Ang));
        light.Direction.z = 20 * cosf(D3DXToRadian(Ang));
        m_D3DDevice->SetLight(0, &light);

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
        m_D3DDevice->SetTransform(D3DTS_WORLD, &World);
        m_D3DDevice->SetTransform(D3DTS_VIEW, &View);
        m_D3DDevice->SetTransform(D3DTS_PROJECTION, &Persp);

        // �����̕`��
        for (i = 0; i < NumMaterials; i++)
        {
            D3DXMATERIAL* mtrl = (D3DXMATERIAL*)(pMaterials->GetBufferPointer());
            mtrl->MatD3D.Ambient = MAmbient;
            m_D3DDevice->SetMaterial(&mtrl->MatD3D);
            pMesh->DrawSubset(i);
        };

        ///////////////////////////
        // ��
        ////////
        // ���[���h�ϊ��i�P�ʍs��̂݁j
        m_D3DDevice->SetTransform(D3DTS_WORLD, &AXWorld);

        for (i = 0; i < AX_NumMaterials; i++)
        {
            // ���`��
            D3DXMATERIAL* mtrl = ((D3DXMATERIAL*)(pAX_Materials->GetBufferPointer()) + i);
            mtrl->MatD3D.Ambient = MAmbient;
            m_D3DDevice->SetMaterial(&mtrl->MatD3D);
            pAX_Mesh->DrawSubset(i);
        };

        m_Mesh1->Render(View, Persp);

        m_D3DDevice->EndScene();
        m_D3DDevice->Present(NULL, NULL, NULL, NULL);


    } while (m_msg.message != WM_QUIT);
    return 0;
}
