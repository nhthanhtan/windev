// 1512488_GDIPlusRibbon.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "1512488_GDIPlusRibbon.h"


// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

												// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	CoInitialize(NULL);
	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_1512488GDIPLUSRIBBON, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_1512488GDIPLUSRIBBON));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	CoUninitialize();
	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_1512488GDIPLUSRIBBON);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
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
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, L"Paint Using GDI+", WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

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
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
int X_BD ,Y_BD;
int X_KT,Y_KT;
bool Dr = false;
int TypeHinh = 0 ,TypeBut = 1;

typedef struct CSharp
{
	int Trai;
	int Tren, Rong ,Dai ,L ,LBut;
};
std::vector<CSharp> hinh;

GdiplusStartupInput gdi;
ULONG_PTR           gdiToken;
STDMETHODIMP CCommandHandler::Execute(
	UINT nCmdID,
	UI_EXECUTIONVERB verb,
	const PROPERTYKEY* key,
	const PROPVARIANT* ppropvarValue,
	IUISimplePropertySet* pCommandExecutionProperties)
{
	UNREFERENCED_PARAMETER(pCommandExecutionProperties);
	UNREFERENCED_PARAMETER(ppropvarValue);
	UNREFERENCED_PARAMETER(key);
	UNREFERENCED_PARAMETER(verb);
	UNREFERENCED_PARAMETER(nCmdID);
	HRESULT hresult;
	HWND hwnd = GetForegroundWindow();
	switch (nCmdID)
	{
	case ID_LINE:
		TypeHinh = 0;
		break;
	case ID_RECTANGLE:
		TypeHinh = 1;
		break;
	case ID_ELLIPSE:
		TypeHinh = 2;
	case ID_SIZE_1:
		TypeBut = 1;
		break;
	case ID_SIZE_2:
		TypeBut = 2;
		break;
	case ID_SIZE_3:
		TypeBut = 3;
		break;
	}
	return S_OK;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_LBUTTONDOWN:
	{
		int a, b;
		a = GET_X_LPARAM(lParam);
		b = GET_Y_LPARAM(lParam);
		X_BD = a;
		Y_BD = b;
		Dr = true;
	}
	break;

	case WM_MOUSEMOVE: 
	{
		int a;
		int b;
		a = GET_X_LPARAM(lParam);
		b = GET_Y_LPARAM(lParam);

		if (Dr) {
			X_KT = a;
			Y_KT = b;
			InvalidateRect(hWnd, NULL, TRUE);
		}
	}
	break;
	case WM_LBUTTONUP: 
	{
		int a;
		int b;
		a = GET_X_LPARAM(lParam);
		b = GET_Y_LPARAM(lParam);
		CSharp tam;

		if (TypeHinh == 0)
		{
			tam.Trai = X_BD;
			tam.Tren = Y_BD;
			tam.Rong = X_KT;
			tam.Dai = Y_KT;
			tam.L = 0;
		}
		if (TypeHinh == 1)
		{
			tam.L = 1;
			if (GetKeyState(VK_SHIFT) & 0x8000)
			{
				if ((X_BD < X_KT) && (Y_BD < Y_KT))
				{
					tam.Trai = X_BD;
					tam.Tren = Y_BD;
					tam.Rong = X_KT - X_BD;
					tam.Dai = X_KT - X_BD;
				}
				if ((X_BD > X_KT) && (Y_BD < Y_KT))
				{
					tam.Trai = X_KT;
					tam.Tren = Y_BD;
					tam.Rong = X_BD - X_KT;
					tam.Dai = X_BD - X_KT;
				}
				if ((X_BD > X_KT) && (Y_BD > Y_KT))
				{
					tam.Trai = X_BD - Y_BD + Y_KT;
					tam.Tren = Y_KT;
					tam.Rong = Y_BD - Y_KT;
					tam.Dai = Y_BD - Y_KT;
				}
				if ((X_BD < X_KT) && (Y_BD > Y_KT))
				{
					tam.Trai = X_BD;
					tam.Tren = Y_KT;
					tam.Rong = Y_BD - Y_KT;
					tam.Dai = Y_BD - Y_KT;
				}
			}
			else
			{
				if ((X_BD < X_KT) && (Y_BD < Y_KT)){
					tam.Trai = X_BD;
					tam.Tren = Y_BD;
					tam.Rong = X_KT - X_BD;
					tam.Dai = Y_KT - Y_BD;
				}
				if ((X_BD > X_KT) && (Y_BD < Y_KT)){
					tam.Trai = X_KT;
					tam.Tren = Y_BD;
					tam.Rong = X_BD - X_KT;
					tam.Dai = Y_KT - Y_BD;
				}
				if ((X_BD > X_KT) && (Y_BD > Y_KT)){
					tam.Trai = X_KT;
					tam.Tren = Y_KT;
					tam.Rong = X_BD - X_KT;
					tam.Dai = Y_BD - Y_KT;
				}
				if ((X_BD < X_KT) && (Y_BD > Y_KT)){
					tam.Trai = X_BD;
					tam.Tren = Y_KT;
					tam.Rong = X_KT - X_BD;
					tam.Dai = Y_BD - Y_KT;
				}
			}
		}
		if (TypeHinh == 2)
		{
			tam.L = 2;
			tam.Trai = X_BD;
			tam.Tren = Y_BD;
			tam.Rong = X_KT - X_BD;
			if (GetKeyState(VK_SHIFT) & 0x8000)
			{
				if ((X_BD > X_KT && Y_BD < Y_KT) || (X_BD < X_KT && Y_BD > Y_KT))
					tam.Dai = X_BD - X_KT;
				if ((X_BD < X_KT && Y_BD < Y_KT) || (X_BD > X_KT && Y_BD > Y_KT))
					tam.Dai = X_KT - X_BD;
			}
			else
				tam.Dai = Y_KT - Y_BD;
		}
		if (TypeBut == 1)
			tam.LBut = 1;
		if (TypeBut == 2)
			tam.LBut = 2;
		if (TypeBut == 3)
			tam.LBut = 3;
		hinh.push_back(tam);

		Dr = false;
		InvalidateRect(hWnd, NULL, TRUE);
	}
					   break;
	case WM_CREATE:
		InitializeFramework(hWnd);
		GdiplusStartup(&gdiToken, &gdi, NULL);
		break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT paintstruct;
		HDC hdc = BeginPaint(hWnd, &paintstruct);
		for (int i = 0; i < hinh.size(); i++)
		{
			Pen* but = new Pen(Color(255, 0, 0, 0), hinh[i].LBut);
			Graphics* grap = new Graphics(hdc);
			if (hinh[i].L == 0)
				grap->DrawLine(but, hinh[i].Trai, hinh[i].Tren, hinh[i].Rong, hinh[i].Dai);

			if (hinh[i].L == 1)
				grap->DrawRectangle(but, hinh[i].Trai, hinh[i].Tren, hinh[i].Rong, hinh[i].Dai);

			if (hinh[i].L == 2)
				grap->DrawEllipse(but, hinh[i].Trai, hinh[i].Tren, hinh[i].Rong, hinh[i].Dai);
			delete but;
			delete grap;
		}
		if (Dr) {
			Pen* but1 = new Pen(Color(255, 0, 0, 0), TypeBut);
			Graphics* grap1 = new Graphics(hdc);
			if (TypeHinh == 0)
				grap1->DrawLine(but1, X_BD, Y_BD, X_KT, Y_KT);
			if (TypeHinh == 1)
			{
				if (GetKeyState(VK_SHIFT) & 0x8000)
				{
					if ((X_BD < X_KT) && (Y_BD < Y_KT))
						grap1->DrawRectangle(but1, X_BD, Y_BD, X_KT - X_BD, X_KT - X_BD);
					if ((X_BD > X_KT) && (Y_BD < Y_KT))
						grap1->DrawRectangle(but1, X_KT, Y_BD, X_BD - X_KT, X_BD - X_KT);
					if ((X_BD > X_KT) && (Y_BD > Y_KT))
						grap1->DrawRectangle(but1, X_BD - Y_BD + Y_KT, Y_KT, Y_BD - Y_KT, Y_BD - Y_KT);
					if ((X_BD < X_KT) && (Y_BD > Y_KT))
						grap1->DrawRectangle(but1, X_BD, Y_KT, Y_BD - Y_KT, Y_BD - Y_KT);
				}
				else
				{
					if ((X_BD < X_KT) && (Y_BD < Y_KT))
						grap1->DrawRectangle(but1, X_BD, Y_BD, X_KT - X_BD, Y_KT - Y_BD);
					if ((X_BD > X_KT) && (Y_BD < Y_KT))
						grap1->DrawRectangle(but1, X_KT, Y_BD, X_BD - X_KT, Y_KT - Y_BD);
					if ((X_BD > X_KT) && (Y_BD > Y_KT))
						grap1->DrawRectangle(but1, X_KT, Y_KT, X_BD - X_KT, Y_BD - Y_KT);
					if ((X_BD < X_KT) && (Y_BD > Y_KT))
						grap1->DrawRectangle(but1, X_BD, Y_KT, X_KT - X_BD, Y_BD - Y_KT);
				}
			}
			if (TypeHinh == 2)
			{
				if (GetKeyState(VK_SHIFT) & 0x8000)
				{
					if ((X_BD > X_KT && Y_BD < Y_KT) || (X_BD < X_KT && Y_BD > Y_KT))
						grap1->DrawEllipse(but1, X_BD, Y_BD, X_KT - X_BD, X_BD - X_KT);

					if ((X_BD < X_KT && Y_BD < Y_KT) || (X_BD > X_KT && Y_BD > Y_KT))
						grap1->DrawEllipse(but1, X_BD, Y_BD, X_KT - X_BD, X_KT - X_BD);
				}
				else
					grap1->DrawEllipse(but1, X_BD, Y_BD, X_KT - X_BD, Y_KT - Y_BD);
			}
			delete but1;
			delete grap1;
		}
		EndPaint(hWnd, &paintstruct);
	}
	break;
	case WM_DESTROY:
		DestroyFramework();
		PostQuitMessage(0);
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
