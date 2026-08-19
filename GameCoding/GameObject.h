#pragma once

class MonoBehaviour;

class GameObject : public enable_shared_from_this<GameObject>
{
public:
	GameObject(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);
	~GameObject();

	void Awake();
	void Start();

	void Update();
	void LateUpate();
	void FixedUpdate();
	
	shared_ptr<Component> GetFixedComponent(ComponentType type);
	shared_ptr<Transform> GetTransform();

	shared_ptr<Transform> GetorAddTransform();
	void AddComponent(shared_ptr<Component> component);

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

protected:
	array<shared_ptr<Component>, FIXED_COMPONENT_COUNT> _components;
	vector<shared_ptr<MonoBehaviour>> _scripts;
};

