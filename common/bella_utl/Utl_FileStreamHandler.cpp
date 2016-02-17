/////////////////////////////////////////////////////////////////
//
//  Utilities - File Stream
// 
//  A reusable package including things we need for MessRenderer.
//
//  
//
//  Copyright (c) 2015 Bella Q
//  
/////////////////////////////////////////////////////////////////

#include <iterator>

#include "Utl_FileStreamHandler.h"
#include "Utl_LogMsg.h"

namespace Utl {

	void CFileStreamHandler::_init( const string& t_file ) {
		if( _bInited ) {
			// MsgErr( "already inited" );
			return;
		}

		_fileStream.open( t_file );
		if( !_fileStream || _fileStream.bad() ) {
			// MsgErr( "open file error" );
			LogError << "_FILE_STREAM_HANDLER: open file error. " << t_file << LogEndl;
			CloseFile();
			return;
		}

		_bInited = true;
	}

	void CFileStreamHandler::Deinit() {
		if( !_bInited ) {
			// MsgErr( "hasn't been inited" );
			return;
		}

		// Assert( _CachedStrs.empty(), "try to deinit when we still have unhandled strs" );
		_clearCachedStrs();
		CloseFile();
		_bInited = false;
	}

	void CFileStreamHandler::CloseFile() {
		if( _bInited ) {
			_fileStream.close();
			_fileStream.clear();
		}
	}

	void CFileStreamHandler::_clearCachedStrs() {
		queue<string>().swap( _cachedStrs );
	}


	bool CFileStreamHandler::_getNextStrFromCachedStrsWithCommentStripped( string& t_Str ) {
		string Str;
		if( !_cachedStrs.empty() ) {
			// fetch the next one from our cached queue
			Str = _cachedStrs.front();
			// if the string starts with "//", abandon this line
			string CommentPrefix( "//" );
			if( !Str.compare( 0, CommentPrefix.size(), CommentPrefix ) ) {
				_clearCachedStrs();
			} else {
				// return this str
				t_Str = Str;
				_cachedStrs.pop();
				return true;
			}
		}

		return false;
	}

	void CFileStreamHandler::_fillCachedStrs( const string& t_Str ) {
		_clearCachedStrs();
		std::stringstream strstr( t_Str );
		std::istream_iterator<std::string> it( strstr );
		std::istream_iterator<std::string> end;

		while( it != end ) {
			_cachedStrs.push( *it );
			++it;
		}
	}

	bool CFileStreamHandler::_getNextStr( string& t_Str ) {
		t_Str.clear();
		if( !_bInited ) {
			// MsgErr( "hasn't inited" );
			return false;
		}

		string str;
		if( !_getNextStrFromCachedStrsWithCommentStripped( str ) ) {
			// read and fill a line from stream
			if( !_safeReadLine( str ) ) {
				// we don't have anything new, clear everything and quit
				Deinit();
				return false;
			}

			_fillCachedStrs( str );
			if( !_getNextStrFromCachedStrsWithCommentStripped( str ) ) {
				Deinit();
				return false;
			}
		}

		t_Str = str;
		return true;
	}


	bool CFileStreamHandler::_safeReadLine( string& t_Str ) {
		// The characters in the stream are read one-by-one using a std::streambuf.
		// That is faster than reading them one-by-one using the std::istream.
		// Code that uses streambuf this way must be guarded by a sentry object.
		// The sentry object performs various tasks,
		// such as thread synchronization and updating the stream state.
		t_Str.clear();
		std::istream::sentry se( _fileStream, true );
		std::streambuf* sb = _fileStream.rdbuf();

		bool checkingStr = true;
		while( checkingStr ) {
			int c = sb->sbumpc();
			switch( c ) {
			case '\n':
				checkingStr = t_Str.empty();  //don't stop here if we got nothing
				break;
			case '\r':
				if( sb->sgetc() == '\n' ) sb->sbumpc();
				checkingStr = t_Str.empty();
				break;
			case EOF:
				// Also handle the case when the last line has no line ending
				if( t_Str.empty() ) _fileStream.setstate( std::ios::eofbit );
				checkingStr = false;
				break;
			default:
				t_Str += ( char )c;
			}
		}

		if( t_Str.empty() )   return false;
		return true;

	}

	CFileStreamHandler& CFileStreamHandler::operator >> ( string& t_Val ) {
		if( _bInited ) _getNextStr( t_Val );
		return *this;
	}


	CFileStreamHandler& CFileStreamHandler::operator >> ( int& t_Val ) {
		if( _bInited ) {
			string Str;
			( *this ) >> Str;
			t_Val = _strToInt( Str );
		}

		return *this;
	}


	CFileStreamHandler& CFileStreamHandler::operator >> ( bool& t_Val ) {
		if( _bInited ) {
			string Str;
			( *this ) >> Str;
			t_Val = ( _strToInt( Str ) != 0 );
		}

		return *this;
	}

	CFileStreamHandler& CFileStreamHandler::operator >> ( us& t_Val ) {
		if( _bInited ) {
			int IntVal;
			( *this ) >> IntVal;
			t_Val = ( us )IntVal;
		}

		return *this;
	}

	CFileStreamHandler& CFileStreamHandler::operator >> ( ul& t_Val ) {
		if( _bInited ) {
			int IntVal;
			( *this ) >> IntVal;
			t_Val = ( ul )IntVal;
		}

		return *this;
	}

	CFileStreamHandler& CFileStreamHandler::operator >> ( float& t_Val ) {
		if( _bInited ) {
			string Str;
			( *this ) >> Str;
			t_Val = _strToFloat( Str );
		}

		return *this;
	}


	int CFileStreamHandler::_strToInt( const string& t_Str ) {
		return atoi( t_Str.c_str() );
	}

	float CFileStreamHandler::_strToFloat( const string& t_Str ) {
		return ( float )atof( t_Str.c_str() );
	}

}