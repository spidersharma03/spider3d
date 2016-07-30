/*
 *  MacOSXDeviceContext.cpp
 *  Spider3D
 *
 *  Created by Prashant on 21/08/12.
 *
 */


#ifdef _MACOSX_

#import <Cocoa/Cocoa.h>
#import <OpenGL/gl.h>
#import <Carbon/Carbon.h>

#include "MacOSXDeviceContext.h"
#include "OglRenderer.h"
#include "Cursor.h"
#include "EventListener.h"


namespace spider3d
{
	
	sbool MacOSXDeviceContext::loop()
	{
		NSEvent *event;
		Event	sevent;
		
		//storeMouseLocation();
		
		event = [NSApp nextEventMatchingMask:NSAnyEventMask untilDate:nil inMode:NSDefaultRunLoopMode dequeue:YES];
		if (event != nil);
		{
			bzero(&sevent,sizeof(sevent));
			
			switch([(NSEvent *)event type])
			{
				case NSKeyDown:
					postKeyEvent(event,sevent,true);
					break;
					
				case NSKeyUp:
					postKeyEvent(event,sevent,false);
					break;
					
				case NSFlagsChanged:
					sevent.eventType = SKEY_EVENT;
					sevent.keyInput.Shift = ([(NSEvent *)event modifierFlags] & NSShiftKeyMask) != 0;
					sevent.keyInput.Control = ([(NSEvent *)event modifierFlags] & NSControlKeyMask) != 0;
					
					if (IsShiftDown != sevent.keyInput.Shift)
					{
						sevent.keyInput.Char = KEY_SHIFT;
						sevent.keyInput.Key = KEY_SHIFT;
						sevent.keyInput.PressedDown = sevent.keyInput.Shift;
						
						IsShiftDown = sevent.keyInput.Shift;
						
						postEvent(sevent);
					}
					
					if (IsControlDown != sevent.keyInput.Control)
					{
						sevent.keyInput.Char = KEY_CONTROL;
						sevent.keyInput.Key = KEY_CONTROL;
						sevent.keyInput.PressedDown = sevent.keyInput.Control;
						
						IsControlDown = sevent.keyInput.Control;
						
						postEvent(sevent);
					}
					
					[NSApp sendEvent:event];
					break;
					
				case NSLeftMouseDown:
					sevent.eventType = SMOUSE_EVENT;
					sevent.mouseInput.Event = LEFT_MOUSE_PRESSED_DOWN;
					MouseButtonStates |= EMBSM_LEFT;
					sevent.mouseInput.ButtonStates = MouseButtonStates;
					postMouseEvent(event,sevent);
					break;
					
				case NSLeftMouseUp:
					sevent.eventType = SMOUSE_EVENT;
					MouseButtonStates &= !EMBSM_LEFT;
					sevent.mouseInput.ButtonStates = MouseButtonStates;
					sevent.mouseInput.Event = LEFT_MOUSE_LEFT_UP;
					postMouseEvent(event,sevent);
					break;
					
				case NSOtherMouseDown:
					sevent.eventType = SMOUSE_EVENT;
					sevent.mouseInput.Event = MIDDLE_MOUSE_PRESSED_DOWN;
					MouseButtonStates |= EMBSM_MIDDLE;
					sevent.mouseInput.ButtonStates = MouseButtonStates;
					postMouseEvent(event,sevent);
					break;
					
				case NSOtherMouseUp:
					sevent.eventType = SMOUSE_EVENT;
					MouseButtonStates &= !EMBSM_MIDDLE;
					sevent.mouseInput.ButtonStates = MouseButtonStates;
					sevent.mouseInput.Event = MIDDLE_MOUSE_LEFT_UP;
					postMouseEvent(event,sevent);
					break;
					
				case NSMouseMoved:
				case NSLeftMouseDragged:
				case NSRightMouseDragged:
				case NSOtherMouseDragged:
					sevent.eventType = SMOUSE_EVENT;
					sevent.mouseInput.Event = SMOUSE_MOVED;
					sevent.mouseInput.ButtonStates = MouseButtonStates;
					postMouseEvent(event,sevent);
					break;
					
				case NSRightMouseDown:
					sevent.eventType = SMOUSE_EVENT;
					sevent.mouseInput.Event = RIGHT_MOUSE_PRESSED_DOWN;
					MouseButtonStates |= EMBSM_RIGHT;
					sevent.mouseInput.ButtonStates = MouseButtonStates;
					postMouseEvent(event,sevent);
					break;
					
				case NSRightMouseUp:
					sevent.eventType = SMOUSE_EVENT;
					sevent.mouseInput.Event = RIGHT_MOUSE_LEFT_UP;
					MouseButtonStates &= !EMBSM_RIGHT;
					sevent.mouseInput.ButtonStates = MouseButtonStates;
					postMouseEvent(event,sevent);
					break;
					
				case NSScrollWheel:
					sevent.eventType = SMOUSE_EVENT;
					sevent.mouseInput.Event = SMOUSE_WHEEL;
					sevent.mouseInput.Wheel = [(NSEvent *)event deltaY];
					if (sevent.mouseInput.Wheel < 1.0f)
						sevent.mouseInput.Wheel *= 10.0f;
					else
						sevent.mouseInput.Wheel *= 5.0f;
					postMouseEvent(event,sevent);
					break;
					
				default:
					[NSApp sendEvent:event];
					break;
			}
		}
		[event release];
				
		return 0;//(![[NSApp delegate] isQuit] && IsActive);
	}
	
