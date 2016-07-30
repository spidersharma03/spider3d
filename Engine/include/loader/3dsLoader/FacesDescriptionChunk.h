#ifndef _FACE_DESCRIPTION_CHUNK_H

#define _FACE_DESCRIPTION_CHUNK_H


#define NUM_MAX_VERTICES  65535

#include "Chunk.h"
#include "ChunkChopper.h"
#include "Chunk_Types.h"
#include "KeyFramer.h"
#include "NormalGenerator.h"
#include "MeshNode.h"
#include "Appearance.h"
#include "SMeshBuffer.h"
#include <set>

using namespace spider3d::appearance;
using namespace spider3d::scene;

typedef struct Vector2
{
	float x,y;
	Vector2()
	{
		x = y = 0.0f;
	}

	Vector2(float x, float y):x(x),y(y)
	{
	}

	Vector2(Vector2& v):x(v.x),y(v.y)
	{
	}
}Vector2;

typedef vector<int>* IntArrayRef;
typedef set<int>* IntArrayReference;

/*******************************************************************************
* This chunk describes all the triangles that make up a mesh.
* Each triangle is defined in terms of three indexes each of which
* is a point reference to a vertex in the vertex list loaded
* by the triangular mesh chunk.
* After loading the Smoothing chunk the normals for the mesh
* are generated accordingly.
******************************************************************************/
class FacesDescriptionChunk :public Chunk 
{

	/***************************************************************************
	* Maintains a two way mapping between coordinates
	* and vertices.  A coordinate to vertex is one to many
	* Vertex to coordinate is one to one.
	* In this class we maintain the definition that a coordinate
	* is a point in 3D space and a vertex is a coordinate serving
	* as one of three defining a face.
	**************************************************************************/
	class PointMapper //extends HashMap 
	{
		////////////////////////////////////////////////////////////////////////
		// Private Member Variables ////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////
	private:

		vector < IntArrayReference >* coordinateSet;
		IntArrayReference sharedCoordinates;
		IntArrayReference returnSet;
		map < Vector3* , IntArrayReference > pointMap;

		/***********************************************************************
		* Constructs a PointMapper with a
		* the number of coordinates initialized to size.
		* @param size the number of coordinates in the set.
		**********************************************************************/
	public:
		PointMapper(int size)
		{
			//coordinateSet = new array< IntArrayRef >[size];
			coordinateSet = new vector< IntArrayReference >[size];
			coordinateSet->resize(size);
			sharedCoordinates = 0;
		}

		/***********************************************************************
		* Adds an index for a coordinate to the set of vertices mapped
		* to that coordinate. All coordinates may have one or more vertices
		* that use them.
		* @param coordinate the coordinate being mapped to the vertexNum
		* @param vertexNum the number of the vertex using the coordinate
		**********************************************************************/
		void addCoordinate(Vector3* coordinate, int vertexNum) 
		{
			/*sharedCoordinates = (Set) get(coordinate);
			if (sharedCoordinates == null) {
			sharedCoordinates = new HashSet();
			put(coordinate, sharedCoordinates);
			}
			sharedCoordinates.add(new Integer(vertexNum));
			coordinateSet[vertexNum] = sharedCoordinates;*/
			if ( pointMap.find ( coordinate ) !=  pointMap.end() )
			{
				sharedCoordinates = ( pointMap.find( coordinate ))->second; 
			}
			else
			{
				sharedCoordinates = new set<int>();
				//pointMap.insert ( pair<Vector3*, IntArrayReference>(coordinate , sharedCoordinates) );
				pointMap[coordinate] = sharedCoordinates;
			}
			//if ( sharedCoordinates->binary_search(vertexNum) == -1 )
				sharedCoordinates->insert(vertexNum);

			//if ( coordinateSet[vertexNum].binary_search(sharedCoordinates) == -1 )
				(*coordinateSet)[vertexNum] = sharedCoordinates;
				//coordinateSet->push_back(sharedCoordinates);
				int n = coordinateSet->size();
				int i=0;
				i++;
		}

