#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 aNormal;
uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

out vec3 ourColor;
out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;
out vec3 normalPosInView;
out vec3 lightPosInView;  
out vec3 FragPosInView;  
out vec3 viewPosInView;  
out vec3 resultColor;
void main()
{
	//gl_Position = transform * vec4(aPos, 1.0f);
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	ourColor = aColor;
	//Normal = vec3( projection * view * model * vec4(aNormal, 1.0));
	TexCoords = vec2(aTexCoord.x, aTexCoord.y);

	//worldPos
	Normal = mat3(transpose(inverse(model))) * aNormal;
	FragPos =  vec3(model * vec4(aPos, 1.0));

	//viewPos
	//FragPosInView = vec3(view * model * vec4(aPos, 1.0f));
	//normalPosInView = mat3(transpose(inverse(view * model))) * aNormal;
	//lightPosInView = vec3(view * vec4(lightPos, 1.0f));


	// world 
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	vec3 viewDir = normalize(viewPos - FragPos);

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
	
	resultColor = (ambient + diffuse + specular) * objectColor;
}