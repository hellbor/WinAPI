﻿#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include<stdio.h>
#include"resource.h"

CONST CHAR* g_VALUES[] = { "This", "is", "my", "first", "List", "Box" };

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgProcAddItem(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//BOOL CALLBACK DlgProcDeleteItem(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgProcEditItem(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG), NULL, DlgProc, 0);
	return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
		SendMessage(hwnd, WM_SETICON, 0, (LPARAM)hIcon);

		HWND hList = GetDlgItem(hwnd, IDC_LIST1);
		for (int i = 0; i < sizeof(g_VALUES) / sizeof(g_VALUES[0]); i++)
		{
			SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)g_VALUES[i]);
		}
		SendMessage(hList, LB_SETCURSEL, 0, 0);
	}
	break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_LIST1:
			switch (HIWORD(wParam))
			{
			case LBN_DBLCLK:
				DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_ADD), hwnd, DlgProcEditItem, 0);
				break;
			}
			break;
		case IDC_BUTTON_ADD:
			DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_ADD), hwnd, DlgProcAddItem, 0);
			break;
		case IDC_BUTTON_DELETE:
			//DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_DELETE), hwnd, DlgProcDeleteItem, 0);
		{
			HWND hListBox = GetDlgItem(hwnd, IDC_LIST1);
			INT i = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
			SendMessage(hListBox, LB_DELETESTRING, i, 0);
		}
		case IDOK:
		{
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE]{};
			HWND hList = GetDlgItem(hwnd, IDC_LIST1);
			INT i = SendMessage(hList, LB_GETCURSEL, 0, 0);
			SendMessage(hList, LB_GETTEXT, i, (LPARAM)sz_buffer);

			CHAR sz_message[SIZE]{};
			sprintf(sz_message, "Вы выбрали пункт №%i со значением\"%s\".", i, sz_buffer);
			MessageBox(hwnd, sz_message, "Info", MB_OK | MB_ICONINFORMATION);
		}
		break;
		case WM_KEYDOWN:
			MessageBox(hwnd, "DELETE pressed", "Info", MB_OK | MB_ICONINFORMATION);
			switch (wParam)
			{
			case VK_DELETE:
				SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_DELETE), (LPARAM)GetDlgItem(hwnd, IDC_LIST1));
				break;
			}
			break;
		case IDCANCEL:EndDialog(hwnd, 0); break;
		}
		break;
	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;
	}
	return FALSE;
}
BOOL CALLBACK DlgProcAddItem(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		SetFocus(GetDlgItem(hwnd, IDC_EDIT_ADD));
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE]{};
			HWND hEditAdd = GetDlgItem(hwnd, IDC_EDIT_ADD);
			SendMessage(hEditAdd, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			if (strlen(sz_buffer) == 0)break;

			HWND hList = GetDlgItem(GetParent(hwnd), IDC_LIST1);
			//GetParent(hwnd) - возвращает родительское окно (HWND родительского окна) для указанного окна.
			if (SendMessage(hList, LB_FINDSTRING, -1, (LPARAM)sz_buffer) == LB_ERR)
			{
				SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)sz_buffer);
			}
			else
			{
				INT result = MessageBox
				(
					hwnd,
					"Такое значение уже есть в списке, хотите ввести другое значение?",
					"Info",
					MB_YESNO | MB_ICONQUESTION
				);
				if (result == IDYES)break;
			}
		}
		case IDCANCEL:
			EndDialog(hwnd, 0);
			break;
		}
		break;
	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;
	}
	return FALSE;
}

//BOOL CALLBACK DlgProcDeleteItem(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//	switch (uMsg)
//	{
//	case WM_INITDIALOG:
//		SetFocus(GetDlgItem(hwnd, IDC_EDIT_DELETE));
//		break;
//	case WM_COMMAND:
//		switch (LOWORD(wParam))
//		{
//		case IDOK:
//		{
//			CONST INT SIZE = 256;
//			CHAR sz_buffer[SIZE]{};
//			HWND hEditDelete = GetDlgItem(hwnd, IDC_EDIT_DELETE);
//			SendMessage(hEditDelete, WM_CLEAR, 0, 0);
//
//			HWND hList = GetDlgItem(GetParent(hwnd), IDC_LIST1);
//			SendMessage(hList, LB_FINDSTRING, -1, 0);
//			SendMessage(hList, LB_DELETESTRING, 0, 0);
//			MessageBox(hwnd, "Вы действительно хотите выбранную удалить строку?", "Info", MB_YESNO | MB_ICONQUESTION);
//		}
//		case IDCANCEL:
//			EndDialog(hwnd, 0);
//			break;
//		}
//		break;
//	case WM_CLOSE:
//		EndDialog(hwnd, 0);
//		break;
//	}
//	return FALSE;
//}

BOOL CALLBACK DlgProcEditItem(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)"Edit item");
		HWND hParent = GetParent(hwnd);
		HWND hListBox = GetDlgItem(hParent, IDC_LIST1);
		HWND hEdit = GetDlgItem(hwnd, IDC_EDIT_ADD);
		CONST INT SIZE = 256;
		CHAR sz_buffer[SIZE]{};
		INT i = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
		SendMessage(hListBox, LB_GETTEXT, i, (LPARAM)sz_buffer);
		SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_buffer);
	}
	break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			HWND hParent = GetParent(hwnd);
			HWND hListBox = GetDlgItem(hParent, IDC_LIST1);
			HWND hEdit = GetDlgItem(hwnd, IDC_EDIT_ADD);
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE];
			SendMessage(hEdit, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			INT i = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
			SendMessage(hListBox, LB_DELETESTRING, i, 0);
			SendMessage(hListBox, LB_INSERTSTRING, i, (LPARAM)sz_buffer);
		}
		case IDCANCEL:
			EndDialog(hwnd, 0);
			break;
		}
		break;
	case WM_CLOSE:
		EndDialog(hwnd, 0);
	}
	return FALSE;
}