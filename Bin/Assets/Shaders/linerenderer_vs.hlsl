#include "shaderdefines.hlsl"
// Buffers
cbuffer MatrixBuffer
{
	matrix worldMat;
	matrix viewMat;
	matrix projMat;
};

//  Vertex Types
struct VS_IN
{
	float3 pos		: POSITION;
	float3 dir		: DIRECTION;
	float2 scale	: SCALE;
	float4 colour	: COLOR;
	float rotation	: ROTATION;
	int textureID	: TEXID;
};

struct VS_OUT
{
	float4 pos		: SV_POSITION;
	float3 dir		: DIRECTION;
	float2 scale	: SCALE;
	float4 colour	: COLOR;
	float rotation : ROTATION;
	int textureID : TEXID;
};

//=============================
//	Object Shader Vertex
//=============================
VS_OUT LineVS(VS_IN _input)
{
	VS_OUT outVert;

	outVert.pos = float4(_input.pos, 1.0f);// ToScreenSpace(float4(_input.pos, 1.0f), worldMat, viewMat, projMat);
	//outVert.pos = mul(float4(_input.pos, 1.0f), worldMat);
	//outVert.pos = mul(outVert.pos, viewMat);
	//outVert.pos = mul(outVert.pos, projMat);

	outVert.dir = _input.dir;
	outVert.scale = _input.scale;
	outVert.colour = _input.colour;
	outVert.rotation = _input.rotation;

	outVert.textureID = _input.textureID;

	return outVert;
}
