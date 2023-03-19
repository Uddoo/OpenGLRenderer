#version 330

in vec4 vCol;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 colour;

struct DirectionalLight
{
	vec3 colour;
	float ambientIntensity;
	vec3 direction;
	float diffuseIntensity;
};

struct Material
{
	float specularIntensity;
	float shininess;
};
  
uniform sampler2D theTexture;
uniform DirectionalLight directionalLight;
uniform Material material;

uniform vec3 eyePosition;

void main()																		
{											
	vec4 ambientColour = vec4(directionalLight.colour, 1.0f) * directionalLight.ambientIntensity;

	float diffuseFactor = max(dot(normalize(Normal), normalize(directionalLight.direction)), 0.0f);
	vec4 diffuseColour = vec4(directionalLight.colour, 1.0f) * directionalLight.diffuseIntensity * diffuseFactor;

	vec4 specularColour = vec4(0, 0, 0, 0);

	if(diffuseFactor > 0.0f) // Only calculate specular if there is diffuse light
	{
		vec3 fragToEye = normalize(eyePosition - FragPos); // Direction from fragment to eye
		vec3 reflectVertex = normalize(reflect(directionalLight.direction, normalize(Normal))); // Direction of reflection

		float specularFactor = dot(fragToEye, reflectVertex); // Calculate specular factor
		if(specularFactor > 0.0f)
		{
			specularFactor = pow(specularFactor, material.shininess); // Calculate specular factor
			specularColour = vec4(directionalLight.colour, 1.0f) * material.specularIntensity * specularFactor; // Calculate specular colour, 颜色 * 光照强度 * 镜面反射因子
			//specularColour = vec4(directionalLight.colour * material.specularIntensity * specularFactor, 1.0f);
		}
	}

	colour = texture(theTexture, TexCoord) * (ambientColour + diffuseColour + specularColour);													
}