		/***********************************************************************
		* Gets all the coordinates for a particular vertex that
		* also share that vertex after the smoothing groups have been
		* accounted for.  Any coordinates that are not both shared
		* by the vertex and do not share a smoothing group with the coordinate
		* will not be returned.
		* @param coordinateNum the number of the coordinate to get the set
		* of vertices for that share it.
		* @param smoothGroups the group of coordinates used to filter out the
		* non-shared vertices.
		**********************************************************************/
		IntArrayReference getSharedCoordinates(int coordinateNum, unsigned* smoothGroups)
		{
			returnSet = new set<int>();
			IntArrayReference& sharingVertices = (*coordinateSet)[coordinateNum];
			int coordinateMask = smoothGroups[coordinateNum];
			
			set<int>::iterator it;

		    for ( it=sharingVertices->begin() ; it != sharingVertices->end(); it++ )
			{
				int vertex = (*it);
				int nextMask = smoothGroups[vertex];
				if ((nextMask & coordinateMask) != 0)
				{
					returnSet->insert( vertex );
				}
			}
			return returnSet;
		}

		void free()
		{

			sharedCoordinates->clear();
			sharedCoordinates = 0;

			//returnSet->clear();
			//returnSet = 0;

			pointMap.clear();

			coordinateSet->clear();
			coordinateSet = 0;
			/*for (int i = 0; i < coordinateSet.length; i++) {
			sharedCoordinates = coordinateSet[i];
			if (sharedCoordinates != null) {
			Iterator e = sharedCoordinates.iterator();
			while (e.hasNext()) {
			Object pt = e.next();
			pt = null;
			}
			sharedCoordinates = null;
			}
			coordinateSet[i] = null;
			}
			coordinateSet = null;
			returnSet = null;*/
		}
	};



	////////////////////////////////////////////////////////////////////////////
	// Private Member Variable /////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////
private:
	ChunkChopper* chopper;
	vector<Vector3*>* meshFaceIndexVertexArray ;
	vector<Vector2*>* textureTriangles ;
	PointMapper* shareMap ;
	//vector3df* coordinates ;
	vector<Vector3*>* coordinates; 
	vector<unsigned>* facesIndex ;
	vector<Vector2*>* texturePoints ;
	vector<Vector3*>* smoothingNormals ;
	vector<Vector3*>* curObject3dSmoothGrp ;
	vector<Vector3*>* sharedNormals ;
	vector<int> smoothGroupVec;
	//Object3D object3D = null;
	//PolygonAttributes patt = null;
	//TextureAttributes texAtt = null;
	//RenderingAttributes ratt = null;
	//ColoringAttributes colatt = null;
	//TriangleArray tarray = null;
	//NormalGenerator normalGenerator = null;
	//GeometryInfo ginfo = null;
	Vector3* limits ;
	vector<Vector2*>* currentTexCoord ;
	vector<Vector3*>* currentVerticesX ;
	vector<Vector3*>* normals ;
	unsigned* smoothGroups ;    // Structures for resize

	//May 26 Normal flip fix
	bool bFlipNormal ;

	/***************************************************************************
	* Default Appeareance
	**************************************************************************/
	//static final Appearance DEFAULT_APPEARANCE;


	/*static {
	DEFAULT_APPEARANCE = new Appearance();
	Material defaultMaterial = new Material();
	defaultMaterial.setAmbientColor(new Color3f(.5f, .5f, .5f));
	DEFAULT_APPEARANCE.setMaterial(defaultMaterial);
	}*/

public:



	FacesDescriptionChunk()
	{
		bFlipNormal = false;
		textureTriangles = 0;
		currentTexCoord  = 0;
		smoothingNormals = 0;
		texturePoints    = 0;
		normals          = 0;
		smoothGroups     = 0;
	}

