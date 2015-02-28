//===========================================
//
//	SHADER FILE
//	Diffuse Pixel shader
//
//===========================================

#version 400

//DATA STRUCTURES
struct TBaseLight
{
	vec4 gLightColour;
	float gSpecularPower;
};
struct TPointLight
{
	TBaseLight tBaseLight;
	vec3 vecPosition;
	vec3 vecAttenuation;
};
struct TDirectionalLight
{
	TBaseLight tBaseLight;
	vec3 vecDirection;
};
struct TSpotLight
{
	TBaseLight tBaseLight;
	vec3 vecDirection;
	vec3 vecPosition;
	vec3 vecAttenuation;
	float fCutoff;
};

//INPUT
in vec3 worldPos;
in vec3 position;
in vec2 texCoord;
in vec3 normal;
in vec3 tangent;

//OUTPUT
out vec4 outputColour;

//UNIFORM Variables
uniform sampler2D gShaderTexture;
uniform sampler2D gDepthTexture;
uniform sampler2D gNormalMapTexture;
uniform vec3 gCameraPosition;

//LIGHTING Variables
const int MAX_LIGHTS = 4;
uniform vec4 gAmbientLight;
uniform int gNumPointLights;
uniform int gNumSpotLights;
uniform TPointLight gPointLight[MAX_LIGHTS];	//Point light array
uniform TSpotLight gSpotLight[MAX_LIGHTS];		//Spot light array
uniform TDirectionalLight gDirectionalLight;	//Single Directional Light

//FUNCTIONS
vec3 CalculateBumpedNormal();
vec4 CalculateSpotLight(int _index, vec3 _normal);
vec4 CalculatePointLight(int _index, vec3 _normal);
vec4 CalculateDirectionalLight(vec3 _normal);
vec4 CalculateGeneralLight(TBaseLight _light, vec3 _direction, vec3 _normal);

//================== PIXEL SHADER ==============
void main(void)
{
	vec4 vecLightingColour;
	vec4 textureColour;
	vec3 BumpedNormal = CalculateBumpedNormal();
	textureColour = texture(gShaderTexture, texCoord);
	
	vecLightingColour =  gAmbientLight;
	vecLightingColour += CalculateDirectionalLight(normal);	
	vecLightingColour.w = 1.0f;
	for(int iPoint = 0; iPoint < gNumPointLights; ++iPoint)
	{
		vecLightingColour += CalculatePointLight(iPoint, BumpedNormal);
		vecLightingColour.w = 1.0f;
	}
	for(int iSpot = 0; iSpot < gNumSpotLights; ++iSpot)
	{
		vecLightingColour += CalculateSpotLight(iSpot, BumpedNormal);
		vecLightingColour.w = 1.0f;
	}
	//Multiply texture colour with lighting
	outputColour = textureColour;//vecLightingColour * textureColour;
	outputColour.a = textureColour.a;
	//float fDepth = texture(gDepthTexture, texCoord).x;
	//outputColour = vec4(fDepth, fDepth, fDepth, 1.0f);
}

vec3 CalculateBumpedNormal()
{
	vec3 Normal = normalize(normal);
	vec3 Tangent = normalize(tangent);
	Tangent = normalize(Tangent - (dot(Tangent, Normal) * Normal));
	vec3 BiTangent = cross(Tangent, Normal);
	vec3 BumpNormal = (2.0f * texture(gNormalMapTexture, texCoord).xyz) - vec3(1.0f, 1.0f, 1.0f);
	mat3 NormalMatrix = mat3(Tangent, BiTangent, Normal);
	vec3 FinalNormal = normalize(NormalMatrix * BumpNormal);
	return FinalNormal;
}
vec4 CalculateSpotLight(int _index, vec3 _normal)
{
	vec3 vecToLight = normalize(worldPos - gSpotLight[_index].vecPosition);
	float fSpotFactor = dot(vecToLight, gSpotLight[_index].vecDirection);
	vec4 LightColour = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	if(fSpotFactor > gSpotLight[_index].fCutoff)
	{
		vec3 LightDirection = worldPos - gSpotLight[_index].vecPosition;
		float fLightDistance = length(LightDirection);
	
		LightColour = CalculateGeneralLight(gSpotLight[_index].tBaseLight, LightDirection, _normal);
		float fAttenuation	= gSpotLight[_index].vecAttenuation.x 
							+ (gSpotLight[_index].vecAttenuation.y * fLightDistance)
							+ (gSpotLight[_index].vecAttenuation.z * fLightDistance * fLightDistance);
		LightColour /= fAttenuation;
		LightColour.w = 1.0f;

		//This will produce the outer cone fade
		LightColour *= (1.0f - (1.0f - fSpotFactor) * (1.0f / (1.0f - gSpotLight[_index].fCutoff)));
	}
	return LightColour;
}
vec4 CalculatePointLight(int _index, vec3 _normal)
{
	vec4 LightColour;
	vec3 LightDirection = worldPos - gPointLight[_index].vecPosition;
	float fLightDistance = length(LightDirection);
	
	LightColour = CalculateGeneralLight(gPointLight[_index].tBaseLight, LightDirection, _normal);
	float fAttenuation	= gPointLight[_index].vecAttenuation.x 
						+ (gPointLight[_index].vecAttenuation.y * fLightDistance)
						+ (gPointLight[_index].vecAttenuation.z * fLightDistance * fLightDistance);
	LightColour /= fAttenuation;
	LightColour.w = 1.0f;
	return LightColour;
}
vec4 CalculateDirectionalLight(vec3 _normal)
{
	return CalculateGeneralLight(gDirectionalLight.tBaseLight, gDirectionalLight.vecDirection, _normal);
}
vec4 CalculateGeneralLight(TBaseLight _light, vec3 _direction, vec3 _normal)
{
	vec3 LightDirection = -_direction;
	vec3 Normal = normalize(_normal);

	float fDiffuseFactor = dot(Normal, LightDirection);

	vec4 DiffuseColour = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	vec4 SpecularColour = vec4(0.0f, 0.0f, 0.0f, 0.0f);

	if(fDiffuseFactor > 0.0f)
	{
		DiffuseColour = _light.gLightColour * fDiffuseFactor;

		vec3 VertexToCamera = normalize(gCameraPosition - worldPos);
		vec3 LightReflection = normalize(reflect(_direction, Normal));
		float fSpecularFactor = dot(VertexToCamera, LightReflection);
		fSpecularFactor = pow(fSpecularFactor, _light.gSpecularPower);
		if(fSpecularFactor > 0.0f)
		{
			SpecularColour = _light.gLightColour * fSpecularFactor;
		}
	}

	return (DiffuseColour + SpecularColour);
}