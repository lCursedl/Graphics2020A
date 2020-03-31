#pragma once
#include "CCamera.h"
class CCameraFP : public CCamera
{
public:

	CCameraFP();

	void move();
	/** \fn void move()
	*	\brief Modifies Pos depending in which direction the Camera is moving
	*
	*	Can move Left, Right, Forward, Backward
	*/

	void getKeyPress(WPARAM key);
	/** \fn void getKeyPress(WPARAM key)
	*	\brief Process the pressed key value received and determine corresponding action
	*	@param[in] key WPARAM value of key pressed
	*
	*	Detects W, A, S, D, Left Arrow & Right Arrow keys.
	*/

	void getKeyRelease(WPARAM key);
	/** \fn void getKeyRelease(WPARAM key)
	*	\brief Process the released key value received and determine corresponding action 
	*	@param[in] key WPARAM value of key released
	*
	*	Detects W, A, S, D, Left Arrow & Right Arrow keys.
	*/

	void rotateFront(glm::vec3 Dir);
	/** \fn void rotateFront(glm::vec3 Dir)
	*	\brief Rotates the Camera from its Front vector depending on a direction
	*	@param[in] Dir Vec3 with Direction value
	*
	*	Rotation is dictaded by m_maxFrontRotation.
	*/

	void rotateRight(glm::vec3 Dir);
	/** \fn void rotateRight(glm::vec3 Dir)
	*	\brief Rotates the Camera from its Right vector depending on a direction
	*	@param[in] Dir Vec3 with Direction Value
	*
	*	Rotation is dictaded by m_maxRightRotation.
	*/

	float m_maxFrontRotation;	/**< float which stores how much can the Camera rotate in its Front vector*/
	float m_maxRightRotation;	/**< float which stores how much can the Camera rotate in its Right vector */
	float m_RightRotation;		/**< float which stores the current rotation amount in the Camera's Right vector */
	float m_FrontRotation;		/**< float which stores the current rotation amount in the Camera's Front vector */
};
/** \class CCameraFP
*	\brief Class which inherits of CCamera, designed for First Person.
*
*	Rotations and movement are limited.
*/