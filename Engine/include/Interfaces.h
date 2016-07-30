#ifndef _INTERFACES_H

#define _INTERFACES_H


#include "coreMath.h"

typedef struct lightData
{
 enum LIGHT_TYPE { POINT_LIGHT = 0 , DIRECTIONAL_LIGHT , SPOT_LIGHT }; 
 Color3 ambient;
 Color3 diffuse;
 Color3 specular;
 float shininess;
}LightData;

typedef struct PointLightData:LightData
{
 Point3 position;
 Point3 attenuation;
}PointLightData;

typedef struct SpotLightData:PointLightData
{
 Vector3 direction;
 float spotAngle;
}SpotLightData;

typedef struct DirectionalLightData:LightData
{
 Vector3 direction;
}DirectionalLightData;

typedef struct meshBuffer
{
 float* coords;
 float* colors;
 float* texcoords;
 float* normals;
 unsigned int* indices;
}MeshBuffer;

#endif