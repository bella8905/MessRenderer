/////////////////////////////////////////////////////////////////
//
//  Application Framework
//
// 
//
//  Copyright (c) 2015 Bella Q
//  
/////////////////////////////////////////////////////////////////

#include "Utl_Include.h"
#include "stb_image/stb_image.h"
#include "stb_image/stb_image_write.h"

#include "MessRendererApp.h"


namespace MessRenderer {

	const string g_imageFilePrefix = "images/screenshot_";

	CApp* CApp::_activeApp = 0;



	CApp::~CApp() {}

	void CApp::_setInfo() {
		_info._winWidth = 800;
		_info._winHeight = 600;

#ifdef __APPLE__
		_info._majorVersion = 3;
		_info._minorVersion = 2;
#else
		_info._majorVersion = 4;
		_info._minorVersion = 3;
#endif

		_info._FSAASamples = 4;

		// clear all flags
		_info._flags._all = 0;
		_info._flags._cursor = 1;

#ifdef _DEBUG
		_info._flags._debug = 1;
#endif
	}

	void CApp::_startup() {

	}

	void CApp::_update( double t_deltaTime ) {
	
	}

	void CApp::_updateControls( double t_deltaTime ) {
		if( Utl::GL_GetKeyOrMouseButtonPressed( GLFW_KEY_ESCAPE ) ) {
			glfwSetWindowShouldClose( _window, 1 );
		} 

		if( Utl::GL_GetKeyOrMouseButtonPressed( GLFW_KEY_F11 ) ) {
			_screenPrint();
		}
	}

	void CApp::_render() {

	}

	void CApp::_shutdown() {

	}

	// resize window
	void CApp::_onWindowResize( int t_w, int t_h ) {
		_info._winWidth = t_w;
		_info._winHeight = t_h;
	}

	void CApp::_glfw_onWindowResize( GLFWwindow* t_win, int t_w, int t_h ) {
		_activeApp->_onWindowResize( t_w, t_h );
	}



// 	void CApp::_glfw_onMouseButton( GLFWwindow* t_win, int t_button, int t_action, int t_mods ) {
// 		switch( t_action ) {
// 			case GLFW_PRESS:
// 			{
// 				_activeApp->_onMouseButtonPressed( t_button );
// 				_activeApp->_onMouseButtonRepeated( t_button );
// 			} break;
// 			case GLFW_RELEASE:
// 			{
// 				_activeApp->_onMouseButtonReleased( t_button );
// 			} break;
// 			case GLFW_REPEAT:
// 			{
// 				_activeApp->_onMouseButtonRepeated( t_button );
// 			} break;
// 			default: break;
// 		}
// 
// 	}

	// mouse move
	void CApp::_onMouseMove( double t_x, double t_y ) {

	}

	void CApp::_glfw_onMouseMove( GLFWwindow* t_win, double t_x, double t_y ) {
		_activeApp->_onMouseMove( t_x, t_y );
	}

	// mouse enter
	void CApp::_onMouseEnter( int t_bEntered ) {

	}

	void CApp::_glfw_onMouseEnter( GLFWwindow* t_win, int t_bEntered ) {
		_activeApp->_onMouseEnter( t_bEntered );
	}

	// mouse scroll
	void CApp::_onMouseScroll( double t_offset ) {

	}

	void CApp::_glfw_onMouseScroll( GLFWwindow* t_win, double t_xoffset, double t_yoffset ) {
		_activeApp->_onMouseScroll( t_yoffset );
	}

// 	// on key
// 	void CApp::_onKeyPressed( int t_key, int t_mods ) {
// //		LogMsg << "key pressed" << LogEndl;
// 
// 		if( t_key == GLFW_KEY_ESCAPE ) {
// 			glfwSetWindowShouldClose( _window, 1 );
// 		}
// 
// 		// screen print
// 		if( t_key == GLFW_KEY_F11 ) {
// 			_screenPrint();
// 		}
// 	}
// 



// 	void CApp::_glfw_onKey( GLFWwindow* t_win, int t_key, int t_scancode, int t_action, int t_mods ) {
// 		switch( t_action ) {
// 			case GLFW_PRESS:
// 			{
// 				_activeApp->_onKeyPressed( t_key, t_mods );
// 				_activeApp->_onKeyRepeated( t_key, t_mods );
// 			} break;
// 			case GLFW_RELEASE:
// 			{
// 				_activeApp->_onKeyReleased( t_key, t_mods );
// 			} break;
// 			case GLFW_REPEAT:
// 			{
// 				_activeApp->_onKeyRepeated( t_key, t_mods );
// 			} break;
// 			default: break;
// 		}
// 
// 	}

