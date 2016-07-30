/*
 *  GUIPanel.cpp
 *  testCpp
 *
 *  Created by Prashant on 30/03/11.
 *  Copyright 2011 Sourcebits Technologies. All rights reserved.
 *
 */

#include "gui/GUIPanel.h"


namespace spider3d
{
	namespace gui 
	{
		
		GUIPanel::GUIPanel(si32 width, si32 height):GUIContainer(width, height)
		{
			createGeometry();
		}
		
		GUIPanel::GUIPanel(GUIContainer* parent, si32 width, si32 height):GUIContainer(parent, width, height)
		{
			createGeometry();
		}
		
		void GUIPanel::render()
		{
//			GUIContainer::render();
//
//			glPushMatrix();
//			glTranslatef(m_GlobalTransform.m02, m_GlobalTransform.m12, 0.0f);
//			
//			//glBegin(GL_QUADS);
////			
////			//for( int i=0; i<16; i+=4)
////			//glColor4ub(m_Colors[i], m_Colors[i+1], m_Colors[i+2], m_Colors[i+3]);
////			glColor4ub(m_Colors[0], m_Colors[1], m_Colors[2], m_Colors[3]);
////			for( int i=0; i<8; i+=2)
////			{
////				//glVertex2f(m_vertices[i], m_vertices[i+1]);
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
//			r1 = g1 = b1 = a1 = 200;
//			r2 = g2 = b2 = a2 = 100;
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
//			glPopMatrix();

		}
		
		sbool GUIPanel::OnEvent( Event& event)
		{
			return m_Parent ? m_Parent->OnEvent(event) : false;
		}
		
		
		void GUIPanel::createGeometry()
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