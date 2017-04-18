
// testAccDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "vld.h"
#include "testAcc.h"
#include "testAccDlg.h"
#include "afxdialogex.h"

#include "ProcessUtils.h"


#include <oleacc.h> // IAccessible 头文件
#include <WINUSER.h>
//#include <winable.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define ACCEPT_TEXT (_T("接受"))
#define CONNECTING_TEXT (_T("正在建立连接..."))
#define DISCONNECT_TEXT (_T("断开"))
#define REMOTE_CONTROL_TEXT (_T("正在控制您的计算机\r\n按下“Shift+ESC”键可以停止受控"))

#define TEST_ACC_DAEMON_EXE (_T("testAccDaemon.exe"))

#pragma comment(lib,"shlwapi.lib")
#pragma comment(lib,"Oleacc.lib ")
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

IAccessible *paccMainWindow = NULL;

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};


CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CtestAccDlg 对话框

CtestAccDlg::CtestAccDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CtestAccDlg::IDD, pParent)
{
	ProcessUtils::isRunning(_T("testacc"));
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	mNames[0] = _TEXT("王圆圆");
	mNames[1] = _TEXT("刘伟");

	m_pFoundAcceptCallback = new FoundAcceptCallback();

	::CoInitialize(NULL);

	HRESULT hr;
	hr = AccessibleObjectFromWindow(mHwndChatWnd,OBJID_WINDOW, IID_IAccessible,(void**)&paccMainWindow);
	if(S_OK != hr){
		//::MessageBox(NULL,_T("没有找到聊天窗口的IAccessible接口！"),_T("Error"),MB_OK);
		return;
	}
}

CtestAccDlg::~CtestAccDlg(){
	delete m_pFoundAcceptCallback;
}

void CtestAccDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CtestAccDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDSTART, &CtestAccDlg::OnBnClickedStart)
	ON_BN_CLICKED(ID_STOP, &CtestAccDlg::OnBnClickedStop)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CtestAccDlg 消息处理程序

BOOL CtestAccDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	CWnd *startBtn = GetDlgItem(ID_START);
	::SendMessage(startBtn->GetSafeHwnd(),WM_LBUTTONDOWN,0,0);  // 初始化的时候点击“启动”按钮
    ::SendMessage(startBtn->GetSafeHwnd(),WM_LBUTTONUP,0,0);
	getCurrentPath();
	//ProcessUtils::startSingleProcess(mCurrentPath, TEST_ACC_DAEMON_EXE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CtestAccDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CtestAccDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CtestAccDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void drawRect(int x1,int y1,int x2,int y2){
//CClientDC dc(this);//选定当前画图环境
HDC hDC = GetDC(NULL);  //可以直接操纵hDC
CDC dc;
dc.Attach(hDC);
CBrush br;
br.CreateStockObject(NULL_BRUSH);
dc.SelectObject(&br);
CPen pen(PS_SOLID,2,RGB(255,0,0));//做一支红色粗细为1的笔
dc.SelectObject(&pen);//将pen放到dc上

dc.Rectangle(x1,y1,x2,y2);//画一个矩形
}

void drawText(int x1,int y1,int x2,int y2,CString& text){
	HDC hDC = GetDC(NULL);  //可以直接操纵hDC
	CDC dc;
	dc.Attach(hDC);
	
	/*
	CBrush br;
	br.CreateStockObject(NULL_BRUSH);
	dc.SelectObject(&br);
	CPen pen(PS_SOLID,2,RGB(255,0,0));//做一支红色粗细为1的笔
	dc.SelectObject(&pen);//将pen放到dc上
	*/
	

	LPRECT lpRect = new RECT();     // 设置位置
	lpRect->left = 0;
	lpRect->top = 0;
	lpRect->right = 1920;
	lpRect->bottom = 1080;


	
	HFONT hf;                         // 设置字体大小
	LOGFONT lf;
	lf.lfHeight=50;
	lf.lfWidth=20;
	lf.lfEscapement=0;
	hf=CreateFontIndirect(&lf);
	SelectObject(dc,hf);
	

	dc.SetTextColor(RGB(255,0,0));    // 设置文字颜色为红色

	//dc.SetBkMode(TRANSPARENT);       // 设置字体透明
	//dc.DrawText(CString("未连接状态"),lpRect,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	//text = _TEXT("未连接状态");
	::TextOut(dc,100,200,text,text.GetLength());

	delete lpRect;
}

