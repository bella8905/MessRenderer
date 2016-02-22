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

#include "MessRendererApp.h"


CCamera::CCamera() : _pos( vec4( 0 ) ), _aim( vec4( 0.f, 0.f, 0.f, -1.f ) ) {
}


CCamera::~CCamera( void ) {
}


void CCamera::Setup( const vec4& t_pos, const vec4& t_aim ) {
	_pos = t_pos;
	_aim = t_aim;
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
	// scroll, camera zoom,
	//  keep the aiming point and the aiming direction, move camera along camera z
	//
	/////////////////////////////////////////////////////////////////
	GLFWwindow* window = MessRenderer::CApp::GetAppWindow();
	if( !window ) return;

	if( GLFW_PRESS == glfwGetMouseButton( window, GLFW_MOUSE_BUTTON_LEFT ) ) {

	}



}

void CFreeFlyCamera::Update( double t_delta ) {
}





