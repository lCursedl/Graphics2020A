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
	glm::vec3 Pos;		/**< Vec3 for Position */
	glm::vec3 LAt;		/**< Vec3 for LookAt */
	glm::vec3 Up;		/**< Vec3 for Up */
	// Structure only receives reference to world's Up
	glm::vec3 Front;	/**< Vec3 for Front */
	glm::vec3 Right;	/**< Vec3 for Right */
	// Variables to generate PM
	float NearPlane;	/**< float variable */
	float FarPlane;		/**< float variable */
	float FOV;			/**< float Field Of View value */
	float Width;		/**< float variable */
	float Height;		/**< float variable */
};
/**	\struct CameraDesc
*	\brief Structure which defines the required attributes to initialize a Camera
*/

class CCamera
{
public:
	glm::mat4 View;					/**< Mat4 for View */
	glm::mat4 Proj;					/**< Mat4 for Projection */
	glm::vec3 Up;					/**< Vec3 for calculated Up */

	CameraDesc Desc;				/**< Local variable which stores the information received from init(CameraDesc Ref) */

	glm::vec3 mInitPos;				/**< Vec3 for mouse initial position */
	glm::vec3 mEndPos;				/**< Vec3 for mouse final position */
	glm::vec3 mDir;					/**< Vec3 for mouse direction */

	bool mIsClicked;				/**< bool to determine if mouse's right click is pressed */
	bool mForward;					/**< bool to determine if camera is moving forward */
	bool mBack;						/**< bool to determine if camera is moving backwards */
	bool mLeft;						/**< bool to determine if camera is moving left */
	bool mRight;					/**< bool to determine if camera is moving right */
	bool mUp;						/**< bool to determine if camera is moving up */
	bool mDown;						/**< bool to determine if camera is moving down */
	bool mRotateLeft;				/**< bool to determine if camera is rotating counter clock wise */
	bool mRotateRight;				/**< bool to determine if camera is rotating clock wise */

	CCamera();
	~CCamera();

	int init(CameraDesc Ref);
	/** \fn void init(CameraDesc Ref)
	*	\brief Receives a CameraDesc to copy its values and calculate new vectors.
	*	@param[in] Ref CameraDesc with the desired information.
	*	\return a int indicating if the operation was succesfull.
	*/
	//Setters

	void setPos(glm::vec3 rPos);
	/** \fn void setPos(glm::vec3 rPos)
	*	\brief Sets Camera position vector value
	*	@param[in] rPos Vec3 with data
	*/

	void setLAt(glm::vec3 rLAt);
	/** \fn setLAt(glm::vec3 rLAt)
	*	\brief Sets Camera look at vector value
	*	@param[in] rLAt Vec3 with data
	*/

	void setUp(glm::vec3 rUp);
	/** \fn void setUp(glm::vec3 rUp)
	*	\brief Sets Camera reference up vector value
	*	@param[in] rUp Vec3 with data
	*/

	void setFront(glm::vec3 rAt, glm::vec3 rPos);
	/** \fn void setFront(glm::vec3 rAt, glm::vec3 rPos)
	*	\brief Calculates Camera front vector and sets it
	*	@param[in] rAt Vec3 with LookAt vector value
	*	@param[in] rPos Vec3 with Position vector value
	*/

	void setRight(glm::vec3 rUp, glm::vec3 rFront);
	/** \fn void setRight(glm::vec3 rUp, glm::vec3 rFront)
	*	\brief Calculates Camera Right vector and sets it
	*	@param[in] rUp Vec3 with Up vector value
	*	@param[in] rFront Vec3 with Front vector value
	*/

	void setUp(glm::vec3 rFront, glm::vec3 rRight);
	/** \fn void setUp(glm::vec3 rFront, glm::vec3 rRight)
	*	\brief Calculates true Up vector and sets it
	*	@param[in] rFront Vec3 with Front vector value
	*	@param[in] rRight Vec3 with Right vector value
	*/

	void setFOV(float rFOV);
	/** \fn void setFOV(float rFOV)
	*	\brief Sets Camera FOV
	*	@param[in] rFOV float with value
	*/

	void setWidth(float rWidth);
	/** \fn void setWidth(float rWidth)
	*	\brief Sets Camera Width for PM
	*	@param[in] rWidth float with value
	*/

	void setHeigth(float rHeigth);
	/** \fn void setHeigth(float rHeigth)
	*	\brief Sets Camera Height for PM
	*	@param[in] rHeight float with value
	*/

	void setNear(float rNear);
	/** \fn void setNear(float rNear)
	*	\brief Sets Camera Near Plane for PM
	*	@param[in] rNear float with value
	*/

