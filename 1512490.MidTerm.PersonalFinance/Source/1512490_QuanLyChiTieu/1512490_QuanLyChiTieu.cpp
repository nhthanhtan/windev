// 1512490_QuanLyChiTieu.cpp : Defines the entry point for the application.
//IDC_MY1512490QUANLYCHITIEU

#include "stdafx.h"
#include "1512490_QuanLyChiTieu.h"
#include "stdafx.h"
#include "MoHinh.h"
#include <windowsx.h>
#include <vector>
#include <fstream>
#include <locale>
#include <codecvt>
#include <string>
#include <commctrl.h>
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "ComCtl32.lib")


#define MAX_LOADSTRING 100
#define FILE_PATH		L"data.txt"//file chưa toàn bộ thông tin khi tắt chương trình
#define CHART_HEIGHT	30
#define CHART_WIDTH		600
#define COLOR_A			RGB(73,134,238)//xét màu sắc của các thanh trong biểu đồ
#define COLOR_B			RGB(220,51,55)
#define COLOR_C			RGB(157,40,177)
#define COLOR_D			RGB(245,195,14)
#define COLOR_E			RGB(61,187,85)
#define COLOR_F			RGB(142,111,100)
#define DEFAULT_COLOR	RGB(255,255,255)

//hecxa của các phím L và N
#define VK_CHAR_L		0x4C
#define VK_CHAR_N		0x4E

// biến toàn cục
HINSTANCE hInst;								// current instance
HWND g_hWnd;
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
WCHAR types[6][25] =
{
	L"Ăn uống", L"Di chuyển", L"Nhà cửa",
	L"Xe cộ", L"Nhu yếu phẩm", L"Dịch vụ",
};
COLORREF color[7] = { COLOR_A, COLOR_B, COLOR_C, COLOR_D, COLOR_E, COLOR_F, DEFAULT_COLOR };
int currX[7];
int currY;
long long* typeMoney;
int currTypeIndex = 6;

int defaultWidth = 700, defaultHeight = 700;
int g_ItemCount = 0;
long long g_totalMoney = 0;
bool isAdding = true;
std::vector<MoHinh*> listItem;
int currSelectedIndex = 0;

