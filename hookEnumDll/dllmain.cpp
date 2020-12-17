// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣
#include "stdafx.h"


HHOOK        g_hHook=NULL;
HINSTANCE g_hinstDll=NULL;
HMODULE hModule;

/********************** EnumWindows **************************/
LPVOID fpEnumWindows;
LPVOID fpNewEnumWindows;
BYTE        OldEnumWindowsCode[5], NewEnumWindowsCode[5];
BOOL WINAPI MyEnumWindows( WNDENUMPROC lpEnumFunc, LPARAM lParam);
/****************************************************************/

/********************** EnumChildWindows **************************/
LPVOID fpEnumChildWindows;
LPVOID fpNewEnumChildWindows;
BYTE        OldEnumChildWindowsCode[5], NewEnumChildWindowsCode[5];
BOOL WINAPI MyEnumChildWindows(HWND hWndParent,WNDENUMPROC lpEnumFunc,LPARAM lParam);
/****************************************************************/

/********************** GetWindow **************************/
LPVOID fpGetWindow;
LPVOID fpNewGetWindow;
BYTE        OldGetWindowCode[5], NewGetWindowCode[5];
HWND WINAPI MyGetWindow(HWND hWnd, UINT uCmd);
/****************************************************************/

/********************** FindWindowW **************************/
LPVOID fpFindWindowW;
LPVOID fpNewFindWindowW;
BYTE        OldFindWindowWCode[5], NewFindWindowWCode[5];
HWND WINAPI MyFindWindowW(LPCSTR lpClassName, LPCSTR lpWindowName);
/****************************************************************/

/********************** FindWindowExW **************************/
LPVOID fpFindWindowExW;
LPVOID fpNewFindWindowExW;
BYTE        OldFindWindowExWCode[5], NewFindWindowExWCode[5];
HWND WINAPI MyFindWindowExW(LPCSTR lpClassName, LPCSTR lpWindowName);
/****************************************************************/

BOOL  bHook = false;
void HookOn();
void HookOff();
bool Init();


//��깳�Ӻ������
//��깳�ӽ���̬���ӿ�ע�뵽��������û�б�����ã�����ֱ�ӷ���
LRESULT WINAPI HookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	return(CallNextHookEx(g_hHook, nCode, wParam, lParam));
}

//���Ӱ�װ����
extern "C" bool __declspec(dllexport) __stdcall InstallHook(HWND hWnd)
{
	if(!g_hHook){
		//HHOOK SetWindowsHookExA(
		//	int       idHook,			//���ӵ�����
		//	HOOKPROC  lpfn,				//���Ӻ����ĵ�ַ
		//	HINSTANCE hmod,				//�������Ӻ�����ģ����
		//	DWORD     dwThreadId		//ָ�����ӵ��̣߳����ָ��ȷ�����̣߳���Ϊ�̹߳��ӣ����ָ��Ϊ�գ���Ϊȫ�ֹ��ӡ�
		//	);
		//
		//WH_MOUSE:��װ���������Ϣ�Ĺ���
		g_hHook = SetWindowsHookEx(WH_MOUSE, (HOOKPROC)HookProc, g_hinstDll, 0);
	}
	if (!g_hHook)
	{
		MessageBoxA(0, "ʧ�ܣ�", "", MB_OK);
		MessageBoxW(0, L"ʧ�ܣ�", L"", MB_OK);
		return false;
	}
	HookOn();
	return true;
}

//����ж�غ���
extern "C" BOOL __declspec(dllexport) __stdcall UninstallHook()
{
	BOOL result=false;
	if(g_hHook) {
		if(bHook) HookOff();
		//UnhookWindowsHookEx�������ڽ�һ���ɺ���SetWindowsHookEx��װ�Ĺ��Ӻ����ӹ�������ɾ�������ѹ���
		result=UnhookWindowsHookEx(g_hHook);
	}
	g_hHook=NULL;
	return result;
}

