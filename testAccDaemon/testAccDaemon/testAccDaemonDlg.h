
// testAccDaemonDlg.h : 头文件
//

#pragma once


// CtestAccDaemonDlg 对话框
class CtestAccDaemonDlg : public CDialogEx
{
// 构造
public:
	CtestAccDaemonDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TESTACCDAEMON_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);


private:
	TCHAR  mCurrentPath[MAX_PATH];
	void getCurrentPath();
};
