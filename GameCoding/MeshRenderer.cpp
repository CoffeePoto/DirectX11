#include "pch.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "Game.h"
#include "Pipeline.h"

MeshRenderer::MeshRenderer(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext)
	: Super(ComponentType::MeshRenderer), _device(device)
{
	//dx 장치의 생성 순서는 렌더링 파이프라인의 순서에 엄격히 의존하지 않는다.
	//렌더링 파이프라인 과정과 생성 순서가 조금 뒤바뀌어도 괜찮다.

	//VertexData
	_geometry = make_shared<Geometry<VertexTextureData>>();
	GeometryHelper::CreateRectangle(_geometry);
	//_geometry = make_shared<Geometry<VertexColorData>>();
	//GeometryHelper::CreateRectangle(_geometry, Color(0.f, 0.f, 1.0f, 1.0f));

	//VertexBuffer
	_vertexBuffer = make_shared<VertexBuffer>(device);
	_vertexBuffer->Create(_geometry->GetVertices());

	//IndexBuffer
	_indexBuffer = make_shared<IndexBuffer>(device);
	_indexBuffer->Create(_geometry->GetIndices());


	//셰이더 설정 및 정보 전달
	_vertexShader = make_shared<VertexShader>(device);
	_vertexShader->Create(L"Default.hlsl", "VS", "vs_5_0");
	//_vertexShader->Create(L"Color.hlsl", "VS", "vs_5_0");

	_inputLayout = make_shared<InputLayout>(device);
	_inputLayout->Create(VertexTextureData::descs, _vertexShader->GetBlob());
	//_inputLayout->Create(VertexColorData::descs, _vertexShader->GetBlob());

	_pixelShader = make_shared<PixelShader>(device);
	_pixelShader->Create(L"Default.hlsl", "PS", "ps_5_0");
	//_pixelShader->Create(L"Color.hlsl", "PS", "ps_5_0");

	//레스터라이저 설정
	_rasterizerState = make_shared<RasterizerState>(device);
	_rasterizerState->Create();

	_blendState = make_shared<BlendState>(device);
	_blendState->Create();

	_cameraBuffer = make_shared<ConstantBuffer<CameraData>>(device, deviceContext);
	_cameraBuffer->Create();
	_transformBuffer = make_shared<ConstantBuffer<TransformData>>(device, deviceContext);
	_transformBuffer->Create();

	_texture1 = make_shared<Texture>(device);
	_texture1->Create(L"Images/Ocean.jpg");

	_samplerState = make_shared<SamplerState>(device);
	_samplerState->Create();
}

MeshRenderer::~MeshRenderer()
{

}

void MeshRenderer::Update()
{

	_cameraData.matView = Camera::S_MatView;
	//_cameraData.matView = Matrix::Identity;
	_cameraData.matProjection = Camera::S_MatProjection;
	//_cameraData.matProjection = Matrix::Identity;
	//물체 이동 시 변하는 위치 값을 받기 위함
	_cameraBuffer->CopyData(_cameraData);

	_transformData.matWorld = GetTransform()->GetWorldMatrix();
	//물체 이동 시 변하는 위치 값을 받기 위함
	_transformBuffer->CopyData(_transformData);

	//Render
	Render(GGame->GetPipeline());
}

void MeshRenderer::Render(shared_ptr<Pipeline> pipeline)
{
	PipelineInfo info;
	info.inputLayout = _inputLayout;
	info.vertexShader = _vertexShader;
	info.rasterizerState = _rasterizerState;
	info.pixelShader = _pixelShader;
	info.blendState = _blendState;

	pipeline->UpdatePipeline(info);

	//IA
	pipeline->SetVertexBuffer(_vertexBuffer);
	pipeline->SetIndexBuffer(_indexBuffer);
	//VS
	pipeline->SetConstantBuffer(0, SS_VertexShader, _cameraBuffer);
	pipeline->SetConstantBuffer(1, SS_VertexShader, _transformBuffer);
	//PS
	pipeline->SetTexture(0, SS_PixelShader, _texture1);
	pipeline->SetSamplerState(0, SS_PixelShader, _samplerState);

	//OM
	pipeline->DrawIndexed(_geometry->GetIndexCount(), 0, 0);
}