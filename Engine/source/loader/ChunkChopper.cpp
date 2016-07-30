#include "loader/3dsloader/ChunkChopper.h"
#include "loader/3dsloader/ChunkMap.h"
#include "loader/3dsloader/KeyFramer.h"


int   ChunkChopper::NumVertices         = 0;
long  ChunkChopper::FrameCnt            = 0;
int   ChunkChopper::fatherID            = 0;
int   ChunkChopper::hierarchyID         = -1;
AABB ChunkChopper::Bbox ;



ChunkChopper::ChunkChopper(SceneManager* smgr)
{
	loader             = 0; 
	defaultHierarchyId = 0 ;
	filesize           = 0;
	mNumModels         = 1;
	curModelNum        = 0;
	resizeDivisor      = 1.0f;
	mainChunk          = new Chunk("MainChunk");
	chunkMap		   = new ChunkMap(mainChunk);
	keyFramer          = new KeyFramer(this);
	sceneGroup         = new Node(0,0);
	sceneBase          = new SceneBase();
	this->smgr         = smgr;
}


Scene* ChunkChopper::loadSceneBase(const char* fileName, Loader3DS* loader)
{
	this->loader        = loader;

   	sceneBase->setSceneNode( sceneGroup );

	getByteBuffer( fileName );

	int mainChunkID         = chunkBuffer->getShort();
    long mainChunkLength    = chunkBuffer->getInt();
	
	loadSubChunks(mainChunk, 0);


	return sceneBase;
}

char* ChunkChopper::getByteBuffer(const char* fileName)
{
	FILE *fp;
	long len;
	fp = fopen( fileName, "rb");
	fseek(fp , 0 , SEEK_END); //go to end
	len = ftell ( fp ); //get position at end (length)
	fseek( fp , 0 , SEEK_SET); //go to beg.
	chunkBuffer = new ChunkBuffer(len);
	char* buf = chunkBuffer->getData();
	fread( buf , len , 1 , fp ); //read into buffer
	fclose(fp);
	return buf;
}


void ChunkChopper::setModelBbox(AABB& Bbox)
{
	//ChunkChopper.Bbox = Bbox;
}


AABB& ChunkChopper::getModelBbox()
{
	return Bbox;
}


KeyFramer* ChunkChopper::getKeyFramer()
{
	return keyFramer;
}


void ChunkChopper::addObject(string& name, Node* group)
{
	sceneGroup->addChild(group);
	currentGroup = static_cast<MeshNode*>(group);
	currentObjectName = name;
	sceneBase->addNamedObjects(name, group);
}


string ChunkChopper::getObjectName()
{
	return currentObjectName;
}


void ChunkChopper::setObjectName(string name)
{
	currentObjectName = name;
}


Node* ChunkChopper::getGroup()
{
	return currentGroup;
}


void ChunkChopper::pushData(int key, void* data)
{
	//dataMap.insert( pair<int,void*>(key , data) );
	dataMap[key] = data;
}


void* ChunkChopper::popData(int key)
{
	if ( dataMap.find ( key ) != dataMap.end() )
	{
		void* value = ( dataMap.find ( key ) )->second;
		dataMap.erase(key);
		return value;
	}
	return 0;
}


void ChunkChopper::setNamedObject(string key, Node* value)
{
	sceneBase->addNamedObjects( key , value );
}

bool ChunkChopper::hasLights()
{
	//return (base.getLightNodes() != null && base.getLightNodes().length > 0);
	return true;
}

//void addBehaviorNode(Behavior behavior)
//{
//base.addBehaviorNode(behavior);
//}

//void addLightNode(Light light)
//{
//    base.addLightNode(light);
//}

//void addViewGroup(TransformGroup viewGroup)
//{
//    base.addViewGroup(viewGroup);
//}

Node* ChunkChopper::getNamedObject(string key)
{
	return sceneBase->getNamedObject( key);
}


Node* ChunkChopper::getNamedTransformGroup(string& key)
{
	Node* object = getNamedObject(key);
	
    Node* node = dynamic_cast<Node*>(object);

	if ( node )
	{
	  return node;
	}
	else if ( node == 0)
	{
	  return 0;
	}
	return 0;
}

long ChunkChopper::getLong()
{
	return chunkBuffer->getLong();
}

/***************************************************************************
* Reads a short and returns it as a signed
* int.
* @return Short Value
**************************************************************************/
int ChunkChopper::getShort()
{
	return chunkBuffer->getShort();
}

/***************************************************************************
* Reads a short and returns it as an unsigned
* int.
* @return Unsigned Short Value
**************************************************************************/
int ChunkChopper::getUnsignedShort()
{
	return chunkBuffer->getShort() & 0xFFFF;
}

