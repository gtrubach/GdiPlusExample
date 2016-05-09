// GdiPlusExample.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "GdiPlusExample.h"

using namespace Gdiplus;

#define MAX_LOADSTRING 100

#define NAVY_BLUE RGB(0, 0, 128)
#define WHITE RGB(255, 255, 255)
#define YELLOW RGB(255, 255, 0)

#define MAX_BUFFER_SIZE 1024

struct Params
{
	std::wstring m_text;
	CHOOSEFONT m_cf;
	LOGFONT m_lf;
	CHOOSECOLOR m_ccCircuit;
	CHOOSECOLOR m_ccFill;
	CHOOSECOLOR m_ccBackground;
	Gdiplus::PointF m_pStart;
	float m_fRotAngle;
	float m_fScale;
	BOOL m_bAntialiasing;
	BOOL m_bNonRectRg;

	COLORREF m_crCustColors[16];

	Params() :m_text(L"Text"), m_pStart(0.0f, 0.0f), m_fRotAngle(0.0f), m_fScale(1.0f), m_bAntialiasing(true), m_bNonRectRg(false)
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
	Status status = GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	if (status != Ok)
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

				Color color;
				color.SetFromCOLORREF(params.m_ccBackground.rgbResult);
				graphics.Clear(color);

				HFONT fnIndirect = CreateFontIndirect(params.m_cf.lpLogFont);
				Font font(hdc, fnIndirect);
				if (params.m_bAntialiasing)
					graphics.SetSmoothingMode(SmoothingModeHighQuality);

				graphics.ScaleTransform(params.m_fScale, params.m_fScale);
				//graphics.RotateTransform(params.m_fRotAngle);

				StringFormat strformat;
				GraphicsPath path;
				FontFamily fnFamily;
				font.GetFamily(&fnFamily);
				path.AddString(params.m_text.c_str(), params.m_text.length(), &fnFamily, font.GetStyle(), font.GetSize(), params.m_pStart, &strformat);

				color.SetFromCOLORREF(params.m_ccCircuit.rgbResult);
				Pen pen(color, 6);
				pen.SetLineJoin(LineJoinRound);

				graphics.DrawPath(&pen, &path);

				color.SetFromCOLORREF(params.m_ccFill.rgbResult);
				SolidBrush brush(color);
				graphics.FillPath(&brush, &path);
			}
			EndPaint(hWnd, &ps);
        }
        break;
	case WM_LBUTTONDOWN:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_TEXT_PARAMS), hWnd, TextParams);
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_ERASEBKGND:
		return TRUE;
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
		{
			SetDlgItemText(hDlg, IDC_EDIT_TEXT, params.m_text.c_str());
			SetDlgItemInt(hDlg, IDC_EDIT_POINT_X, (UINT)params.m_pStart.X, FALSE);
			SetDlgItemInt(hDlg, IDC_EDIT_POINT_Y, (UINT)params.m_pStart.Y, FALSE);

			std::wstringstream wss;
			wss << params.m_fRotAngle;
			SetDlgItemText(hDlg, IDC_EDIT_ROTATE, wss.str().c_str());

			wss.str(L"");
			wss << params.m_fScale;
			SetDlgItemText(hDlg, IDC_EDIT_SCALE, wss.str().c_str());

			CheckDlgButton(hDlg, IDC_CHECK_ANTIALIASING, params.m_bAntialiasing);
			CheckDlgButton(hDlg, IDC_CHECK_NONSTANDARDRG, params.m_bNonRectRg);
		}
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
			{
				wchar_t textChar[MAX_BUFFER_SIZE];
				GetDlgItemText(hDlg, IDC_EDIT_TEXT, textChar, MAX_BUFFER_SIZE);
				params.m_text = textChar;

				BOOL bX, bY;
				REAL x = (REAL)GetDlgItemInt(hDlg, IDC_EDIT_POINT_X, &bX, FALSE);
				REAL y = (REAL)GetDlgItemInt(hDlg, IDC_EDIT_POINT_Y, &bY, FALSE);
				if (!bX)
					x = params.m_pStart.X;
				if (!bY)
					y = params.m_pStart.Y;
				params.m_pStart.X = x;
				params.m_pStart.Y = y;

				GetDlgItemText(hDlg, IDC_EDIT_ROTATE, textChar, MAX_BUFFER_SIZE);
				try
				{
					params.m_fRotAngle = std::stof(textChar);
				}
				catch (std::invalid_argument)
				{
					params.m_fRotAngle = 0;
				}

				GetDlgItemText(hDlg, IDC_EDIT_SCALE, textChar, MAX_BUFFER_SIZE);
				try
				{
					params.m_fScale = std::stof(textChar);
				}
				catch (std::invalid_argument)
				{
					params.m_fScale = 0;
				}

				params.m_bAntialiasing = IsDlgButtonChecked(hDlg, IDC_CHECK_ANTIALIASING);
				params.m_bNonRectRg = IsDlgButtonChecked(hDlg, IDC_CHECK_NONSTANDARDRG);
			}
		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
