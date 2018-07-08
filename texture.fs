#version 330 core
out vec4 FragColor; //这个是片段将要显示的颜色

in vec3 ourColor;	//颜色值
in vec2 TexCoords;	//纹理坐标
in vec3 Normal;		//法线
in vec3 FragPos;	//片段位置

uniform vec3 objectColor;	//物体颜色
uniform vec3 lightColor;	//光照颜色
uniform vec3 lightPos;		//光的位置
uniform vec3 viewPos;		//眼睛位置

//材质 分为 环境，漫反射，镜面反射 三种 所作用的材质
struct Material {
	//纹理中本身就包含了颜色属性 使用纹理作为三种反射的材质 完全可以
	//环境光与漫反射光都与反射材质相关性大  所用  环境材质与反射材质可以一样
	sampler2D diffuse;
	//镜面光 与镜面材质 则是另外一组相关参数
	sampler2D specular;
    float shininess;	//反光度  主要作用于 镜面反射
};
uniform Material material; //被照物体材质

/*
	漫反射：与物体表面的法线相关，本质就是将光线的颜色与物体颜色的融合之后，将融合之后的值的一部分显示给别人看，
	法线与光线方向的点积即为这一部分的值
	镜面反射:  与物体表面的法线 和视角的方向都成正相关
	环境反射： 与光线方向无关  只和光颜色与物体颜色相关
*/


/*
	方向光源
	最显著的特点就是 方向光源对所有片段的方向都是一样的
*/
//方向光源
struct DirLight
{
	vec3 direction; //方向
	vec3 ambient;	//环境反射
	vec3 diffuse;	//漫反射
	vec3 specular;	//镜面反射
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
	点光源
	显著特点就是对于一个片段来说 都要重新计算对应的光的入射方向
	朝四周进行光照
	衰减
	效果就是存在一个光强度的变化 使用一个二次方程表示
*/
//点光源
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
	// 漫反射着色
	float diff = max(dot(normal, lightDir), 0.0);
	// 镜面光着色
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// 衰减
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance +
		light.quadratic * (distance * distance));

	// 合并结果
	vec3 ambient = light.ambient  * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}
/*
	聚光：
	显著特点：呈现了类似于手电筒的效果
*/
struct SpotLight {
	vec3 position;
	vec3 direction;
	float cutOff;		//内切角
	float outerCutOff;  //外切角

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
	// 漫反射着色
	float diff = max(dot(normal, lightDir), 0.0);
	// 镜面光着色
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// 衰减
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance +
		light.quadratic * (distance * distance));

	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	float influence = intensity * attenuation;
	// 合并结果
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
	// 属性
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	// 第一阶段：定向光照
	vec3 result = vec3(0.0f, 0.0f, 0.0f);
	//result += CalcDirLight(dirLight, norm, viewDir);
	// 第二阶段：点光源
	for (int i = 0; i < NR_POINT_LIGHTS; i++)
		result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
	// 第三阶段：聚光
	//result += CalcSpotLight(spotLight, norm, FragPos, viewDir);
	//test
	//result = result * vec3(texture(material.diffuse, TexCoords));
	FragColor = vec4(result, 1.0);
}

//struct DirLight
//{
//	vec3 direction; //方向
//	vec3 ambient;	//环境反射
//	vec3 diffuse;	//漫反射
//	vec3 specular;	//镜面反射
//};