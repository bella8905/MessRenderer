/////////////////////////////////////////////////////////////////
//
//  freefly camera - main
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
#include "glm/gtx/transform.hpp"

#include "Utl_Include.h"

#include "MessRendererApp.h"


class CFreeflyCamApp : public MessRenderer::CApp {
public:
	CFreeflyCamApp() : MessRenderer::CApp( "Freefly Camera" ) {}
	~CFreeflyCamApp() {}

protected:
	CScene _scene;
	CFreeFlyCamera _freeflyCam;
	CView _view;

protected:
	virtual void _startup();
	virtual void _updateControls( double _deltaTime );
	virtual void _render();
	virtual void _shutdown();

	void _initModules();
	void _deinitModules();
	virtual void _setupScene();

};

void CFreeflyCamApp::_initModules() {
	CShaderContainer::GetInstance().Init();
	CGeoContainer::GetInstance().Init();
}

void CFreeflyCamApp::_deinitModules() {
	CGeoContainer::GetInstance().Deinit();
	CShaderContainer::GetInstance().Deinit();
}

void CFreeflyCamApp::_setupScene() {
	////////////////////////////////////////////////////////
	// init scenes

 	glm::vec3 camPos( 0.f, 0.f, 2.0f );
 	glm::vec3 camTarget( 0.f, 0.f, 0.f );
 	glm::vec3 camFace = glm::normalize( camTarget - camPos );

	_freeflyCam.Setup( Utl::ToPositon( camPos ), Utl::ToPositon( camTarget ) );

 	// _view.SetCameraPostionFaceAndUp( Utl::ToPositon( camPos ), Utl::ToDirection( camFace ) );
 	// _view.SetHorizontalFieldOfView( Utl::DegToRad( 80.f ) );
	_freeflyCam.UpdateView( &_view );
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
	mat4 rot_y90 = glm::rotate( glm::mat4(), 90 * Utl::g_o2Pi, glm::vec3( 0, 1, 0 ) );
	float scale_s = 0.5f;
	float scale_xs = 0.1f;

	CObj obj( GEO_SPIDER );
	// obj_sphere._material = blinnMat;
	obj.SetupModelMatrix( translate_center, rot_x30 * rot_y90, scale_s );
	obj.SetShader( SD_NORMAL_TEST );
	_scene.AddObj( obj );


}


void CFreeflyCamApp::_startup() {
	_initModules();

	_setupScene();

    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LESS );

	glCullFace( GL_BACK );
	glFrontFace( GL_CCW );


}

void CFreeflyCamApp::_updateControls( double _deltaTime ) {
	CApp::_updateControls( _deltaTime );

	_freeflyCam.UpdateControl( _deltaTime );
	_freeflyCam.UpdateView( &_view );

}

void CFreeflyCamApp::_render() {
	glClearColor( 0.5f, 0.5f, 0.5f, 1.f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	if( !_info._flags._fullScreen ) {
		glViewport( 0, 0, _info._winWidth, _info._winHeight );
	}

	_scene.Draw();

}

void CFreeflyCamApp::_shutdown() {
	_deinitModules();
}


// app entry
int main( int argc, const char ** argv ) {
	CFreeflyCamApp *app = new CFreeflyCamApp();
    app->Run();                                  
	delete app;                                     
	return 0;                                      
}