bool Init()
{
	//��ȡģ��user32.dll�ľ��
	hModule=GetModuleHandle(L"user32.dll");
	DWORD   *addr;

	/**************************************  EnumWindows Init  *********************************************************/
	//��ȡ����EnumWindows�ĵ�ַ
	fpEnumWindows = GetProcAddress(hModule, "EnumWindows");
	if(fpEnumWindows == NULL){
		return false;
	}
	//�µĺ���ָ���ַ
	fpNewEnumWindows=(LPVOID)MyEnumWindows;
	NewEnumWindowsCode[0] = 0xe9;//������ת��ָ��jmp�Ļ���ָ�0xe9Ϊ�����ת��0xffΪ������ת��
	
	addr=(DWORD *)(NewEnumWindowsCode+1);
	//�μ����תָ���Ϊ5���ֽڣ����JMP��ָ����㹫ʽΪ��Ŀ���ַ-��ǰ��ַ-5
	*addr=DWORD(MyEnumWindows)-DWORD(fpEnumWindows)-5;
	//��ԭʼ�ĺ����ֳ����б���
	RtlMoveMemory(OldEnumWindowsCode, fpEnumWindows, 5);
	/*********************************************************************************************************************/

	/**************************************  EnumChildWindows Init  *********************************************************/
	//��ȡ����EnumChildWindows�ĵ�ַ
	fpEnumChildWindows = GetProcAddress(hModule, "EnumChildWindows");
	if(fpEnumChildWindows == NULL){
		return false;
	}
	//�µĺ���ָ���ַ
	fpNewEnumChildWindows=(LPVOID)MyEnumChildWindows;
	NewEnumChildWindowsCode[0] = 0xe9;//������ת��ָ��jmp�Ļ���ָ�0xe9Ϊ�����ת��0xffΪ������ת��
	
	addr=(DWORD *)(NewEnumChildWindowsCode+1);
	//�μ����תָ���Ϊ5���ֽڣ����JMP��ָ����㹫ʽΪ��Ŀ���ַ-��ǰ��ַ-5
	*addr=DWORD(MyEnumChildWindows)-DWORD(fpEnumChildWindows)-5;
	//��ԭʼ�ĺ����ֳ����б���
	RtlMoveMemory(OldEnumChildWindowsCode, fpEnumChildWindows, 5);
	/*********************************************************************************************************************/
		
	/**************************************  GetWindow Init  *********************************************************/
	//��ȡ����GetWindow�ĵ�ַ
	fpGetWindow = GetProcAddress(hModule, "GetWindow");
	if(fpGetWindow == NULL){
		return false;
	}
	//�µĺ���ָ���ַ
	fpNewGetWindow=(LPVOID)MyGetWindow;
	NewGetWindowCode[0] = 0xe9;//������ת��ָ��jmp�Ļ���ָ�0xe9Ϊ�����ת��0xffΪ������ת��

	addr=(DWORD *)(NewGetWindowCode+1);
	//�μ����תָ���Ϊ5���ֽڣ����JMP��ָ����㹫ʽΪ��Ŀ���ַ-��ǰ��ַ-5
	*addr=DWORD(MyGetWindow)-DWORD(fpGetWindow)-5;
	//��ԭʼ�ĺ����ֳ����б���
	RtlMoveMemory(OldGetWindowCode, fpGetWindow, 5);
	/*********************************************************************************************************************/

	/**************************************  FindWindowW Init  *********************************************************/
	//��ȡ����FindWindowW�ĵ�ַ
	fpFindWindowW = GetProcAddress(hModule, "FindWindowW");
	if(fpFindWindowW == NULL){
		return false;
	}
	//�µĺ���ָ���ַ
	fpNewFindWindowW=(LPVOID)MyFindWindowW;
	NewFindWindowWCode[0] = 0xe9;//������ת��ָ��jmp�Ļ���ָ�0xe9Ϊ�����ת��0xffΪ������ת��

	addr=(DWORD *)(NewFindWindowWCode+1);
	//�μ����תָ���Ϊ5���ֽڣ����JMP��ָ����㹫ʽΪ��Ŀ���ַ-��ǰ��ַ-5
	*addr=DWORD(MyFindWindowW)-DWORD(fpFindWindowW)-5;
	//��ԭʼ�ĺ����ֳ����б���
	RtlMoveMemory(OldFindWindowWCode, fpFindWindowW, 5);
	/*********************************************************************************************************************/
	
	/**************************************  FindWindowExW Init  *********************************************************/
	//��ȡ����FindWindowA�ĵ�ַ
	fpFindWindowExW = GetProcAddress(hModule, "FindWindowExW");
	if(fpFindWindowExW == NULL){
		return false;
	}
	//�µĺ���ָ���ַ
	fpNewFindWindowExW=(LPVOID)MyFindWindowExW;
	NewFindWindowExWCode[0] = 0xe9;//������ת��ָ��jmp�Ļ���ָ�0xe9Ϊ�����ת��0xffΪ������ת��

	addr=(DWORD *)(NewFindWindowExWCode+1);
	//�μ����תָ���Ϊ5���ֽڣ����JMP��ָ����㹫ʽΪ��Ŀ���ַ-��ǰ��ַ-5
	*addr=DWORD(MyFindWindowExW)-DWORD(fpFindWindowExW)-5;
	//��ԭʼ�ĺ����ֳ����б���
	RtlMoveMemory(OldFindWindowExWCode, fpFindWindowExW, 5);
	/*********************************************************************************************************************/
	
	return true;
}


//�Զ��庯��
BOOL WINAPI MyEnumWindows(WNDENUMPROC lpEnumFunc, LPARAM lParam)
{
	BOOL m_Return;
	m_Return = FALSE;
	return m_Return;
}

//�Զ��庯��
BOOL WINAPI MyEnumChildWindows(HWND hWndParent,WNDENUMPROC lpEnumFunc,LPARAM lParam)
{
	return FALSE;
}

//�Զ��庯��
HWND WINAPI MyGetWindow(HWND hWnd, UINT uCmd)
{
	return NULL;
}

//�Զ��庯��
HWND WINAPI MyFindWindowW(LPCSTR lpClassName, LPCSTR lpWindowName)
{
	HookOn();
	return NULL;
}