	sbool MacOSXDeviceContext::createWindow()
	{
		//CGDisplayErr            error;
		bool                    result=false;
		//CGDirectDisplayID       display=CGMainDisplayID();
		//CGLPixelFormatObj       pixelFormat;
		//CGRect                  displayRect;
		// CFDictionaryRef         displaymode, olddisplaymode;
		// GLint                   numPixelFormats, newSwapInterval;
		
		int alphaSize = 8;
		int depthSize = m_DisplayParams.ZBufferBits;
        
		m_ScreenWidth  = m_DisplayParams.WindowWidth;
		m_ScreenHeight = m_DisplayParams.WindowHeight;
		
		
		if (!m_DisplayParams.Fullscreen)
		{
			if(!m_DisplayParams.WindowID) //create another window when WindowId is null
			{
				//m_DeviceContextParams.windowSize.width,m_DeviceContextParams.windowSize.height
				// m_pWindow = [[NSWindow alloc] initWithContentRect:NSMakeRect(0,0,100,100 ) styleMask:NSTitledWindowMask+NSClosableWindowMask+NSResizableWindowMask backing:NSBackingStoreBuffered defer:FALSE];
				
				NSRect windowRect = NSMakeRect(10.0f, 10.0f, m_ScreenWidth, m_ScreenHeight );
				
				m_pWindow = [[NSWindow alloc] initWithContentRect:windowRect 
														styleMask:( NSResizableWindowMask | NSClosableWindowMask | NSTitledWindowMask) 
														  backing:NSBackingStoreBuffered defer:NO];
				
				[m_pWindow makeKeyAndOrderFront:nil];
			}
			
			if ( !m_DisplayParams.WindowID )
			{
				NSOpenGLPixelFormatAttribute windowattribs[] =
				{
					NSOpenGLPFANoRecovery,
					NSOpenGLPFAAccelerated,
					NSOpenGLPFADepthSize,     (NSOpenGLPixelFormatAttribute)depthSize,
					NSOpenGLPFAColorSize,     (NSOpenGLPixelFormatAttribute)m_DisplayParams.Bits,
					NSOpenGLPFAAlphaSize,     (NSOpenGLPixelFormatAttribute)alphaSize,
					NSOpenGLPFASampleBuffers, (NSOpenGLPixelFormatAttribute)1,
					NSOpenGLPFASamples,       (NSOpenGLPixelFormatAttribute)2,
					NSOpenGLPFAStencilSize,   (NSOpenGLPixelFormatAttribute)(m_DisplayParams.Stencilbuffer?1:0),
					NSOpenGLPFADoubleBuffer,
					(NSOpenGLPixelFormatAttribute)nil
				};
				
				//if (m_DisplayParams.antiAliasFactor<2)
				{
					windowattribs[ 9] = (NSOpenGLPixelFormatAttribute)0;
					windowattribs[11] = (NSOpenGLPixelFormatAttribute)0;
				}
				
				NSOpenGLPixelFormat *format;
				for (int i=0; i<3; ++i)
				{
					if (1==i)
					{
						// Second try without stencilbuffer
						if (m_DisplayParams.Stencilbuffer)
						{
							windowattribs[13]=(NSOpenGLPixelFormatAttribute)0;
						}
						else
							continue;
					}
					else if (2==i)
					{
						// Third try without Doublebuffer
						printf("No doublebuffering available !");
						windowattribs[14]=(NSOpenGLPixelFormatAttribute)nil;
					}
					
					format = [[NSOpenGLPixelFormat alloc] initWithAttributes:windowattribs];
					if (format == NULL)
					{
						if ( 1 )//m_DeviceContextParams.antiAliasFactor>1)
						{
							while (!format && windowattribs[12]>1)
							{
								windowattribs[12] = (NSOpenGLPixelFormatAttribute)((int)windowattribs[12]-1);
								format = [[NSOpenGLPixelFormat alloc] initWithAttributes:windowattribs];
							}
							
							if (!format)
							{
								windowattribs[9] = (NSOpenGLPixelFormatAttribute)0;
								windowattribs[11] = (NSOpenGLPixelFormatAttribute)0;
								format = [[NSOpenGLPixelFormat alloc] initWithAttributes:windowattribs];
								if (!format)
								{
									// reset values for next try
									windowattribs[9] = (NSOpenGLPixelFormatAttribute)1;
									windowattribs[11] = (NSOpenGLPixelFormatAttribute)2;
								}
								else
								{
									printf("No FSAA available !");
								}
								
							}
						}
					}
					else
						break;
				}
				//m_DeviceContextParams.antiAliasFactor = windowattribs[11];
				m_DisplayParams.Stencilbuffer=(windowattribs[13]==1);
				
				if (format != NULL)
				{
					m_pOGLContext = [[NSOpenGLContext alloc] initWithFormat:format shareContext:NULL];
					[format release];
				}
				
				if (m_pOGLContext != NULL)
				{
					if (!m_DisplayParams.WindowID)
					{
						[m_pWindow center];
						
						[m_pWindow setDelegate:[NSApp delegate]];
						
						[m_pOGLContext setView:[m_pWindow contentView]];
						
						[m_pWindow setAcceptsMouseMovedEvents:TRUE];
						
						[m_pWindow setIsVisible:TRUE];
						
						[m_pWindow makeKeyAndOrderFront:nil];
					}
					// In case an externally created window is presented to Morpheus2D :                                Comment / vektor / Jun 13,2012 - Implement later
					else //use another window for drawing
					    [m_pOGLContext setView:(NSView*)m_DisplayParams.WindowID];
					 
					
					m_CGLContext = (CGLContextObj) [m_pOGLContext CGLContextObj];
					CGLSetCurrentContext(m_CGLContext);
					// DeviceWidth = CreationParams.WindowSize.Width;
					// DeviceHeight = CreationParams.WindowSize.Height;
					result = true;
				}
			}

		}
		
		return (result);
		
	}

