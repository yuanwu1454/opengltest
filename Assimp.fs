#version 330 core
out vec4 FragColor; //�����Ƭ�ν�Ҫ��ʾ����ɫ

in vec2 TexCoords;	//��������
in vec3 Normal;		//����
in vec3 FragPos;	//Ƭ��λ��

uniform samplerCube skybox;
uniform vec3 objectColor;	//������ɫ
uniform vec3 lightColor;	//������ɫ
uniform vec3 lightPos;		//���λ��
uniform vec3 viewPos;		//�۾�λ��

							//���� ��Ϊ �����������䣬���淴�� ���� �����õĲ���
struct Material {
	//�����б���Ͱ�������ɫ���� ʹ��������Ϊ���ַ���Ĳ��� ��ȫ����
	//��������������ⶼ�뷴���������Դ�  ����  ���������뷴����ʿ���һ��
	sampler2D texture_diffuse1;
	//����� �뾵����� ��������һ����ز���
	sampler2D texture_specular1;

	//�������� ���µ�ģ���а������ ��������ͼ��
	sampler2D texture_ambient1;

	float shininess;	//�����  ��Ҫ������ ���淴��
};
uniform Material material; //�����������

/*
�����䣺���������ķ�����أ����ʾ��ǽ����ߵ���ɫ��������ɫ���ں�֮�󣬽��ں�֮���ֵ��һ������ʾ�����˿���
��������߷���ĵ����Ϊ��һ���ֵ�ֵ
���淴��:  ���������ķ��� ���ӽǵķ��򶼳������
�������䣺 ����߷����޹�  ֻ�͹���ɫ��������ɫ���
*/


/*
�����Դ
���������ص���� �����Դ������Ƭ�εķ�����һ����
*/
//�����Դ
struct DirLight
{
	vec3 direction; //����
	vec3 ambient;	//��������
	vec3 diffuse;	//������
	vec3 specular;	//���淴��
};

uniform DirLight dirLight;
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {


	//����
		vec3 R = reflect(-viewDir, normal); //����
	//����
	//	float ratio = 1.00 / 1.52;
	//	vec3 R = refract(-viewDir, normalize(Normal), ratio); 
	vec3 sky = texture(skybox, R).rgb;
	//	FragColor = vec4(texture(skybox, R).rgb + result, 1.0);

	//FragColor = vec4(result, 1.0);

	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	//δʹ�ù����ɫ ʹ�õ���������������ʾ
	//vec3 ambient = light.ambient  * vec3(texture(material.texture_ambient1, TexCoords));
	vec3 ambient = vec3(texture(material.texture_ambient1, TexCoords));
	//vec3 ambientColor = vec3(texture(material.texture_ambient1, TexCoords));
	//vec3 ambient = vec3(light.ambient.x * ambientColor.x, light.ambient.y * ambientColor.y, light.ambient.z * ambientColor.z);
	vec3 diffuse = light.diffuse  * diff * vec3(texture(material.texture_diffuse1, TexCoords));
	//vec3 diffuse = light.diffuse  * diff * vec3(texture(skybox, R));
	//vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));
	vec3 specular = light.specular  * spec * vec3(texture(skybox, R));

	//return (ambient + diffuse + specular + vec3(0.0));
	//diffuse = vec3(1, 1, 1);
	//return sky + vec3(0.1);
	//return vec3(diffuse.x*sky.r, diffuse.y*sky.g, diffuse.z*sky.b);
	//return diffuse;
	return ambient + diffuse + specular;
	//return vec3(0.0);
}

vec4 CalcCubeLight(DirLight light, vec3 normal, vec3 viewDir) {


	//����
	vec3 R = reflect(-viewDir, normal); //����
										//����
										//	float ratio = 1.00 / 1.52;
										//	vec3 R = refract(-viewDir, normalize(Normal), ratio); 
	//vec4 sky = texture(skybox, R);

	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	//δʹ�ù����ɫ ʹ�õ���������������ʾ
	vec3 ambient = light.ambient  * vec3(texture(material.texture_ambient1, TexCoords));
	//vec3 ambient = vec3(texture(material.texture_ambient1, TexCoords));
	//vec3 ambientColor = vec3(texture(material.texture_ambient1, TexCoords));
	//vec3 ambient = vec3(light.ambient.x * ambientColor.x, light.ambient.y * ambientColor.y, light.ambient.z * ambientColor.z);
	vec3 diffuse = light.diffuse  * diff * vec3(texture(material.texture_diffuse1, TexCoords));
	//vec3 diffuse = light.diffuse  * diff * vec3(texture(skybox, R));
	vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));
	//vec3 specular = light.specular  * spec * vec3(texture(skybox, R));
	
	//float reflect_intensity = texture(material.texture_specular1, TexCoords).r;
	//vec4 reflect_color;
	//if (reflect_intensity > 0.1) // Only sample reflections when above a certain treshold
	//	reflect_color = texture(skybox, R) * reflect_intensity;

	//return (ambient + diffuse + specular + vec3(0.0));
	//diffuse = vec3(1, 1, 1);
	//return sky + vec3(0.1);
	//return vec3(diffuse.x*sky.r, diffuse.y*sky.g, diffuse.z*sky.b);
	//return diffuse;
	//return vec3(0.0);
	//if ambient.r > 0.0 

	vec4 reflect_color = vec4(0.0);
	vec3 temp = ambient;
