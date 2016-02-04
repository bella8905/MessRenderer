/////////////////////////////////////////////////////////////////
//
//  Practice_01_28_16 - view
//
//  MessRenderer - A very messy renderer
//
//  Copyright (c) 2016 Bella Q
//  
/////////////////////////////////////////////////////////////////


#include "View.h"


static struct {
	CView _view;
	
}View;

void View_SetAsActive( const CView& t_view ) {
	View._view = t_view;
}

CView* View_GetActive() {
	return &View._view;
}

void CView::initDefaults() {
	// mark all things as dirty at start
	_dirtyFlags = _DIRTY_FLAG_ALL;

	_bPerspective = true;

	// view 
	vec4 camPos( 0.f, 0.f, 0.f, 1.f );
	vec4 camFace( 0.f, 0.f, -1.f, 0.f );
	vec4 camUp( 0.f, 1.f, 0.f, 0.f );
	SetCameraPostionFaceAndUp( camPos, camFace, camUp );

	// horizontal filed of view
	float horizontalFOV = 80;
	SetHorizontalFieldOfView( Utl::DegToRad( horizontalFOV ) );

}

void CView::updateWorld2ViewMatrix() {
    if( ( _dirtyFlags & _DIRTY_FLAG_WORLD_TO_VIEW_MATRIX ) != 0  ) {
        if( ( _dirtyFlags & _DIRTY_FLAG_VIEW_TO_WORLD_MATRIX ) == 0 ) {
            // inverse matrix
			_world2ViewMatrix = Utl::GetInverseTranslationRotation( _view2WorldMatrix );
            _dirtyFlags &= ~_DIRTY_FLAG_WORLD_TO_VIEW_MATRIX;
        } else {
			assert( false && "World to View matrix was never set. Call a SetCamera** function first. " );
        }
    }
}

void CView::updateView2WorldMatrix() {
    if( ( _dirtyFlags & _DIRTY_FLAG_VIEW_TO_WORLD_MATRIX ) != 0  ) {
        if( ( _dirtyFlags & _DIRTY_FLAG_WORLD_TO_VIEW_MATRIX ) == 0 ) {
            // inverse matrix
			_view2WorldMatrix = Utl::GetInverseTranslationRotation( _world2ViewMatrix );
            _dirtyFlags &= ~_DIRTY_FLAG_VIEW_TO_WORLD_MATRIX;
        } else {
			assert( false && "View to World matrix was never set. Call a SetCamera** function first. " );
        }
    }
}



void CView::updateView2ProjMatrix() {
    if( ( _dirtyFlags & _DIRTY_FLAG_VIEW_TO_PROJ_MATRIX ) != 0  ) {
        if( ( _dirtyFlags & _DIRTY_FLAG_PROJ_TO_VIEW_MATRIX ) == 0 ) {
            // inverse matrix
			getInverseOfView2Proj( _proj2ViewMatrix, _view2ProjMatrix );
            _dirtyFlags &= ~_DIRTY_FLAG_VIEW_TO_PROJ_MATRIX;
        } else {
            // compute view2world and world2view matrix from scratch
			if( _bPerspective ) {
				computePerspectiveViewProjMatrices( _view2ProjMatrix, _proj2ViewMatrix );
			} else {
				computeOrthographicViewProjMatrices( _view2ProjMatrix, _proj2ViewMatrix );
			}

            _dirtyFlags &= ~_DIRTY_FLAG_VIEW_TO_PROJ_MATRIX;
            _dirtyFlags &= ~_DIRTY_FLAG_PROJ_TO_VIEW_MATRIX;

        }
    }
}

void CView::updateProj2ViewMatrix() {
    if( ( _dirtyFlags & _DIRTY_FLAG_PROJ_TO_VIEW_MATRIX ) != 0  ) {
        if( ( _dirtyFlags & _DIRTY_FLAG_VIEW_TO_PROJ_MATRIX ) == 0 ) {
            // inverse matrix
			getInverseOfView2Proj( _view2ProjMatrix, _proj2ViewMatrix );
            _dirtyFlags &= ~_DIRTY_FLAG_PROJ_TO_VIEW_MATRIX;
		} else {
			// compute view2world and world2view matrix from scratch
			if( _bPerspective ) {
				computePerspectiveViewProjMatrices( _view2ProjMatrix, _proj2ViewMatrix );
			} else {
				computeOrthographicViewProjMatrices( _view2ProjMatrix, _proj2ViewMatrix );
			}

            _dirtyFlags &= ~_DIRTY_FLAG_VIEW_TO_PROJ_MATRIX;
            _dirtyFlags &= ~_DIRTY_FLAG_PROJ_TO_VIEW_MATRIX;

        }
    }
}


