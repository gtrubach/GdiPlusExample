// GdiPlusExample.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "GdiPlusExample.h"

using namespace Gdiplus;

#define MAX_LOADSTRING 100

#define NAVY_BLUE RGB(0, 0, 128)
#define WHITE RGB(255, 255, 255)
#define YELLOW RGB(255, 255, 0)

struct Params
{
	std::wstring m_text;
	CHOOSEFONT m_cf;
	LOGFONT m_lf;
	CHOOSECOLOR m_ccCircuit;
	CHOOSECOLOR m_ccFill;
	CHOOSECOLOR m_ccBackground;
	Gdiplus::PointF m_pStart;
	double m_dRotAngle;
	float m_fScale;
	bool m_bSmoothing;
	bool m_bNonRectRg;

	COLORREF m_crCustColors[16];

	Params() :m_text(L"Text"),m_pStart(0,0), m_dRotAngle(0), m_fScale(1), m_bSmoothing(true), m_bNonRectRg(false)
	{
		ZeroMemory(&m_lf, sizeof(m_lf));
		m_lf.lfHeight = -64;//-MulDiv(PointSize, GetDeviceCaps(hDC, LOGPIXELSY), 72);
		wcscpy_s(m_lf.lfFaceName, L"Arial");

		ZeroMemory(&m_cf, sizeof(m_cf));
		m_cf.lStructSize = sizeof(CHOOSEFONT);
		m_cf.lpLogFont = &m_lf;
		m_cf.Flags = CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS;

		ZeroMemory(&m_ccCircuit, sizeof(m_ccCircuit));
		ZeroMemory(&m_ccFill, sizeof(m_ccFill));
		ZeroMemory(&m_ccBackground, sizeof(m_ccBackground));
		m_ccCircuit.lStructSize = m_ccFill.lStructSize = m_ccBackground.lStructSize = sizeof(CHOOSECOLOR);
		m_ccCircuit.Flags = m_ccFill.Flags = m_ccBackground.Flags = CC_RGBINIT | CC_FULLOPEN;
		m_ccCircuit.lpCustColors = m_ccFill.lpCustColors = m_ccBackground.lpCustColors = m_crCustColors;

		m_ccCircuit.rgbResult = NAVY_BLUE;
		m_ccFill.rgbResult = WHITE;
		m_ccBackground.rgbResult = YELLOW;
	}

	void SetHWNDOwner(HWND hWnd)
	{
		m_cf.hwndOwner = m_ccCircuit.hwndOwner = m_ccFill.hwndOwner = m_ccBackground.hwndOwner = hWnd;
	}
} params;

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    TextParams(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_GDIPLUSEXAMPLE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
	Status st = GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	if (st != Ok)
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GDIPLUSEXAMPLE));

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

	GdiplusShutdown(gdiplusToken);

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

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GDIPLUSEXAMPLE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_GDIPLUSEXAMPLE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
   
   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   params.SetHWNDOwner(hWnd);

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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
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
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
			{
				Graphics graphics(hdc);
				HFONT fnIndirect = CreateFontIndirect(params.m_cf.lpLogFont);
				Font font(hdc, fnIndirect);
				LinearGradientBrush brush(Rect(0, 0, 100, 100), Color::Red, Color::Yellow, LinearGradientModeHorizontal);
				graphics.DrawString(params.m_text.c_str(), params.m_text.length(), &font, params.m_pStart, &brush);
			}
			EndPaint(hWnd, &ps);
        }
        break;
	case WM_LBUTTONDOWN:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_TEXT_PARAMS), hWnd, TextParams);
		InvalidateRect(hWnd, NULL, TRUE);
		break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
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

// Message handler for text params box.
INT_PTR CALLBACK TextParams(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		SetDlgItemText(hDlg, IDC_EDIT_TEXT, params.m_text.c_str());
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON_FONT:
			ChooseFont(&params.m_cf);
			break;

		case IDC_BUTTON_COLOR_CIRCUIT:
			ChooseColor(&params.m_ccCircuit);
			break;

		case IDC_BUTTON_COLOR_FILL:
			ChooseColor(&params.m_ccFill);
			break;

		case IDC_BUTTON_COLOR_BKG:
			ChooseColor(&params.m_ccBackground);
			break;

		case IDOK:
			wchar_t textChar[1024];
			GetDlgItemText(hDlg, IDC_EDIT_TEXT, textChar, 1024);
			params.m_text = textChar;
		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
