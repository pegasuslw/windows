
// testAccDaemon.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CtestAccDaemonApp:
// �йش����ʵ�֣������ testAccDaemon.cpp
//

class CtestAccDaemonApp : public CWinApp
{
public:
	CtestAccDaemonApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CtestAccDaemonApp theApp;