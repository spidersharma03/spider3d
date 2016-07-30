#ifndef _TEXTURE_CHUNK_H

#define _TEXTURE_CHUNK_H

#include "Chunk.h"
#include "Chunk_Types.h"


/*******************************************************************************
 * Loads percentage values from binary data representing them.
 ******************************************************************************/
class TextureChunk :public Chunk
{

    /***************************************************************************
     * Gets the current texture image from the chopper
     * creates a texture with it and sets that texture
     * on the chopper.
     * @param chopper  the parser containing the state of parsing
     **************************************************************************/
public:
    void initialize(ChunkChopper* chopper)
    {
        string* textureName = (string*)chopper->popData(TEXTURE_NAME);        
        Texture* texture =  chopper->createTexture(*textureName);
        //chopper->pushData( TEXTURE , texture );
        
		//textureName = chopper.getFileManagerRef().checkThisNameInScene(chopper.getFileManagerRef().vecTexNamesInScene,
        //chopper->getChangeTexNameMap(), textureName);

        chopper->addMatTextureNameInMap( chopper->getCurMatName() , *textureName);
        chopper->addTextureImageMap( *textureName , texture );
    }
};

#endif