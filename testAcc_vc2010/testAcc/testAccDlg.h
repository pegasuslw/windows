
// testAccDlg.h : 头文件
//

#include <oleacc.h> // IAccessible 头文件
#include <WINUSER.h>
#pragma once


class CtestAccDlg;
class ICallback;
class CheckConnectingCallback;
class CheckConnectedCallback;
class FoundAcceptCallback;


// CtestAccDlg 对话框
class CtestAccDlg : public CDialogEx
{
// 构造
public:
	CtestAccDlg(CWnd* pParent = NULL);	// 标准构造函数
	CtestAccDlg::~CtestAccDlg();       // 析构函数

// 对话框数据
	enum { IDD = IDD_TESTACC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
	
	int  mState; // 1是断开， 2是正在连接， 3是连接成功
	TCHAR* mNames[2];   // 这些名字的对话框 将会进行自动连接
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
		if(dlg->getState() == 2 || dlg->getState() == 3){   // 如果是正在连接 或已经连接上， 那么就不用去尝试连接了
			return;
		}
		::ShowWindow(dlg->mHwndChatWnd,SW_SHOWMINIMIZED);  // 最小化窗口
		::ShowWindow(dlg->mHwndChatWnd,SW_NORMAL);  // 正常化窗口
		long x1,y1,width,height;
		x1=0;
		y1=0;
		width=0;
		height=0;
		HRESULT result = pAccParent->accLocation(&x1,&y1,&width,&height,varChild);
		if(S_OK == result){
			CPoint pt; 
			GetCursorPos(&pt);//获取鼠标在屏幕的当前位置
			SetCursorPos(x1+width/2,y1+height/2);//移动到某点坐标
			mouse_event(MOUSEEVENTF_RIGHTDOWN,0,0,0,0);//点下右键
			mouse_event(MOUSEEVENTF_RIGHTUP,0,0,0,0);//松开右键
			mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);//点下左键
			mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);//松开左键

			SetCursorPos(pt.x,pt.y);//移动到某点坐标
			//::ShowWindow(dlg->mHwndChatWnd,SW_SHOWMINIMIZED);  // 最小化窗口
		}
			
	}
};