/*
 *  GUIManager.h
 *  testCpp
 *
 *  Created by Prashant on 28/03/11.
 *  Copyright 2011 Sourcebits Technologies. All rights reserved.
 *
 */

#ifndef _GUI_MANAGER_H
#define _GUI_MANAGER_H

#include "GUIContainer.h"

namespace spider3d
{
	namespace gui 
	{
	
		class GUIManager: public GUIContainer
		{
		public:
			
			GUIManager();
			
			void drawGUI();
			
			virtual void render();

			sbool processEvents( Event& event);

			sbool OnEvent(Event& event)
			{
				return false;
			}
			
		protected:
			
			void processEvents();
		};
		
	}
}
#endif