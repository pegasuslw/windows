// testAccDlg.cpp : implementation file
//

#include "stdafx.h"
#include "testAcc.h"
#include "testAccDlg.h"

#include <oleacc.h> // IAccessible 接口相关
#include <WINUSER.h>
#include <winable.h>
#include <windows.h>
#include <mshtml.h>   
#include <comdef.h>

#include <fstream.h>
#include <stdlib.h>
#include <Tlhelp32.h>
#include "Shlwapi.h"
#include <Vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma comment(lib,"shlwapi.lib")

/////////////////////////////////////////////////////////////////////////////
// CTestAccDlg dialog

CTestAccDlg::CTestAccDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestAccDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestAccDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestAccDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestAccDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTestAccDlg, CDialog)
	//{{AFX_MSG_MAP(CTestAccDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestAccDlg message handlers

BOOL CTestAccDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTestAccDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestAccDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

IAccessible *paccMainWindow = NULL;

void findAcc(IAccessible *pAccParent){
	HRESULT hr;
	

	IAccessible*    paccControl = NULL; //输入框的 IAccessible 接口
			
	LONG    childCount = 0;
	LONG    obtainCount = 0;
	if (S_OK!=pAccParent->get_accChildCount(&childCount)){
		//::MessageBox(NULL,"获取child count错误","错误",MB_OK);
		return;
	}
	if(0 == childCount){
		return;
	}
	VARIANT* pArray = new VARIANT[childCount];
	HRESULT result = AccessibleChildren(pAccParent,
											0,
											childCount,
											pArray, 
											&obtainCount);

	if(S_OK != result){
		return;
	}
	for (int x = 0; x < obtainCount; x++)
    {
        VARIANT vtChild = pArray[x];
        // If it's an accessible object, get the IAccessible, and recurse.
        if (vtChild.vt == VT_DISPATCH)
        {
            IDispatch* pDisp = vtChild.pdispVal;
            IAccessible* pChild = NULL;
            hr = pDisp->QueryInterface(IID_IAccessible, (void**) &pChild);
            if (hr == S_OK)
            {
				BSTR bstrName;
				VARIANT varChild;
				varChild.vt = VT_I4;
				varChild.lVal = CHILDID_SELF;
				pChild->get_accName(varChild,&bstrName);
				//LPWSTR wszName = malloc(1024*sizeof(WCHAR));
				//wnsprintfW(wszName, 1024, L"%s", bName);
				if(NULL!=bstrName /*&& bstrName==_T("取消")*/){
					//::MessageBox(NULL,"找到了","111",MB_OK);
					::MessageBox(NULL, _T(bstrName),"111",MB_OK);
				}
                
                findAcc(pChild);
                pChild->Release();
            }
            pDisp->Release();
        }
        // Else it's a child element so we have to call accNavigate on the parent,
        //   and we do not recurse because child elements can't have children.
        else
        {
			BSTR bstrName;
			VARIANT varChild;
			varChild.vt = VT_I4;
			varChild.lVal = vtChild.lVal;
            pAccParent->get_accName(varChild,&bstrName);
            if(NULL!=bstrName && 0==strcmp((const char*)bstrName,_T("取消"))){
					::MessageBox(NULL,"找到了","222",MB_OK);
			}
            
        }
    }
    delete[] pArray;
    return ;
	//paccMainWindow->Release();
}


void CTestAccDlg::OnOK() 
{
	HWND hChatWnd = ::FindWindow(NULL,"我有一只小小鸟");
	if(!hChatWnd){
		::MessageBox(NULL,"无法找到","无法找到聊天窗口！",MB_OK);
		return;
	}

	HRESULT hr;
	hr = AccessibleObjectFromWindow(hChatWnd,OBJID_WINDOW, IID_IAccessible,(void**)&paccMainWindow);
	if(S_OK != hr){
		::MessageBox(NULL,"没有找到聊天窗口的IAccessible接口！","错误",MB_OK);
		return;
	}
	
	findAcc(paccMainWindow);
	CDialog::OnOK();

}