	// on drop
	void CApp::_onDrop( int t_count, const char** t_paths ) {

	}

	void CApp::_glfw_onDrop( GLFWwindow* t_win, int t_count, const char** t_paths ) {
		_activeApp->_onDrop( t_count, t_paths );
	}


	// error 
	void CApp::_onError( int t_error, const char* t_desc ) {
		LogError << "GLFW ERROR: code " << t_error << " msg: " << t_desc << LogEndl;
	}

    void CApp::_onDebugMessage( unsigned int t_source,
                                unsigned int t_type,
                                unsigned int t_id,
                                unsigned int t_severity,
                                int t_length,
                                const char* t_message,
                                void* t_userParam ) {
        char src_str[2048]; /* source */
        char type_str[2048]; /* type */
        char sev_str[2048]; /* severity */

        switch( t_source ) {
            case 0x8246:
                strcpy( src_str, "API" );
                break;
            case 0x8247:
                strcpy( src_str, "WINDOW_SYSTEM" );
                break;
            case 0x8248:
                strcpy( src_str, "SHADER_COMPILER" );
                break;
            case 0x8249:
                strcpy( src_str, "THIRD_PARTY" );
                break;
            case 0x824A:
                strcpy( src_str, "APPLICATION" );
                break;
            case 0x824B:
                strcpy( src_str, "OTHER" );
                break;
            default:
                strcpy( src_str, "undefined" );
                break;
        }

        switch( t_type ) {
            case 0x824C:
                strcpy( type_str, "ERROR" );
                break;
            case 0x824D:
                strcpy( type_str, "DEPRECATED_BEHAVIOR" );
                break;
            case 0x824E:
                strcpy( type_str, "UNDEFINED_BEHAVIOR" );
                break;
            case 0x824F:
                strcpy( type_str, "PORTABILITY" );
                break;
            case 0x8250:
                strcpy( type_str, "PERFORMANCE" );
                break;
            case 0x8251:
                strcpy( type_str, "OTHER" );
                break;
            case 0x8268:
                strcpy( type_str, "MARKER" );
                break;
            case 0x8269:
                strcpy( type_str, "PUSH_GROUP" );
                break;
            case 0x826A:
                strcpy( type_str, "POP_GROUP" );
                break;
            default:
                strcpy( type_str, "undefined" );
                break;
        }

        switch( t_severity ) {
            case 0x9146:
                strcpy( sev_str, "HIGH" );
                break;
            case 0x9147:
                strcpy( sev_str, "MEDIUM" );
                break;
            case 0x9148:
                strcpy( sev_str, "LOW" );
                break;
            case 0x826B:
                strcpy( sev_str, "NOTIFICATION" );
                break;
            default:
                strcpy( sev_str, "undefined" );
                break;
        }

        LogError << "source: " << src_str << "type: " << type_str << "id: " << t_id << "severity: " << sev_str << "length: " << t_length << "message: " << t_message << "user param: " << *(int*)t_userParam << LogEndl;

    }

	void CApp::_glfw_onError( int t_error, const char* t_desc ) {
		_activeApp->_onError( t_error, t_desc );
	}

    void CApp::_debugCallback( unsigned int t_source,
                               unsigned int t_type,
                               unsigned int t_id,
                               unsigned int t_severity,
                               int t_length,
                               const char* t_message,
                               void* t_userParam ) {
        _activeApp->_onDebugMessage( t_source, t_type, t_id, t_severity, t_length, t_message, t_userParam );
    }


