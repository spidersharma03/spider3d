#ifndef EXTRUSION_H
#define EXTRUSION_H

#include "GeneralShape2D.h" 
#include "GeneralPath.h" 

namespace spider3d
{
	namespace scene
	{
		class IndexedMeshBuffer;
		class Mesh;
	}
}

using namespace spider3d::scene;

namespace spider3d
{
	namespace cad
	{

		class Extrusion
		{

		private:

			GeneralShape2D *shape ;
			GeneralPath *path;
			float* coord;
			float* tcoords;
			float* normals;
			float* colors;
			double** startcap_coord;
			double** endcap_coord;
			unsigned short* indices;
			void* appearance;
			int npoints_shape , npoints_path , cap;
			IndexedMeshBuffer* meshBuffer;
			Mesh* mesh;
			sbool m_bFlipNormal;
			//GLuint texName;

		public:
			Extrusion(GeneralShape2D * , GeneralPath *, sbool bFlipNormal = false);
			Extrusion(GeneralShape2D * , GeneralPath * , void *);
			virtual ~Extrusion();
			int getNumPolygons();
			Mesh* getMesh();

		private:
			void generateCoordinates();
			void generateTextureCoordinates();
			void generateIndices();
			void generateNormals();
			void sub(float* , float* , float* );
			void crossprod( float* , float* , float* );
			void GenerateCaps();
			void createMesh();
		};

	}
}
#endif