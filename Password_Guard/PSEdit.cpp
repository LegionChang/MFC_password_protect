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

	return CEdit::WindowProc(message, wParam, lParam);
}


LRESULT CPSEdit::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ( (message==EM_GETLINE)|| (message==WM_GETTEXT) )
	{
		if (isThisMessage == FALSE)
		{
			return 0;
		}
		isThisMessage = FALSE;
	}
	return CEdit::DefWindowProc(message, wParam, lParam);
}
