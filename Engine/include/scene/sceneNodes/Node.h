#ifndef _NODE_H
#define _NODE_H

#include "coreMath.h"
#include "spiderDataTypes.h"
#include "AABB.h"
#include <vector>
#include "Ray.h"

//#include "NodeAnimator.h"
using namespace std;
using namespace spider3d::core;

namespace spider3d
{
	namespace picking
	{
		class PickResult;
	}
}
using namespace spider3d::picking;

typedef vector<PickResult*> PickResultArray;


namespace spider3d
{

	enum CULL_OPERATION { CULL_OFF = 0 , CULL_FRUSTUM_PLANE , CULL_FRUSTUM_BOX }; 

	namespace scene
	{

		class NodeAnimator;

		typedef class SceneManager* SceneManagerptr;;

		typedef class Node* Nodeptr; 

		class Node
		{
		public:

			virtual void render();  	

			virtual void OnRegister();

			virtual void Update(sf32);

			virtual void print();

			PickResultArray pick(Ray& ray);

			PickResultArray pickSorted(Ray& ray);

			PickResult* pickClosest(Ray& ray);

			PickResult* pickAny(Ray& ray);

			virtual AABB& getLocalAABB() 
			{
				return bbox;
			}; 

			virtual AABB getGlobalAABB() 
			{
				AABB globalAABB;
				globalAABB.set(getLocalAABB());
				globalAABB.transform(globalTransform);
				globalAABB.reCalculateAABB();
				return globalAABB;
			};

			void setPickable(sbool bPickable);

			void addChild(Nodeptr);

			void addChild(Nodeptr , su32);

			void addAnimator(NodeAnimator*);

			void setTransform(sf32*);

			void setTransform(Matrix4f&);

			void setVisible(sbool);

			void setCullOperation(CULL_OPERATION cullState);

			sbool isCulled();

			void setCullState(sbool);

			sbool isTransformChanged();

			void setTransformChanged(sbool);

			sbool getVisible();

			Matrix4f getLocalTransform();

			Matrix4f& getGlobalTransform();

			sbool detach();

			sbool removeChild(Node*);

			void removeAllChildren();

			Node& getChild(su32);

			vector<Node*>& getChildren();

			su32 numChildren();

			sbool hasChanged();

			void setChanged();

			Nodeptr getParent();

			CULL_OPERATION getCullOperation();

			sc8* getName();

			void setName(sc8*);

			Node( Nodeptr , SceneManagerptr);

			void UpdateTransform();

			//bool operator < (const Node& node) const;

			// inline methods
            inline sf32 getDistanceFromCameraSq()
			{
				return m_DistFromCameraSq;
			}

			SceneManager* getSceneManager();

			AABB bbox;

		protected:

			virtual PickResult* intersectRay(Ray&);

			sc8 name[50];

			Nodeptr parent;

			SceneManagerptr sceneManager;

			vector<Node*> Children;
			
			vector<PickResult*> m_vecPickResults;

			vector<NodeAnimator*> Animators;

			Matrix4f localTransform;

			Matrix4f globalTransform;

			Point4 translation;

			Point3 scale;

			sf32 rotation[4];

			sbool isRegistered;

			inline void updateGlobalTransform();

			sbool _mTransformChanged;

			sbool _mVisible;

			CULL_OPERATION cullOperation;

			sbool cullState;

			sbool m_Pickable;

			sf32 m_DistFromCameraSq;
		};


	}

}

#endif