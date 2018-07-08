#ifndef UNIFORM_H
#define UNIFORM_H

//��uniform
class Uniform
{
public:
	
	std::map<std::string, glm::vec3> vec3Map;
	std::map<std::string, float> fMap;
	Uniform() {
	}
	void setAttrName(std::string name, glm::vec3 v) {		
		vec3Map[name] = v;
	}
	void setAttrName(std::string name, float v) {
		fMap[name] = v;
	}
	void setAttrName(std::string name, float v[]) { //Ĭ�ϸ���Ϊ3
		vec3Map[name] = glm::vec3(v[0], v[1], v[2]);
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
		for (auto iter = vec3Map.cbegin(); iter != vec3Map.cend(); iter++) {
			programShader.setVec3((uniformName + "." + iter->first).c_str(), (glm::vec3)(iter->second));
		}
		for (auto iter = fMap.cbegin(); iter != fMap.cend(); iter++) {
			programShader.setFloat((uniformName + "." + iter->first).c_str(), iter->second);
		}
	}
};
//vec3 direction; //����
//vec3 ambient;	//��������
//vec3 diffuse;	//������
//vec3 specular;	//���淴��
Uniform createDirLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) {
	Uniform dirlight = Uniform();
	dirlight.setAttrName("direction", direction);
	dirlight.setLightAttrName(ambient, diffuse, specular);
	return dirlight;
}
#endif UNIFORM_H
