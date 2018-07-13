#ifndef TEST_H
#define TEST_H

struct myStruct
{
	int a;
	int b;
};
void foo(int x) {
	vector<Vertex> v;
	Vertex v1;
	printf("%d \n", sizeof(v));
	v.push_back(v1);
	printf("%d \n", sizeof(v));
	v.push_back(v1);
	printf("%d", sizeof(v));
	glm::vec3 i;
	printf("%d", sizeof(i));
	printf("\n%d", sizeof(v1.Normal));
	printf("\n%d", sizeof(v1.TexCoords));
	printf("\n%d", sizeof(v1.Position));
	printf("\n%d", sizeof(Vertex));
	printf("\n%d", sizeof(myStruct));
	printf("\n%d", sizeof(myStruct().a));
	//v1(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f));
	//printf("%d", sizeof(v));
}

#endif
