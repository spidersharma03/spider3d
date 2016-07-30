#ifndef _LIGHTNODE_H

#define _LIGHTNODE_H



#include "Node.h"

#include "LightData.h"


using namespace spider3d::appearance;


namespace spider3d
{

	namespace scene
	{


		class LightNode:public Node
		{

		public:

			void print();

			void render();

			void OnRegister();

			AABB& getLocalAABB() ;

			void setLightData(LightData&);

			void setLightType(LIGHT_TYPE);

            LightData& getLightData();

			LightNode(Nodeptr, SceneManagerptr);

			LightNode(SceneManagerptr);

			virtual ~LightNode();

		private:

			LightData lightData;

		};


	}

}

#endif 