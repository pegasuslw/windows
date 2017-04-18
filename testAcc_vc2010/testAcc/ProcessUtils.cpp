
//ProcessUtils.cpp : 实现文件
//
#include "stdafx.h"
#include "ProcessUtils.h"
#include"tlhelp32.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

bool ProcessUtils::isRunning(const TCHAR *szName)
{
	BOOL bResult = FALSE;
	HANDLE hSnapshot = NULL;				
	PROCESSENTRY32 ProcessEntry;	
	DWORD dwPID = 0;			

	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);

	ProcessEntry.dwSize = sizeof(PROCESSENTRY32);
	bResult = Process32First(hSnapshot,&ProcessEntry);
	while(bResult)
	{
		//判断是否为要结束的进程
		if (0 == _tcscmp(szName,ProcessEntry.szExeFile) )
		{
			dwPID = ProcessEntry.th32ProcessID;
            break;
		}

		//获取下一个进程
		bResult = Process32Next(hSnapshot,&ProcessEntry);
	}

	HANDLE hProcess = NULL;
	hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,dwPID);

	//结束进程
	if(hProcess)
	{
		//bResult = TerminateProcess(hProcess,0);
		return true;
	}

	return false;
}

void  ProcessUtils::startSingleProcess(const TCHAR *szProcessPath,const TCHAR *szProcessName){
	if(ProcessUtils::isRunning(szProcessName)){
		return;  // 已经在运行了， 就不启动testAcc.exe了
	}
	
	TCHAR path[MAX_PATH];
	memset(path,0,MAX_PATH);
	lstrcat(path,szProcessPath);
	lstrcat(path,szProcessName);

	STARTUPINFO startupinfo;
	PROCESS_INFORMATION proc_info;
	memset(&startupinfo,0,sizeof(STARTUPINFO));
	startupinfo.cb=sizeof(STARTUPINFO);
	::CreateProcess(path,NULL,NULL,NULL,FALSE,NORMAL_PRIORITY_CLASS,NULL,NULL,&startupinfo,&proc_info); //启动testAcc.ext
}
