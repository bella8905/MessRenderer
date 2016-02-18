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

#pragma once

#include <unordered_map>
#include <string>

#include "Utl_Include.h"

class CAssetManager {
private:
    CAssetManager() {}
    CAssetManager( const CAssetManager& t_manager );

private:
    struct STEXTURE_DATA{
        /////////////////////////////////////////////////////////////////
        //
        //  sets of textures
        //  use texture path as key, and GL texture handler as value,
        //  in order to keep a single copy of texture data.
        //  
        /////////////////////////////////////////////////////////////////
        std::unordered_map<std::string, GLuint> _textures;

        GLuint GetId( std::string t_imagePath );
        void Remove( GLuint t_textureId );
        void Clear();
    }_textures;



public:
    static CAssetManager& GetInstance() {
        static CAssetManager instance;
        return instance;
    }


};


