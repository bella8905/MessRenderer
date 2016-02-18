/////////////////////////////////////////////////////////////////
//
//  Practice_11_19_14 - asset manager
//
//  reusable assets
//  textures, etc,...
// 
//  MessRenderer - A very messy renderer
//
//  Copyright (c) 2016 Bella Q
//  
/////////////////////////////////////////////////////////////////

#include "Utl_GLHelper.h"

#include "AssetManager.h"

GLuint CAssetManager::STEXTURE_DATA::GetId(  std::string t_imagePath  ) {
    std::unordered_map<std::string, GLuint>::const_iterator got = _textures.find( t_imagePath );
    if( got == _textures.end() ){
        // generate texture and return id
        GLuint textureId = 0;
        Utl::GL_LoadImage( textureId, t_imagePath.c_str(), GL_TEXTURE_2D );
        if( textureId > 0 ) {
            _textures[t_imagePath] = textureId; 
        }
        return textureId;
    } else {
        return got->second;
    }
}

void CAssetManager::STEXTURE_DATA::Remove(  GLuint t_textureId  ){

}

void CAssetManager::STEXTURE_DATA::Clear() {
    
}