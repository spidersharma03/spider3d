#ifndef _MESH_UPDATER_H
#define _MESH_UPDATER_H

namespace spider3d
{
	namespace scene
	{
		class MeshBuffer;

		class MeshBufferUpdater
		{
		public:
			virtual void updateMeshBuffer(MeshBuffer* meshBufer) = 0;
		};

	}
}

#endif