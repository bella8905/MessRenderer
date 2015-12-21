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

struct GLFWwindow;

namespace MessRenderer {

	class CApp {
	public:
		CApp( std::string t_title ) : _window( 0 ), _bRunning( false ) {
			_info._title = t_title;
		}
		virtual ~CApp() = 0;

	public:
		struct APP_INFO {
			std::string _title;
			int _winWidth;
			int _winHeight;
			int _majorVersion;
			int _minorVersion;
			int _FSAASamples;

			// bools
			union {
				struct {
					unsigned int _fullScreen : 1;
					unsigned int _vsync : 1;
					unsigned int _cursor : 1;
					unsigned int _stereo : 1;
					unsigned int _debug : 1;
				};

				unsigned int _all;
			}_flags;
		};

	protected:
		static CApp* _activeApp;
		APP_INFO _info;

		bool _bRunning;
		GLFWwindow* _window;

	protected:
		bool _isActive() { return ( _activeApp != 0 )/* && ( _activeApp == this )*/; }

		/////////////////////////////////////////////////////////////////
		//
		//  application functions
		//  
		/////////////////////////////////////////////////////////////////
		virtual void _setInfo();
		virtual void _startup();
		virtual void _update( double _deltaTime );
		virtual void _render();
		virtual void _shutdown();


		/////////////////////////////////////////////////////////////////
		//
		//  log system info
		//  
		/////////////////////////////////////////////////////////////////
		void _logGLParams();
		void _logSysInfo();
	


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
		virtual void _onError( int t_error, const char* t_desc );

		/////////////////////////////////////////////////////////////////
		//
		//  glfw
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
		//  GLFWerrorfun
		static void _glfw_onError( int t_error, const char* t_desc );

		void _glfw_registerCallbacks();
		bool _glfw_createWindow();

	public:
		void Run();
	};


}