	/***************************************************************************
	* Reads the number of faces from the ChunkChopper.
	* For each face read three shorts representing
	* indices of vertices loaded by the TriangularMeshChunk
	*
	* @param chopper chopper the has the data
	**************************************************************************/
	void loadData(ChunkChopper* chopper) 
	{

		this->chopper = chopper;
		int numFaces = chopper->getUnsignedShort();
		//May 26 Normal flip fix
		bFlipNormal = false;

		facesIndex = new vector<unsigned>(numFaces);
		//facesIndex->set_used(numFaces);
		shareMap = new PointMapper(numFaces * 3);
		coordinates = (vector<Vector3*>*) chopper->popData(VERTEX_LIST);
		texturePoints = (vector<Vector2*>*) chopper->popData(TEXTURE_COORDINATES);

		meshFaceIndexVertexArray = new vector<Vector3*>(numFaces * 3);
		//meshFaceIndexVertexArray->set_used(numFaces * 3);
		//for ( int i = 0 ; i < numFaces * 3 ; i ++ )
        //   (*meshFaceIndexVertexArray)[i] = 0;

		int ii = 0;

		chopper->pushData(chopper->getID(), meshFaceIndexVertexArray);

		int numVertices = 3 * numFaces;
		ChunkChopper::NumVertices = numVertices;
		if (texturePoints != 0) 
		{
			textureTriangles = new vector<Vector2*>(numFaces * 3);
			//textureTriangles->set_used(numFaces * 3);
			for ( int i = 0 ; i < numFaces * 3 ; i++ )
				(*textureTriangles)[i] = 0;
		}

		for (int i = 0; i < numFaces; i++)
		{
			int vertexIndex = i * 3;
			int index0 = chopper->getUnsignedShort();
			int index1 = chopper->getUnsignedShort();
			int index2 = chopper->getUnsignedShort();

			(*facesIndex)[i] = vertexIndex;

			(*meshFaceIndexVertexArray)[vertexIndex] = (*coordinates)[index0];
			(*meshFaceIndexVertexArray)[vertexIndex + 1] = (*coordinates)[index1];
			(*meshFaceIndexVertexArray)[vertexIndex + 2] = (*coordinates)[index2];

			shareMap->addCoordinate( (*coordinates)[index0], vertexIndex);
			shareMap->addCoordinate( (*coordinates)[index1], vertexIndex + 1);
			shareMap->addCoordinate( (*coordinates)[index2], vertexIndex + 2);


			if (textureTriangles != 0)
			{
				(*textureTriangles)[vertexIndex] = (*texturePoints)[index0];
				(*textureTriangles)[vertexIndex + 1] = (*texturePoints)[index1];
				(*textureTriangles)[vertexIndex + 2] = (*texturePoints)[index2];
			}

			//This is a bit masked value that is used to
			//determine which edges are visible... not needed.
			int temp = chopper->getUnsignedShort();
		}
	}

