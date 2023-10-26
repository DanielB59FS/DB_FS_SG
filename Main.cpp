// Main.cpp
// Program entry point and main control is here.
// Adapted for SWA course use by Nick Penney and Derek Bliss
// Full Sail University
// Last updated February 28, 2012
//
#include "pch.h"
#include "EntryState.h"
#include "MenuState.h"
#include "ShopState.h"
#include "LevelState.h"
#include "ExitState.h"
#include "View\ViewManager.h"



//////////////////////////////////////////////////////////////////////////////
// File Scope Globals
namespace {
	DEVMODE		gDevMode;
	EntryState	entryState;
	MenuState	menuState;
	LevelState	levelState;
#ifdef BUGS
	ShopState	shopState;
	ExitState	exitState;
#endif
	AbstractState	*currentState 	= NULL;		// Current state of application
	HINSTANCE		ghInstance		= NULL;		// Application instance
	HWND			ghMainWnd		= NULL;		// Main Window (window manager)
}

//////////////////////////////////////////////////////////////////////////////
// Forward  and external Declarations
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void UpdateApplication();
void TransitionState(unsigned int _transition);

//////////////////////////////////////////////////////////////////////////////
// Program Entry Point
//////////////////////////////////////////////////////////////////////////////
int APIENTRY WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPSTR     lpCmdLine,
					 int       nCmdShow)
{
	/////////////////////////////////////////////
	// Do any program wide Initialization here
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	/////////////////////////////////////////////
	// Create Main Window. 
	WNDCLASSEX wcex;
	wcex.cbSize			= sizeof(WNDCLASSEX); 
	wcex.style			= CS_OWNDC;
	wcex.lpfnWndProc	= (WNDPROC)MainWndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= NULL;
	wcex.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= WINDOW_TITLE;
	wcex.hIconSm		= NULL;

	if(RegisterClassEx(&wcex) == 0)
		return -1;

	// Select window styles
	DWORD uiStyle= WS_POPUP | WS_SYSMENU, uiStyleX= NULL;

	// Create the main 3D window
	ghMainWnd = CreateWindowEx(uiStyleX, wcex.lpszClassName, WINDOW_TITLE, uiStyle,
		(GetSystemMetrics(SM_CXSCREEN)>>1) - (WINDOW_WIDTH>>1), 
		(GetSystemMetrics(SM_CYSCREEN)>>1) - (WINDOW_HEIGHT>>1), 
		WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);

	if (!ghMainWnd)
		return -1;

	// Make sure window manager stays hidden
	ShowCursor(NULL);
	ShowWindow(ghMainWnd, SW_SHOW);
	UpdateWindow(ghMainWnd);
	SetFocus(ghMainWnd);

	/////////////////////////////////////////////
	// Message Pump - Use the form that goes idle and waits for 
	// messages, not continually running.
	MSG msg;
	do
	{	
		if(GetMessage(&msg, NULL, 0, 0)) 
		{
			if(!TranslateMessage(&msg)) 
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		if(ghMainWnd)
		{
			UpdateApplication();
			if(currentState)
				currentState->RenderFrame();
		}
	}
	while(msg.message != WM_QUIT); 

	// Return termination code
	return (int)msg.wParam;
}


/////////////////////////////////////////////////////////////////////
// Main window message procedure. This is the window manager for
// the application
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, 
							 WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		// Window is created, the application is starting
	case WM_CREATE:
		// Initialize state
		TransitionState(ENTRY);
		break;

		// Window is destroyed, it's the end of the application
	case WM_DESTROY:
		// Cleanup
		if(currentState)
			currentState->Shutdown();
		currentState = 0;
		SaiphApp::CleanUp();
		ViewManager::GetInstance().Shutdown();
		ghMainWnd = NULL;
		ShowCursor(TRUE);
		PostQuitMessage(0);
		break;

	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// Use an accumulator to mediate time differences between frames
void UpdateApplication()
{
	static StopWatch timer;
	static float accumulator = 0.0f;
	const static float timestep = 0.01f;
	static unsigned int result;

	accumulator += timer.GetElapsedSeconds();
	timer.Reset();

	if(accumulator > 0.05f)
		accumulator = 0.05f;

	while(accumulator >= timestep)
	{
		// Update the Model
		if(currentState)
			TransitionState(currentState->Heartbeat(timestep));
		accumulator -= timestep;
	}

	// Let the View know we have new data to paint
	InvalidateRect(ghMainWnd, NULL, FALSE);
}

void TransitionState(unsigned int _transition)
{
	if(_transition == NOCHANGE)
		return;

	if(currentState)
	{
		currentState->Shutdown();
		currentState = NULL;
	}

	switch(_transition)
	{
	case ENTRY:
		currentState = &entryState;
		break;

	case SHOP:
#ifdef BUGS
		currentState = &shopState;
		break;
#endif
	case MENU:
		currentState = &menuState;
		break;
	case LEVEL:
		currentState = &levelState;
		break;
	case EXIT:
#ifdef BUGS
		currentState = &exitState;
		break;
#endif
	default:
		DestroyWindow(ghMainWnd);
		return;
	};

	if(currentState)
		currentState->Init();
}