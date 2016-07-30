#ifdef WIN32

#include "device/spiderWin32Display.h"


namespace spider3d
{
	typedef struct DisplayMap
	{
		SpiderWin32Display* display;
	}DisplayMap;

	DisplayMap displayMap;

	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL 0x020A
#endif
#ifndef WHEEL_DELTA
#define WHEEL_DELTA 120
#endif

		Event sEvent;
        Win32DeviceContext* display = 0;
		static su32 ClickCount=0;

		if (GetCapture() != hWnd && ClickCount > 0)
			ClickCount = 0;

		switch (message)
		{

		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

		case WM_MOUSEWHEEL:
			sEvent.eventType = SMOUSE_EVENT;
			sEvent.mouseInput.Wheel = ((sf32)((short)HIWORD(wParam))) / (sf32)WHEEL_DELTA;
			sEvent.mouseInput.Event = SMOUSE_WHEEL;

			POINT p; 
			p.x = 0; p.y = 0;
			ClientToScreen(hWnd, &p);
			sEvent.mouseInput.X = LOWORD(lParam) - p.x;
			sEvent.mouseInput.Y = HIWORD(lParam) - p.y;
			display = displayMap.display;
			if( display )
			display->postEvent(sEvent);

			break;

		case WM_LBUTTONDOWN:
			ClickCount++;
			SetCapture(hWnd);
			sEvent.eventType = SMOUSE_EVENT;
			sEvent.mouseInput.Event = LEFT_MOUSE_PRESSED_DOWN;
			sEvent.mouseInput.X = (short)LOWORD(lParam);
			sEvent.mouseInput.Y = (short)HIWORD(lParam);
			display = displayMap.display;
			if( display )
			display->postEvent(sEvent);
			return 0;

		case WM_LBUTTONUP:
			ClickCount--;
			if (ClickCount<1)
			{
			ClickCount=0;
			ReleaseCapture();
			}
			sEvent.eventType = SMOUSE_EVENT;
			sEvent.mouseInput.Event = LEFT_MOUSE_LEFT_UP;
			sEvent.mouseInput.X = (short)LOWORD(lParam);
			sEvent.mouseInput.Y = (short)HIWORD(lParam);
			display = displayMap.display;
			if( display )
			display->postEvent(sEvent);
			return 0;

		case WM_RBUTTONDOWN:
			ClickCount++;
			SetCapture(hWnd);
			sEvent.eventType = SMOUSE_EVENT;
			sEvent.mouseInput.Event = RIGHT_MOUSE_PRESSED_DOWN;
			sEvent.mouseInput.X = (short)LOWORD(lParam);
			sEvent.mouseInput.Y = (short)HIWORD(lParam);
			display = displayMap.display;
			if( display )
			display->postEvent(sEvent);
			return 0;

		case WM_RBUTTONUP:
			ClickCount--;
			if (ClickCount<1)
			{
			ClickCount=0;
			ReleaseCapture();
			}
			sEvent.eventType = SMOUSE_EVENT;
			sEvent.mouseInput.Event = RIGHT_MOUSE_LEFT_UP;
			sEvent.mouseInput.X = (short)LOWORD(lParam);
			sEvent.mouseInput.Y = (short)HIWORD(lParam);
			display = displayMap.display;
			if( display )
			display->postEvent(sEvent);
			return 0;

	  case WM_MBUTTONDOWN:
			ClickCount++;
			SetCapture(hWnd);
			sEvent.eventType = SMOUSE_EVENT;
			sEvent.mouseInput.Event = MIDDLE_MOUSE_PRESSED_DOWN;
			sEvent.mouseInput.X = (short)LOWORD(lParam);
			sEvent.mouseInput.Y = (short)HIWORD(lParam);
			display = displayMap.display;
			if( display )
			display->postEvent(sEvent);
			return 0;

			case WM_MBUTTONUP:
			ClickCount--;
			if (ClickCount<1)
			{
			ClickCount=0;
			ReleaseCapture();
			}
			sEvent.eventType = SMOUSE_EVENT;
			sEvent.mouseInput.Event = MIDDLE_MOUSE_LEFT_UP;
			sEvent.mouseInput.X = (short)LOWORD(lParam);
			sEvent.mouseInput.Y = (short)HIWORD(lParam);
			display = displayMap.display;
			if( display )
			display->postEvent(sEvent);
			return 0;

	 case WM_MOUSEMOVE:
			sEvent.eventType = SMOUSE_EVENT;
			sEvent.mouseInput.Event = SMOUSE_MOVED;
			sEvent.mouseInput.X = (short)LOWORD(lParam);
			sEvent.mouseInput.Y = (short)HIWORD(lParam);
			display = displayMap.display;
			if( display )
			display->postEvent(sEvent);
			return 0;

	 case WM_KEYDOWN:
	 case WM_KEYUP:
			{
			BYTE allKeys[256];

			sEvent.eventType = SKEY_EVENT;
			sEvent.keyInput.Key = (KEY_CODE)wParam;
			sEvent.keyInput.PressedDown = (message==WM_KEYDOWN);
			
			WORD KeyAsc=0;
			GetKeyboardState(allKeys);
			ToAscii((UINT)wParam,(UINT)lParam,allKeys,&KeyAsc,0);

			sEvent.keyInput.Shift = ((allKeys[VK_SHIFT] & 0x80)!=0);
			sEvent.keyInput.Control = ((allKeys[VK_CONTROL] & 0x80)!=0);
			sEvent.keyInput.Char = (KeyAsc & 0x00ff); //KeyAsc >= 0 ? KeyAsc : 0;

			display = displayMap.display;
			if( display )
			display->postEvent(sEvent);

			return 0;
			}
		case WM_ERASEBKGND:
			return 0;
		case WM_SIZE:
		{
			// resize
			display = displayMap.display;
			if (display)
				display->setResized(true);
		}
		return 0;
		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				BeginPaint(hWnd, &ps);
				EndPaint(hWnd, &ps);
			}
			return 0;
		case WM_SYSCOMMAND:
			// prevent screensaver or monitor powersave mode from starting
			if ((wParam & 0xFFF0) == SC_SCREENSAVE ||
				(wParam & 0xFFF0) == SC_MONITORPOWER)
				return 0;
			break;

