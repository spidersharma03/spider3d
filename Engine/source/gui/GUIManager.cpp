/*
 *  GUIManager.cpp
 *  testCpp
 *
 *  Created by Prashant on 28/03/11.
 *  Copyright 2011 Sourcebits Technologies. All rights reserved.
 *
 */

#include "gui/GUIManager.h"

namespace spider3d
{
	namespace gui 
	{
		
		GUIManager::GUIManager()
		{
		}
		
		void GUIManager::drawGUI()
		{
			// Update All the GUI Components
			update();
			
			// Render All GUI Components
			render();
		}
		
		void GUIManager::render()
		{
			/*glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluOrtho2D(0, VIEWPORT_WIDTH, 0.0, VIEWPORT_HEIGHT);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			GUIContainer::render();
			glDisable(GL_BLEND);*/
			
		}
		
		sbool GUIManager::processEvents( Event& event)
		{
			GUIComponent* component = getComponentFromPoint(event.mouseInput.X, event.mouseInput.Y);
			if( component )
				component->OnEvent(event);
			
			return true;
		}
		
	}
}