#include "particlesystem.h"

// Globals
// Data would normally be read from files
GLfloat vertices_p[] = {
 -0.1f, -0.1f, 0.0f,
 0.1f, -0.1f, 0.0f,
 -0.1f, 0.1f, 0.0f
};

 GLubyte minitex[4][4][3] =
 {
	 { {10, -20, 0}, { 45, 6,1}, { 8, 5,5}, { 2,9,6}},
	 { { -145, 34,78}, {-98, 248,243}, { 11,32,-9}, { -77, 87,-90}},
	 { { -80, 65,44}, { 50,255,67}, {-76, -50,0}, { 50, -50,5}},
	 { { 50,-255,33}, { -214, 9,0}, { -158, -66,52}, {-55, 50,44}},
 };


 GLfloat texcoord_p[36*2] = {
 						1.0, 0.0,
 						0.0, 0.0,
 						0.0, 1.0,
 						1.0, 0.0,
 						0.0, 1.0,
 						1.0, 1.0,

 						1.0, 0.0,
 						0.0, 0.0,
 						0.0, 1.0,
 						1.0, 0.0,
 						0.0, 1.0,
 						1.0, 1.0,

 						1.0, 0.0,
 						0.0, 0.0,
 						0.0, 1.0,
 						1.0, 0.0,
 						0.0, 1.0,
 						1.0, 1.0,

 						1.0, 0.0,
 						0.0, 0.0,
 						0.0, 1.0,
 						1.0, 0.0,
 						0.0, 1.0,
 						1.0, 1.0,

 						1.0, 0.0,
 						0.0, 0.0,
 						0.0, 1.0,
 						1.0, 0.0,
 						0.0, 1.0,
 						1.0, 1.0,

 						1.0, 0.0,
 						0.0, 0.0,
 						0.0, 1.0,
 						1.0, 0.0,
 						0.0, 1.0,
 						1.0, 1.0,
 					};


void init_particles(int count, int height, GLuint* shader, unsigned int *vertexArrayObjID)
{
	unsigned int vertexBufferObjID;
	unsigned int texCoordBufferObjID;
	GLuint minitexid;
  int width = 4;

	// Load and compile shader
	glUseProgram(*shader);

	// Upload geometry to the GPU:
	glGenVertexArrays(1, &vertexArrayObjID); // Allocate and activate Vertex Array Object
	glBindVertexArray(vertexArrayObjID);
	glGenBuffers(1, &vertexBufferObjID); // Allocate Vertex Buffer Objects
	glGenBuffers(1, &texCoordBufferObjID);

	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_p), vertices_p, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(*shader, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(*shader, "in_Position"));
	// VBO for texCoord data
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 36*2*sizeof(GLfloat), texcoord_p, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(*shader, "inTexCoord"), 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(*shader, "inTexCoord"));
	// End of upload of geometry

	glUniform1i(glGetUniformLocation(*shader, "tex"), 0);
	glUniform1f(glGetUniformLocation(*shader, "count"), count); // Upload variables
	glUniform1f(glGetUniformLocation(*shader, "width"), width);
	glUniform1f(glGetUniformLocation(*shader, "height"), height);

	// init minitex
	glGenTextures(1, &minitexid); // texture id
	glBindTexture(GL_TEXTURE_2D, minitexid);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 4, 4, 0, GL_RGB, GL_UNSIGNED_BYTE, minitex);
	printError("init arrays");
}


void render_rainy_cloud(int count, GLfloat a, GLuint* shader, unsigned int *vertexArrayObjID)
{
	mat4 trans;
	glUniform1f(glGetUniformLocation(*shader, "time"), a);
	trans = T(-0.1, 10, 10);
	glUniformMatrix4fv(glGetUniformLocation(*shader, "myMatrix"), 1, GL_TRUE, trans.m);
	glBindVertexArray(vertexArrayObjID);	// Select VAO
	glDrawArraysInstanced(GL_TRIANGLES, 0, 4, count); //points
  glEnable(GL_DEPTH_TEST);
	glutSwapBuffers();
}
