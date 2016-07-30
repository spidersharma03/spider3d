#ifndef _POSITION_CHUNK_H

#define _POSITION_CHUNK_H

#include "Chunk.h"
#include "ChunkChopper.h"
#include "KeyFramer.h"

/*******************************************************************************
 * Loads the position of a mesh as defined in the 3ds file.
 * This position may need to be converted to another coordinate
 * system by KeyFramerInfoChunk.
 *  * {@see KeyFramerInfoChunk} for more information about using
 * animations from a 3ds file
 ******************************************************************************/
class PositionChunk :public Chunk
{
    /***************************************************************************
     * Loads the position for a shape and KeyFramerInfoChunk
     * @param chopper the ChunkChopper containing the state of the parser.  
     **************************************************************************/
public :
    void loadData(ChunkChopper* chopper)
    {        
		
        int flags = chopper->getUnsignedShort();
        string name = chopper->getObjectName();
        chopper->getLong();

        int numKeys = chopper->getUnsignedInt();
        /*ArrayList savePositionList = new ArrayList();//for saving data
        */

		vector<Vector3*> points;
		points.resize(ChunkChopper::FrameCnt);
        for ( int i = 0 ; i < (int)ChunkChopper::FrameCnt ; i ++ )
           points[i] = 0;
		//Point4f posNFrame = null;
        for(int i =0; i < numKeys; i++)
        {
            long keyNumber = chopper->getUnsignedInt();
            int  accelerationData = chopper->getUnsignedShort(); 

			Vector3* position = new Vector3();
            Vector3 p = chopper->getPoint();
			position->x = p.x;
			position->y = p.y;
			position->z = p.z;

            //For Kf writing or saving file
            //posNFrame = new Point4f();
            //posNFrame.x = position.x;
            //posNFrame.y = position.y;
            //posNFrame.z = position.z;
            //posNFrame.w = (float) keyNumber;

            if(i==0)
            {
                chopper->getKeyFramer()->setPosition(*position);
            }

			if ( keyNumber >= (int)ChunkChopper::FrameCnt)continue;
            points[(int) keyNumber] = position;

            //savePositionList.add(posNFrame); // saving data
        }

        chopper->getKeyFramer()->setNumPositionKeys(numKeys);
        chopper->getKeyFramer()->setPositions( points );
        //chopper.getMaxModelDS().addMeshPositionList(name, savePositionList);*/
    }
};

#endif