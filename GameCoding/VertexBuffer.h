#pragma once

class VertexBuffer
{
public:
	VertexBuffer(ComPtr<ID3D11Device> device);
	~VertexBuffer();

	ComPtr<ID3D11Buffer> GetComPtr() { return _vertexBuffer; }
	uint32 GetStride() { return _stride; }
	uint32 GetOffset() { return _offset; }
	uint32 GetCount() { return _count; }
	
	//템플릿을 사용하는 함수의 경우 헤더에서 작성해야 컴파일 에러가 안난다.
	template<typename T>
	void Create(const vector<T>& vertices)
	{
		_stride = sizeof(T);
		_count = static_cast<uint32>(vertices.size());

		//정점 정보를 gpu에 넘겨줘야 한다. => 버퍼 디스크립션 작성
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		//cpu와 gpu가 어떻게 이 버퍼를 가지고 작업할 것인지
		//정점 버퍼는 한번 작업하고 고칠 일이 없어서 immutable
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		//Input Assembler에서 건내 주는 vertex buffer라고 표기
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.ByteWidth = (uint32)(_stride * _count);

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		//정점 정보가 담겨있는 RAM의 주소 정보를 gpu에게 넘겨준다.
		//gpu는 이 정보를 기반으로 RAM에서 데이터를 복사해 이용한다.
		data.pSysMem = vertices.data();

		//gpu쪽에서 해당 정보를 전부 가지고 있는 버퍼를 생성
		HRESULT hr = _device->CreateBuffer(&desc, &data, _vertexBuffer.GetAddressOf());
		CHECK(hr);
	}
private:
	ComPtr<ID3D11Device> _device;
	ComPtr<ID3D11Buffer> _vertexBuffer;

	uint32 _stride = 0;
	uint32 _offset = 0;
	uint32 _count = 0;
};