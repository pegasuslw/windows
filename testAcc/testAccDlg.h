// testAccDlg.h : header file
//

#if !defined(AFX_TESTACCDLG_H__7C692570_25CE_47B4_A8C2_E3CFD0120D73__INCLUDED_)
#define AFX_TESTACCDLG_H__7C692570_25CE_47B4_A8C2_E3CFD0120D73__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTestAccDlg dialog

class CTestAccDlg : public CDialog
{
// Construction
public:
	CTestAccDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTestAccDlg)
	enum { IDD = IDD_TESTACC_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestAccDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTestAccDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTACCDLG_H__7C692570_25CE_47B4_A8C2_E3CFD0120D73__INCLUDED_)