	void CApp::_logGLParams() {
		enum ParamValType { SINGLE_INT, VEC_INT, SINGLE_BOOL, };

		struct glPrama {
			GLenum _param;
			string _paramStr;
			ParamValType _valType;
		} glParams[] = {
			// https://www.opengl.org/sdk/docs/man4/
				{ GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS", SINGLE_INT },
				{ GL_MAX_CUBE_MAP_TEXTURE_SIZE, "GL_MAX_CUBE_MAP_TEXTURE_SIZE", SINGLE_INT },
				// max count of buffer, that rendering in same time.
				// for more advanced effects where we want to split the output from our rendering into different images
				{ GL_MAX_DRAW_BUFFERS, "GL_MAX_DRAW_BUFFERS", SINGLE_INT },
				{ GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, "GL_MAX_FRAGMENT_UNIFORM_COMPONENTS", SINGLE_INT },
				{ GL_MAX_TEXTURE_IMAGE_UNITS, "GL_MAX_TEXTURE_IMAGE_UNITS", SINGLE_INT },
				{ GL_MAX_TEXTURE_SIZE, "GL_MAX_TEXTURE_SIZE", SINGLE_INT },
				// Varying floats are those sent from the vertex shader to the fragment shaders. 
				// NOT SURE WHY VARYING_FLOATS IS INVALID ENUM ON MY DESKTOP. TRY IT LATER SOMEWHERE ELSE
				// { GL_MAX_VARYING_FLOATS,                "GL_MAX_VARYING_FLOATS",                SINGLE_INT },
				{ GL_MAX_VERTEX_ATTRIBS, "GL_MAX_VERTEX_ATTRIBS", SINGLE_INT },
				{ GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, "GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS", SINGLE_INT },
				{ GL_MAX_VERTEX_UNIFORM_COMPONENTS, "GL_MAX_VERTEX_UNIFORM_COMPONENTS", SINGLE_INT },
				{ GL_MAX_VIEWPORT_DIMS, "GL_MAX_VIEWPORT_DIMS", VEC_INT },
				{ GL_LINE_WIDTH_RANGE, "GL_LINE_WIDTH_RANGE", VEC_INT },
				{ GL_STEREO, "GL_STEREO", SINGLE_BOOL },
		};

		us count = sizeof( glParams ) / sizeof( glPrama );
		LogMsg << "GL Context Params: " << LogEndl;
		for( us i = 0; i < count; ++i ) {
			LogMsg << glParams[i]._paramStr << ": ";
			switch( glParams[i]._valType ) {
			case SINGLE_INT:
			{
				int v = 0;
				glGetIntegerv( glParams[i]._param, &v );
				LogMsg << v;
			} break;
			case VEC_INT:
			{
				int v[2];
				glGetIntegerv( glParams[i]._param, v );
				LogMsg << v[0] << " " << v[1];
			} break;
			case SINGLE_BOOL:
			{
				unsigned char s = 0;
				glGetBooleanv( glParams[i]._param, &s );
				LogMsg << ( us )s;
			} break;
			default: break;
			}
			LogMsg << LogEndl;
		}
		LogMsg << LogEndl;
	}




	void CApp::_logSysInfo() {
		const GLubyte* renderer = glGetString( GL_RENDERER );
		const GLubyte* version = glGetString( GL_VERSION );
		LogMsg << "Renderer: " << renderer << LogEndl;
		LogMsg << "OpenGL version supported " << version << LogEndl << LogEndl;

		// log monitors info
		int count;
		GLFWmonitor** monitors = glfwGetMonitors( &count );
		LogMsg << "Monitors connected:" << LogEndl;
		for( int i = 0; i < count; ++i ) {
			GLFWmonitor* mon = monitors[i];
			LogMsg << i << ": " << glfwGetMonitorName( mon );
			const GLFWvidmode* vmode = glfwGetVideoMode( mon );
			LogMsg << ", width: " << vmode->width << ", height: " << vmode->height << "." << LogEndl;
		}
		LogMsg << LogEndl;

		// log context info
		_logGLParams();
	}

	bool CApp::_glfw_createWindow() {
		_setInfo();

		glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, _info._majorVersion );
		glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, _info._minorVersion );
		// disable all of the functionality from previous versions of OpenGL that has been marked as removal in the future
		glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
		// mark functions deprecation
		glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
		glfwWindowHint( GLFW_SAMPLES, _info._FSAASamples );
        if( _info._flags._debug ) {
            glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE );
            if( GLEW_KHR_debug ) {
                int param = -1;
                glDebugMessageCallback( ( GLDEBUGPROC )_debugCallback, &param );
                glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS );
            } else {
                LogWarning << "debug features not supported" << LogEndl;
            }
        }

		// create window
		if( _info._flags._fullScreen ) {
			GLFWmonitor* mon = glfwGetPrimaryMonitor();

			if( _info._winWidth == 0 || _info._winHeight == 0 ) {
				const GLFWvidmode* mode = glfwGetVideoMode( mon );
				glfwWindowHint( GLFW_RED_BITS, mode->redBits );
				glfwWindowHint( GLFW_GREEN_BITS, mode->greenBits );
				glfwWindowHint( GLFW_BLUE_BITS, mode->blueBits );
				glfwWindowHint( GLFW_REFRESH_RATE, mode->refreshRate );

				_info._winWidth = mode->width;
				_info._winHeight = mode->height;
			}
			
			_window = glfwCreateWindow( _info._winWidth, _info._winHeight, _info._title.c_str(), mon, 0 );
		} else {
			_window = glfwCreateWindow( _info._winWidth, _info._winHeight, _info._title.c_str(), 0, 0 );
		}


		if( !_window ) {
			LogError << "could not open GLFW window" << LogEndl;
			glfwTerminate();
			return false;
		}

		glfwMakeContextCurrent( _window );
		return true;
	}

	void CApp::_glfw_registerCallbacks() {
		glfwSetErrorCallback( CApp::_glfw_onError );
		glfwSetWindowSizeCallback( _window, CApp::_glfw_onWindowResize );
// 		glfwSetKeyCallback( _window, CApp::_glfw_onKey );
// 		glfwSetMouseButtonCallback( _window, CApp::_glfw_onMouseButton );
		glfwSetCursorPosCallback( _window, CApp::_glfw_onMouseMove );
		glfwSetDropCallback( _window, CApp::_glfw_onDrop );
		glfwSetCursorEnterCallback( _window, CApp::_glfw_onMouseEnter );
		glfwSetScrollCallback( _window, CApp::_glfw_onMouseScroll );
	}



	void CApp::_screenPrint() {
		string time = Utl::GetTime( Utl::TIME_STAMP_FILE_NAME );
		string imagefile = g_imageFilePrefix + time + ".png";

		ul size = _info._winWidth * _info._winHeight * 3;

		// dynamic memory is fine, cause we may change the viewport size
		// or better, we can pre allocate a large enough memory 
		// for video capture, we'd better do memory allocation first
		unsigned char* buffer = new unsigned char[size];
		glReadPixels( 0, 0, _info._winWidth, _info._winHeight, GL_RGB, GL_UNSIGNED_BYTE, buffer );
		unsigned char* lastRow = buffer + ( _info._winWidth * 3 * ( _info._winHeight - 1 ) );
		if( !stbi_write_png( imagefile.c_str(), _info._winWidth, _info._winHeight, 3, lastRow, -3 * _info._winWidth ) ) {
			LogError << "can't write to image: " << imagefile << LogEndl;
			// also the error msg
			LogError << strerror( errno ) << LogEndl;
		} else {
			LogPass << "image saved: " << imagefile << LogEndl;
		}

		delete[] buffer;

	}

	void CApp::Run() {

		if( _isActive() ) {
			LogError << "an application is running" << LogEndl;
			return;
		}

		if( _bRunning ) {
			LogError << "the application is already running" << LogEndl;
			return;
		}

		_bRunning = true;
		_activeApp = this;

		if( !glfwInit() ) {
			LogError << "could not start GLFW" << LogEndl;
			return;
		}

		if( !_glfw_createWindow() || !_window ) return;
		_glfw_registerCallbacks();


		// glew
		// GLEW obtains information on the supported extensions from the graphics driver. 
		// Experimental or pre-release drivers, however, might not report every available extension through the standard mechanism, 
		// in which case GLEW will report it unsupported. To circumvent this situation, 
		// the glewExperimental global switch can be turned on by setting it to GL_TRUE before calling glewInit(), 
		// which ensures that all extensions with valid entry points will be exposed.
		glewExperimental = GL_TRUE;
		glewInit();

		_logSysInfo();

		_startup();

		while( !glfwWindowShouldClose( _window ) ) {
			static double prevTime = glfwGetTime();
			double curTime = glfwGetTime();
			_deltaTime = curTime - prevTime;
			prevTime = curTime;

			_update( _deltaTime );
			_render();

			glfwSwapBuffers( _window );

			glfwPollEvents();
			Utl::GL_ReceiveAndUnpackEventData();

			_updateControls( _deltaTime );


		}

		glfwTerminate();

		_shutdown();

		_bRunning = false;

	}

}