	/***************************************************************************
	* Loads a mesh onto the scene graph with the specified data
	* from subchunks.
	* If there is no material, this will put a default
	* material on the shape.
	**************************************************************************/
	void initialize(ChunkChopper* chopper) 
	{
		int childIndex = 0;
		string* materialName = 0;
		string objfirstname = chopper->getCurNameOfObject();

		vector<Mesh*>* object3DVec = new vector<Mesh*>();

		int* polyMode = (int*) chopper->popData(NAMED_OBJECT_POLY_MODE);
		if ( polyMode == 0) 
		{
			polyMode = new int;
			*polyMode = 2;
		}

		bool* visibilityMode = (bool*) chopper->popData(NAME_OBJ_VISIBILITY);
		if ( visibilityMode == 0 ) 
		{
			visibilityMode = new bool;
			*visibilityMode = true;
		}


		// If there is no material groups than original faces index
		// is added in the Map
		if (chopper->getMaterialFaceIndicesMap().empty() )
		{
			chopper->getMaterialFaceIndicesMap().insert( pair<string , vector<unsigned>*>(chopper->getCurNameOfObject() + "<null" , facesIndex ));
		}

		// For Fliping the vertices of the mesh
		int n = coordinates->size();
		flipVertices(objfirstname, coordinates , chopper);

		// Smoothing group of the current mesh
		smoothGroups = (unsigned*) chopper->popData(SMOOTH);

		// According the smooth groups the smoothing normals are
		// calculated
		if (smoothGroups != 0) 
		{
			normals = generateNormals( meshFaceIndexVertexArray );
			smoothNormals(normals , shareMap, smoothGroups);

			// For saving the 3ds file
			//smoothGroupVec = new Vector();
			//chopper.getMaxModelDS().addMeshSmoothGroup(objfirstname, smoothGroupVec);
		}

		// Current mesh transformgroup
		Node* transformGroup = chopper->getGroup();

		// Creating Object3D according the material groups and if
		// material groups are not present than single object3D is
		// created per mesh
		map<string , vector<unsigned>* >& materialFaceMap = chopper->getMaterialFaceIndicesMap();

		map<string , vector<unsigned>* >::iterator it = materialFaceMap.begin();
		int ii = 0;
		int nn = materialFaceMap.size();
		//for (; it != it.atEnd(); ++it)
		while ( it != materialFaceMap.end()  ) 
			//while ( !materialFaceMap.getIterator().atEnd() ) 
		{
			ii++; 
			const string& meshMatName = it->first;
		
			// Craete Mesh for Each material
			Mesh* object3D = new Mesh();

			vector<unsigned>* curFacesIndex = it->second;

			if (ChunkChopper::NumVertices > 0) 
			{
                currentVerticesX = new vector<Vector3*>(curFacesIndex->size() * 3);
				currentTexCoord  = new vector<Vector2*>(curFacesIndex->size() * 3);
				// Copy vertices
				for (unsigned j = 0; j < curFacesIndex->size() ; j++)
				{
					unsigned index = (*curFacesIndex)[j];
					(*currentVerticesX)[j * 3]	   = (*meshFaceIndexVertexArray)[index ];
					(*currentVerticesX)[j * 3 + 1] = (*meshFaceIndexVertexArray)[index + 1];
					(*currentVerticesX)[j * 3 + 2] = (*meshFaceIndexVertexArray)[index + 2];

					if (textureTriangles != 0) {
						(*currentTexCoord)[j * 3] = (*textureTriangles)[index];
						(*currentTexCoord)[j * 3 + 1] = (*textureTriangles)[index + 1];
						(*currentTexCoord)[j * 3 + 2] = (*textureTriangles)[index + 2];
					}
				}

				int index = meshMatName.find_first_of( '<' );
				if (index != -1) 
				{
					materialName  = new string();
					*materialName = meshMatName.substr( index + 1, meshMatName.size() );
				} else
				{
					materialName = 0;
				}

				Appearance* appearance1 = chopper->getAppearance(*materialName);

				Appearance* appearance = 0;
				
				if( !appearance1 )
				{
					appearance1 = new Appearance();
				}
				//May 26 Normal flip fix
				if ( bFlipNormal )
				{
					appearance1->getPolygonAttributes().cullFace = ECF_FRONT;
				}
				else
				{
					appearance1->getPolygonAttributes().cullFace = ECF_BACK;
				}

				if ( appearance1 != 0 ) 
				{                
					string* textureName = 0;
					Texture* texture    = 0;

					if ( materialName )
					 textureName = chopper->getMatTextureName(*materialName);

					if ( textureName )
					 texture = chopper->getTextureImageMap(*textureName);

					if ( texture )
					{
					  //appearance->setTexture(0,texture);
					}                       
					
				} 
				
				vector<Vector3*>* curMeshSmoothNormals = 0;

				if ( smoothGroups )
				 curMeshSmoothNormals = curMeshSmoothingGroup(objfirstname, curFacesIndex, smoothingNormals, smoothGroups);


				int numVertices = 3 * curFacesIndex->size();
				int  numRemainingVertices = 0;

				object3D->setAppearance(appearance1);

				if ( (numVertices - NUM_MAX_VERTICES) > 0 )
				{
                    int start = 0;
     				int end   = NUM_MAX_VERTICES;

					do
					{
						SMeshBuffer* splitMeshBuffer = new SMeshBuffer(end-start,COORDINATES | NORMALS | TEXTURE_COORDINATE_2);
						int cnt1 = 0;
						int cnt2 = 0;

						for ( int i = start ; i < end ; i ++ )
						{
							float s = 0.0f;
							float t = 0.0f;

							if ( textureTriangles )
							{
								s = (*currentTexCoord)[i]->x;
								t = (*currentTexCoord)[i]->y;
							}
							sf32 *bufferCoordinates = splitMeshBuffer->getCoordinates();
							bufferCoordinates[cnt1] = (*currentVerticesX)[i]->x;
							bufferCoordinates[cnt1+1] = (*currentVerticesX)[i]->y;
							bufferCoordinates[cnt1+2] = (*currentVerticesX)[i]->z;

							sf32 *bufferTexCoordinates = splitMeshBuffer->getTexCoords(0);
							bufferTexCoordinates[cnt2] = s;
							bufferTexCoordinates[cnt2+1] = t;
							cnt1 +=3;
							cnt2 +=2;
						}
						cnt1 = cnt2 = 0;

						splitMeshBuffer->setPrimitiveType(TRIANGLES);
						splitMeshBuffer->reCalculateAABB();

						if (smoothGroups == 0)
						{
							NormalGenerator* normalGenerator = new NormalGenerator();
							normalGenerator->generateNormals( splitMeshBuffer );
							delete normalGenerator;
							normalGenerator = 0;
						} 

						if ( curMeshSmoothNormals )
						{
							sf32 *bufferNormals = splitMeshBuffer->getNormals();
							for ( int i = start ; i < end ; i ++ )
							{
								bufferNormals[cnt1] = (*curMeshSmoothNormals)[i]->x ;
								bufferNormals[cnt1+1] = (*curMeshSmoothNormals)[i]->y ;
								bufferNormals[cnt1+2] = (*curMeshSmoothNormals)[i]->z ;
								cnt1 +=3;
							}
						}
						

						start += NUM_MAX_VERTICES;
						numRemainingVertices = numVertices - start;
						int threshold = ( numRemainingVertices > NUM_MAX_VERTICES ) ? NUM_MAX_VERTICES : numRemainingVertices;
						end   = start + threshold ;

						object3D->addMeshBuffer ( splitMeshBuffer );
						
					}while ( numRemainingVertices  > 0 );


				}

				else
				{
					SMeshBuffer* meshBuffer = new SMeshBuffer(numVertices,COORDINATES | NORMALS | TEXTURE_COORDINATE_2);
					int cnt1 = 0;
					int cnt2 = 0;

					for ( int i = 0 ; i < numVertices ; i ++ )
					{
						float s = 0.0f;
						float t = 0.0f;

						if ( textureTriangles )
						{
							s = (*currentTexCoord)[i]->x;
							t = (*currentTexCoord)[i]->y;
						}

						sf32 *bufferCoordinates = meshBuffer->getCoordinates();
						bufferCoordinates[cnt1] = (*currentVerticesX)[i]->x;
						bufferCoordinates[cnt1+1] = (*currentVerticesX)[i]->y;
						bufferCoordinates[cnt1+2] = (*currentVerticesX)[i]->z;

						sf32 *bufferTexCoordinates = meshBuffer->getTexCoords(0);
						bufferTexCoordinates[cnt2] = s;
						bufferTexCoordinates[cnt2+1] = t;
						cnt1 +=3;
						cnt2 +=2;

					}
					
					meshBuffer->setPrimitiveType(TRIANGLES);
					meshBuffer->reCalculateAABB();

					if (smoothGroups == 0)
					{
						NormalGenerator* normalGenerator = new NormalGenerator();
						normalGenerator->generateNormals( meshBuffer );
						delete normalGenerator;
						normalGenerator = 0;
					} 

					if ( curMeshSmoothNormals )
					{
						cnt1 = 0;
						sf32 *bufferNormals = meshBuffer->getNormals();
						for ( int i = 0 ; i < numVertices ; i ++ )
						{
							bufferNormals[cnt1] = (*curMeshSmoothNormals)[i]->x ;
							bufferNormals[cnt1+1] = (*curMeshSmoothNormals)[i]->y ;
							bufferNormals[cnt1+2] = (*curMeshSmoothNormals)[i]->z ;
							cnt1 +=3;
						}
					}

					object3D->addMeshBuffer(meshBuffer);
				}

				MeshNode* meshGroup = (MeshNode*)(transformGroup);
				meshGroup->addMesh(object3D);
				object3DVec->push_back ( object3D );
				//tarray = null;
			}
			it++;
		}



		//if ( node )
		//{
		//	//vector<Node*>& vecChildren = node->getChildren();
		//	//MeshNode* meshGroup = dynamic_cast<MeshNode*>(vecChildren[0]);
		//	if ( meshGroup )
		//	{
		//		//meshGroup->setName(*materialName);
		//		//object3D->setHardwareMappingHint( EHM_STATIC , EBT_VERTEX_AND_INDEX );
		//		meshGroup->setMesh(object3D);
		//	}
		//	childIndex++;
		//}
		//chopper->getShape3DList().insert( pair<string, vector<Mesh*>*>(objfirstname, object3DVec));
		chopper->getShape3DList()[objfirstname] = object3DVec;
		// To null the Unused Varaibles of NameObj

		free(); 

	}






private:
	/***************************************************************************
	* Flip X coordinate of vertices if mesh matrix has negative determinant
	* @param Mesh Name
	* @param Vertices
	* @param Chunkchopper
	**************************************************************************/
	void flipVertices(string& objfirstname, vector<Vector3*>* currentVertices, ChunkChopper* chopper) 
	{
		if ( chopper->getFlipVertMatrixMap().find(objfirstname) != chopper->getFlipVertMatrixMap().end() ) 
		{
			return;
		}

		Matrix4f* tempt3d = 0;
		if ( chopper->namedObjectCoordinateSystems.find(objfirstname) != chopper->namedObjectCoordinateSystems.end() )
			tempt3d = (Matrix4f*) (chopper->namedObjectCoordinateSystems.find(objfirstname))->second;

		if (tempt3d == 0 )
		{
			return;
		}


		float det = getDeterminant( *tempt3d );

		// Flip X coordinate of vertices if mesh matrix
		//has negative determinant 
		if ( det < 0.0) 
		{
			//May 26 Normal flip fix
			bFlipNormal = true;
			Matrix4f inv_matrix ;
			Matrix4f M ;
			Vector3 tmp ;

			inv_matrix.multiply(*tempt3d);
			inv_matrix = inv_matrix.getInverse();

			M = *tempt3d;

			matrix_scale_xyz(M, -1.0f, 1.0f, 1.0f);

			M.multiply(inv_matrix);

			//Saving flip vertice Matrix for file Writer.
			chopper->addFlipVertMatrixMap(objfirstname , &M );

			for (unsigned i = 0; i < currentVertices->size() ; ++i ) 
			{
				M.transformPoint( *( (*currentVertices)[i]) );
			}
			//tmp = null;
			//tempt3d = null;
		}
	}

