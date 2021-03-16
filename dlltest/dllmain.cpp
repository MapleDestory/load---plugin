#include "resource.h"

#if defined(__cplusplus)
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <algorithm>
#endif

#include <windows.h>
#include <commctrl.h>

#pragma comment(lib, "ComCtl32.lib")

// I made this struct to more conveniently store the
// positions / size of each window in the dialog
typedef struct SizeAndPos_s
{
	int x, y, width, height;
} SizeAndPos_t;

// Typically these would be #defines, but there
// is no reason to not make them constants
const WORD ID_btnHELLO = 1;
const WORD ID_btnQUIT = 2;
const WORD ID_CheckBox = 3;
const WORD ID_txtEdit = 4;
const WORD ID_btnShow = 5;

//                                    x,      y,      width,  height
const SizeAndPos_t mainWindow = { 150,    150,    300,    300 };

const SizeAndPos_t btnHello = { 20,     50,     80,     25 };
const SizeAndPos_t btnQuit = { 120,    50,     80,     25 };
const SizeAndPos_t chkCheck = { 20,     90,     185,    35 };

const SizeAndPos_t txtEdit = { 20,     150,    150,    20 };
const SizeAndPos_t btnShow = { 180,    150,    80,     25 };

HWND txtEditHandle = NULL;

// hwnd:    All window processes are passed the handle of the window
//         that they belong to in hwnd.
// msg:     Current message (e.g., WM_*) from the OS.
// wParam:  First message parameter, note that these are more or less
//          integers, but they are really just "data chunks" that
//          you are expected to memcpy as raw data to float, etc.
// lParam:  Second message parameter, same deal as above.
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	switch (msg)
	{

		case WM_CREATE:
		// Create the buttons
		//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

		// Note that the "parent window" is the dialog itself. Since we are
		// in the dialog's WndProc, the dialog's handle is passed into hwnd.
		//
		//CreateWindow( lpClassName,        lpWindowName,       dwStyle,                                x,          y,          nWidth,         nHeight,            hWndParent,     hMenu,              hInstance,      lpParam
		//CreateWindow( windowClassName,    initial text,       style (flags),                          xPos,       yPos,       width,          height,             parentHandle,   menuHandle,         instanceHandle, param);
		CreateWindow(TEXT("Button"), TEXT("Hello"), WS_VISIBLE | WS_CHILD, btnHello.x, btnHello.y, btnHello.width, btnHello.height, hwnd, (HMENU)ID_btnHELLO, NULL, NULL);

		CreateWindow(TEXT("Button"), TEXT("Quit"), WS_VISIBLE | WS_CHILD, btnQuit.x, btnQuit.y, btnQuit.width, btnQuit.height, hwnd, (HMENU)ID_btnQUIT, NULL, NULL);

		// Create a checkbox
		//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		CreateWindow(TEXT("button"), TEXT("CheckBox"), WS_VISIBLE | WS_CHILD | BS_CHECKBOX, chkCheck.x, chkCheck.y, chkCheck.width, chkCheck.height, hwnd, (HMENU)ID_CheckBox, NULL, NULL);

		// Create an edit box (single line text editing), and a button to show the text
		//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//Handle        = CreateWindow(windowClassName,    windowName,           style,                              xPos,       yPos,       width,          height,             parentHandle,   menuHandle,         instanceHandle, param);
		txtEditHandle = CreateWindow(TEXT("Edit"), TEXT("Initial Text"), WS_CHILD | WS_VISIBLE | WS_BORDER, txtEdit.x, txtEdit.y, txtEdit.width, txtEdit.height, hwnd, (HMENU)ID_txtEdit, NULL, NULL);

		//CreateWindow( windowClassName,    windowName,         style,                                  xPos,      yPos,      width,          height,           parentHandle,   menuHandle,         instanceHandle, param);
		CreateWindow(TEXT("Button"), TEXT("Show"), WS_VISIBLE | WS_CHILD, btnShow.x, btnShow.y, btnShow.width, btnShow.height, hwnd, (HMENU)ID_btnShow, NULL, NULL);

		// Create an Updown control. Note that this control will allow you to type in non-number characters, but it will not affect the state of the control

		break;

		// For more information about WM_COMMAND, see
		// https://msdn.microsoft.com/en-us/library/windows/desktop/ms647591(v=vs.85).aspx
		case WM_COMMAND:

		// The LOWORD of wParam identifies which control sent
		// the WM_COMMAND message. The WM_COMMAND message is
		// sent when the button has been clicked.
		if (LOWORD(wParam) == ID_btnHELLO)
		{
			MessageBox(hwnd, TEXT("Hello!"), TEXT("Hello"), MB_OK);
		}
		else if (LOWORD(wParam) == ID_btnQUIT)
		{
			//CloseWindow(hwnd);
			DestroyWindow(hwnd);
		}
		else if (LOWORD(wParam) == ID_CheckBox)
		{
			UINT checked = IsDlgButtonChecked(hwnd, ID_CheckBox);

			if (checked)
			{
				CheckDlgButton(hwnd, ID_CheckBox, BST_UNCHECKED);
				MessageBox(hwnd, TEXT("The checkbox has been unchecked."), TEXT("CheckBox Event"), MB_OK);
			}
			else
			{
				CheckDlgButton(hwnd, ID_CheckBox, BST_CHECKED);
				MessageBox(hwnd, TEXT("The checkbox has been checked."), TEXT("CheckBox Event"), MB_OK);
			}
		}
		else if (LOWORD(wParam) == ID_btnShow)
		{
			int textLength_WithNUL = GetWindowTextLength(txtEditHandle) + 1;
			// WARNING: If you are compiling this for C, please remember to remove the (TCHAR*) cast.
			TCHAR* textBoxText = (TCHAR*)malloc(sizeof(TCHAR) * textLength_WithNUL);

			GetWindowText(txtEditHandle, textBoxText, textLength_WithNUL);

			MessageBox(hwnd, textBoxText, TEXT("Here's what you typed"), MB_OK);

			free(textBoxText);
		}
		break;

		case WM_CLOSE:
			DestroyWindow(hwnd);
		break;

		case WM_DESTROY:
			PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

extern "C" __declspec(dllexport) 
int SDK_Impl_Instance(const std::string& Hash)
{
	//MessageBoxA(nullptr, Hash.c_str(), "", MB_ICONINFORMATION | MB_OK);
	
	MSG  msg;
	WNDCLASS mainWindowClass = { 0 };

	mainWindowClass.lpszClassName = TEXT("JRH.MainWindow");

	mainWindowClass.hInstance = GetModuleHandleW(NULL);
	mainWindowClass.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	mainWindowClass.lpfnWndProc = WndProc;
	mainWindowClass.hCursor = LoadCursor(0, IDC_ARROW);

	RegisterClass(&mainWindowClass);

	::CreateWindow(mainWindowClass.lpszClassName, TEXT("Main Window"), WS_OVERLAPPEDWINDOW | WS_VISIBLE, mainWindow.x, mainWindow.y, mainWindow.width, mainWindow.height, NULL, 0, GetModuleHandleW(NULL), NULL);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, PVOID lpReserved)
{
	switch (dwReason)
	{
		case DLL_PROCESS_ATTACH:
		// attach to process, return FALSE to fail
		break;

		case DLL_PROCESS_DETACH:
		// detaching from process
		break;

		case DLL_THREAD_ATTACH:
		// attach to thread within process
		break;

		case DLL_THREAD_DETACH:
		// detach from thread within process
		break;
	}
	return TRUE;
}