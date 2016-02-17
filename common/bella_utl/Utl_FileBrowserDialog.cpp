/////////////////////////////////////////////////////////////////
//
//  Utilities - File Browser
// 
//  A reusable package including things we need for MessRenderer.
// 
//  Cross platform( hopefully ) file browser,
//  open a file browser dialog and pass back the file name selected.
//
// 
//
//  Copyright (c) 2016 Bella Q
//  
/////////////////////////////////////////////////////////////////

#include <tchar.h>

#include "Utl_FileBrowserDialog.h"

namespace Utl {

	CFileBrowserDialog::CFileBrowserDialog() {

		_defaultExtension = 0;
		_filter = 0;
		_filterIndex = 0;
		_flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
		_initialDir = _T( "..\\..\\common\\" );
		_owner = 0;
		_title = "Open File...";
	}


	bool CFileBrowserDialog::ShowDialog() {
		OPENFILENAME ofn;

		ZeroMemory( &ofn, sizeof( ofn ) );

		ofn.lStructSize = sizeof( ofn );
		ofn.hwndOwner = _owner;
		ofn.lpstrDefExt = _defaultExtension;
		ofn.lpstrFile = _fileName;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrFilter = _filter;
		ofn.nFilterIndex = _filterIndex;
		ofn.lpstrInitialDir = _initialDir;
		ofn.lpstrTitle = _title;
		ofn.Flags = _flags;

		GetOpenFileName( &ofn );

		if( _tcslen( _fileName ) == 0 ) return false;

		return true;
	}

}