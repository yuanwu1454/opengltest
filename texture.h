#ifndef TEXTURE_H
#define TEXTURE_H

glInt generateTexture(const char* resName, bool alpha = false) {
	glInt textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	// Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
	if (alpha) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// ���ز���������
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); //��תy��
	unsigned char *data = stbi_load(resName, &width, &height, &nrChannels, 0);
	std::string fileExtension;

	string filename = resName;
	size_t pos = filename.find_last_of('.');
	if (pos != std::string::npos)
	{
		fileExtension = filename.substr(pos, filename.length());
	}
	glInt rgb = GL_RGB;
	if (fileExtension == ".png") {
		rgb = GL_RGBA;
	}
	glInt modAlpha = GL_RGB;
	if (alpha) {
		modAlpha = GL_RGBA;
	}

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, modAlpha, width, height, 0, rgb, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	return textureId;
}

glInt TextureFromFile(const char* resName, std::string directory) {
	std::string resStr(resName);
	return generateTexture((directory + "/" + resStr).c_str());
}

unsigned int loadCubemap(vector<std::string> faces, std::string directory)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load((directory + "/" + faces[i]).c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

class TextureCache
{
public:
	map<std::string, glInt> texureIDCache;
	TextureCache() {
	}
	
	void cacheTexture(std::string name, bool alpha = false) {
		glInt textureID = generateTexture(name.c_str(), alpha);
		texureIDCache[name] = textureID;
	}

	glInt getCacheTextureID(const char* str) {
		glInt textureID = texureIDCache[str];
		return textureID;
	}
private:
};
#endif