/////////////////////////////////////////////////////////////////
//
//  Utilities - Math
// 
//  A reusable package including things we need for MessRenderer.
//
//
//  Copyright (c) 2015 Bella Q
//  
/////////////////////////////////////////////////////////////////

#include "Utl_Math.h"
#include "glm/gtx/transform.hpp"
#include <assert.h>
#include "View.h"

namespace Utl {

	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	//
	// global variables
	//
	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	const float g_Pi = atan( 1.0f ) * 4.0f;
	const float g_Pi2o = 180.0f / g_Pi;
	const float g_o2Pi = g_Pi / 180.0f;


	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	//
	// angle conversion
	//
	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////

	// radian to degree
	float RadToDeg( const float& t_rad ) {
		return t_rad * g_Pi2o;
	}

	// degree to radian
	float DegToRad( const float& t_deg ) {
		return t_deg * g_o2Pi;
	}



	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	//
	// vectors and points 
	//
	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	vec3 ToVec3( const vec4& t_v ) {
		return vec3( t_v.x, t_v.y, t_v.z );
	}

	vec4 ToPositon( const vec3& t_v ) {
		return vec4( t_v, 1.f );
	}


	vec4 ToDirection( const vec3& t_v ) {
		return vec4( t_v, 0.f );
	}

	bool SamePoints( const vec3& t_p1, const vec3& t_p2 ) {
		return Equals( t_p1.x, t_p2.x ) && Equals( t_p1.y, t_p2.y ) && Equals( t_p1.z, t_p2.z );
	}


	bool AreParallel( const vec4& t_v1, const vec4& t_v2 ) {
		assert( Equals( t_v1.w, 0.f ) );
		if( Equals( t_v1.y * t_v2.z, t_v1.z * t_v2.y ) &&
			Equals( t_v1.z * t_v2.x, t_v1.x * t_v2.z ) &&
			Equals( t_v1.x * t_v2.y, t_v1.y * t_v2.x ) ) {
			return true;
		}

		return false;
	}

	bool ArePerpendicular( const vec4& t_v1, const vec4& t_v2 ) {
		return Equals( glm::dot( t_v1, t_v2 ), 0.f );
	}

	bool IsPosition( const vec4& t_v ) {
		return Equals( t_v.w, 1.f );
	}


	bool IsDirection( const vec4& t_v ) {
		return Equals( t_v.w, 0.f );
	}




	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	//
	// matrix 
	//
	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	mat4 Matrix_ToMat4( const mat3& t_rot ) {
		mat4 rot(	
					vec4( t_rot[0], 0.f ),
					vec4( t_rot[1], 0.f ),
					vec4( t_rot[2], 0.f ),
					vec4( 0.f, 0.f, 0.f, 1.f )
				);
		return rot;
	}

	mat3 Matrix_ToMat3( const mat4& t_mat ) {
		mat3 rot = mat3(	
							vec3( t_mat[0] ),
							vec3( t_mat[1] ),
							vec3( t_mat[2] )
					);
		return rot;

	}

	mat4 Matrix_GetTransformMatrixFromTranslateRotateScale( const vec3& t_Trans, const vec3& t_Rotat, const vec3& t_Scale ) {
		mat4 translationMat = glm::translate( t_Trans );
		mat4 rotationMat = glm::rotate( t_Rotat.x, glm::vec3( 1.f, 0.f, 0.f ) );
		rotationMat = rotationMat*glm::rotate( glm::mat4(), t_Rotat.y, glm::vec3( 0.f, 1.f, 0.f ) );
		rotationMat = rotationMat*glm::rotate( glm::mat4(), t_Rotat.z, glm::vec3( 0.f, 0.f, 1.f ) );
		mat4 scaleMat = glm::scale( t_Scale );
		return translationMat*rotationMat*scaleMat;
	}
	

	mat4 Matrix_GetInverseTranslationRotation( const mat4& t_transform ) {
		mat3 rot = Matrix_ToMat3( t_transform );
		mat3 inv_rot = glm::transpose( rot );
		vec3 translate;
		Matrix_GetTranslation( t_transform, translate );

		mat4 result = Matrix_ToMat4( inv_rot );
		result[3][0] = -result[0][0] * translate[0] - result[1][0] * translate[1] - result[2][0] * translate[2];
		result[3][1] = -result[0][1] * translate[0] - result[1][1] * translate[1] - result[2][1] * translate[2];
		result[3][2] = -result[0][2] * translate[0] - result[1][2] * translate[1] - result[2][2] * translate[2];

		return result;
	}

    bool Matrix_IsOrthonormal( const mat4& t_m, float t_lengthThreshold, float t_cosineThreshold ) {
        float len0 = glm::length( t_m[0] );
        float len1 = glm::length( t_m[1] );
        float len2 = glm::length( t_m[2] );
        float dot0 = glm::dot( t_m[0],  t_m[1] );
        float dot1 = glm::dot( t_m[1],  t_m[2] );
        float dot2 = glm::dot( t_m[2],  t_m[0] );
        return
            Equals( t_m[3][3], 1.f ) &&                                                            //m33 must be exactly 1.0
            Equals( t_m[0][3], 0.f ) && Equals( t_m[1][3], 0.f ) && Equals( t_m[2][3], 0.f ) &&    //m03, m13 and m23 must be exactly 0.0
            abs( len0 - 1.0 ) <= t_lengthThreshold &&                                           //the length of each rotation row must be unit.
            abs( len1 - 1.0 ) <= t_lengthThreshold &&                                           //the length of each rotation row must be unit.
            abs( len2 - 1.0 ) <= t_lengthThreshold &&                                           //the length of each rotation row must be unit.
            abs( dot0 ) <= t_cosineThreshold &&                                                 //the angle between each rotation row must be 90 degrees.
            abs( dot1 ) <= t_cosineThreshold &&                                                 //the angle between each rotation row must be 90 degrees.
            abs( dot2 ) <= t_cosineThreshold;
    }



