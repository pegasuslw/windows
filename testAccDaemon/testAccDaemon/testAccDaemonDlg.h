
// testAccDaemonDlg.h : ͷ�ļ�
//

#pragma once


// CtestAccDaemonDlg �Ի���
class CtestAccDaemonDlg : public CDialogEx
{
// ����
public:
	CtestAccDaemonDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TESTACCDAEMON_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
