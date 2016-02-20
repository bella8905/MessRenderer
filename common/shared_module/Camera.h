/////////////////////////////////////////////////////////////////
//
//  Practice_11_19_14 - camera
//
//  MessRenderer - A very messy renderer
//
//  Copyright (c) 2016 Bella Q
//  
/////////////////////////////////////////////////////////////////

#pragma once
#include "Utl_Include.h"

/////////////////////////////////////////////////////////////////
//
//  Camera Base
//  
/////////////////////////////////////////////////////////////////
class CCamera {
public:
	CCamera();
	virtual ~CCamera();

protected:
    /////////////////////////////////////////////////////////////////
    //
    //  camera position
    //  
    /////////////////////////////////////////////////////////////////
	vec4 _pos;
    /////////////////////////////////////////////////////////////////
    //
    //  camera aiming point
    //  
    /////////////////////////////////////////////////////////////////
    vec4 _aim;


public:
    void Setup( const vec4& t_pos, const vec4& t_aim );

    vec4 GetPos() { return _pos; }
    vec4 GetAiming() { return _aim; }
    vec4 GetFacing() { return glm::normalize( _aim - _pos ); }
};


/////////////////////////////////////////////////////////////////
//
//  Freefly Camera
//  
/////////////////////////////////////////////////////////////////
class CFreeFlyCamera : public CCamera {
public:
    CFreeFlyCamera() {}
    virtual  ~CFreeFlyCamera() {}

private:
    /////////////////////////////////////////////////////////////////
    //
    // control buttons
    // 
    // left button dragging, camera orbit, 
    //  keep the aiming point, but orbit the camera along camera aim/position sphere
    //  horizontal: changing latitude
    //  vertical  : changing longitude 
    // 
    // right button dragging, camera pan,
    //  keep the aiming direction, but move the camera left or right.
    //  horizontal: pan along camera +-x
    //  vertical  : pan along camera +-y
    //
    // scroll, camera zoom,
    //  keep the aiming point and the aiming direction, move camera along camera z
    //
    /////////////////////////////////////////////////////////////////
    void updateControl( float t_delta, int t_key, int t_action );

public:
    void Update( float t_delta );
    

};



