#include <windows.h>
#include <stdbool.h>
#include <stdint.h>

static bool quit = false;

static BITMAPINFO bitmapInfo;
static HBITMAP bitmap;
static HDC context;

struct renderTarget
{
	int width;
	int height;

	uint32_t* data;
};

static struct renderTarget frame = { 0 };

LRESULT CALLBACK WindowProcessMessage(HWND hWindow, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_QUIT:
		case WM_DESTROY:
		{
			quit = true;
			break;
		}

		case WM_PAINT:
		{
			static PAINTSTRUCT paint;
			static HDC windowContext;

			windowContext = BeginPaint(hWindow, &paint);

			BitBlt(
				windowContext,

				paint.rcPaint.left,
				paint.rcPaint.top,
				paint.rcPaint.right - paint.rcPaint.left,
				paint.rcPaint.bottom - paint.rcPaint.top,

				context,

				paint.rcPaint.left,
				paint.rcPaint.top,

				SRCCOPY
			);

			EndPaint(hWindow, &paint);

			break;
		}

		case WM_SIZE:
		{
			bitmapInfo.bmiHeader.biWidth = LOWORD(lParam);
			bitmapInfo.bmiHeader.biHeight = HIWORD(lParam);

			if (bitmap)
				DeleteObject(bitmap);

			bitmap = CreateDIBSection(
				NULL,
				&bitmapInfo,
				DIB_RGB_COLORS,
				&frame.data,
				0,
				0
			);

			SelectObject(context, bitmap);

			frame.width = LOWORD(lParam);
			frame.height = HIWORD(lParam);

			break;
		}

		default:
			return DefWindowProc(hWindow, uMsg, wParam, lParam);
	}

	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hLast, PSTR pCmd, int nShow)
{
	// Setup window class
	static WNDCLASS window = { 0 };
	static const wchar_t windowName[] = L"awesome_class";

	window.lpszClassName = (PCSTR)windowName;
	window.lpfnWndProc = WindowProcessMessage;
	window.hInstance = hInstance;

	if (RegisterClass(&window) == 0)
		return -1;

	// Create window
	HWND hWindow = CreateWindow(
		(PCSTR)windowName,
		"The Window title I think", // Title

		WS_OVERLAPPEDWINDOW, // Type

		CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, // X, Y, W, H

		NULL, // Parent
		NULL, // Menu
		hInstance, // Instance
		NULL // Additional data
	);

	if (!hWindow)
		return -1;

	// Setup GDI
	bitmapInfo.bmiHeader.biSize = sizeof(bitmapInfo.bmiHeader);
	bitmapInfo.bmiHeader.biPlanes = 1;
	bitmapInfo.bmiHeader.biBitCount = 32;
	bitmapInfo.bmiHeader.biCompression = BI_RGB; // Uncompressed RGB

	context = CreateCompatibleDC(0);

	ShowWindow(hWindow, nShow);

	// Process inputs
	while (!quit)
	{
		static MSG message = { 0 };

		while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		// Draw random garbage
		static unsigned int pixel = 0;
		int aspect = (frame.width * frame.height);

		frame.data[(pixel++) % aspect] = rand();
		frame.data[rand() % aspect] = 0;

		InvalidateRect(hWindow, NULL, FALSE);
		UpdateWindow(hWindow);
	}

	return 0;
}
