#version 330 core
out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
void main()
{
//    float ambientStrength = 0.1;
//    vec3 ambient = ambientStrength * lightColor;

//    vec3 result = ambient * objectColor;
//    FragColor = vec4(result, 1.0);

   FragColor = vec4(1.0); // 将向量的四个分量全部设置为1.0
}