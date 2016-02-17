/////////////////////////////////////////////////////////////////
//
//  texture - main
//
//  basic color map
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
#include "stb_image/stb_image.h"

#include "Utl_Include.h"

#include "MessRendererApp.h"

/////////////////////////////////////////////////////////////////
//
//  texture viewer shader 
//
/////////////////////////////////////////////////////////////////
class CTextureViewerSD : public CShader {
public:
    CTextureViewerSD() : _texture( 0 ) {
        const std::string VS_FILE = "shaders/texture_viewer.vert";
        const std::string FS_FILE = "shaders/texture_viewer.frag";

        initSP( VS_FILE, FS_FILE );
		_image._scale = 1.f;
    }

    virtual ~CTextureViewerSD() {}

public:
    void onBindShader() {
        glBindTexture( GL_TEXTURE_2D, _texture );

        ///////////////////////////////////////////////////
        // use window size for now
        ///////////////////////////////////////////////////
        MessRenderer::CApp* activeApp = MessRenderer::CApp::GetActiveApp();
        if( activeApp) {
            float winWidth, winHeight;
            activeApp->GetWindowSize( winWidth, winHeight );

            glBindBuffer( GL_UNIFORM_BUFFER, _viewport_ubo );
            float* winSize = ( float* )glMapBufferRange( GL_UNIFORM_BUFFER, 0, 2 * sizeof( float ), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_UNSYNCHRONIZED_BIT );
            winSize[0] = winWidth;
            winSize[1] = winHeight;
            glUnmapBuffer( GL_UNIFORM_BUFFER );
            glBindBuffer( GL_UNIFORM_BUFFER, 0 );

            glBindBufferBase( GL_UNIFORM_BUFFER, 0, _viewport_ubo );
        }

		glBindBuffer( GL_UNIFORM_BUFFER, _image_ubo );
		IMAGE_STRUCT* image = ( IMAGE_STRUCT* )glMapBufferRange( GL_UNIFORM_BUFFER, 0, sizeof( IMAGE_STRUCT ), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_UNSYNCHRONIZED_BIT );
		*image = _image;
		glUnmapBuffer( GL_UNIFORM_BUFFER );
		glBindBuffer( GL_UNIFORM_BUFFER, 0 );

		glBindBufferBase( GL_UNIFORM_BUFFER, 1, _image_ubo );

    }

	void LoadImage( const char* t_imageFileName ) {
		int width, height;
		int forceChannels = 4;
		unsigned char* imageData = stbi_load( t_imageFileName, &width, &height, 0, forceChannels );
		if( !imageData ) {
			LogError << "couldn't load " << t_imageFileName << LogEndl;
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

		// copy image data into opengl texture
		if( _texture > 0 ) {
			glDeleteTextures( 1, &_texture );
		}

		glGenTextures( 1, &_texture );
		// bind texture to active slot
		glActiveTexture( GL_TEXTURE1 );
		glBindTexture( GL_TEXTURE_2D, _texture );
		// glTexStorage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height );
		// glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, imageData );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData );
		// use sampler in texture directly
		// wrapping
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );
		// filtering
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		// border color
		GLfloat color[4] = { 0.5f, 0.5f, 0.5f, 1.f };
		glTexParameterfv( GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color );

		glBindTexture( GL_TEXTURE_2D, 0 );

		// free memory
		// delete imageData;

	}

	void ScaleImage( float t_delta ) {
		_image._scale += t_delta;
	}

protected:
    /////////////////////////////////////////////////////////////////
    //
    // texture unit
    //
    /////////////////////////////////////////////////////////////////
    GLuint _texture;
    /////////////////////////////////////////////////////////////////
    //
    // ubo which stores viewport related attributes
    //
    /////////////////////////////////////////////////////////////////
    GLuint _viewport_ubo;

	/////////////////////////////////////////////////////////////////
	//
	// ubo which stores image related attributes
	//
	/////////////////////////////////////////////////////////////////
	GLuint _image_ubo;

