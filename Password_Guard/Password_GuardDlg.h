
// Password_GuardDlg.h : ͷ�ļ�
//

#pragma once
#include "psedit.h"


// CPassword_GuardDlg �Ի���
class CPassword_GuardDlg : public CDialogEx
{
// ����
public:
	CPassword_GuardDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_PASSWORD_GUARD_DIALOG };

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
	CPSEdit m_PSEdit;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedBtnInstall();
	afx_msg void OnBnClickedBtnUninstall();
};
