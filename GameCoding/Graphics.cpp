#include "pch.h"
#include "Graphics.h"

Graphics::Graphics(HWND hwnd)
{
	_hwnd = hwnd;
	//_width = GWinSizeX;
	//_height = GWinSizeY;

	CreateDeviceAndSwapChain();
	CreateRenderTargetView();
	SetViewport();
}

Graphics::~Graphics()
{
}

void Graphics::RenderBegin()
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

void Graphics::RenderEnd()
{
	// 전면 버퍼 <-> 후면 버퍼 교체 과정
	// 정확히 말하면, 후면 버퍼의 정보를 전면 버퍼에 복사하고, 화면에 출력해달라고 요청하는 작업
	HRESULT hr = _swapChain->Present(1, 0);
	CHECK(hr);
}

void Graphics::CreateDeviceAndSwapChain()
{
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	//디스크립션 설정
	desc.BufferDesc.Width = GWinSizeX;
	desc.BufferDesc.Height = GWinSizeY; //화면 사이즈와 동일하게 버퍼 사이즈 지정
	desc.BufferDesc.RefreshRate.Numerator = 60; // Numerator = 분자
	desc.BufferDesc.RefreshRate.Denominator = 1; // Denominator = 분모, 화면 주사율 지정
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 8bits * 4짜리로 만들겠다.
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; // 지금 단계에서는 일단 무시.
	desc.SampleDesc.Count = 1; // 계단 현상(aliasing) 방지를 위해서면 숫자를 높이면 된다.
	desc.SampleDesc.Quality = 0;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 최종 결과물을 그려주는 역할로 사용하겠다는 의미.
	desc.BufferCount = 1; // BackBuffer 개수
	desc.OutputWindow = _hwnd; // 윈도우 핸들
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

void Graphics::CreateRenderTargetView()
{
	HRESULT hr;

	ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
	CHECK(hr);

	_device->CreateRenderTargetView(backBuffer.Get(), nullptr, _renderTargetView.GetAddressOf());
	CHECK(hr);
}

void Graphics::SetViewport()
{
	_viewport.TopLeftX = 0.f;
	_viewport.TopLeftY = 0.f;
	_viewport.Width = static_cast<float>(GWinSizeX);
	_viewport.Height = static_cast<float>(GWinSizeY);
	_viewport.MinDepth = 0.f;
	_viewport.MaxDepth = 1.f;
}
