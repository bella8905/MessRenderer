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

#include "Utl_LogMsg.h"
#include "Utl_Math.h"
#include "Utl_GLHelper.h"


namespace Utl {

    void GL_LoadImage( GLuint& t_texture, const char* t_imageFileName, GLenum t_target ) {

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
    }
}