#pragma once
#include "stdafx.h"
#include <vector>

class Sphere
{
private:
	GLfloat R, G, B;
	GLfloat radius;
	int longitude;
	int latitude;

	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> texCoords;
	std::vector<unsigned int> indices;

public:
	void SetSphere(GLfloat radius, int longitude, int latitude, GLfloat R, GLfloat G, GLfloat B);
	void BeginEndSphere();
	void VertexArraySphere();
	void DrawSphere(int num);
};