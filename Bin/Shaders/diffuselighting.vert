//===========================================
//
//	SHADER FILE
//	Diffuse Vertex shader
//
//===========================================

#version 400

//INPUT
in vec3 inputPosition;
in vec2 inputTexCoord;
in vec3 inputNormal;
in vec3 inputTangent;

//OUTPUT
out vec3 worldPos;
out vec3 position;
out vec2 texCoord;
out vec3 normal;
out vec3 tangent;

//UNIFORM Variables
uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

//================== VERTEX SHADER ==============
void main(void)
{
	gl_Position = worldMatrix * vec4(inputPosition, 1.0f);
	gl_Position = viewMatrix * gl_Position;
	gl_Position = projectionMatrix * gl_Position;

	
	//Calculate normal vectors
	worldPos = (worldMatrix * vec4(inputPosition, 1.0f)).xyz;
	normal = normalize((worldMatrix * vec4(inputNormal, 1.0f)).xyz);
	position = gl_Position.xyz;
	texCoord = inputTexCoord;
	tangent = normalize((worldMatrix * vec4(inputTangent, 1.0f)).xyz);
}