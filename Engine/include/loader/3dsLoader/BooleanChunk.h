#ifndef _BOOLEAN_CHUNK_H

#define _BOOLEAN_CHUNK_H

#include "Chunk.h"


/*******************************************************************************
 * A boolean chunk is true if it is present otherwise there is no chunk and that
 * represents false. This chunk will set chopper data to Boolean true with a key
 * that is the id of the chunk. These have no subchunks. Only String data.
 ******************************************************************************/
class BooleanChunk : public Chunk 
{
    
    ////////////////////////////////////////////////////////////////////////////
    // Private Member Varaibles ////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
private:

	bool booleanVal ;

    /***************************************************************************
     * If this method is being called then a boolean true will be set on the
     * chunk chopper with a key that is the id of this chunk.
     * @param chopper the chopper on which the boolean true data is to be set 
     **************************************************************************/
public:
	
	void loadData(ChunkChopper* chopper) 
	{
        booleanVal = true;
        chopper->pushData(chopper->getID() , &booleanVal);
    }

};


#endif