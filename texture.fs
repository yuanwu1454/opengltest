#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

in vec3 normalPosInView;
in vec3 lightPosInView;
in vec3 FragPosInView;
in vec3 viewPosInView;
in vec3 resultColor;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

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

	//ambient
	float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

	//diffuse
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	//specular
	float specularStrength = 0.5;
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
	vec3 specular = specularStrength * spec * lightColor;
	
	vec3 result = (ambient + diffuse + specular) * objectColor;

	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.5);	
	//FragColor = FragColor * vec4(resultColor, 1.0);
	//FragColor = FragColor * vec4(result, 1.0);
	FragColor = vec4(result, 1.0);
	//FragColor = vec4(resultColor, 1.0);
}