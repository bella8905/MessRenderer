/////////////////////////////////////////////////////////////////
//
//  Practice_11_19_14 - light
//
//  MessRenderer - A very messy renderer
//
//  Copyright (c) 2016 Bella Q
//  
/////////////////////////////////////////////////////////////////

#pragma once

#include "Utl_Include.h"


class CLight {
public:
	CLight();
	CLight( const vec3& t_pos, const vec3& t_ls, const vec3& t_ld, const vec3& t_la );
	virtual ~CLight( void );

protected:
	vec3 _pos;
	vec3 _ls;     // specular light color
	vec3 _ld;     // diffuse light color
	vec3 _la;     // ambient light color

public:
	// for ubo
	struct SLIGHTING_UBO_DATA {
		vec3 _lightPosInWorld;
		float _pading1;
		vec4 _ls, _ld, _la;
	};

	void Setup( const vec3& t_pos, const vec3& t_ls, const vec3& t_ld, const vec3& t_la );
	vec3& GetPos() { return _pos; }
	void SetPos( const vec3& t_val ) { _pos = t_val; }
	vec3& GetLa() { return _la; }
	void SetLa( const vec3& t_val ) { _la = t_val; }
	vec3& GetLd() { return _ld; }
	void SetLd( const vec3& t_val ) { _ld = t_val; }
	vec3& GetLs() { return _ls; }
	void SetLs( const vec3& t_val ) { _ls = t_val; }
	void PopulateUBOData( SLIGHTING_UBO_DATA& t_uboData );
};

extern CLight g_simpleLight;