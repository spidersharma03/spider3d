#ifndef _KEYFRAMER_H

#define _KEYFRAMER_H


#include "coreMath.h"
#include "Quaternion.h"
#include "Chunk.h"
#include "Node.h"
#include "Mesh.h"
#include "RotPosScalePathAnimator.h"
#include <string>
#include <vector>
#include <map>

using namespace spider3d::scene;
using namespace std;

/*******************************************************************************
 * Creating KeyFrame animation for each mesh Object and dummy
 ******************************************************************************/
class KeyFramer
{
    ////////////////////////////////////////////////////////////////////////////
    // Private Member Variables ////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
private:
	ChunkChopper* chopperRef  ;
    map< Node* , Node*>      lastGroupMap  ;
    map< int , Node*>       fatherMap   ;
    int      id  ;
    Node*    father    ;
	Node*    dummyObject;
    Mesh         globalShape3D ;
    Vector3*    points        ;
    string      meshName;

    //For Mesh Pivot
    Vector3     pivotCenter   ;
    Vector3     pivot         ;

    //For Transform Keys
    Vector3      position     ;
    Quaternion     rotation     ;
    Vector3      scale        ;

    vector<Quaternion*>     rotationKeys ;
    vector<Vector3*>      positions    ;
    vector<Vector3*>      scales       ;

    int          numPositionKeys;
    int          numRotationKeys;
    int          numScaleKeys;

    //For Bounding Box
    Matrix4f  bboxT3d       ;
    Matrix4f  globalbboxT3d  ;
    //BoundingBox  Bbox           = new BoundingBox();
    Vector3      meshBboxMin ;
    Vector3      meshBboxMax ;
    Vector3      modelBboxMin ;
    Vector3      modelBboxMax ;
    Vector3      vertex    ;


public:

    /**
     * Set ChunkChopper Class Reference
     * @param chopperRef ChunkChopper Class Reference
     */
    KeyFramer(ChunkChopper* chopperRef);
    


    /***************************************************************************
     * Retrieves the named object for the current key framer
     * inserts the rotation, position and pivot transformations for frame 0
     * and assigns the coordinate system to it.
     *
     * The inverse of the local coordinate system converts from 3ds
     * semi-absolute coordinates (what is in the file) to local coordinates.
     *
     * Then these local coordinates are converted with matrix
     * that will instantiate them to absolute coordinates:
     * Xabs = sx a1 (Xl-Px) + sy a2 (Yl-Py) + sz a3 (Zl-Pz) + Tx
     * Yabs = sx b1 (Xl-Px) + sy b2 (Yl-Py) + sz b3 (Zl-Pz) + Ty
     * Zabs = sx c1 (Xl-Px) + sy c2 (Yl-Py) + sz c3 (Zl-Pz) + Tz
     * Where:
     * (Xabs,Yabs,Zabs) = absolute coordinate
     * (Px,Py,Pz) = mesh pivot (constant)
     * (X1,Y1,Z1) = local coordinates
     * @param meshName Mesh Name
     * @param testObject Object
     * @param transformGroup TransformGroup
     * @return Behavior Of Mesh
     */
    void* createBehavior(string meshName, Node* transformGroup, void* testObject);

    /***************************************************************************
     * Sets the number of position keys for each mesh
     * @param numPositionKeys Number of position Keys
     * @param Number of Position Keys
     **************************************************************************/
    void setNumPositionKeys(int numPositionKeys)
	{
       this->numPositionKeys = numPositionKeys;
    }

    /***************************************************************************
     * Sets the number of rotation keys for each mesh
     * @param numRotationKeys Number of rotation Keys
     * @param Number of Rotation Keys
     **************************************************************************/
    void setNumRotationKeys(int numRotationKeys) 
	{
        this->numRotationKeys = numRotationKeys;
    }

    /***************************************************************************
     * Sets the number of scale keys for each mesh
     * @param numScaleKeys Number of scale Keys
     * @param Number of Scale Keys
     **************************************************************************/
    void setNumScaleKeys(int numScaleKeys)
	{
        this->numScaleKeys = numScaleKeys;
    }

    /***************************************************************************
     * returns number of position keys for this mesh
     * @return Number of position Keys
     */
    int getNumPositionKeys()
	{
     return numPositionKeys;
    }

    /***************************************************************************
     * returns number of rotation keys for this mesh
     * @return Number of rotation Keys
     */
    int getNumRotationKeys()
	{
     return numRotationKeys;
    }

    /***************************************************************************
     * returns nof of scale keys for this mesh
     * @return Number of scale Keys
     */
    int getNumScaleKeys()
	{
     return numScaleKeys;
    }

	    /***************************************************************************
     * Sets the center of the bounding box that the pivot
     * should offset.
     * @param center Pivot center
     */
    void setPivotCenter(Vector3 center)
    {
        this->pivotCenter = center;
    }

    /**
     * Get Pivot Center
     * @return pivot Center
     */
    Vector3 getPivotCenter()
    {
        return pivotCenter;
    }

    /***************************************************************************
     * Called to set the coordinate system transform for an object named
     * objectName.
     * This is the first t
     * @param objectName Mesh Name
     * @param coordinateSystem Local Coordinate System
     */
    void setCoordinateSystem(string objectName, Matrix4f* coordinateSystem);
    /***************************************************************************
     * Sets the group that will be used to center rotations.
     * This is applied to the mesh after all other transforms
     * have been applied.
     * @param rotation Quat4f
     */
    void setRotation(Quaternion rotation)
    {
        this->rotation = rotation;
    }

