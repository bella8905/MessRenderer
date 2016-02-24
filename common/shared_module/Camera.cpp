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


void CCamera::Setup( const vec4& t_pos, const vec4& t_aim, const vec4& t_up ) {
	_pos = t_pos;
	_aim = t_aim;
	_up = t_up;
}



CFreeFlyCamera::CFreeFlyCamera() {
	_cameraHehaviors[CAMERA_PAN]._mouseButtonId = GLFW_MOUSE_BUTTON_MIDDLE;
	_cameraHehaviors[CAMERA_PAN]._speed = 0.3f;
	_cameraHehaviors[CAMERA_ORBIT]._mouseButtonId = GLFW_MOUSE_BUTTON_LEFT;
	_cameraHehaviors[CAMERA_ORBIT]._speed = 100.f;
	_cameraHehaviors[CAMERA_ZOOM]._mouseButtonId = GLFW_MOUSE_BUTTON_RIGHT;
	_cameraHehaviors[CAMERA_ZOOM]._speed = 1.f;
}

void CFreeFlyCamera::UpdateControl( double t_delta ) {
	/////////////////////////////////////////////////////////////////
	//
	// control buttons
	// 
	// middle button dragging, camera pan,
	//  keep the aiming direction and distance to camera, 
	//  but move the camera left/right along camera up and right axis.
	//
	// left button dragging, camera orbit, 
	//  keep the aiming point and distance to camera, 
	//  but orbit the camera along camera aim/position sphere
	//
	// right button dragging, camera zoom,
	//  keep the aiming point and the aiming direction, 
	//  move camera along camera facing axis.
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

			if( Utl::Equals( deltaX, 0.0 ) && Utl::Equals( deltaY, 0.0 ) ) {
				return;
			}

			switch( b ) {
			case CAMERA_PAN:
				{
					vec3 z = glm::normalize( vec3( _pos - _aim ) );
					vec3 right = glm::normalize( glm::cross( z, vec3( _up ) ) );
					_pos += (float)deltaX * (float)t_delta * _cameraHehaviors[b]._speed * Utl::ToDirection( right ) + (float)deltaY * (float)t_delta * _cameraHehaviors[b]._speed * _up;
					_aim += (float)deltaX * (float)t_delta * _cameraHehaviors[b]._speed * Utl::ToDirection( right ) + (float)deltaY * (float)t_delta * _cameraHehaviors[b]._speed * _up;
				} break;
			case CAMERA_ORBIT:
				{ 
					Utl::SRay oldRay = _getCameraCursorRayFromCursorPos( _cameraHehaviors[b]._cursorX, _cameraHehaviors[b]._cursorY );
					Utl::SRay newRay = _getCameraCursorRayFromCursorPos( x, y );
					vec3 startSide = vec3( oldRay._Dir );
					vec3 endSide = vec3( newRay._Dir );
					vec3 rotAxis = glm::normalize( glm::cross( startSide, endSide ) );
					float rotAngle = acos( glm::dot( startSide, endSide ) ) * 2 * (float)t_delta * _cameraHehaviors[b]._speed; 
					glm::quat rotQuat = glm::angleAxis( rotAngle, rotAxis );
					mat3 rot_quat = glm::toMat3( rotQuat );
					vec3 facing = vec3( glm::normalize( _aim - _pos ) );
					facing = rot_quat * facing;
					_up = Utl::ToDirection( glm::normalize( rot_quat * vec3( _up ) ) );
				    float dist = glm::distance( _aim, _pos );
					_pos = _aim - Utl::ToDirection( dist * facing );
				} break;
			case CAMERA_ZOOM:
				{
					float delta = abs( deltaX ) > abs( deltaY ) ? (float)deltaX : (float)deltaY;
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

	t_view->SetCameraPostionFaceAndUp( _pos, glm::normalize( _aim - _pos ), _up );
}

void CFreeFlyCamera::Update( double t_delta ) {
}


Utl::SRay CFreeFlyCamera::_getCameraCursorRayFromCursorPos( const double& t_x, const double& t_y ) {
	// nds ( -1 , 1 )
	int winWidth, winHeight;
	glfwGetWindowSize( MessRenderer::CApp::GetAppWindow(), &winWidth, &winHeight );
	float x = ( 2.f * (float)t_x ) / ( float )winWidth - 1.f;
	float y = 1.f - ( 2.f * (float)t_y ) / ( float )winHeight;
	vec3 dir_nds( x, y, 1.f );
	vec4 dir_clip( dir_nds.x, dir_nds.y, -1.f, 0.f );
	// get the current used camera's proj matrix
	vec4 dir_eye = View_GetActive()->GetProj2ViewMatrix() * dir_clip;
	dir_eye = vec4( dir_eye.x, dir_eye.y, -1.f, 0.f );
	Utl::SRay ray_eye( vec4( 0.f, 0.f, 0.f, 1.f ), dir_eye );
	Utl::SRay ray_wor = ray_eye.Transform( View_GetActive()->GetView2WorldMatrix() );

	return ray_wor;
}



