#include "Sphere.h"
#include <iostream>

void Sphere::SetSphere(GLfloat radius, int longitude, int latitude, GLfloat R, GLfloat G, GLfloat B)
{
	this->radius = radius;
	this->longitude = longitude;
	this->latitude = latitude;
	this->R = R;
	this->G = G;
	this->B = B;
}

void Sphere::BeginEndSphere()
{
	const float PI = 3.141592f;

	GLfloat x, y, z, pi, theta;
	float nx, ny, nz;
	float lengthInv = 1.0f / radius;
	int space = 30;
	
	glColor3f(R, G, B);
	glBegin(GL_TRIANGLE_STRIP);
	for (pi = 0; pi <PI; pi += PI/space)
	{
		for (theta = 0; theta <2*PI; theta += PI/space)
		{
			x = radius * cosf(theta)*sinf(pi);
			y = radius * sinf(theta)*sinf(pi);
			z = radius * cosf(pi);
			glVertex3f(x, y, z);
			nx = x * lengthInv;
			ny = y * lengthInv;
			nz = z * lengthInv;
			glNormal3f(nx, ny, nz);

			x = radius * cosf(theta)*sinf(pi + PI / space);
			y = radius * sinf(theta)*sinf(pi + PI / space);
			z = radius * cosf(pi + PI / space);
			glVertex3f(x, y, z);
			nx = x * lengthInv;
			ny = y * lengthInv;
			nz = z * lengthInv;
			glNormal3f(nx, ny, nz);
		}
	}
	glEnd();
}

void Sphere::VertexArraySphere()
{
	const float PI = 3.141592f;

	std::vector<float>().swap(vertices);
	std::vector<float>().swap(normals);
	std::vector<float>().swap(texCoords);
	std::vector<unsigned int>().swap(indices);

	float x, y, z, xy;
	float sliceLongitude = 2 * PI / longitude; // a circle
	float sliceLatitude = PI / latitude; // half circle
	float longitudeAngle, latitudeAngle;
	float lengthInv = 1.0f / radius;
	float nx, ny, nz;
	float s, t;
	
	for (int i = 0; i <= latitude; ++i)
	{
		latitudeAngle = PI / 2 - i * sliceLatitude;
		xy = radius * cosf(latitudeAngle);
		z = radius * sinf(latitudeAngle);
		for (int j = 0; j <= longitude; ++j)
		{
			longitudeAngle = j * sliceLongitude;
			x = xy * cosf(longitudeAngle);
			y = xy * sinf(longitudeAngle);
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);

			nx = x * lengthInv;
			ny = y * lengthInv;
			nz = z * lengthInv;
			normals.push_back(nx);
			normals.push_back(ny);
			normals.push_back(nz);

			s = (float)j / longitude;
			t = (float)i / latitude;
			texCoords.push_back(s);
			texCoords.push_back(t);
		}
	}

	unsigned int point1, point2;
	for (int i = 0; i < latitude; ++i)
	{
		point1 = i * (longitude + 1);
		point2 = point1 + longitude + 1;
		for (int j = 0; j < longitude; ++j, ++point1, ++point2)
		{
			if (i != 0)
			{
				indices.push_back(point1);
				indices.push_back(point2);
				indices.push_back(point1 + 1);
			}
			if (i != latitude - 1)
			{
				indices.push_back(point1 + 1);
				indices.push_back(point2);
				indices.push_back(point2 + 1);
			}
		}
	}
}

void Sphere::DrawSphere(int num)
{
	if (num == 1)
	{
		BeginEndSphere();
	}
	else if (num == 2)
	{
		VertexArraySphere();
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
		glNormalPointer(GL_FLOAT, 0, &normals[0]);
		glTexCoordPointer(2, GL_FLOAT, 0, &texCoords[0]);
		glDrawElements(GL_TRIANGLES, (unsigned int)indices.size(), GL_UNSIGNED_INT, indices.data());
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
}