#ifndef _CHUNKCHOPPER_H

#define _CHUNKCHOPPER_H

/*******************************************************************************
 * A singleton flyweight factory responsible for chopping the data up and sending
 * it to the corresponding chunks(which are flyweights ala the flyweight pattern)
 * for processing.
 * This will sequentially read a 3ds file, load or skip chunks and subchunks and
 * initialize the data for the chunks.
 * <p>
 * Retrieved data may be stored as state in the ChunkChopper
 * via {@link #pushData} for use by other chunks.
 * <p>
 * Features not supported; unknown chunks are skipped.
 ******************************************************************************/
#include "Chunk.h"
#include "Loader3DS.h"
//#include "CustomMeshSceneNode.h"
//#include "CEmptySceneNode.h"
//#include "CDummyTransformationSceneNode.h"
#include "MeshNode.h"
#include "ChunkBuffer.h"
#include "SceneBase.h"

#include <vector>

using namespace std;

class ChunkMap;
class KeyFramer;


class ChunkChopper
{

    ////////////////////////////////////////////////////////////////////////////
    //Private Varaibles ////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
private :
	//Logger          logger      = Logger.getLogger(ChunkChopper.class.getName());
    Chunk*           mainChunk ;  
    ChunkMap*        chunkMap  ;  

    //filemanager.FileManager filemanager = null;

    //TextureLoader   textureLoader           = null;
	  SceneManager* smgr;
      Loader3DS*       loader                   ;
      Node*				 sceneGroup      ;
	  ChunkBuffer*           chunkBuffer ;
      SceneBase*       sceneBase  ;
      //ByteBuffer      chunkBuffer             = null;
      int             chunkID                  ;
      MeshNode*     currentGroup        ;
      string           currentObjectName        ;
      vector<int>      chunkIdList              ;
      string           curMeshNameObj           ;
      int*            facesMatIndex            ;
      int             defaultHierarchyId       ;
      map< string , void* > baseMap            ;
	  map< int , void* >  dataMap			   ;
	  map<string , Matrix4f*> flipVertexMatrixMap ;
      map<string , string*>   MatTextureNameMap   ;
	  map<string , vector<Vector3>* >  meshVerticesList;
      map<string , vector<unsigned>* >  materialFaceIndicesMap;
      map<string , Texture*> textureImageMap;
      map<string , Appearance*> appearanceMap;
      map<string , vector<Mesh*>* > Shape3DList;
	  map<string , vector<MeshNode*>* > MeshNodeList;
      long            filesize                ;
      int             mNumModels              ;
      int             curModelNum             ;
      //ReadableByteChannel channel             = null;

	  bool         finish                     ;

public:
	    /** KeyFramer Class reference */
    KeyFramer*       keyFramer;



    ////////////////////////////////////////////////////////////////////////////
    //Static Variables /////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
	static int NumVertices;
    /**Global Bounding Box of model*/
	static AABB   Bbox ;
    /**Debug flag*/
    static bool  debug   ;
    //Current chunk for which debugging info is viewed if debug == true
    /**Debug chunk, Current chunk for which debugging info is viewed if debug == true*/
    static Chunk debugChunk;
    /**Scale Factor of model*/
    static float scalefactor;

    /**Current model frame count , total no of frames for animation*/
    static long  FrameCnt           ;
    /**FatherId variable*/
    static int   fatherID           ;
    /**hierarchyId variable*/
    static int   hierarchyID        ;

    ////////////////////////////////////////////////////////////////////////////
    //Public Varaibles /////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    // Variable orfor resize
    /**Resize divisor variable*/
    float            resizeDivisor          ; // Resize executes -> Computes divisor and stores here
    /**MaxDataStructure Reference variable*/
    //MaxDataStructure maxDS                   = null;
    /**Current Material name variable*/
    string  curMatName ;
    /**Change Material Name Hashmap variable*/
    map<string , string> changeMatNameMap ;
    /**Resize divisor variable*/
	map<string , Matrix4f*> namedObjectCoordinateSystems;  // JULY25
 
	/*
	Constructor
	*/
    ChunkChopper(SceneManager* smgr);
   