	void setFar(float rFar);
	/** \fn void setFar(float rFar)
	*	\brief Sets Camera Far Plane for PM
	*	@param[in] rFar float with value
	*/

	//Getters

	glm::vec3 getPos();
	/** \fn glm::vec3 getPos()
	*	\brief Obtains Position
	*	\return a Vec3 with Camera's position
	*/

	glm::vec3 getLAt();
	/** \fn glm::vec3 getLAt()
	*	\brief Obtains Look At
	*	\return a Vec3 with Camera's Look At vector 
	*/

	glm::vec3 getUp();
	/** \fn glm::vec3 getUp()
	*	\brief Obtains real Up
	*	\return a Vec3 with Camera's Up vector
	*/

	glm::vec3 getFront();
	/** \fn glm::vec3 getFront()
	*	\brief Obtains Front
	*	\return a Vec3 with Camera's Front vector
	*/

	glm::vec3 getRight();
	/** \fn glm::vec3 getRight()
	*	\brief Obtains Right
	*	\return a Vec3 with Camera's Right vector
	*/

	float getFOV();
	/** \fn float getFOV()
	*	\brief Obtains FOV
	*	\return a float with Camera's FOV value for PM
	*/

	float getWidth();
	/** \fn float getWidth()
	*	\brief Obtains Width
	*	\return a float with Camera's Width value for PM
	*/

	float getHeigth();
	/** \fn float getHeigth()
	*	\brief Obtains Height
	*	\return a float with Camera's Heigth value for PM
	*/

	float getNear();
	/** \fn float getNear()
	*	\brief Obtains Near Plane
	*	\return a float with Camera's Near Plane value for PM
	*/

	float getFar();
	/** \fn float getFar()
	*	\brief Obtains Far Plane
	*	\return a float with Camera's Far Plane value for PM
	*/

	virtual void updateVM();
	/** \fn virtual void updateVM()
	*	\brief Updates Free Camera View Matrix
	*/

	void updatePM();
	/** \fn void updatePM()
	*	\brief Updates Free Camera Projection Matrix
	*/

	virtual void move();
	/** \fn virtual void move()
	*	\brief Modifies Pos depending in which direction the Camera is moving
	*
	*	Can move Left, Right, Up, Down, Forward, Backward
	*/

	void rotate();
	/** \fn void rotate()
	*	\brief Rotates the Camera in the Z axis (Roll) depending on the input 
	*/

	void rotate(glm::vec3 mouseDir);
	/** \fn void rotate(glm::vec3 mouseDir)
	*	\brief Rotates the Camera in the X and Y axis (Pitch & Yaw) depending on the direction
	*	@param[in] mouseDir Vec3 with Direction value
	*/

	void rotateUp(glm::vec3 Dir);
	/** \fn void rotateUp(glm::vec3 Dir)
	*	\brief Rotates the Camera from its Up vector depending on a direction
	*	@param[in] Dir Vec3 with Direction value
	*
	*	Can rotate 360°.
	*/
	virtual void rotateRight(glm::vec3 Dir);
	/** \fn virtual void rotateRight(glm::vec3 Dir)
	*	\brief Rotates the Camera from its Right vector depending on a direction
	*	@param[in] Dir Vec3 with Direction Value
	*
	*	Can rotate 360°.
	*/
	virtual void rotateFront(glm::vec3 Dir);
	/** \fn virtual void rotateFront(glm::vec3 Dir)
	*	\brief Rotates the Camera from its Front vector depending on a direction
	*	@param[in] Dir Vec3 with Direction value
	*
	*	Can rotate 360°.
	*/
	void createVM();
	/** \fn void createVM()
	*	\brief Creates a View Matrix from the required vectors
	*/
	void getKeyPress(WPARAM key);
	/** \fn void getKeyPress(WPARAM key)
	*	\brief Process the pressed key value received and determine corresponding action
	*	@param[in] key WPARAM value of key pressed
	*
	*	Detects W, A, S, D, Q, E, Left Arrow & Right Arrow keys.
	*/
	void getKeyRelease(WPARAM key);
	/** \fn void getKeyRelease(WPARAM key)
	*	\brief Process the released key value received and determine corresponding action 
	*	@param[in] key WPARAM value of key released
	*
	*	Detects W, A, S, D, Q, E, Left Arrow & Right Arrow keys.
	*/
	CBuffer m_CBNeverChanges;		/**< Camera's Buffer for VM */
	CBuffer m_CBChangesEveryFrame;	/**< Camera's Buffer for WM and color */
	CBuffer m_CBChangesOnResize;	/**< Camera's Buffer for PM */
};
/** \class CCamera
*	\brief Class which contains functions and members for camera creation/usage.
*
*	Type of camera created is Free Camera.
*/