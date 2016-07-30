#ifndef _SCALE_CHUNK_H

#define _SCALE_CHUNK_H

#include "Chunk.h"
#include "ChunkChopper.h"
#include "KeyFramer.h"

/*******************************************************************************
 * Extracts scale information from the 3ds file which
 * is then used by the mesh info chunk to construct a 
 * animation.
 ******************************************************************************/
class ScaleChunk :public Chunk {

    /***************************************************************************
     * Loads the scale for a shape
     * and notifies the KeyFramerInfoChunk
     * @param chopper the ChunkChopper containing the state of the parser.  
     **************************************************************************/
public:

    void loadData(ChunkChopper* chopper)
	{
        int flags = chopper->getUnsignedShort();
        chopper->getLong();

        int numKeys = chopper->getUnsignedInt();

		vector<Vector3*> scales;
		scales.resize(ChunkChopper::FrameCnt);
        for ( int i = 0 ; i < (int)ChunkChopper::FrameCnt ; i ++ )
           scales[i] = 0;
        //List scaleList = new ArrayList();// for saving Data
        //Point4f scaleNFrame = null;// for saving Data

        for (int i = 0; i < numKeys; i++) 
		{
            long keyNumber = chopper->getUnsignedInt();
            int accelerationData = chopper->getUnsignedShort();

            float scaleX = chopper->getFloat();
            float scaleY = chopper->getFloat();
            float scaleZ = chopper->getFloat();

            Vector3* scale = new Vector3(scaleX, scaleZ, scaleY);

            //For saving Data
            //scaleNFrame = new Point4f();
            //scaleNFrame.x = scale.x;
            //scaleNFrame.y = scale.y;
            //scaleNFrame.z = scale.z;
            //scaleNFrame.w = (float) keyNumber;

            if (i == 0)
			{
                chopper->getKeyFramer()->setScale(*scale);
            }
			if ( keyNumber >= (int)ChunkChopper::FrameCnt)continue;
            scales[(int) keyNumber] = scale;
            //scaleList.add(scaleNFrame);//For saving Data
        }
        chopper->getKeyFramer()->setNumScaleKeys(numKeys);
        chopper->getKeyFramer()->setScales(scales);
        //chopper.getMaxModelDS().addMeshScaleList(chopper.getObjectName(), (ArrayList) scaleList);*/
    }
};

#endif