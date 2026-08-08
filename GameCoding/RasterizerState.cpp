#include "pch.h"
#include "RasterizerState.h"

RasterizerState::RasterizerState(ComPtr<ID3D11Device> device)
	: _device(device)
{
}

RasterizerState::~RasterizerState()
{
}

void RasterizerState::Create()
{
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	/*
		FillMode : 어떻게 채울 것인가.
		WIREFRAME : 물체의 삼각형 프레임을 출력
		SOLID : 물체의 색상을 채워서 출력
	*/
	desc.FillMode = D3D11_FILL_SOLID;
	/*
		CullMode : 어떻게 자를 것인가. 어떤 기준으로 물체가 뒤에 있음을 판단할 것인가.(or 카메라 밖에 있거나)
		NONE : 카메라 범위 내에 있으면 전부 출력
		BACK : 물체 뒤에 있으면 출력하지 않음
		FRONT : 물체 앞에 있으면 출력하지 않음
	*/
	desc.CullMode = D3D11_CULL_BACK;
	/*
		FrontCounterClockwise : 삼각형 그리는 방향이 반시계인가, 시계인가에 따라 앞면, 후면임을 판단하는 기준
		true : 시계방향으로 그리면 앞면으로 판단
		false : 반시계방향으로 그리면 앞면으로 판단
	*/
	desc.FrontCounterClockwise = false;

	HRESULT hr = _device->CreateRasterizerState(&desc, _rasterizerState.GetAddressOf());
	CHECK(hr);
}
