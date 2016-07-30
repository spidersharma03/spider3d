#include "EventListener.h"
#include "Node.h"
#include "CameraNode.h"

using namespace spider3d::scene;

unsigned char* data;

class SEventListner:public EventListener
{
public:

	SEventListner(SceneManager* smgr, Nodeptr rootNode, CameraNode *camera):m_Smgr(smgr),m_rootNode(rootNode),m_Camera(camera)
	{
		data = new unsigned char[36];
		for( int i=0; i<36; i+=4)
		{
			data[i] = 155;
			data[i+1] = 0;
			data[i+2] = 0;
			data[i+3] = 0;
		}
	}

	sbool OnEvent(Event& sEvent)
	{
		Event inputEvent = sEvent;

		/*if ( inputEvent.eventType == KEY_EVENT )
		{
		  for( unsigned i = 0; i<256; i++ ) 
			keyCodes[inputEvent.keyInput.Key] = inputEvent.keyInput.PressedDown;
		}*/

		/*if( inputEvent.keyInput.Key == KEY_KEY_D )
		{
			m_Camera->setEnable(false);
		}
		if( inputEvent.keyInput.Key == KEY_KEY_C )
		{
			m_Camera->setEnable(true);
		}*/
		float x = (float)inputEvent.mouseInput.X;
		float y = (float)inputEvent.mouseInput.Y;
		//su32 viewPort[4] = {(su32)m_ViewPort.x,(su32)m_ViewPort.y,(su32)m_ViewPort.width,(su32)m_ViewPort.height};
		Ray ray;
		PickResultArray pickResults;
		PickResult* pickClosestResult = 0;
		switch(inputEvent.mouseInput.Event)
		{
		case LEFT_MOUSE_PRESSED_DOWN:
			//mouseEvent[0] = true;
			break;
		case SMOUSE_MOVED:
			ray = m_Camera->getRayFromScreenCoordinates((si32)x,(si32)(600-y));
			pickClosestResult = m_rootNode->pickClosest(ray);
			if(pickClosestResult)
			{
				MeshNode* meshNode = (MeshNode*)pickClosestResult->getNode();
				Point3 pickPoint = pickClosestResult->getIntersection().getIntersectionPointLocal();
				Point3 pickTexPoint = pickClosestResult->getIntersection().getTextureCoordinates();
				Vector3 pickNormal = pickClosestResult->getIntersection().getIntersectionNormalLocal();
				Point3 *pickPrimitive = pickClosestResult->getIntersection().getIntersectedPrimitiveCoordinatesLocal();
				//float r = (float)rand()/RAND_MAX;
				//float g = (float)rand()/RAND_MAX;
				//float b = (float)rand()/RAND_MAX;
				Appearance *app = meshNode->getAppearance();
				//meshNode->getAppearance()->getMaterial().emissive.set(r,g,b,1.0f);
				meshNode->pickPoint = pickPoint;
				meshNode->pickNormal = pickNormal;
				meshNode->pickPrimitive = pickPrimitive;
				TextureUnitState *texUnit = app->getTextureUnitState(0);
				if(texUnit)
				{
					Texture* texture = texUnit->getTexture();
					int w = texture->getWidth();
					int h = texture->getHeight();
					int x = w*pickTexPoint.x;
					int y = h*pickTexPoint.y;
					Rect rect(x,y,1,1);
					m_Smgr->getRenderer()->updateTexture(rect,data,texture);
				}
			}
			break;
		case LEFT_MOUSE_LEFT_UP:
			//mouseEvent[0] = false;
			break;
		case RIGHT_MOUSE_PRESSED_DOWN:
			//mouseEvent[1] = true;
			break;
		case RIGHT_MOUSE_LEFT_UP:
			//mouseEvent[1] = false;
			break;
		case MIDDLE_MOUSE_PRESSED_DOWN:
			//mouseEvent[2] = true;
			break;
		case MIDDLE_MOUSE_LEFT_UP:
			//mouseEvent[2] = false;
			break;
		default:
			break;
		}
		return true;
	}

	Node *m_rootNode;
	CameraNode* m_Camera;
	SceneManager* m_Smgr;
};