#version 330 core
out vec4 FragColor; //�����Ƭ�ν�Ҫ��ʾ����ɫ

in vec2 TexCoords;	//��������
in vec3 Normal;		//����
in vec3 FragPos;	//Ƭ��λ��

uniform samplerCube skybox;
uniform vec3 viewPos;
struct Material {
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
	sampler2D texture_ambient1;
	float shininess;	//�����  ��Ҫ������ ���淴��
};
uniform Material material; //�����������

struct DirLight
{
	vec3 direction; //����
	vec3 ambient;	//��������
	vec3 diffuse;	//������
	vec3 specular;	//���淴��
};
uniform DirLight dirLight;

vec4 CalcCubeLight(DirLight light, vec3 normal, vec3 viewDir) {
	vec3 R = reflect(-viewDir, normal);
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	vec3 ambient = light.ambient  * vec3(texture(material.texture_ambient1, TexCoords));
	vec3 diffuse = light.diffuse  * diff * vec3(texture(material.texture_diffuse1, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));

	vec4 reflect_color = vec4(0.0);
	vec3 temp = ambient;
	if (temp.r > 0.1 && temp.g < 0.01 && temp.b < 0.01)
		reflect_color = texture(skybox, R) ;
	return vec4(ambient + diffuse + specular, 1.0) + reflect_color;
}

void main()
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);
	FragColor = CalcCubeLight(dirLight, norm, viewDir);
}