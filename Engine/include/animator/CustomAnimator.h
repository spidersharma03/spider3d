#include "NodeAnimator.h"
#include "MeshBuffer.h"

using namespace spider3d::effects;

namespace spider3d
{
	namespace scene
	{ 
		class CustomAnimator:public NodeAnimator
		{
		public:
			CustomAnimator(MeshBuffer* meshBuffer , MeshBufferUpdater* meshBufferUpdater):meshBuffer(meshBuffer),meshBufferUpdater(meshBufferUpdater)
			{
				
			}

			void animate(sf32 time)
			{
				meshBuffer->updateMeshBuffer(meshBufferUpdater);
			}

		private:
			MeshBuffer* meshBuffer;
			MeshBufferUpdater* meshBufferUpdater;
		};

	}
}