	void MacOSXDeviceContext::OnResize()
	{
	}
	
	void MacOSXDeviceContext::setResize(su32 width, su32 height)
	{
		// set new window size
		m_ScreenWidth = width;
		m_ScreenHeight = height;
		
		// update the size of the opengl rendering context
		[m_pOGLContext update];
		
		// resize the driver to the inner pane size
		if (m_pWindow)
		{
			NSRect driverFrame = [m_pWindow contentRectForFrameRect:[m_pWindow frame]];
			//getVideoDriver()->OnResize(core::dimension2d<u32>( (s32)driverFrame.size.width, (s32)driverFrame.size.height));
			m_pRenderer->setViewPort( 0 , 0 , (su32)driverFrame.size.width, (su32)driverFrame.size.height);
			
			sf32 ratio = (sf32)(su32)driverFrame.size.height/ (su32)driverFrame.size.width;
			m_pSceneManager->getActiveCamera()->setAspectRatio( ratio );
		}
		else
		{
		}
		
		if (m_DisplayParams.WindowID && m_pOGLContext)
			[(NSOpenGLContext *)m_pOGLContext update];
	}
	
	void MacOSXDeviceContext::closeDisplay() const
	{
		if (m_pWindow != NULL)
		{
			[m_pWindow setIsVisible:FALSE];
			
			if (m_pOGLContext != NULL)
			{
				[m_pOGLContext clearDrawable];
				[m_pOGLContext release];
				//m_pOGLContext = NULL;
			}
			
			[m_pWindow setReleasedWhenClosed:TRUE];
			[m_pWindow release];
			//m_pWindow = NULL;
		}
		else
		{
			if (m_CGLContext != NULL)
			{
				if(m_DisplayParams.WindowID)
				{
					[(NSOpenGLContext *)m_pOGLContext clearDrawable];
					[(NSOpenGLContext *)m_pOGLContext release];
					//m_pOGLContext = NULL;
				}
				else
				{
					CGLSetCurrentContext(NULL);
					CGLClearDrawable(m_CGLContext);
					CGLDestroyContext(m_CGLContext);
					CGReleaseAllDisplays();
				}
			}
		}
		
		//IsActive = false;
		//m_CGLContext = NULL;
	}
	
