#include "Camera.h"
#include "Game.h"
#include <math.h>
#include "GraphicsEngine.h"

Camera::Camera( Camera_ID id, const Vect& pos, const Vect& target, const Vect& up )
	: id( id ), pos( pos ), target( target )
{
	this->direction = pos - target;
	this->direction.norm();

	this->right = ( -direction ).cross( up );
	this->right.norm();

	this->up = direction.cross( right );
	this->up.norm();

}

Camera::~Camera()
{
}

void Camera::update( Time currentTime )
{
	//update frustum
	updateFrustum();
	//update view and proj matrices
	updateView();
	updateProjection();
}

void Camera::setFrustumData( float fov, float aspect, float nearClipping, float farClipping )
{
	this->fov = fov;
	this->aspect = aspect;
	this->nearPlane = nearClipping;
	this->farPlane = farClipping;
	
	this->nearHeight = 2.0f * nearPlane * tanf( fov * MATH_PI_180 / 2.0f );
	this->farHeight = 2.0f * farPlane * tanf( fov * MATH_PI_180 / 2.0f );

	this->nearWidth = aspect * nearHeight;
	this->farWidth = aspect * farHeight;
}

void Camera::setViewPort( const int xPos, const int yPos, const int width, const int height )
{
	vp_xPos = xPos;
	vp_yPos = yPos;
	vp_width = width;
	vp_height = height;
	GraphicsEngine::setViewPort( xPos, yPos, width, height );
}

void Camera::setOrientation( const Vect& pos, const Vect& target, const Vect& up )
{
	this->pos = pos;
	this->target = target;

	this->direction = this->pos - this->target;
	this->direction.norm();

	this->up = up;
	this->right = this->up.cross( this->direction );
	this->right.norm();

	this->up = this->direction.cross( this->right );
	this->up.norm();


}

void Camera::translateZ( float scale )
{
	pos = pos + (direction * scale);
}

void Camera::translateY( float scale )
{
	pos = pos + ( up * scale );
}

void Camera::translateX( float scale )
{
	pos = pos + ( right * scale );
}

void Camera::pitch( float rot )
{
	Vect tmpUp = this->up;
	this->up -= rot * this->direction;
	this->up.norm();
	this->direction += rot * tmpUp;
	this->direction.norm();
}

void Camera::roll( float rot )
{
	Vect tmpRight = this->right;
	this->right -= rot * this->up;
	this->right.norm();
	this->up	+= rot * tmpRight; 
	this->up.norm();
}

void Camera::yaw( float rot )
{
	Vect tmpRight = this->right;
	this->right	+= rot * direction;
	this->right.norm();
	this->direction -= rot * tmpRight;
	this->direction.norm();
}

Matrix& Camera::getProjMat()
{
	return this->proj_Mat;
}

Matrix& Camera::getViewMat()
{
	return this->view_Mat;
}

void Camera::updateFrustum()
{
	updateFrustumPoints();
	updateFrustumNorms();
}

void Camera::updateFrustumPoints()
{
	this->nearTopLeft = this->pos - ( this->direction * this->nearPlane ) + ( 0.5f * this->nearHeight * this->up ) - ( 0.5f * this->nearWidth * this->right );
	this->nearBotLeft = this->nearTopLeft - ( this->nearHeight * this->up );
	this->nearBotRight = this->nearBotLeft + ( this->nearWidth * this->right );
	this->nearTopRight = this->nearTopLeft + ( this->nearWidth * this->right );

	this->farTopLeft = this->pos - ( this->direction * this->farPlane ) + ( 0.5f * this->farHeight * this->up ) - ( 0.5f * this->farWidth * this->right );
	this->farBotLeft = this->farTopLeft - ( this->farHeight * this->up );
	this->farBotRight = this->farBotLeft + ( this->farWidth * this->right );
	this->farTopRight = this->farTopLeft + ( this->farWidth * this->right );
}

void Camera::updateFrustumNorms()
{
	//get the 3 vectors from near bot right 
	Vect Xa = this->nearBotLeft - this->nearBotRight;
	Vect Ya = this->nearTopRight - this->nearBotRight;
	Vect Za = this->farBotRight - this->nearBotRight;
	//get the 3 vectors from far top left
	Vect Xb = this->farTopRight - this->farTopLeft;
	Vect Yb = this->farBotLeft - this->farTopLeft;
	Vect Zb = this->nearTopLeft - this->farTopLeft;

	//calculate norms
	this->frontNorm = Ya.cross( Xa );
	this->frontNorm.norm();

	this->rightNorm = Za.cross( Ya );
	this->rightNorm.norm();

	this->botNorm = Xa.cross( Za );
	this->botNorm.norm();

	this->backNorm = Xb.cross( Yb );
	this->backNorm.norm();

	this->leftNorm = Yb.cross( Zb );
	this->leftNorm.norm();

	this->topNorm = Zb.cross( Xb );
	this->topNorm.norm();
}

void Camera::updateProjection()
{

	proj_Mat = Matrix( 2.0f *nearPlane/nearWidth, 0, 0, 0,
						0, 2.0f * nearPlane/nearHeight, 0, 0,
						0, 0, -( ( farPlane + nearPlane ) / ( farPlane - nearPlane ) ), -1,
						0, 0, -( ( 2.0f * farPlane * nearPlane ) / ( farPlane - nearPlane ) ), 0 );
}

void Camera::updateView()
{
	view_Mat = Matrix( right[x], up[x], direction[x], 0,
					   right[y], up[y], direction[y], 0,
					   right[z], up[z], direction[z], 0,
					   (-pos).dot(right), (-pos).dot(up), (-pos).dot(direction), 1 );
}