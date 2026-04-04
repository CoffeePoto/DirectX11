/*
    이 코드는 Vertex Shader와 Pixel Shader를 한번에 작성한 코드.
    VS와 PS는 분리해서 작성 가능.
    이 hlsl 파일을 컴파일하면 cso파일이 생기는데, 컴파일된 cso 파일을 들고 있어도
    괜찮다. (미리 컴파일된 파일을 들고 있는 것이 실행 시간에는 더 도움이 된다.)
*/

struct VS_INPUT
{
    //POSITION, COLOR : SemanticName ->  InputLayout에 넘겨줘야 할 정보
    float4 position : POSITION;
    float2 uv : TEXCOORD;
    //float4 color : COLOR;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    //float4 color : COLOR;
};

//t0 레지스터에 gpu 메모리에 있는 이미지 데이터를 사용하겠다는 의미
Texture2D texture0 : register(t0);
// s0 레지스터에 이미지 데이터를 어떻게 읽을 것인지를 등록
SamplerState sampler0 : register(s0);

VS_OUTPUT VS(VS_INPUT input)
{
    //원래 행렬을 이용한 계산이 필요하지만 일단은 그대로 토스
    VS_OUTPUT output;
    output.position = input.position;
    output.uv = input.uv;
    //output.color = input.color;
    
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
    float4 color = texture0.Sample(sampler0, input.uv);
    
    return color;
}