	/////////////////////
	float getDeterminant(Matrix4f& M)
	{
		float d = ( M.m00 * M.m11 - M.m01 * M.m10 ) * (M.m22 * M.m33 - M.m23 * M.m32) -
				(M.m00 * M.m12 - M.m02 * M.m10) * (M.m21 * M.m33 - M.m23 * M.m31 ) +
				(M.m00 * M.m13 - M.m03 * M.m10) * (M.m21 * M.m32 - M.m22 * M.m31) +
				(M.m01 * M.m12 - M.m02 * M.m11) * (M.m20 * M.m33 - M.m23 * M.m30) -
				(M.m01 * M.m13 - M.m03 * M.m11) * (M.m20 * M.m32 - M.m22 * M.m30) +
				(M.m02 * M.m13 - M.m03 * M.m12) * (M.m20 * M.m31 - M.m21 * M.m30);

		return d;
	}
	/***************************************************************************
	* Matrix of flip vertices
	* @param t3d
	* @param x
	* @param y
	* @param z
	**************************************************************************/
	void matrix_scale_xyz(Matrix4f& t3d, float x, float y, float z) 
	{
		t3d.m00  *= x;
		t3d.m10  *= x;
		t3d.m20  *= x;
		t3d.m30  *= x;

		
		t3d.m01  *= y;
		t3d.m11  *= y;
		t3d.m21  *= y;
		t3d.m31  *= y;
		
		t3d.m02  *= z;
		t3d.m12  *= z;
		t3d.m22  *= z;
		t3d.m32  *= z;
	}


