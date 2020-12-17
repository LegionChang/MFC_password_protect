
// Password_GuardDlg.h : 头文件
//

#pragma once
#include "psedit.h"


// CPassword_GuardDlg 对话框
class CPassword_GuardDlg : public CDialogEx
{
// 构造
public:
	CPassword_GuardDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PASSWORD_GUARD_DIALOG };

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
	CPSEdit m_PSEdit;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedBtnInstall();
	afx_msg void OnBnClickedBtnUninstall();
};
