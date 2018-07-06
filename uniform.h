#ifndef UNIFORM_H
#define UNIFORM_H

//将uniform
class Uniform
{
public:
	std::vector <std::pair <std::string, glm::vec3 > > vec3List;
	std::vector <std::pair <std::string, float > > fList;
	Uniform() {
	}
	void setAttrName(std::string name, glm::vec3 v) {
		vec3List.push_back(make_pair(name, v));
	}
	void setAttrName(std::string name, float v) {
		fList.push_back(make_pair(name, v));
	}
	void setAttrName(std::string name, float v[]) { //默认个数为3
		vec3List.push_back(make_pair(name, glm::vec3(v[0], v[1], v[2])));
	}
	void setLightAttrName(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) {
		setAttrName("ambient", ambient);
		setAttrName("diffuse", diffuse);
		setAttrName("specular", specular);
	}

	void setAttenuationAttrName(float constant, float linear, float quadratic) {
		setAttrName("constant", constant);
		setAttrName("linear", linear);
		setAttrName("quadratic", quadratic);
	}

	void setCutAttrName(float cutOff, float outerCutOff) {
		setAttrName("cutOff", cutOff);
		setAttrName("outerCutOff", outerCutOff);
	}

	void bind(Shader programShader, std::string uniformName) {
		programShader.use();
		for (auto iter = vec3List.cbegin(); iter != vec3List.cend(); iter++) {
			programShader.setVec3((uniformName + "." + (*iter).first).c_str(), (glm::vec3)(*iter).second);
		}
		for (auto iter = fList.cbegin(); iter != fList.cend(); iter++) {
			programShader.setFloat((uniformName + "." + (*iter).first).c_str(), (*iter).second);
		}
	}
};
//vec3 direction; //方向
//vec3 ambient;	//环境反射
//vec3 diffuse;	//漫反射
//vec3 specular;	//镜面反射
Uniform createDirLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) {
	Uniform dirlight = Uniform();
	dirlight.setAttrName("direction", direction);
	dirlight.setLightAttrName(ambient, diffuse, specular);
	return dirlight;
}
#endif UNIFORM_H
