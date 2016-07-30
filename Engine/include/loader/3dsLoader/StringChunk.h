#ifndef _STRING_H

#define _STRING_H

#include "Chunk.h"
#include "ChunkChopper.h"
#include "Chunk_Types.h"

/*******************************************************************************
 * These have no subchunks. Only String data terminated with a null. For
 * strings with unknown length use {@link ChunkChopper#getString} This can
 * also be used for chunks that have data of a known length beginning with a
 * string with unnessecary(you don't want to use it) data following.
 ******************************************************************************/
class StringChunk :public Chunk {

    /***************************************************************************
     * Reads in all the data for this chunk and makes a string out of it.
     * This will set the data in the chopper with a key of this chunks id.
     * @param chopper the chopper that is doing the parsing.  
     **************************************************************************/
public:
	void loadData(ChunkChopper* chopper) 
	{
        char* stringArray = chopper->getChunkBytes();

		string* value = new string();
		*value += stringArray;
        //String value = new String(stringArray, 0, stringArray.length - 1);
        /*if ( value->findFirst((char) (0x0000)) > 0) {
            *value = value->subString(0, value->findFirst((char) (0x0000)));*/
		if ( value->find_first_of((char) (0x0000)) > 0) {
			*value = value->substr(0, value->find_first_of((char) (0x0000)));
        }       

        if( chopper->getID() == MATERIAL_NAME )
        {
           // value = chopper.getFileManagerRef().checkThisNameInScene(chopper.getFileManagerRef().vecMatNamesInScene,
            //        chopper.getChangeMatNameMap(), value);
            chopper->setCurMatName(*value);
        }

        chopper->pushData(chopper->getID(), value);
    }
};

#endif