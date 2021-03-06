/////////////////////////////////////////////////////////////////
//
//  Utilities - Global
// 
//  A reusable package including things we need for MessRenderer.
//
//  Copyright (c) 2015 Bella Q
//  
/////////////////////////////////////////////////////////////////

#pragma once

#include <iostream>
#include <string>
#include <time.h>

using std::string;


#define Clamp( x, xmin, xmax )	x < xmin ? xmin : ( x > xmax ? xmax : x )

typedef unsigned short us;
typedef unsigned long ul;
typedef unsigned int uint;

namespace Utl {

	template<typename T>
	bool Equals( const T& t_x, const T& t_y ) { return std::abs( t_x - t_y ) < std::numeric_limits<T>::epsilon(); }

	template bool Equals( const float& t_x, const float& t_y );
	template bool Equals( const double& t_x, const double& t_y );

	enum EQURY_TIME_TYPE {
		TIME_STAMP,  // %d-%m-%Y %H:%M:%S
		TIME_STAMP_FILE_NAME,  // %d_%m_%H_%M_%S, use it if we want a attach time to file name
	};

	string GetTime( EQURY_TIME_TYPE t_time = TIME_STAMP );

}

