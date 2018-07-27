#version 330 core
out vec4 FragColor; //这个是片段将要显示的颜色

uniform vec3 color;
void main()
{
	FragColor = vec4(color, 1.0);
}