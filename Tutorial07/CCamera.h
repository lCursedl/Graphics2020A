 #pragma once

#include <windows.h>
#include "CBuffer.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#define STEP 0.025f
#define ROTATESTEP 0.25f

struct CameraDesc
{
	// Minimum variables required to generate camera's VM
	glm::vec3 Pos;	
	glm::vec3 LAt;
	glm::vec3 Up;
	// Structure only receives reference to world's Up
	glm::vec3 Front;
	glm::vec3 Right;
	// Variables to generate PM
	float NearPlane;
	float FarPlane;
	float FOV;
	float Width;
	float Height;
};

class CCamera
{
public:
	glm::mat4 View;
	glm::mat4 Proj;
	glm::vec3 Up;

	CameraDesc Desc;

	glm::vec3 mInitPos;
	glm::vec3 mEndPos;
	glm::vec3 mDir;

	bool mIsClicked;
	bool mForward;
	bool mBack;
	bool mLeft;
	bool mRight;
	bool mUp;
	bool mDown;
	bool mRotateLeft;
	bool mRotateRight;

	CCamera();
	~CCamera();

	int init(CameraDesc Ref);
	//Setters
	void setPos(glm::vec3 rPos);
	void setLAt(glm::vec3 rLAt);
	void setUp(glm::vec3 rUp);
	void setFront(glm::vec3 rAt, glm::vec3 rPos);
	void setRight(glm::vec3 rUp, glm::vec3 rFront);
	void setUp(glm::vec3 rFront, glm::vec3 rRight);
	void setFOV(float rFOV);
	void setWidth(float rWidth);
	void setHeigth(float rHeigth);
	void setNear(float rNear);
	void setFar(float rFar);
	//Getters
	glm::vec3 getPos();
	glm::vec3 getLAt();
	glm::vec3 getUp();
	glm::vec3 getFront();
	glm::vec3 getRight();
	float getFOV();
	float getWidth();
	float getHeigth();
	float getNear();
	float getFar();

	virtual void updateVM();
	void updatePM();
	virtual void move();
	void rotate();
	void rotate(glm::vec3 mouseDir);
	void rotateUp(glm::vec3 Dir);
	virtual void rotateRight(glm::vec3 Dir);
	virtual void rotateFront(glm::vec3 Dir);
	void createVM();
	void getKeyPress(WPARAM key);
	void getKeyRelease(WPARAM key);

	CBuffer m_CBNeverChanges;
	CBuffer m_CBChangesEveryFrame;
	CBuffer m_CBChangesOnResize;
};