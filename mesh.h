#ifndef MESH_H
#define MESH_H
struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	//glm::vec3 Tangent;
	//glm::vec3 Bitangent;
};

struct Texture {
	glInt id;
	std::string type;
	aiString path;
};

class Mesh {
public:
	/*  ��������  */
	std::vector<Vertex> vertices;
	std::vector<glInt> indices;
	std::vector<Texture> textures;
	/*  ����  */
	Mesh(std::vector<Vertex> vertices, std::vector<glInt> indices, std::vector<Texture> textures) {
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;
		setupMesh();
	}
	void Draw(Shader shader) {
		shader.use();
		//�˴������� ����id �Ѿ����������
		glInt diffuseNr = 1;
		glInt specularNr = 1;
		
		for (glInt i = 0; i < textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i); // �ڰ�֮ǰ������Ӧ������Ԫ
											  // ��ȡ������ţ�diffuse_textureN �е� N��
			std::string number;
			std::string name = textures[i].type;
			if (name == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if (name == "texture_specular")
				number = std::to_string(specularNr++);
			
			shader.setFloat(("material." + name + number).c_str(), i);
			glBindTexture(GL_TEXTURE_2D, textures[i].id);
		}

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
private:
	/*  ��Ⱦ����  */
	glInt VAO, VBO, EBO;
	/*  ����  */
	void setupMesh() {
		//���ɻ������
		glGenBuffers(1, &VBO);
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &EBO);

		//��VAO
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); 
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(glInt), &indices[0], GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glEnableVertexAttribArray(0);


		// normal attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Normal)));
		glEnableVertexAttribArray(1);

		// texture coord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, TexCoords)));
		glEnableVertexAttribArray(2);
		
		//glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
		//glEnableVertexAttribArray(3);

		//glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
		//glEnableVertexAttribArray(4);

		//unbind
		glBindVertexArray(0);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
};
#endif MESH_H