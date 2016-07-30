#ifndef _SCENE_BASE_H

#define _SCENE_BASE_H

#include "Scene.h"

namespace spider3d
{
	namespace appearance
	{
		class Appearance;
	}
}

using namespace spider3d::appearance;

class SceneBase: public Scene
{

public:

	virtual void addNamedObjects( string , Node* ) ;

	virtual Node* getNamedObject( string ) ;

	virtual void setSceneNode( Node* ) ;
	
	virtual Node* getSceneNode() ;

	void setAppearance(Appearance*);

	SceneBase();

	virtual ~SceneBase();

private:

	map< string , Node* > namedObjMap;

	Node* sceneNode;
};

#endif