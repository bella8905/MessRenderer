/////////////////////////////////////////////////////////////////
//
//  Application Framework
//
// 
//
//  Copyright (c) 2015 Bella Q
//  
/////////////////////////////////////////////////////////////////

#include "MessRendererApp.h"

namespace MessRenderer {

	CApp* CApp::_activeApp = 0;



	CApp::~CApp() {}

	void CApp::_setInfo() {

	}

	void CApp::_startup() {

	}

	void CApp::_update() {

	}

	void CApp::_render() {

	}

	void CApp::_shutdown() {

	}

	// resize window
	void CApp::_onWindowResize( int t_w, int t_h ) {

	}

	void CApp::_glfw_onWindowResize( GLFWwindow* t_win, int t_w, int t_h ) {

	}

	// mouse button
	void CApp::_onMouseButton( int t_button, int t_action ) {

	}

	void CApp::_glfw_onMouseButton( GLFWwindow* t_win, int t_button, int t_action, int t_mods ) {

	}

	// mouse move
	void CApp::_onMouseMove( double t_x, double t_y ) {

	}

	void CApp::_glfw_onMouseMove( GLFWwindow* t_win, double t_x, double t_y ) {

	}

	// mouse scroll
	void CApp::_onMouseScroll( double t_offset ) {

	}

	void CApp::_glfw_onMouseScroll( GLFWwindow* t_win, double t_xoffset, double t_yoffset ) {

	}

	// on key
	void CApp::_onKey( int t_key, int t_action, int t_mods ) {

	}

	void CApp::_glfw_onKey( GLFWwindow* t_win, int t_key, int t_scancode, int t_action, int t_mods ) {

	}



	void CApp::Run() {

		if( _activeApp != 0 ) {
			LogError << ""
		}
	}


}