	/***************************************************************************
	* Current Mesh have 1 or more materials so according
	* to materials in single mesh the smoothing normals are
	* arranged.
	* @param Current NamedObject name
	* @param Current faces index according the material group
	* @param Whole mesh smoothing normals
	* @param Current mesh smoothing group(Index of smoothing Normals)
	* @return Smoothing normals according to material group
	**************************************************************************/
	vector<Vector3*>* curMeshSmoothingGroup(string meshName, vector<unsigned>* curFacesIndex , vector<Vector3*>* smoothNormals, unsigned* smoothGroups) 
	{
		//curObject3dSmoothGrp = new vector3df[numFaceIndex * 3];
		if (curFacesIndex != 0) 
		{
			if ( curFacesIndex > 0) 
			{
				curObject3dSmoothGrp = new vector<Vector3*>(curFacesIndex->size() * 3);
				//curObject3dSmoothGrp->set_used(curFacesIndex->size() * 3);

				for (unsigned j = 0; j < curFacesIndex->size() ; j++)
				{
					unsigned index = (*curFacesIndex)[j];
					(*curObject3dSmoothGrp)[j * 3] = (*smoothNormals)[index];
					(*curObject3dSmoothGrp)[j * 3 + 1] = (*smoothNormals)[index + 1];
					(*curObject3dSmoothGrp)[j * 3 + 2] = (*smoothNormals)[index + 2];
					smoothGroupVec.push_back( smoothGroups[index]);
				}
			}
		}
		return curObject3dSmoothGrp;
	}

