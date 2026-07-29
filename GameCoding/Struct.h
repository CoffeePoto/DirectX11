#pragma once
#include "Types.h"

struct Vertex
{
	Vec3 position;
	//Color color;
	Vec2 uv;
};

struct TransformData
{
	Vec3 offset;
	float dummy;
	//constant buffer를 만들 때는 16바이트 정렬을 해야하므로
	/*
		Gemini 설명
		1. 하드웨어 표준: GPU 레지스터가 128비트(16바이트) 기반이기 때문입니다.
		2. 성능: 메모리 정렬이 되어 있어야 GPU가 가장 빠르게 데이터를 가져올 수 있습니다.
		3. 데이터 일관성: C++의 CPU 메모리 레이아웃과 쉐이더의 GPU 메모리 레이아웃을 완벽하게 동기화하기 위해 더미 데이터를 사용합니다.
	*/
};