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
	// TODO: �ڴ����ר�ô����/����û���
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
	// TODO: �ڴ����ר�ô����/����û���
	if ( (message==WM_GETTEXT)|| (message==EM_GETLINE) )
	{
		if (isThisMessage == FALSE)
		{
			//AfxMessageBox(L"�ҵ�����,�ɲ������㿴Ŷ!");
			return 0;
		}
		isThisMessage = FALSE;
	}
	return CEdit::DefWindowProc(message, wParam, lParam);
}