float CView::getHorizontalPerspective( const mat4& t_view2Proj ) {
	return t_view2Proj[0][0];
}

float CView::getVerticalPerspective( const mat4& t_view2Proj ) {
	return t_view2Proj[1][1];
}

void CView::computePerspectiveViewProjMatrices( mat4& t_view2Proj, mat4& t_proj2View ) {
	float perspHorizontal = getHorizontalPerspective( _view2ProjMatrix );
	float perspVertical = getVerticalPerspective( _view2ProjMatrix );

	float clipNear = 0.1f;
	float clipFar = 100.f;
	float sx = perspHorizontal;
	float sy = perspVertical;
	float sz = -( clipFar + clipNear ) / ( clipFar - clipNear );
	float pz = -( 2.f * clipFar * clipNear ) / ( clipFar - clipNear );

	t_view2Proj = mat4( vec4( sx, 0.f, 0.f, 0.f ),
						vec4( 0.f, sy, 0.f, 0.f ),
						vec4( 0.f, 0.f, sz, -1.f ),
						vec4( 0.f, 0.f, pz, 0.f ) 
						);

	getInverseOfView2Proj( _view2ProjMatrix, t_proj2View );

}

void CView::computeOrthographicViewProjMatrices( mat4& t_view2Proj, mat4& t_proj2View  ) {

}

void CView::getInverseOfProj2View( const mat4& t_proj2View, mat4& t_view2Proj ) {
	// proj to view matrix
	//	a	0	0	0 
	//	0	b	0	0
	//	0	0	0	-1
	//	0	0	c	d

	float a = t_proj2View[0][0];
	float b = t_proj2View[1][1];
	float c = t_proj2View[2][3];
	float d = t_proj2View[3][3];
	t_view2Proj = mat4( vec4( 1.f / a, 0.f, 0.f, 0.f ),
						vec4( 0.f, 1.f / b, 0.f, 0.f ),
						vec4( 0.f, 0.f, d / c, -1.f ),
						vec4( 0.f, 0.f, 1.f / c, 0.f ) 
						);

}

void CView::getInverseOfView2Proj( const mat4& t_view2Proj, mat4& t_proj2View ) {
	// view to proj matrix
	//	sx	0	0	0 
	//	0	sy	0	0
	//	0	0	sz	pz
	//	0	0	-1	0

	float sx = t_view2Proj[0][0];
	float sy = t_view2Proj[1][1];
	float sz = t_view2Proj[2][2];
	float pz = t_view2Proj[3][2];

	t_proj2View = mat4( vec4( 1.f / sx, 0.f, 0.f, 0.f ),
						vec4( 0.f, 1.f / sy, 0.f, 0.f ),
						vec4( 0.f, 0.f, 0.f, 1.f / pz ),
						vec4( 0.f, 0.f, -1.f, sz / pz ) 
						);
}


void CView::updateWorld2ProjMatrix() {
    if( ( _dirtyFlags & _DIRTY_FLAG_WORLD_TO_PROJ_MATRIX ) != 0  ) {
		_world2ProjMatrix = GetView2ProjMatrix() * GetWorld2ViewMatrix();
        _dirtyFlags &= ~_DIRTY_FLAG_WORLD_TO_PROJ_MATRIX;
    }
}

const mat4& CView::GetWorld2ViewMatrix()  { 
    if( ( _dirtyFlags & _DIRTY_FLAG_WORLD_TO_VIEW_MATRIX ) != 0 ) {
        updateWorld2ViewMatrix();
    } 

    return _world2ViewMatrix;
}

const mat4& CView::GetView2WorldMatrix() {
    if( ( _dirtyFlags & _DIRTY_FLAG_VIEW_TO_WORLD_MATRIX ) != 0 ) {
        updateView2WorldMatrix();
    } 

    return _view2WorldMatrix;
}


const mat4& CView::GetView2ProjMatrix() {
    if( ( _dirtyFlags & _DIRTY_FLAG_VIEW_TO_PROJ_MATRIX ) != 0 ) {
        updateView2ProjMatrix();
    } 

    return _view2ProjMatrix;
}


const mat4& CView::GetProj2ViewMatrix() {
    if( ( _dirtyFlags & _DIRTY_FLAG_PROJ_TO_VIEW_MATRIX ) != 0 ) {
        updateProj2ViewMatrix();
    } 

    return _proj2ViewMatrix;
}


const mat4& CView::GetWorld2ProjMatrix() {
	if( ( _dirtyFlags & _DIRTY_FLAG_WORLD_TO_PROJ_MATRIX ) != 0 ) {
		updateWorld2ProjMatrix();
	}

	return _world2ProjMatrix;
}


