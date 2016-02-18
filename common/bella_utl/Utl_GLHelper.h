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
#pragma  once 

#include "gl/glew.h" 

namespace Utl {
    /////////////////////////////////////////////////////////////////
    //
    // load an image into OpenGL texture, 
    // and return the texture unit.
    //
    /////////////////////////////////////////////////////////////////
    void GL_LoadImage( GLuint& t_texture, const char* t_imageFile, GLenum t_target = GL_TEXTURE_2D );

    /*
    /////////////////////////////////////////////////////////////////
    //
    // upload data into OpenGL buffer, 
    // and return buffer handler
    //
    /////////////////////////////////////////////////////////////////
    void GL_GenBuffer( GLuint& t_buffer, GLenum t_target, void* t_bufferData, unsigned int t_bufferLength, GLenum t_usage );
    */
}