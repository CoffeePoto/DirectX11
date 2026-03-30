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
	_width = GWinSizeX;
	_height = GWinSizeY;

	//장치&스왑체인 생성, 뷰포트 설정
	CreateDeviceAndSwapChain();
	CreateRenderTargetView();
	SetViewport();

	//버텍스 정보 기입
	//셰이더 설정 및 정보 전달
	CreateGeometry();
	CreateVS();
	CreateInputLayout();
	CreatePS();
}

void Game::Update()
{
}

void Game::Render()
{
	RenderBegin();

	// IA - VS - RS - PS - OM
	uint32 stride = sizeof(Vertex);
	uint32 offset = 0;

	//IA
	_deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &stride, &offset);
	_deviceContext->IASetInputLayout(_inputLayout.Get());
	_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 그리는 방식

	//VS
	_deviceContext->VSSetShader(_vertexShader.Get(), nullptr, 0);

	//PS
	_deviceContext->PSSetShader(_pixelShader.Get(), nullptr, 0);

	//OM
	_deviceContext->Draw(_vertices.size(), 0);

	RenderEnd();
}

void Game::CreateDeviceAndSwapChain()
{
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	//디스크립션 설정
	desc.BufferDesc.Width = _width;
	desc.BufferDesc.Height = _height; //화면 사이즈와 동일하게 버퍼 사이즈 지정
	desc.BufferDesc.RefreshRate.Numerator = 60; // Numerator = 분자
	desc.BufferDesc.RefreshRate.Denominator = 1; // Denominator = 분모, 화면 주사율 지정
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 8bits * 4짜리로 만들겠다.
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; // 지금 단계에서는 일단 무시.
	desc.SampleDesc.Count = 1; // 계단 현상(aliasing) 방지를 위해서면 숫자를 높이면 된다.
	desc.SampleDesc.Quality = 0;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 최종 결과물을 그려주는 역할로 사용하겠다는 의미.
	desc.BufferCount = 1; // BackBuffer 개수
	desc.OutputWindow = _hwnd; //윈도우 핸들
	desc.Windowed = true;
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	//_device.Get();
	//_device.GetAddressOf();

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr, 
		D3D_DRIVER_TYPE_HARDWARE, // 그래픽카드를 사용하겠다고 지정
		nullptr,
		0,
		nullptr, // feature level에 대한 배열을 건내야 한다(DX 버전 선택), 채우지 않아도 지원가능한 상위 버전 선택.  
		0, // 배열 크기, nullptr 지정했으니 0개
		D3D11_SDK_VERSION,
		&desc, // 스왑체인 디스크립션 지정
		_swapChain.GetAddressOf(), // _device에다가 결과물을 받아준다.
		_device.GetAddressOf(), 
		nullptr,
		_deviceContext.GetAddressOf()
	);

	// 제대로 생성되었는지 확인하는 매크로.
	// 장치를 생성하고 난 다음에는 반드시 생성되었는지 확인하는 것이 좋다.
	CHECK(hr);
}

/*
   흐름:
   1. swapchain의 후면 버퍼에 해당하는 리소스를 backBuffer에 지정
   2. CreateRenderTargetView를 통해 backBuffer를 묘사하는 RenderTargetView 형태를 생성
   3. GPU와 통신하면서 RenderTargetView의 정보를 통해 그린 그림을 backBuffer에 기록하는 형태로 작동
*/
void Game::CreateRenderTargetView()
{
	HRESULT hr;

	ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
	CHECK(hr);

	_device->CreateRenderTargetView(backBuffer.Get(), nullptr, _renderTargetView.GetAddressOf());
	CHECK(hr);
}

void Game::SetViewport()
{
	_viewport.TopLeftX = 0.f;
	_viewport.TopLeftY = 0.f;
	_viewport.Width = static_cast<float>(_width);
	_viewport.Height = static_cast<float>(_height);
	_viewport.MinDepth = 0.f;
	_viewport.MaxDepth = 1.f;

}

