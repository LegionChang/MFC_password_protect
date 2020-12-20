// dllmain.cpp : 定义 DLL 应用程序的入口点。
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


//鼠标钩子函数入口
//鼠标钩子将动态链接库注入到其他程序，没有别的作用，所以直接返回
LRESULT WINAPI HookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	return(CallNextHookEx(g_hHook, nCode, wParam, lParam));
}

//钩子安装函数
extern "C" bool __declspec(dllexport) __stdcall InstallHook(HWND hWnd)
{
	if(!g_hHook){
		g_hHook = SetWindowsHookEx(WH_MOUSE, (HOOKPROC)HookProc, g_hinstDll, 0);
	}
	if (!g_hHook)
	{
		MessageBoxA(0, "失败！", "", MB_OK);
		MessageBoxW(0, L"失败！", L"", MB_OK);
		return false;
	}
	HookOn();
	return true;
}

//钩子卸载函数
extern "C" BOOL __declspec(dllexport) __stdcall UninstallHook()
{
	BOOL result=false;
	if(g_hHook) {
		if(bHook) HookOff();
		result=UnhookWindowsHookEx(g_hHook);
	}
	g_hHook=NULL;
	return result;
}

bool Init()
{
	//获取模块user32.dll的句柄
	hModule=GetModuleHandle(L"user32.dll");
	DWORD   *addr;

	/**************************************  EnumWindows Init  *********************************************************/
	//获取函数EnumWindows的地址
	fpEnumWindows = GetProcAddress(hModule, "EnumWindows");
	if(fpEnumWindows == NULL){
		return false;
	}
	//新的函数指针地址
	fpNewEnumWindows=(LPVOID)MyEnumWindows;
	NewEnumWindowsCode[0] = 0xe9;//无条件转移指令jmp的机器指令（0xe9为相对跳转、0xff为绝对跳转）
	
	addr=(DWORD *)(NewEnumWindowsCode+1);
	//段间的跳转指令长度为5个字节，因此JMP的指令计算公式为：目标地址-当前地址-5
	*addr=DWORD(MyEnumWindows)-DWORD(fpEnumWindows)-5;
	//将原始的函数现场进行保存
	RtlMoveMemory(OldEnumWindowsCode, fpEnumWindows, 5);
	/*********************************************************************************************************************/

	/**************************************  EnumChildWindows Init  *********************************************************/
	fpEnumChildWindows = GetProcAddress(hModule, "EnumChildWindows");
	if(fpEnumChildWindows == NULL){
		return false;
	}
	fpNewEnumChildWindows=(LPVOID)MyEnumChildWindows;
	NewEnumChildWindowsCode[0] = 0xe9;
	
	addr=(DWORD *)(NewEnumChildWindowsCode+1);
	*addr=DWORD(MyEnumChildWindows)-DWORD(fpEnumChildWindows)-5;
	RtlMoveMemory(OldEnumChildWindowsCode, fpEnumChildWindows, 5);
	/*********************************************************************************************************************/
		
	/**************************************  GetWindow Init  *********************************************************/
	fpGetWindow = GetProcAddress(hModule, "GetWindow");
	if(fpGetWindow == NULL){
		return false;
	}
	fpNewGetWindow=(LPVOID)MyGetWindow;
	NewGetWindowCode[0] = 0xe9;
	addr=(DWORD *)(NewGetWindowCode+1);
	*addr=DWORD(MyGetWindow)-DWORD(fpGetWindow)-5;
	RtlMoveMemory(OldGetWindowCode, fpGetWindow, 5);
	/*********************************************************************************************************************/

	/**************************************  FindWindowW Init  *********************************************************/
	fpFindWindowW = GetProcAddress(hModule, "FindWindowW");
	if(fpFindWindowW == NULL){
		return false;
	}
	fpNewFindWindowW=(LPVOID)MyFindWindowW;
	NewFindWindowWCode[0] = 0xe9;
	addr=(DWORD *)(NewFindWindowWCode+1);
	*addr=DWORD(MyFindWindowW)-DWORD(fpFindWindowW)-5;
	RtlMoveMemory(OldFindWindowWCode, fpFindWindowW, 5);
	/*********************************************************************************************************************/
	
	/**************************************  FindWindowExW Init  *********************************************************/
	fpFindWindowExW = GetProcAddress(hModule, "FindWindowExW");
	if(fpFindWindowExW == NULL){
		return false;
	}
	fpNewFindWindowExW=(LPVOID)MyFindWindowExW;
	NewFindWindowExWCode[0] = 0xe9;
	addr=(DWORD *)(NewFindWindowExWCode+1);
	*addr=DWORD(MyFindWindowExW)-DWORD(fpFindWindowExW)-5;
	RtlMoveMemory(OldFindWindowExWCode, fpFindWindowExW, 5);
	/*********************************************************************************************************************/
	
	return true;
}


//自定义函数
BOOL WINAPI MyEnumWindows(WNDENUMPROC lpEnumFunc, LPARAM lParam)
{
	BOOL m_Return;
	m_Return = FALSE;
	return m_Return;
}

