/*
 *  GUIWindow.cpp
 *  testCpp
 *
 *  Created by Prashant on 28/03/11.
 *  Copyright 2011 Sourcebits Technologies. All rights reserved.
 *
 */

#include "gui/GUIWindow.h"
#include "gui/GUIPanel.h"
#include "gui/GUIButton.h"

namespace spider3d
{
	namespace gui 
	{
		GUIWindow::GUIWindow(GUIContainer* parent, si32 width, si32 height):GUIContainer(parent, width,height)
		{
			createWindow();
			createGeometry();
			m_bFocused = true;
		}
		
		GUIWindow::GUIWindow(GUIContainer* parent, si32 width, si32 height, si32 flags):GUIContainer(parent, width,height),m_Flags(flags)
		{
			createWindow();
			createGeometry();
			m_bFocused = true;
		}
		
		void GUIWindow::render()
		{
//			GUIContainer::render();
//
//			glPushMatrix();
//			
//			glTranslatef(m_GlobalTransform.m02, m_GlobalTransform.m12, 0.0f);
//
//			//glBegin(GL_QUADS);
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
//			r1 = g1 = b1 = a1 = 10;
//			r2 = g2 = b2 = a2 = 250;
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

		void GUIWindow::createGeometry()
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
		
		sbool GUIWindow::OnEvent( Event& event)
		{
			static int currx, prevx;
			static int curry, prevy;

			
			if( event.mouseInput.Event == spider3d::LEFT_MOUSE_PRESSED_DOWN && event.mouseInput.Event != spider3d::SMOUSE_MOVED )
			{
				printf("Mouse Pressed\n");
				currx = event.mouseInput.X;
				curry = event.mouseInput.Y;
				prevx = event.mouseInput.X;
				prevy = event.mouseInput.Y;
			}
			if( event.mouseInput.Event == spider3d::LEFT_MOUSE_PRESSED_DOWN && event.mouseInput.Event == spider3d::SMOUSE_MOVED )
			{
				printf("Mouse Dragged\n");
				currx = event.mouseInput.X;
				curry = event.mouseInput.Y;
			}
			
			setPosition(m_X + (currx-prevx) , m_Y + (curry-prevy));

			
			//if( event.mouseInput.Event == spider3d::LEFT_MOUSE_LEFT_UP)
			{
				//printf("Mouse Pressed\n");
				prevx = currx;
				prevy = curry;
			}
			
				
			return m_Parent ? m_Parent->OnEvent(event) : false;
		}
		
		void GUIWindow::createWindow()
		{
			float titleBorderHeight = 0.05f;
			float titleBorderOptionButtonsPanelWidth = 0.12f;
			if( m_Flags & TITLE_BORDER )
			{
				m_TitleBorderPanel = new GUIPanel(this, m_Width, m_Height * titleBorderHeight);
				m_TitleBorderPanel->setPosition(0, m_Height - m_Height * titleBorderHeight);
				
				m_TitleBorderOptionButtonsPanel = new GUIPanel(m_TitleBorderPanel, m_Width*titleBorderOptionButtonsPanelWidth, m_Height*0.05f);
				m_TitleBorderOptionButtonsPanel->setPosition(m_Width - m_Width*titleBorderOptionButtonsPanelWidth, 0);
				
				Color3 color;
				color.x = 1.0f;
				m_TitleBorderOptionButtonsPanel->setColor(color);

				if(m_Flags & CLOSE_BUTTON)
				{
					m_CloseButton = new GUIButton(m_TitleBorderOptionButtonsPanel, m_Width*titleBorderOptionButtonsPanelWidth*0.4f, m_Height*0.05f);
					m_CloseButton->setPosition(m_Width * titleBorderOptionButtonsPanelWidth - m_Width*titleBorderOptionButtonsPanelWidth*0.4f, 0);
					color.y = 1.0f;
					m_CloseButton->setColor(color);
				}
				if(m_Flags & MINMIZE_BUTTON)
				{
					m_MinimizeButton = new GUIButton(m_TitleBorderOptionButtonsPanel,m_Width*titleBorderOptionButtonsPanelWidth*0.4f, m_Height*0.05f);
					m_MinimizeButton->setPosition(m_Width * titleBorderOptionButtonsPanelWidth - 2*m_Width*titleBorderOptionButtonsPanelWidth*0.4f, 0);
					color.x = 0.0f;
					m_MinimizeButton->setColor(color);
				}
				if(m_Flags & MAXIMIZE_BUTTON)
				{
					m_MaximizeButton = new GUIButton(m_TitleBorderOptionButtonsPanel, m_Width*titleBorderOptionButtonsPanelWidth*0.4f, m_Height*0.05f);
					m_MaximizeButton->setPosition(m_Width * titleBorderOptionButtonsPanelWidth - 3*m_Width*titleBorderOptionButtonsPanelWidth*0.4f, 0);
					color.x = 0.9f;
					color.y = 0.3f;
					m_MaximizeButton->setColor(color);
				}
			}
			
			if( m_Flags & STATUS_BAR )
			{
				m_StatusBarPanel = new GUIPanel(this, m_Width, m_Height*0.05f);
				m_StatusBarPanel->setPosition(0, 0);
			}
		}
		
		
		
	}
}