/***************************************************************************
* reads a float from the chunkBuffer.
* @return Float Value
**************************************************************************/
float ChunkChopper::getFloat()
{
	return chunkBuffer->getFloat();
}

/***************************************************************************
* Reads 3 floats x,z,y from the chunkbuffer.
* Since 3ds has z as up and y as pointing in whereas
* java3d has z as pointing forward and y as pointing up;
* this returns new Vector3f(x,-z,y)
* @return Vector3f
**************************************************************************/
Vector3 ChunkChopper::getVector()
{
	Vector3 v(getPoint());
	return v;
}

/***************************************************************************
* Reads 3 floats x,z,y from the chunkbuffer.
* Since 3ds has z as up and y as pointing in whereas
* java3d has z as pointing forward and y as pointing up;
* this returns new Point3f(x,-z,y)
* @return Poimt3f
**************************************************************************/
Vector3 ChunkChopper::getPoint()
{
	float x = chunkBuffer->getFloat();
	float z = -chunkBuffer->getFloat();
	float y = chunkBuffer->getFloat();
	Vector3 v( x , y , z);
	return v;
}

/***************************************************************************
* Reads an int and returns it
* @return Int Value
**************************************************************************/
int ChunkChopper::getInt()
{
	return chunkBuffer->getInt();
}

/***************************************************************************
* Reads an int and returns it
* unsigned, any ints greater than MAX_INT
* will break.
* @return Unsigned Int Value
**************************************************************************/
int ChunkChopper::getUnsignedInt()
{
	return chunkBuffer->getInt() & 0xFFFFFFFF;
}

/***************************************************************************
* Reads a byte, unsigns it, returns the corresponding int.
* @return Unsigned Byte value
**************************************************************************/
int ChunkChopper::getUnsignedByte()
{
	return chunkBuffer->getChar() & 0xFF;
}

/***************************************************************************
* Reads a number of bytes corresponding to the
* number of bytes left in the current chunk and returns an array
* containing them.
* @return an array containing all the bytes for the current chunk.
**************************************************************************/
char* ChunkChopper::getChunkBytes()
{
	int size = chunkBuffer->limit() - chunkBuffer->position();
	char* retVal = new char[size];
	chunkBuffer->get(retVal , size);
	return retVal;
}

/***************************************************************************
* Fills bytes with data from the chunk buffer.
* @param bytes the array to fill with data.
**************************************************************************/
void ChunkChopper::get(char* bytes , int size)
{
	chunkBuffer->get(bytes , size);
}

/***************************************************************************
* Sets the data map used to store values
* that chunks may need to retrieve later.
* @param dataMap the hashmap that will be used to store
* and retrieve values for use by chunks.
**************************************************************************/
void ChunkChopper::setDataMap(map<int , void*>& dataMap)
{
	//this->dataMap = dataMap;
}

/***************************************************************************
* This reads bytes until it gets 0x00 and returns
* the corresponding string.
* @return Return String
**************************************************************************/
string ChunkChopper::getString()
{
	string string ;
	char charIn = chunkBuffer->getChar();
	while (charIn != 0x00)
	{
    string += charIn; 
	charIn = (char) chunkBuffer->getChar();
	}
	int n = chunkBuffer->position();
	return string;
}

/***************************************************************************
* Gets the id of the current chunk.
* @return id of the current chunk as read
* from the chunkBuffer.  It will be a signed <code>short</code>.
**************************************************************************/
int ChunkChopper::getID()
{
	return chunkID;
}

/***************************************************************************
* Loads the image to server as a texture.
* @param textureImageName name of the image that
* is going to be set to be the texture.
* @return Texture Class instance
**************************************************************************/
Texture* ChunkChopper::createTexture(string& textureImageName)
{
	Texture* texture = 0;
	
	//texture = smgr->getVideoDriver()->getTexture( textureImageName );  
  
	return texture;
}

/***************************************************************************
* Get Scene Branchgroup    
* @return
**************************************************************************/
Node* ChunkChopper::getSceneGroup()
{
	return sceneGroup;
}

/***************************************************************************
* Put appeareance info in map as a object and material name as a key
* @param key Material Name
* @param Value Appeareance data
**************************************************************************/
void ChunkChopper::setAppearanceMap(string key, Appearance* Value)
{
	//appearanceMap.insert(pair<string,Appearance*>(key, Value));
	appearanceMap[key] = Value;
}

/***************************************************************************
* Get Appeaeance of the material
* @param key Material Name
* @return Appeareance Data
**************************************************************************/
Appearance* ChunkChopper::getAppearance(string key)
{
	if ( appearanceMap.find(key) !=  appearanceMap.end() )
	  return appearanceMap.find(key)->second;

	return 0;
}

