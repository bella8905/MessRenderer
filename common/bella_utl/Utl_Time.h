/////////////////////////////////////////////////////////////////
//
//  Utilities - Time
// 
//  a reusable package including things we need for MessRenderer.
//
//  Copyright (c) 2015 Bella Q
//  
/////////////////////////////////////////////////////////////////
#pragma once

#include <time.h>
#include "Utl_LogMsg.h"

namespace Utl {

	struct STimer {
		clock_t _StartTime;

		void StartTimer( string t_Module ) {
			LogError << t_Module << LogEndl;
			_StartTime = clock();
		}

		void EndTimer() {
			int elapsedTime = ( clock() - _StartTime ) / CLOCKS_PER_SEC;
			int hours = elapsedTime / 3600;
			int minutes = elapsedTime % 3600 / 60;
			int seconds = elapsedTime % 3600 % 60;

			std::cout << "Time Elapsed: " << hours << "h, " << minutes << "m, " << seconds << "s." << std::endl;
		}
	};

}