    /***************************************************************************
     * This sequentially parses the chunks out of the input stream and
     * constructs the 3D entities represented within.
     * A Chunk is a little endian data structure consists of a
     * 6 byte header followed by subchunks and or data.
     * The first short int(little endian) represent the id
     * of the chunk.  The next int represent the total
     * length of the chunk(total of data, subchunks and chunk header).
     * <p>
     * The first chunk is the main chunk (id=4D4D) and its length
     * is always the length of the file. It only contains sub chunks.
     * Other chunks may contain data, subchunks or both.  If the format
     * of a chunk is unknown skipped.
     * <p>
     * Subclasses of chunk will all automagically load the subchunks.
     * It is the programmers responsibility to ensure that the data
     * preceeding the subchunks is loaded or skipped as
     * required and that something useful is done with the data.  If data from the
     * subchunks is needed in order to initialize components then that code should be
     * placed in {@link Chunk#initialize}.  Otherwise the data may be dealt with in
     * {@link Chunk#loadData}.  Also, if a chunk has data preceeding its subchunks
     * it communicates how many bytes long that data is by returning it from loadData.
     * <p>
     * This chopper reads a file in order from beginning to end
     * @param inputStream the stream with the data to be parsed.
     * @param loader the loader that will be configured from the data.
     * @param modelSize size in bytes of the file to read.
     * @return
     **************************************************************************/
    Scene* loadSceneBase(const char* fileName , Loader3DS* loader);
   

    /***************************************************************************
     * Allocates and loads a byte buffer from the channel
     * @param channel the file channel to load the data from
     * @param channelSize Channel Size
     * @return a direct byte buffer containing all the data of the channel at position 0
     * @throws IOException
     **************************************************************************/
    char* getByteBuffer(const char* fileName);

        
    /***************************************************************************
     * Sets the global bounding box of the model
     * @param Bbox Model Global Bounding Box
     **************************************************************************/
    void setModelBbox(AABB& Bbox);

    /***************************************************************************
     * Get the global Bounding box of model    
     * @return Model Bounding Box
     **************************************************************************/
	AABB& getModelBbox();

    /***************************************************************************
     * Gets the key framer chunk
     * These should be their own objects instead of chunks.     
     * @return KeyFramer Class Reference
     **************************************************************************/
    KeyFramer* getKeyFramer();
    /***************************************************************************
     * Adds a group to the choppers scene group
     * and sets the current name and group.
     * @param name Group Name Which attached to scene
     * @param group the current group that the chopper
     * will be adding things too.
     **************************************************************************/
    void addObject(string&, Node* group);

    /***************************************************************************
     * Gets the name of the current object
     * the chopper is working with.  The value returned
     * from this generally is either set by a NamedObjectChunk
     * and is the name of the name of the last object added
     * or is the name set by setObjectName.
     * @return the name of the current object being
     * constructed.
     **************************************************************************/
    string getObjectName();
    /***************************************************************************
     * Sets the name of the current object.
     * The name of the current object can also be set
     * with {@link #addObject}
     * @param name the name that the current object should be set to.
     **************************************************************************/
    void setObjectName(string name);
    /***************************************************************************
     * Gets the group for the current object
     * the chopper is working with.  The value returned
     * from this generally gets set by a NamedObjectChunk
     * and is the name of the last object added.
     * @return the group for the current object being
     * constructed.
     **************************************************************************/
    Node* getGroup();

    /***************************************************************************
     * Used to store data that may later be used
     * by another chunk.
     * @param key the look up key.
     * @param data the data to store.
     **************************************************************************/
    //void pushData(Object key, Object data);
	void pushData(int key, void* data);

    /***************************************************************************
     * Gets a datum that had been retrieved and stored
     * via {@link #pushData} earlier and removes it.
     * @param key the key used to store the datum earlier.
     * @return
     **************************************************************************/
    //Object popData(Object key);
	void* popData(int key);

    /***************************************************************************
     * Sets a named object in the loader.
     * @param key the key name of the object
     * @param value the named Object.
     **************************************************************************/
    //void setNamedObject(String key, Object value);
	void setNamedObject(string key, Node* value);

    /***************************************************************************
     * Returns true if there have been lights loaded.
     * @return true if there are lights.
     **************************************************************************/
    bool hasLights();

    /***************************************************************************
     * Adds a behavior to the scene base.
     * @param behavior the behavior to add to the scene base.
     **************************************************************************/
    //void addBehaviorNode(Behavior behavior);

