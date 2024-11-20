#include "pch.h"
#include "CppUnitTest.h"
#include "../starman/Camera.h"

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
    TEST_CLASS(UnitTest)
    {
    public:
        TEST_METHOD(TestMethod1)
        {
            D3DXMATRIX mat { };
            const D3DXVECTOR3 UPWARD { 0.0f, 1.0f, 0.0f };
            D3DXVECTOR3 eyePos { 0.0f, 2.0f, -2.0f };
            D3DXVECTOR3 lookAtPos { 0.0f, 0.0f, 0.0f };
            D3DXMatrixLookAtLH(&mat,
                &eyePos,
                &lookAtPos,
                &UPWARD);
            D3DXMATRIX viewMat = Camera::GetViewMatrix();
            Assert::AreEqual((mat == viewMat), TRUE);
        }
        TEST_METHOD(TestMethod2)
        {
            D3DXMATRIX mat { };
            float viewAngle { D3DX_PI / 4 };
            D3DXMatrixPerspectiveFovLH(
                &mat,
                viewAngle,
                static_cast<float>(1920) / 1080,
                0.1f,
                500.0f);
            D3DXMATRIX projMat = Camera::GetProjMatrix();
            Assert::AreEqual((mat == projMat), TRUE);
        }
        TEST_METHOD(TestMethod3)
        {
            D3DXVECTOR3 eyePos { 0.0f, 2.0f, -2.0f };
            D3DXVECTOR3 lookAtPos { 0.0f, 0.0f, 0.0f };
            float viewAngle { D3DX_PI / 4 };
            Camera::SetLookAtPos(lookAtPos);
        }
};
}
