/*
 *  GUIComponent.cpp
 *  testCpp
 *
 *  Created by Prashant on 28/03/11.
 *  Copyright 2011 Sourcebits Technologies. All rights reserved.
 *
 */

#include "gui/GUIComponent.h"

namespace spider3d
{
	namespace gui 
	{
		
		GUIComponent::GUIComponent(si32 width, si32 height):m_Width(width),m_Height(height)
		{
			m_bFocused = false;
			m_X = m_Y = 0;
			m_Color.x = m_Color.y = m_Color.z = 128;
			m_Color.w = 255;
			m_Transparency = 0.0f;
			m_bEnable = true;
			m_Parent = 0;
			createGeometry();
		}
		
		GUIComponent::GUIComponent(GUIComponent* parent, si32 width, si32 height):m_Parent(parent),m_Width(width),m_Height(height)
		{
			m_bFocused = false;
			m_X = m_Y = 0;
			m_Color.x = m_Color.y = m_Color.z = 128;
			m_Color.w = 255;
			m_Transparency = 0.0f;
			m_bEnable = true;
			createGeometry();
		}
		
		void GUIComponent::setEnable(sbool bEnable)
		{
			m_bEnable = bEnable;
		}
		
		void GUIComponent::addEventListner(EventListener* eventListner)
		{
		}
		
		void GUIComponent::addFocusListner(IFocusListner* focusListner)
		{
		}
		
		void GUIComponent::setSize(si32 width, si32 height)
		{
			m_Width = width;
			m_Height = height;
		}
		
		void GUIComponent::setPosition(si32 x, si32 y)
		{
			m_X = x;
			m_Y = y;
			m_LocalTransform.m02 = (sf32)m_X;
			m_LocalTransform.m12 = (sf32)m_Y;
		}
		
		void GUIComponent::setTransform(Matrix3f& transform)
		{
			m_LocalTransform = transform;
		}
		
		void GUIComponent::setColor(Color4& color)
		{
			m_Color = color;
			for( int i=0; i<16; i+=4)
			{
				m_Colors[i]   = m_Color.x*255;
				m_Colors[i+1] = m_Color.y*255;
				m_Colors[i+2] = m_Color.z*255;
				m_Colors[i+3] = m_Color.w*255;
			}
		}
		
		void GUIComponent::setColor(Color3& color)
		{
			m_Color.x = color.x;
			m_Color.y = color.y;
			m_Color.z = color.z;

			for( int i=0; i<16; i+=4)
			{
				m_Colors[i]   = m_Color.x*255;
				m_Colors[i+1] = m_Color.y*255;
				m_Colors[i+2] = m_Color.z*255;
				m_Colors[i+3] = m_Color.w*255;
			}
		}
		
		void GUIComponent::setTransparency(sf32 transparency)
		{
			if( transparency < 0.0f || transparency > 1.0f )
				return;
			
			m_Transparency = transparency;
			
			for( int i=0; i<16; i+=4)
			{
				m_Colors[i+3] = m_Color.w * ( 1.0f - m_Transparency ) * 255;
			}
		}
		
	}
}

