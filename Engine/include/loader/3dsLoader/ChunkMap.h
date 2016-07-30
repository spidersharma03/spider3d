#ifndef _CHUNK_MAP_H

#define _CHUNK_MAP_H

#include "Chunk.h"
//#include "FramesDescriptionChunk.h"
#include "Chunks_3DS.h"

#include "Chunk_Types.h"
/*******************************************************************************
* A Hashmap with the chunk names as values with keys
* being the chunk id.
******************************************************************************/


	

class ChunkMap 
{

	// Color chunks
public:
	/***************************************************************************
	* singleton constructor.
	* @param mainChunk Header Chunk Of file
	**************************************************************************/
	ChunkMap(Chunk* mainChunk)
	{
		this->mainChunk = mainChunk;
		initializeDataMap();
	}

	
private:

	Chunk* mainChunk  ;

	Chunk* keyFramerChunk  ;
	Chunk* editorChunk  ;
	Chunk* triangularMeshChunk  ;
	Chunk* facesDescriptionChunk  ;
	Chunk* framesDescriptionChunk  ;
	Chunk* textureChunk  ;
	Chunk* lightChunk  ;
	Chunk* namedObjectChunk  ;
	Chunk* polygonModeChunk  ;
	Chunk* nameObjVisibiltyChunk  ;
	Chunk* materialChunk  ;
	Chunk* keyFramerInfoChunk  ;
	Chunk* cameraTargetInfoChunk  ;
	Chunk* cameraInfoChunk  ;
	Chunk* spotLightTagetInfo  ;
	Chunk* omniLightInfoChunk  ;
	Chunk* spotLightInfoChunk  ;
	Chunk* spotLightChunk  ;
	Chunk* kfVersionChunk  ;
	Chunk* floatChunk  ;
	Chunk* framesChunk  ;
	Chunk* pivotChunk  ;
	Chunk* objectDummyNameChunk  ;
	Chunk* targetDescriptionChunk  ;
	Chunk* positionChunk  ;
	Chunk* rotationChunk  ;
	Chunk* scaleChunk  ;
	Chunk* hierarchyInfoChunk  ;
	Chunk* boundingBoxChunk  ;
	Chunk* stringChunk  ;
	Chunk* globalColorChunk  ;
	Chunk* booleanChunk  ;
	Chunk* percentageChunk  ;
	Chunk* cameraChunk  ;
	Chunk* colorChunk  ;
	Chunk* vertex3ListChunk  ;
	Chunk* vertex2ListChunk  ;
	Chunk* axisChunk  ;
	Chunk* facesMaterialChunk  ;
	Chunk* smoothingChunk  ;
	Chunk* hotSpotChunk ;

	
	/***************************************************************************
	* Called when debugging is turned on. The keys are cast to short so that
	* they are improperly signed since java will be reading improperly signed
	* ids out of the file.
	**************************************************************************/

