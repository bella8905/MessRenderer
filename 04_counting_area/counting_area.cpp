/////////////////////////////////////////////////////////////////
//
//  area counting and painting - main
//
//  count number of fragments covered by scene,
//  and use the counter to decide the brightness of objects in scene.
//  this could give an effect of distance based rendering.
//
//  objects further away cover less area, offering a darker scene.
//  objects closer to camera cover more area, offering a brighter scene.
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

class CRenderer : public MessRenderer::CApp {
public:
    CRenderer() : MessRenderer::CApp( "Distance based painting" ) {}
    ~CRenderer() {}

protected:
    CScene _scene;
	CView _view;

protected:
    virtual void _startup();
    virtual void _update( double t_deltaTime );
    virtual void _render();
    virtual void _shutdown();

    virtual void _onKeyPressed( int t_key, int t_action, int t_mods );
    virtual void _onMouseScroll( double t_offset  );

    void _initModules();
    void _deinitModules();
    virtual void _setupScene();

};

void CRenderer::_initModules() {
    CShaderContainer::GetInstance().Init();
    CGeoContainer::GetInstance().Init();
    SArcball::InitArcball();
}

void CRenderer::_deinitModules() {
    SArcball::DeinitArcball();
    CGeoContainer::GetInstance().Deinit();
    CShaderContainer::GetInstance().Deinit();

}

void CRenderer::_setupScene() {
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


    CObj obj_sphere( GEO_UNIT_SPHERE );
    // obj_sphere._material = blinnMat;
    obj_sphere.SetupModelMatrix( translate_center, rot_noRot, scale_xs );
    // obj_sphere._shaderType = SD_SINGLE_COLOR;
    _scene.AddObj( obj_sphere );


}


void CRenderer::_startup() {
    _initModules();
    _setupScene();

    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );

    glCullFace( GL_BACK );
    glFrontFace( GL_CCW );


}

void CRenderer::_update( double t_deltaTime ) {

}

void CRenderer::_render() {
    glClearColor( 0.5f, 0.5f, 0.5f, 1.f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    if( !_info._flags._fullScreen ) {
        glViewport( 0, 0, _info._winWidth, _info._winHeight );
    }


	// count area
	_scene.ApplyShaderToAllObjs( SD_AREA_COUNT );
    _scene.Draw();

	// paint area
	_scene.ApplyShaderToAllObjs( SD_AREA_PAINT );
	_scene.Draw();

}

void CRenderer::_shutdown() {
    _deinitModules();
}

void CRenderer::_onKeyPressed( int t_key, int t_action, int t_mods ) {
}

void CRenderer::_onMouseScroll( double t_offset ) {
   CView* view = View_GetActive();
   vec4 pos, face, up, right;
   view->GetCameraPositionFaceUpAndRight( pos, face, up, right );

   static float zoomSpeed = 0.1f;
   vec4 newPos = pos + (float)t_offset * zoomSpeed * face;
   view->SetCameraPostionFaceAndUp( newPos, face, up );

}


// app entry
int main( int argc, const char ** argv ) {
    CRenderer *app = new CRenderer();
    app->Run();                                  
    delete app;                                     
    return 0;                                      
}