/***************************************************************************
* Get MaxDataStructure reference   
* @return MaxDataStructure reference
**************************************************************************/
/*MaxDataStructure getMaxModelDS()
{
return maxDS;
}*/

/***************************************************************************
* Put Mesh flip matrix of the matrix reqired by exporter
* @param meshName
* @param t3d
* @param Current Mesh Name
* @param Transform3d (Flip vertices matrix)
**************************************************************************/
void ChunkChopper::addFlipVertMatrixMap(string meshName, Matrix4f* t3d)
{
	if ( flipVertexMatrixMap.find(meshName) != flipVertexMatrixMap.end() )
	{
	    //flipVertexMatrixMap.insert(pair<string,Matrix4f*>(meshName, t3d));
		flipVertexMatrixMap[meshName] = t3d;
	}
}

/***************************************************************************
* Get Mesh Flip Marix Hashmap
* @return  Hashmap
**************************************************************************/
map<string, Matrix4f*>& ChunkChopper::getFlipVertMatrixMap()
{
	return flipVertexMatrixMap;
}

/***************************************************************************
* Put Material name as key and texture name as a object
* @param matName Material Name
* @param textureName Texture Name
**************************************************************************/
void ChunkChopper::addMatTextureNameInMap(string matName, string textureName)
{
	string* name = new string();
	*name = textureName;
	//MatTextureNameMap.insert(pair<string,string*>(matName, name));
	MatTextureNameMap[matName] = name;
}

/***************************************************************************
* Get texture name of that Material
* @param matName Material Name
* @return Texture Name
**************************************************************************/
string* ChunkChopper::getMatTextureName(string& matName)
{
	if ( MatTextureNameMap.find ( matName ) != MatTextureNameMap.end() )
	return MatTextureNameMap.find ( matName )->second;
}

/***************************************************************************
* Add texture as a data and texture name as a key in a hashmap
* @param textureName Texture Name
* @param texture Texture Data
**************************************************************************/
void ChunkChopper::addTextureImageMap(string textureName, Texture* texture)
{
   textureImageMap[textureName] = texture;
}

/***************************************************************************
* Get Texture
* @param textureName Texture Name
* @return Texture Data
**************************************************************************/
Texture* ChunkChopper::getTextureImageMap(string& textureName)
{
	if ( textureImageMap.find(textureName) != textureImageMap.end() )
		return textureImageMap.find(textureName)->second;
	return 0;
}

/***************************************************************************
* Get All Object3d's, Key : group Name, Entry : Object3d
* @return All Object3d's Hashmap
**************************************************************************/
map< string , vector<Mesh*>* >& ChunkChopper::getShape3DList()
{
	return Shape3DList;
}

map<string , vector<MeshNode*>* >& ChunkChopper::getMeshNodeList()
{
	return MeshNodeList;
}

/***************************************************************************
* Set current Mesh Name
* @param curMeshNameObj Current Mesh Name
**************************************************************************/
void ChunkChopper::setCurNameOfObject(string curMeshNameObj)
{
	this->curMeshNameObj = curMeshNameObj;
}

/***************************************************************************
* Get current Mesh name
* @return Current Mesh Name
**************************************************************************/
string ChunkChopper::getCurNameOfObject()
{
	return curMeshNameObj;
}

/***************************************************************************
* Get Hashmap of Faces indices of material
* @return Hashmap
**************************************************************************/
map<string , vector<unsigned>* >& ChunkChopper::getMaterialFaceIndicesMap()
{
	return materialFaceIndicesMap;
}

/***************************************************************************
* Get Current Material name
* @return Current Material Name
**************************************************************************/
string ChunkChopper::getCurMatName()
{
	return curMatName;
}

/***************************************************************************
* Set current Material Name
* @param curMatName Current Material Name
**************************************************************************/
void ChunkChopper::setCurMatName(string curMatName)
{
	this->curMatName = curMatName ;
}

/***************************************************************************
* Get Default hierarchy Id
* @return Hierarchy Id of current Mesh
**************************************************************************/
int ChunkChopper::getDefaultHeirarchyId()
{
	defaultHierarchyId++;
	return defaultHierarchyId;
}

/***************************************************************************
* Get faces indices of current Material
* @return Int Array of Current faces Indices According Material
**************************************************************************/
int* ChunkChopper::getMatGroupFacesIndexArray()
{
	return facesMatIndex;
}

/***************************************************************************
* Get hashmap of change material name and original name
* @return Material Name Hashmap
**************************************************************************/
map<string , string>& ChunkChopper::getChangeMatNameMap()
{
	return changeMatNameMap;
}