void Game::RenderBegin()
{
	// Output Merger에 그림을 그릴 RenderTarget(도화지)의 정보 전달
	_deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), nullptr);
	// 그림을 그리기 전에 기본 배경색으로 RenderTargetView(도화지) 초기화
	_deviceContext->ClearRenderTargetView(_renderTargetView.Get(), _clearColor);
	// 어느 정도 크기로 그릴것인지 설정
	// viewport 구조체에는 시작 좌표, 가로, 세로, 최소/최대 깊이 정보가 담겨있다.
	// Rasterizer Stage: 3d 공간 정점들을 2d 모니터 좌표로 변환하는 단계
	_deviceContext->RSSetViewports(1, &_viewport);
}

void Game::RenderEnd()
{
	// 전면 버퍼 <-> 후면 버퍼 교체 과정
	// 정확히 말하면, 후면 버퍼의 정보를 전면 버퍼에 복사하고, 화면에 출력해달라고 요청하는 작업
	HRESULT hr = _swapChain->Present(1, 0);
	CHECK(hr);
}

void Game::CreateGeometry()
{
	//정점 정보 기입
	_vertices.resize(3);

	_vertices[0].position = Vec3(-0.5f, -0.5f, 0.f);
	_vertices[0].color = Color(1.f, 0.f, 0.f, 1.f);

	_vertices[1].position = Vec3(0.f, 0.5f, 0.f);
	_vertices[1].color = Color(0.f, 1.f, 0.f, 1.f);

	_vertices[2].position = Vec3(0.5f, -0.5f, 0.f);
	_vertices[2].color = Color(0.f, 0.f, 1.f, 1.f);

	//정점 정보를 gpu에 넘겨줘야 한다. => 버퍼 디스크립션 작성
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	//cpu와 gpu가 어떻게 이 버퍼를 가지고 작업할 것인지
	//정점 버퍼는 한번 작업하고 고칠 일이 없어서 immutable
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	//Input Assembler에서 건내 주는 vertex buffer라고 표기
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.ByteWidth = (uint32)(sizeof(Vertex) * _vertices.size());

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));
	//정점 정보가 담겨있는 RAM의 주소 정보를 gpu에게 넘겨준다.
	//gpu는 이 정보를 기반으로 RAM에서 데이터를 복사해 이용한다.
	data.pSysMem = _vertices.data();

	//gpu쪽에서 해당 정보를 전부 가지고 있는 버퍼를 생성
	_device->CreateBuffer(&desc, &data, _vertexBuffer.GetAddressOf());
}

void Game::CreateInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0
		},
		{//D3D11_APPEND_ALIGNED_ELEMENT는 구조의 크기를 자동으로 계산하는 매크로
			"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, /*D3D11_APPEND_ALIGNED_ELEMENT*/ 12, D3D11_INPUT_PER_VERTEX_DATA, 0
		},
	};

	const int32 count = sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC);
	_device->CreateInputLayout(layout, count, _vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(), _inputLayout.GetAddressOf());
}

void Game::LoadShaderFromFile(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob)
{
	const uint32 compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

	//pch.h의 d3dcompiler.h 라이브러리에 포함된 함수
	HRESULT hr = D3DCompileFromFile(
		path.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		name.c_str(),
		version.c_str(),
		compileFlag,
		0,
		blob.GetAddressOf(),
		nullptr
	);
	//제대로 생성되었는지 체크
	CHECK(hr);
}

// 셰이더를 로드해서 Blob을 만들고, Blob을 이용해서 Vertex Shader를 생성
void Game::CreateVS()
{
	//vsblob 정보 채우기
	LoadShaderFromFile(L"Default.hlsl", "VS", "vs_5_0", _vsBlob);

	HRESULT hr = _device->CreateVertexShader(
		_vsBlob->GetBufferPointer(),
		_vsBlob->GetBufferSize(),
		nullptr,
		_vertexShader.GetAddressOf()
	);

	CHECK(hr);
}

void Game::CreatePS()
{
	//psblob 정보 채우기
	LoadShaderFromFile(L"Default.hlsl", "PS", "ps_5_0", _psBlob);

	HRESULT hr = _device->CreatePixelShader(
		_psBlob->GetBufferPointer(),
		_psBlob->GetBufferSize(),
		nullptr,
		_pixelShader.GetAddressOf()
	);

	CHECK(hr);
}



