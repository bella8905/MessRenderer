/////////////////////////////////////////////////////////////////
//
//  Practice_11_19_14 - material
//
//  MessRenderer - A very messy renderer
//
//  Copyright (c) 2016 Bella Q
//  
/////////////////////////////////////////////////////////////////

#pragma once

#include "Utl_Include.h"

class CMaterial {
public:
	CMaterial( const Utl::CColor& t_kd, const bool& t_hasSpecular, const Utl::CColor& t_ks, const float& t_splExp, const Utl::CColor& t_ka );
	CMaterial();
	virtual ~CMaterial();

protected:
	bool _hasSpecular;
	Utl::CColor _ks;     // specular reflectance factor
	float _splExp;  // specular power
	Utl::CColor _kd;     // diffuse reflectance factor
	Utl::CColor _ka;     // ambient reflectance factor

public:
	// for ubo
	struct SMATERIAL_UBO_DATA {
		float _splExp;
		float _pading1, _pading2, _pading3;
		vec4 _ks, _kd, _ka;

		// emissive, transparent, reflective

		SMATERIAL_UBO_DATA() : _kd( 0.8f, 0.8f, 0.8f, 1.f ), _ka( 0.2f, 0.2f, 0.2f, 1.f ), _ks( 0.f, 0.f, 0.f, 1.0f ), _splExp( 0.f ) {}
	};

	Utl::CColor& GetKd() { return _kd; }
	void SetKd( const Utl::CColor& t_val ) { _kd = t_val; }
	Utl::CColor& GetKs() { return _ks; }
	void SetKs( const Utl::CColor& t_val ) { _ks = t_val; }
	Utl::CColor& GetKa() { return _ka; }
	void SetKa( const Utl::CColor& t_val ) { _ka = t_val; }
	float& GetSplExp() { return _splExp; }
	void SetSplExp( const float& t_val ) { _splExp = t_val; }
	bool& GetHasSpecular() { return _hasSpecular; }
	void PopulateUBOData( SMATERIAL_UBO_DATA& t_uboData );
	// void GenerateUBO( GLuint& t_uboIndex );
};



// a default material
extern CMaterial g_defaultMat;