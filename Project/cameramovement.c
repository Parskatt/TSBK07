#include "cameramovement.h"

void keyboard(unsigned char c, int x, int y, mat4* worldToViewMatrix, vec3* cam_pos, vec3* cam_dir)
{
	float a = 1;
	vec3 rot = SetVector(0,0,0);
	vec3 front_vec =  SetVector(0,0,-1);
	vec3 back_vec = SetVector(0,0,1);
	vec3 left_vec = SetVector(-1,0,0);
	vec3 right_vec = SetVector(1,0,0);
	switch (c)
	{
	case 27:
		exit(0);
		break;
	case 'w':
		rot = MultMat3Vec3(TransposeMat3(mat4tomat3(*worldToViewMatrix)),front_vec);
		break;
	case 's':
		rot = MultMat3Vec3(TransposeMat3(mat4tomat3(*worldToViewMatrix)),back_vec);
		break;
	case 'a':
		rot = MultMat3Vec3(TransposeMat3(mat4tomat3(*worldToViewMatrix)),left_vec);
		break;
	case 'd':
		rot = MultMat3Vec3(TransposeMat3(mat4tomat3(*worldToViewMatrix)),right_vec);
		break;
	}
	*cam_pos = SetVector(cam_pos->x +a*rot.x,cam_pos->y + a*rot.y,cam_pos->z + a*rot.z);//- b*v.x, cam_pos.y, cam_pos.z+b*v.z);
	*cam_dir = SetVector(cam_dir->x +a*rot.x,cam_dir->y + a*rot.y,cam_dir->z + a*rot.z);
	*worldToViewMatrix = lookAt(cam_pos->x,cam_pos->y,cam_pos->z, cam_dir->x,cam_dir->y,cam_dir->z,0,1,0);
	/*glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, worldToViewMatrix.m);
	glutPostRedisplay();*/ //Needs to be moved
}

void mouse(int button, int state, int x, int y, int* prevx, int* prevy)
{
	if (state == GLUT_DOWN)
	{
		*prevx = x;
		*prevy = y;
	}
	//glutPostRedisplay();
}

void mouseDragged(int x, int y, int* prevx, int* prevy, mat4* worldToViewMatrix, vec3* cam_dir, vec3* cam_pos)
{
	vec3 temp, p={0.0,0.0,0.0};
	mat4 m;

	p.y = *prevx-x;
	p.x = -(y-*prevy);
	p.z = 0;

	mat3 wv3 = TransposeMat3(mat4tomat3(*worldToViewMatrix));
	p = MultMat3Vec3(wv3, p); //InvertMat3
	m = ArbRotate(p, sqrt(p.x*p.x + p.y*p.y) / 50.0);
	temp = SetVector(cam_dir->x - cam_pos->x, cam_dir->y - cam_pos->y,cam_dir->z - cam_pos->z);

	*cam_dir = VectorAdd(MultMat3Vec3(mat4tomat3(m),temp),*cam_pos);
	*worldToViewMatrix = lookAt(cam_pos->x,cam_pos->y,cam_pos->z, cam_dir->x,cam_dir->y,cam_dir->z,0,1,0);
	//glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, worldToViewMatrix.m);

	*prevx = x;
	*prevy = y;
	//glutPostRedisplay();
}