//自定义函数
BOOL WINAPI MyEnumChildWindows(HWND hWndParent,WNDENUMPROC lpEnumFunc,LPARAM lParam)
{
	return FALSE;
}

//自定义函数
HWND WINAPI MyGetWindow(HWND hWnd, UINT uCmd)
{
	return NULL;
}

//自定义函数
HWND WINAPI MyFindWindowW(LPCSTR lpClassName, LPCSTR lpWindowName)
{
	HookOn();
	return NULL;
}

//自定义函数
HWND WINAPI MyFindWindowExW(LPCSTR lpClassName, LPCSTR lpWindowName)
{
	HookOn();
	return NULL;
}

void HookOn()
{
	DWORD	dwProtect, dwWriteByte ;
	HANDLE hPro=GetCurrentProcess();

	/************************************** EnumWindows地址跳转 ************************************/
	VirtualProtect(fpEnumWindows, 5, PAGE_READWRITE, &dwProtect );
	WriteProcessMemory(GetCurrentProcess(), fpEnumWindows, NewEnumWindowsCode, 5, 0 ) ;
	VirtualProtect(fpEnumWindows, 5, dwProtect, &dwProtect );
	/***********************************************************************************************/

	/************************************** EnumChildWindows地址跳转 *******************************/
	VirtualProtect(fpEnumChildWindows, 5, PAGE_READWRITE, &dwProtect );
	WriteProcessMemory(GetCurrentProcess(), fpEnumChildWindows, NewEnumChildWindowsCode, 5, 0 ) ;
	VirtualProtect(fpEnumChildWindows, 5, dwProtect, &dwProtect );
	/***********************************************************************************************/

	/************************************** GetWindow地址跳转 ************************************/
	VirtualProtect(fpGetWindow, 5, PAGE_READWRITE, &dwProtect );
	WriteProcessMemory(GetCurrentProcess(), fpGetWindow, NewGetWindowCode, 5, 0 ) ;
	VirtualProtect(fpGetWindow, 5, dwProtect, &dwProtect );
	/***********************************************************************************************/

	/************************************** FindWindowW地址跳转 ************************************/
	VirtualProtect(fpFindWindowW, 5, PAGE_READWRITE, &dwProtect );
	WriteProcessMemory(GetCurrentProcess(), fpFindWindowW, NewFindWindowWCode, 5, 0 ) ;
	VirtualProtect(fpFindWindowW, 5, dwProtect, &dwProtect );
	/***********************************************************************************************/

	/************************************** FindWindowExW地址跳转 ************************************/
	VirtualProtect(fpFindWindowExW, 5, PAGE_READWRITE, &dwProtect );
	WriteProcessMemory(GetCurrentProcess(), fpFindWindowExW, NewFindWindowWCode, 5, 0 ) ;
	VirtualProtect(fpFindWindowExW, 5, dwProtect, &dwProtect );
	/***********************************************************************************************/

	bHook=true;
}

void HookOff()
{
 	DWORD	dwProtect, dwWriteByte ;

	/************************************* EnumWindows地址跳回 *************************************/
	VirtualProtect(fpEnumWindows, 5, PAGE_READWRITE, &dwProtect );
	WriteProcessMemory(GetCurrentProcess(), fpEnumWindows, OldEnumWindowsCode, 5, 0 ) ;
	VirtualProtect(fpEnumWindows, 5, dwProtect, &dwProtect );
	/***********************************************************************************************/

	/************************************** EnumChildWindows地址跳转 ************************************/
	VirtualProtect(fpEnumChildWindows, 5, PAGE_READWRITE, &dwProtect );
	WriteProcessMemory(GetCurrentProcess(), fpEnumChildWindows, OldEnumChildWindowsCode, 5, 0 ) ;
	VirtualProtect(fpEnumChildWindows, 5, dwProtect, &dwProtect );
	/***********************************************************************************************/

	/************************************** GetWindow地址跳转 ************************************/
	VirtualProtect(fpGetWindow, 5, PAGE_READWRITE, &dwProtect );
	WriteProcessMemory(GetCurrentProcess(), fpGetWindow, OldGetWindowCode, 5, 0 ) ;
	VirtualProtect(fpGetWindow, 5, dwProtect, &dwProtect );
	/***********************************************************************************************/

	/************************************** FindWindowW地址跳转 ************************************/
	VirtualProtect(fpFindWindowW, 5, PAGE_READWRITE, &dwProtect );
	WriteProcessMemory(GetCurrentProcess(), fpFindWindowW, OldFindWindowWCode, 5, 0 ) ;
	VirtualProtect(fpFindWindowW, 5, dwProtect, &dwProtect );
	/***********************************************************************************************/

	/************************************** FindWindowExW地址跳转 ************************************/
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
	case DLL_PROCESS_ATTACH:	//当DLL被进程 <<第一次>> 调用时，导致DllMain函数被调用
		if(!Init())
		{
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