	sbool MacOSXDeviceContext::postEvent(Event& event) const
	{
		return false;
	}
	
	void MacOSXDeviceContext::setResized(sbool bResized)
	{
		_mResized = bResized;
	}
	
	sbool MacOSXDeviceContext::isResized()
	{
		return 0;
	}
	
	void MacOSXDeviceContext::setTitle(char* title)
	{
	}
	
	MacOSXDeviceContext::MacOSXDeviceContext(const DisplayParams& displayParams) : DeviceContext(displayParams)
	{
	}
	
	
	void MacOSXDeviceContext::initKeycodes()
	{
		KeyCodes[NSUpArrowFunctionKey]     = KEY_UP;
		KeyCodes[NSDownArrowFunctionKey]   = KEY_DOWN;
		KeyCodes[NSLeftArrowFunctionKey]   = KEY_LEFT;
		KeyCodes[NSRightArrowFunctionKey]  = KEY_RIGHT;
		KeyCodes[NSF1FunctionKey]          = KEY_F1;
		KeyCodes[NSF2FunctionKey]          = KEY_F2;
		KeyCodes[NSF3FunctionKey]          = KEY_F3;
		KeyCodes[NSF4FunctionKey]          = KEY_F4;
		KeyCodes[NSF5FunctionKey]          = KEY_F5;
		KeyCodes[NSF6FunctionKey]          = KEY_F6;
		KeyCodes[NSF7FunctionKey]          = KEY_F7;
		KeyCodes[NSF8FunctionKey]          = KEY_F8;
		KeyCodes[NSF9FunctionKey]          = KEY_F9;
		KeyCodes[NSF10FunctionKey]         = KEY_F10;
		KeyCodes[NSF11FunctionKey]         = KEY_F11;
		KeyCodes[NSF12FunctionKey]         = KEY_F12;
		KeyCodes[NSF13FunctionKey]         = KEY_F13;
		KeyCodes[NSF14FunctionKey]         = KEY_F14;
		KeyCodes[NSF15FunctionKey]         = KEY_F15;
		KeyCodes[NSF16FunctionKey]         = KEY_F16;
		KeyCodes[NSHomeFunctionKey]        = KEY_HOME;
		KeyCodes[NSEndFunctionKey]         = KEY_END;
		KeyCodes[NSInsertFunctionKey]      = KEY_INSERT;
		KeyCodes[NSDeleteFunctionKey]      = KEY_DELETE;
		KeyCodes[NSHelpFunctionKey]        = KEY_HELP;
		KeyCodes[NSSelectFunctionKey]      = KEY_SELECT;
		KeyCodes[NSPrintFunctionKey]       = KEY_PRINT;
		KeyCodes[NSExecuteFunctionKey]     = KEY_EXECUT;
		KeyCodes[NSPrintScreenFunctionKey] = KEY_SNAPSHOT;
		KeyCodes[NSPauseFunctionKey]       = KEY_PAUSE;
		KeyCodes[NSScrollLockFunctionKey]  = KEY_SCROLL;
		KeyCodes[0x7F]                     = KEY_BACK;
		KeyCodes[0x09]                     = KEY_TAB;
		KeyCodes[0x0D]                     = KEY_RETURN;
		KeyCodes[0x03]                     = KEY_RETURN;
		KeyCodes[0x1B]                     = KEY_ESCAPE;
	}
	
