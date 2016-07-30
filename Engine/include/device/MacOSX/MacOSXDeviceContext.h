/*
 *  MacOSXDeviceContext.h
 *  Spider3D
 *
 *  Created by Prashant on 21/08/12.
 *
 */


#ifndef _MACOSX_DEVICE_CONTEXT_H_
#define _MACOSX_DEVICE_CONTEXT_H_


#ifdef _MACOSX_

#include "spiderDisplay.h"
#include "spiderDisplayParams.h"
#include "Cursor.h"

#include <map>


namespace spider3d
{
    class MacOSXDeviceContext : public DeviceContext
	{
		
	public:
		
        virtual sbool loop(); 
		
		virtual void OnResize() ;
		
		virtual void closeDisplay() const;
		
		virtual sbool postEvent(Event&) const;
		
		virtual void setResized(sbool);
		
		virtual sbool isResized();
		
		virtual void setTitle(char* title);

		virtual sbool createWindow();
		
		MacOSXDeviceContext(const DisplayParams&);
		
		void setResize(su32 width, su32 height);

		//! Implementation of the win32 cursor control
		class CursorMacOSX : public Cursor
		{
		public:
			
			//CursorMacOSX(unsigned windowWidth, unsigned windowHeight, HWND hwnd)
//			: windowWidth(windowWidth), windowHeight(windowHeight), invWindowWidth(0.0f),invWindowHeight(0.0f), 
//			HWnd(hwnd), BorderX(0), BorderY(0)
//			{
//				if (windowWidth!=0)
//					invWindowWidth = 1.0f / windowWidth;
//				
//				if (windowHeight!=0)
//					invWindowHeight = 1.0f / windowHeight;
//				
//				BorderX = GetSystemMetrics(SM_CXDLGFRAME);
//				BorderY = GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYDLGFRAME);
//			}
			
			
			virtual void setVisible(sbool visible)
			{
			}
			
			
			//! Sets the new position of the cursor.
			virtual void setPosition(si32 x, si32 y)
			{
				//RECT rect;
				//rect.left = 0;rect.right = 0;
				//rect.bottom = 0;rect.top = 0;
				this->x = (sf32)x;
				this->y = (sf32)y;
			}
			
			//! Sets the new position of the cursor.
			virtual void setRelativePosition(sf32 x, sf32 y)
			{
				//RECT rect;
				
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
			}
			
			unsigned windowWidth, windowHeight;
			float invWindowWidth, invWindowHeight;
			//HWND HWnd;
			
			si32 BorderX, BorderY;
		};
				
	private:
		
		void initKeycodes();
		void storeMouseLocation();
		void postMouseEvent(void *event, Event &ievent);
		void postKeyEvent(void *event, Event &ievent, bool pressed);
		
		int  m_ScreenWidth, m_ScreenHeight;

		void createRenderer();
		
		void createSceneManager();
        
		NSWindow         *m_pWindow;
		
		CGLContextObj    m_CGLContext;
		
		NSOpenGLContext  *m_pOGLContext;
		// HANDLE OF THIS WIN32 Window
		//HWND hWND;						
		std::map<int,int>	KeyCodes;
		
		sbool IsShiftDown;
		sbool IsControlDown;
		su32  MouseButtonStates;
		
	};
}

#endif


#endif
