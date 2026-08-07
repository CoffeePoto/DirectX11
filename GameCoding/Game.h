#pragma once

class Game
{
public:
	Game();
	~Game();

public:
	void Init(HWND hwnd);
	void Update();
	void Render();

public:
	//레스터라이저 생성
	void CreateRasterizerState();
	void CreateSamplerState();
	void CreateBlendState();

private:
	HWND _hwnd;

	//Engine
	shared_ptr<Graphics> _graphics;

	//Geometry
	shared_ptr<Geometry<VertexTextureData>> _geometry;

	//IA
	shared_ptr<VertexBuffer> _vertexBuffer;
	shared_ptr<IndexBuffer> _indexBuffer;
	shared_ptr<InputLayout> _inputLayout;
	
	//VS
	shared_ptr<VertexShader> _vertexShader;
	//CBuffer
	TransformData _transformData;
	shared_ptr<ConstantBuffer<TransformData>> _constantBuffer;
	Vec3 _localPosition = { 0.0f, 0.0f, 0.0f };
	Vec3 _localRotation = { 0.0f, 0.0f, 0.0f };
	Vec3 _localScale = { 1.0f, 1.0f, 1.0f };

	//Rasterizer
	ComPtr<ID3D11RasterizerState> _rasterizerState = nullptr;

	//PS
	shared_ptr<PixelShader> _pixelShader;
	shared_ptr<Texture> _texture1;

	//SamplerState
	ComPtr<ID3D11SamplerState> _samplerState = nullptr;
	ComPtr<ID3D11BlendState> _blendState = nullptr;
};