	void MacOSXDeviceContext::storeMouseLocation()
	{
		NSPoint	p;
		int	x,y;
		
		p = [NSEvent mouseLocation];
		
		if (m_pWindow != NULL)
		{
			p = [m_pWindow convertScreenToBase:p];
			//x = (int)p.x;
			//y = DeviceHeight - (int)p.y;
		}
		else
		{
			x = (int)p.x;
			y = (int)p.y;
			//y -= (ScreenHeight - DeviceHeight);
		}
		
		//((CCursorControl *)m_pCursor)->updateInternalCursorPosition(x,y);
	}
	
	void MacOSXDeviceContext::postMouseEvent(void *event, Event & ievent)
	{
		bool post = true;
		
		if (m_pWindow != NULL)
		{
			ievent.mouseInput.X = (int)[(NSEvent *)event locationInWindow].x;
			ievent.mouseInput.Y = m_ScreenHeight - (int)[(NSEvent *)event locationInWindow].y;
			
			if (ievent.mouseInput.Y < 0)
				post = false;
		}
		else
		{
			ievent.mouseInput.X = (int)[NSEvent mouseLocation].x;
			ievent.mouseInput.Y = m_ScreenHeight - (int)[NSEvent mouseLocation].y;
		}
		
		if (post)
			postEvent(ievent);
		
		[NSApp sendEvent:(NSEvent *)event];
	}
	
	void MacOSXDeviceContext::postKeyEvent(void *event, Event &ievent, bool pressed)
	{
		NSString *str;
		std::map<int,int>::const_iterator iter;
		unsigned int result,c,mkey,mchar;
		const unsigned char *cStr;
		BOOL skipCommand;
		
		str = [(NSEvent *)event characters];
		if ((str != nil) && ([str length] > 0))
		{
			mkey = mchar = 0;
			skipCommand = false;
			c = [str characterAtIndex:0];
			
			iter = KeyCodes.find(c);
			if (iter != KeyCodes.end())
				mkey = (*iter).second;
			else
			{
				// workaround for period character
				if (c == 0x2E)
				{
					mkey = KEY_PERIOD;
					mchar = '.';
				}
				else
				{
					cStr = (unsigned char *)[str cStringUsingEncoding:NSWindowsCP1252StringEncoding];
					if (cStr != NULL && strlen((char*)cStr) > 0)
					{
						mchar = cStr[0];
						mkey = toupper(mchar);
						if ([(NSEvent *)event modifierFlags] & NSCommandKeyMask)
						{
							if (mkey == 'C' || mkey == 'V' || mkey == 'X')
							{
								mchar = 0;
								skipCommand = true;
							}
						}
					}
				}
			}
			
			ievent.eventType = SKEY_EVENT;
			ievent.keyInput.Key = (KEY_CODE)mkey;
			ievent.keyInput.PressedDown = pressed;
			ievent.keyInput.Shift = ([(NSEvent *)event modifierFlags] & NSShiftKeyMask) != 0;
			ievent.keyInput.Control = ([(NSEvent *)event modifierFlags] & NSControlKeyMask) != 0;
			ievent.keyInput.Char = mchar;
			
			if (skipCommand)
				ievent.keyInput.Control = true;
			else if ([(NSEvent *)event modifierFlags] & NSCommandKeyMask)
				[NSApp sendEvent:(NSEvent *)event];
			
			postEvent(ievent);
		}
		[str release];
	}
	
}	

#endif