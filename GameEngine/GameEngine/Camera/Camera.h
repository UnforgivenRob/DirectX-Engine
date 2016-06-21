#ifndef CAMERA_H
#define CAMERA_H

#include <MathEngine.h>
#include "Node.h"
#include "Time.h"

enum Camera_ID
{
	None,
	Camera1,
};

class Camera : public Node
{
public:
	Camera( Camera_ID id, const Vect& pos, const Vect& target, const Vect& up );
	~Camera();

	void update( Time currentTime );
	void setFrustumData( const float fovY, const float aspect, const float nearClipping, const float farClipping );
	void setViewPort( const int xPos, const int yPos, const int width, const int height );
	void setOrientation( const Vect& pos, const Vect& target, const Vect& up );
	void translateX( float scale );
	void translateY( float scale );
	void translateZ( float scale );
	void pitch( float rot );
	void roll( float rot );
	void yaw( float rot );

	Matrix& getProjMat();
	Matrix& getViewMat();

	//static Matrix perspective(float fovy, float aspect, float n, float f);
private:
	//prevent from being called
	Camera();

	//updating functions for various data
	void updateFrustum();
	void updateFrustumPoints();
	void updateFrustumNorms();
	void updateProjection();
	void updateView();

	//id
	Camera_ID id;

protected:
	//camera data
	Vect target;
	Vect pos;
	Vect direction;
	Vect up;
	Vect right;

	//Frustum data
	float fov;
	float aspect;
	float nearPlane;
	float farPlane;
	float nearHeight;
	float nearWidth;
	float farHeight;
	float farWidth;

	//viewport Data
	int vp_xPos;
	int vp_yPos;
	int vp_width;
	int vp_height;

	//frustum points
	Vect nearBotLeft;
	Vect nearBotRight;
	Vect nearTopLeft;
	Vect nearTopRight;
	Vect farBotLeft;
	Vect farBotRight;
	Vect farTopLeft;
	Vect farTopRight;

	//frustm face normals
	Vect topNorm;
	Vect leftNorm;
	Vect rightNorm;
	Vect botNorm;
	Vect frontNorm;
	Vect backNorm;


	//view and projection matrix
	Matrix view_Mat;
	Matrix proj_Mat;
	
};

#endif