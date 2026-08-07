#pragma once

enum ShaderScope
{
	//BitFlag
	SS_None = 0, // 0000
	SS_VertexShader = (1 << 0), // 0001 = 1, shift 하지 않음
	SS_PixelShader = (1 << 1), // 0010 = 2, 왼쪽으로 shift 1칸
	SS_Both = SS_VertexShader | SS_PixelShader // 0011 = 3, 두 비트의 or 연산
};

class Shader
{
public:
	Shader(ComPtr<ID3D11Device> device);
	virtual ~Shader();

	virtual void Create(const wstring& path, const string& name, const string& version) abstract;

	ComPtr<ID3DBlob> GetBlob() { return _blob; }

protected:
	void LoadShaderFromFile(const wstring& path, const string& name, const string& version);

protected:
	wstring _path;
	string _name;
	ComPtr<ID3D11Device> _device;
	ComPtr<ID3DBlob> _blob;
};

class VertexShader : public Shader
{
	using Super = Shader;
public:
	VertexShader(ComPtr<ID3D11Device> device);
	~VertexShader();

	virtual void Create(const wstring& path, const string& name, const string& version);

	ComPtr<ID3D11VertexShader> GetComPtr() { return _vertexShader; }

protected:
	ComPtr<ID3D11VertexShader> _vertexShader;
};

class PixelShader : public Shader
{
	using Super = Shader;
public:
	PixelShader(ComPtr<ID3D11Device> device);
	~PixelShader();

	virtual void Create(const wstring& path, const string& name, const string& version);

	ComPtr<ID3D11PixelShader> GetComPtr() { return _pixelShader; }

protected:
	ComPtr<ID3D11PixelShader> _pixelShader;
};