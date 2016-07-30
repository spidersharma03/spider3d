#ifndef _TEXTURE_PROJECTOR_NODE_H
#define _TEXTURE_PROJECTOR_NODE_H

#include "Node.h"
#include "MeshNode.h"
#include "TextureUnitState.h"
#include "NodeAnimator.h"
#include <map>

namespace spider3d
{
	namespace effects
	{
		class TextureProjector;
	}
}

using namespace spider3d::effects;

namespace spider3d
{
	namespace scene
	{
		class TextureProjectorNode: public Node
		{
		public:

			static const int PROJECTION_POLICY_CAMERA = 0;
			static const int PROJECTION_POLICY_PROJECTOR = 1;
			static const int PROJECTION_POLICY_PROJECTOR_3D = 2;

			TextureProjectorNode( Texture* texture, SceneManagerptr sceneManagerPtr, Nodeptr parent );
			
			void addScope( MeshNode* meshNode );

			void removeScope( MeshNode* meshNode );

			void removeAllScopes();

			void setProjectionPolicy(int);

			virtual void render();  	

			virtual void OnRegister();

			virtual void print();

			TextureUnitState* getProjectorTextureUnitState();

		private:
			class ProjectorUpdater: public NodeAnimator
			{
			public:
				ProjectorUpdater(TextureProjector* textureProjector);
			
				virtual void animate(sf32 time);
				
				void setProjectionPolicy(int);

			public:
				TextureProjector* textureProjector;
				TextureAttributes* projectorTextureAttributes;
				int projectionPolicy;
			};

			Texture* texture;
			TextureUnitState* projectorTexUnitState;
			ProjectorUpdater* projectorUpdator;
			TextureProjector* textureProjector;
			map<MeshNode*, TextureUnitState*> m_oldTexUnitMap;

			Point3 projectorPosition;
			Point3 projectorTarget;
			Vector3 projectorUp;
			int projectionPolicy;
		};
	}
}
#endif