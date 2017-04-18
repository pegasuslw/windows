
#include "stdafx.h"
#include <WINUSER.h>

#pragma once

class ProcessUtils{

public:
	static bool isRunning(const TCHAR *szProcessName);
	static void startSingleProcess(const TCHAR *szProcessPath,const TCHAR *szProcessName);
};