/***************************************************************************
* Get hashmap of change Texture name and original name
* @return Texture Name hashmap
**************************************************************************/
map<string , string>& ChunkChopper::getChangeTexNameMap()
{
	return changeMatNameMap;
}

/***************************************************************************
* Get mesh vertices list
* @param meshname Mesh Name
* @return Vertices list
**************************************************************************/
vector<Vector3>* ChunkChopper::getMeshVertices(string meshname)
{
	if ( meshVerticesList.find ( meshname ) != meshVerticesList.end() )
	{
		vector<Vector3>* vertices = ( meshVerticesList.find ( meshname ) )->second;
		meshVerticesList.erase(meshname);	
		return vertices;
	}
	//return (Point3f[]) meshVerticesList.remove(meshname);
	return 0;
}

/*******************************************************************************
* Put Mesh name as a key and vertices list as a object
* @param meshName Mesh name
* @param vertices Vertices List
******************************************************************************/
void ChunkChopper::setMeshVertices(string meshName, vector<Vector3>* vertices)
{
	 //meshVerticesList.insert( pair<string,vector<Vector3>*>(meshName, vertices));
 	 meshVerticesList[meshName] = vertices;
}


/***************************************************************************
* The base class Chunk takes care of loading subchunks for
* all chunks types.  It occurs as follows:
* <ol>
*     <li>The chunk id (short) is read
*     <li>The chunk length(int) is read
*     <li>A subchunk is looked up from the map of publish
*         subchunk types of the current chunk.
*     <li>If it isn't found during the lookup it is skipped.
*     <li>Otherwise it is requested to {@link #pushData}
*     <li>The return value, if there is one, is used to determine
*         where its next subchunk is. A return value of 0 signifies
*         that the next subchunk is nigh.
*     <li>The chunk's subchunks are then loaded.
*     <li>The chunks initialize method is called.
* </ol>
* @param parentChunk Father ChunkID
* @param level Level Of SunChunks
* @throws CannotChopException
*/
void ChunkChopper::loadSubChunks(Chunk* parentChunk, int level) 
{
	float count;
	level++;

	//int size = chunkBuffer->limit();

	bool flag = chunkBuffer->hasRemaining();
	while ( chunkBuffer->hasRemaining() )//hasRemaining() evaluates limit - position.
	{
		float curPos    = chunkBuffer->position();
		chunkID         = chunkBuffer->getShort();

		//if (chunkID != null)
		{
			chunkIdList.push_back(chunkID);
		}            

		if (chunkID == -20480) { // Keyframer ChunkID
		}

		if ( chunkID == (short)0xB011 )
		{
			int kk = 0;
		}
		Chunk* chunk = parentChunk->getSubChunk(chunkID);

		int currentChunkLength  = chunkBuffer->getInt() - 6;  //length includes this 6 byte header.
		int finishedPosition    = chunkBuffer->position() + currentChunkLength;
		int previousLimit       = chunkBuffer->limit();

		chunkBuffer->limit(chunkBuffer->position() + currentChunkLength);

		
		if (chunk != 0 && currentChunkLength != 0) 
		{
			//try 
			{
				chunk->loadData(this);
			} 
			/*catch (BufferUnderflowException e) 
			{
				chunkBuffer.position(finishedPosition);
				chunkBuffer.limit(previousLimit);
				throw new CannotChopException(" tried to read too much data from the buffer. Trying to recover.", e);
			}*/

			//try 
			//{
				if (chunkBuffer->hasRemaining())
				{
					loadSubChunks(chunk, level);
				}
				chunk->initialize(this);
			//} 
			/*catch (CannotChopException e) 
			{                    
				logger.log(Level.SEVERE, chunk.toString() + "Trying to continue");
			}*/
		}

		chunkBuffer->position(finishedPosition);
		chunkBuffer->limit(previousLimit);
	}
}

/***************************************************************************
* To release the memory
**************************************************************************/
void ChunkChopper::free()
{
	/*chunkBuffer = null;
	chunkID = null;
	currentObjectName = null;
	chunkTester = null;

	// JUN03 MEM LEAK FIX
	chunkMap    = null;
	mainChunk   = null;
	keyFramer   = null;

	appearanceMap.clear();
	appearanceMap = null;

	Shape3DList.clear();
	Shape3DList = null;
	curMeshNameObj = null;

	materialFaceIndicesMap.clear();
	materialFaceIndicesMap = null;

	facesMatIndex = null;

	MatTextureNameMap.clear();
	MatTextureNameMap = null;

	changeMatNameMap.clear();
	changeMatNameMap = null;

	textureImageMap.clear();
	textureImageMap =null;

	dataMap.clear();
	dataMap = null;*/


}


