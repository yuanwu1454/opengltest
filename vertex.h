#ifndef VERTEX_H
#define VERTEX_H

enum VertexType
{
	ARRAY,
	ELEMENT
};
class CCVertex
{
public:
	glInt VAO, VBO, EBO;
	VertexType _type;
	glInt _drawNum = 0;
	// Camera options
	// Constructor with vectors
	CCVertex(){}
	static CCVertex * create() {
		CCVertex * ret = new (std::nothrow) CCVertex();
		return ret;
	}
	~CCVertex() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}
	void createVertex(vector<float> vertices, vector<int> attribute) {
		//生成缓存对象
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		//绑定VAO
		glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
			bindAttributeVertex(attribute);
		glBindVertexArray(0);
		_drawNum = vertices.size();
		_type = ARRAY;
	}
	void createVertex(vector<float> vertices, vector<glInt> indices, vector<int> attribute) {
		//生成缓存对象
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		//绑定VAO
		glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glInt)*indices.size(), &indices[0], GL_STATIC_DRAW);
			bindAttributeVertex(attribute);
		glBindVertexArray(0);
		_type = ELEMENT;
		_drawNum = indices.size();
	}

	void Draw() {
		glBindVertexArray(VAO);
		switch (_type)
		{
		case ARRAY:
			glDrawArrays(GL_TRIANGLES, 0, _drawNum);
			break;
		case ELEMENT:
			glDrawElements(GL_TRIANGLES, _drawNum, GL_UNSIGNED_INT, 0);
			break;
		default:
			printf("UNKNOWN VERTICE OR NOT BIND VAO\n");
			break;
		}
		glBindVertexArray(0);
	}
protected:

private:
	void bindAttributeVertex(vector<int> attribute) {
		int step = 0;
		for (int i = 0; i < attribute.size(); i++) {
			step = step + attribute[i];
		}
		int initOffset = 0;
		for (int i = 0; i < attribute.size(); i++)
		{
			glVertexAttribPointer(i, attribute[i], GL_FLOAT, GL_FALSE, step * sizeof(float), (void*)(initOffset * sizeof(float)));
			glEnableVertexAttribArray(i);
			initOffset += attribute[i];
		}
	}
};

#endif
glInt createVertices(glInt VAO, glInt* VBO, vector<float> vertices, vector<int> attribute) {
	//生成缓存对象
	glGenBuffers(1, VBO);
	glGenVertexArrays(1, &VAO);

	//绑定VAO
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	int step = 0;
	for (int i = 0; i < attribute.size(); i++) {
		step = step + attribute[i];
	}

	int initOffset = 0;
	for (int i = 0; i < attribute.size(); i++) {
		glVertexAttribPointer(i, attribute[i], GL_FLOAT, GL_FALSE, step * sizeof(float), (void*)(initOffset * sizeof(float)));
		glEnableVertexAttribArray(i);
		initOffset += attribute[i];
	}
	glBindVertexArray(0);
	return VAO;
}

glInt createVerticesWithElements(glInt VAO, glInt* VBO, glInt* EBO, vector<float> vertices, vector<glInt> indices, vector<int> attribute) {
	//生成缓存对象
	glGenBuffers(1, VBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, EBO);

	//绑定VAO
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glInt)*indices.size(), &indices[0], GL_STATIC_DRAW);

	int step = 0;
	for (int i = 0; i < attribute.size(); i++) {
		step = step + attribute[i];
	}
	int initOffset = 0;
	for (int i = 0; i < attribute.size(); i++)
	{
		glVertexAttribPointer(i, attribute[i], GL_FLOAT, GL_FALSE, step * sizeof(float), (void*)(initOffset * sizeof(float)));
		glEnableVertexAttribArray(i);
		initOffset += attribute[i];
	}
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	return VAO;
}

glInt createPointsWithArrays(glInt VAO, glInt* VBO) {
	vector<float> points = {
		-0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // top-left
		0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // top-right
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom-right
		-0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // bottom-left
	};
	vector<int> attribute = {2, 3};
	return createVertices(VAO, VBO, points, attribute);
}

glInt createCellCubeWithArrays(glInt VAO, glInt* VBO) {
	vector<float> vertices = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	vector<int> attribute = { 3 };
	return createVertices(VAO, VBO, vertices, attribute);
}

void createCellCubeWithArrays(CCVertex* vet) {
	vector<float> vertices = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	vector<int> attribute = { 3 };
	vet->createVertex(vertices, attribute);
	//return createVertices(VAO, VBO, vertices, attribute);
}