//�Զ��庯��
HWND WINAPI MyFindWindowExW(LPCSTR lpClassName, LPCSTR lpWindowName)
{
	HookOn();
	return NULL;
}

void HookOn()
{
	DWORD	dwProtect, dwWriteByte ;
	HANDLE hPro=GetCurrentProcess();

	/************************************** EnumWindows��ַ��ת ************************************/
	VirtualProtect(fpEnumWindows, 5, PAGE_READWRITE, &dwProtect );
	WriteProcessMemory(GetCurrentProcess(), fpEnumWindows, NewEnumWindowsCode, 5, 0 ) ;
	VirtualProtect(fpEnumWindows, 5, dwProtect, &dwProtect );
	/***********************************************************************************************/

	/************************************** EnumChildWindows��ַ��ת *******************************/
	VirtualProtect(fpEnumChildWindows, 5, PAGE_READWRITE, &dwProtect );
	WriteProcessMemory(GetCurrentProcess(), fpEnumChildWindows, NewEnumChildWindowsCode, 5, 0 ) ;
	VirtualProtect(fpEnumChildWindows, 5, dwProtect, &dwProtect );
	/***********************************************************************************************/

	/************************************** GetWindow��ַ��ת ************************************/
	VirtualProtect(fpGetWindow, 5, PAGE_READWRITE, &dwProtect );
	WriteProcessMemory(GetCurrentProcess(), fpGetWindow, NewGetWindowCode, 5, 0 ) ;
	VirtualProtect(fpGetWindow, 5, dwProtect, &dwProtect );
	/***********************************************************************************************/

	/************************************** FindWindowW��ַ��ת ************************************/
	VirtualProtect(fpFindWindowW, 5, PAGE_READWRITE, &dwProtect );
	WriteProcessMemory(GetCurrentProcess(), fpFindWindowW, NewFindWindowWCode, 5, 0 ) ;
	VirtualProtect(fpFindWindowW, 5, dwProtect, &dwProtect );
	/***********************************************************************************************/

	/************************************** FindWindowExW��ַ��ת ************************************/
	VirtualProtect(fpFindWindowExW, 5, PAGE_READWRITE, &dwProtect );
	WriteProcessMemory(GetCurrentProcess(), fpFindWindowExW, NewFindWindowWCode, 5, 0 ) ;
	VirtualProtect(fpFindWindowExW, 5, dwProtect, &dwProtect );
	/***********************************************************************************************/

	bHook=true;
}

void HookOff()
{
 	DWORD	dwProtect, dwWriteByte ;

	/************************************* EnumWindows��ַ���� *************************************/
	VirtualProtect(fpEnumWindows, 5, PAGE_READWRITE, &dwProtect );
	WriteProcessMemory(GetCurrentProcess(), fpEnumWindows, OldEnumWindowsCode, 5, 0 ) ;
	VirtualProtect(fpEnumWindows, 5, dwProtect, &dwProtect );
	/***********************************************************************************************/

	/************************************** EnumChildWindows��ַ��ת ************************************/
	VirtualProtect(fpEnumChildWindows, 5, PAGE_READWRITE, &dwProtect );
	WriteProcessMemory(GetCurrentProcess(), fpEnumChildWindows, OldEnumChildWindowsCode, 5, 0 ) ;
	VirtualProtect(fpEnumChildWindows, 5, dwProtect, &dwProtect );
	/***********************************************************************************************/

	/************************************** GetWindow��ַ��ת ************************************/
	VirtualProtect(fpGetWindow, 5, PAGE_READWRITE, &dwProtect );
	WriteProcessMemory(GetCurrentProcess(), fpGetWindow, OldGetWindowCode, 5, 0 ) ;
	VirtualProtect(fpGetWindow, 5, dwProtect, &dwProtect );
	/***********************************************************************************************/

	/************************************** FindWindowW��ַ��ת ************************************/
	VirtualProtect(fpFindWindowW, 5, PAGE_READWRITE, &dwProtect );
	WriteProcessMemory(GetCurrentProcess(), fpFindWindowW, OldFindWindowWCode, 5, 0 ) ;
	VirtualProtect(fpFindWindowW, 5, dwProtect, &dwProtect );
	/***********************************************************************************************/

	/************************************** FindWindowExW��ַ��ת ************************************/
	VirtualProtect(fpFindWindowExW, 5, PAGE_READWRITE, &dwProtect );
	WriteProcessMemory(GetCurrentProcess(), fpFindWindowExW, OldFindWindowExWCode, 5, 0 ) ;
	VirtualProtect(fpFindWindowExW, 5, dwProtect, &dwProtect );
	/***********************************************************************************************/

	bHook = false;
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	g_hinstDll=hModule;
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:	//��DLL������ <<��һ��>> ����ʱ������DllMain����������
		if(!Init())
		{
			//             MessageBoxA(NULL,"Init","ERROR",MB_OK);
			return(false);
		}
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		if(bHook) HookOff();
		break;
	}
	return TRUE;
}

