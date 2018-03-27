#include "cameramovement.h"

void camera_init(vec3 *cam_pos,vec3 *cam_dir,mat4 *projectionMatrix,mat4 *worldToViewMatrix)
{
	*cam_pos = SetVector(0, 0, 0);
	*cam_dir = SetVector(0, 0, 10);
	*worldToViewMatrix = lookAt(cam_pos->x, cam_pos->y, cam_pos->z, cam_dir->x, cam_dir->y, cam_dir->z, 0,1,0);
	*projectionMatrix = frustum(-0.16, 0.16, -0.09, 0.09, 0.2, 1000.0);
}

void keyboard(unsigned char c, int x, int y, mat4* worldToViewMatrix, vec3* cam_pos, vec3* cam_dir)
{
	static float a = 0.05;
	vec3 direction = SetVector(0,0,0);
	switch (c)
	{
	case 27:
		exit(0);
		break;
	case 'w':
		direction = SetVector(0,0,-1);
		break;
	case 's':
		direction = SetVector(0,0,1);
		break;
	case 'a':
		direction = SetVector(-1,0,0);
		break;
	case 'd':
		direction = SetVector(1,0,0);
		break;
	case 'f': //For fullscreen
		glutToggleFullScreen();
		break;
	default:
		direction = SetVector(0,0,0);
		a = 0.0;
		break;
	}
	vec3 rot = MultMat3Vec3(TransposeMat3(mat4tomat3(*worldToViewMatrix)),direction);
	//*cam_speed = SetVector(cam_speed->x +a*rot.x,cam_speed->y + a*rot.y,cam_speed->z + a*rot.z);
	*cam_pos = SetVector(cam_pos->x +a*rot.x,cam_pos->y + a*rot.y,cam_pos->z + a*rot.z);//- b*v.x, cam_pos.y, cam_pos.z+b*v.z);
	*cam_dir = SetVector(cam_dir->x +a*rot.x,cam_dir->y + a*rot.y,cam_dir->z + a*rot.z);
	*worldToViewMatrix = lookAt(cam_pos->x,cam_pos->y,cam_pos->z, cam_dir->x,cam_dir->y,cam_dir->z,0,1,0);
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y, int* prevx, int* prevy)
{
	if (state == GLUT_DOWN)
	{
		*prevx = x;
		*prevy = y;
	}
	glutPostRedisplay();
}

void mouseDragged(int x, int y, int* prevx, int* prevy, mat4* worldToViewMatrix, vec3* cam_dir, vec3* cam_pos)
{
	vec3 temp, p={0.0,0.0,0.0};
	mat4 m;
	p.y = *prevx-x;//Make an orthogonal vector
	p.x = -(y-*prevy);
	p.z = 0;
	mat3 wv3 = TransposeMat3(mat4tomat3(*worldToViewMatrix));//View->World
	p = MultMat3Vec3(wv3, p); //send p to world coordinates
	m = ArbRotate(p, sqrt(p.x*p.x + p.y*p.y) / 200.0);
	temp = SetVector(cam_dir->x - cam_pos->x, cam_dir->y - cam_pos->y,cam_dir->z - cam_pos->z);
	*cam_dir = VectorAdd(MultMat3Vec3(mat4tomat3(m),temp),*cam_pos);
	*worldToViewMatrix = lookAt(cam_pos->x,cam_pos->y,cam_pos->z, cam_dir->x,cam_dir->y,cam_dir->z,0,1,0);
	*prevx = x;
	*prevy = y;
	glutPostRedisplay();
}