	struct IMAGE_STRUCT {
		float _scale;
	}_image;

protected:
 

    virtual void onInit(){
        glGenBuffers( 1, &_viewport_ubo );
        glBindBuffer( GL_UNIFORM_BUFFER, _viewport_ubo );
        glBufferData( GL_UNIFORM_BUFFER, sizeof( GLfloat ) * 2, NULL, GL_DYNAMIC_DRAW );
        glBindBuffer( GL_UNIFORM_BUFFER, 0 );

		glGenBuffers( 1, &_image_ubo );
		glBindBuffer( GL_UNIFORM_BUFFER, _image_ubo );
		glBufferData( GL_UNIFORM_BUFFER, sizeof( IMAGE_STRUCT ), NULL, GL_DYNAMIC_DRAW );
		glBindBuffer( GL_UNIFORM_BUFFER, 0 );

    }

    virtual void onDeinit() {
		if( _texture > 0 ) {
			glDeleteTextures( 1, &_texture );
		}

        // viewport ubo
        glDeleteBuffers( 1, &_viewport_ubo );
		// image ubo
		glDeleteBuffers( 1, &_image_ubo );
    }
};



/////////////////////////////////////////////////////////////////
//
//  renderer app
//
/////////////////////////////////////////////////////////////////
class CRenderer : public MessRenderer::CApp {
public:
    CRenderer() : MessRenderer::CApp( "Texture Viewer" ), _textureViewerSD( 0 ) {}
    ~CRenderer() {}

protected:
    /////////////////////////////////////////////////////////////////
    //
    // a dummy vao which actually takes no vertex data,
    // but fires rendering command for 4 points
    //
    /////////////////////////////////////////////////////////////////
    GLuint _vao;

    CTextureViewerSD* _textureViewerSD;

protected:
    virtual void _startup();
    virtual void _render();
    virtual void _shutdown();
	virtual void _onKey( int t_key, int t_action, int t_mods );
	virtual void _onMouseScroll( double t_offset );
};


void CRenderer::_startup() {
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
	// blending
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    glCullFace( GL_BACK );
    glFrontFace( GL_CCW );

    _textureViewerSD = new CTextureViewerSD();
	const char* defaultImage = "../../common/textures/skulluvmap.png";
	_textureViewerSD->LoadImage( defaultImage );

    glGenVertexArrays(1, &_vao);
}

void CRenderer::_render() {
    glClearColor( 0.5f, 0.5f, 0.5f, 1.f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    if( !_info._flags._fullScreen ) {
        glViewport( 0, 0, _info._winWidth, _info._winHeight );
    }


    glBindVertexArray( _vao );
    _textureViewerSD->BindShader();
    glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
    glBindVertexArray( 0 );
}

void CRenderer::_shutdown() {
    glDeleteVertexArrays( 1, &_vao );
    if( _textureViewerSD ) {
        delete _textureViewerSD;
    }
}

void CRenderer::_onKey( int t_key, int t_action, int t_mods ) {
	if( t_key == GLFW_KEY_SPACE && t_action == GLFW_PRESS ) {
		Utl::CFileBrowserDialog fileBrowser;
		// filtering image files
		fileBrowser._filter = "png files(*.png)\0*.png\0jpg files(*.jpg)\0*.jpg\0\0";
		if( fileBrowser.ShowDialog() ) {
			_textureViewerSD->LoadImage( fileBrowser._fileName );
		}

	}
}

void CRenderer::_onMouseScroll( double t_offset ) {
	if( _textureViewerSD ) {
		static float speed = 0.1f;
		_textureViewerSD->ScaleImage( ( float )t_offset * speed );
	} 
}



// app entry
int main( int argc, const char ** argv ) {

    CRenderer *app = new CRenderer();
    app->Run();                                  
    delete app;                                     
    return 0;                                      
}