		case WM_USER:
			return 0;
		}

		return DefWindowProc(hWnd, message, wParam, lParam);
	}


	SpiderWin32Display::SpiderWin32Display(const SpiderDisplayParams &displayParams ):displayParams(displayParams)
	{
		// get handle to exe file
		HINSTANCE hInstance = GetModuleHandle(0);

		// create the window if we need to and we do not use the null device
		if ( !displayParams.WindowID )
		{
			const sc8* ClassName = "spiderWin32Display";
			const sc8* Space = "Spider3D";

			// Register Class
			WNDCLASSEX wcex;
			wcex.cbSize		= sizeof(WNDCLASSEX);
			wcex.style		= CS_HREDRAW | CS_VREDRAW;
			wcex.lpfnWndProc	= WndProc;
			wcex.cbClsExtra		= 0;
			wcex.cbWndExtra		= 0;
			wcex.hInstance		= hInstance;
			wcex.hIcon         = NULL;
			wcex.hCursor       = LoadCursor(NULL, IDC_ARROW);
			wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
			wcex.lpszMenuName	= 0;
			wcex.lpszClassName	= (LPCSTR)ClassName;
			wcex.hIconSm		= 0;

			RegisterClassEx(&wcex);

			// calculate client size

			RECT clientSize;
			clientSize.top = 0;
			clientSize.left = 0;
			clientSize.right  = (LONG)displayParams.WindowWidth;
			clientSize.bottom = (LONG)displayParams.WindowHeight;

			DWORD style = WS_POPUP;
			DWORD		dwExStyle;

			//if (!displayParams.Fullscreen)
			//	style = WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SIZEBOX | WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

			DEVMODE dmScreenSettings;					// Device Mode

			if (displayParams.Fullscreen)
			{
				memset(&dmScreenSettings,0,sizeof(dmScreenSettings));		// Makes Sure Memory's Cleared
				dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
				dmScreenSettings.dmPelsWidth	= displayParams.WindowWidth;			// Selected Screen Width
				dmScreenSettings.dmPelsHeight	= displayParams.WindowHeight;			// Selected Screen Height
				dmScreenSettings.dmBitsPerPel	= displayParams.Bits;				// Selected Bits Per Pixel
				dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;
			}

			bool fullscreen = displayParams.Fullscreen;
			
			if ( fullscreen )
			{
			 if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
			 {
				if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","NeHe GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
				{
					fullscreen=false;				// Select Windowed Mode (Fullscreen=FALSE)
				}
				else
				{
					// Pop Up A Message Box Letting User Know The Program Is Closing.
					MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
					return;					// Exit And Return FALSE
				}
			 }
			}

			if(fullscreen)
			{
				dwExStyle = WS_EX_APPWINDOW;					// Window Extended Style
				style = WS_POPUP;						// Windows Style
				ShowCursor(FALSE);						// Hide Mouse Pointer
			}
			else
			{
				dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
				style = WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SIZEBOX | WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
			}

			//AdjustWindowRect(&clientSize, style, FALSE);
			AdjustWindowRectEx(&clientSize, style, FALSE, dwExStyle);
		
			const sf32 realWidth = clientSize.right - clientSize.left;
			const sf32 realHeight = clientSize.bottom - clientSize.top;

			sf32 windowLeft = (GetSystemMetrics(SM_CXSCREEN) - realWidth) / 2;
			sf32 windowTop = (GetSystemMetrics(SM_CYSCREEN) - realHeight) / 2;

			if (displayParams.Fullscreen)
			{
				windowLeft = 0;
				windowTop = 0;
			}

			// create window

			//hWND = CreateWindow( (LPCSTR)ClassName, (LPCSTR)Space, style, windowLeft, windowTop,
			//	realWidth, realHeight, NULL, NULL, hInstance, NULL);
			hWND = CreateWindowEx( dwExStyle,
			(LPCSTR)ClassName,
			(LPCSTR)Space,
			style,
			windowLeft, windowTop,
			realWidth, realHeight, 
			NULL, NULL, hInstance, NULL);

			this->displayParams.WindowID = hWND;

			ShowWindow(hWND, SW_SHOW);
			//ShowWindow(hWND, SW_SHOWNORMAL);

			UpdateWindow(hWND);

			MoveWindow(hWND, windowLeft, windowTop, realWidth, realHeight, TRUE);

			// set this as active window
			SetActiveWindow(hWND);
			
			SetForegroundWindow(hWND);

			// create cursor control

			cursorWindows = new CursorWindows(displayParams.WindowWidth, displayParams.WindowHeight , hWND);

			createRenderer();

			createSceneManager();

			displayMap.display = this;
		}
		// External Window
		if ( displayParams.WindowID )
		{
			//this->displayParams.WindowID = displ

		}


	}


	sbool SpiderWin32Display::postEvent(Event& sEvent) const
	{
		sbool absorbed = false;

		// Post the event to all registered event listners
		for( unsigned e=0; e<m_vecEventListners.size(); e++ )
		{
			m_vecEventListners[e]->OnEvent(sEvent);
		}
		return absorbed;
	}

	void SpiderWin32Display::createRenderer()
	{
		renderer = OglRenderer::getSingleton(displayParams);
	}


	sbool SpiderWin32Display::loop()
	{
		MSG msg;

		bool quit = false;

		//STimer::tick();
		sceneManager->getTimer()->tick();

		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);

			//if (ExternalWindow && msg.hwnd == hWND)
			//	WndProc(hWND, msg.message, msg.wParam, msg.lParam);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
				quit = true;
		}

		if (!quit)
			OnResize();

		//if(!quit)
		//	pollJoysticks();

		return !quit;
	}

	void SpiderWin32Display::OnResize()
	{
		if ( !_mResized )
			return;

		RECT r;
		GetClientRect(hWND, &r);
		
		renderer->setViewPort( 0 , 0 , r.right, r.bottom);
        
		sf32 ratio = (sf32)r.right/ r.bottom;
		sceneManager->getActiveCamera()->setAspectRatio( ratio );

	    _mResized = false;
	}

	void SpiderWin32Display::setResized(sbool resizeFlag)
	{
		_mResized = resizeFlag;
	}

	sbool SpiderWin32Display::isResized()
	{
		return _mResized;
	}

	void SpiderWin32Display::closeDisplay() const
	{
		MSG msg;
		PeekMessage(&msg, NULL, WM_QUIT, WM_QUIT, PM_REMOVE);
		PostQuitMessage(0);
		PeekMessage(&msg, NULL, WM_QUIT, WM_QUIT, PM_REMOVE);
		DestroyWindow(hWND);
	}

	void SpiderWin32Display::setEventListener(EventListener* eventListener)
	{
		this->eventListener = eventListener;
	}

	void SpiderWin32Display::addEventListener(EventListener* eventListener)
	{
		m_vecEventListners.push_back(eventListener);
	}

	Renderer* SpiderWin32Display::getRenderer() const
	{
		return renderer;
	}

	SceneManager* SpiderWin32Display::getSceneManager() const
	{
		return sceneManager;
	}

	void SpiderWin32Display::createSceneManager()
	{
		sceneManager = new SceneManager(this);
		sceneManager->setRenderer(*renderer);
		sceneManager->getActiveCamera()->setAspectRatio(displayParams.WindowWidth/displayParams.WindowHeight);
		sceneManager->getActiveCamera()->setViewPort(Rect(0,0,displayParams.WindowWidth,displayParams.WindowHeight));
	}

	void SpiderWin32Display::setTitle(char* title)
	{
		m_Title = title;
		DWORD dwResult;
		SendMessageTimeoutW(this->hWND, WM_SETTEXT, 0,
			reinterpret_cast<LPARAM>(title),
			SMTO_ABORTIFHUNG, 2000, &dwResult);
	}

	SpiderDisplay* createDeviceEx(const SpiderDisplayParams& displayParams)
	{
		return 0;
	}

	

}

#endif