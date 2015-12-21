/////////////////////////////////////////////////////////////////
//
//  Utilities - FileStream
// 
//  A reusable package including things we need for MessRenderer.
// 
//  Cross platform( hopefully ) file handler,
//  reading in variables one by one, and omitting any comments,
//  from a text file.
//
//
//  How does it work:
//
//  Everytime we try to read in val,
//
//  1.	if we have strings cached in queue,
//		get the first one and test if it is a comment line,
//		( starts with "//" or something we set ).
//
//  2.  else, cached queue is empty,
//		get the next line, and fill each str in the line to cache.
//		and try to get the first one from cache again.
//
//  3.  any step failed will cause deinit the stream,
//	    and >> will return 0 because stream is deinited.
// 
//
//  Copyright (c) 2015 Bella Q
//  
/////////////////////////////////////////////////////////////////

#pragma once

#include <fstream>
#include <sstream>
#include <queue>

#include "Utl_Global.h"

using std::ifstream;
using std::istringstream;
using std::queue;

namespace Utl {

	/////////////////////////////////////////////////////////////////
	//
	// read in files in a safer way
	// kindly strip any comments starting with "//"
	// and be nicer to different end flags
	//  
	/////////////////////////////////////////////////////////////////
	class SFileStreamHandler {

	public:
		SFileStreamHandler( const string& t_File ) : _bInited( false ) { _init( t_File ); }
		~SFileStreamHandler() {}

	private:
		bool _bInited;

		ifstream _fileStream;
		queue<string> _cachedStrs;

	public:
		/////////////////////////////////////////////////////////////////
		//
		//  read operators
		//  
		/////////////////////////////////////////////////////////////////
		SFileStreamHandler& operator >> ( int& t_Val );
		SFileStreamHandler& operator >> ( us& t_Val );
		SFileStreamHandler& operator >> ( ul& t_Val );
		SFileStreamHandler& operator >> ( string& t_Val );
		SFileStreamHandler& operator >> ( float& t_Val );
		SFileStreamHandler& operator >> ( bool& t_Val );
		/////////////////////////////////////////////////////////////////
		//
		//  void operator
		//  With the void operator, the extraction operation
		//  returns not 0 only when the stream is inited,
		//  and we still have something to read in.
		//  ( we will deinit the stream in process of extraction ).
		//
		//  This allows us to do something fancy like,
		//  while( ss >> node ) {...}
		//  
		/////////////////////////////////////////////////////////////////
		operator void*( ) { if( _bInited ) return this; else return 0; }



		bool IsInited() const { return _bInited; }
		void CloseFile();

		/////////////////////////////////////////////////////////////////
		//
		//  Deinit
		//  Clear cache and close file.
		//  
		/////////////////////////////////////////////////////////////////
		void Deinit();

	private:
		/////////////////////////////////////////////////////////////////
		//
		//  open a file
		//  
		/////////////////////////////////////////////////////////////////
		void _init( const string& t_file );

		void _clearCachedStrs();
		bool _getNextStrFromCachedStrsWithCommentStripped( string& t_Str );

		bool _getNextStr( string& t_Str );
		/////////////////////////////////////////////////////////////////
		//
		//  http://stackoverflow.com/a/6089413
		//  
		/////////////////////////////////////////////////////////////////
		bool _safeReadLine( string& t_Str );
		void _fillCachedStrs( const string& t_Str );
		int _strToInt( const string& t_Str );
		float _strToFloat( const string& t_Str );
	};

}