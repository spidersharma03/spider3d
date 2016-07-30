#ifndef _SPIDERDISPLAY_H

#define _SPIDERDISPLAY_H

#include "spiderDataTypes.h"
#include "spiderDisplayParams.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "STimer.h"
#include "EventListener.h"

using namespace spider3d::scene;
using namespace spider3d::render;

namespace spider3d
{
	class Cursor;

	class DeviceContext
	{
	public:
				
		DeviceContext(  const DisplayParams&  displayParams)
        {
			m_DisplayParams = displayParams;
		}
		
		~DeviceContext();
		
		SceneManager* getSceneManager() const
		{
			return m_pSceneManager;
		}

		Renderer* getRenderer() const
		{
			return m_pRenderer;
		}
		
		Cursor* getCursor()
		{
			return m_pCursor;
		}
		
		void setEventListener(EventListener* eventListener)
		{
			this->eventListener = eventListener;
		}
		
		void addEventListener(EventListener* eventListener)
		{
			m_vecEventListners.push_back(eventListener);
		}
		
		virtual sbool createWindow() = 0;
		
		virtual void closeDisplay() const = 0;

		virtual sbool loop() = 0;

		virtual void OnResize() = 0;

		virtual void setTitle(char*) = 0;
		
	protected:
		SceneManager* m_pSceneManager;
		Renderer*	  m_pRenderer;
		Cursor*       m_pCursor;

		EventListener* eventListener;
		vector<EventListener*> m_vecEventListners;

		DisplayParams m_DisplayParams;
		
		STimer spiderTimer;
		
		sbool _mResized;
		
		char* m_Title;
	};

}

#endif