    /***************************************************************************
     * Adds a light to the scene base.
     * @param light the light to add to the scene base.
     **************************************************************************/
    //void addLightNode(Light light);

    /***************************************************************************
     * Adds a camera transform to the scene base.
     * @param viewGroup the transform group to add as a view.
     **************************************************************************/
    //void addViewGroup(TransformGroup viewGroup);

    /***************************************************************************
     * Sets a named Object in the loader.
     * @param key the key used as the name for which the object will be returned
     * @return
     **************************************************************************/
    //Object getNamedObject(String key);
	Node* getNamedObject(string key);

    /***************************************************************************
     * Gets and cast the named object for the
     * key provided.  Its an error if its not
     * a transform group.
     * @param key Group Name
     * @return Transform Group
     **************************************************************************/
    Node* getNamedTransformGroup(string& key);

    /***************************************************************************
     * Gets a long from the chunk Buffer     
     * @return Long Value
     **************************************************************************/
    long getLong();

    /***************************************************************************
     * Reads a short and returns it as a signed
     * int.
     * @return Short Value
     **************************************************************************/
    int getShort();

    /***************************************************************************
     * Reads a short and returns it as an unsigned
     * int.
     * @return Unsigned Short Value
     **************************************************************************/
    int getUnsignedShort();

    /***************************************************************************
     * reads a float from the chunkBuffer.
     * @return Float Value
     **************************************************************************/
    float getFloat();

    /***************************************************************************
     * Reads 3 floats x,z,y from the chunkbuffer.
     * Since 3ds has z as up and y as pointing in whereas
     * java3d has z as pointing forward and y as pointing up;
     * this returns new Vector3f(x,-z,y)
     * @return Vector3f
     **************************************************************************/
	Vector3 getVector();

    /***************************************************************************
     * Reads 3 floats x,z,y from the chunkbuffer.
     * Since 3ds has z as up and y as pointing in whereas
     * java3d has z as pointing forward and y as pointing up;
     * this returns new Point3f(x,-z,y)
     * @return Poimt3f
     **************************************************************************/
    Vector3 getPoint();

    /***************************************************************************
     * Reads an int and returns it
     * @return Int Value
     **************************************************************************/
    int getInt();

    /***************************************************************************
     * Reads an int and returns it
     * unsigned, any ints greater than MAX_INT
     * will break.
     * @return Unsigned Int Value
     **************************************************************************/
    int getUnsignedInt();

    /***************************************************************************
     * Reads a byte, unsigns it, returns the corresponding int.
     * @return Unsigned Byte value
     **************************************************************************/
    int getUnsignedByte();
    /***************************************************************************
     * Reads a number of bytes corresponding to the
     * number of bytes left in the current chunk and returns an array
     * containing them.
     * @return an array containing all the bytes for the current chunk.
     **************************************************************************/
    char* getChunkBytes();
    /***************************************************************************
     * Fills bytes with data from the chunk buffer.
     * @param bytes the array to fill with data.
     **************************************************************************/
    void get(char* bytes , int size);

    /***************************************************************************
     * Sets the data map used to store values
     * that chunks may need to retrieve later.
     * @param dataMap the hashmap that will be used to store
     * and retrieve values for use by chunks.
     **************************************************************************/
    void setDataMap(map<int , void*>& dataMap);
    /***************************************************************************
     * This reads bytes until it gets 0x00 and returns
     * the corresponding string.
     * @return Return String
     **************************************************************************/
    string getString();

    /***************************************************************************
     * Gets the id of the current chunk.
     * @return id of the current chunk as read
     * from the chunkBuffer.  It will be a signed <code>short</code>.
     **************************************************************************/
    int getID();

    /***************************************************************************
     * Loads the image to server as a texture.
     * @param textureImageName name of the image that
     * is going to be set to be the texture.
     * @return Texture Class instance
     **************************************************************************/
    Texture* createTexture(string& textureImageName);

    /***************************************************************************
     * Get Scene Branchgroup    
     * @return
     **************************************************************************/
    Node* getSceneGroup();

    /***************************************************************************
     * Put appeareance info in map as a object and material name as a key
     * @param key Material Name
     * @param Value Appeareance data
     **************************************************************************/
	void setAppearanceMap(string , Appearance* Value);

