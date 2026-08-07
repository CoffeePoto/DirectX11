#include "pch.h"
#include "Game.h"

Game::Game()
{
}

Game::~Game()
{
}

void Game::Init(HWND hwnd)
{
	_hwnd = hwnd;
	//_width = GWinSizeX;
	//_height = GWinSizeY;
	_graphics = make_shared<Graphics>(hwnd);
	_vertexBuffer = make_shared<VertexBuffer>(_graphics->GetDevice());
	_indexBuffer = make_shared<IndexBuffer>(_graphics->GetDevice());
	_inputLayout = make_shared<InputLayout>(_graphics->GetDevice());
	_geometry = make_shared<Geometry<VertexTextureData>>();
	_vertexShader = make_shared<VertexShader>(_graphics->GetDevice());
	_pixelShader = make_shared<PixelShader>(_graphics->GetDevice());
	_constantBuffer = make_shared<ConstantBuffer<TransformData>>(_graphics->GetDevice(), _graphics->GetDeviceContext());
	_texture1 = make_shared<Texture>(_graphics->GetDevice());

	//dx 장치의 생성 순서는 렌더링 파이프라인의 순서에 엄격히 의존하지 않는다.
	//렌더링 파이프라인 과정과 생성 순서가 조금 뒤바뀌어도 괜찮다.
	// 
	//버텍스 정보 기입
	/*
		VertexData
		1 3
		0 2
	*/
	//VertexData
	GeometryHelper::CreateRectangle(_geometry);
	//VertexBuffer
	_vertexBuffer->Create(_geometry->GetVertices());
	//IndexBuffer
	_indexBuffer->Create(_geometry->GetIndices());

	//셰이더 설정 및 정보 전달
	_vertexShader->Create(L"Default.hlsl", "VS", "vs_5_0");
	_inputLayout->Create(VertexTextureData::descs, _vertexShader->GetBlob());
	_pixelShader->Create(L"Default.hlsl", "PS", "ps_5_0");

	_texture1->Create(L"Images/Ocean.jpg");
	_constantBuffer->Create();

	//레스터라이저 설정
	CreateRasterizerState();
	CreateSamplerState();
}

void Game::Update()
{
	//SRT
	Matrix matScale = Matrix::CreateScale(_localScale);
	Matrix matRotation = Matrix::CreateRotationX(_localRotation.x);
	matRotation *= Matrix::CreateRotationY(_localRotation.y);
	matRotation *= Matrix::CreateRotationZ(_localRotation.z);
	Matrix matTranslation = Matrix::CreateTranslation(_localPosition);
	Matrix matWorld = matScale * matRotation * matTranslation; //SRT 연산
	_transformData.matWorld = matWorld;

	//물체 이동 시 변하는 위치 값을 받기 위함
	_constantBuffer->CopyData(_transformData);
}

void Game::Render()
{
	_graphics->RenderBegin();

	// IA - VS - RS - PS - OM
	uint32 stride = sizeof(VertexTextureData);
	uint32 offset = 0;

	auto _deviceContext = _graphics->GetDeviceContext();

	//IA
	_deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer->GetComPtr().GetAddressOf(), &stride, &offset);
	_deviceContext->IASetIndexBuffer(_indexBuffer->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);
	_deviceContext->IASetInputLayout(_inputLayout->GetComPtr().Get());
	_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 그리는 방식

	//VS
	_deviceContext->VSSetShader(_vertexShader->GetComPtr().Get(), nullptr, 0);
	_deviceContext->VSSetConstantBuffers(0, 1, _constantBuffer->GetComPtr().GetAddressOf()); //상수 버퍼 세팅

	//RS
	_deviceContext->RSSetState(_rasterizerState.Get()); // 레스터라이저 바인딩

	//PS
	_deviceContext->PSSetShader(_pixelShader->GetComPtr().Get(), nullptr, 0);
	_deviceContext->PSSetShaderResources(0, 1, _texture1->GetComPtr().GetAddressOf());
	_deviceContext->PSSetSamplers(0, 1, _samplerState.GetAddressOf());

	//OM
	//_deviceContext->Draw(_vertices.size(), 0);
	//인덱스를 추가해줘서 그리기 방법이 바뀜.
	_deviceContext->DrawIndexed(_geometry->GetIndexCount(), 0, 0);
	_deviceContext->OMSetBlendState(_blendState.Get(), nullptr, 0xFFFFFFFF);

	_graphics->RenderEnd();
}

void Game::CreateRasterizerState()
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

	HRESULT hr = _graphics->GetDevice()->CreateRasterizerState(&desc, _rasterizerState.GetAddressOf());
	CHECK(hr);
}

void Game::CreateSamplerState()
{
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	//samplerstate 설정
	desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	//RGBA 순서
	desc.BorderColor[0] = 0;
	desc.BorderColor[1] = 0;
	desc.BorderColor[2] = 0;
	desc.BorderColor[3] = 1;
	//여기는 나중에
	desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.MaxAnisotropy = 16;
	desc.MaxLOD = FLT_MAX;
	desc.MinLOD = FLT_MIN;
	desc.MipLODBias = 0.0f;

	_graphics->GetDevice()->CreateSamplerState(&desc, _samplerState.GetAddressOf());
}

void Game::CreateBlendState()
{
	D3D11_BLEND_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	
	//알파값에 따라 어떻게 색상을 섞을지를 결정하는 단계
	//반투명 상태 시 어떻게 처리할지를 결정하는건데, 비교적 덜 중요
	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;

	desc.RenderTarget[0].BlendEnable = true;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HRESULT hr = _graphics->GetDevice()->CreateBlendState(&desc, _blendState.GetAddressOf());
	CHECK(hr);
}