	void initializeDataMap() {
		keyFramerChunk          = new Chunk("KeyFramerChunk");
		editorChunk             = new Chunk("EditorChunk");
		triangularMeshChunk     = new Chunk("TriangularMeshChunk");
		facesDescriptionChunk   = new FacesDescriptionChunk();
		framesDescriptionChunk  = new FramesDescriptionChunk();
		textureChunk            = new TextureChunk();
		lightChunk              = new LightChunk();
		namedObjectChunk        = new NamedObjectChunk();
		polygonModeChunk        = new PolygonModeChunk();
		nameObjVisibiltyChunk   = new NameObjVisibilityChunk();
		materialChunk           = new MaterialChunk();
		keyFramerInfoChunk      = new KeyFramerInfoChunk();
		cameraTargetInfoChunk   = new CameraTargetInfoChunk();
		cameraInfoChunk         = new CameraInfoChunk();
		spotLightTagetInfo      = new SpotLightTargetInfoChunk();
		omniLightInfoChunk      = new OmniLightInfoChunk();
		spotLightInfoChunk      = new SpotLightInfoChunk();
		spotLightChunk          = new SpotLightChunk();
		kfVersionChunk          = new KFVersionChunk();
		floatChunk              = new FloatChunk();
		framesChunk             = new FramesChunk();
		pivotChunk              = new PivotChunk();
		objectDummyNameChunk    = new ObjectDummyNameChunk();
		targetDescriptionChunk  = new TargetDescriptionChunk();
		positionChunk           = new PositionChunk();
		rotationChunk           = new RotationChunk();
		scaleChunk              = new ScaleChunk();
		hierarchyInfoChunk      = new HierarchyInfoChunk();
		boundingBoxChunk        = new BoundingBoxChunk();
		stringChunk             = new StringChunk();
		globalColorChunk        = new GlobalColorChunk();
		booleanChunk            = new BooleanChunk();
		percentageChunk         = new PercentageChunk();
		cameraChunk             = new CameraChunk();
		colorChunk              = new ColorChunk();
		vertex3ListChunk        = new Vertex3ListChunk();
		vertex2ListChunk        = new Vertex2ListChunk();
		axisChunk               = new AxisChunk();
		facesMaterialChunk      = new FacesMaterialChunk();
		smoothingChunk          = new SmoothingChunk();
		hotSpotChunk            = new HotSpotChunk();

		mainChunk->addSubChunk(VERSION, floatChunk);
		mainChunk->addSubChunk(EDITOR, editorChunk);
		mainChunk->addSubChunk(KEYFRAMER, keyFramerChunk);
		mainChunk->addSubChunk(HOTSPOT, hotSpotChunk);

		editorChunk->addSubChunk(MATERIAL, materialChunk);
		editorChunk->addSubChunk(SCALE, floatChunk);
		editorChunk->addSubChunk(NAMED_OBJECT, namedObjectChunk);

		keyFramerChunk->addSubChunk(FRAMES_CHUNK, framesChunk);
		keyFramerChunk->addSubChunk(MESH_INFO, keyFramerInfoChunk);

		keyFramerChunk->addSubChunk(AMBIENT_LIGHT_INFO, keyFramerInfoChunk);
		keyFramerChunk->addSubChunk(CAMERA_INFO, cameraInfoChunk);
		keyFramerChunk->addSubChunk(CAMERA_TARGET_INFO, cameraTargetInfoChunk);
		keyFramerChunk->addSubChunk(OMNI_LIGHT_INFO, omniLightInfoChunk);
		keyFramerChunk->addSubChunk(SPOT_LIGHT_TARGET_INFO, spotLightTagetInfo);
		keyFramerChunk->addSubChunk(SPOT_LIGHT_INFO, spotLightInfoChunk);
		keyFramerChunk->addSubChunk(KFTIME, floatChunk);
		keyFramerChunk->addSubChunk(KFVERSION, kfVersionChunk);

		omniLightInfoChunk->addSubChunk(NAME_AND_FLAGS, framesDescriptionChunk);
		omniLightInfoChunk->addSubChunk(HIERARCHY_INFO, hierarchyInfoChunk);
		omniLightInfoChunk->addSubChunk(POSITION, positionChunk);

		spotLightInfoChunk->addSubChunk(NAME_AND_FLAGS, framesDescriptionChunk);
		spotLightInfoChunk->addSubChunk(HIERARCHY_INFO, hierarchyInfoChunk);
		spotLightInfoChunk->addSubChunk(POSITION, positionChunk);

		cameraInfoChunk->addSubChunk(NAME_AND_FLAGS, framesDescriptionChunk);
		cameraInfoChunk->addSubChunk(HIERARCHY_INFO, hierarchyInfoChunk);
		cameraInfoChunk->addSubChunk(POSITION, positionChunk);

		keyFramerInfoChunk->addSubChunk(NAME_AND_FLAGS, framesDescriptionChunk);
		keyFramerInfoChunk->addSubChunk(PIVOT, pivotChunk);
		keyFramerInfoChunk->addSubChunk(OBJECT_DUMMY_NAME, objectDummyNameChunk);
		keyFramerInfoChunk->addSubChunk(POSITION, positionChunk);
		keyFramerInfoChunk->addSubChunk(ROTATION, rotationChunk);
		keyFramerInfoChunk->addSubChunk(SCALE_TRACK, scaleChunk);
		keyFramerInfoChunk->addSubChunk(HIERARCHY_INFO, hierarchyInfoChunk);
		keyFramerInfoChunk->addSubChunk(BOUNDING_BOX, boundingBoxChunk);

		cameraTargetInfoChunk->addSubChunk(NAME_AND_FLAGS, targetDescriptionChunk);
		cameraTargetInfoChunk->addSubChunk(HIERARCHY_INFO, hierarchyInfoChunk);
		cameraTargetInfoChunk->addSubChunk(POSITION, positionChunk);

		spotLightTagetInfo->addSubChunk(NAME_AND_FLAGS, targetDescriptionChunk);
		spotLightTagetInfo->addSubChunk(POSITION, positionChunk);
		spotLightTagetInfo->addSubChunk(HIERARCHY_INFO, hierarchyInfoChunk);

		materialChunk->addSubChunk(MATERIAL_NAME, stringChunk);
		materialChunk->addSubChunk(AMBIENT_COLOR, globalColorChunk);
		materialChunk->addSubChunk(DIFFUSE_COLOR, globalColorChunk);
		materialChunk->addSubChunk(SPECULAR_COLOR, globalColorChunk);
		materialChunk->addSubChunk((short)0xA200, textureChunk);

		materialChunk->addSubChunk(TWO_SIDED, booleanChunk);
		materialChunk->addSubChunk(SHININESS, percentageChunk);
		materialChunk->addSubChunk(SHININESS_STRENGTH, percentageChunk);
		materialChunk->addSubChunk(TRANSPARENCY, percentageChunk);
		materialChunk->addSubChunk(SELF_ILLUMINATED, percentageChunk);

		namedObjectChunk->addSubChunk(MESH, triangularMeshChunk);
		namedObjectChunk->addSubChunk(CAMERA, cameraChunk);
		namedObjectChunk->addSubChunk(LIGHT, lightChunk);
		namedObjectChunk->addSubChunk(NAMED_OBJECT_POLY_MODE, polygonModeChunk);
		namedObjectChunk->addSubChunk(NAME_OBJ_VISIBILITY, nameObjVisibiltyChunk);

		lightChunk->addSubChunk(RANGE_START, floatChunk);
		lightChunk->addSubChunk(COLOR, colorChunk);
		lightChunk->addSubChunk(RANGE_END, floatChunk);
		lightChunk->addSubChunk(MULTIPLIER, floatChunk);
		lightChunk->addSubChunk((short)0x4610, spotLightChunk);

		textureChunk->addSubChunk(TEXTURE_NAME, stringChunk);

		triangularMeshChunk->addSubChunk(VERTEX_LIST, vertex3ListChunk);
		triangularMeshChunk->addSubChunk(TEXTURE_COORDINATES, vertex2ListChunk);
		triangularMeshChunk->addSubChunk(FACES_DESCRIPTION, facesDescriptionChunk);
		triangularMeshChunk->addSubChunk(COORDINATE_AXES, axisChunk);

		facesDescriptionChunk->addSubChunk(FACES_MATERIAL, facesMaterialChunk);
		facesDescriptionChunk->addSubChunk(SMOOTH, smoothingChunk);
	}


};



#endif