#ifndef _ALTERNATE_APPEARANCE_NODE_H
#define _ALTERNATE_APPEARANCE_NODE_H

#include <vector>
#include <map>
#include "Node.h"

using namespace std;

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
		class MeshNode;
			
		class AlternateAppearanceNode :	private Node
		{
		public:

			AlternateAppearanceNode(SceneManagerptr sceneManagerPtr, Nodeptr parent);
			
			AlternateAppearanceNode(Appearance*, SceneManagerptr sceneManagerPtr, Nodeptr parent);

			Appearance* getAppearance();

			void setAppearance(Appearance* appearance);

			void addScope(MeshNode*);

			void removeScope(MeshNode*);
			
			void removeAllScopes();

			virtual void render();  	

			virtual void OnRegister();

			virtual void print();

			virtual ~AlternateAppearanceNode();

		private:
			Appearance* m_pAppearance;
			map<MeshNode*,Appearance*> m_oldAppearanceMap;
		};

	}
}
#endif