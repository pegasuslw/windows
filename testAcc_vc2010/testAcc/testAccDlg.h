
// testAccDlg.h : ͷ�ļ�
//

#include <oleacc.h> // IAccessible ͷ�ļ�
#include <WINUSER.h>
#pragma once


class CtestAccDlg;
class ICallback;
class CheckConnectingCallback;
class CheckConnectedCallback;
class FoundAcceptCallback;


// CtestAccDlg �Ի���
class CtestAccDlg : public CDialogEx
{
// ����
public:
	CtestAccDlg(CWnd* pParent = NULL);	// ��׼���캯��
	CtestAccDlg::~CtestAccDlg();       // ��������

// �Ի�������
	enum { IDD = IDD_TESTACC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	BOOL FindAccessible(IAccessible* accParent,
		LPTSTR szName,
		LPTSTR szRole,
		LPTSTR szClass,
		IAccessible** paccChild,
		VARIANT* pvarChild);
		HWND mHwndChatWnd;
private:
	
	int  mState; // 1�ǶϿ��� 2���������ӣ� 3�����ӳɹ�
	TCHAR* mNames[2];   // ��Щ���ֵĶԻ��� ��������Զ�����
	TCHAR  mCurrentPath[MAX_PATH];
	void getCurrentPath();

	HWND findChatWindow();
	bool checkChatWindowAccessible();
	void checkState();
	DWORD WINAPI CtestAccDlg::drawViews();
	int findAcc(IAccessible *pAccParent,CString  text2find,ICallback *pCallback);
	void startConnect();
	FoundAcceptCallback *m_pFoundAcceptCallback;
	
public:
	int getState(){return mState;}
	void setState(int state){mState = state;}
#if 0
	inline void EnableMemLeakCheck()  
	{  
		_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);  
	}
#endif
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedOk2();
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedMainframe();
	afx_msg void OnBnClickedStop();
	afx_msg void OnDestroy();
	afx_msg void OnClose();
};

class ICallback{
public:
	virtual void doWork(CtestAccDlg* dlg, IAccessible *pAccParent, VARIANT varChild){}
};

class CheckConnectingCallback : public ICallback{
	public:
		virtual void doWork(CtestAccDlg* dlg, IAccessible *pAccParent, VARIANT varChild){
			dlg->setState(2); 
		}
};

class CheckConnectedCallback : public ICallback{
	public:
		virtual void doWork(CtestAccDlg* dlg, IAccessible *pAccParent, VARIANT varChild){
			dlg->setState(3); 
		}
};

class FoundAcceptCallback : public ICallback{
public:
	virtual void doWork(CtestAccDlg* dlg, IAccessible *pAccParent, VARIANT varChild){
		if(dlg->getState() == 2 || dlg->getState() == 3){   // ������������� ���Ѿ������ϣ� ��ô�Ͳ���ȥ����������
			return;
		}
		::ShowWindow(dlg->mHwndChatWnd,SW_SHOWMINIMIZED);  // ��С������
		::ShowWindow(dlg->mHwndChatWnd,SW_NORMAL);  // ����������
		long x1,y1,width,height;
		x1=0;
		y1=0;
		width=0;
		height=0;
		HRESULT result = pAccParent->accLocation(&x1,&y1,&width,&height,varChild);
		if(S_OK == result){
			CPoint pt; 
			GetCursorPos(&pt);//��ȡ�������Ļ�ĵ�ǰλ��
			SetCursorPos(x1+width/2,y1+height/2);//�ƶ���ĳ������
			mouse_event(MOUSEEVENTF_RIGHTDOWN,0,0,0,0);//�����Ҽ�
			mouse_event(MOUSEEVENTF_RIGHTUP,0,0,0,0);//�ɿ��Ҽ�
			mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);//�������
			mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);//�ɿ����

			SetCursorPos(pt.x,pt.y);//�ƶ���ĳ������
			//::ShowWindow(dlg->mHwndChatWnd,SW_SHOWMINIMIZED);  // ��С������
		}
			
	}
};