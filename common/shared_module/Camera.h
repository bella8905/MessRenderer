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

class CView;

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
    CFreeFlyCamera();
    virtual  ~CFreeFlyCamera() {}

protected:
	enum CAMERA_BEHAVIOR {
		CAMERA_PAN = 0,
		CAMERA_ORBIT,
		CAMERA_ZOOM,

		CAMERA_BEHAVIOR_COUNTER,
	};

	struct S_CAMERA_BEHAVIOR_DATA {
		double _cursorX;
		double _cursorY;
		int _mouseButtonId;
		float _speed;
	}_cameraHehaviors[CAMERA_BEHAVIOR_COUNTER];

protected:
	Utl::SRay _getRayFromCursorPos( const double& t_x, const double& t_y );

public:
    void Update( double t_delta );
    
	/////////////////////////////////////////////////////////////////
	//
	//  Update camera controls
	//  Put this after GLFW_PollEvents
	//  
	/////////////////////////////////////////////////////////////////
	void UpdateControl( double t_delta );
	void UpdateView( CView* t_view );
};



