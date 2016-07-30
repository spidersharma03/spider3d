/*
 *  GUIComponent.h
 *  testCpp
 *
 *  Created by Prashant on 28/03/11.
 *  Copyright 2011 Sourcebits Technologies. All rights reserved.
 *
 */

#ifndef _GUI_COMPONENT_H
#define _GUI_COMPONENT_H

//#include<GL/glut.h>

#include "coreMath.h"
#include "EventListener.h"

using namespace spider3d::core;
using namespace spider3d;

#define VIEWPORT_WIDTH 800
#define VIEWPORT_HEIGHT 600



class IFocusListner
{
public:
	IFocusListner()
	{
	}
};

namespace spider3d
{
	namespace gui 
	{
		class GUIContainer;
		
		class GUIComponent : public EventListener
		{
		public:
			
			GUIComponent(si32 width, si32 height);
			
			GUIComponent(GUIComponent*, si32 width, si32 height);

			virtual void render() = 0;
				
			virtual void update() = 0;

			void setEnable(sbool);
			
			void addEventListner(EventListener* );
			
			void addFocusListner(IFocusListner* );
			
			void setSize(si32, si32);
			
			void setPosition(si32, si32);
			
			void setTransform(Matrix3f&);
			
			void setColor(Color4&);
			
			void setColor(Color3&);
			
			void setTransparency(sf32);
			
			inline GUIComponent* getParent()
			{    
				return m_Parent;
			}

			inline bool hasFocus()
			{
				return m_bFocused;
			}
			
			inline Matrix3f& getTransform()
			{
				return m_GlobalTransform;
			}
			
			//virtual sbool OnEvent( Event& );

			virtual GUIComponent* getComponentFromPoint(si32 x, si32 y)
			{
				if( isPointInside(x,y) )
				{
					return this;
				}
				else 
				{
					return 0;
				}
			}
			
			inline bool isPointInside(si32 x, si32 y)
			{
				return ( x < ( m_GlobalTransform.m02 + m_Width) && x > ( m_GlobalTransform.m02 ) 
						&& y < ( m_GlobalTransform.m12 + m_Height) && y > ( m_GlobalTransform.m12) );
			}
			
			friend class GUIContainer;
			
		protected:
			
			virtual void createGeometry()
			{};
			
			Matrix3f m_GlobalTransform;
			Matrix3f m_LocalTransform;
			sbool m_bFocused;
			si32 m_Width, m_Height;
			si32 m_X, m_Y;
			Color4 m_Color;
			sf32 m_Transparency;
			sbool m_bEnable;
			GUIComponent *m_Parent;
			
			sf32 *m_vertices;
			su8  *m_Colors;
		};
	}
	
}
#endif