    /***************************************************************************
     * Sets the pivot that will be used to as a pivot for
     * these transfomations.
     * @param pivot Pivot Of Mesh
     **************************************************************************/
    void setPivot(Vector3& pivot)
    {
        this->pivot = pivot;
    }

    /**
     * Get Pivot of current mesh
     * @return Pivot
     */
    Vector3 getPivot()
    {
        return pivot;
    }

    /***************************************************************************
     * Sets the scale for x y and z axis for objects.
     * This is applied to the mesh before the rotation transform
     * has been applied.
     * @param scale Scale Of mesh
     */
    void setScale(Vector3 scale)
    {
        this->scale = scale;
    }


    /***************************************************************************
     * Sets the scale information necessary for animation.s
     * @param scaleKeys list of Vector3f, which may contain null elements,
     * containing a position for some keys.
     **************************************************************************/
    void setScales(vector<Vector3*>& scales)
    {
        this->scales = scales;
    }

    /***************************************************************************
     * Sets the group that will be used to translate the mesh..
     * This is applied to the mesh just before the rotation transform
     * has been applied.
     * @param position Position of mesh
     */
    void setPosition(Vector3& position)
    {
        this->position = position;
    }


    /***************************************************************************
     * Sets the orientation information necessary for animation.s
     * @param quats Rotation quat4f array of mesh
     */
    void setRotationKeys(vector<Quaternion*>& quats)
    {
        rotationKeys = quats;
    }

    /**
     * Set Position Keys of mesh
     * @param points Position Keys
     */
    void setPositions(vector<Vector3*>& points)
    {
        positions = points;
    }

    /**
     * Set Dummy Object
     * @param object Dummy Object
     */
    void setDummyObject(Node*  object)
    {
        dummyObject = (Node*)object;
    }

    /***************************************************************************
     * returns true if position keys and orientation
     * keys are longer than one element each.
     * @param fatherID Father Id
     * @param newFather Father TransformGroup
     */
    void addFather(int fatherID, Node* newFather);
    /***************************************************************************
     * Sets the id for these frames.
     * @param id the id for these frames.
     **************************************************************************/
    void setID(int id)
    {
        this->id = id;
    }

    /**
     * To calculate the global bounding box of the mesh.
     */
    void globalBboxCalculation();
	

private:

	vector<Mesh*> removeChildren(MeshNode* meshGroup);

    void addChildren( vector<Mesh*>& vecMeshes , MeshNode* meshGroup);

    void createBoundingBox(Node& lastGroup);
    
	void calculateBounds(Mesh& modifiedObject3D);
    
	Matrix4f& calculateCumulativeTransform(Node* objectGroup);
    
	Matrix4f& getParentTransform(Node* objectGroup);
    
	Matrix4f getCurrentTransform(Node* objectGroup);
    
	void insertChild(Node* objectGroup);
    // interpolate the position keys between the key frames in animation
    void interpolatePositions();
    /***************************************************************************
     * Interpolate the scale keys between the key frames in animation
     **************************************************************************/
    void interpolateScales();
    /***************************************************************************
     *  interpolate the scale keys between the key frames in animation
     **************************************************************************/
    void interpolateRotations();
    
    /***************************************************************************
     * Looks up the current object.
     * objectGroup is returned if it is the right one to return
     * otherwise a new dummy object may be returned.
     * If it isn't there it gets the dummy object
     * from the frames description chunk.
     **************************************************************************/
    Node* getObjectByName(string& objectName, Node* objectGroup, void* testObject);
    /***************************************************************************
     * Builds a transform group from the zeroth key of the
     * position and rotation tracks.
     * @return transform group with position and rotation information
     **************************************************************************/
    Node* buildKeysGroup(Matrix4f& t3d);
    /***************************************************************************
     * Builds a pivot group that will allow the objects
     * to be positioned properly according to their rotations
     * and positions.
     * @param coordinateTransform the coordinate system defining the
     * location and orientation of the local axis. This is not modified.
     * @param pivot the pivot defined in the 3ds file loaded by pivot chunk.
     * This is not changed.
     **************************************************************************/
    Node* buildPivotGroup(Matrix4f& coordinateTransform, Vector3& pivot);
    
    /***************************************************************************
     * Hierarchically adds the provided groups in order to parentGroup.
     * groups[0] is added to parentGroup, groups[1] is added to groups[0] etc.
     * @return the last group added (groups[groups.length - 1]).
     **************************************************************************/
    Node* addGroups(Node* parentGroup, vector<Node*>& groups);
  
    /***************************************************************************
     * Builds a rotation position interpolator for use on this mesh using position and rotation information
     * adds it to targetGroup.
     * This does not set the capability bits that need to be set for the animation
     * to be used. The capability bits of the targetGroup must be set by the client application.
     * The alpha object on the Interpolator must also be enabled.
     * The Interpolator must also have its scheduling bounds set.
     * @param pivotGroup transform group which will be operated on by the interpolator.
     * @param interpolatorAxis the axis that about which rotations will be centered.
     **************************************************************************/
    //TODO... This needs to use both a rotation interpolator and a position interpolator
    //in case there are keys with no position information but position information and
    //vice versa right now its using RotPosPathInterpolator
    
	RotPosScalePathAnimator* buildInterpolator(Node* targetGroup, Matrix4f& axisOfTransform, string& meshname);
     
   
    // JUN03 MEM LEAK FIX
	protected:

	void finalize();
	
    void computeTransform( float , Matrix4f& , Matrix4f& );
};



#endif
