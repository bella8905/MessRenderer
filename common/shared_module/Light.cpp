#include "Light.h"


CLight::CLight() : _pos( vec3() ), _ls( vec3() ), _ld( vec3() ), _la( vec3() ) {

}

CLight::CLight( const vec3& t_pos, const vec3& t_ls, const vec3& t_ld, const vec3& t_la ) : _pos( t_pos ), _ls( t_ls ), _ld( t_ld ), _la( t_la ) {
}


CLight::~CLight( void ) {
}


void CLight::Setup( const vec3& t_pos, const vec3& t_ls, const vec3& t_ld, const vec3& t_la ) {
	_pos = t_pos;
	_ls = t_ls;
	_ld = t_ld;
	_la = t_la;
}

void CLight::PopulateUBOData( SLIGHTING_UBO_DATA& t_uboData ) {
	t_uboData._la = Utl::ToDirection( _la );
	t_uboData._ld = Utl::ToDirection( _ld );
	t_uboData._ls = Utl::ToDirection( _ls );
	t_uboData._lightPosInWorld = _pos;
}


CLight g_simpleLight;