HWND hComboBox, hMoneyInput, hDescriptionInput, hTotalMoney, hMoneyInputMonth;
HWND hAddButton, hClose, hParser;
HWND g_hListview;
HWND  hPercentA, hPercentB, hPercentC, hPercentD, hPercentE, hPercentF;
HWND hStaticType, hWelcome;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK Clear(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

HWND createListView(long lExtStyle, HWND parentWnd, long ID, HINSTANCE hParentInst, int x, int y, int nWidth, int nHeight, long lStyle);//tạo listview
bool insertItemToList(HWND m_hListview, int mItemCount); //tạo thư mục mới từ dữ liệu đầu vào, chèn vào listview và list item.
void writeListItemToFile(std::wstring path); //File I/O
void loadListItemFromFile(std::wstring path); //File I/O
void loadAllItemToListview(HWND m_hListview); //đưa tất cả các item vào listview
void drawStatistics(HDC hdc); //tính toán và vẽ
void drawLine(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color); //vẽ đường thẳng
void fillRectangle(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color); //đường thẳng thêm màu sắc
void setWindowText(HWND hWnd, long long value, std::wstring textBefore, std::wstring textAfter); 
int getTypeIndex(MoHinh* item); //so sánh mặt hàng và tiến hành lấy được địa chỉ để thực hiện
void enableInputWindow(); //bật tính hiệu điều khiển
int detectCoord(int x, int y); //tính năng trỏ chuột hiện thông tin
void showTypeLabel(int index, HDC hdc); //hiển thị loại
void garbageCollector(); //GC
void addItemProc(HWND m_hListview); //thêm mục vào tiến trình
void handleKeyDown(HWND hWnd, int type); //phím tắt
void theParser();//xuất thông báo phân tích cho người dùng

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MY1512490QUANLYCHITIEU, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY1512490QUANLYCHITIEU));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY1512490QUANLYCHITIEU));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_MY1512490QUANLYCHITIEU);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // Store instance handle in our global variable

	hWnd = CreateWindowEx(0, szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, defaultWidth, defaultHeight, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc = NULL;

	switch (message)
	{
	case WM_CREATE:
	{
		//Init extended common controls
		INITCOMMONCONTROLSEX icc;
		icc.dwSize = sizeof(icc);
		icc.dwICC = ICC_WIN95_CLASSES;
		InitCommonControlsEx(&icc);

		//Get main window DC
		hdc = GetDC(hWnd);
		g_hWnd = hWnd;

		HFONT hFont;
		//tạo group box
		hFont = CreateFont(16, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
		//tạo nút bắm thoát chương trình
		
		hClose = CreateWindowEx(0, L"BUTTON", L"Thoát", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 300, 580, 120, 32, hWnd, (HMENU)IDC_BUTTON_CLOSE, hInst, NULL);
		SendMessage(hAddButton, WM_SETFONT, WPARAM(hFont), TRUE);
		//tạo nút thêm thông tin
		hAddButton = CreateWindowEx(0, L"BUTTON", L"THÊM", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  40, 270, 120, 32, hWnd, (HMENU)IDC_BUTTON_ADD, hInst, NULL);
		SendMessage(hAddButton, WM_SETFONT, WPARAM(hFont), TRUE);
		//tạo ra nút để cho sự phân tích
		hParser = CreateWindowEx(0, L"BUTTON", L"Phân tích", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 200, 500, 120, 32, hWnd, (HMENU)IDC_BUTTON_PARSER, hInst, NULL);
		SendMessage(hParser, WM_SETFONT, WPARAM(hFont), TRUE);
		//tạo ra danh sách lựa chọn
		hFont = CreateFont(16, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
		hComboBox = CreateWindowEx(0, WC_COMBOBOX, TEXT(""),
			CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
			 40,  50, 120, 100, hWnd, NULL, hInst,
			NULL);
		SendMessage(hComboBox, WM_SETFONT, WPARAM(hFont), TRUE);
		//tạo tiền nhập vào
		hMoneyInput = CreateWindowEx(0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER,  40,  80, 120, 20, hWnd, NULL, hInst, NULL);
		SendMessage(hMoneyInput, WM_SETFONT, WPARAM(hFont), TRUE);
		SendMessage(hMoneyInput, EM_SETCUEBANNER, TRUE, (LPARAM)L"Nhập số tiền...");
		//mô tả thông tin sử dụng tiền
		hDescriptionInput = CreateWindowEx(0, L"EDIT", L"Ăn sáng...", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL,  40,  110, 120, 150, hWnd, NULL, hInst, NULL);
		SendMessage(hDescriptionInput, WM_SETFONT, WPARAM(hFont), TRUE);
		//khung Thêm 1 loại chi tiêu
		HWND hGroupboxA = CreateWindowEx(0, L"BUTTON", L"Thêm một loại chi tiêu", WS_CHILD | WS_VISIBLE | BS_GROUPBOX | WS_GROUP, 20, 20, 160, 300, hWnd, (HMENU)IDC_GROUPBOX, hInst, NULL);
		SendMessage(hGroupboxA, WM_SETFONT, WPARAM(hFont), TRUE);

		//so sánh
		HWND hGroupboxD = CreateWindowEx(0, L"BUTTON", L"Phân tích kết quả chi tiêu", WS_CHILD | WS_VISIBLE | BS_GROUPBOX | WS_GROUP, 20, 480, 640, 80, hWnd, (HMENU)IDC_GROUPBOX, hInst, NULL);
		SendMessage(hGroupboxD, WM_SETFONT, WPARAM(hFont), TRUE);
		//truyền vào tiền tháng trước để so sánh
		hMoneyInputMonth = CreateWindowEx(0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, 40, 510, 120, 20, hWnd, NULL, hInst, NULL);
		SendMessage(hMoneyInputMonth, WM_SETFONT, WPARAM(hFont), TRUE);
		SendMessage(hMoneyInputMonth, EM_SETCUEBANNER, TRUE, (LPARAM)L"Tiền tháng trước...");


		//khung danh sách chi tiêu
		HWND hGroupboxB = CreateWindowEx(0, L"BUTTON", L"Toàn bộ danh sách các chi tiêu", WS_CHILD | WS_VISIBLE | BS_GROUPBOX | WS_GROUP, 190, 20, 470, 300, hWnd, (HMENU)IDC_GROUPBOX, hInst, NULL);
		SendMessage(hGroupboxB, WM_SETFONT, WPARAM(hFont), TRUE);

		//khởi tạo listview
		long extStyle = WS_EX_CLIENTEDGE;
		long style = LVS_REPORT | LVS_ICON | LVS_EDITLABELS | LVS_SHOWSELALWAYS;

		g_hListview = createListView(extStyle, hWnd, IDL_LISTVIEW, hInst,  210,  50, 430, 250, style);



		//tạo group thống kê
		HWND hGroupboxC = CreateWindowEx(0, L"BUTTON", L"Thông tin thống kê", WS_CHILD | WS_VISIBLE | BS_GROUPBOX | WS_GROUP, 20,  330, 640, 140, hWnd, (HMENU)IDC_GROUPBOX, hInst, NULL);
		SendMessage(hGroupboxC, WM_SETFONT, WPARAM(hFont), TRUE);
		//tổng tiền mà người dùng đã sử dụng
		hTotalMoney = CreateWindowEx(0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY,  315,  360, 120, 20, hWnd, NULL, hInst, NULL);
		SendMessage(hTotalMoney, WM_SETFONT, WPARAM(hFont), TRUE);

		hFont = CreateFont(16, 0, 0, 0, FW_DONTCARE, FALSE, TRUE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
		HWND hTemp = CreateWindowEx(0, L"STATIC", L"Tổng tiền", WS_CHILD | WS_VISIBLE,  245, 362, 60, 15, hWnd, NULL, hInst, NULL);
		SendMessage(hTemp, WM_SETFONT, WPARAM(hFont), TRUE);

		hFont = CreateFont(14, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
		hStaticType = CreateWindowEx(0, L"STATIC", L"", WS_CHILD | WS_VISIBLE, 410,  437, 230, 15, hWnd, (HMENU)NULL, hInst, NULL);
		SendMessage(hStaticType, WM_SETFONT, WPARAM(hFont), TRUE);


		
		hFont = CreateFont(16, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI");

		hPercentA = CreateWindowEx(0, L"STATIC", L"10%", WS_CHILD, 20, 397, 30, 15, hWnd, (HMENU)NULL, hInst, NULL);
		SendMessage(hPercentA, WM_SETFONT, WPARAM(hFont), TRUE);

		hPercentB = CreateWindowEx(0, L"STATIC", L"10%", WS_CHILD, 20,  397, 30, 15, hWnd, (HMENU)NULL, hInst, NULL);
		SendMessage(hPercentB, WM_SETFONT, WPARAM(hFont), TRUE);

		hPercentC = CreateWindowEx(0, L"STATIC", L"10%", WS_CHILD, 20,  397, 30, 15, hWnd, (HMENU)NULL, hInst, NULL);
		SendMessage(hPercentC, WM_SETFONT, WPARAM(hFont), TRUE);

		hPercentD = CreateWindowEx(0, L"STATIC", L"10%", WS_CHILD, 20,  397, 30, 15, hWnd, (HMENU)NULL, hInst, NULL);
		SendMessage(hPercentD, WM_SETFONT, WPARAM(hFont), TRUE);

		hPercentE = CreateWindowEx(0, L"STATIC", L"10%", WS_CHILD, 20,  397, 30, 15, hWnd, (HMENU)NULL, hInst, NULL);
		SendMessage(hPercentE, WM_SETFONT, WPARAM(hFont), TRUE);

		hPercentF = CreateWindowEx(0, L"STATIC", L"10%", WS_CHILD, 20,  397, 30, 15, hWnd, (HMENU)NULL, hInst, NULL);
		SendMessage(hPercentF, WM_SETFONT, WPARAM(hFont), TRUE);

		hFont = CreateFont(45, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI");

		hWelcome = CreateWindowEx(0, L"STATIC", L"Quản Lý Chi Tiêu", WS_CHILD,  200, 397, 300, 50, hWnd, (HMENU)NULL, hInst, NULL);
		SendMessage(hWelcome, WM_SETFONT, WPARAM(hFont), TRUE);

		//Initialize combobox
		TCHAR A[16];
		int  k = 0;

		memset(&A, 0, sizeof(A));

		for (k = 0; k < 6; k += 1)
		{
			wcscpy_s(A, sizeof(A) / sizeof(TCHAR), (TCHAR*)types[k]);

			// thêm chuỗi vào combobox
			SendMessage(hComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)A);
		}


		// lựa chọn  
		SendMessage(hComboBox, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

		//File I/O
		loadListItemFromFile(FILE_PATH);
		loadAllItemToListview(g_hListview);
		g_ItemCount = listItem.size();

		setWindowText(hTotalMoney, g_totalMoney, L"", L"");
	}
	break;
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		if (wmEvent == CBN_SELCHANGE)
		{
			WCHAR buffer[25];
			int i = SendMessage(hComboBox, CB_GETCURSEL, NULL, NULL);
			wsprintf(buffer, L"%s...", types[i]);
			SetWindowText(hDescriptionInput, buffer);
		}

		switch (wmId)
		{
		case IDC_BUTTON_ADD:
			//thêm sự kiện vào
			addItemProc(g_hListview);
			break;

		case IDC_BUTTON_CLOSE:
			//đóng toàn bộ chương trình đang chạy
			DestroyWindow(hWnd);
			break;

		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;

		case ID_FILE_CLEAR:
			//hiển thị thông báo xóa toàn bộ thông tin
			DialogBox(hInst, MAKEINTRESOURCE(IDD_CLEAR_DIALOG), hWnd, Clear);
			break;

		case ID_FILE_NEW:
			//tạo mới thêm 1 sự kiện
			addItemProc(g_hListview);
			break;

		case IDM_EXIT:
			//giống như close đóng toàn bộ chương trình
			DestroyWindow(hWnd);
			break;
		case IDC_BUTTON_PARSER:
			theParser();
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	case WM_CTLCOLORSTATIC:
	{
		//TODO: Draw the static text fore color and back color
		wmId = LOWORD(wParam);
		HDC hdcStatic = (HDC)wParam;
		COLORREF color;

		//tiến hành chọn màu sắc
		if ((HWND)lParam == hPercentA) 
		{
			color = COLOR_A;
		}
		else if ((HWND)lParam == hPercentB)
		{
			color = COLOR_B;
		}
		else if ((HWND)lParam == hPercentC)
		{
			color = COLOR_C;
		}
		else if ((HWND)lParam == hPercentD)
		{
			color = COLOR_D;
		}
		else if ((HWND)lParam == hPercentE)
		{
			color = COLOR_E;
		}
		else if ((HWND)lParam == hPercentF)
		{
			color = COLOR_F;
		}
		else if ((HWND)lParam == hWelcome)
		{
			SetTextColor(hdcStatic, COLOR_B);
			return (BOOL)GetSysColorBrush(COLOR_WINDOW);
		}
		else
		{
			return (BOOL)GetSysColorBrush(COLOR_WINDOW);
		}

		SetTextColor(hdcStatic, RGB(255, 255, 255));
		SetBkColor(hdcStatic, color);

		return (BOOL)GetSysColorBrush(COLOR_WINDOW);
	}

	case WM_NOTIFY:
		//TODO: Listen to ListView notifications
		switch (((NMHDR*)lParam)->code)
		{
		case LVN_KEYDOWN:
			if (LPNMLVKEYDOWN(lParam)->wVKey == VK_DELETE && currSelectedIndex >= 0 && currSelectedIndex < listItem.size() && listItem.size() > 0) {
				//xóa listview
				ListView_DeleteItem(g_hListview, currSelectedIndex);

				//trừ tổng của toàn bộ tiền
				g_totalMoney -= listItem[currSelectedIndex]->mMoney;
				g_ItemCount--;
				setWindowText(hTotalMoney, g_totalMoney, L"", L"");

				//xóa item của danh sách
				listItem.erase(listItem.begin() + currSelectedIndex);

				//tiến hành vẽ lại
				RedrawWindow(g_hWnd, NULL, NULL, RDW_INTERNALPAINT | RDW_UPDATENOW | RDW_ERASENOW | RDW_INVALIDATE);
			}
			else if (LPNMLVKEYDOWN(lParam)->wVKey == VK_CHAR_N)
			{
				//sử dụng phím tắt Ctrl N
				handleKeyDown(hWnd, 0);
			}
			else if (LPNMLVKEYDOWN(lParam)->wVKey == VK_CHAR_L)
			{
				//sử dụng phím tắt Ctrl L
				handleKeyDown(hWnd, 1);
			}
			break;

		case NM_CLICK:
		{
			int position = ListView_GetNextItem(g_hListview, -1, LVNI_SELECTED);
			if (position >= 0 && position < listItem.size())
			{
				currSelectedIndex = position;
				MoHinh* item = listItem[position];
				setWindowText(hMoneyInput, item->mMoney, L"", L"");
				SetWindowText(hDescriptionInput, item->mDescription.c_str());
				SendMessage(hComboBox, CB_SETCURSEL, WPARAM(getTypeIndex(item)), NULL);
			}
		}
		break;

		case NM_DBLCLK:
			//Do nothing
			break;
		default:
			break;
		}
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		drawStatistics(hdc);
		fillRectangle(hdc,  400,  435,  405,  430 + (5 * CHART_HEIGHT / 6), color[currTypeIndex]);
		EndPaint(hWnd, &ps);
		break;
	case WM_MOUSEMOVE:
	{
		int x = GET_X_LPARAM(lParam);
		int y = GET_Y_LPARAM(lParam);

		int typeIndex = detectCoord(x, y);

		if (typeIndex != -1 && typeIndex != currTypeIndex)
		{
			currTypeIndex = typeIndex;
			showTypeLabel(currTypeIndex, hdc);
			RedrawWindow(g_hWnd, NULL, NULL, RDW_INTERNALPAINT | RDW_UPDATENOW | RDW_ERASENOW | RDW_INVALIDATE);
		}
		else
		{
			//ShowWindow(hStaticType, SW_HIDE);
		}

	}
	break;

	case WM_KEYDOWN:
	{
		if (wParam == VK_CHAR_N)
		{
			handleKeyDown(hWnd, 0);
		}
		else if (wParam == VK_CHAR_L)
		{
			handleKeyDown(hWnd, 1);
		}
	}
	break;
	case WM_DESTROY:
	{
		//
		writeListItemToFile(FILE_PATH);
		garbageCollector();
		PostQuitMessage(0);
	}
	break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}


	return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK Clear(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	HWND hParent;
	HWND hWnd;
	RECT rect;
	switch (message)
	{
	case WM_INITDIALOG:
		//Center the dialog in parent
		if ((hParent = GetParent(hDlg)) == NULL)
		{
			hParent = GetDesktopWindow();
		}

		GetWindowRect(hParent, &rect);

		SetWindowPos(hDlg,
			HWND_TOP,
			rect.left + defaultWidth / 2 - 250,
			rect.top + defaultHeight / 2 - 100,
			0, 0,
			SWP_NOSIZE);

		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			//Clear
			g_totalMoney = 0;
			g_ItemCount = 0;
			ListView_DeleteAllItems(g_hListview);
			setWindowText(hTotalMoney, 0, L"", L"");
			listItem.clear();
			RedrawWindow(g_hWnd, NULL, NULL, RDW_INTERNALPAINT | RDW_UPDATENOW | RDW_ERASENOW | RDW_INVALIDATE);
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDCANCEL)
		{
			//Do nothing
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

HWND createListView(long lExtStyle, HWND parentWnd, long ID, HINSTANCE hParentInst, int x, int y, int nWidth, int nHeight, long lStyle)
{
	//khởi tạo listview
	HWND m_hListView = CreateWindowEx(lExtStyle, WC_LISTVIEW, _T("List View"),
		WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_TABSTOP | lStyle,
		x, y, nWidth, nHeight, parentWnd, (HMENU)ID, hParentInst, NULL);

	//chèn vào listview 3 cột như đề bài
	LVCOLUMN lvCol;

	lvCol.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;

	lvCol.fmt = LVCFMT_LEFT;
	lvCol.cx = 100;
	lvCol.pszText = _T("Loại chi tiêu");
	ListView_InsertColumn(m_hListView, 0, &lvCol);

	lvCol.fmt = LVCFMT_LEFT;
	lvCol.pszText = _T("Số tiền");
	lvCol.cx = 100;
	ListView_InsertColumn(m_hListView, 1, &lvCol);

	lvCol.fmt = LVCFMT_LEFT;
	lvCol.cx = 207;
	lvCol.pszText = _T("Nội dung");
	ListView_InsertColumn(m_hListView, 2, &lvCol);

	return m_hListView;
}

bool insertItemToList(HWND m_hListview, int mItemCount)
{
	MoHinh* item = new MoHinh();
	WCHAR* buffer;
	//xác định xem mô tả chi tiêu có trống hay không và xuất thông báo yêu cầu nhập đầy đủ thông tin
	int len = GetWindowTextLength(hDescriptionInput);
	if (len > 0)
	{
		buffer = new WCHAR[len + 1];
		GetWindowText(hDescriptionInput, buffer, len + 1);
		item->mDescription = std::wstring(buffer);
	}
	else
	{
		MessageBox(g_hWnd, L"Mô tả không được để trống!", L"Thông báo", MB_ICONWARNING | MB_OK);
		return false;
	}

	//xác định xem số tiền chi tiêu có trống hay không và xuất thông báo yêu cầu nhập số tiền chi tiêu
	len = GetWindowTextLength(hMoneyInput);
	if (len > 0)
	{
		buffer = new WCHAR[len + 1];
		GetWindowText(hMoneyInput, buffer, len + 1);
		item->mMoney = _wtoi64(buffer);
	}
	else
	{
		MessageBox(g_hWnd, L"Số tiền không được để trống!", L"Thông báo", MB_ICONWARNING | MB_OK);
		return false;
	}

	buffer = new WCHAR[20];
	GetWindowText(hComboBox, buffer, 20);
	wcscpy_s(item->mType, buffer);

	listItem.push_back(item);

	g_totalMoney += item->mMoney;

	LV_ITEM lv;

	lv.mask = LVIF_TEXT | LVIF_PARAM | LVIF_IMAGE;

	lv.iItem = mItemCount;
	lv.iSubItem = 0;
	lv.pszText = item->mType;
	ListView_InsertItem(m_hListview, &lv);

	lv.mask = LVIF_TEXT;

	lv.iSubItem = 1;
	buffer = new WCHAR[20];
	wsprintf(buffer, L"%I64d", item->mMoney);
	lv.pszText = buffer;
	ListView_SetItem(m_hListview, &lv); 
	lv.iSubItem = 2;
	lv.pszText = (WCHAR*)item->mDescription.c_str();
	ListView_SetItem(m_hListview, &lv);
	setWindowText(hTotalMoney, g_totalMoney, L"", L"");

	RedrawWindow(g_hWnd, NULL, NULL, RDW_INTERNALPAINT | RDW_UPDATENOW | RDW_ERASENOW | RDW_INVALIDATE);

	return true;
}


void writeListItemToFile(std::wstring path)
{
	//mở file data lên
	const std::locale utf8_locale = std::locale(std::locale(), new std::codecvt_utf8<wchar_t>());
	std::wofstream f(path);
	f.imbue(utf8_locale);

	//đầu tiên viết tổng tiền vào trong file
	f << g_totalMoney << std::endl;

	//viết tên chi tiêu, tiền và mô tả xuống file data
	for (int i = 0; i < listItem.size(); i++)
	{
		f << std::wstring(listItem[i]->mType) << std::endl;
		f << listItem[i]->mMoney << std::endl;
		f << std::wstring(listItem[i]->mDescription) << std::endl;
	}
	f.close();
}

void loadListItemFromFile(std::wstring path)
{
	//mở file để load dữ liệu được lưu
	const std::locale utf8_locale = std::locale(std::locale(), new std::codecvt_utf8<wchar_t>());
	std::wfstream f;
	f.imbue(utf8_locale);
	f.open(path, std::ios::in);

	std::wstring buffer;
	if (f.is_open())
	{
		//đọc được tổng tiền 
		if (getline(f, buffer))
		{
			g_totalMoney = _wtoi64(buffer.c_str());
		}

		//lấy được các item
		while (getline(f, buffer))
		{
			MoHinh* item = new MoHinh();

			wcscpy_s(item->mType, buffer.c_str());
			getline(f, buffer);
			item->mMoney = _wtoi64(buffer.c_str());
			getline(f, buffer);
			item->mDescription = buffer;

			listItem.push_back(item);
		}
	}
	f.close();
}

void loadAllItemToListview(HWND m_hListview)
{
	LV_ITEM lv;
	WCHAR* buffer = new WCHAR[20];

	for (int i = 0; i < listItem.size(); i++)
	{
		lv.mask = LVIF_TEXT | LVIF_PARAM | LVIF_IMAGE;

		lv.iItem = i;
		lv.iSubItem = 0;
		lv.pszText = listItem[i]->mType;
		ListView_InsertItem(m_hListview, &lv);

		lv.mask = LVIF_TEXT;

		lv.iSubItem = 1;
		buffer = new WCHAR[20];
		wsprintf(buffer, L"%I64d", listItem[i]->mMoney);
		lv.pszText = buffer;
		ListView_SetItem(m_hListview, &lv); 

		lv.iSubItem = 2;
		lv.pszText = (WCHAR*)listItem[i]->mDescription.c_str();
		ListView_SetItem(m_hListview, &lv);
	}
}

void drawStatistics(HDC hdc)
{
	if (g_totalMoney <= 0)
	{
		fillRectangle(hdc,  + 40,  + 395,  + 40 + CHART_WIDTH,  395 + CHART_HEIGHT, color[6]);
		ShowWindow(hPercentA, SW_HIDE);
		ShowWindow(hPercentB, SW_HIDE);
		ShowWindow(hPercentC, SW_HIDE);
		ShowWindow(hPercentD, SW_HIDE);
		ShowWindow(hPercentE, SW_HIDE);
		ShowWindow(hPercentF, SW_HIDE);
		ShowWindow(hWelcome, SW_SHOW);

		currTypeIndex = 6;
		ShowWindow(hStaticType, SW_HIDE);
		return;
	}

	ShowWindow(hWelcome, SW_HIDE);

	typeMoney = new long long[6];
	for (int i = 0; i < 6; i++)
	{
		typeMoney[i] = 0;
	}

	int graphWidth = CHART_WIDTH;
	WCHAR buffer[5];
	currY =  395;
	currX[7];
	currX[0] =  40;
	float percent;

	for (int i = 0; i < listItem.size(); i++)
	{
		typeMoney[getTypeIndex(listItem[i])] += listItem[i]->mMoney;
	}


	//vẽ đối tượng ăn uống
	percent = (typeMoney[0] * 1.0 / g_totalMoney); 
	currX[1] = currX[0] + percent*graphWidth; 
	if (percent >= 0.07)
	{
		ShowWindow(hPercentA, SW_SHOW);
		MoveWindow(hPercentA, currX[0] + percent*graphWidth / 2 - 15, currY + 7, 27, 15, FALSE);
		setWindowText(hPercentA, int(percent * 100), L"", L"%");
	}
	else
	{
		ShowWindow(hPercentA, SW_HIDE);
	}

	//vẽ đối tượng di chuyển
	percent = (typeMoney[1] * 1.0 / g_totalMoney);
	currX[2] = currX[1] + percent*graphWidth;
	if (percent >= 0.07)
	{
		MoveWindow(hPercentB, currX[1] + percent*graphWidth / 2 - 15, currY + 7, 27, 15, FALSE);
		ShowWindow(hPercentB, SW_SHOW);
		setWindowText(hPercentB, int(percent * 100), L"", L"%");
	}
	else
	{
		ShowWindow(hPercentB, SW_HIDE);
	}



	//vẽ đối tương nhà cửa
	percent = (typeMoney[2] * 1.0 / g_totalMoney);
	currX[3] = currX[2] + percent*graphWidth;
	if (percent >= 0.07)
	{
		ShowWindow(hPercentC, SW_SHOW);
		MoveWindow(hPercentC, currX[2] + percent*graphWidth / 2 - 15, currY + 7, 27, 15, FALSE);
		setWindowText(hPercentC, int(percent * 100), L"", L"%");
	}
	else
	{
		ShowWindow(hPercentC, SW_HIDE);
	}


	//vẽ đối tương xe cộ
	percent = (typeMoney[3] * 1.0 / g_totalMoney);
	currX[4] = currX[3] + percent*graphWidth;
	if (percent >= 0.07)
	{
		MoveWindow(hPercentD, currX[3] + percent*graphWidth / 2 - 15, currY + 7, 27, 15, FALSE);
		ShowWindow(hPercentD, SW_SHOW);
		setWindowText(hPercentD, int(percent * 100), L"", L"%");
	}
	else
	{
		ShowWindow(hPercentD, SW_HIDE);
	}


	//vẽ đối tượng nhu yếu phẩm
	percent = (typeMoney[4] * 1.0 / g_totalMoney);
	currX[5] = currX[4] + percent*graphWidth;
	if (percent >= 0.07)
	{
		MoveWindow(hPercentE, currX[4] + percent*graphWidth / 2 - 15, currY + 7, 27, 15, FALSE);
		ShowWindow(hPercentE, SW_SHOW);
		setWindowText(hPercentE, int(percent * 100), L"", L"%");
	}
	else
	{
		ShowWindow(hPercentE, SW_HIDE);
	}


	//vẽ đối tượng dịch vụ
	percent = (typeMoney[5] * 1.0 / g_totalMoney);
	currX[6] = currX[5] + percent*graphWidth;
	if (percent >= 0.07)
	{
		MoveWindow(hPercentF, currX[5] + percent*graphWidth / 2 - 15, currY + 7, 27, 15, FALSE);
		ShowWindow(hPercentF, SW_SHOW);
		setWindowText(hPercentF, int(percent * 100), L"", L"%");
	}
	else
	{
		ShowWindow(hPercentF, SW_HIDE);
	}

	//điền đầy đủ vào biểu đồ hình chữ nhật
	for (int i = 0; i < 6; i++)
	{
		fillRectangle(hdc, currX[i], currY, currX[i + 1], currY + CHART_HEIGHT, color[i]);
	}
}


void drawLine(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color)
{
	HPEN hpen = CreatePen(PS_SOLID, 10, color);
	SelectObject(hdc, hpen);
	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);
}

void fillRectangle(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color)
{
	RECT* rect = new RECT;
	rect->left = x1;
	rect->top = y1;
	rect->right = x2;
	rect->bottom = y2;
	HBRUSH hbrush = CreateSolidBrush(color);

	//Fill
	FillRect(hdc, rect, hbrush);
}

void setWindowText(HWND hWnd, long long value, std::wstring textBefore, std::wstring textAfter)
{
	WCHAR buffer[255];
	if (value < 10)
	{
		wsprintf(buffer, L"0%I64d", value);
	}
	else
	{
		wsprintf(buffer, L"%I64d", value);
	}

	SetWindowText(hWnd, (textBefore + std::wstring(buffer) + textAfter).c_str());
}

int getTypeIndex(MoHinh* item)
{
	if (wcscmp(item->mType, L"Ăn uống") == 0)
	{
		return 0;
	}
	else if (wcscmp(item->mType, L"Di chuyển") == 0)
	{
		return 1;
	}
	else if (wcscmp(item->mType, L"Nhà cửa") == 0)
	{
		return 2;
	}
	else if (wcscmp(item->mType, L"Xe cộ") == 0)
	{
		return 3;
	}
	else if (wcscmp(item->mType, L"Nhu yếu phẩm") == 0)
	{
		return 4;
	}
	else
	{
		return 5;
	}
}

void enableInputWindow()
{
	EnableWindow(hComboBox, true);
	EnableWindow(hMoneyInput, true);
	EnableWindow(hDescriptionInput, true);
	SetWindowText(hAddButton, L"THÊM");
	ShowWindow(hClose, SW_SHOW);

}



int detectCoord(int x, int y)
{
	if (g_totalMoney <= 0)
	{
		return -1;
	}
	if (y > currY && y < currY + CHART_HEIGHT)
	{
		if (x > currX[0] && x < currX[1])
		{
			return 0;
		}
		else if (x > currX[1] && x < currX[2])
		{
			return 1;
		}
		else if (x > currX[2] && x < currX[3])
		{
			return 2;
		}
		else if (x > currX[3] && x < currX[4])
		{
			return 3;
		}
		else if (x > currX[4] && x < currX[5])
		{
			return 4;
		}
		else if (x > currX[5] && x < currX[6])
		{
			return 5;
		}
	}

	return -1;
}

void showTypeLabel(int index, HDC hdc)
{
	std::wstring ws = std::to_wstring(typeMoney[index]);
	switch (index)
	{
	case 0:
		ws = L"Ăn Uống (Sáng, trưa, chiều, tối,..): " + ws;
		break;
	case 1:
		ws = L"Di Chuyển (Bus, đổ xăng, taxi,..): " + ws;
		break;
	case 2:
		ws = L"Nhà Cửa (Tiền thuê nhà, nước, điện,..): " + ws;
		break;
	case 3:
		ws = L"Xe Cộ (Đổ xăng, vá xe, thay nhớt,..): " + ws;
		break;
	case 4:
		ws = L"Nhu Yếu Phẩm (Xà bông, sữa tắm,..): " + ws;
		break;
	case 5:
		ws = L"Dịch Vụ (Internet, thẻ cào điện thoại,..): " + ws;
		break;
	default:
		break;
	}

	ShowWindow(hStaticType, SW_SHOW);
	SetWindowText(hStaticType, ws.c_str());
}

void garbageCollector()
{
	for (int i = 0; i < listItem.size(); i++)
	{
		delete listItem[i];
	}
}

void addItemProc(HWND m_hListview)
{

	if (isAdding)
	{
		if (insertItemToList(m_hListview, g_ItemCount))
		{
			g_ItemCount++;
			isAdding = true;
			enableInputWindow();
		}
	}
	else
	{
		isAdding = true;
		enableInputWindow();
	}
}

void handleKeyDown(HWND hWnd, int type)
{
	//0: Ctrl-N
	//1: Ctrl-L

	if (type == 0)
	{
		int ctrl = GetKeyState(VK_LCONTROL);
		if (ctrl & 0x8000) { // 1: DOWN, 0: UP
							 //Ctrl + N
			addItemProc(g_hListview);
		}
	}
	else if (type == 1)
	{
		//Handle Ctrl+L event to clear list
		int ctrl = GetKeyState(VK_LCONTROL);
		if (ctrl & 0x8000) { // 1: DOWN, 0: UP
							 //Ctrl + N
			DialogBox(hInst, MAKEINTRESOURCE(IDD_CLEAR_DIALOG), hWnd, Clear);
		}
	}
}

//hàm phân tích yêu cầu
void theParser()
{
	MoHinh* item = new MoHinh();
	WCHAR* buffer;
	long long tempmoney = 0;
	int len = GetWindowTextLength(hMoneyInputMonth);
	if (len > 0)
	{
		buffer = new WCHAR[len + 1];
		GetWindowText(hMoneyInput, buffer, len + 1);
		item->mMoney = _wtoi64(buffer);
		tempmoney = g_totalMoney-item->mMoney;
		if (tempmoney > 0)
		{
			MessageBox(g_hWnd, L"Tháng này số tiền chi nhiều hơn tháng trước. Cần kiểm tra lại thông tin chi tiêu cho chính xác!", L"Thông báo", MB_ICONWARNING | MB_OK);
		}
		else
		{
			MessageBox(g_hWnd, L"Tháng này chi tiêu có tiết kiệm hơn tháng trước. Cần phát huy để có thể mua được nhiều thứ hơn!", L"Thông báo", MB_ICONWARNING | MB_OK);
		}
		
	}
	else
	{
		MessageBox(g_hWnd, L"Số tiền không được để trống!", L"Thông báo", MB_ICONWARNING | MB_OK);
		//return false;
	}
}