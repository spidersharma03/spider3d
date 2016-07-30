#ifndef _CHUNK_H

#define _CHUNK_H

#include <string>
#include <map>

using namespace std;
/*******************************************************************************
 * The base class for all chunks. Chunks are  flyweights and should be managed
 * by  {@link ChunkChopper}  Every chunk should know how many bytes of data(or
 * be able figure out)  to read before its subchunks are found. Chunks that only have
 * subchunks need not overrided loadData.
 * Chunks may store data for later use in loadData with {@link pushData}
 * If a chunk needs to initialize components with from subchunks, that data
 * can be retrieved with  {@link ChunkChopper#popData} inside the  {@link
 * #initialize} method.  {@link #loadData} is called at the beginning of the
 * loading process, before any data or subchunks have been loaded(the chunk
 * itself must load its data).
 * <p>
 * During loadData, if the length of data before subchunks is unknown,
 * {@link ChunkChopper#getChunkBytes} may be called and all the data
 * for the chunk(including subchunks) will be read and returned.
 ******************************************************************************/
class ChunkChopper;

class Chunk
{
   
    ////////////////////////////////////////////////////////////////////////////
    // Private Member variables ////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
private:

	map< int , Chunk* > subChunkMap;
	map<int,Chunk*>::iterator it;
    char* name;
    char* description;
   
    /***************************************************************************
     * default no-arg constructror.
     * @param chunkName Chunk Name
     **************************************************************************/
public:

	Chunk(char* chunkName) 
	{
		name = new char[100];
        strcpy( name ,  chunkName);
    }

    /***************************************************************************
     * default no-arg constructror.
     **************************************************************************/
    Chunk() {
    }

    /***************************************************************************
     * Hasmap of all childrens
     * @param id Chunk Id
     * @param subChunk Child chunk reference
     **************************************************************************/
    void addSubChunk(int id, Chunk* subChunk)
	{
		//subChunkMap.insert( pair<int,Chunk*>( id ,subChunk ) ); 
		subChunkMap[id] = subChunk ; 
    }

    /***************************************************************************
     * Get the subchunk class reference
     * @param id Chunk Id
     * @return Child Chunk
     **************************************************************************/
    Chunk* getSubChunk(int id)
	{
		if ( subChunkMap.find(id) == subChunkMap.end() )
		  return 0;

		return ( ( subChunkMap.find(id) )->second);
    }

    /***************************************************************************
     * This method is called after all the  current chunks subchunks are loaded.
     * Any data stored by those subchunks may be retrieved via {@link
     * ChunkChopper#popData}
     * The default implementation does nothing.
     * @param chopper ChunkChopper may contain data loaded by subchunks.
     **************************************************************************/
    virtual void initialize(ChunkChopper* chopper) {
    }

    /***************************************************************************
     * This is called by the chunk chopper before any of the chunk's 
     * subchunks  are loaded.  Any data loaded that may need to be 
     * used later by superchunks should be stored in
     * the chunk chopper via {@link ChunkChopper#popData}
     * The default implementation does nothing.
     * @param chopper ChunkChopper may contain data loaded by subchunks.
     **************************************************************************/
    virtual void loadData(ChunkChopper* chopper) {
    }

    /***************************************************************************
     * Sets  nice human readable name for the chunk.
     * @param name Name to use for display of this chunk.
     **************************************************************************/
    void setName(char* name) {
        strcpy( this->name ,  name);
    }

    /***************************************************************************
     * Gets a human readable name for the chunk.
     * @return Name used to display the chunk.
     **************************************************************************/
    char* getName() {
        return name;
    }

    /***************************************************************************
     * Get chunk description
     * @return Chunk Description
     **************************************************************************/
    char* getDescription() {
        return description;
    }

    /***************************************************************************
     * Set Description of the chunk
     * @param desc
     **************************************************************************/
    void setDescription(char* desc) {
        strcpy( this->description ,  desc);
    }

};


#endif