void CView::updateViewPort() {
    if( ( _dirtyFlags & _DIRTY_FLAG_VIEW_PORT ) != 0  ) {

        _dirtyFlags &= ~_DIRTY_FLAG_VIEW_PORT;
    }
}

const CView::SViewPort* CView::GetViewPort() {
	if( ( _dirtyFlags & _DIRTY_FLAG_VIEW_PORT ) != 0 ) {
		updateViewPort();
	}

	return &_viewPort;
}



void CView::SetCameraPostionFaceAndUp( vec4 t_pos, vec4 t_facing, vec4 t_up ) {
	assert( Utl::IsPosition( t_pos ) );
	assert( Utl::IsDirection( t_facing ) );
	assert( Utl::IsDirection( t_up ) );
	
	vec3 pos = Utl::ToVec3( t_pos );
	vec3 face = Utl::ToVec3( t_facing );
	vec3 up = Utl::ToVec3( t_up );
	vec3 z = -face;
	vec3 x = glm::cross( up, z );
	vec3 y = glm::cross( z, x );

// 
// 	// inverse of a translation matrix
// 	// T(v)^-1 = T(-v)
// 	mat4 t( vec4( 1.f, 0.f, 0.f, 0.f ),
// 			vec4( 0.f, 1.f, 0.f, 0.f ),
// 			vec4( 0.f, 0.f, 1.f, 0.f ),
// 			vec4( -pos, 1.f ) );
// 
// 	// inverse of a rotation matrix
// 	// for orthonormalized matrix,
// 	// R(v)^-1 = transpose( R(v) )
// 	mat4 r( vec4( x.x, y.x, z.x, 0.f ),
// 			vec4( x.y, y.y, z.y, 0.f ),
// 			vec4( x.z, y.z, z.z, 0.f ),
// 			vec4( 0.f, 0.f, 0.f, 1.f ) );
// 
// 	
// 	_world2ViewMatrix = r * t;

    _view2WorldMatrix = mat4( 
                                Utl::ToDirection( x ), 
                                Utl::ToDirection( y ), 
                                Utl::ToDirection( z ), 
                                t_pos 
                                );
	// dirty flags
	_dirtyFlags |= _DIRTY_FLAG_WORLD_TO_VIEW_MATRIX | _DIRTY_FLAG_WORLD_TO_PROJ_MATRIX;
	_dirtyFlags &= ~_DIRTY_FLAG_VIEW_TO_WORLD_MATRIX;
}


void CView::SetHorizontalFieldOfView( float t_radAngle ) {
	assert( _bPerspective );

	float perspecive = 1.f / tan( t_radAngle * 0.5f );
	SetHorizontalPerspective( perspecive );
}


void CView::SetHorizontalPerspective( float t_perspective ) {
	assert( !Utl::Equals( t_perspective, 0.f ) );
	assert( _bPerspective );

	if( Utl::Equals( _view2ProjMatrix[0][0], t_perspective ) ) {
		return;
	}

	_view2ProjMatrix[0][0] = t_perspective;

	float defaultAspect = 800.f / 600.f;
	float verticalPerspective = t_perspective * defaultAspect;
	_view2ProjMatrix[1][1] = verticalPerspective;

	// dirty flags
	_dirtyFlags |= _DIRTY_FLAG_VIEW_TO_PROJ_MATRIX | _DIRTY_FLAG_PROJ_TO_VIEW_MATRIX | _DIRTY_FLAG_WORLD_TO_PROJ_MATRIX;
}



void CView::SetAspectRatio( float t_aspect ) {
	float oldAspect = _view2ProjMatrix[1][1] / _view2ProjMatrix[0][0];
	if( Utl::Equals( oldAspect, t_aspect ) ) {
		return;
	}

	float verticalPerspective = _view2ProjMatrix[0][0] * t_aspect;
	_view2ProjMatrix[1][1] = verticalPerspective;

	// dirty flags
	_dirtyFlags |= _DIRTY_FLAG_VIEW_TO_PROJ_MATRIX | _DIRTY_FLAG_PROJ_TO_VIEW_MATRIX | _DIRTY_FLAG_WORLD_TO_PROJ_MATRIX;

}


void CView::GetCameraPosition( vec4& t_pos ) {
    mat4 view2world = GetView2WorldMatrix();
    t_pos = view2world[3];
}

void CView::GetCameraPositionFaceUpAndRight( vec4& t_pos, vec4& t_face, vec4& t_up, vec4& t_right ) {
    mat4 view2world = GetView2WorldMatrix();
    // tx, ty, tz
    t_pos   = view2world[3];
    // r00, r01, r02
    t_right = view2world[0];
    // r10, r11, r12
    t_up    = view2world[1];
    // r20, r21, r22
    t_face  = -view2world[2];
}