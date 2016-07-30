#ifndef _AXIS_CHUNK

#define _AXIS_CHUNK

#include "Chunk.h"
#include "ChunkChopper.h"
#include "KeyFramer.h"

/*******************************************************************************
 * Extracts the local coordinate that will act
 * as a shape's axis that will be used by the mesh
 * info chunk.
 ******************************************************************************/
class AxisChunk: public Chunk
{
    ////////////////////////////////////////////////////////////////////////////
    // Private Member Variables ////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
private:

    Vector3     xAxis ;
    Vector3     zAxis ;
    Vector3     yAxis ;
    Vector3     origin ;
    //Transform3D transform   = null;

    /***************************************************************************
     * Loads the local coordinate system for the current mesh.
     *
     * @param chopper the ChunkChopper containing the state of the parser.
     *
     * The location of the local coordinate system is defined relative to
     * positions and rotations at frame 0.  But the orientation is absolutely
     * defined.
     *
     * With either 3x3 or 4x4 rotation, translation, there
     * is a simple relationship between each matrix and the resulting coordinate
     * system. The first three columns of the matrix define the direction vector of the
     * X, Y and Z axii respectively.
     * If a 4x4 matrix is defined as:
     * <code>
     *     | A B C D |
     * M = | E F G H |
     *     | I J K L |
     *     | M N O P |
     * </code>
     * Then the direction vector for each axis is as follows:
     *
     * <code>
     * X-axis = [ A E I ]
     * Y-axis = [ B F J ]
     * Z-axis = [ C G K ]
     * </code>
     *
     **************************************************************************/
    void loadData(ChunkChopper* chopper) {
        xAxis = chopper->getPoint();
        zAxis = chopper->getPoint();
        yAxis = chopper->getPoint();
        origin = chopper->getPoint();

		Matrix4f* transform = new Matrix4f();
		
		float* data = new float[16];
		/*data[0] = xAxis.x; data[4] = yAxis.x; data[8]  = -zAxis.x; data[12] = origin.x;
        data[1] = xAxis.y; data[5] = yAxis.y; data[9]  = -zAxis.y; data[13] = origin.y;
		data[2] = xAxis.z; data[6] = yAxis.z; data[10] = -zAxis.z; data[14] = origin.z;
		data[3] = 0      ; data[7] = 0      ; data[11] =   0;      data[15] = 1;*/
		
		data[0] = xAxis.x; data[1] = yAxis.x; data[2]  = -zAxis.x; data[3] = origin.x;
        data[4] = xAxis.y; data[5] = yAxis.y; data[6]  = -zAxis.y; data[7] = origin.y;
		data[8] = xAxis.z; data[9] = yAxis.z; data[10] = -zAxis.z; data[11] = origin.z;
		data[12] = 0      ; data[13] = 0      ; data[14] =   0;      data[15] = 1;

		transform->set(data);

		delete data;
        
        string meshName = chopper->getObjectName();
        chopper->getKeyFramer()->setCoordinateSystem(meshName, transform);
    }
};



#endif