	// only for affine transformation
	void Matrix_GetTranslation( const mat4& t_transform, vec3& t_out ) {
		t_out = vec3( t_transform[3][0], t_transform[3][1], t_transform[3][2] );
	}

	void Matrix_GetTranslation( const mat4& t_transform, mat4& t_out ) {
		t_out = mat4();
		t_out[3] = t_transform[3];
	}


	void Matrix_GetRotation( const mat4& t_transform, mat3& t_out ) {
		vec3 scale;
		Matrix_GetScale( t_transform, scale );
		mat3 rotScale = Matrix_ToMat3( t_transform );
		t_out = mat3( rotScale[0] / scale[0],
					  rotScale[1] / scale[1],
					  rotScale[2] / scale[2]
					  );
	}

	void Matrix_GetRotation( const mat4& t_transform, mat4& t_out ) {
		mat3 rot;
		Matrix_GetRotation( t_transform, rot );
		t_out = Matrix_ToMat4( rot );
	}


	void Matrix_GetScale( const mat4& t_transform, vec3& t_out ) {
		mat3 rotScale = Matrix_ToMat3( t_transform );
		t_out[0] = glm::length( rotScale[0] );
		t_out[1] = glm::length( rotScale[1] );
		t_out[2] = glm::length( rotScale[2] );
	}

	void Matrix_GetScale( const mat4& t_transform, mat4& t_out ) {
		vec3 scale;
		Matrix_GetScale( t_transform, scale );
		t_out = mat4( vec4( scale[0], 0.f, 0.f, 0.f ),
					  vec4( 0.f, scale[1], 0.f, 0.f ),
					  vec4( 0.f, 0.f, scale[2], 0.f ),
					  vec4( 0.f, 0.f, 0.f, 1.f )
					  );
	}


	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	//
	// Ray
	//
	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	float SRay::RayIntersectTestWithSphere( const vec4& t_center, const float& t_radius ) {
		vec4 posObj = _Origin;
		vec4 dirObj = _Dir;

		// solve function: At2 + Bt + C = 0
		float A = dirObj.x * dirObj.x + dirObj.y * dirObj.y + dirObj.z * dirObj.z;
		float B = 2 * ( ( posObj.x - t_center.x ) * dirObj.x + ( posObj.y - t_center.y ) * dirObj.y + ( posObj.z - t_center.z ) * dirObj.z );
		float C = ( posObj.x - t_center.x ) * ( posObj.x - t_center.x ) + ( posObj.y - t_center.y ) * ( posObj.y - t_center.y ) + ( posObj.z - t_center.z ) * ( posObj.z - t_center.z ) - t_radius * t_radius;

		if( Utl::Equals( A, 0.f ) ) {
			LogError << "anything wrong with ray?" << LogEndl;
			return -1.f;
		}

		float Radicand = B * B - 4 * A * C;
		if( Radicand < 0 )  return -1.f;

		float t1 = ( -B + sqrt( Radicand ) ) * 0.5f / A;
		float t2 = ( -B - sqrt( Radicand ) ) * 0.5f / A;

		float tmax = max( t1, t2 );
		float tmin = min( t1, t2 );

		if( tmax < 0 ) return -1.f;

		float t;
		if( tmin >= 0 ) t = tmin;
		else t = tmax;

		return t;
	}


	float SRay::RayIntersectTestWithAxisAlignedBox( const vec3& t_mins, const vec3& t_maxs ) {
		vec4 posObj = _Origin;
		vec4 dirObj = _Dir;

		// if xd, yd, zd equals to zero, skip this direction
		float tNear = -std::numeric_limits<float>::infinity();
		float tFar = std::numeric_limits<float>::infinity();
		float t1, t2;

		if( Utl::Equals( dirObj[0], 0.f ) && Utl::Equals( dirObj[1], 0.f ) && Utl::Equals( dirObj[2], 0.f ) )   return -1.f;

		for( int i = 0; i < 3; ++i ) {
			if( Utl::Equals( dirObj[i], 0.f ) ) {
				if( posObj[i] > t_maxs[i] || posObj[i] < t_mins[i] )  return -1.f;
			} else {
				t1 = ( t_mins[i] - posObj[i] ) / dirObj[i];
				t2 = ( t_maxs[i] - posObj[i] ) / dirObj[i];
				tNear = max( tNear, min( t1, t2 ) );
				tFar = min( tFar, max( t1, t2 ) );

				if( tNear > tFar || tFar < 0 )  return -1.f;
			}
		}

		return tNear;
	}


    /////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////
    //
    // misc
    //
    /////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////

    bool IsPOT( int t_val ) {
        return ( t_val  & ( t_val -1 ) ) == 0 ;
    }

}