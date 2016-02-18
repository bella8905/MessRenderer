/////////////////////////////////////////////////////////////////
//
//  Practice_11_19_14 - material
//
//  MessRenderer - A very messy renderer
//
//  Copyright (c) 2016 Bella Q
//  
/////////////////////////////////////////////////////////////////

#include "Material.h"


CMaterial::CMaterial( const Utl::CColor& t_kd, const bool& t_hasSpecular, const Utl::CColor& t_ks, const float& t_splExp, const Utl::CColor& t_ka ) : _ks( t_ks ), _kd( t_kd ), _ka( t_ka ), _splExp( t_splExp ), _hasSpecular( t_hasSpecular ) {
}

CMaterial::CMaterial() : _splExp( 1.f ), _hasSpecular( false ) {

}

CMaterial::~CMaterial( void ) {
}

void CMaterial::PopulateUBOData( SMATERIAL_UBO_DATA& t_uboData ) {
	t_uboData._kd = _kd._Color;
	t_uboData._ka = _ka._Color;

	if( _hasSpecular ) {
		t_uboData._ks = _ks._Color;
		t_uboData._splExp = _splExp;
	} else {
		// set ks to all zeros
		t_uboData._ks = vec4( 0.f, 0.f, 0.f, 0.f );
		t_uboData._splExp = 0.f;
	}
}

// void CMaterial::GenerateUBO( GLuint& t_uboIndex ) {
// 	 
// }

// a little purple color
CMaterial g_defaultMat( Utl::CColor( 1.f, 0.5f, 1.f ), false, Utl::CColor(), 0.f, Utl::CColor( 1.f, 1.f, 1.f ) );