#version 330 core

//位置颜色纹理法线 只能被顶点着色器来输入 片段着色器只能通过顶点着色器来传递
layout(location = 0) in vec3 aPos;

//MVP
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	// 顶点位置 gl_Position
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
}