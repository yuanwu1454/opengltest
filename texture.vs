#version 330 core

//λ����ɫ������ ֻ�ܱ�������ɫ�������� Ƭ����ɫ��ֻ��ͨ��������ɫ��������
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 aNormal;

//MVP
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//��ɫ������Ƭ��λ��
out vec3 ourColor;
out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

void main()
{
	//out ����
	ourColor = aColor;
	TexCoords = vec2(aTexCoord.x, aTexCoord.y);
	Normal = mat3(transpose(inverse(model))) * aNormal;
	FragPos =  vec3(model * vec4(aPos, 1.0));

	// ����λ�� gl_Position
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
}