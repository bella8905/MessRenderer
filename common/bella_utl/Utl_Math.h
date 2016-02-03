/////////////////////////////////////////////////////////////////
//
//  Utilities - Math
// 
//  a reusable package including things we need for MessRenderer.
//
//  Copyright (c) 2015 Bella Q
//  
/////////////////////////////////////////////////////////////////
#pragma  once 

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/quaternion.hpp"
#include "Utl_Global.h"

using glm::vec4;
using glm::vec3;
using glm::mat4;
using glm::mat3;

namespace Utl {


	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	//
	// global variables
	//
	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	extern const float g_Pi;
	extern const float g_Pi2o;
	extern const float g_o2Pi;


	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	//
	// angle conversion
	//
	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////

	// radian to degree
	extern float RadToDeg( const float& t_rad );
	// degree to radian
	extern float DegToRad( const float& t_deg );


	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	//
	// vectors and points 
	//
	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////

	extern vec3 ToVec3( const vec4& t_v );
	extern vec4 ToPositon( const vec3& t_v );
	extern vec4 ToDirection( const vec3& t_v );
	extern bool SamePoints( const vec3& t_p1, const vec3& t_p2 );
	extern bool AreParallel( const vec4& t_v1, const vec4& t_v2 );
	extern bool ArePerpendicular( const vec4& t_v1, const vec4& t_v2 );
	extern bool IsPosition( const vec4& t_v );
	extern bool IsDirection( const vec4& t_v );

	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	//
	// matrix 
	//
	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	extern mat4 ToMat4( const mat3& t_mat );
	extern mat3 ToMat3( const mat4& t_mat );

	/////////////////////////////////////////////////////////////////
	//
	// get transform matrix from translate, rotate and scale
	//
	/////////////////////////////////////////////////////////////////
	extern mat4 GetTransformMatrixFromTranslateRotateScale( const vec3& t_Trans, const vec3& t_Rotat, const vec3& t_Scale );

	/////////////////////////////////////////////////////////////////
	// belows are only for affine transformations
	/////////////////////////////////////////////////////////////////
	//
	// get translation from transform matrix
	//
	/////////////////////////////////////////////////////////////////
	extern void GetTranslation( const mat4& t_transform, vec3& t_out );
	extern void GetTranslation( const mat4& t_transform, mat4& t_out );

	/////////////////////////////////////////////////////////////////
	//
	// get rotation from transform matrix
	//
	/////////////////////////////////////////////////////////////////
	extern void GetRotation( const mat4& t_transform, mat3& t_out );
	extern void GetRotation( const mat4& t_transform, mat4& t_out );

	/////////////////////////////////////////////////////////////////
	//
	// get scale from transform matrix
	//
	/////////////////////////////////////////////////////////////////
	extern void GetScale( const mat4& t_transform, vec3& t_out );
	extern void GetScale( const mat4& t_transform, mat4& t_out );

	/////////////////////////////////////////////////////////////////
	//
	// get inverse matrix for an transformation,
	// which only contains rotation and translation.
	// this is the most common matrix we see for our engine.
	//
	/////////////////////////////////////////////////////////////////
	extern mat4 GetInverseTranslationRotation( const mat4& t_transform );




	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	//
	// Ray and Intersection 
	//
	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	struct SRay {
		vec4 _Origin;
		vec4 _Dir;  // normalized direction vector

		SRay() {}
		SRay( const vec4& t_Origin, const vec4& t_Dir ) : _Origin( t_Origin ), _Dir( glm::normalize( t_Dir ) ) {}
		SRay( const vec3& t_Origin, const vec3& t_Dir ) : _Origin( t_Origin, 1.f ), _Dir( glm::normalize( t_Dir ), 0.f ) {}

		vec4 GetPointOnRay( const float& t_Scale ) const { return _Origin + t_Scale * _Dir; }
		SRay Transform( const mat4& t_TransMat ) const { return SRay( t_TransMat * _Origin, t_TransMat * _Dir ); }
		SRay Translate( const vec3& t_offset ) const { return SRay( vec4( t_offset, 0.f ) + _Origin, _Dir ); }
		SRay GetRefletedRay( const vec4& t_InstPoint, const vec4& t_SurfaceNormal ) const {
			vec4 RflRayDir;
			vec4 NormalizedNormal = glm::normalize( t_SurfaceNormal );
			if( AreParallel( _Dir, NormalizedNormal ) ) RflRayDir = NormalizedNormal;
			else if( ArePerpendicular( _Dir, NormalizedNormal ) ) RflRayDir = _Dir;
			else RflRayDir = _Dir - 2.f* NormalizedNormal* ( glm::dot( _Dir, NormalizedNormal ) );

			return SRay( t_InstPoint, RflRayDir );
		}
	};

}