HWND CtestAccDlg::findChatWindow(){
	HWND hChatWnd = ::FindWindow(NULL,_T("王圆圆")); 
	if (!hChatWnd){
		hChatWnd = ::FindWindow(NULL,_T("刘伟"));
	}else{
		return hChatWnd;
	}

	if(!hChatWnd){
		return NULL;
	}else{
		return hChatWnd;
	}
}

bool CtestAccDlg::checkChatWindowAccessible(){
	LONG    childCount = 0;

	if (NULL != paccMainWindow ){
		if (S_OK == paccMainWindow->get_accChildCount(&childCount)){
			return true;
		}else{
			paccMainWindow = NULL;
		}
	}

	// 到这里的 paccMainwindow 一定为空
	if(S_OK != AccessibleObjectFromWindow(mHwndChatWnd,OBJID_WINDOW, IID_IAccessible,(void**)&paccMainWindow)){
		paccMainWindow = NULL;
		return false;
	}

	if (S_OK == paccMainWindow->get_accChildCount(&childCount)){
		return true;
	}else{
		paccMainWindow = NULL;
		return false;
	}

	return false;
}

void CtestAccDlg::checkState()
{
	mState = 1;
	CheckConnectedCallback p1;
	findAcc(paccMainWindow,CONNECTING_TEXT, &p1);
	if (2 == mState){   // 正在连接状态
		return;
	}

	CheckConnectedCallback p2;
	for(int i=0;i<sizeof(mNames)/sizeof(mNames[0]);i++){
		CString name = CString(mNames[i]) + CString(REMOTE_CONTROL_TEXT);
		findAcc(paccMainWindow,name, &p2);
	}
	findAcc(paccMainWindow,REMOTE_CONTROL_TEXT, &p2);
	if (3 == mState){   // 已经连接成功状态
		return;
	}

	return;
}


void CtestAccDlg::startConnect(){
	findAcc(paccMainWindow,ACCEPT_TEXT,m_pFoundAcceptCallback);
}

