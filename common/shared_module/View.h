/////////////////////////////////////////////////////////////////
//
//  Practice_01_28_16 - view
//
//  view and view related class
// 
//  Camera Scale is not allowed for now.
//
//  MessRenderer - A very messy renderer
//
//  Copyright (c) 2016 Bella Q
//  
/////////////////////////////////////////////////////////////////


#pragma once
#include "Utl_Include.h"


class CView {

public:
	/////////////////////////////////////////////////////////////////
	//
	// 
	//
	/////////////////////////////////////////////////////////////////
	struct SViewPort {
		us _x, _y, _width, _height;
        float _maxZ, _minZ;

		SViewPort() {
			// defaults
			_x = 0;
			_y = 0;
			_width = 800;
			_height = 600;
			_maxZ = 1;
			_minZ = -1;
		}
	};

	struct SFrustum {

	};

public:
	CView() { initDefaults();  }
	virtual ~CView() {}


protected:

	/////////////////////////////////////////////////////////////////
	//
	// if it is a perspective view or orthographic view 
	//
	/////////////////////////////////////////////////////////////////
	bool _bPerspective;

    /////////////////////////////////////////////////////////////////
    //
    // view port, 
    // this could be a full window view,
    // or just part of the whole screen.
    //
    /////////////////////////////////////////////////////////////////
     SViewPort _viewPort;

    /////////////////////////////////////////////////////////////////
    //
    // transform matrix converting points from world to view space.
    //
    // View matrix, is the V in MVP.
    //
    /////////////////////////////////////////////////////////////////
    mat4 _world2ViewMatrix;

	/////////////////////////////////////////////////////////////////
	//
	// transform matrix converting points from view to world space.
	//
    // this is the inverse of _world2ViewMatrix, 
	//
	/////////////////////////////////////////////////////////////////
	mat4 _view2WorldMatrix;


	/////////////////////////////////////////////////////////////////
	//
	// transform matrix converting points from view space to homogeneous 
	// clip space
	// 
	// Projection matrix, is the P in MVP.
	//
	/////////////////////////////////////////////////////////////////
	mat4 _view2ProjMatrix;

	/////////////////////////////////////////////////////////////////
	//
	// transform matrix converting points from homogeneous 
	// clip space to view space.
	//
	// this is the inverse of _view2ProjMatrix,
	//
	/////////////////////////////////////////////////////////////////
	mat4 _proj2ViewMatrix;

    /////////////////////////////////////////////////////////////////
    //
    // transform matrix converting points from world space 
    // to clip space.
    // 
    // _view2ProjMatrix * _view2WorldMatrix
    //
    /////////////////////////////////////////////////////////////////
    mat4 _world2ProjMatrix;

    /////////////////////////////////////////////////////////////////
    //
    // dirty flags.
    // update components only if they are tainted.
    //
    /////////////////////////////////////////////////////////////////
    us _dirtyFlags;
    static const us _DIRTY_FLAG_WORLD_TO_VIEW_MATRIX    = 1 << 0;
    static const us _DIRTY_FLAG_VIEW_TO_WORLD_MATRIX    = 1 << 1;
    static const us _DIRTY_FLAG_VIEW_TO_PROJ_MATRIX     = 1 << 2;
    static const us _DIRTY_FLAG_PROJ_TO_VIEW_MATRIX     = 1 << 3;
    static const us _DIRTY_FLAG_WORLD_TO_PROJ_MATRIX    = 1 << 4;
    static const us _DIRTY_FLAG_VIEW_PORT               = 1 << 5;

    static const us _DIRTY_FLAG_ALL                     = _DIRTY_FLAG_WORLD_TO_VIEW_MATRIX | 
                                                          _DIRTY_FLAG_VIEW_TO_WORLD_MATRIX |
                                                          _DIRTY_FLAG_VIEW_TO_PROJ_MATRIX |
                                                          _DIRTY_FLAG_PROJ_TO_VIEW_MATRIX |
                                                          _DIRTY_FLAG_WORLD_TO_PROJ_MATRIX |
                                                          _DIRTY_FLAG_VIEW_PORT;

protected:
	void initDefaults();

	/////////////////////////////////////////////////////////////////
	//
	// update attributes
	//
	/////////////////////////////////////////////////////////////////
    void updateWorld2ViewMatrix();
    void updateView2WorldMatrix();
    void updateView2ProjMatrix();
    void updateProj2ViewMatrix();
    void updateWorld2ProjMatrix();
    void updateViewPort();

//     /////////////////////////////////////////////////////////////////
//     //
//     // compute view2world and world2view matrices from scratch
//     //
//     /////////////////////////////////////////////////////////////////
//     void computeWorldViewMatrices();

    /////////////////////////////////////////////////////////////////
    //
    // compute view2world and world2view matrices from scratch
    //
    /////////////////////////////////////////////////////////////////
	void computePerspectiveViewProjMatrices( mat4& t_view2Proj, mat4& t_proj2View );
	void computeOrthographicViewProjMatrices( mat4& t_view2Proj, mat4& t_proj2View );
	void getInverseOfProj2View( const mat4& t_proj2View, mat4& t_view2Proj );
	void getInverseOfView2Proj( const mat4& t_view2Proj, mat4& t_proj2View );
	float getHorizontalPerspective( const mat4& t_view2Proj );
	float getVerticalPerspective( const mat4& t_view2Proj );


public:
    /////////////////////////////////////////////////////////////////
    //
    // return the updated matrix
    //
    /////////////////////////////////////////////////////////////////
    const mat4& GetWorld2ViewMatrix();
    const mat4& GetView2WorldMatrix();
    const mat4& GetView2ProjMatrix();
    const mat4& GetProj2ViewMatrix();
    const mat4& GetWorld2ProjMatrix();
    const SViewPort* GetViewPort();

    void GetCameraPosition( vec4& t_pos );
    void GetCameraPositionFaceUpAndRight( vec4& t_pos, vec4& t_face, vec4& t_up, vec4& t_right );

    /////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////
    //
	// init camera
    //
    /////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////////////
    //
    // setup camera.
    // position.
    // facing direction.
    // almost up direction.
    // 
    // it doesn't require the facing direction being 
    // perpendicular to the up direction
    //
    /////////////////////////////////////////////////////////////////
	void SetCameraPostionFaceAndUp( vec4 t_pos, vec4 t_facing, vec4 t_up = vec4( 0.f, 1.f, 0.f, 0.f ) );



	/////////////////////////////////////////////////////////////////
	//
	// set horizontal perspective by horizontal fov angle
	//
	/////////////////////////////////////////////////////////////////
	void SetHorizontalFieldOfView( float t_radAngle );

	/////////////////////////////////////////////////////////////////
	//
	// set horizontal perspective directly
	// this value is a non linear scale where 1.0 == 45 degrees angle of view
	// smaller value gives a wider fov.
	//
	/////////////////////////////////////////////////////////////////
	void SetHorizontalPerspective( float t_perspective );

	void SetAspectRatio( float t_aspect );

};

void View_SetAsActive( CView* t_view );
CView* View_GetActive();