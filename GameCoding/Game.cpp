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
	_pipeline = make_shared<Pipeline>(_graphics->GetDeviceContext());

	_vertexBuffer = make_shared<VertexBuffer>(_graphics->GetDevice());
	_indexBuffer = make_shared<IndexBuffer>(_graphics->GetDevice());
	_inputLayout = make_shared<InputLayout>(_graphics->GetDevice());

	_geometry = make_shared<Geometry<VertexTextureData>>();

	_vertexShader = make_shared<VertexShader>(_graphics->GetDevice());
	_rasterizerState = make_shared<RasterizerState>(_graphics->GetDevice());
	_pixelShader = make_shared<PixelShader>(_graphics->GetDevice());

	_constantBuffer = make_shared<ConstantBuffer<TransformData>>(_graphics->GetDevice(), _graphics->GetDeviceContext());
	_texture1 = make_shared<Texture>(_graphics->GetDevice());

	_samplerState = make_shared<SamplerState>(_graphics->GetDevice());
	_blendState = make_shared<BlendState>(_graphics->GetDevice());

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
	_rasterizerState->Create();
	_samplerState->Create();
	_blendState->Create();
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

	PipelineInfo info;
	info.inputLayout = _inputLayout;
	info.vertexShader = _vertexShader;
	info.rasterizerState = _rasterizerState;
	info.pixelShader = _pixelShader;
	info.blendState = _blendState;

	_pipeline->UpdatePipeline(info); 

	//IA
	_pipeline->SetVertexBuffer(_vertexBuffer);
	_pipeline->SetIndexBuffer(_indexBuffer);
	//VS
	_pipeline->SetConstantBuffer(0, SS_VertexShader, _constantBuffer);
	//PS
	_pipeline->SetTexture(0, SS_PixelShader, _texture1);
	_pipeline->SetSamplerState(0, SS_PixelShader, _samplerState);
	
	//OM
	_pipeline->DrawIndexed(_geometry->GetIndexCount(), 0, 0);

	_graphics->RenderEnd();
}