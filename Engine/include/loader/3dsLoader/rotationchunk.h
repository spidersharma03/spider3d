#ifndef _ROTATION_CHUNK_H

#define _ROTATION_CHUNK_H

#include "Chunk.h"
#include "ChunkChopper.h"
#include "KeyFramer.h"

/*******************************************************************************
 * Extracts the rotation information from the 3ds file.
 * Rotations occur about a pivot at a position within
 * a local coordinate system. The rotation information
 * is provided to the KeyFramerInfoChunk which then converts
 * it to a global coordinate system and applies animation
 * information.
 * {@see KeyFramerInfoChunk} for more information about using
 * animations from a 3ds file
 ******************************************************************************/
class RotationChunk :public Chunk {

    /***************************************************************************
     *  String that will be used as a data object in the transform that the 
     *  RotationInterpolator will be a child of so it may be look up later.
     **************************************************************************/
	public:

    //const static String ROTATION_TAG = "ROTATION_INTERPOLATOR";

    /***************************************************************************
     * Loads the quaternion for a rotation of a shape
     * and notifies mesh info chunk.
     * @param chopper the ChunkChopper containing the state of the parser.  
     **************************************************************************/
    void loadData(ChunkChopper* chopper) 
	{

		string name = chopper->getObjectName();
        int flags = chopper->getUnsignedShort();
        // unknown data
        chopper->getLong();
        // no of keys in animation for this mesh
        int numKeys = chopper->getUnsignedInt();

        Quaternion* previousQuat = 0;
        
		int framecnt = (int) ChunkChopper::FrameCnt;

        vector<Quaternion*> quatarray;
		quatarray.resize(framecnt);
        for ( int i = 0 ; i < framecnt ; i ++ )
           quatarray[i] = 0;

        //ArrayList<Point5f> rotationList = new ArrayList<Point5f>();  // for exporting file
        //Point5f rotPoint = null;// for exporting file

        for (int i = 0; i < numKeys; i++)
		{
            long frameNumber = chopper->getUnsignedInt();//Part of the track header

            int accelerationData = chopper->getUnsignedShort();//Part of the track header
            getSplineTerms(accelerationData, chopper);//Part of the track header

            float angle = chopper->getFloat();
            Vector3 vector = chopper->getVector();

            //Saving rotaion keys for exporter
            //rotPoint = new Point5f(0f,0f,0f,0f,0f);
            //rotPoint.x = angle;
            //rotPoint.y = vector.x;
            //rotPoint.z = vector.y;
            //rotPoint.u = vector.z;
            //rotPoint.v = frameNumber;
            //////////////////////////////////////////

			Quaternion* quat = new Quaternion();
            *quat = getQuaternion(vector, angle);

            if ( previousQuat != 0 )
			{
               *quat *= (*previousQuat);
            }
            previousQuat = quat;

			if ( frameNumber >= (int)ChunkChopper::FrameCnt)continue;
            quatarray[(int) frameNumber] = quat;
            
            if (i == 0)
			{
                chopper->getKeyFramer()->setRotation(*quat);
            }

            //rotationList.add(rotPoint);
        }
        chopper->getKeyFramer()->setNumRotationKeys(numKeys);
        chopper->getKeyFramer()->setRotationKeys(quatarray);
        //chopper->getMaxModelDS().addMeshRotationList(name, rotationList);*/
    }


	/***************************************************************************
     * This converts a 3ds angle and axis to 
     * a quaternion rotation.  Successive
     * rotations are relative to the first so each
     * rotation must be made absolute by multiplying
     * it with its predecessor
     * @param axis Axis of rotation
     * @param angle Angle to rotate Mesh
     * @return Quat4f
     */
	Quaternion getQuaternion(Vector3 axis, float angle) 
	{
		double l = axis.length();
        Quaternion quat;
        if (l < 1e-8)
		{
			quat.X = quat.Y = quat.Z = 0.0f;
            quat.W = 1.0f;
        }
		else 
		{
            double omega = -0.5 * angle;
            double s = sin(omega) / l;
            quat.X = (float) (s * axis.x);
            quat.Y = (float) (s * axis.y);
            quat.Z = (float) (s * axis.z);
            quat.W = (float) cos(omega);
        }
        //float sinA = (float) sin(angle / 2.0f);
        //float cosA = (float) cos(angle / 2.0f);        
        return quat;
    }

    /***************************************************************************
     * This only reads the spline data and should be part
     * of the track header when it gets invented.
     * @param chopper an integer representing the bits that 
     * determine which of the five possible spline terms are present in the 
     * data and should be read.
     * @param chopper what to read the data from
     * The possible spline values are are 
     * <ol>
     * <li> Tension
     * <li> Continuity
     * <li> Bias
     * <li> EaseTo
     * <li> EaseFrom
     * </ol>
     **************************************************************************/
private:
     void getSplineTerms( int accelerationData, ChunkChopper* chopper) 
	 {
        int bits = accelerationData;
        for (int i = 0; i < 5; i++) 
		{
            bits = bits >> i;
            if ((bits & 1) == 1) {
            }
        }
    }

    
};

#endif