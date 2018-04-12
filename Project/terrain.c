#include "terrain.h"

TerrainObject* new_terrain(char* texture, mat4 pos, int height_scale)
{
  TextureData ttex;
  TerrainObject* out_terrain = malloc(sizeof(TerrainObject));
  LoadTGATextureData("Textures/cave_terrain.tga", &ttex);
  LoadTGATextureSimple(texture, &out_terrain->texture_id);
  out_terrain->model_ptr = GenerateTerrain(&ttex, height_scale);
  out_terrain->position = pos;
  return out_terrain;
}

void render_terrain(TerrainObject* object, mat4* worldToViewMatrix, mat4* projectionMatrix, GLuint* shader)
{
  mat4 totMatrix;
  glUseProgram(*shader); //program used when drawing octagon
	totMatrix = Mult(*projectionMatrix,Mult(*worldToViewMatrix,object->position));
	glUniformMatrix4fv(glGetUniformLocation(*shader, "totMatrix"), 1, GL_TRUE, totMatrix.m);
  glUniform1i(glGetUniformLocation(*shader, "texUnit"), GL_TEXTURE0);//Right now texture 0 is hardcoded, change later
  glBindTexture(GL_TEXTURE_2D, object->texture_id);//Maybe dont bind every time? (Karin pls fix)
	DrawModel(object->model_ptr, *shader, "inPosition", "inNormal", "inTexCoord");
}

Model* GenerateTerrain(TextureData *tex, int height_scale)
{
	int vertexCount = tex->width * tex->height;
	int triangleCount = (tex->width-1) * (tex->height-1) * 2;
	int x, z;
	vec3 p0 = SetVector(0.0,0.0,0.0);
	vec3 p1 = SetVector(0,0,0);
	vec3 p2 = SetVector(0,0,0);
	vec3 n = SetVector(0,1,0);

	GLfloat *vertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *normalArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *texCoordArray = malloc(sizeof(GLfloat) * 2 * vertexCount);
	GLuint *indexArray = malloc(sizeof(GLuint) * triangleCount*3);

	printf("bpp %d\n", tex->bpp);
	printf("triangles: %d\n", triangleCount);

	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{
// Vertex array. You need to scale this properly
			vertexArray[(x + z * tex->width)*3 + 0] = x / 1.0;
			vertexArray[(x + z * tex->width)*3 + 1] = tex->imageData[(x + z * tex->width) * (tex->bpp/8)] / height_scale;
			vertexArray[(x + z * tex->width)*3 + 2] = z / 1.0;
// Normal vectors. You need to calculate these.

			if (x > 0 && z > 0)
			{
				p0 = SetVector((x-1.0)/1.0, tex->imageData[(x-1 + z * tex->width) * (tex->bpp/8)] / 20.0, z / 1.0);
				p1 = SetVector((x+1.0)/1.0, tex->imageData[(x+1 + z * tex->width) * (tex->bpp/8)] / 20.0, z / 1.0);
				p2 = SetVector(x/1.0, tex->imageData[(x + (z-1) * tex->width) * (tex->bpp/8)] / 20.0, (z-1) / 1.0);
				n = CrossProduct(VectorSub(p2, p1), VectorSub(p0, p1));
				n = Normalize(n);
			}
			else
			{
				n = SetVector(0.0, 1.0, 0.0);
			}

			normalArray[(x + z * tex->width)*3 + 0] = n.x; //0.0;
			normalArray[(x + z * tex->width)*3 + 1] = n.y;//n.y; //1.0;
			normalArray[(x + z * tex->width)*3 + 2] = n.z; //0.0;
// Texture coordinates. You may want to scale them.
			texCoordArray[(x + z * tex->width)*2 + 0] = x; // (float)x / tex->width;
			texCoordArray[(x + z * tex->width)*2 + 1] = z; // (float)z / tex->height;
		}
	for (x = 0; x < tex->width-1; x++)
		for (z = 0; z < tex->height-1; z++)
		{
		// Triangle 1
			indexArray[(x + z * (tex->width-1))*6 + 0] = x + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 1] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 2] = x+1 + z * tex->width;
		// Triangle 2
			indexArray[(x + z * (tex->width-1))*6 + 3] = x+1 + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 4] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 5] = x+1 + (z+1) * tex->width;
		}

	// End of terrain generation

	// Create Model and upload to GPU:

	Model* model = LoadDataToModel(
			vertexArray,
			normalArray,
			texCoordArray,
			NULL,
			indexArray,
			vertexCount,
			triangleCount*3);

	return model;
}

/*
main : main.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o main -I$(commondir) -I$(commondir)Linux -DGL_GLEXT_PROTOTYPES main.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c cameramovement.c load_shaders.c objects.c terrain.c -lXt -lX11 -lGL -lm
*/
