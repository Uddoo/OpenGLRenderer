#version 330

in vec4 vCol;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 colour;

const int MAX_POINT_LIGHTS = 3; // 与CommonValues中的MAX_POINT_LIGHTS保持一致

struct Light
{
	vec3 colour;
	float ambientIntensity;
	float diffuseIntensity;
};

struct DirectionalLight
{
	Light base;
	vec3 direction;
};

struct PointLight
{
	Light base;
	vec3 position;
	float constant;
	float linear;
	float exponent;
};

struct Material
{
	float specularIntensity;
	float shininess;
};

uniform int pointLightCount;

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];

uniform sampler2D theTexture;
uniform Material material;

uniform vec3 eyePosition;

vec4 CalcLightByDirection(Light light, vec3 direction)
{
	vec4 ambientColour = vec4(light.colour, 1.0f) * light.ambientIntensity;

	float diffuseFactor = max(dot(normalize(Normal), normalize(direction)), 0.0f);
	vec4 diffuseColour = vec4(light.colour, 1.0f) * light.diffuseIntensity * diffuseFactor;

	vec4 specularColour = vec4(0, 0, 0, 0);

	if(diffuseFactor > 0.0f) // Only calculate specular if there is diffuse light
	{
		vec3 fragToEye = normalize(eyePosition - FragPos); // Direction from fragment to eye
		vec3 reflectVertex = normalize(reflect(direction, normalize(Normal))); // Direction of reflection

		float specularFactor = dot(fragToEye, reflectVertex); // Calculate specular factor
		if(specularFactor > 0.0f)
		{
			specularFactor = pow(specularFactor, material.shininess); // Calculate specular factor
			specularColour = vec4(light.colour, 1.0f) * material.specularIntensity * specularFactor; // Calculate specular colour, 颜色 * 光照强度 * 镜面反射因子
			//specularColour = vec4(Light.colour * material.specularIntensity * specularFactor, 1.0f);
		}
	}

	return ambientColour + diffuseColour + specularColour;
}

vec4 CalcDirectionalLight()
{
	return CalcLightByDirection(directionalLight.base, directionalLight.direction); 
}

vec4 CalcPointLights()
{
	vec4 totalColour = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	
	for(int i = 0; i < pointLightCount; i++)
	{
		vec3 direction = FragPos - pointLights[i].position;
		float distance = length(direction);
		direction = normalize(direction);

		vec4 colour = CalcLightByDirection(pointLights[i].base, direction);
		float attenuation = pointLights[i].exponent * distance * distance + pointLights[i].linear * distance + pointLights[i].constant;

		totalColour += (colour / attenuation);
	}

	return totalColour;
}

void main()																		
{											
	vec4 finalColour = CalcDirectionalLight();
	finalColour += CalcPointLights();

	colour = texture(theTexture, TexCoord) * finalColour;													
}