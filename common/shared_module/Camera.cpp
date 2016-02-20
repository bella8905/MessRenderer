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


CCamera::CCamera() : _pos( vec3( 0 ) ), _aim( vec3( 0.f, 0.f, -1.f ) ) {
}


CCamera::~CCamera( void ) {
}


void CCamera::Setup( const vec4& t_pos, const vec4& t_aim ) {
	_pos = t_pos;
	_aim = t_aim;
}



void CFreeFlyCamera::updateControl( float t_delta, int t_key, int t_action ) {

}

void CFreeFlyCamera::Update( float t_delta ) {
}





