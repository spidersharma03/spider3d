/*
 *  GUIButton.cpp
 *  testCpp
 *
 *  Created by Prashant on 30/03/11.
 *  Copyright 2011 Sourcebits Technologies. All rights reserved.
 *
 */

#include "gui/GUIButton.h"
#include "gui/GUIContainer.h"

namespace spider3d
{
	namespace gui 
	{
		
		GUIButton::GUIButton(si32 width, si32 height):GUIComponent(width, height)
		{
			createGeometry();
			m_bPressed = false;
		}

		GUIButton::GUIButton(GUIContainer* parent, si32 width, si32 height):GUIComponent((GUIComponent*)parent, width, height)
		{
			parent->addComponent(this);
			createGeometry();
			m_bPressed = false;
		}

		void GUIButton::render()
		{			
//			glPushMatrix();
//			glTranslatef(m_GlobalTransform.m02, m_GlobalTransform.m12, 0.0f);
//			
//			//glBegin(GL_QUADS);
////			
////			glColor4ub(m_Colors[0], m_Colors[1], m_Colors[2], m_Colors[3]);
////			for( int i=0; i<8; i+=2)
////			{
////				glVertex2f(m_vertices[i], m_vertices[i+1]);
////			}
////			glEnd();
//			
//			float x1, y1, x2, y2;
//			x1 = m_vertices[0];
//			y1 = m_vertices[1];
//			x2 = m_vertices[2];
//			y2 = m_vertices[3];
//			float step = 1.0f;///VIEWPORT_HEIGHT;
//			float r1,g1,b1,a1,r2,g2,b2,a2,r,g,b,a;
//			r1 = g1 = b1 = a1 = 10;
//			r2 = g2 = b2 = a2 = 10;
//			if( m_bPressed )
//			{
//				r1 = g1 = b1 = a1 = 100;
//				r2 = g2 = b2 = a2 = 100;
//			}
//			if( !m_bPressed )
//			{
//				r1 = g1 = b1 = a1 = 200;
//				r2 = g2 = b2 = a2 = 200;
//			}
//			
//			glBegin(GL_LINES);
//			for( int l=0; l<m_Height; l++ )
//			{
//				float alpha = (float)l/m_Height;
//				r = (alpha) * r1 + (1.0f-alpha)*r2;
//				glColor4ub(r, r, r, 250);
//				y1 += step;
//				y2 += step;
//				glVertex2f(x1, y1);
//				glVertex2f(x2, y2);
//			}
//			glEnd();
//
//			
//			glPopMatrix();
		}

		void GUIButton::update()
		{
			if(m_Parent)
			{
				Matrix3f& parentTransform = m_Parent->getTransform();
				m_GlobalTransform.multiply(parentTransform, m_LocalTransform);
			}
		}
		
		sbool GUIButton::OnEvent( Event& event)
		{
			if( event.mouseInput.Event == spider3d::LEFT_MOUSE_PRESSED_DOWN)
				m_bPressed = true;
			if( event.mouseInput.Event == spider3d::LEFT_MOUSE_LEFT_UP)
				m_bPressed = false;

			return m_Parent ? m_Parent->OnEvent(event) : false;
		}
		
		void GUIButton::createGeometry()
		{
			m_vertices = new sf32[4*2];
			m_Colors   = new su8[4*4];
			m_vertices[0] = 0;
			m_vertices[1] = 0;
			m_vertices[2] = m_Width;
			m_vertices[3] = 0;
			m_vertices[4] = m_Width;
			m_vertices[5] = m_Height;
			m_vertices[6] = 0.0f;
			m_vertices[7] = m_Height;
			
			for( int i=0; i<16; i++)
			{
				m_Colors[i] = 128;
			}
			
		}
	}
}