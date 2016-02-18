/////////////////////////////////////////////////////////////////
//
//  Utilities - File related funcs
// 
//  A reusable package including things we need for MessRenderer.
// 
//
//  Copyright (c) 2016 Bella Q
//  
/////////////////////////////////////////////////////////////////

#include "Utl_File.h"

namespace Utl {

	bool DoesFileExist( std::string t_filePath ) {
		if( FILE* file = fopen( t_filePath.c_str(), "r" ) ) {
			fclose( file );
			return true;
		} else {
			return false;
		}
	}

	void GetFilePath( std::string t_filespec, std::string& t_path ) {
		std::size_t found = t_filespec.find_last_of( "/\\" );
		t_path = t_filespec.substr( 0, found );
	}

	void GetFilePathWithSlash( std::string t_filespec, std::string& t_path ) {
		std::size_t found = t_filespec.find_last_of( "/\\" );
		t_path = t_filespec.substr( 0, found + 1 );
	}

	void GetFileName( std::string t_filespec, std::string& t_filename ) {
		std::size_t found = t_filespec.find_last_of( "/\\" );
		t_filename = t_filespec.substr( found + 1 );
	}

	void GetFileBaseName( std::string t_filespec, std::string& t_basename ) {
		std::size_t last = t_filespec.find_last_of( "." );
		std::size_t first = t_filespec.find_last_of( "/\\" );
		t_basename = t_filespec.substr( first + 1, last - first );
	}

	void GetFileExtension( std::string t_filespec, std::string& t_ext ) {
		std::size_t dot = t_filespec.find_last_of( "." );
		t_ext = t_filespec.substr( dot + 1 );
	}

}