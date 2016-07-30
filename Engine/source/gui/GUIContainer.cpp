/*
 *  GUIContainer.cpp
 *  testCpp
 *
 *  Created by Prashant on 28/03/11.
 *  Copyright 2011 Sourcebits Technologies. All rights reserved.
 *
 */

#include "gui/GUIContainer.h"

namespace spider3d
{
	namespace gui 
	{
		GUIContainer::GUIContainer():GUIComponent(0, 0)
		{
		}

		GUIContainer::GUIContainer(si32 width, si32 height):GUIComponent(width, height)
		{
		}
		
		GUIContainer::GUIContainer(GUIContainer* parent, si32 width, si32 height):GUIComponent(parent,width, height)
		{
			parent->addComponent(this);
		}
		
		void GUIContainer::addComponent(GUIComponent* component)
		{
			m_vecComponents.push_back(component);
			component->m_Parent = this;
		}
		
		
		GUIComponent* GUIContainer::getComponentAt(si32 index)
		{
			GUIComponent* component = 0;
			if( index >= m_vecComponents.size()-1 )
				component = 0;
			else 
				component = m_vecComponents[index];
			
			return component;
		}
		
		vector<GUIComponent*>& GUIContainer::getComponents()
		{
			return m_vecComponents;
		}
		
		GUIComponent* GUIContainer::remove(si32 index)
		{
			return 0;
		}
		
		GUIComponent* GUIContainer::remove(GUIComponent* component)
		{
			return 0;
		}
		
		void GUIContainer::update()
		{
			if(m_Parent)
			{
				Matrix3f& parentTransform = m_Parent->getTransform();
				m_GlobalTransform.multiply(parentTransform, m_LocalTransform);
			}
			for( unsigned i=0; i<m_vecComponents.size(); i++ )
			{
				m_vecComponents[i]->update();
			}
		}
		
		void GUIContainer::render()
		{
			for( unsigned c=0; c<m_vecComponents.size(); c++ )
			{
				if(m_vecComponents[c])
					m_vecComponents[c]->render();
			}
		}
	}
}