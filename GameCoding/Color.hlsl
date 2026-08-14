struct VS_INPUT
{
    //POSITION, COLOR : SemanticName ->  InputLayout에 넘겨줘야 할 정보
    float4 position : POSITION;
    float4 color : COLOR;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

// t0 레지스터에 gpu 메모리에 있는 이미지 데이터를 사용하겠다는 의미
Texture2D texture0 : register(t0);
// s0 레지스터에 이미지 데이터를 어떻게 읽을 것인지를 등록
SamplerState sampler0 : register(s0);

// 이동을 위한 값을 CPU에서 받을 상수 버퍼를 b0 레지스터에 등록
// cbuffer : GPU에서 사용할 상수 데이터를 담는 구조체
cbuffer TransformData : register(b0)
{
    row_major matrix matWorld;
    row_major matrix matView;
    row_major matrix matProjection;
}

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    
    //WVP
    float4 position = mul(input.position, matWorld); // World
    position = mul(position, matView); // View
    position = mul(position, matProjection); // Projection
    
    output.position = position;
    output.color = input.color;
    
    return output;
}

/*
    여기까지가 Vertex Shader의 역할
    SV_POSITION은 픽셀 셰이더로 넘겨질 정점의 최종 clip 공간에서의 위치를 나타냄.
    그 후 Rasterizer 단계에서 점과 점 사이의 색깔을 보간하고, 픽셀 셰이더로 토스
    RS 단계는 코드로 건드릴 수 없는 단계
*/

float4 PS(VS_OUTPUT input) : SV_TARGET
{    
    return input.color;
}