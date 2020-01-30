#pragma once
#include "CCamera.h"
class CCameraFP : public CCamera
{
public:
	glm::vec3 Front;

	void move();
	void updateVM();
};