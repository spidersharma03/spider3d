#ifndef _SPIDERWIN32DISPLAY_H
#define _SPIDERWIN32DISPLAY_H

#ifdef WIN32


#include "spiderDisplay.h"
#include "spiderDisplayParams.h"
#include "OglRenderer.h"
#include "Cursor.h"
#include <windows.h>
#include <Windowsx.h>

namespace spider3d
{

	class Win32DeviceContext : public DeviceContext
	{

	public:

        sbool loop(); 

		virtual void OnResize() ;

		virtual void closeDisplay() const;

		virtual void setEventListener(EventListener*);

		virtual void addEventListener(EventListener*);

		virtual sbool postEvent(Event&) const;

		virtual void setResized(sbool);

		virtual sbool isResized();

		Win32DeviceContext(const SpiderDisplayParams&);

				//! Implementation of the win32 cursor control
		class CursorWindows : public Cursor
		{
		public:

			CursorWindows(unsigned windowWidth, unsigned windowHeight, HWND hwnd)
				: windowWidth(windowWidth), windowHeight(windowHeight), invWindowWidth(0.0f),invWindowHeight(0.0f), 
					HWnd(hwnd), BorderX(0), BorderY(0)
			{
				if (windowWidth!=0)
					invWindowWidth = 1.0f / windowWidth;

				if (windowHeight!=0)
					invWindowHeight = 1.0f / windowHeight;

				BorderX = GetSystemMetrics(SM_CXDLGFRAME);
				BorderY = GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYDLGFRAME);
			}

			
			virtual void setVisible(sbool visible)
			{
				CURSORINFO info;
				info.cbSize = sizeof(CURSORINFO);

				if ( visible )
				{
					while ( GetCursorInfo(&info) )
					{
						if ( info.flags == CURSOR_SHOWING )
						{
							bVisible = visible;
							break;
						}
						ShowCursor(true);   // this only increases an internal display counter in windows, so it might have to be called some more
					}
				}
				else
				{
					while ( GetCursorInfo(&info) )
					{
						if ( info.flags == 0 )  // cursor hidden
						{
							bVisible = visible;
							break;
						}
						ShowCursor(false);   // this only decreases an internal display counter in windows, so it might have to be called some more
					}
				}
			}


			//! Sets the new position of the cursor.
			virtual void setPosition(si32 x, si32 y)
			{
				RECT rect;
				rect.left = 0;rect.right = 0;
				rect.bottom = 0;rect.top = 0;
				SetCursorPos(x + rect.left + BorderX, y + rect.top + BorderY);
				this->x = (sf32)x;
				this->y = (sf32)y;
			}

			//! Sets the new position of the cursor.
			virtual void setRelativePosition(sf32 x, sf32 y)
			{
				RECT rect;
				if (GetWindowRect(HWnd, &rect))
				SetCursorPos(x*windowWidth + rect.left + BorderX, y*windowHeight + rect.top + BorderY);
				this->x = x*windowWidth;
				this->y = y*windowHeight;
			}

			virtual void getPosition(si32* x, si32* y)
			{
				*x = (si32)this->x;
				*y = (si32)this->y;
			}

			virtual void getRelativePosition(sf32* x, sf32* y)
			{
				updateInternalCursorPosition();
				*x = this->x * invWindowWidth;
				*y = this->y * invWindowHeight;
			}
			/** Used to notify the cursor that the window was resized. */
			virtual void OnResize(su32 width, su32 height)
			{
				if (width!=0)
					invWindowWidth = 1.0f / width;
				else 
					invWindowWidth = 0.0f;
 
				if (height!=0)
					invWindowHeight = 1.0f / height;
				else
					invWindowHeight = 0.0f;
			}

		private:

			//! Updates the internal cursor position
			void updateInternalCursorPosition()
			{
				POINT p;
				if (!GetCursorPos(&p))
				{
					DWORD xy = GetMessagePos();
					p.x = GET_X_LPARAM(xy);
					p.y = GET_Y_LPARAM(xy);
				} 

				RECT rect;
				if (GetWindowRect(HWnd, &rect))
				{
					x = p.x-rect.left-BorderX;
					y = p.y-rect.top-BorderY;
				}
				else
				{
					// window seems not to be existent, so set cursor to
					// a negative value
					x = -1;
					y = -1;
				}
			}

			unsigned windowWidth, windowHeight;
			float invWindowWidth, invWindowHeight;
			HWND HWnd;
			
			si32 BorderX, BorderY;
		};

		Cursor* getCursor()
		{
			return cursorWindows;
		}

		void setTitle(char* title);

	private:

		void createRenderer();

		void createSceneManager();
        
		// HANDLE OF THIS WIN32 Window
		HWND hWND;
	};
}

#endif

#endif