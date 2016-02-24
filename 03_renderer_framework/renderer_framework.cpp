/////////////////////////////////////////////////////////////////
//
//  renderer framework - main
//
//
//  Copyright (c) 2016 Bella Q
//  
/////////////////////////////////////////////////////////////////

#include "Camera.h"
#include "Shader.h"
#include "Geometry.h"
#include "Light.h"
#include "Scene.h"
#include "View.h"
#include "stb_image/stb_image_write.h"
#include "assimp/scene.h"
#include "glm/gtx/transform.hpp"

#include "Utl_Include.h"

#include "MessRendererApp.h"

class CTestApp : public MessRenderer::CApp {
public:
	CTestApp() : MessRenderer::CApp( "test app" ) {}
	~CTestApp() {}

protected:
	CScene _scene;
	CView _view;

protected:
	virtual void _startup();
	virtual void _update( double t_deltaTime );
	virtual void _render();
	virtual void _shutdown();

	virtual void _onKeyPressed( int t_key, int t_action, int t_mods );

	void _initModules();
	void _deinitModules();
	virtual void _setupScene();

};

void CTestApp::_initModules() {
	CShaderContainer::GetInstance().Init();
	CGeoContainer::GetInstance().Init();
	SArcball::InitArcball();
}

void CTestApp::_deinitModules() {
	SArcball::DeinitArcball();
	CGeoContainer::GetInstance().Deinit();
	CShaderContainer::GetInstance().Deinit();

}

void CTestApp::_setupScene() {
	////////////////////////////////////////////////////////
	// init scenes

 	glm::vec3 camPos( 0.f, 0.f, 2.0f );
 	glm::vec3 camTarget( 0.f, 0.f, 0.f );
 	glm::vec3 camFace = glm::normalize( camTarget - camPos );

	_view.SetCameraPostionFaceAndUp( Utl::ToPositon( camPos ), Utl::ToDirection( camFace ) );
	_view.SetHorizontalFieldOfView( Utl::DegToRad( 80.f ) );
	View_SetAsActive( &_view );

	// light
	vec3 lightPos( 0.f, 0.f, 2.f );
	vec3 lightLs( 1.f, 1.f, 1.f );
	vec3 lightLd( 0.7f, 0.7f, 0.7f );
	vec3 lightLa( 0.2f, 0.2f, 0.2f );

	g_simpleLight.Setup( lightPos, lightLs, lightLd, lightLa );

	// material
	bool hasSpecular = true;
	Utl::CColor ks( 1.f, 1.f, 1.f );
	float specularExp = 10.f;
	Utl::CColor kd( 1.f, 0.5f, 0.f );
	Utl::CColor ka( 1.f, 1.f, 1.f );

	CMaterial blinnMat( kd, hasSpecular, ks, specularExp, ka );

	// geos
	vec3 translate_left( -0.8f, 0.f, 0.f );
	vec3 translate_center( 0.f, 0.f, 0.f );
	vec3 translate_right( 0.8f, 0.f, 0.f );
	glm::mat3 rot_noRot( 1.f );
	mat4 rot_x30 = glm::rotate( glm::mat4(), 30 * Utl::g_o2Pi, glm::vec3( 1, 0, 0 ) );
	float scale_s = 0.5f;
	float scale_xs = 0.1f;

	// cube 
	CObj obj_cube( GEO_UNIT_CUBE );
	obj_cube.SetupModelMatrix( translate_left, rot_x30, scale_s );
	obj_cube.SetShader( SD_NORMAL_TEST );
	_scene.AddObj( obj_cube );

	CObj obj_sphere( GEO_SPIDER );
	// obj_sphere._material = blinnMat;
	obj_sphere.SetupModelMatrix( translate_right, rot_noRot, scale_xs );
	obj_sphere.SetShader( SD_PHONG );
	_scene.AddObj( obj_sphere );


	CObj obj_triangle( GEO_TRIANGLE );
	obj_triangle.SetupModelMatrix( translate_center, rot_noRot, scale_s );
	obj_triangle.SetShader( SD_NORMAL_TEST );
	_scene.AddObj( obj_triangle );

}


void CTestApp::_startup() {
	_initModules();
	_setupScene();

    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LESS );

	glCullFace( GL_BACK );
	glFrontFace( GL_CCW );


}

void CTestApp::_update( double t_deltaTime ) {
	
}

void CTestApp::_render() {
	glClearColor( 0.5f, 0.5f, 0.5f, 1.f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	if( !_info._flags._fullScreen ) {
		glViewport( 0, 0, _info._winWidth, _info._winHeight );
	}

	_scene.Draw();

}

void CTestApp::_shutdown() {
	_deinitModules();
}

void CTestApp::_onKeyPressed( int t_key, int t_action, int t_mods ) {
}


// app entry
int main( int argc, const char ** argv ) {
	CTestApp *app = new CTestApp();
    app->Run();                                  
	delete app;                                     
	return 0;                                      
}