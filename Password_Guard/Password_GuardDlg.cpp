
// Password_GuardDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Password_Guard.h"
#include "Password_GuardDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HHOOK m_keyBHHook;
HHOOK m_keyBLHook;
void SetPKBHook();

extern bool isThisMessage;

LRESULT  CALLBACK ProtectKBProc(int nCode,WPARAM wParam,LPARAM lParam)
{
	
	return 0;
}


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

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


// CPassword_GuardDlg 对话框




CPassword_GuardDlg::CPassword_GuardDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPassword_GuardDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPassword_GuardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDT_PS, m_PSEdit);
	if (pDX->m_bSaveAndValidate)
	{
		isThisMessage = TRUE;
	}

}

BEGIN_MESSAGE_MAP(CPassword_GuardDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_INSTALL, &CPassword_GuardDlg::OnBnClickedBtnInstall)
	ON_BN_CLICKED(IDC_BTN_UNINSTALL, &CPassword_GuardDlg::OnBnClickedBtnUninstall)
END_MESSAGE_MAP()


// CPassword_GuardDlg 消息处理程序

BOOL CPassword_GuardDlg::OnInitDialog()
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
	SetPKBHook();


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPassword_GuardDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPassword_GuardDlg::OnPaint()
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
HCURSOR CPassword_GuardDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void SetPKBHook()
{
	m_keyBLHook = SetWindowsHookEx(WH_KEYBOARD_LL, ProtectKBProc, NULL, 0);
	m_keyBHHook = SetWindowsHookEx(WH_KEYBOARD, ProtectKBProc, AfxGetApp()->m_hInstance, GetCurrentThreadId());
}


BOOL CPassword_GuardDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN || pMsg->message == WM_KEYUP)
	{
		SetPKBHook();
	}
	if (GetFocus() == GetDlgItem(IDC_EDT_PS))
	{
		SetPKBHook();
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

//导入钩子的安装与卸载函数
extern "C" bool __stdcall InstallHook(HWND hWnd);
extern "C" bool __stdcall UninstallHook();

void CPassword_GuardDlg::OnBnClickedBtnInstall()
{
	// TODO: 在此添加控件通知处理程序代码

	UninstallHook();
	InstallHook(0);
	UninstallHook();

	if(InstallHook(0))
		GetDlgItem(IDC_STATIC_PROTENUM)->SetWindowText(L"防枚举安装成功！");
	else GetDlgItem(IDC_STATIC_PROTENUM)->SetWindowText(L"防枚举安装错误！");

}


void CPassword_GuardDlg::OnBnClickedBtnUninstall()
{
	// TODO: 在此添加控件通知处理程序代码
	if(UninstallHook())
		GetDlgItem(IDC_STATIC_PROTENUM)->SetWindowText(L"防枚举卸载成功！");
	else GetDlgItem(IDC_STATIC_PROTENUM)->SetWindowText(L"防枚举卸载错误！");
}
