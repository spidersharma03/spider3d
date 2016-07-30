#include "loader/3dsloader/KeyFramer.h"
#include "loader/3dsloader/ChunkChopper.h"
#include "TCBKeyFrame.h"
#include "appearance/Appearance.h"

using namespace spider3d::appearance;

KeyFramer::KeyFramer(ChunkChopper* chopperRef)
    {
        this->chopperRef = chopperRef;
		meshBboxMin.set(1.0e38f, 1.0e38f, 1.0e38f);
        meshBboxMax.set(-1.0e38f, -1.0e38f, -1.0e38f);
        modelBboxMin.set(1.0e38f, 1.0e38f, 1.0e38f);
        modelBboxMax.set(-1.0e38f, -1.0e38f, -1.0e38f);
        //globalShape3D = new Shape3D();
		father = 0;
    }


    void* KeyFramer::createBehavior(string meshName, Node* transformGroup, void* testObject) 
	{
        this->meshName = meshName;
        //bboxT3d.setIdentity();
        //globalbboxT3d.setIdentity();
        
		Node* objectGroup = getObjectByName(meshName, transformGroup, testObject);

        if (objectGroup == 0) 
		{
            return 0;
        }

        //TransformInterpolator behavior = null;
		Matrix4f* coordinateSystem = 0;

		if ( chopperRef->namedObjectCoordinateSystems.find(meshName) == chopperRef->namedObjectCoordinateSystems.end() )
		{
			coordinateSystem = new Matrix4f();
		}
		else
		{
		    coordinateSystem = chopperRef->namedObjectCoordinateSystems.find(meshName)->second;
		}

	       vector<Mesh*> vecMeshes = removeChildren((MeshNode*)objectGroup);

        Matrix4f* coordinateTransform = coordinateSystem == 0 ? new Matrix4f() : new Matrix4f(*coordinateSystem);

        Matrix4f targetTransform ;
        
		Node* targetGroup = new Node(0,0);
		//targetGroup->getRelativeTransformationMatrix() = targetTransform;

        //CDummyTransformationSceneNode* localCoordinates = new CDummyTransformationSceneNode(0,0,0);

        //CDummyTransformationSceneNode* keysGroup = (CDummyTransformationSceneNode*)buildKeysGroup(*coordinateTransform);
		//Node* keysGroup = buildKeysGroup(*coordinateTransform);
        //matrix4 t3dtemp ;
		//t3dtemp = keysGroup->getRelativeTransformation();

		vector<Node*> groups;
		//groups.push_back(localCoordinates);
		//groups.push_back(keysGroup);
		groups.push_back(targetGroup);
		groups.push_back( buildPivotGroup(*coordinateTransform, pivot) );
        
		MeshNode* lastGroup =  (MeshNode*)addGroups(objectGroup, groups);

        addChildren(  vecMeshes , lastGroup);

        //lastGroupMap.insert(pair<Node*,Node*>(objectGroup, lastGroup));
        lastGroupMap[objectGroup] = lastGroup;

		insertChild(objectGroup);
        
		//Transform3D cumulativeNodeT3D = new Transform3D();

        //if (objectGroup != null) 
		//{
        //    cumulativeNodeT3D = calculateCumulativeTransform(objectGroup);
        //}

        //globalbboxT3d.mul(cumulativeNodeT3D, globalbboxT3d);
        
		RotPosScalePathAnimator* animator = buildInterpolator(targetGroup, targetTransform, meshName);

        //if (animator != 0)
		{
            //matrix4 identity;
			//identity.makeIdentity();
			//keysGroup->getRelativeTransformationMatrix() = identity;
            //behavior.setEnable(false);
            //targetGroup.addChild(behavior);
			Matrix4f outMat;
            computeTransform(0.0f, targetTransform , outMat);
			targetGroup->setTransform(outMat);
			if (animator)
			targetGroup->addAnimator(animator);
        }

        //createBoundingBox(lastGroup);
        //globalBboxCalculation();

        //t3dtemp = null;
        //cumulativeNodeT3D = null;

        return 0;
    }

  
	void KeyFramer::computeTransform( float val , Matrix4f& matrix , Matrix4f& outMat)
	{
		Vector3 point, scaleFactors;
		point = position;
        scaleFactors = scale;
        Quaternion quat = rotation;

		Matrix4f tMat = quat.getMatrix();

		tMat = tMat.getTranspose();

		tMat.setTranslation ( point );

		Matrix4f keyFrameTransform(tMat);

		Matrix4f t3d = matrix;

		Matrix4f axis(t3d);
		t3d = t3d.getInverse();
		Matrix4f axisInverse(t3d);
		
		Matrix4f transform , scaleTransform;
		transform.multiply(axis,keyFrameTransform);
		scaleTransform.setScale(scaleFactors);
		transform.multiply(scaleTransform);
		//transform.setScale(scaleFactors);
		transform.multiply(axisInverse);

		outMat = transform;
	}
   
    void  KeyFramer::createBoundingBox(Node& lastGroup) 
	{

       /* int numChildren = lastGroup.numChildren();

        Shape3D tempShape = new Shape3D();

        for (int i = 0; i < numChildren; i++) {

            Node node = lastGroup.getChild(i);

            if (node instanceof Object3D) {

                Object3D object3D = (Object3D) node;

                Object3D modifiedObject3D = (Object3D) object3D.cloneNode(true);
                calculateBounds(modifiedObject3D);

                Enumeration enumeration = modifiedObject3D.getAllGeometries();

                while (enumeration.hasMoreElements()) {
                    Geometry geometry = (Geometry) enumeration.nextElement();
                    tempShape.addGeometry(geometry);
                }
            }
        }

        if (numChildren > 0)
        {
            BoundingBox bounds = (BoundingBox) tempShape.getBounds();
            Point3d lower = new Point3d(), upper = new Point3d();
            float[] centre = new float[3];
            bounds.getLower(lower);
            bounds.getUpper(upper);

            centre[0] = (float) (lower.x + upper.x) / 2.0f;
            centre[1] = (float) (lower.y + upper.y) / 2.0f;
            centre[2] = (float) (lower.z + upper.z) / 2.0f;

            float dx = (float) (upper.x - lower.x);
            float dy = (float) (upper.y - lower.y);
            float dz = (float) (upper.z - lower.z);

            AABB aabb = new AABB();
            aabb.setOrigin(centre);
            aabb.setX(Math.abs(dx));
            aabb.setY(Math.abs(dy));
            aabb.setZ(Math.abs(dz));

            GeometryArray garray = (GeometryArray) aabb.getGeometry();

            Point3f data[] = new Point3f[garray.getVertexCount()];
            for (int i = 0; i < data.length; i++)
            {
                data[i] = new Point3f();
            }

            garray.getCoordinates(0, data);

            bboxT3d.invert();

            for (int i = 0; i < data.length; i++)
            {
                bboxT3d.transform(data[i]);
            }

            garray.setCoordinates(0, data);
            globalShape3D.addGeometry(garray);

            for (int i = 0; i < numChildren; i++)
            {
                Node node = lastGroup.getChild(i);
                if (node instanceof Object3D)
                {
                    Object3D object3D = (Object3D) node;
                    object3D.setAABB(aabb);
                }
            }

            aabb.setVisible(false);
            aabb.setPickable(false);
            lastGroup.addChild(aabb);
        }*/
    }

    void KeyFramer::calculateBounds(Mesh& modifiedObject3D)
    {

        /*Enumeration enumeration = modifiedObject3D.getAllGeometries();

        while (enumeration.hasMoreElements())
        {
            GeometryArray geomarray = (GeometryArray) enumeration.nextElement();
            if (geomarray != null)
            {
                Point3f data[] = new Point3f[geomarray.getVertexCount()];
                for (int j = 0; j < data.length; j++)
                {
                    data[j] = new Point3f();
                }
                geomarray.getCoordinates(0, data);
                for (int i = 0; i < data.length; i++)
                {
                    bboxT3d.transform(data[i]);
                }
                geomarray.setCoordinates(0, data);
            }
        }*/

    }

	Matrix4f& KeyFramer::calculateCumulativeTransform(Node*  objectGroup)
    {
        // Get Parent T3D
        /*Transform3D parentT3D = getParentTransform(objectGroup);
        Transform3D currentKeyT3D = getCurrentTransform(objectGroup);

        Transform3D cumulativeTransform = new Transform3D();
        TransformGroup meshTG = (TransformGroup) lastGroupMap.get(objectGroup);

        int numChildren = meshTG.numChildren();

        if (numChildren > 0)
        {
            Node node = meshTG.getChild(0);
            if (node instanceof Object3D)
            {
                Object3D currentObj3D = (Object3D) node;

                if (parentT3D == null) // There is no father TG, so only have currKey T3D
                {
                    cumulativeTransform.set(currentKeyT3D);
                } else // Multiply the parentLink's T3D with currentKey T3D
                {
                    cumulativeTransform.mul(parentT3D, currentKeyT3D);
                }
                currentObj3D.setCumulativeTransform(cumulativeTransform);
            }
        }
        return cumulativeTransform;*/
		Matrix4f mat;
		return mat;
    }

    Matrix4f& KeyFramer::getParentTransform(Node*  objectGroup)
    {
		Matrix4f parentT3D;
        // If Father is another TG..Father is parent Link --> Get Cumulative Transfrom form Fathers object3D
        //if (objectGroup->getParent() instanceof TransformGroup)
        //{

        //    Node* parentTG = objectGroup->getParent();
        //    Node* meshTG = parentTG->getChild(0); // Get the [-p][M-1] mesh matrix
        //    Node* node = meshTG->getChild(0);
        //    if (node instanceof Object3D)
        //    {
        //        Object3D parentLinkObject3D = (Object3D) node;
        //        parentT3D = parentLinkObject3D.getCumulativeTransform();
        //    }
        //} // No father TG
        //else if (objectGroup.getParent() instanceof BranchGroup)
        //{
        //    parentT3D = null;
        //}
        return parentT3D;
    }

    Matrix4f KeyFramer::getCurrentTransform(Node* objectGroup)
    {
        // Set Current Object3D's T3D
        Matrix4f* currentKeyT3D = new Matrix4f();
		const vector<Node*>& children0 = objectGroup->getChildren();
		Node* currentKeyTG  = *(children0.begin());
		const vector<Node*>& children1 = currentKeyTG->getChildren();
        Node* node = *(children1.begin());
        //if (node instanceof TransformGroup)
        {
			const vector<Node*>& children2 = currentKeyTG->getChildren();
            currentKeyTG = *(children2.begin());
			*currentKeyT3D = currentKeyTG->getGlobalTransform();
        }
        return *currentKeyT3D;
		
    }

    void KeyFramer::insertChild(Node* objectGroup)
    {
        if (father == 0) 
		{
            return;
        }

		Node* lasttg =  0;

		if ( lastGroupMap.find(father) == lastGroupMap.end() )
		{
			lasttg = 0;
		}
		else
		{
			lasttg = lastGroupMap.find(father)->second;
		}

        Node* tempgroup = objectGroup->getParent();

        if (tempgroup != 0)
        {
            tempgroup->removeChild(objectGroup);
        }

        Node* group = 0;
        if (lasttg != 0) 
		{
            group = lasttg->getParent();
        }

        if (group != 0) 
		{
            group->addChild(objectGroup);
        }
    }

    // interpolate the position keys between the key frames in animation
    void KeyFramer::interpolatePositions()
    {
        int n =positions.size();
        // list containing the key frame number between which the interpolation will be done
        vector<int> framelist ;
        framelist.push_back(0);
        for (int i = 1; i < n; i++)
        {
            if (positions[i] != 0)
            {
                framelist.push_back(i);
            }
        }

        for (unsigned i = 1; i < framelist.size(); i++)
        {
            int begin = framelist[i - 1];
            int end = framelist[i];
            for (int j = begin; j < end; j++)
            {
                Vector3* temp = positions[j];
                if (temp == 0)
                {
                    positions[j] = new Vector3();
                    positions[j]->interpolate(*positions[begin], *positions[end], (float) (j - begin) / (end - begin));
                }
            }
        }

        int last = framelist[framelist.size() - 1];

        if (last < n - 1)
        {
            for (int i = last; i < n; i++)
            {
                positions[i] = positions[last];
            }
        }

    }

    /***************************************************************************
     * Interpolate the scale keys between the key frames in animation
     **************************************************************************/
    void KeyFramer::interpolateScales()
    {
        int n = scales.size();
        // list containing the key frame number between
        // which the interpolation will be done
        vector<int> framelist;
        framelist.push_back(0);
        for (int i = 1; i < n; i++)
        {
            if (scales[i] != 0)
            {
                framelist.push_back(i);
            }
        }

        for (unsigned i = 1; i < framelist.size(); i++)
        {
            int begin = framelist[i - 1];
            int end = framelist[i];
            for (int j = begin; j < end; j++)
            {
                Vector3* temp = scales[j];
                if (temp == 0)
                {
                    scales[j] = new Vector3();
                    scales[j]->interpolate(*scales[begin], *scales[end], (float) (j - begin) / (end - begin));
                }
            }
        }

        int last = framelist[framelist.size() - 1];

        if (last < n - 1)
        {
            for (int i = last; i < n; i++)
            {
                scales[i] = scales[last];
            }
        }

    }

    /***************************************************************************
     *  interpolate the scale keys between the key frames in animation
     **************************************************************************/
    void KeyFramer::interpolateRotations()
    {
        int n = rotationKeys.size();
        // list containing the key frame number between which the interpolation will be done
        vector<int> framelist;
        framelist.push_back(0);
        for (int i = 1; i < n; i++)
        {
            if (rotationKeys[i] != 0)
            {
                framelist.push_back(i);
            }
        }

        for (unsigned i = 1; i < framelist.size(); i++)
        {
            int begin = framelist[i - 1];
            int end = framelist[i];
            for (int j = begin; j < end; j++)
            {
                Quaternion* temp = rotationKeys[j];
                if (temp == 0)
                {
                    rotationKeys[j] = new Quaternion();
                    rotationKeys[j]->slerp(*rotationKeys[begin], *rotationKeys[end], (float) (j - begin) / (end - begin));
                }
            }
        }

        int last = framelist[framelist.size() - 1];

        if (last < n - 1)
        {
            for (int i = last; i < n; i++)
            {
                rotationKeys[i] = rotationKeys[last];
            }
        }
    }
    
    vector<Mesh*> KeyFramer::removeChildren(MeshNode* meshGroup)
    {
        //const list<ISceneNode*>& children = group->getChildren();
        //group->removeAll();
		if ( meshGroup )
		{
		  //core::list<ISceneNode*>::ConstIterator it = node->getChildren().begin();
			vector<Mesh*> children = meshGroup->getAllMeshes();
			meshGroup->removeAllMeshes();
			return children;
		}
		
    }

    void KeyFramer::addChildren(vector<Mesh*>& children, MeshNode* meshGroup)
    {
        if (meshGroup == 0)
        {
            return;
        }
		for( unsigned c=0; c<children.size(); c++ )
			meshGroup->addMesh(children[c]);
    }

    /***************************************************************************
     * Looks up the current object.
     * objectGroup is returned if it is the right one to return
     * otherwise a new dummy object may be returned.
     * If it isn't there it gets the dummy object
     * from the frames description chunk.
     **************************************************************************/
    Node* KeyFramer::getObjectByName(string& objectName, Node* objectGroup, void* testObject)
    {
        //This means its a dummy object.  It needs to be created.
        if (objectGroup == 0 && testObject == 0)
        {
			Matrix4f* mat = new Matrix4f();
            chopperRef->namedObjectCoordinateSystems[objectName] = mat;
            //chopperRef->namedObjectCoordinateSystems.insert(pair<string,Matrix4f*>(objectName, mat) );
			objectGroup = dummyObject;
        }
        return objectGroup;
    } 

    /***************************************************************************
     * Builds a transform group from the zeroth key of the
     * position and rotation tracks.
     * @return transform group with position and rotation information
     **************************************************************************/
    Node* KeyFramer::buildKeysGroup(Matrix4f& t3d1)
    {
        Matrix4f positionTransform;
        positionTransform.setTranslation(position);

        Matrix4f rotationTransform = rotation.getMatrix();
		rotationTransform = rotationTransform.getTranspose();

        Matrix4f scaleTransform ;
        scaleTransform.setScale(scale);       

        Matrix4f keyTransform(positionTransform);

		keyTransform.multiply(rotationTransform);
        keyTransform.multiply(scaleTransform);

		Node* node = new Node(0,0);
		node->setTransform(keyTransform);
		return node;

    }

    /***************************************************************************
     * Builds a pivot group that will allow the objects
     * to be positioned properly according to their rotations
     * and positions.
     * @param coordinateTransform the coordinate system defining the
     * location and orientation of the local axis. This is not modified.
     * @param pivot the pivot defined in the 3ds file loaded by pivot chunk.
     * This is not changed.
     **************************************************************************/
    Node* KeyFramer::buildPivotGroup(Matrix4f& coordinateTransform, Vector3& pivot)
    {
        Matrix4f pivotTransform ;
        Matrix4f pivotTransform1;
        Matrix4f coordinateTransformInv(coordinateTransform);
		coordinateTransformInv = coordinateTransformInv.getInverse();

		pivotTransform.multiply(coordinateTransformInv);
        pivotTransform1.multiply(coordinateTransformInv);

        //if (pivot != null)
        {
            Matrix4f pivot3d ;
            Matrix4f pivot3d1 ;
			Vector3 pivotneg ;
			pivot.scale(-1.0f);
            pivotneg.set(pivot);

            //if (pivotCenter != null)
            {
				pivot.sub(pivotCenter);
            }
            pivot3d.setTranslation(pivot);
            //pivotneg.negate();
            //pivot3d1.set(pivotneg);
			pivotTransform.multiply( pivot3d , pivotTransform);
        }

        //bboxT3d.mul(pivotTransform1);
        //globalbboxT3d.mul(pivotTransform);
		MeshNode* node = new MeshNode(0,chopperRef->getSceneManager());
		//CMeshSceneNode* node = new CMeshSceneNode(0, 0, chopperRef->getSceneManager(),0,
		//					vector3df(0,0,0),vector3df(0,0,0),vector3df(1.0f, 1.0f, 1.0f));
		node->setTransform( pivotTransform);
		//Appearance *app = new Appearance();
		//app->getPolygonAttributes().cullFace = ECF_BACK;
		//app->getMaterial().diffuse.set(1.0f,1.0f,0.0f,1.0f);
		//node->setAppearance(app);

		return node;

    }
    
   
	Node* KeyFramer::addGroups(Node* parentGroup, vector<Node*>& groups)
    {
        Node* nextGroup = parentGroup;
        for (unsigned i = 0; i < groups.size(); i++)
        {
            nextGroup->addChild(groups[i]);
            nextGroup = groups[i];
        }
        return groups[groups.size() - 1];
    }
  
    
   
	RotPosScalePathAnimator* KeyFramer::buildInterpolator(Node* targetGroup, Matrix4f& axisOfTransform, string& meshname)
    {
        
        interpolatePositions();

        interpolateScales();

        interpolateRotations();

		int maxKeys = numPositionKeys > numRotationKeys ? numPositionKeys : numRotationKeys;
		
		maxKeys = maxKeys > numScaleKeys ? maxKeys : numScaleKeys ;

        int numKeys = positions.size();

       
        RotPosScalePathAnimator* tcbrotator = 0;
        

        if (maxKeys > 1)
        {
            float* knots = new float[numKeys];
            TCBKeyFrame* tcbkeyframes;
			tcbkeyframes = new TCBKeyFrame[numKeys];
      
            for (int i = 0; i < numKeys; i++)
            {
                //Knots need to be between 0(beginning) and 1(end)
                knots[i] = (i == 0 ? 0 : ((float) i / ((float) (numKeys - 1))));

                TCBKeyFrame* frame = new TCBKeyFrame(knots[i], *positions[i], *rotationKeys[i], *scales[i] , 0, 0, 0);
				tcbkeyframes[i] = *frame;

            }
 
            //This gives a continuous loop at a rate of 30 fps
            //Alpha alpha = new Alpha(-1, (long) (1.5 * numKeys / .008));

            //alpha.setStartTime(System.currentTimeMillis());
            //alpha.setDuplicateOnCloneTree(true);
            tcbrotator = new RotPosScalePathAnimator( targetGroup, axisOfTransform , tcbkeyframes , numKeys);

			return tcbrotator;

        }
        return 0;
    }

     
   

    void KeyFramer::addFather(int fatherID, Node* newFather)
    {
        Node* keygroup = 0;
        if ( newFather == 0 )
			return;

		if (fatherID < 0)
        {
            Node* group = newFather->getParent();
            if (group != 0)
            {
                group->removeChild(newFather);
            }
            chopperRef->getSceneGroup()->addChild(newFather);
            father = 0;
        } 
		else
        {
			if ( fatherMap.find(fatherID) != fatherMap.end() )
				father = fatherMap.find(fatherID)->second;

			Node* tempgroup = 0;

			if ( lastGroupMap.find(father) !=  lastGroupMap.end() )
				 tempgroup = lastGroupMap.find(father)->second;

            if (tempgroup != 0)
            {
                keygroup = tempgroup->getParent();
            }
        }
        //fatherMap.insert(pair<int,Node*>(id, newFather));
        fatherMap[id] = newFather;
    }

   
    void KeyFramer::globalBboxCalculation() 
	{
        /*if (!meshName.equals("$$$DUMMY")) {
            points = (Point3f[])chopperRef.getMeshVertices(meshName);

            Object3D object3d = null;
            if (chopperRef.getShape3DList().get(meshName) instanceof Vector) {
                object3d = (Object3D) ((Vector) chopperRef.getShape3DList().get(meshName)).get(0);
            } else {
                object3d = (Object3D) chopperRef.getShape3DList().get(meshName);
            }
            if ( object3d == null) return;
            if ( object3d.isDummy() ) return;

            if (points != null) {
                for (int i = 0; i < points.length; i++) {

                    globalbboxT3d.transform(points[i], vertex);

                    if (meshBboxMin.x > vertex.x) {
                        meshBboxMin.x = vertex.x;
                    }
                    if (meshBboxMin.y > vertex.y) {
                        meshBboxMin.y = vertex.y;
                    }
                    if (meshBboxMin.z > vertex.z) {
                        meshBboxMin.z = vertex.z;
                    }

                    if (meshBboxMax.x < vertex.x) {
                        meshBboxMax.x = vertex.x;
                    }
                    if (meshBboxMax.y < vertex.y) {
                        meshBboxMax.y = vertex.y;
                    }
                    if (meshBboxMax.z < vertex.z) {
                        meshBboxMax.z = vertex.z;
                    }

                    points[i] = null;
                }
                points = null;
                if (modelBboxMin.x > meshBboxMin.x) {
                    modelBboxMin.x = meshBboxMin.x;
                }
                if (modelBboxMin.y > meshBboxMin.y) {
                    modelBboxMin.y = meshBboxMin.y;
                }
                if (modelBboxMin.z > meshBboxMin.z) {
                    modelBboxMin.z = meshBboxMin.z;
                }
                if (modelBboxMax.x < meshBboxMax.x) {
                    modelBboxMax.x = meshBboxMax.x;
                }
                if (modelBboxMax.y < meshBboxMax.y) {
                    modelBboxMax.y = meshBboxMax.y;
                }
                if (modelBboxMax.z < meshBboxMax.z) {
                    modelBboxMax.z = meshBboxMax.z;
                }
                Bbox.setLower(meshBboxMin.x, meshBboxMin.y, meshBboxMin.z);
                Bbox.setUpper(meshBboxMax.x, meshBboxMax.y, meshBboxMax.z);

                if (object3d != null) {
                    object3d.setBounds(Bbox);                
                }

                Bbox.setLower(modelBboxMin.x, modelBboxMin.y, modelBboxMin.z);
                Bbox.setUpper(modelBboxMax.x, modelBboxMax.y, modelBboxMax.z);
                chopperRef.setModelBbox(Bbox);
            }
        }*/
    }



    // JUN03 MEM LEAK FIX
    void KeyFramer::finalize() 
	{
        // TODO Auto-generated method stub
        /*lastGroupMap.clear();
        lastGroupMap = null;
        fatherMap.clear();
        fatherMap = null;
        super.finalize();*/
    }


	void KeyFramer::setCoordinateSystem(string objectName, Matrix4f* coordinateSystem)
    {
		//chopperRef->namedObjectCoordinateSystems.insert( pair<string,Matrix4f*>( objectName ,coordinateSystem ) );
   		chopperRef->namedObjectCoordinateSystems[objectName] = coordinateSystem;
	}





