#pragma once
#include "afxwin.h"
class CPSEdit :
	public CEdit
{
public:
	CPSEdit(void);
	~CPSEdit(void);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};