int CtestAccDlg::findAcc(IAccessible *pAccParent,CString  text2find, ICallback *pCallback){
	return 0;
}
#if 0
int CtestAccDlg::findAcc(IAccessible *pAccParent,CString  text2find, ICallback *pCallback){

	if(NULL == pAccParent){
		return -1;
	}

	HRESULT hr;
	IAccessible*    paccControl = NULL; //输入框的 IAccessible 接口
			
	LONG    childCount = 0;
	LONG    obtainCount = 0;
	if (S_OK!=pAccParent->get_accChildCount(&childCount)){
		//::MessageBox(NULL,"获取child count错误","错误",MB_OK);
		return -1;
	}
	if(0 == childCount){
		return -1;
	}
	VARIANT* pArray = new VARIANT[childCount];
	HRESULT result = AccessibleChildren(pAccParent,
											0,
											childCount,
											pArray, 
											&obtainCount);

	if(S_OK != result){
		delete[] pArray;
		return -1;
	}

	long x1,y1,width,height;
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
				VARIANT varChild;
				VariantInit(&varChild);
				varChild.vt = VT_I4;
				varChild.lVal = CHILDID_SELF;
#if 0
				
				if(draw){
					x1=0;
					y1=0;
					width=0;
					height=0;
					result = pChild->accLocation(&x1,&y1,&width,&height,varChild);
					drawRect(x1,y1,x1+width,y1+height);
				}else{
					BSTR bstrName;
					pChild->get_accName(varChild,&bstrName);
  					if(NULL!=bstrName){
						CString str1(bstrName);
						CString str2(ACCEPT_TEXT); 
						if(str1 == str2){
							//::MessageBox(NULL,_T("找到了"),_T("111"),MB_OK);      // 找到了
							return 0;
						}
					}
				}
				
#else if

					BSTR bstrName;
					pChild->get_accName(varChild,&bstrName);
  					if(NULL!=bstrName){
						CString str1(bstrName);
						CString str2(text2find); 
						if(str1 == str2){
							//::MessageBox(NULL,_T("找到了"),_T("111"),MB_OK);      // 找到了
							if(NULL != pCallback){
								pCallback->doWork(this,pAccParent, varChild);
							}
							pChild->Release();
							pDisp->Release();
							delete[] pArray;
							return 0;
						}
					}
#endif
				
#if 0
				if (0 == findAcc(pChild,text2find,pCallback)){
					pChild->Release();
					pDisp->Release();
					delete[] pArray;
					return 0;
				}
#endif 
				findAcc(pChild,text2find,pCallback);
                pChild->Release();
            }
            pDisp->Release();
        }
        // Else it's a child element so we have to call accNavigate on the parent,
        //   and we do not recurse because child elements can't have children.
        else
        {
			BSTR bstrName = NULL;
			VARIANT varChild;
			VariantInit(&varChild);
			varChild.vt = VT_I4;
			varChild.lVal = vtChild.lVal;
			
#if 0
			if(true){
				x1=0;
				y1=0;
				width=0;
				height=0;
				result = pAccParent->accLocation(&x1,&y1,&width,&height,varChild);
				drawRect(x1,y1,x1+width,y1+height);
			}else{
				pAccParent->get_accName(varChild,&bstrName);
				if(NULL!=bstrName ){
					CString str1(bstrName);
					CString str2(ACCEPT_TEXT); 
					if(str1 == str2){                   
					
						VARIANT varFocus;
						HRESULT  result = pAccParent->get_accFocus(&varFocus);
						if(S_OK == result){
							pAccParent->get_accName(varChild,&bstrName);
							if(NULL!=bstrName ){
								CString str1(bstrName);
								CString str2(ACCEPT_TEXT);
								if(str1 == str2){
									::MessageBox(NULL,_T("找到了"),_T("333"),MB_OK);
									HWND hChatWnd = ::FindWindow(NULL,_T("王圆圆"));
									if (!hChatWnd){
									hChatWnd = ::FindWindow(NULL,_T("刘伟"));
									}

									if(!hChatWnd){
										::MessageBox(NULL,_T("没有找到聊天对话框"),_T("Error"),MB_OK);
										delete[] pArray;
										return 0;
									}else{
										::SendMessage(hChatWnd,WM_KEYUP,VK_RETURN,0xc00f0001);
										::SendMessage(hChatWnd,WM_KEYDOWN,VK_RETURN,0x000f0001);
									}
									
								}
								
							}
						}
					}
				}
			
			}
#else if
				pAccParent->get_accName(varChild,&bstrName);
				if(NULL ==bstrName ){
					continue;
				}

				CString str1(bstrName);
				CString str2(text2find); 
				if(str1 != str2){                   
					continue;
				}

				//pAccParent->get_accState(

				/*
				VARIANT varFocus;
				HRESULT  result = pAccParent->get_accFocus(&varFocus);
				if(S_OK != result){
						continue;
				}
				*/

				//::MessageBox(NULL,_T("找到了"),_T("333"),MB_OK);
				HWND hChatWnd = ::FindWindow(NULL,_T("王圆圆"));
				if (!hChatWnd){
					hChatWnd = ::FindWindow(NULL,_T("刘伟"));
				}

				if(!hChatWnd){
						//::MessageBox(NULL,_T("没有找到聊天对话框"),_T("Error"),MB_OK);
					delete[] pArray;
					return -1;
				}else{

					x1=0;
					y1=0;
					width=0;
					height=0;
					result = pAccParent->accLocation(&x1,&y1,&width,&height,varChild);
					drawRect(x1,y1,x1+width,y1+height);
					

#if 1
					if(NULL != pCallback){
						pCallback->doWork(this,pAccParent, varChild);
					}
#endif
					//::SendMessage(hChatWnd,WM_KEYUP,VK_RETURN,0xc00f0001);
					//::SendMessage(hChatWnd,WM_KEYDOWN,VK_RETURN,0x000f0001);
			}
#endif
              
        }

		
    }
	
    delete[] pArray;
    return -1;
	//paccMainWindow->Release();
}
#endif

#if 0
static DWORD WINAPI ThreadB1(LPVOID lpParam){
	HWND hChatWindow = NULL;
	/*if (hChatWindow){
		//MessageBox("123","123");
		;
	}else{
		::MessageBox(NULL,"456","456",MB_OK);
		return 0;
	}
	*/
	while (true)
	{
		hChatWindow = ::FindWindow(NULL,_T("王圆圆"));
		if (!hChatWindow)
		{
			hChatWindow = ::FindWindow(NULL,_T("刘伟"));
		}
		
		if (hChatWindow == NULL)
		{
			Sleep(1000);
			continue;
		}

		::SendMessage(hChatWindow,WM_SETFOCUS,0,0);  // 使chat窗口获得焦点
		::SendMessage(hChatWindow,WM_ACTIVATE,0,0);  // 使chat窗口获得焦点
		Sleep(100);

		::SendMessage(hChatWindow,WM_KEYUP,VK_TAB,0xc00f0001);   // 发送tab键
		::SendMessage(hChatWindow,WM_KEYDOWN,VK_TAB,0x000f0001);
		findAcc(paccMainWindow,ACCEPT_TEXT);
		Sleep(200);
	}
	return 0;
}
#endif

