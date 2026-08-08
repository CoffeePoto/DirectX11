#pragma once
class SamplerState
{
public:
	SamplerState(ComPtr<ID3D11Device> device);
	~SamplerState();

	void Create();

	ComPtr<ID3D11SamplerState> GetComPtr() { return _samplerState; }

private:
	ComPtr<ID3D11Device> _device;
	ComPtr<ID3D11SamplerState> _samplerState;
};

