/*
 *  GUIButton.h
 *  testCpp
 *
 *  Created by Prashant on 30/03/11.
 *  Copyright 2011 Sourcebits Technologies. All rights reserved.
 *
 */

#ifndef _GUI_BUTTON_H
#define _GUI_BUTTON_H

#include "GUIComponent.h"


namespace spider3d
{
	namespace gui 
	{
		class GUIButton: public GUIComponent
		{
		public:
			GUIButton(si32 width, si32 height);
			
			GUIButton(GUIContainer* parent, si32 width, si32 height);
			
			virtual void render();
			
			virtual void update();
			
			virtual sbool OnEvent( Event& );
			
		protected:
			sbool m_bPressed;
			virtual void createGeometry();
		};
	}
}

#endif