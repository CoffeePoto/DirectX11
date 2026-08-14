#pragma once

class GameObject
{
public:
	GameObject(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);
	~GameObject();

	void Update();
	void Render(shared_ptr<Pipeline> pipeline);

private:
	ComPtr<ID3D11Device> _device;

	//Geometry
	shared_ptr<Geometry<VertexTextureData>> _geometry;
	//shared_ptr<Geometry<VertexColorData>> _geometry;

	//IA
	shared_ptr<VertexBuffer> _vertexBuffer;
	shared_ptr<IndexBuffer> _indexBuffer;
	shared_ptr<InputLayout> _inputLayout;

	//VS
	shared_ptr<VertexShader> _vertexShader;

	//Rasterizer
	shared_ptr<RasterizerState> _rasterizerState;

	//PS
	shared_ptr<PixelShader> _pixelShader;
	shared_ptr<Texture> _texture1;

	//SamplerState
	shared_ptr<SamplerState> _samplerState;
	shared_ptr<BlendState> _blendState;

private:
	//CBuffer
	TransformData _transformData;
	shared_ptr<ConstantBuffer<TransformData>> _constantBuffer;
	shared_ptr<Transform> _transform = make_shared<Transform>();
	shared_ptr<Transform> _parent = make_shared<Transform>();
};

