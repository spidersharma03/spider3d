#ifndef _CHUNK_TYPES_H

#define _CHUNK_TYPES_H


enum CHUNK_TYPE
	{
		/**Color chunks*/
		COLOR = (short)0x0010  ,  // Percent chunks
		/**Color chunks*/
		PERCENTAGE_FLOAT = (short)0x0030 , 
		/**Color chunks*/
		PERCENTAGE_INT = (short)0x0031 ,    
		//--> Mesh Version
		/**Mesh Version*/
		VERSION = (short)0x2 ,
		//--> 3D Editior chunk
		/**3D Editior chunk*/
		EDITOR = (short)0x3D3D ,
		//--> Master Scale chunk
		/**Master Scale chunk*/
		SCALE = (short)0x100 ,
		//--> Name Object block chunk
		/**Name Object block chunk*/
		NAMED_OBJECT = (short)0x4000 ,
		//--> Mesh visibility chunk for MDD format
		/**Mesh visibility chunk for MDD format*/
		NAME_OBJ_VISIBILITY = (short)0x4013 ,
		//--> Mesh polygon mode for MDD format
		/**Mesh polygon mode for MDD format*/
		NAMED_OBJECT_POLY_MODE =  (short)0x4012 ,
		//--> Triangular mesh chunk
		/**Triangular mesh chunk*/
		MESH =  (short)0x4100 ,
		//--> 3D vertices chunk
		/**3D vertices chunk*/
		VERTEX_LIST = (short)0x4110 ,
		//--> Face Index
		/**Face Index*/
		FACES_DESCRIPTION = (short)0x4120 ,
		//--> Material Name and Material face indices chunk
		/**Material Name and Material face indices chunk*/
		FACES_MATERIAL = (short)0x4130 ,
		//--> Texture coordinate chunk
		/**Texture coordinate chunk*/
		TEXTURE_COORDINATES = (short)0x4140 ,
		//--> Smooth Chunk
		/**Smooth Chunk*/
		SMOOTH = (short)0x4150 ,
		//--> Local Coordinate chunk
		/**Local Coordinate chunk*/
		COORDINATE_AXES = (short)0x4160 ,
		//--> Camera chunk
		/**Camera chunk*/
		CAMERA = (short)0x4700 ,
		//--> Light Block chunk
		/**Light Block chunk*/
		LIGHT = (short)0x4600 ,
		//--> SpotLight chunk
		/**SpotLight chunk*/
		SPOTLIGHT = (short)0x4610 ,
		//--> Light Range Start
		/**Light Range Start*/
		RANGE_START = (short)0x4659 ,
		//--> Light Range End
		/**Light Range End*/
		RANGE_END = (short)0x465A ,
		//--> Light Attenuation
		/**Light Attenuation*/
		ATTENUATED = (short)0x4625 ,
		//--> Light Multiplier
		/**Light Multiplier*/
		MULTIPLIER = (short)0x465B ,
		//--> Material Block chunk
		/**Material Block chunk*/
		MATERIAL = (short)0xAFFF ,
		//--> Material Name chunk
		/**Material Name chunk*/
		MATERIAL_NAME = (short)0xA000 ,
		//--> Ambient Color chunk
		/**Ambient Color chunk*/
		AMBIENT_COLOR =  (short)0xA010 ,
		//--> Diffuse color chunk
		/**Diffuse color chunk*/
		DIFFUSE_COLOR = (short)0xA020 ,
		//--> Specular color chunk
		/**Specular color chunk*/
		SPECULAR_COLOR = (short)0xA030 ,
		//--> Shininess chunk
		/**Shininess chunk*/
		SHININESS = (short)0xA040 ,
		//--> Shininess chunk
		/**Shininess chunk*/
		SHININESS_STRENGTH = (short)0xA041 ,
		//--> Transparency chunk
		/**Transparency chunk*/
		TRANSPARENCY =  (short)0xA050 ,
		//--> Two sided chunk
		/**Two sided chunk*/
		TWO_SIDED =  (short)0xA081 ,
			//--> Self Illum chunk
			/**Self Illum chunk*/
			SELF_ILLUMINATED =  (short)0xA084 ,
			//--> Texture chunk
			/**Texture chunk*/
			TEXTURE =  (short)0xA200 ,
			//--> Texture Name chunk
			/**Texture Name chunk*/
			TEXTURE_NAME =  (short)0xA300 ,
			//--> Keyframer Block chunk
			/**Keyframer Block chunk*/
			KEYFRAMER =  (short)0xB000 ,
			//--> Ambient light Info chunk
			/**Ambient light Info chunk*/
			AMBIENT_LIGHT_INFO =  (short)0x0B001 ,
			//--> Mesh Info Chunk
			/**Mesh Info Chunk*/
			MESH_INFO =  (short)0x0B002 ,
			//--> Camera info chunk
			/**Camera info chunk*/
			CAMERA_INFO =  (short)0x0B003 ,
			//--> Camera target info
			/**Camera target info*/
			CAMERA_TARGET_INFO =  (short)0x0B004 ,
			//--> Omni light info
			/**Omni light info*/
			OMNI_LIGHT_INFO =  (short)0x0B005 ,
			//--> Spotlight target info
			/**Spotlight target info*/
			SPOT_LIGHT_TARGET_INFO =  (short)0x0B006 ,
			//--> Spotlight info chunk
			/**Spotlight info chunk*/
			SPOT_LIGHT_INFO =  (short)0x0B007 ,
			//--> Frame Chunk
			/**Frame Chunk*/
			FRAMES_CHUNK =  (short)0x0B008 ,
			//--> NamedObject Chunk
			/**NamedObject Chunk*/
			NAME_AND_FLAGS =  (short)0xB010 ,
			//--> Object dummy chunk
			/**Object dummy chunk*/
			OBJECT_DUMMY_NAME =  (short)0xB011 ,
			//--> Pivot chunk
			/**Pivot chunk*/
			PIVOT =  (short)0xB013 ,
			//--> Position chunk
			/**Position chunk*/
			POSITION =  (short)0xB020 ,
			//--> Scale chunk
			/**Scale chunk*/
			SCALE_TRACK =  (short)0xB022 ,
			//--> Rotation chunk
			/**Rotation chunk*/
			ROTATION =  (short)0xB021 ,
			//--> Bounding Box chunk
			/**Bounding Box chunk*/
			BOUNDING_BOX =  (short)0x0B014 ,
			//--> Hierarchy info
			/**Hierarchy info*/
			HIERARCHY_INFO =  (short)0xB030 ,
			//--> Keyframe time chunk
			/**Keyframe time chunk*/
			KFTIME =  (short)0xB009 ,
			//--> KeyFrame Version chunk
			/**KeyFrame Version chunk*/
			KFVERSION =  (short)0xB00A ,
			//--> Hopspot data chunk for Mdd format
			/**Hopspot data chunk for Mdd format*/
			HOTSPOT =  (short)0xB100 

	};




#endif