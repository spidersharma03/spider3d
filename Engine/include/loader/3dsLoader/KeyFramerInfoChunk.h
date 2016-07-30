#ifndef _KEY_FRAMER_INFO_CHUNK_H

#define _KEY_FRAMER_INFO_CHUNK_H

#include "Chunk.h"
#include "ChunkChopper.h"


/*******************************************************************************
 * A KeyFramerInfoChunk stores information about things
 * that happen to meshes: Position information, rotation
 * information, scale information, pivot information 
 * and frame information.
 * Together with the frames chunk thes are used
 * display animation behaviors.
 ******************************************************************************/
class KeyFramerInfoChunk :public Chunk 
{
    
	//private Behavior frameBehavior = null;




    /***************************************************************************
     * Retrieves the named object for the current key framer
     * inserts the rotation, position and pivot transformations for frame 0
     * and assigns the coordinate system to it.
     * The inverse of the local coordinate system converts from 3ds 
     * semi-absolute coordinates (what is in the file) to local coordinates.
     * Then these local coordinates are converted with matrix 
     * that will instantiate them to absolute coordinates:
     * Xabs = sx a1 (Xl-Px) + sy a2 (Yl-Py) + sz a3 (Zl-Pz) + Tx
     * Yabs = sx b1 (Xl-Px) + sy b2 (Yl-Py) + sz b3 (Zl-Pz) + Ty
     * Zabs = sx c1 (Xl-Px) + sy c2 (Yl-Py) + sz c3 (Zl-Pz) + Tz
     * Where:
     * (Xabs,Yabs,Zabs) = absolute coordinate
     * (Px,Py,Pz) = mesh pivot (constant)
     * (X1,Y1,Z1) = local coordinates
     * @param chopper the ChunkChopper containing the current state of the parser. 
     **************************************************************************/
public:

    void initialize(ChunkChopper* chopper) 
	{
        string meshName = chopper->getObjectName();        
        chopper->getKeyFramer()->createBehavior(meshName,
                chopper->getNamedTransformGroup(meshName),
                chopper->getNamedObject(meshName));
        //if (frameBehavior != null) {
        //    chopper.addBehaviorNode(frameBehavior);
        //}
    }
};

#endif