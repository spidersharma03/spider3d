#include "Node.h"

class CustomProjectorGeometryNode: public Node
{
	CustomProjectorGeometryNode( Nodeptr , SceneManagerptr);

	void render();  	

	void OnRegister();

	void print();
};