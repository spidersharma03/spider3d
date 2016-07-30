#include "loader/SceneBase.h"

void SceneBase::addNamedObjects( string name , Node* value ) 
{
	namedObjMap[name] = value;
}

Node* SceneBase::getNamedObject( string name)
{
	if ( namedObjMap.find(name) != namedObjMap.end() )
	return namedObjMap.find(name)->second ;

	return 0;
}

void SceneBase::setSceneNode( Node* node)
{
	sceneNode = node;
}

Node* SceneBase::getSceneNode()
{
	return sceneNode;
}

void SceneBase::setAppearance(Appearance* app)
{
}

SceneBase::SceneBase()
{
	sceneNode = 0;
}

SceneBase::~SceneBase()
{
	namedObjMap.clear();
}