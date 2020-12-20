
// Password_GuardDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CPassword_GuardDlg �Ի���




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


// CPassword_GuardDlg ��Ϣ�������

BOOL CPassword_GuardDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	SetPKBHook();


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CPassword_GuardDlg::OnPaint()
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
	// TODO: �ڴ����ר�ô����/����û���
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

//���빳�ӵİ�װ��ж�غ���
extern "C" bool __stdcall InstallHook(HWND hWnd);
extern "C" bool __stdcall UninstallHook();

void CPassword_GuardDlg::OnBnClickedBtnInstall()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	UninstallHook();
	InstallHook(0);
	UninstallHook();

	if(InstallHook(0))
		GetDlgItem(IDC_STATIC_PROTENUM)->SetWindowText(L"��ö�ٰ�װ�ɹ���");
	else GetDlgItem(IDC_STATIC_PROTENUM)->SetWindowText(L"��ö�ٰ�װ����");

}


void CPassword_GuardDlg::OnBnClickedBtnUninstall()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(UninstallHook())
		GetDlgItem(IDC_STATIC_PROTENUM)->SetWindowText(L"��ö��ж�سɹ���");
	else GetDlgItem(IDC_STATIC_PROTENUM)->SetWindowText(L"��ö��ж�ش���");
}
