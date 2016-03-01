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


class CMultiViewportsApp : public MessRenderer::CApp {
public:
	CMultiViewportsApp() : MessRenderer::CApp( "Multiple Viewports" ) {}
	~CMultiViewportsApp() {}

protected:
	CScene _scene;
	CView _view1;
	CView _view2;

protected:
	virtual void _startup();
	virtual void _updateControls( double _deltaTime );
	virtual void _render();
	virtual void _shutdown();
	virtual void _onWindowResize( int t_w, int t_h );

	void _initModules();
	void _deinitModules();
	virtual void _setupScene();

};

void CMultiViewportsApp::_initModules() {
	CShaderContainer::GetInstance().Init();
	CGeoContainer::GetInstance().Init();
}

void CMultiViewportsApp::_deinitModules() {
	CGeoContainer::GetInstance().Deinit();
	CShaderContainer::GetInstance().Deinit();
}


void CMultiViewportsApp::_setupScene() {
	////////////////////////////////////////////////////////
	// init scenes

 	glm::vec3 camPos( 0.f, 0.f, 2.0f );
	// glm::vec3 camPos( 0.15f, -0.63f, 1.89f );
 	glm::vec3 camTarget( 0.f, 0.f, 0.f );
 	glm::vec3 camFace = glm::normalize( camTarget - camPos );
	glm::vec3 camUp( 0.f, 1.f, 0.f );

	_view1.SetCameraPostionFaceAndUp( Utl::ToPositon( camPos ), Utl::ToDirection( camFace ) );
	_view1.SetHorizontalFieldOfView( Utl::DegToRad( 80.f ) );
	CView::SViewPort viewport;
	viewport._width = 400;
	viewport._height = 600;
	_view1.SetViewport( viewport );

	_view2 = _view1;
	viewport._x = 400;
	_view2.SetViewport( viewport );

/*	View_SetAsActive( &_view1 );*/

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

	CObj obj( GEO_UNIT_CUBE );
	// obj_sphere._material = blinnMat;
	obj.SetupModelMatrix( translate_center, rot_x30, scale_s );
	obj.SetShader( SD_NORMAL_TEST );
	_scene.AddObj( obj );


}


void CMultiViewportsApp::_startup() {
	_initModules();

	_setupScene();

    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LESS );

	glCullFace( GL_BACK );
	glFrontFace( GL_CCW );


}

void CMultiViewportsApp::_updateControls( double _deltaTime ) {
	CApp::_updateControls( _deltaTime );


}



void CMultiViewportsApp::_render() {

	View_SetAsActive( &_view1 );
	glClearColor( 0.5f, 0.5f, 0.5f, 1.f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	if( !_info._flags._fullScreen ) {
		// glViewport( 0, 0, _info._winWidth, _info._winHeight );
	}

	_scene.Draw();


	View_SetAsActive( &_view2 );
	glClearColor( 1.f, 0.f, 0.f, 1.f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	_scene.Draw();

}

void CMultiViewportsApp::_shutdown() {
	_deinitModules();
}

void CMultiViewportsApp::_onWindowResize( int t_w, int t_h ) {
	MessRenderer::CApp::_onWindowResize( t_w, t_h );
// 	// also update the view
// 	// get a copy of viewport
// 	CView::SViewPort viewport = *_view1.GetViewPort();
// 	viewport._width = t_w;
// 	viewport._height = t_h;
// 	_view1.SetViewport( viewport );
}


// app entry
int main( int argc, const char ** argv ) {
	CMultiViewportsApp *app = new CMultiViewportsApp();
    app->Run();                                  
	delete app;                                     
	return 0;                                      
}