    /***************************************************************************
     * Get Appeaeance of the material
     * @param key Material Name
     * @return Appeareance Data
     **************************************************************************/
    Appearance* getAppearance(string key);

    /***************************************************************************
     * Get MaxDataStructure reference   
     * @return MaxDataStructure reference
     **************************************************************************/
    //MaxDataStructure getMaxModelDS();

    /***************************************************************************
     * Put Mesh flip matrix of the matrix reqired by exporter
     * @param meshName
     * @param t3d
     * @param Current Mesh Name
     * @param Transform3d (Flip vertices matrix)
     **************************************************************************/
	void addFlipVertMatrixMap(string meshName, Matrix4f* t3d);

    /***************************************************************************
     * Get Mesh Flip Marix Hashmap
     * @return  Hashmap
     **************************************************************************/
    map<string, Matrix4f*>& getFlipVertMatrixMap();

    /***************************************************************************
     * Put Material name as key and texture name as a object
     * @param matName Material Name
     * @param textureName Texture Name
     **************************************************************************/
    void addMatTextureNameInMap(string matName, string textureName);
    /***************************************************************************
     * Get texture name of that Material
     * @param matName Material Name
     * @return Texture Name
     **************************************************************************/
    string* getMatTextureName(string& matName);

    /***************************************************************************
     * Add texture as a data and texture name as a key in a hashmap
     * @param textureName Texture Name
     * @param texture Texture Data
     **************************************************************************/
    void addTextureImageMap(string textureName, Texture* texture);

    /***************************************************************************
     * Get Texture
     * @param textureName Texture Name
     * @return Texture Data
     **************************************************************************/
    Texture* getTextureImageMap(string& textureName);
    /***************************************************************************
     * Get All Object3d's, Key : group Name, Entry : Object3d
     * @return All Object3d's Hashmap
     **************************************************************************/
    map<string , vector<Mesh*>* >& getShape3DList();

	/***************************************************************************
     * Get All Object3d's, Key : group Name, Entry : Object3d
     * @return All Object3d's Hashmap
     **************************************************************************/
    map<string , vector<MeshNode*>* >& getMeshNodeList();

    /***************************************************************************
     * Set current Mesh Name
     * @param curMeshNameObj Current Mesh Name
     **************************************************************************/
    void setCurNameOfObject(string curMeshNameObj);

    /***************************************************************************
     * Get current Mesh name
     * @return Current Mesh Name
     **************************************************************************/
    string getCurNameOfObject();

    /***************************************************************************
     * Get Hashmap of Faces indices of material
     * @return Hashmap
     **************************************************************************/
    map<string , vector<unsigned>*>& getMaterialFaceIndicesMap();

    /***************************************************************************
     * Get Current Material name
     * @return Current Material Name
     **************************************************************************/
    string getCurMatName();

    /***************************************************************************
     * Set current Material Name
     * @param curMatName Current Material Name
     **************************************************************************/
    void setCurMatName(string curMatName);

    /***************************************************************************
     * Get Default hierarchy Id
     * @return Hierarchy Id of current Mesh
     **************************************************************************/
    int getDefaultHeirarchyId();

    /***************************************************************************
     * Get faces indices of current Material
     * @return Int Array of Current faces Indices According Material
     **************************************************************************/
    int* getMatGroupFacesIndexArray();

    /***************************************************************************
     * Get hashmap of change material name and original name
     * @return Material Name Hashmap
     **************************************************************************/
    map<string , string>& getChangeMatNameMap();

    /***************************************************************************
     * Get hashmap of change Texture name and original name
     * @return Texture Name hashmap
     **************************************************************************/
    map<string , string>& getChangeTexNameMap();
    /***************************************************************************
     * Get mesh vertices list
     * @param meshname Mesh Name
     * @return Vertices list
     **************************************************************************/
	vector<Vector3>* getMeshVertices(string meshname);

/*******************************************************************************
 * Put Mesh name as a key and vertices list as a object
 * @param meshName Mesh name
 * @param vertices Vertices List
 ******************************************************************************/
	void setMeshVertices(string meshName, vector<Vector3>* vertices);


	SceneManager* getSceneManager()
	{
		return smgr;
	}
    
	protected:

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
    void loadSubChunks(Chunk* parentChunk, int level);
    /***************************************************************************
     * To release the memory
     **************************************************************************/
	private:

	void free();
    
};


#endif