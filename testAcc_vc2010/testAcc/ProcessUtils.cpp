
//ProcessUtils.cpp : ʵ���ļ�
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
		//�ж��Ƿ�ΪҪ�����Ľ���
		if (0 == _tcscmp(szName,ProcessEntry.szExeFile) )
		{
			dwPID = ProcessEntry.th32ProcessID;
            break;
		}

		//��ȡ��һ������
		bResult = Process32Next(hSnapshot,&ProcessEntry);
	}

	HANDLE hProcess = NULL;
	hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,dwPID);

	//��������
	if(hProcess)
	{
		//bResult = TerminateProcess(hProcess,0);
		return true;
	}

	return false;
}

void  ProcessUtils::startSingleProcess(const TCHAR *szProcessPath,const TCHAR *szProcessName){
	if(ProcessUtils::isRunning(szProcessName)){
		return;  // �Ѿ��������ˣ� �Ͳ�����testAcc.exe��
	}
	
	TCHAR path[MAX_PATH];
	memset(path,0,MAX_PATH);
	lstrcat(path,szProcessPath);
	lstrcat(path,szProcessName);

	STARTUPINFO startupinfo;
	PROCESS_INFORMATION proc_info;
	memset(&startupinfo,0,sizeof(STARTUPINFO));
	startupinfo.cb=sizeof(STARTUPINFO);
	::CreateProcess(path,NULL,NULL,NULL,FALSE,NORMAL_PRIORITY_CLASS,NULL,NULL,&startupinfo,&proc_info); //����testAcc.ext
}
