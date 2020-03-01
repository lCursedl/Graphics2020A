#include "CCameraFP.h"

CCameraFP::CCameraFP()
{
	m_maxFrontRotation = 20.f;
	m_maxRightRotation = 45.f;
	m_FrontRotation = 0.f;
	m_RightRotation = 0.f;
}

void CCameraFP::move()
{
	if (mForward)
	{
		Desc.Pos += (getFront() * STEP);
	}
	if (mBack)
	{
		Desc.Pos += (getFront() * -STEP);
	}

	if (mLeft)
	{
		Desc.Pos += (getRight() * -STEP);
	}
	if (mRight)
	{
		Desc.Pos += (getRight() * STEP);
	}

	glm::mat4 Axis
	{
		Desc.Right.x, Desc.Right.y, Desc.Right.z, 0,
		Up.x,		  Up.y,			Up.z		, 0,
		Desc.Front.x, Desc.Front.y, Desc.Front.z, 0,
		0,			  0,			0,			  1
	};

	glm::mat4 Pos
	{
		1, 0, 0, -Desc.Pos.x,
		0, 1, 0, -Desc.Pos.y,
		0, 0, 1, -Desc.Pos.z,
		0, 0, 0, 1
	};

	Pos *= Axis;

	View = Pos;

	updateVM();
}

void CCameraFP::getKeyPress(WPARAM key)
{
	//Z Movement
	if (key == 'W')
	{
		mForward = true;
	}
	else if (key == 'S')
	{
		mBack = true;
	}
	//X Movement
	if (key == 'A')
	{
		mLeft = true;
	}
	else if (key == 'D')
	{
		mRight = true;
	}

	if (key == VK_LEFT)
	{
		mRotateLeft = true;
	}
	else if (key == VK_RIGHT)
	{
		mRotateRight = true;
	}
}

void CCameraFP::getKeyRelease(WPARAM key)
{
	if (key == 'W')
	{
		mForward = false;
	}
	else if (key == 'S')
	{
		mBack = false;
	}
	if (key == 'A')
	{
		mLeft = false;
	}
	else if (key == 'D')
	{
		mRight = false;
	}
	if (key == VK_LEFT)
	{
		mRotateLeft = false;
	}
	else if (key == VK_RIGHT)
	{
		mRotateRight = false;
	}
}

void CCameraFP::rotateFront(glm::vec3 Dir)
{
	float camcos = cosf(Dir.z / 100.f);
	float camsin = sinf(Dir.z / 100.f);

	if (Dir.z < 0)
	{
		if ((m_FrontRotation + Dir.z) >= -m_maxFrontRotation)
		{
			m_FrontRotation += Dir.z;
			glm::mat4 RotZ
			{
				camcos, -camsin,0  ,	0.f,
				camsin, camcos,	0.f,	0.f,
				0.f, 0.f,		1.f,	0.f,
				0.f, 0.f,		0.f,	1.f
			};
			View *= RotZ;
			updateVM();
		}
	}
	else
	{
		if (m_FrontRotation + Dir.z <= m_maxFrontRotation)
		{
			m_FrontRotation += Dir.z;
			glm::mat4 RotZ
			{
				camcos, -camsin,0  ,	0.f,
				camsin, camcos,	0.f,	0.f,
				0.f, 0.f,		1.f,	0.f,
				0.f, 0.f,		0.f,	1.f
			};
			View *= RotZ;
			updateVM();
		}
	}	
}

void CCameraFP::rotateRight(glm::vec3 Dir)
{
	float camcos = cosf(Dir.y / 100.f);
	float camsin = sinf(Dir.y / 100.f);

	if (Dir.y < 0)
	{
		if (m_RightRotation + Dir.y >= -m_maxRightRotation)
		{
			m_RightRotation += Dir.y;
			glm::mat4 RotY
			{
				1.f, 0.f,	  0.f,	  0.f,
				0.f, camcos, -camsin, 0.f,
				0.f, camsin, camcos,  0.f,
				0.f, 0.f,	 0.f,	  1.f
			};
			View *= RotY;
			updateVM();
		}
	}
	else
	{
		if (m_RightRotation + Dir.y <= m_maxRightRotation)
		{
			m_RightRotation += Dir.y;
			glm::mat4 RotY
			{
				1.f, 0.f,	  0.f,	  0.f,
				0.f, camcos, -camsin, 0.f,
				0.f, camsin, camcos,  0.f,
				0.f, 0.f,	 0.f,	  1.f
			};
			View *= RotY;
			updateVM();
		}
	}

	
}
