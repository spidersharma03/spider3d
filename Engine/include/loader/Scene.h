#ifndef _SCENE_H

#define _SCENE_H

#include "Chunk.h"

namespace spider3d
{
	namespace scene
	{
		class Node;
	}
}

using namespace spider3d::scene;

class Scene
{
public:

	virtual void addNamedObjects( string , Node* ) = 0;

	virtual Node* getNamedObject( string ) = 0;

	virtual void setSceneNode( Node* ) = 0;

	virtual Node* getSceneNode() = 0;

};

#endif