//static DWORD WINAPI drawViews(LPVOID lpParam){
DWORD WINAPI CtestAccDlg::drawViews(){
	if(!paccMainWindow){
		return -1;
	}

	long x1,y1,width,height;
	VARIANT varChild;
	VariantInit(&varChild);
	varChild.vt = VT_I4;
	varChild.lVal = CHILDID_SELF;
	x1=0;
	y1=0;
	width=0;
	height=0;
	HRESULT result = paccMainWindow->accLocation(&x1,&y1,&width,&height,varChild);
	drawRect(x1,y1,x1+width,y1+height);
	

	//int ret = findAcc(paccMainWindow,ACCEPT_TEXT,NULL);
	//int ret = findAcc(paccMainWindow,CONNECTING_TEXT);
    //int ret = findAcc(paccMainWindow,DISCONNECT_TEXT);

	CString text;
	switch(mState){
	case 1:  
		text = _TEXT("未连接状态");
		break;
	case 2:
		text = _TEXT("正在连接状态");
		break;
	case 3:
		text = _TEXT("已连接状态");
		break;
	default:
		text = _TEXT("未连接状态");
		break;

	}

	//drawText(0,0,500,500,text);

	return 0;
}

#if 0
static DWORD WINAPI ThreadB2(LPVOID lpParam){
	while(true){
	
	findAcc(paccMainWindow,ACCEPT_TEXT);
		//Sleep(1000);
	}

	return 0;
	
}
#endif

void CtestAccDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	

	mHwndChatWnd = findChatWindow();
	if(NULL == mHwndChatWnd){
		return;
	}
	
	//::ShowWindow(mHwndChatWnd,SW_SHOWMINIMIZED);  // 最小化窗口
	//::ShowWindow(mHwndChatWnd,SW_NORMAL);  // 正常化窗口
	//HRESULT hr;
	//hr = AccessibleObjectFromWindow(mHwndChatWnd,OBJID_WINDOW, IID_IAccessible,(void**)&paccMainWindow);
	//if(S_OK != hr){
		//::MessageBox(NULL,_T("没有找到聊天窗口的IAccessible接口！"),_T("Error"),MB_OK);
	//	return;
	//}

	//DWORD   dw1;
    //CreateThread(NULL,0,ThreadB1,NULL,0,&dw1); 

	if(!checkChatWindowAccessible()){
		return;
	}

	checkState();   // 检查目前状态
	
	drawViews();     //  画出矩形 以及状态
	 
#if 0
	LPTSTR buf= new TCHAR[100];
	memset(buf,0,100);
	wsprintf(buf,_TEXT("当前状态：%d"),mState);
	::MessageBox(NULL,buf,_TEXT("info"),MB_OK);
#endif

	startConnect(); // 开始连接

	
	//ThreadB1(NULL);

	//EnableMemLeakCheck();
	CDialogEx::OnTimer(nIDEvent);
}

void CtestAccDlg::OnBnClickedStart()
{
	// TODO: 在此添加控件通知处理程序代码
	SetTimer(1,5000,NULL);

	CWnd *startBtn = GetDlgItem(ID_START);
	startBtn->EnableWindow(false);

	CWnd *stopBtn = GetDlgItem(ID_STOP);
	stopBtn->EnableWindow(true);
}



void CtestAccDlg::OnBnClickedStop()
{
	KillTimer(1);
	CWnd *startBtn = GetDlgItem(ID_START);
	startBtn->EnableWindow(true);

	CWnd *stopBtn = GetDlgItem(ID_STOP);
	stopBtn->EnableWindow(false);
}


void CtestAccDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	//::MessageBox(NULL,_T("Destroy"),_T("Error"),MB_OK);

	// TODO: 在此处添加消息处理程序代码
}


void CtestAccDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//::MessageBox(NULL,_T("Close"),_T("Error"),MB_OK);
	CDialogEx::OnClose();
}


void CtestAccDlg::getCurrentPath(){

	memset(mCurrentPath,0,MAX_PATH);
	GetModuleFileName(NULL,mCurrentPath,MAX_PATH);
	for(int i=lstrlen(mCurrentPath)-1;i>=0;i--){
		if(mCurrentPath[i] != '\\'){
			mCurrentPath[i] = '\0';
			continue;
		}
		break;
	}
}