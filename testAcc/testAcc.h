// testAcc.h : main header file for the TESTACC application
//

#if !defined(AFX_TESTACC_H__AD0C1010_CBD5_42BA_9F7A_287CCF64717D__INCLUDED_)
#define AFX_TESTACC_H__AD0C1010_CBD5_42BA_9F7A_287CCF64717D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTestAccApp:
// See testAcc.cpp for the implementation of this class
//

class CTestAccApp : public CWinApp
{
public:
	CTestAccApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestAccApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTestAccApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTACC_H__AD0C1010_CBD5_42BA_9F7A_287CCF64717D__INCLUDED_)
