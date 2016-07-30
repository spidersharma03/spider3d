#ifndef _TCB_KEYFRAME_H

#define _TCB_KEYFRAME_H

#include "Chunk.h"
#include "coreMath.h"
#include "Quaternion.h"

using namespace spider3d::core;

struct TCBKeyFrame
{
 float tension , continuity , bias;

 float knot;

 Vector3 position , scale;

 Quaternion quat;

 TCBKeyFrame():knot(0), position(0,0,0), quat(0,0,0,1),
	   scale(0,0,0) , tension(0) , continuity(0) , bias(0)
 {
 };

 TCBKeyFrame(float k,  Vector3& pos, Quaternion& q, Vector3& s, float t, float c, float b):knot(k), position(pos), quat(q),
	   scale(s) , tension(t) , continuity(c) , bias(b)
 {
 };

 TCBKeyFrame& operator= (TCBKeyFrame& other)
 {
	 knot = other.knot;
	 position = other.position;
	 quat = other.quat;
	 scale = other.scale;
	 tension = other.tension;
	 continuity = other.continuity;
	 bias = other.bias;
	 return *this;
 }

 TCBKeyFrame(const TCBKeyFrame& other)
 {
	 knot = other.knot;
	 position = other.position;
	 quat = other.quat;
	 scale = other.scale;
	 tension = other.tension;
	 continuity = other.continuity;
	 bias = other.bias;
 }

};

#endif