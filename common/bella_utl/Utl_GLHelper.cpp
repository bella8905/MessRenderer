/////////////////////////////////////////////////////////////////
//
//  Utilities - OpenGL Helper Functions
// 
//  A reusable package including things we need for MessRenderer.
//
//
//  Copyright (c) 2015 Bella Q
//  
/////////////////////////////////////////////////////////////////

#include "stb_image/stb_image.h"
#include <unordered_map>
#include "MessRendererApp.h"

#include "Utl_Include.h"


namespace Utl {

	bool GL_LoadImage( GLuint& t_texture, const char* t_imageFileName, GLenum t_target ) {

        int width, height;
        int forceChannels = 4;
        unsigned char* imageData = stbi_load( t_imageFileName, &width, &height, 0, forceChannels );
        if( !imageData ) {
            LogError << "couldn't load " << t_imageFileName << LogEndl;
			return false;
        }

        // check for wonky texture dimensions
        bool bCheckPOT = true;
        if( bCheckPOT ) {
            if( !Utl::IsPOT( width ) || !Utl::IsPOT( height ) ) {
                LogWarning << "texture is not power-of-2 dimensions " << t_imageFileName << LogEndl;
            }
        }

        // flip image upside down
        // OpenGL expects the 0 on the Y-axis to be at the bottom of the texture, 
        // but images usually have Y-axis 0 at the top.
        int widthInBytes = width * 4;
        unsigned char* top = 0;
        unsigned char* bottom = 0;
        int halfHeight = height / 2;

        for( int row = 0; row < halfHeight; row++ ) {
            top = imageData + row * widthInBytes;
            bottom = imageData + ( height - row - 1 ) * widthInBytes;
            for( int col = 0; col < widthInBytes; col++ ) {
                unsigned char temp = *top;
                *top = *bottom;
                *bottom = temp;
                top++;
                bottom++;
            }
        }


        // The GL spec just specifies that the texture name is free for reuse, 
        // and that the texture object has neither contents nor dimensionality. 
        // It says nothing whatsoever about the memory used for the object. 
        // GL implementations are free to keep the memory hanging around for 
        // subsequent reuse if necessary, to free it some arbitrary time later, 
        // or whatever the driver writer decides is the best behavior.
        // Don't expect a glDeleteTextures call to behave like free () or delete in C/C++. 

        // if( t_texture > 0 ) {
        //     glDeleteTextures( 1, &t_texture );
        // }
        // glGenTextures( 1, &t_texture );
        
        // copy image data into opengl texture
        if( t_texture == 0 ) {
            glGenTextures( 1, &t_texture );
        }


        glBindTexture( t_target, t_texture );
        // glTexStorage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height );
        // glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, imageData );
        glTexImage2D( t_target, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData );
        // use sampler in texture directly
        // wrapping
        glTexParameteri( t_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri( t_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
        // filtering
        glTexParameteri( t_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri( t_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

        glBindTexture( t_target, 0 );

        // free memory
        stbi_image_free( imageData );

		return true;
    }

	struct S_KEY_BUTTON {
		struct S_KEY_BUTTON_DATA {
			bool _held;
			bool _pressed;
			bool _repeated;
			bool _releasedStatic;
			bool _released;

			float _initialRepeatedDelay;
			float _repeatedDelay;

			S_KEY_BUTTON_DATA() : _held( false ), _pressed( false ), _repeated( false ), _released( false ), _releasedStatic( true ),
				_initialRepeatedDelay( 0.1f ), _repeatedDelay( 0.033f ) {

			}
		};


		std::unordered_map<int, S_KEY_BUTTON_DATA> _keyOrMouseButtonMaps;

		S_KEY_BUTTON() {
			_populateData();
		}

		void _populateData() {
#define GLFW_KEY_ITEM( key ) \
			if( !DoesKeyOrMouseButtonExist( GLFW_##key ) ) _keyOrMouseButtonMaps[GLFW_##key] = S_KEY_BUTTON_DATA(); \
			else LogWarning<<"key id repeated? "<<#key<<LogEndl; 
#include "GLFWKeyButton.items"

#define GLFW_MOUSE_BUTTON_ITEM( button ) \
			if( !DoesKeyOrMouseButtonExist( GLFW_MOUSE_##button ) ) _keyOrMouseButtonMaps[GLFW_MOUSE_##button] = S_KEY_BUTTON_DATA(); \
			else LogWarning<<"button id repeated? "<<#button<<LogEndl; 
#include "GLFWMouseButton.items"
		}

		bool DoesKeyOrMouseButtonExist( int t_id ) {
			std::unordered_map<int, S_KEY_BUTTON_DATA>::const_iterator got = _keyOrMouseButtonMaps.find( t_id );
			return got != _keyOrMouseButtonMaps.end();
		}

		void ReceiveAndUnpackEventData() {
			// key button
			int key_id;
#define GLFW_KEY_ITEM( key ) \
			key_id = GLFW_##key; \
			if(  DoesKeyOrMouseButtonExist( key_id ) ) { \
				if( GLFW_PRESS == glfwGetKey( MessRenderer::CApp::GetAppWindow(), key_id ) ) { \
					_keyOrMouseButtonMaps[key_id]._pressed = !_keyOrMouseButtonMaps[key_id]._held; \
					_keyOrMouseButtonMaps[key_id]._held = true; \
					_keyOrMouseButtonMaps[key_id]._releasedStatic = false; \
				} else if( GLFW_RELEASE == glfwGetKey( MessRenderer::CApp::GetAppWindow(), key_id ) ) { \
					_keyOrMouseButtonMaps[key_id]._held = false; \
					_keyOrMouseButtonMaps[key_id]._pressed = false; \
					_keyOrMouseButtonMaps[key_id]._repeated = false; \
					_keyOrMouseButtonMaps[key_id]._released = !_keyOrMouseButtonMaps[key_id]._releasedStatic; \
					_keyOrMouseButtonMaps[key_id]._releasedStatic = true; \
				} \
			} else { \
				LogWarning << "button id not exist? " << #key << LogEndl; \
			}
#include "GLFWKeyButton.items"

			// mouse 
			int button_id;
#define GLFW_MOUSE_BUTTON_ITEM( button ) \
			button_id = GLFW_MOUSE_##button; \
			if( DoesKeyOrMouseButtonExist( button_id ) ) { \
				if( GLFW_PRESS == glfwGetMouseButton( MessRenderer::CApp::GetAppWindow(), button_id ) ) { \
					_keyOrMouseButtonMaps[button_id]._pressed = _keyOrMouseButtonMaps[button_id]._held ? false : true; \
					_keyOrMouseButtonMaps[button_id]._held = true; \
					_keyOrMouseButtonMaps[button_id]._releasedStatic = false; \
				} else if( GLFW_RELEASE == glfwGetMouseButton( MessRenderer::CApp::GetAppWindow(), button_id ) ) { \
					_keyOrMouseButtonMaps[button_id]._held = false; \
					_keyOrMouseButtonMaps[button_id]._pressed = false; \
					_keyOrMouseButtonMaps[button_id]._repeated = false; \
					_keyOrMouseButtonMaps[button_id]._released = !_keyOrMouseButtonMaps[button_id]._releasedStatic; \
					_keyOrMouseButtonMaps[button_id]._releasedStatic = true; \
				} \
			} else { \
				LogWarning << "button id not exist? " << #button << LogEndl; \
			}
#include "GLFWMouseButton.items"
		}

	} KeyOrMouseButtonData;

	void GL_ReceiveAndUnpackEventData() {
		KeyOrMouseButtonData.ReceiveAndUnpackEventData();
	}

	bool GL_GetKeyOrMouseButtonPressed( int t_key ) {
		if( !KeyOrMouseButtonData.DoesKeyOrMouseButtonExist( t_key ) ) {
			LogError << "key is invalid " << LogEndl;
			return false;
		} else {
			return KeyOrMouseButtonData._keyOrMouseButtonMaps[t_key]._pressed;
		}
	}

	bool GL_GetKeyOrMouseButtonHeld( int t_key ) {
		if( !KeyOrMouseButtonData.DoesKeyOrMouseButtonExist( t_key ) ) {
			LogError << "key is invalid " << LogEndl;
			return false;
		} else {
			return KeyOrMouseButtonData._keyOrMouseButtonMaps[t_key]._held;
		}
	}

	bool GL_GetKeyOrMouseButtonRepeated( int t_key ) {
		if( !KeyOrMouseButtonData.DoesKeyOrMouseButtonExist( t_key ) ) {
			LogError << "key is invalid " << LogEndl;
			return false;
		} else {
			return KeyOrMouseButtonData._keyOrMouseButtonMaps[t_key]._repeated;
		}
	}

	bool GL_GetKeyOrMouseButtonReleased( int t_key ) {
		if( !KeyOrMouseButtonData.DoesKeyOrMouseButtonExist( t_key ) ) {
			LogError << "key is invalid " << LogEndl;
			return false;
		} else {
			return KeyOrMouseButtonData._keyOrMouseButtonMaps[t_key]._released;
		}
	}

}