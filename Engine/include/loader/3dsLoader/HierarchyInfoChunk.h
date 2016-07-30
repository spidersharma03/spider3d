#ifndef _HIERARCHY_INFO_CHUNK_H

#define _HIERARCHY_INFO_CHUNK_H

#include "Chunk.h"
#include "ChunkChopper.h"
#include "KeyFramer.h"

/*******************************************************************************
 * A HierarchyInfoChunk stores information about 
 * where an object belong in a hierarchy of object that 
 * have animations which may or may not be related.
 * Each object, including dummy objects, have an identifier
 * used to specify them as hierarchical parents of other 
 * objects for the purpose of key framing.
 ******************************************************************************/
class HierarchyInfoChunk :public Chunk
{
    /***************************************************************************
     * Loads a word of data that describes the parent. 
     **************************************************************************/
public:
	void loadData(ChunkChopper* chopper)
    {
        int hierarchyIdentifier = chopper->getShort();
		ChunkChopper::hierarchyID = hierarchyIdentifier;
        chopper->getKeyFramer()->setID(hierarchyIdentifier);
    }
};

#endif
