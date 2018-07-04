#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

in vec3 normalPosInView;
in vec3 lightPosInView;
in vec3 FragPosInView;
in vec3 viewPosInView;
in vec3 resultColor;

// texture samplers

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
struct Material {
	sampler2D diffuse1;
	sampler2D diffuse2;
	//sampler2D diffuse1;
	//sampler2D diffuse2;
    //vec3 specular;
	sampler2D specular;
    float shininess;
};

uniform Material material;

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;

void main()
{
	// world 
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	vec3 viewDir = normalize(viewPos - FragPos);
	
	// view
//	vec3 norm = normalize(normalPosInView);
//	vec3 lightDir = normalize(lightPosInView - FragPosInView);
//	vec3 viewDir = normalize(-FragPosInView);
	//vec3 texture3 = vec3(mix(texture(material.diffuse1, TexCoords), texture(material.diffuse2, TexCoords), 0.5));
	vec3 texture3 = vec3(texture(material.diffuse1, TexCoords));
	//ambient
	float ambientStrength = 0.1;
//    vec3 ambient = ambientStrength * light.ambient * material.ambient;
	vec3 ambient = light.ambient * texture3;
	//diffuse
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * light.diffuse *  texture3;;

	//specular
	float specularStrength = 0.5;
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);

	vec3 specular = light.specular * spec *(vec3(1.0f) - vec3(texture(material.specular, TexCoords)));
	//vec3 specular = specularStrength * (spec * material.specular) * light.specular;

	vec3 shine = dot(viewDir, viewDir) * vec3(texture(material.diffuse2, TexCoords));
	//vec3 result = ambient + diffuse + specular;
	vec3 result = (ambient + diffuse + specular + shine);
	FragColor = vec4(1.0);
//	FragColor = mix(texture(texture1, TexCoords), texture(texture2, TexCoords), 0.5);	
	//FragColor = FragColor * vec4(resultColor, 1.0);
	FragColor = FragColor * vec4(result, 1.0);
	//FragColor = vec4(result, 1.0);
	//FragColor = vec4(resultColor, 1.0);
}