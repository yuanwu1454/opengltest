#version 330 core

//位置颜色纹理法线 只能被顶点着色器来输入 片段着色器只能通过顶点着色器来传递
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 aNormal;

//MVP
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//颜色纹理法线片段位置
out vec3 ourColor;
out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

void main()
{
	//out 变量
	ourColor = aColor;
	TexCoords = vec2(aTexCoord.x, aTexCoord.y);
	Normal = mat3(transpose(inverse(model))) * aNormal;
	FragPos =  vec3(model * vec4(aPos, 1.0));

	// 顶点位置 gl_Position
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
}