#version 330 core
out vec4 FragColor; //�����Ƭ�ν�Ҫ��ʾ����ɫ

in vec3 ourColor;	//��ɫֵ
in vec2 TexCoords;	//��������
in vec3 Normal;		//����
in vec3 FragPos;	//Ƭ��λ��

uniform vec3 objectColor;	//������ɫ
uniform vec3 lightColor;	//������ɫ
uniform vec3 lightPos;		//���λ��
uniform vec3 viewPos;		//�۾�λ��

//���� ��Ϊ �����������䣬���淴�� ���� �����õĲ���
struct Material {
	//�����б���Ͱ�������ɫ���� ʹ��������Ϊ���ַ���Ĳ��� ��ȫ����
	//��������������ⶼ�뷴���������Դ�  ����  ���������뷴����ʿ���һ��
	sampler2D diffuse;
	//����� �뾵����� ��������һ����ز���
	sampler2D specular;
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
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 ambient = light.ambient  * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
	return (ambient + diffuse + specular);
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

#define NR_POINT_LIGHTS 1
uniform PointLight pointLights[NR_POINT_LIGHTS];

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

	// �ϲ����
	vec3 ambient = light.ambient  * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}
/*
	�۹⣺
	�����ص㣺�������������ֵ�Ͳ��Ч��
*/
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

void main()
{
	// ����
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	// ��һ�׶Σ��������
	vec3 result = vec3(0.0f, 0.0f, 0.0f);
	//result += CalcDirLight(dirLight, norm, viewDir);
	// �ڶ��׶Σ����Դ
	for (int i = 0; i < NR_POINT_LIGHTS; i++)
		result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
	// �����׶Σ��۹�
	//result += CalcSpotLight(spotLight, norm, FragPos, viewDir);
	//test
	//result = result * vec3(texture(material.diffuse, TexCoords));
	FragColor = vec4(result, 1.0);
}

//struct DirLight
//{
//	vec3 direction; //����
//	vec3 ambient;	//��������
//	vec3 diffuse;	//������
//	vec3 specular;	//���淴��
//};