//	if (temp.r > 0.9 && temp.g > 0.1 && temp.b > 0.1) // Only sample reflections when above a certain treshold
	if (temp.r > 0.1 && temp.g < 0.01 && temp.b < 0.01)
		reflect_color = texture(skybox, R) ;
	return vec4(ambient + diffuse + specular, 1.0) + reflect_color;
	//return vec4(diffuse, 1.0); 
	//return vec4(ambient, 1.0) + reflect_color + vec4(specular, 1.0) + vec4(diffuse, 0.0);
	//return reflect_color + vec4(diffuse, 0.0);
}
/*
���Դ
�����ص���Ƕ���һ��Ƭ����˵ ��Ҫ���¼����Ӧ�Ĺ�����䷽��
�����ܽ��й���
˥��
Ч�����Ǵ���һ����ǿ�ȵı仯 ʹ��һ�����η��̱�ʾ
*/
//���Դ

struct PointLight {
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

vec3 clampVec3(vec3 value) {
	return vec3(clamp(value.x, 0.0f, 1.0f), clamp(value.y, 0.0f, 1.0f), clamp(value.z, 0.0f, 1.0f));
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 lightDir = normalize(light.position - fragPos);
	// ��������ɫ
	float diff = max(dot(normal, lightDir), 0.0);
	// �������ɫ
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// ˥��
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance +
		light.quadratic * (distance * distance));
	attenuation = clamp(attenuation, 0.0f, 1.0f);

	// �ϲ����
	vec3 ambient = light.ambient  * vec3(texture(material.texture_diffuse1, TexCoords));
	vec3 diffuse = light.diffuse  * diff * vec3(texture(material.texture_diffuse1, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}

/*
�۹⣺
�����ص㣺�������������ֵ�Ͳ��Ч��
*/
/*
struct SpotLight {
	vec3 position;
	vec3 direction;
	float cutOff;		//���н�
	float outerCutOff;  //���н�

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
#define NR_SPOT_LIGHTS 4
uniform SpotLight spotLights[NR_SPOT_LIGHTS];

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 lightDir = normalize(light.position - fragPos);
	// ��������ɫ
	float diff = max(dot(normal, lightDir), 0.0);
	// �������ɫ
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// ˥��
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance +
		light.quadratic * (distance * distance));

	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	float influence = intensity * attenuation;
	// �ϲ����
	vec3 ambient = light.ambient  * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

	ambient *= influence;
	diffuse *= influence;
	specular *= influence;
	return (ambient + diffuse + specular);
}
*/
void main()
{
	// ����
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);


	//���ʹ��

	// ��һ�׶Σ��������
	vec3 result = vec3(0.0f, 0.0f, 0.0f);
	FragColor = CalcCubeLight(dirLight, norm, viewDir);
	//result += clampVec3(CalcDirLight(dirLight, norm, viewDir));
	// �ڶ��׶Σ����Դ
	//for (int i = 0; i <NR_POINT_LIGHTS; i++)
	//	result += clampVec3(CalcPointLight(pointLights[i], norm, FragPos, viewDir));
	// �����׶Σ��۹�
	//result += CalcSpotLight(spotLight, norm, FragPos, viewDir);



	//����
	//vec3 R = reflect(-viewDir, normalize(Normal)); //����
	//����
//	float ratio = 1.00 / 1.52;
//	vec3 R = refract(-viewDir, normalize(Normal), ratio); 

	//FragColor = vec4(texture(skybox, R).rgb + result, 1.0);


	//// Diffuse
	//vec4 diffuse_color = texture(material.texture_diffuse1, TexCoords);
	//// Reflection
	//vec3 R = reflect(-viewDir, normalize(Normal));
	//float reflect_intensity = texture(material.texture_specular1, TexCoords).r;
	//vec4 reflect_color;
	//if (reflect_intensity > 0.1) // Only sample reflections when above a certain treshold
	//	reflect_color = texture(skybox, R) * reflect_intensity;
	//// Combine them
	//FragColor = diffuse_color + reflect_color;
	//FragColor = vec4(result, 1.0);
}