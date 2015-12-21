/////////////////////////////////////////////////////////////////
//
//  Utilities - Global
// 
//  A reusable package including things we need for MessRenderer.
//
//  Copyright (c) 2015 Bella Q
//  
/////////////////////////////////////////////////////////////////

#include "Utl_Global.h"

namespace Utl {

	string GetTime( EQURY_TIME_TYPE t_timeType ) {
		time_t rawTime = time( 0 );
		tm timeInfo;
		localtime_s( &timeInfo, &rawTime );
		char buffer[80];
		if( t_timeType == TIME_STAMP_FILE_NAME ) {
			strftime( buffer, 80, "%d_%m_%I_%M_%S", &timeInfo );
		} else {
			strftime( buffer, 80, "%d-%m-%Y %I:%M:%S", &timeInfo );
		}

		return string( buffer );
	}

}