#pragma once
#include "CCamera.h"
class CCameraFP : public CCamera
{
public:
	glm::vec3 Front;

	void move(glm::vec3 tras);
	void UpdateVM();
};