#ifndef  MAIN_H_INCLUDED
#define MAIN_H_INCLUDED
 // ! MAIN_H_INCLUDED

#include <windows.h>
#include <gl/gl.h>
#include<math.h>

#define M_PI_2     1.57079632679489661923   
#define M_PI_4     0.785398163397448309616  
#define M_1_PI     0.318309886183790671538  
#define M_2_PI     0.636619772367581343076 

namespace camera
{
	struct SCamera
	{
		float x, y, z;
		float XRot, ZRot;
	}camera = { 0,0,1.7,0,0 };

	void Camera_Apply()
	{
		glRotatef(-camera.XRot, 1, 0, 0);
		glRotatef(-camera.ZRot, 0, 0, 1);
		glTranslatef(-camera.x, -camera.y, -camera.z);
	}

	void Camera_Rotation(float xAngle, float zAngle)
	{
		camera.ZRot += zAngle;
		if (camera.ZRot < 0)camera.ZRot += 360;
		if (camera.ZRot > 360)camera.ZRot -= 360;
		camera.XRot += xAngle;
		if (camera.XRot < 0)camera.XRot = 0;
		if (camera.XRot > 180)camera.XRot = 180;


	}

	void Camera_AutoMoveByMouse(int centerX, int centerY, float speed)
	{
		POINT cur;
		POINT base = { centerX,centerY };
		GetCursorPos(&cur);
		Camera_Rotation((base.y - cur.y) * speed, (base.x - cur.x) * speed);
		SetCursorPos(base.x, base.y);
	}

	void Camera_MoveDirection(int forwardMove, int rightMove, float speed)
	{
		float ugol = -camera.ZRot / 180 * 3.14;

		if (forwardMove > 0)
			ugol += rightMove > 0 ? M_PI_4 : (rightMove < 0 ? -M_PI_4 : 0);
		if (forwardMove < 0)
			ugol += 3.14 + (rightMove > 0 ? -M_PI_4 : (rightMove < 0 ? M_PI_4 : 0));
		if (forwardMove == 0)
		{
			ugol += rightMove > 0 ? M_PI_2 : -M_PI_2;
			if (rightMove == 0)speed = 0;
		}

		if (speed != 0)
		{
			camera.x += sin(ugol) * speed;
			camera.y += cos(ugol) * speed;
		}


	}
}

typedef struct
{
	float x, y, z;
}TCell;

typedef struct
{
	float r, g, b;
}TColor;

typedef struct {
	float x, y,z;
	int type;
	float scale;
}TObject;

typedef struct {
	float u, v;
}TUV;


typedef struct
{
	TObject* items;
	int itemsCnt;
	int type;

}TobjGroup;

#define mapW 100
#define mapH 100

TCell map[mapW][mapH];
TUV mapUV[mapW][mapH];
TCell mapNormal[mapW][mapH];

float plant[] = {
	-0.5f,0,0, 0.5,0,0, 0.5,0,1, -0.5,0,1,
	0,-0.5,0,0,0.5,0,0,0.5,1,0,-0.5,1
};
float plantUV[] = {	0,1, 1,1, 1,0, 0,0 ,0,1 ,1,1 ,1,0 ,0,0};

GLuint plantInd[] = { 0,1,2, 2,3,0, 4,5,6 ,6,7,4 };

int plantIndCnt = sizeof(plantInd) / sizeof(GLuint);

GLuint mapind[mapW - 1][mapH - 1][6];
int mapIndCount = sizeof(mapind) / sizeof(GLuint);

TObject *plantMas = NULL;
int plantCnt = 0;

int tex_wood;

float cube[] = {
	0,0,0, 1,0,0, 1,1,0, 0,1,0,
	0,0,1, 1,0,1, 1,1,1, 0,1,1,
	0,0,0, 1,0,0, 1,1,0, 0,1,0,
	0,0,1, 1,0,1, 1,1,1, 0,1,1
};

float cubeUVLog[] = {0.5,0.5, 1,0.5, 1,0, 0.5,0,
                     0.5,0.5, 1,0.5, 1,0, 0.5,0,
	                  0,0.5, 0.5,0.5, 0,0.5, 0.5,0.5,
	                 0,0,   0.5,0,  0,0,  0.5,0

};

float cubeUVLeaf[] = {0,1, 0.5,1, 0.5,0.5, 0,0.5,
0,1, 0.5,1, 0.5,0.5, 0,0.5,
0,0.5,0.5,0.5,0,0.5,0.5,0.5,
0,1,0.5,1,0,1,0.5,1};

GLuint cubeInd[] = { 0,1,2, 2,3,0, 4,5,6, 6,7,4, 8,9,13,13,12,8,
9,10,14,14,13,9,10,11,15,15,14,10,11,8,12,12,15,11};

int cubeInCnt = sizeof(cubeInd) / sizeof(GLuint);

TobjGroup* tree = NULL;
int treeCNt = 0;

int tex_pole, tex_trava, tex_flower, tex_flower2, tex_grib, tex_tree, tex_tree2;

#endif