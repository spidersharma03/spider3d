/*
 *  GUIContainer.h
 *  testCpp
 *
 *  Created by Prashant on 28/03/11.
 *  Copyright 2011 Sourcebits Technologies. All rights reserved.
 *
 */

#ifndef GUI_CONTAINER_H
#define GUI_CONTAINER_H

#include "GUIComponent.h"
#include <vector>

using namespace std;

namespace spider3d
{
	namespace gui 
	{
		
		class GUIContainer : public GUIComponent 
		{
		public:
			GUIContainer();

			GUIContainer(si32 width, si32 height);
						
			GUIContainer(GUIContainer* parent, si32 width, si32 height);
			
			void addComponent(GUIComponent*);
						
			GUIComponent* getComponentAt(si32);
			
			vector<GUIComponent*>& getComponents();
			
			GUIComponent* remove(si32);
			
			GUIComponent* remove(GUIComponent*);
			
			virtual void update();

			virtual void render() = 0;
			
			virtual GUIComponent* getComponentFromPoint(si32 x, si32 y)
			{
				GUIComponent* component = 0;
				
				for (unsigned i=0; i<m_vecComponents.size(); i++)
				{
					component = m_vecComponents[i]->getComponentFromPoint(x,y);
					if( component )
						return component;
				}
				
				return isPointInside(x, y) ? this : 0;
			}
			
		protected:

			vector<GUIComponent*> m_vecComponents;
		};
	}
}
#endif