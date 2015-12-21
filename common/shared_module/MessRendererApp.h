/////////////////////////////////////////////////////////////////
//
//  Application Framework
//
// 
//
//  Copyright (c) 2015 Bella Q
//  
/////////////////////////////////////////////////////////////////

#pragma once

namespace MessRenderer {

	class CApp {
	public:
		CApp() : _bRunning( false ) {}
		virtual ~CApp() = 0;

	public:
		struct APP_INFO {

		};

	protected:
		static CApp* _activeApp;
		APP_INFO _info;

		bool _bRunning;

	protected:
		/////////////////////////////////////////////////////////////////
		//
		//  application functions
		//  
		/////////////////////////////////////////////////////////////////
		virtual void _setInfo();
		virtual void _startup();
		virtual void _update();
		virtual void _render();
		virtual void _shutdown();

		/////////////////////////////////////////////////////////////////
		//
		//  callbacks
		//  
		/////////////////////////////////////////////////////////////////
		virtual void _onWindowResize( int t_w, int t_h );
		virtual void _onKey( int t_key, int t_action, int t_mods );
		virtual void _onMouseButton( int t_button, int t_action );
		virtual void _onMouseMove( double t_x, double t_y );
		virtual void _onMouseScroll( double t_offset );


		/////////////////////////////////////////////////////////////////
		//
		//  glfw callbacks
		//  Refer to GLFW manual for parameters
		//  
		/////////////////////////////////////////////////////////////////
		//	GLFWwindowsizefun
		static void _glfw_onWindowResize( GLFWwindow* t_win, int t_w, int t_h );
		//  GLFWkeyfun
		static void _glfw_onKey( GLFWwindow* t_win, int t_key, int t_scancode, int t_action, int t_mods );
		//  GLFWmousebuttonfun
		static void _glfw_onMouseButton( GLFWwindow* t_win, int t_button, int t_action, int t_mods );
		//  GLFWcursorposfun
		static void _glfw_onMouseMove( GLFWwindow* t_win, double t_x, double t_y );
		//  GLFWscrollfun
		static void _glfw_onMouseScroll( GLFWwindow* t_win, double t_xoffset, double t_yoffset );


	public:
		void Run();
	};


}