	/***************************************************************************
	* Takes all the normals for all the vertices and averages them with
	* normals with which they share a coordinate and at least one smooth group.
	* @param currentNormals the normals for each face.
	* @param sharedPoints the point mapper that will choose which points are
	* and which are not shared.
	* @param smoothGroups the indexed list of group masks loaded by the smooth chunk.
	* @return normals averaged among the shared vertices in their smoothing groups.
	**************************************************************************/
	void smoothNormals(vector<Vector3*>* currentNormals,  PointMapper* sharedPoints, unsigned* smoothGroups) 
	{
		smoothingNormals = new vector<Vector3*>(currentNormals->size());
		//smoothingNormals->set_used(	currentNormals->size() );

		for (unsigned i = 0; i < currentNormals->size(); i++) 
		{
			IntArrayReference otherPoints = sharedPoints->getSharedCoordinates(i, smoothGroups);

			int numNormals = (*otherPoints).size();

			if ( (otherPoints != 0) && ( numNormals > 0) ) 
			{
				sharedNormals = new vector<Vector3*>();
				sharedNormals->resize( numNormals );

				set<int>::iterator it;

				int j=0;
				for ( it=otherPoints->begin() ; it != otherPoints->end(); it++ )
				{
					(*sharedNormals)[j] = (*currentNormals)[(*it)];
					j++;
				}

				(*smoothingNormals)[i] = averageNormals( sharedNormals );
				//sharedNormals = null;
				//otherPoints.clear();
			}
			else 
			{
				(*smoothingNormals)[i] = (*currentNormals)[i];
			}
			//otherPoints = null;
		}
	}

