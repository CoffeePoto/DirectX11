#include "pch.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "Game.h"
#include "Pipeline.h"
#include "Mesh.h"
#include "Material.h"

MeshRenderer::MeshRenderer(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext)
	: Super(ComponentType::MeshRenderer), _device(device)
{
	//dx 장치의 생성 순서는 렌더링 파이프라인의 순서에 엄격히 의존하지 않는다.
	//렌더링 파이프라인 과정과 생성 순서가 조금 뒤바뀌어도 괜찮다.

	//_texture1 = make_shared<Texture>(device);
	//_texture1->Create(L"Images/Ocean.jpg");
}

MeshRenderer::~MeshRenderer()
{

}