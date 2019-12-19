#include <iostream>
#include <Windows.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

using namespace std;
using namespace DirectX;
using namespace DirectX::PackedVector;

ostream& XM_CALLCONV operator<<(ostream& os, FXMVECTOR V)
{
	XMFLOAT4 dest;
	XMStoreFloat4(&dest, V);

	os << "(" << dest.x << ", " << dest.y << ", " << dest.z << ", " << dest.w << ")";
	return os;
}



/* XMVECTOR 연습하기 1 */
//int main()
//{
//	cout.setf(ios_base::boolalpha);
//
//	/* DirectXMath 라이브러리가 현재 플랫폼에서 지원되는지를 확인합니다. */
//	if (!XMVerifyCPUSupport())
//	{
//		cout << "DirectXMath를 지원하지 않습니다.\n";
//		return 0;
//	}
//
//	/* 벡터의 값 설정 */
//	XMVECTOR n = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
//	XMVECTOR u = XMVectorSet(1.0f, 2.0f, 3.0f, 0.0f);
//	XMVECTOR v = XMVectorSet(-2.0f, 1.0f, -3.0f, 0.0f);
//	XMVECTOR w = XMVectorSet(0.707f, 0.707f, 0.0f, 0.0f);
//
//	/* 벡터의 덧셈, 뺄셈, 스칼라곱 */
//	XMVECTOR a = u + v;
//	XMVECTOR b = u - v;
//	XMVECTOR c = 10.0f * u;
//	
//	/* 벡터의 길이, 정규화, 내적, 외적 계산 */
//	XMVECTOR lu = XMVector3Length(u);
//	XMVECTOR nu = XMVector3Normalize(u);
//	XMVECTOR dotuv = XMVector3Dot(u, v);
//	XMVECTOR crossuv = XMVector3Cross(u, v);
//
//	/* w, n으로부터 proj_n(w), perp_n(w) 값 추출 */
//	XMVECTOR projW;
//	XMVECTOR perpW;
//	XMVector3ComponentsFromNormal(&projW, &perpW, w, n);
//
//	/* proj + perp가 w와 같은지에 대한 여부 검사 */
//	bool eq = XMVector3Equal(projW + perpW, w);
//	bool neq = XMVector3NotEqual(projW + perpW, w);
//
//	/* projW와 perpW 사이의 각도, 모든 요소에 float값이 복사됨. */
//	XMVECTOR angleVec = XMVector3AngleBetweenVectors(projW, perpW);
//	float angleRadians = XMVectorGetX(angleVec);
//	float angleDegrees = XMConvertToDegrees(angleRadians);
//
//	cout << "u = " << u << "\n";
//	cout << "v = " << v << "\n";
//	cout << "w = " << w << "\n";
//	cout << "n = " << n << "\n";
//	cout << "a = u + v = " << a << "\n";
//	cout << "b = u - v = " << b << "\n";
//	cout << "c = 10 * u = " << c << "\n";
//	cout << "nu = u / ||u|| = " << nu << "\n";
//	cout << "lu = ||u|| = " << lu << "\n";
//	cout << "dotuv = u * v = " << dotuv << "\n";
//	cout << "crossuv = u x v = " << crossuv << "\n";
//	cout << "projW = " << projW << "\n";
//	cout << "perpW = " << perpW << "\n";
//	cout << "projW + perpW == w : " << eq << "\n";
//	cout << "projW + perpW != w : " << neq << "\n";
//	cout << "angleVec : " << angleVec << "\n";
//	cout << "angleRadians : " << angleRadians << "\n";
//	cout << "angleDegrees : " << angleDegrees << "\n";
//
//	return 0;
//}



/* XMVECTOR 연습하기 2 */
//int main()
//{
//	cout.setf(ios_base::boolalpha);
//	cout.precision(8);
//
//	if (!XMVerifyCPUSupport())
//	{
//		cout << "DirectXMath를 지원하지 않습니다.\n";
//		return 0;
//	}
//
//	XMVECTOR u = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
//	XMVECTOR n = XMVector3Normalize(u);
//
//	float LU = XMVectorGetX(XMVector3Length(n));
//
//	cout << "LU : " << LU << "\n";
//	if (LU == 1.0f)
//		cout << "길이가 1입니다.\n";
//	else
//		cout << "길이가 1이 아닙니다.\n";
//	
//	float powLU = powf(LU, 1.0e6f);
//	cout << "powLU = " << powLU << "\n";
//}



/* 연습문제 출력 결과 예측하기 */
//int main()
//{
//	cout.setf(ios_base::boolalpha);
//	
//	if (XMVerifyCPUSupport() == false)
//	{
//		cout << "DirectXMath 라이브러리를 지원하지 않습니다.\n";
//		return 0;
//	}
//
//	XMVECTOR p = XMVectorSet(2.0f, 2.0f, 1.0f, 0.0f);
//	XMVECTOR q = XMVectorSet(2.0f, -0.5f, 0.5f, 0.1f);
//	XMVECTOR u = XMVectorSet(1.0f, 2.0f, 4.0f, 8.0f);
//	XMVECTOR v = XMVectorSet(-2.0f, 1.0f, -3.0f, 2.5f);
//	XMVECTOR w = XMVectorSet(0.0f, XM_PIDIV4, XM_PIDIV2, XM_PI);
//}