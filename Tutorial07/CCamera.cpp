#include "CCamera.h"

CCamera::CCamera()
{
	mIsClicked = false;
	mForward = false;
	mBack = false;
	mLeft = false;
	mRight = false;
}

CCamera::~CCamera(){}

int CCamera::init(CameraDesc Ref)
{
	//Set common values
	setPos(Ref.Pos);
	setLAt(Ref.LAt);
	setUp(Ref.Up);
	setFOV(Ref.FOV);
	setWidth(Ref.Width);
	setHeigth(Ref.Height);
	setNear(Ref.NearPlane);
	setFar(Ref.FarPlane);

	//Set Front, Right & Up

	CreateVM();
	UpdateVM();
	UpdatePM();

	return 0;
}

void CCamera::setPos(glm::vec3 rPos)
{
	Desc.Pos = rPos;
}

void CCamera::setLAt(glm::vec3 rLAt)
{
	Desc.LAt = rLAt;
}

void CCamera::setUp(glm::vec3 rUp)
{
	Desc.Up = rUp;
}

void CCamera::setFront(glm::vec3 rAt, glm::vec3 rPos)
{
	Desc.Front = glm::normalize(Desc.LAt - Desc.Pos);
}

void CCamera::setRight(glm::vec3 rUp, glm::vec3 rFront)
{
	Desc.Right = glm::normalize(glm::cross(Desc.Up, Desc.Front));
}

void CCamera::setUp(glm::vec3 rFront, glm::vec3 rRight)
{
	Up = glm::normalize(glm::cross(Desc.Front, Desc.Right));
}

void CCamera::setFOV(float rFOV)
{
	Desc.FOV = rFOV;
}

void CCamera::setWidth(float rWidth)
{
	Desc.Width = rWidth;
}

void CCamera::setHeigth(float rHeigth)
{
	Desc.Height = rHeigth;
}

void CCamera::setNear(float rNear)
{
	Desc.NearPlane = rNear;
}

void CCamera::setFar(float rFar)
{
	Desc.FarPlane = rFar;
}

glm::vec3 CCamera::getPos()
{
	return Desc.Pos;
}

glm::vec3 CCamera::getLAt()
{
	return Desc.LAt;
}

glm::vec3 CCamera::getUp()
{
	return Up;
}

float CCamera::getFOV()
{
	return Desc.FOV;
}

float CCamera::getWidth()
{
	return Desc.Width;
}

float CCamera::getHeigth()
{
	return Desc.Height;
}

float CCamera::getNear()
{
	return Desc.NearPlane;
}

float CCamera::getFar()
{
	return Desc.FarPlane;
}

glm::vec3 CCamera::getFront()
{
	return Desc.Front;
}

glm::vec3 CCamera::getRight()
{
	return Desc.Right;
}

void CCamera::UpdateVM()
{
	Desc.Right =	{ View[0][0], View[0][1], View[0][2] };
	Desc.Up =		{ View[1][0], View[1][1], View[1][2] };
	Desc.Front =	{ View[2][0], View[2][1], View[2][2] };
	Up = Desc.Up;
	Desc.LAt = Desc.Front + Desc.Pos;
}

void CCamera::UpdatePM()
{
	Proj = glm::perspectiveFovLH(Desc.FOV, Desc.Width, Desc.Height, Desc.NearPlane, Desc.FarPlane);
	Proj = glm::transpose(Proj);
}

void CCamera::move()
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
	/*Desc.Pos += (getRight() * tras.x);
	Desc.Pos += (getFront() * tras.z);
	Desc.Pos += (Up * tras.y);*/

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

	UpdateVM();
}

void CCamera::rotate(glm::vec3 mouseDir)
{
	rotateUp(mouseDir);
	UpdateVM();
	rotateRight(mouseDir);
	UpdateVM();
	//CreateVM();
}

void CCamera::rotateUp(glm::vec3 Dir)
{
	float camcos = cosf(Dir.x/100.f);
	float camsin = sinf(Dir.x/100.f);

	glm::mat4 RotX
	{
		camcos,  0.f, camsin,	0.f,
		0.f,	 1.f, 0.f,		0.f,
		-camsin, 0.f, camcos,	0.f,
		0.f,	 0.f, 0.f,		1.f
	};
	View *= RotX;
}

void CCamera::rotateRight(glm::vec3 Dir)
{
	float camcos = cosf(Dir.y / 100.f);
	float camsin = sinf(Dir.y / 100.f);

	glm::mat4 RotY
	{
		1.f, 0.f,	  0.f,	  0.f, 
		0.f, camcos, -camsin, 0.f,
		0.f, camsin, camcos,  0.f,
		0.f, 0.f,	 0.f,	  1.f
	};
	View *= RotY;
}

void CCamera::CreateVM()
{
	setFront(getLAt(), getPos());
	setRight(getUp(), getFront());
	setUp(getFront(), getRight());

	glm::mat4 Axis{
		Desc.Right.x, Desc.Right.y, Desc.Right.z, 0,
		Up.x,		  Up.y,			Up.z		, 0,
		Desc.Front.x, Desc.Front.y, Desc.Front.z, 0,
		0,			  0,			0,			  1
	};

	glm::mat4 Pos{
		1, 0, 0, -Desc.Pos.x,
		0, 1, 0, -Desc.Pos.y,
		0, 0, 1, -Desc.Pos.z,
		0, 0, 0, 1
	};

	View = Pos * Axis;
}