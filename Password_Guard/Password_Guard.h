
// Password_Guard.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CPassword_GuardApp:
// �йش����ʵ�֣������ Password_Guard.cpp
//

class CPassword_GuardApp : public CWinApp
{
public:
	CPassword_GuardApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CPassword_GuardApp theApp;