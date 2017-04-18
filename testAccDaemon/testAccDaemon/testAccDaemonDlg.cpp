
// testAccDaemonDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "testAccDaemon.h"
#include "testAccDaemonDlg.h"
#include "afxdialogex.h"

#include "ProcessUtils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define TEST_ACC_EXE  (_T("testAcc.exe"))

// CtestAccDaemonDlg �Ի���

CtestAccDaemonDlg::CtestAccDaemonDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CtestAccDaemonDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CtestAccDaemonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CtestAccDaemonDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CtestAccDaemonDlg ��Ϣ�������

BOOL CtestAccDaemonDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	getCurrentPath();
	SetTimer(1,5000,NULL);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CtestAccDaemonDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CtestAccDaemonDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CtestAccDaemonDlg::getCurrentPath(){

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

void CtestAccDaemonDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	ProcessUtils::startSingleProcess(mCurrentPath, TEST_ACC_EXE);
	CDialogEx::OnTimer(nIDEvent);
}
