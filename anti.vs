#version 330 core

//λ����ɫ������ ֻ�ܱ�������ɫ�������� Ƭ����ɫ��ֻ��ͨ��������ɫ��������
layout(location = 0) in vec3 aPos;

//MVP
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	// ����λ�� gl_Position
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
}