	/***************************************************************************
	* Averages the normals provided in order to provide
	* smooth, noncreased appearances for meshes.
	* @param normals the normals that should be averaged
	* @return a normalized normal that can be used in place
	* of all the normals provided.
	**************************************************************************/
	Vector3* averageNormals(vector<Vector3*>* normals ) 
	{
		Vector3* newNormal = new Vector3();
		for (unsigned i = 0; i < normals->size(); i++) 
		{
			*newNormal += *( (*normals)[i] );
			//normals[i] = null;
		}
		newNormal->normalize();
		return newNormal;
	}

	/***************************************************************************
	* Generates normals for each vertex of each
	* face that are absolutely normal to the face.
	* @param point0 The first point of the face
	* @param point1 The second point of the face
	* @param point2 The third point of the face
	* @return the three normals that should be
	* used for the triangle represented by the parameters.
	**************************************************************************/
	vector<Vector3*>* generateNormals(vector<Vector3*>* points ) 
	{
		normals = new vector<Vector3*>(points->size() );
		//normals->set_used( points->size() );
		Vector3 v1 ;
		Vector3 v2 ;
		for (unsigned i = 0; i < points->size(); )
		{
			Vector3* normal = new Vector3;

			normal->x = 0.0f;
			normal->y = 0.0f;
			normal->z = 0.0f;

			v1.x = 0.0f; v2.x = 0.0f;
			v1.y = 0.0f; v2.y = 0.0f;
			v1.z = 0.0f; v2.z = 0.0f;

			v1.sub ( *( (*points)[i + 1] ),  *( (*points)[i] ) );
			v2.sub ( *( (*points)[i + 2] ), *( (*points)[i] ) );
			
			if ( !bFlipNormal )
			{
				*normal = v1.cross(v2);
			}
			else
			{
				*normal = v2.cross(v1);
			}

			//if ( normal->getLength() > 1e-7) 
			{
				normal->normalize();
			}

			(*normals)[i++] = normal;
			(*normals)[i++] = normal;
			(*normals)[i++] = normal;
		}
		//release
		//normal = null;
		//v1 = null;
		//v2 = null;
		return normals;
	}

	/***************************************************************************
	* To release the memory which are not used for longer
	**************************************************************************/
	void free() 
	{
		
		if ( meshFaceIndexVertexArray ) 
		{
			meshFaceIndexVertexArray->clear() ;
			meshFaceIndexVertexArray->resize(0);
			
			//for ( int i = 0 ; i < meshFaceIndexVertexArray->size(); i ++ )
			//	delete (*meshFaceIndexVertexArray)[i];
			meshFaceIndexVertexArray = 0;
		}

		if ( smoothingNormals ) 
		{
			smoothingNormals->clear() ;
			//smoothingNormals->set_used(0);
			smoothingNormals = 0;
		}

		chopper->getMaterialFaceIndicesMap().clear();

		if ( facesIndex )
		{
			facesIndex->clear() ;
			//facesIndex->set_used(0);
			facesIndex = 0;
		}

		if ( coordinates )
		{
			coordinates->clear() ;
			//coordinates->set_used(0);
			coordinates = 0;
		}

		if ( textureTriangles )
		{
			textureTriangles->clear();
			//textureTriangles->set_used(0);
			textureTriangles = 0;
		}

		if ( texturePoints )
		{
			texturePoints->clear();
			//texturePoints->set_used(0);
			texturePoints = 0;
		}

		if ( normals )
		{
			normals->clear();
			//normals->set_used(0);
			normals = 0;
		}

		if ( smoothGroups )
		{
			delete[] smoothGroups;
			smoothGroups = 0;
		}

		if ( currentVerticesX )
		{
			currentVerticesX->clear();
			//currentVerticesX->set_used(0);
            currentVerticesX = 0;
		}

		if ( currentTexCoord )
		{
			currentTexCoord->clear();
			//currentTexCoord->set_used(0);
            currentTexCoord = 0;
		}

		shareMap->free();

	}



};

#endif 
