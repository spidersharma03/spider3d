#include "scene/sceneNodes/Node.h"
#include "animator/NodeAnimator.h"
#include "scene/SceneManager.h"
#include "picking/PickResult.h"
#include <stack>
#include <algorithm>

#include<string.h>
#include<stdio.h>
#include<typeinfo>

namespace spider3d
{

namespace scene
{

void Node::setName(sc8* name)
{
  strcpy( this->name , name);
}

sc8* Node::getName()
{
  return name;
}

void Node::render()
{
}

Nodeptr Node::getParent()
{
  return parent;
}

Node::Node(Nodeptr parent , SceneManagerptr sceneManager):sceneManager(sceneManager),parent(parent)
{
 if ( parent )
 parent->addChild(this);
 cullState = CULL_OFF;
 _mVisible = true;
 _mTransformChanged = true;
 m_Pickable = false;
}


void Node::setTransform(Matrix4f& inMatrix)
{
  localTransform.set(inMatrix);
  globalTransform.multiply(localTransform); 
  setTransformChanged(true);
}

void Node::setCullOperation(CULL_OPERATION cullState)
{
	this->cullOperation = cullOperation;
}

CULL_OPERATION Node::getCullOperation( )
{
	return cullOperation;
}

void Node::setCullState( sbool cullFlag)
{

}

sbool Node::isCulled()
{
	return cullState;
}

void Node::addChild(Nodeptr spatial)
{
	  //if ( (spatial->getParent() ) )
  {
  spatial->parent = this;
  spatial->setTransformChanged(true);
  Children.push_back(spatial);
  }

}

void Node::addAnimator(NodeAnimator* animator)
{
	  Animators.push_back(animator);
}


void Node::Update(sf32 time)
{
	for ( su32 j = 0 ; j < Animators.size() ; j ++ )
	{
		NodeAnimator& animator = *( Animators[j] ) ;

		animator.animate(time);
	}

    if ( _mTransformChanged )
	{
		updateGlobalTransform();
	}

	for ( su32 i = 0 ; i < Children.size() ; i ++ )
	{
        Node& node = *( Children[i] ) ;
		node.Update(time);
	}
}


void Node::OnRegister()
{
	for ( su32 i = 0 ; i < Children.size() ; i ++ )
	{
        Node& node = *( Children[i] ) ;
		node.OnRegister();
	}
	_mTransformChanged = false;
}

sbool Node::detach()
{
	return parent->removeChild(this);	
}

sbool Node::removeChild(Node* node)
{
	bool bFind = false;
	for( unsigned c=0; c< Children.size(); c++ )
	{
		if( node == Children[c] )
		{
			Children.erase(	Children.begin() + c );
			bFind = true;
			break;
		}
	}
	return bFind;
}

void Node::removeAllChildren()
{
	int size = Children.size();
	for( unsigned c=0; c< size; c++ )
	{
		Children.erase(	Children.begin() + c );
	}
}

void Node::setPickable(sbool bPickable)
{
	m_Pickable = bPickable;
}

bool PickingSortPredicate( PickResult* pr1,  PickResult* pr2)
{
	return ( pr1->getPickDistanceSquared() < pr2->getPickDistanceSquared() );
}

PickResult* Node::pickAny(Ray& ray)
{
    vector<PickResult*> vecPickResults = pick(ray);
	return (vecPickResults.size() > 0) ? vecPickResults[0] : 0;
}

PickResult* Node::pickClosest(Ray& ray)
{
    PickResultArray vecPickResults = pickSorted(ray);
	return (vecPickResults.size() > 0) ? vecPickResults[0] : 0;
}

PickResultArray Node::pickSorted(Ray& ray)
{
    vector<PickResult*> vecPickResults = pick(ray);
	std::sort( vecPickResults.begin(), vecPickResults.end(), PickingSortPredicate);
	return vecPickResults;
}

PickResultArray Node::pick(Ray& ray)
{
	vector<Node*> pickableNodes;
	stack<Node*> nodes;

	nodes.push(this);
	pickableNodes.push_back(this);
    Node* currentNode;
	// Iterative Tree Traversal. we find all the Pickable Nodes here.
	while (! nodes.empty() )
	{
      currentNode = nodes.top();
	  nodes.pop();
	  for( unsigned c=0; c<currentNode->getChildren().size(); c++ )
	  {
		  Node* child = &currentNode->getChild(c);
		  if( child )
		  {
			  nodes.push(child);
			  pickableNodes.push_back(child);
		  }
	  }
    }

    vector<PickResult*> vecPickResults;
	// Find the Nodes which intersects with this ray, and find PickResult's for each
	for( unsigned p=0; p<pickableNodes.size(); p++ )
	{
		Node* node = pickableNodes[p];
		PickResult* pickResult = node->intersectRay(ray);
		if(pickResult)
			vecPickResults.push_back(pickResult);
	}
	return vecPickResults;
}

PickResult* Node::intersectRay(Ray& ray)
{
	return 0;
}

vector<Node*>& Node::getChildren()
{
	return Children;
}

Node& Node::getChild(su32 index)
{
	return *(Children[index]);
}

su32 Node::numChildren()
{
	return Children.size();
}


Matrix4f Node::getLocalTransform()
{
  Matrix4f mat;
  mat.set(localTransform);
  return mat;
}


Matrix4f& Node::getGlobalTransform()
{
  return globalTransform;
}

sbool Node::getVisible()
{
	return _mVisible;
}

void Node::setVisible(sbool bVisible)
{
	_mVisible = bVisible;
}

void Node::print()
{
  printf("%s%s\n" , "Rendering Node " , this->getName() );
}


void Node::UpdateTransform()
{

      Node* parent = this->getParent();
	  if ( parent )
	  {
	  Matrix4f& globalParent = parent->getGlobalTransform();
	  globalTransform.multiply ( globalParent , globalTransform);
	  //printf("%s%s\n" , "Global Transform of " , getName() );
	  //globalTransform.print();
	  }


	for ( su32 i = 0 ; i < Children.size() ; i ++ )
	{
         Node& node = *(Children[i]); 
        if ( &node )  
		{
		  node.UpdateTransform();
		}
	}

}


void Node::updateGlobalTransform()
{
      Node* parent = this->getParent();
	  if ( parent != 0 )
	  {
	  Matrix4f& globalParent = parent->getGlobalTransform();
	  globalTransform.multiply ( globalParent , localTransform);
	  }

}

sbool Node::isTransformChanged()
{
	return _mTransformChanged;
}

void Node::setTransformChanged(sbool flag)
{
  _mTransformChanged = flag;
  for ( su32 i = 0 ; i < Children.size() ; i ++ )
	{
		Node& node = *( Children[i] );
        if ( &node )  
		{
		  node.setTransformChanged(flag);
		}
	}
}

SceneManager* Node::getSceneManager()
{
	return sceneManager;
}

// Operator is overloaded for transparent sorting
//bool Node::operator < (const Node& node) const
//{
//   return ( m_DistFromCameraSq < node.m_DistFromCameraSq);
//}
}

}