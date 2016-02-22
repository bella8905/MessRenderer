/////////////////////////////////////////////////////////////////
//
//  Practice_11_19_14 - camera
//
//  MessRenderer - A very messy renderer
//
//  Copyright (c) 2016 Bella Q
//  
/////////////////////////////////////////////////////////////////

#include "Camera.h"
#include "View.h"

#include "MessRendererApp.h"


CCamera::CCamera() : _pos( vec4( 0 ) ), _aim( vec4( 0.f, 0.f, 0.f, -1.f ) ) {
}


CCamera::~CCamera( void ) {
}


void CCamera::Setup( const vec4& t_pos, const vec4& t_aim ) {
	_pos = t_pos;
	_aim = t_aim;
}



CFreeFlyCamera::CFreeFlyCamera() {
	_cameraHehaviors[CAMERA_PAN]._mouseButtonId = GLFW_MOUSE_BUTTON_LEFT;
	_cameraHehaviors[CAMERA_PAN]._speed = 0.3f;
	_cameraHehaviors[CAMERA_ORBIT]._mouseButtonId = GLFW_MOUSE_BUTTON_RIGHT;
	_cameraHehaviors[CAMERA_ORBIT]._speed = 1.f;
	_cameraHehaviors[CAMERA_ZOOM]._mouseButtonId = GLFW_MOUSE_BUTTON_MIDDLE;
	_cameraHehaviors[CAMERA_ZOOM]._speed = 1.f;
}

void CFreeFlyCamera::UpdateControl( double t_delta ) {
	/////////////////////////////////////////////////////////////////
	//
	// control buttons
	// 
	// left button dragging, camera pan,
	//  keep the aiming direction, but move the camera left or right.
	//  horizontal: pan along camera +-x
	//  vertical  : pan along camera +-y
	//
	// right button dragging, camera orbit, 
	//  keep the aiming point, but orbit the camera along camera aim/position sphere
	//  horizontal: changing latitude
	//  vertical  : changing longitude 
	//
	// middle button dragging, camera zoom,
	//  keep the aiming point and the aiming direction, move camera along camera z
	//
	/////////////////////////////////////////////////////////////////
	for( int b = 0; b < CAMERA_BEHAVIOR_COUNTER; ++b ) {
		if( Utl::GL_GetKeyOrMouseButtonPressed( _cameraHehaviors[b]._mouseButtonId ) ) {
			glfwGetCursorPos( MessRenderer::CApp::GetAppWindow(), &_cameraHehaviors[b]._cursorX, &_cameraHehaviors[b]._cursorY );
			return;
		}

		if( Utl::GL_GetKeyOrMouseButtonHeld( _cameraHehaviors[b]._mouseButtonId ) ) {
			double x, y;
			glfwGetCursorPos( MessRenderer::CApp::GetAppWindow(), &x, &y );
			double deltaX = x - _cameraHehaviors[b]._cursorX;
			double deltaY = y - _cameraHehaviors[b]._cursorY;

			switch( b ) {
			case CAMERA_PAN:
				{
					_pos.x += -(float)deltaX * (float)t_delta * _cameraHehaviors[b]._speed;
					_pos.y += -(float)deltaY * (float)t_delta * _cameraHehaviors[b]._speed;
					_aim.x += -(float)deltaX * (float)t_delta * _cameraHehaviors[b]._speed;
					_aim.y += -(float)deltaY * (float)t_delta * _cameraHehaviors[b]._speed;
				} break;
			case CAMERA_ORBIT:
				{

				} break;
			case CAMERA_ZOOM:
				{
					float delta = abs( deltaX ) > abs( deltaY ) ? deltaX : deltaY;
					float dist = glm::distance( _pos, _aim );
					dist = max( dist - delta * _cameraHehaviors[b]._speed * (float)t_delta, 1.f );
					vec4 facing = glm::normalize( _aim - _pos );
					_pos = _aim - facing * dist;
				} break;
			default: break;
			}

			_cameraHehaviors[b]._cursorX = x;
			_cameraHehaviors[b]._cursorY = y;
		}


	}

}

void CFreeFlyCamera::UpdateView( CView* t_view ) {
	if( !t_view ) return;

	t_view->SetCameraPostionFaceAndUp( _pos, glm::normalize( _aim - _pos ) );
}

void CFreeFlyCamera::Update( double t_delta ) {
}





