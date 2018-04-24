#include "skybox.h"

SkyBoxObject* new_skybox(char* texture, Model* model, mat4 pos) {
  SkyBoxObject* p = malloc(sizeof(SkyBoxObject));
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  LoadTGATextureSimple(texture, &p->texture_id);
  p->model_ptr = model;
  p->position = pos;
  return p;
}

//should move this to file
GLfloat vertices[6][6*3] =
{
	{ // +x
		0.5,-0.5,-0.5,		// 1
		0.5,0.5,-0.5,		// 2
		0.5,0.5,0.5,			// 6
		0.5,-0.5,0.5,		// 5
	},
	{ // -x
		-0.5,-0.5,-0.5,		// 0 -0
		-0.5,-0.5,0.5,		// 4 -1
		-0.5,0.5,0.5,		// 7 -2
		-0.5,0.5,-0.5,		// 3 -3
	},
	{ // +y
		0.5,0.5,-0.5,		// 2 -0
		-0.5,0.5,-0.5,		// 3 -1
		-0.5,0.5,0.5,		// 7 -2
		0.5,0.5,0.5,			// 6 -3
	},
	{ // -y
		-0.5,-0.5,-0.5,		// 0
		0.5,-0.5,-0.5,		// 1
		0.5,-0.5,0.5,		// 5
		-0.5,-0.5,0.5		// 4
	},
	{ // +z
		-0.5,-0.5,0.5,		// 4
		0.5,-0.5,0.5,		// 5
		0.5,0.5,0.5,			// 6
		-0.5,0.5,0.5,		// 7
	},
	{ // -z
		-0.5,-0.5,-0.5,	// 0
		-0.5,0.5,-0.5,		// 3
		0.5,0.5,-0.5,		// 2
		0.5,-0.5,-0.5,		// 1
	}
};

GLfloat texcoord[6][6*2] =
{
	{
		1.0, 1.0,
		1.0, 0.0, // left OK
		0.0, 0.0,
		0.0, 1.0,
	},
	{
		0.0, 1.0, // right OK
		1.0, 1.0,
		1.0, 0.0,
		0.0, 0.0,
	},
	{
		1.0, 0.0, // top OK
		0.0, 0.0,
		0.0, 1.0,
		1.0, 1.0,
	},
	{
		0.0, 1.0,
		1.0, 1.0,
		1.0, 0.0, // bottom
		0.0, 0.0,
	},
	{
		0.0, 1.0,
		1.0, 1.0,
		1.0, 0.0, // back OK
		0.0, 0.0,
	},
	{
		1.0, 1.0,
		1.0, 0.0, // front OK
		0.0, 0.0,
		0.0, 1.0,
	}
};
GLuint indices[6][6] =
{
	{0, 2, 1, 0, 3, 2},
	{0, 2, 1, 0, 3, 2},
	{0, 2, 1, 0, 3, 2},
	{0, 2, 1, 0, 3, 2},
	{0, 2, 1, 0, 3, 2},
	{0, 2, 1, 0, 3, 2}
};

void make_skybox_object(SkyBoxObject** skybox, char* texture1, char* texture2, char* texture3, char* texture4, char* texture5, char* texture6)
{
	Model* box[6];

	for (int i = 0; i < 6; i++)
	{
		box[i] = LoadDataToModel(
			vertices[i],
			NULL,
			texcoord[i],
			NULL,
			indices[i],
			4,
			6);
	}
	printf("Loading texture %s\n", texture1);
	skybox[0] = new_skybox(texture1, box[0], T(0,0,0));
  printf("Loading texture %s\n", texture2);
	skybox[1] = new_skybox(texture2, box[1], T(0,0,0));
  printf("Loading texture %s\n", texture3);
	skybox[2] = new_skybox(texture3, box[2], T(0,0,0));
  printf("Loading texture %s\n", texture4);
	skybox[3] = new_skybox(texture4, box[3], T(0,0,0));
  printf("Loading texture %s\n", texture5);
	skybox[4] = new_skybox(texture5, box[4], T(0,0,0));
  printf("Loading texture %s\n", texture6);
	skybox[5] = new_skybox(texture6, box[5], T(0,0,0));
}

void draw_skybox(SkyBoxObject* object, mat4 worldToViewMatrix, mat4* projectionMatrix, GLuint* shader)
{
	mat4 totMatrix;
  glUseProgram(*shader);
  worldToViewMatrix.m[3] = 0;
	worldToViewMatrix.m[7] = 0;
	worldToViewMatrix.m[11] = 0;
  glUniformMatrix4fv(glGetUniformLocation(*shader, "modelToWorldMatrix"), 1, GL_TRUE, object->position.m);
  glUniformMatrix4fv(glGetUniformLocation(*shader, "worldToViewMatrix"), 1, GL_TRUE, worldToViewMatrix.m);
  glUniformMatrix4fv(glGetUniformLocation(*shader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix->m);

  glUniform1i(glGetUniformLocation(*shader, "texUnit"), GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, object->texture_id);
	DrawModel(object->model_ptr, *shader, "inPosition", NULL, "inTexCoord");
}

void render_skybox(SkyBoxObject** skybox, mat4 worldToViewMatrix, mat4* projectionMatrix, GLuint* shader)
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	for (int i = 0; i<6; i++)
	{
		draw_skybox(skybox[i], worldToViewMatrix, projectionMatrix, shader);
	}
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}
