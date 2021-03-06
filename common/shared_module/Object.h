/////////////////////////////////////////////////////////////////
//
//  Practice_11_19_14 - object
//
//  MessRenderer - A very messy renderer
//
// an obj is the real object in scene
// which includes the geo type, model transformation, material info, and shader info
//
//  Copyright (c) 2016 Bella Q
//  
/////////////////////////////////////////////////////////////////
#pragma once

#include "Utl_Include.h"
#include "Material.h"
#include "Geometry.h"
#include "Shader.h"
#include "glm/gtx/transform.hpp"

struct SArcball {
	vec3 _center;   // center of the obj, caled by center of bb and model matrix
	bool _isInRot;
	vec3 _rotStartPoint;
	vec3 _rotEndPoint;

	static float _radius;
	// arcball related vbo and vao
	// use vao and vbo and update vbo data to draw points if needed
	// maybe I'm naive to implement it this way

	static GLuint _vao;
	static GLuint _vbo, _ibo;
	static bool _arcball_inited;

	SArcball() : _center( 0.f ), _isInRot( false ) {
	}

	static void InitArcball();
	static void DeinitArcball();

	// if we have rotated model with arcball and return the rotation matrix
	bool RayIntersectTestWithArcball( const Utl::SRay& t_ray, const bool& t_isStart );
	void DrawArcball();

};

class CObj {
public:
	CObj( GEO_TYPE t_type );
	virtual ~CObj();

public:
	// SHADER_TYPE _shaderType;

	bool _selected;
	static bool _drawBB;
    static bool _drawWireframe;
	static bool _drawAcball;
    static bool _drawBones;
    

protected:
	GEO_TYPE _geoType;
	CMaterial _material;
	CShader* _shader;

	// don't set the model matrix and inverse model matrix directly
	vec3 _translate;
	glm::mat3 _rot;
	float _scale;   // only support uniform scaling
	mat4 _modelMat;
	mat4 _invModelMat;

	// arcball
	SArcball _arcball;

protected:
	void resetModelMatrix();
	float rayIntersectWithBB( const Utl::SRay& t_ray, const SBoundBox* t_bb );

public:
	void DrawObj();
	void SetupModelMatrix( const vec3& t_translate, const glm::mat3& t_rot, const float& t_scale );
	void SetupModelMatrix( const vec3& t_translate, const glm::mat4& t_rot, const float& t_scale ) {
		glm::mat3 rot;
		rot[0] = vec3( t_rot[0] );
		rot[1] = vec3( t_rot[1] );
		rot[2] = vec3( t_rot[2] );
		SetupModelMatrix( t_translate, rot, t_scale );
	}

	void RevertModelMatrix() {
		_rot = mat3( 1.f );
		_translate = vec3( 0.f );
		_scale = 1.f;

		resetModelMatrix();
	}


	void SetRot( const mat3& t_rot ) {
		_rot = t_rot;
		resetModelMatrix();
	}

	void SetShader( const SHADER_TYPE& t_shader );
	void SetShader( CShader* t_shader ); 

	// test if bb is hit by ray, and return the dist from ray origin to hit point
	float RayIntersectTestWithBB( const Utl::SRay& t_ray );
	void RayIntersectTestWithArcball( const Utl::SRay& t_ray, const bool& t_isStart );

	// void StartRot() { _arcball._isInRot = true; }
	void EndRot() { _arcball._isInRot = false; }

};

