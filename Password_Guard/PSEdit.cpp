#include "StdAfx.h"
#include "PSEdit.h"
#include "resource.h"
#include "Password_GuardDlg.h"

bool isThisMessage = true;

CPSEdit::CPSEdit(void)
{
}


CPSEdit::~CPSEdit(void)
{
}


LRESULT CPSEdit::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (message == WM_PASTE)
	{
		::OpenClipboard(this->m_hWnd);
		::EmptyClipboard();
		::CloseClipboard();
	}

// 	if (message == WM_GETTEXT )
// 	{
// 		return 0;
// 	}

	return CEdit::WindowProc(message, wParam, lParam);
}


LRESULT CPSEdit::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ( (message==WM_GETTEXT)|| (message==EM_GETLINE) )
	{
		if (isThisMessage == FALSE)
		{
			//AfxMessageBox(L"我的密码,可不能让你看哦!");
			return 0;
		}
		isThisMessage = FALSE;
	}
	return CEdit::DefWindowProc(message, wParam, lParam);
}
