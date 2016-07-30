#ifndef _SKY_BOX_NODE_H
#define _SKY_BOX_NODE_H

#include "Node.h"
#include "Image.h"

namespace spider3d
{

	namespace appearance
	{
		class Appearance;
	}
}

using namespace spider3d::appearance;

namespace spider3d
{
	namespace scene
	{
		class SkyBoxNode: public Node
		{
		public:

			SkyBoxNode( Nodeptr , SceneManagerptr, 
						Image* left, Image* right,
						Image* front, Image* back,
						Image* top  , Image* bottom);

			SkyBoxNode( Nodeptr , SceneManagerptr, Image* images);

			void render();  	

			void OnRegister();

			void print();

		private:

			void createGeometry();
			
			sf32 frontCoords[12];
			sf32 backCoords[12];
			sf32 leftCoords[12];
			sf32 rightCoords[12];
			sf32 topCoords[12];
			sf32 bottomCoords[12];
			sf32 texCoordsFront[8];
			sf32 texCoordsBack[8];
			sf32 texCoordsLeft[8];
			sf32 texCoordsRight[8];
			sf32 texCoordsBottom[8];
			sf32 texCoordsTop[8];

			Image* left;
			Image* right;
			Image* front;
			Image* back;
			Image* top;
			Image* bottom;

			Appearance *frontApp;
			Appearance *backApp;
			Appearance *leftApp;
			Appearance *rightApp;
			Appearance *topApp;
			Appearance *bottomApp;
		};
	}
}
#endif